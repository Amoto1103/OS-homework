#include <iostream>
#include <malloc.h>
#include <stdio.h>
using namespace std;
struct map                         //结构体定义
{
    unsigned int size;
    char* address;
    struct map *next,*prior;
};

struct map* pt=new map;            //记录起始查找指针
struct map* head=new map;        //记录链表头的指针

//分配算法，传入需要分配的内存大小，输出分配的地址
void lmalloc(unsigned size)
{
    struct map* record=pt;
    if(pt==NULL)                   //处理起始查找指针为空的情况！！！！！
    {
        cout<<"没有符合分配要求的空闲区,情况1"<<endl;
        return;
    }
    if (pt->size < size)       //找到满足条件的空闲区
    {
        do{
            pt=pt->next;
            if(pt==record)       //防止进入死循环
            {
                cout<<"没有符合分配要求的空闲区，情况2"<<endl;
                return;
            }
        }while(pt->size < size);
    }
    cout<<"分配的地址为："<<(int)pt->address<<"-"<<(int)(pt->address+size-1)<<" 长度为："<<size<<endl;
    if (pt->size > size)     //如果空闲区大于需要的长度，直接截取,变化地址和空闲区长度
    {
        pt->size-=size;
        pt->address+=size;
        pt=pt->next;
    }
    else if(pt->size==size)  //如果空闲区等于需要的长度，将此链表节点删除并连接前后节点
    {
        struct map* rec=pt;
        pt->prior->next=pt->next;
        pt->next->prior=pt->prior;
        if(pt==pt->next)    //如果只有这一块了却要删除，要把pt置为空，并把头指针置为空
        {
            pt=NULL;
            head->next=NULL;
        }
        else if(pt==head->next)   //如果正好删掉了第一块，要更新头指针信息
        {
            head->next=pt->next;
            pt=pt->next;
        }
        else
        {
            pt=pt->next;
        }
        delete rec;    //删去pt指向的链表节点
    }

}

//释放算法，传入要回收的内存大小和起始地址，释放对应空间
void lfree(unsigned size,char* address)
{
    struct map *tmp_map=head;
    char *a;
    a=address;
    if(head->next==NULL)    //如果此时一点空闲区也不剩的话 要重新初始化
    {
        struct map* newnode=new map;
        head->next=newnode;
        pt=newnode;
        newnode->address=a;
        newnode->size=size;
        newnode->prior=head;
        newnode->next=newnode;
        cout<<"已释放"<<(int)a<<"位置处"<<size<<"大小的内存空间，情况0"<<endl;
        return;
    }
    do{
        tmp_map=tmp_map->next;
        if((tmp_map->next==head->next) && (tmp_map->address<a) )//如果是链表最后的节点，需要在尾部加上一个节点
        {
            if(tmp_map->address+tmp_map->size==a)
            {
                tmp_map->size+=size;
                cout<<"已释放"<<(int)a<<"位置处"<<size<<"大小的内存空间，情况1"<<endl;
                return;
            }
            else if(tmp_map->address+tmp_map->size<a)
            {
                struct map* newnode=new map;
                newnode->prior=tmp_map;
                tmp_map->next->prior=newnode;
                newnode->next=tmp_map->next;
                tmp_map->next=newnode;
                newnode->size=size;
                newnode->address=a;
                cout<<"已释放"<<(int)a<<"位置处"<<size<<"大小的内存空间，情况2"<<endl;
                return;
            }
        }
    }while(!(tmp_map->prior->address<=a && tmp_map->address>a)); //确定释放区的位置
    if(tmp_map->prior->address+tmp_map->prior->size==a)  //如果正好和前面的空闲区相邻
    {
        if(tmp_map->prior->size==0)           //如果正好是申请的地址开头，也就是成为第一块空闲区
        {
            struct map* newnode=new map; //创建新节点
            newnode->prior=tmp_map->prior;
            newnode->next=tmp_map;
            newnode->size=size;
            newnode->address=a;
            tmp_map->prior->next=newnode;
            tmp_map->prior=newnode;
            head->next=newnode;        //更新头指针
            if(tmp_map->next==tmp_map)
            {
                tmp_map->next=newnode;
            }
            if(a+size==tmp_map->address)          //如果与后面的空闲区相邻
            {
                tmp_map->prior->size+=tmp_map->size;
                tmp_map->prior->next=tmp_map->next;
                if(tmp_map->next!=newnode)
                {
                    tmp_map->next->prior=tmp_map->prior;
                    struct map* men=tmp_map;
                    do{
                        men=men->next;
                    }
                    while(men->next!=tmp_map);
                    men->next=newnode;
                }
                if(pt==tmp_map)                //更新查找指针
                {
                    pt=tmp_map->prior;
                }
                delete tmp_map;
            }
            cout<<"已释放"<<(int)a<<"位置处"<<size<<"大小的内存空间，情况3"<<endl;
        }
        else
        {
            tmp_map->prior->size+=size;           //仅与前空闲区相邻
            if(a+size==tmp_map->address)          //与前后都相邻
            {
                tmp_map->prior->size+=tmp_map->size;
                tmp_map->prior->next=tmp_map->next;
                tmp_map->next->prior=tmp_map->prior;
                if(pt==tmp_map)                 //更新查找指针
                {
                    pt=tmp_map->prior;
                }
                delete tmp_map;
            }
            cout<<"已释放"<<(int)a<<"位置处"<<size<<"大小的内存空间，情况4"<<endl;
        }
    }

    else
    {
        if(a+size==tmp_map->address)   //仅与后空闲区相邻
        {
            tmp_map->address-=size;
            tmp_map->size+=size;
            cout<<"已释放"<<(int)a<<"位置处"<<size<<"大小的内存空间，情况5"<<endl;
        }
        else{                    //都不相邻的话，新增一个节点
            struct map* newnode=new map;
            newnode->prior=tmp_map->prior;
            newnode->next=tmp_map;
            newnode->size=size;
            newnode->address=a;
            tmp_map->prior->next=newnode;
            tmp_map->prior=newnode;
            if(tmp_map->next==tmp_map)
            {
                tmp_map->next=newnode;
            }
            cout<<"已释放"<<(int)a<<"位置处"<<size<<"大小的内存空间，情况6"<<endl;
        }
    }

}

void display(int iaddr)
{
    struct map* pointer=head;
    if(head->next==NULL)
    {
        cout<<"现在没有任何可用的空闲区！"<<endl;
    }
    else
    {
        do{
            pointer=pointer->next;
            cout<<"空闲区范围:"<<(int)pointer->address-iaddr<<'-'<<(int)pointer->address+pointer->size-iaddr<<endl;
        }while(pointer->next!=head->next);
    }
    cout<<endl;
}
int main()
{
    char* iniaddr;
    iniaddr= (char*) malloc(1000);    //记录初始地址
    cout<<"初始地址为："<<(int)iniaddr<<"共1000字节"<<endl;
    head->address=iniaddr;           //记录分配到的初始地址
    head->size=0;
    head->next=pt;
    head->prior=NULL;
    pt->address=iniaddr;            //初始化分配查找指针
    pt->size=1000;
    pt->prior=head;
    pt->next=pt;
    char ctr;
    unsigned int size;
    int addr;
    do{
        scanf ("%c", &ctr);
        if(ctr=='m')
        {
            scanf("%u",&size);
            lmalloc(size);
            display(int(iniaddr));
        }
        else if(ctr=='f')
        {
            scanf("%u%u",&size,&addr);
            char* ADDR=(char*) addr;
            lfree(size,ADDR);
            display(int(iniaddr));
        }
        else if(ctr=='q')
        {
            return 0;
        }
    }while (true);
}
