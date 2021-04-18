#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#define MAX_SIZE 1024

struct map *startmap;
struct map *find;
char* startadd;

struct map{
    unsigned m_size;//空闲区大小
    char *m_addr;//空闲区首地址
    struct map *next, *prior;//指向前后节点的指针
};

void init(){
    startadd = (char*) malloc(MAX_SIZE);//申请初始的空间，大小为MAX_SIZE
    if(startadd != NULL){//申请成功
        printf("malloc successfully! Startadd:%p \n",startadd);
    }
    else{//申请失败
        printf("fail to malloc.");
        exit(-1);
    }

    //构建链表
    startmap = (struct map *) malloc(sizeof(struct map));
    if(startmap == NULL){//构建失败
        printf("fail to create a map");
        exit(-1);
    }
    //设置链表初始表项的信息
    startmap->m_size = MAX_SIZE;
    startmap->m_addr = startadd;
    startmap->next = startmap;
    startmap->prior = startmap;
    find = startmap;//查找指针指向首个表项
}

//分配内存
char *lmalloc(unsigned size){
    //需要的内存大于MAX_SIZE
    if (size > MAX_SIZE){
        printf("The size cannot be greater than 1024!\n");
        return NULL;
    }
    //需要内存为0
    if (size == 0){
        printf("The size cannot be 0!\n");
        return NULL;
    }

    struct map* curr = find;
    char *res = NULL;
    do{
        //该空闲区大小小于所需大小
       if(find->m_size < size){
           find = find->next;
       }
       else if (find->m_size > size){//空闲区大小大于所需大小
           res = find->m_addr;
           //修改参数
           find->m_size = find->m_size - size;
           find->m_addr = find->m_addr + size;
           find = find -> next;//修改find指针
           return res;
       }
       else if (find->m_size == size){
            res = find -> m_addr;
            if (find-> next == find && find == startmap){//空闲区全部被分配
                find->m_size = 0;
                find->m_addr = NULL;
            }
            else if(find-> next != find && find == startmap){//地址最小的空闲区被分配
                startmap = find -> next;//修改startmap指针只想新的地址最小的空闲区
                struct map * temp = find;
                //释放节点
                find -> next -> prior = find -> prior;
                find -> prior -> next = find -> next;
                free(temp);
                find = startmap;//修改find指针
            }
            else{
                struct map * temp = find;
                find = find -> next;//修改find指针
                //直接释放节点
                temp -> next -> prior = temp -> prior;
                temp -> prior -> next = temp -> next;
                free(temp);
            }
           return res;
       }
    }while(curr != find);
    printf("space is not enough!\n");//找不到合适的空闲区
    return res;
}

//释放内存
void lfree(unsigned size, char *addr){
    struct map *head = startmap;
    if (size == 0){//输入的size为0不合法
        printf("size cannot be 0!\n");
        return;
    }
    if(head -> next == head && head->m_size == 0){//空闲区不存在
        //直接修改参数
        head->m_size = size;
        head->m_addr = addr;
        printf("free successfully!\n");
        return;
    }
    if (addr + size < head->m_addr ){//释放的空间的地址最小
        //构建新的链表节点
        struct map* temp = (struct map *) malloc(sizeof(struct map));
        temp ->m_size = size;
        temp ->m_addr = addr;
        //把新节点插入链表
        temp -> next = head;
        temp -> prior = head -> prior;
        temp -> prior ->next = temp;
        head -> prior = temp;
        startmap = temp;//使startmap指向地址最小的空闲区
        printf("free successfully!\n");
        return;
    }
    if (addr + size == head->m_addr ){//释放的空间与第一个空闲区相连
        //直接修改参数
        head->m_size = head->m_size + size;
        head->m_addr = addr;
        printf("free successfully!\n");
        return;
    }
    if (addr > head -> prior -> m_addr + head -> prior -> m_size && addr + size <= startadd + MAX_SIZE){//释放的空间在最后一个空闲区之后
        //构建新的节点
        struct map* temp = (struct map *) malloc(sizeof(struct map));
        temp ->m_size =size;
        temp ->m_addr =addr;
        //把节点插入链表
        temp ->next = head;
        temp ->prior = head->prior;
        head->prior = temp;
        temp->prior->next = temp;
        printf("free successfully!\n");
        return;
    }
    if (addr == head -> prior -> m_addr + head -> prior -> m_size && addr + size <= startadd + MAX_SIZE){//释放的空间与最后一个空闲区相连
        //直接修改参数
        head -> prior -> m_size = head -> prior -> m_size + size;
        printf("free successfully!\n");
        return;
    }
    do{
        if(addr > (head->m_addr + head ->m_size) && addr+ size < head -> next -> m_addr){//第四种情况
            //构建新的节点
            struct map* temp = (struct map *) malloc(sizeof(struct map));
            temp ->m_size = size;
            temp ->m_addr = addr;
            //把节点插入链表
            temp -> next = head -> next;
            temp -> prior = head ;
            head ->next = temp;
            temp -> next -> prior = temp;
            printf("free successfully!\n");
            return;
        }
        if(addr == (head->m_addr + head ->m_size) && addr+ size < head -> next -> m_addr){//第一种情况
            //直接修改参数
            head->m_size = head->m_size + size;
            printf("free successfully!\n");
            return;
        }
        if(addr > (head->m_addr + head ->m_size) && addr+ size == head -> next -> m_addr){//第三种情况
            //修改相关参数
            head->next->m_addr = addr;
            head->next->m_size = head->next->m_size + size;
            printf("free successfully!\n");
            return;
        }
        if(addr == (head->m_addr + head ->m_size) && addr+ size == head -> next -> m_addr){//第二种情况
            //find指针指向需要被删除的空闲区
            if (find == head -> next){
                find = head;
            }
            //合并空闲区
            struct map* temp = head -> next;
            head -> m_size = head -> m_size + size + temp -> m_size;//修改size
            //删除节点
            head -> next = temp -> next;
            temp -> next -> prior = head;
            free(temp);
            printf("free successfully!\n");
            return;
        }
        head = head -> next;
    }while(head != startmap);
    printf("input is invaild!\n");//循环一周仍未释放，说明输入不合法
}

//用于打印空闲区情况
void printres(){
    struct map * head = startmap;
    //空闲区被占满
    if (head->m_size == 0){
        printf("No space left!\n");
        return;
    }
    printf("====================================================================\n");
    printf("Free space:\n");
    //循环打印链表
    do{
        printf("Absolute address: %p,Size: %d,Relative address: %d\n",head->m_addr,head->m_size,head->m_addr-startadd);
        head = head-> next;
    }while(head != startmap);//结束的条件为head循环一遍链表与初值相同
    printf("====================================================================\n");
}

//退出时调用
void quit(){
    free(startadd);//释放初始内存
    struct map * head = startmap;
    head -> prior -> next = NULL;//便于循环的判断
    //链表空间的释放
    do{
        struct map * temp = head;
        head = head-> next;
        free(temp);
    }while(head != NULL);//head指向NULL说明全部节点已经被释放
}

//打印提示信息
void printhint(){
    printf("Please input your instruction.\n");
    printf("f for free (need paramater: size and address).\n");
    printf("m for malloc (need paramater: size).\n");
    printf("q for quit.\n");
    printf("h for help .\n");

}

int main() {
    init();//初始化
    printres();//初始的空闲区
    printhint();//打印提示信息

    char c;//用于存储用户的指令
    unsigned size;//存储用户输入的参数
    unsigned raddr;//存储用户输入的参数
    char * addtmp;//临时变量

    while(true){
        do{
            c=getchar();
        }while(c=='\n'||c=='\t'||c==' ');//根据用户输入习惯过滤一些字符

        if(c == 'q'){//退出
            printf("Goodbye!\n");
            break;
        }
        else if(c == 'm'){//分配内存
            scanf("%u", &size);//读取参数
            addtmp = lmalloc(size);//调用lmalloc函数
            if(addtmp == NULL){//分配失败
                printf("fail to malloc!\n");
            }else{//分配成功
                printf("malloc successfully!\nAbsolute address:%p Relative address:%u\n",addtmp,addtmp - startadd);
            }
            printres();//打印空闲区
            //printf("%u",size);
        }
        else if(c == 'f'){//释放内存
            scanf("%u %u",&raddr, &size);//读取参数
            lfree(size, raddr + startadd);//调用lfee函数
            printres();//打印空闲区
        }
        else if (c == 'h'){//获取提示
            printhint();
        }
        else{//输入有误
            printf("Input is wrong.\n");
        }
    }
    quit();//释放内存
    return 0;
}
