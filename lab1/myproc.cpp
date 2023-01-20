#include <iostream>
#include <malloc.h>

#define Msize 1000
using namespace std;

struct map
{
    unsigned int m_size;
    char *m_addr;
    struct map *next, *prior;
};

struct map *nowmap = NULL;

char *lmalloc(unsigned size);
void lfree(unsigned size, char *addr);
void free1(map *freemap, unsigned size);
void free2(map *freemap, unsigned size);
void free3(map *freemap, unsigned size);
void free4(map *freemap, unsigned size, char *addr);

void init_link(char *addr0);
void ml(char *addr0);
void free_link(char *addr0);
void cmd_help();
void show_unuse();
void show_err(int errtype);

char *lmalloc(unsigned size)
{
    // 分配内存
    struct map *newmap = NULL;
    char *a;
    newmap = nowmap;
    while (newmap->m_size < size && newmap->next != nowmap)
        newmap = newmap->next;

    if (newmap->m_size < size)
    {
        // 没有足够分配空间
        show_err(0);
        return NULL;
    }

    a = newmap->m_addr;
    nowmap = newmap->next;
    newmap->m_addr += size;
    if ((newmap->m_size -= size) == 0 && nowmap->next != nowmap)
    {
        // 删除该节点
        nowmap->prior = newmap->prior;
        nowmap->prior->next = nowmap;
        free(newmap);
    }
    return a;
}

void lfree(unsigned size, char *addr)
{
    // 释放内存
    struct map *freemap = NULL;
    char *a;

    a = addr;
    if (nowmap->m_addr < a)
    {
        // addr after nowmap
        for (freemap = nowmap->next; freemap->m_addr < a && freemap != nowmap; freemap = freemap->next)
            ;
        if (freemap == nowmap)
        {
            // 待释放地址在最后
            // 找到最后一个节点,根据情况完成释放
            // printf("end");
            while (freemap->m_addr < freemap->next->m_addr)
                freemap = freemap->next;

            if (freemap->m_addr + freemap->m_size == a)
                // situation 1
                free1(freemap, size);
            else
                // situation 4
                free4(freemap, size, a);
            return;
        }
        else
            freemap = freemap->prior; // 让 freemap 指向待释放地址前一个空闲区
    }
    else
    {
        // addr behind nowmap
        for (freemap = nowmap->prior; freemap->m_addr > a && freemap != nowmap; freemap = freemap->prior)
            ;
        if (freemap == nowmap)
        {
            // 待释放地址在最前
            // 找到第一个节点,根据情况完成释放
            // printf("first");
            while (freemap->m_addr > freemap->prior->m_addr)
                freemap = freemap->prior;
            // printf("%p\n", freemap->m_addr);

            if (a + size == freemap->m_addr)
                // situation 3
                free3(freemap->prior, size);
            else
                // situation 4
                free4(freemap->prior, size, a);
            return;
        }
    }

    // printf("%p\n", freemap->m_addr);
    if (freemap->m_addr + freemap->m_size == a) // situation 1, 2
    {
        // situation 1
        free1(freemap, size);

        if (freemap->m_addr + freemap->m_size == freemap->next->m_addr)
            // situation 2
            free2(freemap, size);
        else
            printf("situation 1: 与前空闲区合并\n");
    }
    else
    {
        if (a + size == freemap->next->m_addr)
            // situation 3
            free3(freemap, size);
        else
            // situation 4
            free4(freemap, size, a);
    }
}

void free1(map *freemap, unsigned size)
{
    freemap->m_size += size;
}

void free2(map *freemap, unsigned size)
{
    printf("situation 2: 与前后空闲区合并，减少节点\n");

    struct map *midmap;
    midmap = freemap->next;
    freemap->m_size += midmap->m_size;
    freemap->next = midmap->next;
    midmap->next->prior = freemap;

    if (nowmap == midmap)
        // nowmap 上移
        nowmap = freemap;

    free(midmap);
}

void free3(map *freemap, unsigned size)
{
    printf("situation 3: 与后空闲区合并\n");

    freemap = freemap->next;
    freemap->m_addr -= size;
    freemap->m_size += size;
}

void free4(map *freemap, unsigned size, char *addr)
{
    printf("situation 4: 与前后空闲区都不相连，增加节点\n");

    struct map *midmap;
    midmap = (map *)malloc(sizeof(map));

    midmap->m_addr = addr;
    midmap->m_size = size;
    midmap->prior = freemap;
    midmap->next = freemap->next;
    midmap->next->prior = midmap;
    freemap->next = midmap;
}

void cmd_help()
{
    // 打印控制指令
    const char *cmd = "\n*********command**********\n"
                      "   结束: q              \n"
                      "   分配空间: m size      \n"
                      "   释放空间: f addr size \n"
                      "   (有效addr: 0~999)    \n"
                      "   (有效size: 1~1000)   \n"
                      "**************************\n";
    printf("%s", cmd);
}

void show_unuse()
{
    // 打印空闲区表格
    struct map *showmap, *firstmap;
    showmap = nowmap;

    // 找到第一个节点
    while (showmap->m_addr > showmap->prior->m_addr)
        showmap = showmap->prior;
    firstmap = showmap;
    printf("\n****************************************\n");
    do
    {
        printf("可用空闲区地址 %p， 大小%u\n", showmap->m_addr, showmap->m_size);
    } while ((showmap = showmap->next) != firstmap);
    printf("****************************************\n");
}

void show_err(int errtype)
{
    // 打印错误类型
    switch (errtype)
    {
    case 0:
        printf("分配失败，没有足够空间\n");
        break;
    case 1:
        printf("非法释放地址，请重新输入\n");
        break;
    case 2:
        printf("非法指令，请重新输入\n");
        break;
    case 3:
        printf("非法释放大小，请重新输入\n");
        break;

    default:
        break;
    }
}

void init_link(char *addr0)
{
    nowmap = (map *)malloc(sizeof(map));
    nowmap->m_addr = addr0;
    nowmap->m_size = Msize;
    nowmap->next = nowmap;
    nowmap->prior = nowmap;
}

void ml(char *addr0)
{
    char cmdchar;
    unsigned int size, a;
    char *addr;

    cmd_help();
    do
    {
        show_unuse();
        printf("\n输入指令>>> ");
        // scanf("%c",&cmdchar);
        do
        {
            // cmdchar = getchar();
            cmdchar = getchar();
        } while (cmdchar == '\n' || cmdchar == '\t' || cmdchar == ' ');

        if (cmdchar == 'm')
        {
            scanf("%u", &size);
            printf("调用lmalloc()分配内存大小 %u\n", size);

            addr = lmalloc(size);
            if (addr)
                printf("分配成功，内存地址 %p\n", addr);
        }
        else if (cmdchar == 'f')
        {
            printf("调用lfree()释放内存\n");
            scanf("%u %u", &a, &size);
            if (a < 0 || a >= Msize)
                // 非法地址
                show_err(1);
            else if(size < 1 || size > 1000)
                // 非法大小
                show_err(3);
            else
                lfree(size, addr0 + a);
        }
        else if (cmdchar == '\n' || cmdchar == '\t' || cmdchar == ' ' || cmdchar == 'q')
        {
        }
        else
            show_err(2);

    } while (cmdchar != 'q');
}

void free_link(char *addr0)
{
    // 释放空间
    free(addr0);

    // 释放链表
    map *midamap = NULL;
    while (nowmap != NULL)
    {
        midamap = nowmap->next->next;

        if (nowmap->next = nowmap)
        {
            free(nowmap);
            nowmap = NULL;
        }
        else
        {
            free(nowmap->next);
            nowmap->next = midamap;
            midamap->prior = nowmap;
        }
    }

        printf("成功释放所有内存，结束\n");
}

int main()
{
    // 分配初始内存 Msize
    char *addr0;
    addr0 = (char *)malloc(Msize);
    printf("所有内存起始地址 %p\n", addr0);

    // 初始化
    init_link(addr0);

    // 读取指令管理分区
    ml(addr0);

    // 结束工作，释放所有内存
    free_link(addr0);
    return 0;
}

