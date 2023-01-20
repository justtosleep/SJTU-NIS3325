#include "msgcom.h"
int main()
{
    struct msgtype buf;
    int qid;
    if ((qid = msgget(MSGKEY, IPC_CREAT | 0666)) == -1)
        return (-1);
    while (1)
    {
        msgrcv(qid, &buf, 1024, 1, MSG_NOERROR);
        printf("Server receive a text\n");
        FILE * fp = fopen("destination.txt", "w+");
        if (fp == NULL) 
        {
            printf("open error! \n");
            exit(1); 
        }
        fputs(buf.text, fp);
        fclose(fp);        

        printf("Server write it in new destination.txt\n");
        buf.mtype = 2;
        strcpy(buf.text, "destination.txt");
        msgsnd(qid, &buf, sizeof(buf.text), 0);
    }
}