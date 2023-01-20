// 无名管道:pipe(fd[2])

#include<stdio.h>

// open函数打开需要的头文件
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

// fork()
#include <unistd.h>

// wait()
#include <sys/wait.h>

# define nbytes 1

int main()
{
    // 文件实际读写的字符数量
    int r = 0, w = 0;

    // 文件描述字
    int chan[2];
    
    // 打开要复制的源文件
	int fpres = open("../test_files/resourse.txt", O_RDONLY);
	if (fpres == -1)
	{		
		// resource.txt文件打开失败，输出错误信息并退出
		perror("resource.txt");
		exit(1);
	}

	// 打开要保存的目标文件
	int fpdes = open("../test_files/destination.txt", O_WRONLY);
	if (fpdes == -1)
	{
		// destination.txt文件打开失败，输出错误信息并退出
		perror("destination.txt");
		exit(1);
	}

    // 创建管道
    pipe(chan);
    if (fork())
    {
        // 父进程关闭管道读端
        close(chan[0]);

        // 逐个字节读写
	    char buf_p[nbytes];

        // 从源文件中逐字节读取数据并写入管道中，直到文件结束
        while ((r = read(fpres, buf_p, nbytes)) > 0)
            write(chan[1], buf_p, r);

        // 父进程关闭管道写端
        close(chan[1]);

        // 等待子进程结束
        wait(NULL);
    }
    else
    {
        // 子进程关闭管道写端
        close(chan[1]);

        // 逐个字节读写
	    char buf_c[nbytes];

        // 从管道中逐字节读取数据并写入目标文件中，直到文件结束
        while ((w = read(chan[0], buf_c, nbytes)) > 0)
            write(fpdes, buf_c, w);

        // 子进程关闭管道读端
        close(chan[0]);

        // 子进程结束
        exit(0);
    }
	
    // 复制完毕，关闭文件
	printf("pipe_copy success.\n");
	close(fpres);
	close(fpdes);
}
