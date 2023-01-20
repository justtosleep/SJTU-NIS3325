#include "msgcom.h"
main()
{
    struct msgtype buf;
    int qid;
    qid = msgget(MSGKEY, IPC_CREAT | 0666);
    buf.mtype = 1;
    FILE *fp = fopen("source.txt", "r+");
    if (fp == NULL)
    {
        printf("open error! \n");
        exit(1);
    }
    fgets(buf.text, 1024, (FILE *)fp);      /* 将source.txt中的内容读入buf */
    fclose(fp);
    msgsnd(qid, &buf, sizeof(buf.text), 0); /* 将buf发到消息队列 */
    msgrcv(qid, &buf, 1024, 2, MSG_NOERROR);
    printf("Client receive a reply from Server: %s\n", buf.text);
}
