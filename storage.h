#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "package.h"

#define N 20 // ���ܿ��

typedef struct storage {
    int space[N];          // ���ܿռ�
    int list[N][3];       // ����б�ͻ���λ��(���) 123(���) 0(����0��) 2(��ʾ����)
    int package_num;      // ��������
    int biggest_slot;     //���µ�����ȱ�ڵĿ��
    int empty_space;       //ʣ��λ��
    struct storage* next;
} storage;

storage* head_stor = NULL;

storage* init() {
    storage* p = (storage*)malloc(sizeof(storage));
    if (p == NULL) {
        printf("memory exceeded\n");
        exit(1);
    }
    memset(p->space, 0, sizeof(p->space)); // ��ʼ�� space ����
    memset(p->list, 0, sizeof(p->list));   // ��ʼ�� list ����
    p->package_num = 0;                     // ��ʼ����������Ϊ 0
    p->biggest_slot = N;                    // ��ʼ������λ
    p->empty_space = N;                     // ��ʼ�����пռ�
    p->next = NULL;                         // ��ʼ����һ��ָ��Ϊ NULL
    return p;
}

storage* load_stor(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "�޷����ļ�: %s\n", filename);
        return NULL;
    }

    storage* tail = NULL; // ����βָ��
    while (1) {
        storage* p = init(); // ��ʼ���µ� storage �ṹ��
        if (fscanf(file, "%d", &p->space[0]) != 1) {
            free(p); // ��ȡʧ�ܣ��ͷ��ڴ�
            if (feof(file)) break; // �����ļ�ĩβ
            fprintf(stderr, "��ȡ����ʧ��\n");
            fclose(file);
            return NULL;
        }
        for (int i = 1; i < N; i++) {
            if (fscanf(file, "%d", &p->space[i]) != 1) {
                free(p); // ��ȡʧ�ܣ��ͷ��ڴ�
                if (feof(file)) break; // �����ļ�ĩβ
                fprintf(stderr, "��ȡ����ʧ��\n");
                fclose(file);
                return NULL;
            }
        }
        
        // ��ȡ biggest_slot  empty_space �� package_num
        if (fscanf(file, "%d %d %d", &p->biggest_slot, &p->empty_space,&p->package_num) != 3) {
            free(p); // ��ȡʧ�ܣ��ͷ��ڴ�
            fprintf(stderr, "��ȡ biggest_slot,empty_space��package_numʧ��\n");
            fclose(file);
            return NULL;
        }

        // ��ȡ�����š�λ�úͳ���
        for (int i = 0; i < p->package_num; i++) {
            if (fscanf(file, "%d %d %d", &p->list[i][0], &p->list[i][1], &p->list[i][2]) != 3) {
                free(p); // ��ȡʧ�ܣ��ͷ��ڴ�
                fprintf(stderr, "��ȡ�����š�λ�úͳ���ʧ��\n");
                fclose(file);
                return NULL;
            }
        }

        // ���µ� storage �ṹ����ӵ�������
        if (head_stor == NULL) {
            head_stor = p; // ��һ���ڵ�
            tail = p;
        } else {
            tail->next = p; // ���½ڵ����ӵ�����
            tail = p; // ����βָ��
        }
    }

    fclose(file);
    return head_stor; // ��������ͷָ��
}

void save_stor(const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        fprintf(stderr, "�޷����ļ�: %s\n", filename);
        return;
    }

    storage* current = head_stor; // ������ͷ��ʼ����
    while (current != NULL) {
        // д�� space ����
        for (int i = 0; i < N; i++) {
            fprintf(file, "%d ", current->space[i]);
        }
        fprintf(file, "\n"); // ÿ�� storage ��������

        // д�� biggest_slot �� empty_space
        fprintf(file, "%d %d %d\n", current->biggest_slot, current->empty_space,current->package_num);

        // д������š�λ�úͳ���
        for (int i = 0; i < current->package_num; i++) {
            fprintf(file, "%d %d %d\n", current->list[i][0], current->list[i][1], current->list[i][2]);
        }
        current = current->next; // �ƶ�����һ�� storage
    }

    fclose(file);
}

int trans(package* p) {
    if (p->height > p->width) {
        return p->height;
    } else {
        return p->width;
    }
}

int find_best_place(int* space, int len) {
    int count=0;
    int mark=0;
    int flag=0;
    for(int i=0;i<N;i++){
        if(space[i]==0){
            count++;
        }
        if(count>=len){//�ҵ���
            mark=i;
            flag=1;
            break;
        }
    }
    if(!flag){
        printf("find_best_place failed,package doesn't fit");
        return 1;
    }
    return mark;
    //�ҵ���С����len��λ��
}
int find_present_biggest_slot(int* space) {
    int maxcount=0;
    int currentcount=0;
    for(int i=0;i<N;i++){
        if(space[i]==0){
            currentcount++;
            if(currentcount>maxcount){
                maxcount=currentcount;
            }
        }
        else{
            currentcount=0;
        }
    }
    return maxcount;
    //�ҳ��������0����
}//R

void collate(storage *p){     
    //���� ��ô����Ҫ�ı�����Ķ���   �����λ������   space��ģ��״̬   list�л����λ��   ���ܵ�biggest_slot���� 
    if(p==NULL)
    {
        fprintf(stderr, "���������Ŀ��");
        return ;
    }
    while(p!=NULL)
    {
        int cursor=0;//���ɨ�裬����֮����
        int firstemptyspace=0;//����ұߵĻ��������ǵ�һ����λ
        
        while(cursor!=N)
        {
            if(p->space[cursor]==0)//��λ���������
            {
                cursor++;
                continue;
            }
            //������Ϣ

            int number=p->space[cursor];

            package*ptr = check_package(number); //�ҵ��Ǹ�����

            int k=0;
            for(;k<p->package_num;k++)//����list�����±�
            {
                if(p->list[k][0]==number)break;
            }
            int length=p->list[k][2];
            int pos=p->list[k][1];
            for(int i=pos;i<pos+length;i++)
            {
                p->space[i]=0;//��հ�����Ϣ
            }
            for(int i=firstemptyspace;i<firstemptyspace+length;i++)
            {
                p->space[i]=number;//��Ӱ���
            }
            
            p->list[k][1]=firstemptyspace;

            ptr->start_place = firstemptyspace;  //���»����״̬

            firstemptyspace+=length;
            cursor=firstemptyspace;
        }
        p->biggest_slot=p->empty_space=N-firstemptyspace;
        p=p->next;
    }
    printf("collate command succeed\n");
}//L

int place_package_to_stor(package* p) {
    int len = trans(p);
    storage* ptr = head_stor;
    int cnt = 0;
    while (ptr != NULL) {
        if (ptr->biggest_slot >= len) {
            int starti = find_best_place(ptr->space,len); //����߿�ʼ��
            for (int j = 0; j < len; j++) {
                ptr->space[j + starti] = len; 
            }
            ptr->biggest_slot = find_present_biggest_slot(ptr->space);
            ptr->empty_space -= len;
            //����İ���,��ı䵱ǰlist�洢������״̬
            ptr->list[ptr->package_num][0] = p->number;              
            ptr->list[ptr->package_num][1] = starti;
            ptr->list[ptr->package_num][2] = len;
            ptr->package_num++;//���˸�������
            strcpy(p->situation,"placed");
            p->start_place = starti;
            p->storage_place = cnt;
            return 1;
        }
        cnt++;
        ptr = ptr->next; // ����������һ�� storage
    }
    printf("no more free slot remain\n");
    //û�ռ��� ��ô�͸������� ��ͬһ�еĻ��ﲢ��һ�� ��ôbiggest_slot����empty_space
    //������Ҫһ������
    collate(head_stor);  //������
    ptr = head_stor;
    cnt = 0;
    while(ptr!=NULL){
        if (ptr->biggest_slot >= len) {
            int starti = find_best_place(ptr->space,len); //����߿�ʼ��
            for (int j = 0; j < len; j++) {
                ptr->space[j + starti] = len; 
            }
            ptr->biggest_slot = find_present_biggest_slot(ptr->space);
            ptr->empty_space -= len;

            ptr->list[ptr->package_num][0] = p->number;
            ptr->list[ptr->package_num][1] = starti;
            ptr->list[ptr->package_num][2] = len;
            ptr->package_num++;
            strcpy(p->situation,"placed");
            p->start_place = starti;
            p->storage_place = cnt;
            return 1;
        }
        cnt++;
        ptr = ptr->next; // ����������һ�� storage
    }
    return 0;   //������Ҳ�Ų���, �Ǿ͸ɴ಻����,���߷�����,����������֮����ʱ�����չ������
}

int take_package_from_stor(package*p){
    int n = p->storage_place;
    storage*ptr = head_stor;
    for(int i =0;i<n;i++){
        ptr = ptr->next;
    }
    return 0;
    //ͨ��package�洢������ �ҵ���Ӧ�Ļ��� ͨ�����ܵ�list�ҵ�package��λ�� ֮���Ƴ���package ���»���״̬
    //�Ҳ����ͷ��� 0 �ҵ��˾ͷ��� 1
}//S




