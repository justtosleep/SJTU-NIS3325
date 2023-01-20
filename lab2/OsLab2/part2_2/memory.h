#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#define SHMKEY 18001  /* 共享内存关键字 */
#define SIZE 1024     /* 共享内存长度 */
#define SEMKEY1 19001 /* 信号灯组1关键字 */
#define SEMKEY2 19002 /* 信号灯组2关键字 */

static void semcall(int sid, int op);

int creatsem(key_t key)
{
    int sid;
    union semun
    { /* 如sys/sem.h中未定义 */
        int val;
        struct semid_ds *buf;
        ushort *array;
    } arg;
    if ((sid = semget(key, 1, 0666 | IPC_CREAT)) == -1)
        syserr("semget");
    arg.val = 1;
    if (semctl(sid, 0, SETVAL, arg) == -1)
        syserr("semctl");
    return (sid);
}

void clearsem(int sid)
{
    if (semctl(sid, 0, IPC_RMID) == -1)
        syserr("semctl");
}

void P(int sid)
{
    semcall(sid, -1);
}

void V(int sid)
{
    semcall(sid, 1);
}

static void semcall(int sid, int op)
{
    struct sembuf sb;
    sb.sem_num = 0;
    sb.sem_op = op;
    sb.sem_flg = SEM_UNDO; 
    if (semop(sid, &sb, 1) == -1)
        syserr("semop");
}

void clearshm(int sid, char *addr)
{        
    if (!shmdt(addr))
        syserr("shmdt");
    if (shmctl(sid, IPC_RMID, 0) == -1)
        syserr("shmctl");
}

void syserr(const char *errstr)
{
    fprintf(stderr, errstr);
    exit(1);
}