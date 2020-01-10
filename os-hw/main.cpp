#include <iostream>
#include <malloc.h>
#include <stdio.h>
using namespace std;
struct map                         //�ṹ�嶨��
{
    unsigned int size;
    char* address;
    struct map *next,*prior;
};

struct map* pt=new map;            //��¼��ʼ����ָ��
struct map* head=new map;        //��¼����ͷ��ָ��

//�����㷨��������Ҫ������ڴ��С���������ĵ�ַ
void lmalloc(unsigned size)
{
    struct map* record=pt;
    if(pt==NULL)                   //������ʼ����ָ��Ϊ�յ��������������
    {
        cout<<"û�з��Ϸ���Ҫ��Ŀ�����,���1"<<endl;
        return;
    }
    if (pt->size < size)       //�ҵ����������Ŀ�����
    {
        do{
            pt=pt->next;
            if(pt==record)       //��ֹ������ѭ��
            {
                cout<<"û�з��Ϸ���Ҫ��Ŀ����������2"<<endl;
                return;
            }
        }while(pt->size < size);
    }
    cout<<"����ĵ�ַΪ��"<<(int)pt->address<<"-"<<(int)(pt->address+size-1)<<" ����Ϊ��"<<size<<endl;
    if (pt->size > size)     //���������������Ҫ�ĳ��ȣ�ֱ�ӽ�ȡ,�仯��ַ�Ϳ���������
    {
        pt->size-=size;
        pt->address+=size;
        pt=pt->next;
    }
    else if(pt->size==size)  //���������������Ҫ�ĳ��ȣ���������ڵ�ɾ��������ǰ��ڵ�
    {
        struct map* rec=pt;
        pt->prior->next=pt->next;
        pt->next->prior=pt->prior;
        if(pt==pt->next)    //���ֻ����һ����ȴҪɾ����Ҫ��pt��Ϊ�գ�����ͷָ����Ϊ��
        {
            pt=NULL;
            head->next=NULL;
        }
        else if(pt==head->next)   //�������ɾ���˵�һ�飬Ҫ����ͷָ����Ϣ
        {
            head->next=pt->next;
            pt=pt->next;
        }
        else
        {
            pt=pt->next;
        }
        delete rec;    //ɾȥptָ�������ڵ�
    }

}

//�ͷ��㷨������Ҫ���յ��ڴ��С����ʼ��ַ���ͷŶ�Ӧ�ռ�
void lfree(unsigned size,char* address)
{
    struct map *tmp_map=head;
    char *a;
    a=address;
    if(head->next==NULL)    //�����ʱһ�������Ҳ��ʣ�Ļ� Ҫ���³�ʼ��
    {
        struct map* newnode=new map;
        head->next=newnode;
        pt=newnode;
        newnode->address=a;
        newnode->size=size;
        newnode->prior=head;
        newnode->next=newnode;
        cout<<"���ͷ�"<<(int)a<<"λ�ô�"<<size<<"��С���ڴ�ռ䣬���0"<<endl;
        return;
    }
    do{
        tmp_map=tmp_map->next;
        if((tmp_map->next==head->next) && (tmp_map->address<a) )//������������Ľڵ㣬��Ҫ��β������һ���ڵ�
        {
            if(tmp_map->address+tmp_map->size==a)
            {
                tmp_map->size+=size;
                cout<<"���ͷ�"<<(int)a<<"λ�ô�"<<size<<"��С���ڴ�ռ䣬���1"<<endl;
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
                cout<<"���ͷ�"<<(int)a<<"λ�ô�"<<size<<"��С���ڴ�ռ䣬���2"<<endl;
                return;
            }
        }
    }while(!(tmp_map->prior->address<=a && tmp_map->address>a)); //ȷ���ͷ�����λ��
    if(tmp_map->prior->address+tmp_map->prior->size==a)  //������ú�ǰ��Ŀ���������
    {
        if(tmp_map->prior->size==0)           //�������������ĵ�ַ��ͷ��Ҳ���ǳ�Ϊ��һ�������
        {
            struct map* newnode=new map; //�����½ڵ�
            newnode->prior=tmp_map->prior;
            newnode->next=tmp_map;
            newnode->size=size;
            newnode->address=a;
            tmp_map->prior->next=newnode;
            tmp_map->prior=newnode;
            head->next=newnode;        //����ͷָ��
            if(tmp_map->next==tmp_map)
            {
                tmp_map->next=newnode;
            }
            if(a+size==tmp_map->address)          //��������Ŀ���������
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
                if(pt==tmp_map)                //���²���ָ��
                {
                    pt=tmp_map->prior;
                }
                delete tmp_map;
            }
            cout<<"���ͷ�"<<(int)a<<"λ�ô�"<<size<<"��С���ڴ�ռ䣬���3"<<endl;
        }
        else
        {
            tmp_map->prior->size+=size;           //����ǰ����������
            if(a+size==tmp_map->address)          //��ǰ������
            {
                tmp_map->prior->size+=tmp_map->size;
                tmp_map->prior->next=tmp_map->next;
                tmp_map->next->prior=tmp_map->prior;
                if(pt==tmp_map)                 //���²���ָ��
                {
                    pt=tmp_map->prior;
                }
                delete tmp_map;
            }
            cout<<"���ͷ�"<<(int)a<<"λ�ô�"<<size<<"��С���ڴ�ռ䣬���4"<<endl;
        }
    }

    else
    {
        if(a+size==tmp_map->address)   //��������������
        {
            tmp_map->address-=size;
            tmp_map->size+=size;
            cout<<"���ͷ�"<<(int)a<<"λ�ô�"<<size<<"��С���ڴ�ռ䣬���5"<<endl;
        }
        else{                    //�������ڵĻ�������һ���ڵ�
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
            cout<<"���ͷ�"<<(int)a<<"λ�ô�"<<size<<"��С���ڴ�ռ䣬���6"<<endl;
        }
    }

}

void display(int iaddr)
{
    struct map* pointer=head;
    if(head->next==NULL)
    {
        cout<<"����û���κο��õĿ�������"<<endl;
    }
    else
    {
        do{
            pointer=pointer->next;
            cout<<"��������Χ:"<<(int)pointer->address-iaddr<<'-'<<(int)pointer->address+pointer->size-iaddr<<endl;
        }while(pointer->next!=head->next);
    }
    cout<<endl;
}
int main()
{
    char* iniaddr;
    iniaddr= (char*) malloc(1000);    //��¼��ʼ��ַ
    cout<<"��ʼ��ַΪ��"<<(int)iniaddr<<"��1000�ֽ�"<<endl;
    head->address=iniaddr;           //��¼���䵽�ĳ�ʼ��ַ
    head->size=0;
    head->next=pt;
    head->prior=NULL;
    pt->address=iniaddr;            //��ʼ���������ָ��
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
