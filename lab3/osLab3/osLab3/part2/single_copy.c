// 流的单字符I/O操作：fgetc和fputc

#include<stdio.h>

int main()
{
	// 逐个字符读写
	char ch;

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
		// destination.txt文件打开失败，输出错误信息并退出
		perror("destination.txt");
		exit(1);
	}
 
    // 从源文件中逐字符读取数据并写入目标文件中，直到遇到文件结束符EOF
	while ((ch=fgetc(fpres))!=EOF)
		fputc(ch,fpdes);
	
    // 复制完毕，关闭流文件
	printf("single_copy success.\n");
	fclose(fpres);
	fclose(fpdes);
 
	return 0;
}