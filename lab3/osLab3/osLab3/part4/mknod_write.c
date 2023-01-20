// 有名管道：mknod

#include <stdio.h>

// S_IFIFO的说明
// O_WRONLY 的说明
#include <fcntl.h>

// open函数打开需要的头文件
#include <sys/types.h>
#include <sys/stat.h>

# define nbytes 1

int main()
{
    // 逐个字节读写
    char buf[nbytes];

    // 文件实际读写的字符数量
    int r = 0;

    // 创建有名管道
    mknod("fifo", S_IFIFO | 0666, 0);

    // 以写方式打开管道
    int fd;
    fd = open("fifo", O_WRONLY);

    // 打开要复制的源文件
	int fpres = open("../test_files/resourse.txt", O_RDONLY);
	if (fpres == -1)
	{		
		// resource.txt文件打开失败，输出错误信息并退出
		perror("resource.txt");
		exit(1);
	}

    // 从源文件中逐字节读取数据并写入管道中，直到文件结束
    while ((r = read(fpres, buf, nbytes)) > 0)
        write(fd, buf, r);

    // 复制完毕，关闭管道和文件
	printf("mknod_write success.\n");
    close(fpres);
    close(fd);
}
