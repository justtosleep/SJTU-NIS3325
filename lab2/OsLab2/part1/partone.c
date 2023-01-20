#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void func_ls()
{
    printf("It is signal processing ls function.\n");
}

void func_mv()
{
    printf("It is signal processing mv function.\n");
}

main()
{
    int status_mv, status_ls;
    pid_t pid_mv, pid_ls;
    void func();
    signal(SIGUSR1, func_ls); /* 预置ls信号处理程序 */
    signal(SIGUSR2, func_mv); /* 预置mv信号处理程序 */
    FILE *fp = fopen("parent.txt", "w+");
    if (fp == NULL) 
    {
        printf("open error! \n");
        exit(1); 
    }
    printf("Parent: new parent.txt.\n\n");
    fclose(fp);

    if(pid_ls = fork())
    {
        printf("Parent_ls: send signal_ls.\n");
        kill(pid_ls, SIGUSR1); /* 发送ls信号 */
        wait(&status_ls);      /* 等待ls子进程停止 */
        printf("status_ls=%d: Parent_ls finished:\n", status_ls);
    }
    else
    {
        sleep(10); /* 等待接受ls信号 */   
        printf("Child_ls: signal_ls is received.\n");
        execl ("/bin/ls","ls","-l",(char*)0); /* 映像改换 */
        printf("execl error.\n");         /* 映像改换失败 */
        exit(2);
    }

    printf("\n");

    if (pid_mv = fork())
    {
        printf("Parent_mv: send signal_mv.\n");
        kill(pid_mv, SIGUSR2); /* 发送mv信号 */
        wait(&status_mv);      /* 等待子进程停止 */
        printf("status_mv=%d: Parent_mv finished:\n", status_mv);
    }
    else
    {
        sleep(10); /* 等待接受信号 */   
        printf("Child_mv: signal_mv is received.\n");
        execl ("/bin/mv", "mv", "parent.txt", "child.txt", (char*)0); /* 映像改换 */
        printf("execl error.\n");         /* 映像改换失败 */
        exit(2);
    }
    
}
