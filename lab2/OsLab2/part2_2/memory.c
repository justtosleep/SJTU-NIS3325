#include "memory.h"

extern int creatsem();
extern void P(), V(), syserr(), clearshm(), clearsem();

main()
{
    char *segaddr;
    int segid, sid1, sid2;
    /* 创建共享内存段 */
    if ((segid = shmget(SHMKEY, SIZE, IPC_CREAT | 0666)) == -1)
        syserr("shmget");
    /* 将共享内存映射到进程数据空间 */
    segaddr = shmat(segid, 0, 0);
    sid1 = creatsem(SEMKEY1); /* 创建两个信号灯, 初值为1 */
    sid2 = creatsem(SEMKEY2);
    P(sid2); /* 置信号灯2值为0, 表示缓冲区空 */

    if (!fork())
    {
        while (1)
        { /* 子进程, 接收和输出 */
            P(sid2);
            if(strncmp(segaddr, "end", 3) == 0)     /* 文件结束，退出循环 */
                break;
            
            printf("Child: Received from Parent and write in destination.txt\n"); /* 成功读取共享内存 */
            FILE *fp2 = fopen("destination.txt", "a");  /* 创建新的，或打开已有的destination.txt，在末尾编辑 */
            if (fp2 == NULL)
                syserr("open error");
            fputs(segaddr, fp2);        /* 写入txt文件 */
            fclose(fp2);
            V(sid1);
        }
        printf("Child finished\n");
        exit(0);
    }
    else
    { /* 父进程, 输入和存储 */
        FILE *fp1 = fopen("source.txt", "r+"); /* 打开source.txt，读取通信内容 */
        if (fp1 == NULL)
            syserr("open error");
        while (!feof(fp1))  /* 读取所有内容直到EOF */
        {
            P(sid1);
            fgets(segaddr, 1024, (FILE *)fp1); /* 将source.txt中的内容逐行读入共享内存 */
            printf("Parent: Read in source.txt and write in shared memory\n");
            V(sid2);
        }
        fclose(fp1);

        P(sid1);
        strcpy(segaddr, "end");
        V(sid2);

        wait(0); /* 等待子进程结束 */
        clearshm(segid, segaddr); /* 分离共享内存 */
        clearsem(sid1); /* 清除所有信号量 */
        clearsem(sid2);
        printf("Parent finished\n");
        exit(0);
    }
}