#include "sockcom.h"

int main() 
{
    // 逐个字节读写
    char buf[nbytes];

    // 文件实际读写的字符数量
    int r = 0;

    int sockfd;
    struct sockaddr_in server;

    /* 生成套接字 */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);       
    if(sockfd < 0)
        syserr("Opening stream socket");
           
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(IP);
    server.sin_port = htons(PORT);
    if(connect(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0)
        syserr("connecting stream socket");

    // 打开要复制的源文件
	int fpres = open("./test_files/resource.txt", O_RDONLY);
	// int fpres = open("./test_files/resource.in", O_RDONLY);

	if (fpres == -1)
		// resource.txt文件打开失败，输出错误信息并退出
        syserr("resource_text.txt");

    // 从源文件中逐字节读取数据，直到文件结束
	while ((r = read(fpres, buf, nbytes)) > 0)
    {
        if (send(sockfd, buf, strlen(buf), 0) < 0)
            syserr("sendint message");
        printf("send bytes: %d\n", r);
		bzero(buf, sizeof(buf));
    }

    if (r == 0)
        printf("EOF... disconnect\n");
    else if (r == -1)
        syserr("send files");

    close(fpres);
    close(sockfd);
    exit(0);
}