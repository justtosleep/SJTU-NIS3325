#include "sockcom.h"

int main()
{
    // 逐个字节读写
    char buf[nbytes];

    // 文件实际读写的字符数量
    int count, w = 0;
    
    int sockfd, newsockfd;
    unsigned int length;
    struct sockaddr_in server;
    char buf[nbytes];

    /* 生成插座 */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd <0)
        syserr("socket");

    /* 构成socked名（地址）和建立联系 */
    server.sin_family = AF_INET;                
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    if (bind(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0)
        syserr("bind stream socket");

    /* 获取并打印端口号 */
    length = sizeof(server);
    if (getsockname(sockfd, (struct sockaddr *)&server, &length) < 0)
        fatal("getting socket name");
    printf("socket port #%d\n", ntohs(server.sin_port));    /*  ntohs:将一个16位数由网络字节顺序转换为主机字节顺序 */
    listen(sockfd, 5);

    while (newsockfd = accept(sockfd, (struct sockaddr *)0, (unsigned int *)0))
    {
        /* 子进程 */
        if (!fork())                          
        {
            // 打开要保存的目标文件
            int fpdes = open("./test_files/destination.txt", O_WRONLY);
            // int fpdes = open("./test_files/destination.in", O_WRONLY);
            if (fpdes == -1)
                // destination.txt文件打开失败，输出错误信息并退出
                syserr("destination.txt");
            
            while (1) 
            {
                count = recv(newsockfd, buf, sizeof(buf), 0);
                if (count < 0)
                    syserr("receive file failed");
                else 
                {
                    w = write(fpdes, buf, count);
                    if (w == -1 && w != count)
                        syserr("write");
                    printf("receivebytes: %d\n", w);
                    if (count < nbytes)
                        break;
                }
                /* 调用库函数，清缓冲区 */
                bzero(buf, sizeof(buf));            
            }
            printf("Success\n");
            close(fpdes);  
        }
        close(newsockfd);
    }
    close(sockfd);
}