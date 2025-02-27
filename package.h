#include<stdio.h>
#include<string.h>
#include<stdlib.h>

typedef struct PackageNode{
    int number;   //编号,与货物一一对应
    int weight;   
    int height;   
    int width;
    char name[20];   
    char type[10];
    char situation[10];   
    char owner[20];   
    int storage_place;   //存储的货架位置 0就是head 1就是head->next
    int start_place;      //在货架位置上的起点 0就是space[0]之后存储的
    struct PackageNode *next;
}package;

package* head_package=NULL;          //不是哨兵,只是普通的头
package* tail_package=NULL;          //方便更新,那么就需要维护一个尾节点

package *create_package(int number,int weight,int height,int width,char *name,char *type,char* owner){
    package * p = (package*)malloc(sizeof(package));
    p->number = number;
    p->weight = weight;
    p->height = height;
    p->width = width;
    p->start_place = -1;
    p->storage_place = -1;
    strcpy(p->name,name);
    strcpy(p->type,type);
    strcpy(p->situation,"undefined");
    strcpy(p->owner,owner);
    return p;
}//制造包裹

package* init_package()//初始化package链表结点
{
    package* p=(package*)malloc(sizeof(package));
    if(p==NULL){
        printf("memory exceeded\n");
        exit(1);
    }
    strcpy(p->type,"undefined");
    strcpy(p->situation,"undefined");
    strcpy(p->owner,"undefined");
    strcpy(p->name,"undefined");
    p->number=0;
    p->weight=0;
    p->height=0;
    p->width=0;
    p->storage_place=0;
    p->start_place=0;
    p->next=NULL;
    return p;
}

void save_package(const char* filename)
{
    FILE* file=fopen(filename,"w");
    if(file ==  NULL)
    {
        fprintf(stderr,"无法打开文件:%s\n",filename);
        return;
    }

    package* current = head_package;//从表头开始遍历
    while(current != NULL)
    {
        //写入
        fprintf(file,"%d %d %d %d %d %d ",current->number,current->weight,current->height,current->width,current->storage_place,current->start_place);
        fprintf(file,"%s %s %s %s\n",current->name,current->type,current->situation,current->owner);
        current = current->next;
    }
    fclose(file);
}

package* load_package(const char* filename)
{
    FILE* file = fopen(filename,"r");
    if(file == NULL)
    {
        fprintf(stderr,"无法打开文件：%s\n",filename);
        return NULL;

    }
    while(1)
    {
        package* p=init_package();
        if(fscanf(file,"%d %d %d %d %d %d %s %s %s %s",&p->number,&p->weight,&p->height,&p->width,&p->storage_place,&p->start_place,p->name,p->type,p->situation,p->owner)!=10)
        {
            free(p);
            if(feof(file))break;
            fprintf(stderr,"读取数据失败\n");
            fclose(file);
            return NULL;
        }
        //将新的结点加入链表
        if(head_package==NULL)
        {
            head_package = p;
            tail_package = p;
        }
        else
        {
            tail_package->next = p;
            tail_package = p;
        }
    }
    fclose(file);
    return head_package;
}

package* check_package(int number){
    if(head_package==NULL)
    {
        printf("check_package failed,货架为空\n");
        return NULL;
    }

    //开始查找
    package* temp=head_package;
    while(temp)
    {
        if(temp->number==number)
        {
            printf("查找成功！");
            return temp;
        }
        temp=temp->next;
    }

    //如果没找到
        printf("check_package failed,包裹不存在\n");
    return temp;
    //通过编号查找包裹
}

void add_package(package* p){
    
    //如果之前货架是空
    if(head_package==NULL)
    {   
        head_package=p;
        tail_package=p;
    }
    else{
        tail_package->next=p;
        tail_package = p;
    }
    p->next=NULL;
    printf("存储包裹成功！\n");
    //将这个包裹接入包裹链表
}

void delete_package(int number) {
    if (head_package == NULL) {
        printf("delete_package failed,货架为空\n");
        return;
    }

    package* temp = head_package;
    package* prev = NULL;

    // 找到要删除的包裹
    while (temp != NULL && temp->number != number) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) {
        printf("delete_package failed,包裹不存在\n");
        return;
    }

    // 删除头节点
    if (temp == head_package) 
    {
        head_package = temp->next;
        if (head_package == NULL) 
        {
            tail_package = NULL;  // 如果链表变为空，tail_package也需要置为NULL
        }
    } 
    else 
    {
        prev->next = temp->next;
    }

    // 如果删除的是尾节点，更新 tail_package
    if (temp->next == NULL) {
        tail_package = prev;
        tail_package->next=NULL;
    }

    free(temp);
    printf("deleted_succeed\n");

    //如果包裹录入错误,那么就把它彻底抹除,从链表里删去
}


