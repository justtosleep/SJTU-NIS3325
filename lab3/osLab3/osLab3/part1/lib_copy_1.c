// 文件的库函数：fread(buf, size, nitems, fp), fwrite(buf, size, nitems, fp)

#include<stdio.h>

// 运行时间
#include <time.h>

# define nitems 1
# define size 1

int main()
{
    clock_t start, finish;
    double duration;
    start = clock();

	// 逐个字节读写
	char buf[size];

    // 文件实际读写的数据对象个数
    int r = 0, w = 0;

	// 打开要复制的源文件
	FILE* fpres = fopen("../test_files/resourse.txt", "r");
	if (fpres == NULL)
	{		
		// resource.txt文件打开失败，输出错误信息并退出
		perror("resource.txt");
		exit(1);
	}

	// 打开要保存的目标文件
	FILE* fpdes = fopen("../test_files/destination.txt", "w");
	if (fpdes == NULL)
	{
		// rdestination.txt文件打开失败，输出错误信息并退出
		perror("destination.txt");
		exit(1);
	}
 
    // 从源文件中逐字节读取数据并写入目标文件中，直到文件结束
	while ((r = fread(buf, size, nitems, fpres)) > 0)
    {
		w = fwrite(buf, r, nitems,fpdes);
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
	printf("lib_copy success.\n");
	fclose(fpres);
	fclose(fpdes);

    finish = clock();
    duration = (double)(finish - start) / CLOCKS_PER_SEC;
    printf("运行时间：%f seconds\n", duration);
 
	return 0;
}