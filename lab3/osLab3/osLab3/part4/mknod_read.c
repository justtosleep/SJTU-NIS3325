// 有名管道：mknod

#include<stdio.h>

// O_RDONLY的说明
#include <fcntl.h>

// open函数打开需要的头文件
#include <sys/types.h>
#include <sys/stat.h>

# define NAPTIME 5
# define nbytes 1

int main()
{
    // 逐个字节读写
    char buf[nbytes];

    // 文件实际读写的字符数量
    int w = 0;

    // 打开要保存的目标文件
	int fpdes = open("../test_files/destination.txt", O_WRONLY);
	if (fpdes == -1)
	{
		// destination.txt文件打开失败，输出错误信息并退出
		perror("destination.txt");
		exit(1);
	}

    // 以读方式打开管道，可以不等待写打开
    int fd;
    fd = open("fifo", O_RDONLY | O_NDELAY);

    // 从管道中逐字节读取数据并写入目标文件中，直到文件结束
    // 若管道为空，进程睡眠等待
    while ((w = read(fd, buf, nbytes)) != -1)
        if (w == 0)
            sleep(NAPTIME);
        else
            write(fpdes, buf, w);

    // 复制完毕，关闭管道和文件
	printf("mknod_read success.\n");
    close(fpdes);
    close(fd);
}