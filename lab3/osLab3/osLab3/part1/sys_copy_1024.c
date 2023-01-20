// 文件的系统调用：read(fd, buf, nbytes), write(fd, buf, nbytes)

#include<stdio.h>

// open函数打开需要的头文件
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

// 运行时间
#include <time.h>

# define nbytes 1024

int main()
{
    clock_t start, finish;
    double duration;
    start = clock();

	// 1024字节读写
	char buf[1024];

    // 文件实际读写的字符数量
    int r = 0, w = 0;

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
 
    // 从源文件中逐字节读取数据并写入目标文件中，直到文件结束
	while ((r = read(fpres, buf, nbytes)) > 0)
    {
		w = write(fpdes, buf, r);
        if(w == -1 && w != r)
        {
            // 写入过程出错
            perror("write");
		    exit(3);
        }
    }

    if (r == -1)
    {
        // 读入过程出错
        perror("read");
        exit(2);
    }
    
    // 复制完毕，关闭文件
	printf("sys_copy success.\n");
	close(fpres);
	close(fpdes);

    finish = clock();
    duration = (double)(finish - start) / CLOCKS_PER_SEC;
    printf("运行时间：%f seconds\n", duration);
 
	return 0;
}