#include<stdio.h>
#include<string.h>
#include<stdlib.h>

typedef struct PackageNode{
    int number;   //���,�����һһ��Ӧ
    int weight;   
    int height;   
    int width;
    char name[20];   
    char type[10];
    char situation[10];   
    char owner[20];   
    int storage_place;   //�洢�Ļ���λ�� 0����head 1����head->next
    int start_place;      //�ڻ���λ���ϵ���� 0����space[0]֮��洢��
    struct PackageNode *next;
}package;

package* head_package=NULL;          //�����ڱ�,ֻ����ͨ��ͷ
package* tail_package=NULL;          //�������,��ô����Ҫά��һ��β�ڵ�

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
}//�������

package* init_package()//��ʼ��package������
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
        fprintf(stderr,"�޷����ļ�:%s\n",filename);
        return;
    }

    package* current = head_package;//�ӱ�ͷ��ʼ����
    while(current != NULL)
    {
        //д��
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
        fprintf(stderr,"�޷����ļ���%s\n",filename);
        return NULL;

    }
    while(1)
    {
        package* p=init_package();
        if(fscanf(file,"%d %d %d %d %d %d %s %s %s %s",&p->number,&p->weight,&p->height,&p->width,&p->storage_place,&p->start_place,p->name,p->type,p->situation,p->owner)!=10)
        {
            free(p);
            if(feof(file))break;
            fprintf(stderr,"��ȡ����ʧ��\n");
            fclose(file);
            return NULL;
        }
        //���µĽ���������
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
        printf("check_package failed,����Ϊ��\n");
        return NULL;
    }

    //��ʼ����
    package* temp=head_package;
    while(temp)
    {
        if(temp->number==number)
        {
            printf("���ҳɹ���");
            return temp;
        }
        temp=temp->next;
    }

    //���û�ҵ�
        printf("check_package failed,����������\n");
    return temp;
    //ͨ����Ų��Ұ���
}

void add_package(package* p){
    
    //���֮ǰ�����ǿ�
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
    printf("�洢�����ɹ���\n");
    //��������������������
}

void delete_package(int number) {
    if (head_package == NULL) {
        printf("delete_package failed,����Ϊ��\n");
        return;
    }

    package* temp = head_package;
    package* prev = NULL;

    // �ҵ�Ҫɾ���İ���
    while (temp != NULL && temp->number != number) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) {
        printf("delete_package failed,����������\n");
        return;
    }

    // ɾ��ͷ�ڵ�
    if (temp == head_package) 
    {
        head_package = temp->next;
        if (head_package == NULL) 
        {
            tail_package = NULL;  // ��������Ϊ�գ�tail_packageҲ��Ҫ��ΪNULL
        }
    } 
    else 
    {
        prev->next = temp->next;
    }

    // ���ɾ������β�ڵ㣬���� tail_package
    if (temp->next == NULL) {
        tail_package = prev;
        tail_package->next=NULL;
    }

    free(temp);
    printf("deleted_succeed\n");

    //�������¼�����,��ô�Ͱ�������Ĩ��,��������ɾȥ
}


