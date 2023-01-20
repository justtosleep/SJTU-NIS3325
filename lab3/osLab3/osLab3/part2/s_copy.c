// 行的输入与输出：fgets和fputs（仅限于行结构的文本文件）

#include <stdio.h>
 
int main()
{
    // 读写缓冲区，每次最多16个字符
	char buf[17];

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

    // 从源文件中逐16字符读取数据并写入目标文件中，直到文件结束
	while(fgets(buf,16,fpres)!=NULL)
	{		
		fputs(buf,fpdes);
	}

    // 复制完毕，关闭流文件
	printf("s_copy success.\n");
	fclose(fpres);
	fclose(fpdes);

	return 0;
} 