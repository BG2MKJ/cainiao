#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "package.h"

#define N 20 // 货架宽度

typedef struct storage {
    int space[N];          // 货架空间
    int list[N][3];       // 编号列表和货物位置(起点) 123(编号) 0(放在0处) 2(表示长度)
    int package_num;      // 货物数量
    int biggest_slot;     //当下的最大空缺口的宽度
    int empty_space;       //剩余位置
    struct storage* next;
} storage;

storage* head_stor = NULL;

storage* init() {
    storage* p = (storage*)malloc(sizeof(storage));
    if (p == NULL) {
        printf("memory exceeded\n");
        exit(1);
    }
    memset(p->space, 0, sizeof(p->space)); // 初始化 space 数组
    memset(p->list, 0, sizeof(p->list));   // 初始化 list 数组
    p->package_num = 0;                     // 初始化货物数量为 0
    p->biggest_slot = N;                    // 初始化最大槽位
    p->empty_space = N;                     // 初始化空闲空间
    p->next = NULL;                         // 初始化下一个指针为 NULL
    return p;
}

storage* load_stor(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "无法打开文件: %s\n", filename);
        return NULL;
    }

    storage* tail = NULL; // 链表尾指针
    while (1) {
        storage* p = init(); // 初始化新的 storage 结构体
        if (fscanf(file, "%d", &p->space[0]) != 1) {
            free(p); // 读取失败，释放内存
            if (feof(file)) break; // 到达文件末尾
            fprintf(stderr, "读取数据失败\n");
            fclose(file);
            return NULL;
        }
        for (int i = 1; i < N; i++) {
            if (fscanf(file, "%d", &p->space[i]) != 1) {
                free(p); // 读取失败，释放内存
                if (feof(file)) break; // 到达文件末尾
                fprintf(stderr, "读取数据失败\n");
                fclose(file);
                return NULL;
            }
        }
        
        // 读取 biggest_slot  empty_space 和 package_num
        if (fscanf(file, "%d %d %d", &p->biggest_slot, &p->empty_space,&p->package_num) != 3) {
            free(p); // 读取失败，释放内存
            fprintf(stderr, "读取 biggest_slot,empty_space或package_num失败\n");
            fclose(file);
            return NULL;
        }

        // 读取货物编号、位置和长度
        for (int i = 0; i < p->package_num; i++) {
            if (fscanf(file, "%d %d %d", &p->list[i][0], &p->list[i][1], &p->list[i][2]) != 3) {
                free(p); // 读取失败，释放内存
                fprintf(stderr, "读取货物编号、位置和长度失败\n");
                fclose(file);
                return NULL;
            }
        }

        // 将新的 storage 结构体添加到链表中
        if (head_stor == NULL) {
            head_stor = p; // 第一个节点
            tail = p;
        } else {
            tail->next = p; // 将新节点链接到链表
            tail = p; // 更新尾指针
        }
    }

    fclose(file);
    return head_stor; // 返回链表头指针
}

void save_stor(const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        fprintf(stderr, "无法打开文件: %s\n", filename);
        return;
    }

    storage* current = head_stor; // 从链表头开始遍历
    while (current != NULL) {
        // 写入 space 数组
        for (int i = 0; i < N; i++) {
            fprintf(file, "%d ", current->space[i]);
        }
        fprintf(file, "\n"); // 每个 storage 结束后换行

        // 写入 biggest_slot 和 empty_space
        fprintf(file, "%d %d %d\n", current->biggest_slot, current->empty_space,current->package_num);

        // 写入货物编号、位置和长度
        for (int i = 0; i < current->package_num; i++) {
            fprintf(file, "%d %d %d\n", current->list[i][0], current->list[i][1], current->list[i][2]);
        }
        current = current->next; // 移动到下一个 storage
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
        if(count>=len){//找到了
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
    //找到最小大于len的位置
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
    //找出最大连续0数量
}//R

void collate(storage *p){     
    //整理 那么就需要改变下面的东西   货物的位置属性   space的模拟状态   list中货物的位置   货架的biggest_slot属性 
    if(p==NULL)
    {
        fprintf(stderr, "整理操作无目标");
        return ;
    }
    while(p!=NULL)
    {
        int cursor=0;//光标扫描，所到之处搬
        int firstemptyspace=0;//如果右边的货想左并这是第一个空位
        
        while(cursor!=N)
        {
            if(p->space[cursor]==0)//空位，光标右移
            {
                cursor++;
                continue;
            }
            //货物信息

            int number=p->space[cursor];

            package*ptr = check_package(number); //找到那个货物

            int k=0;
            for(;k<p->package_num;k++)//查找list对于下标
            {
                if(p->list[k][0]==number)break;
            }
            int length=p->list[k][2];
            int pos=p->list[k][1];
            for(int i=pos;i<pos+length;i++)
            {
                p->space[i]=0;//清空包裹信息
            }
            for(int i=firstemptyspace;i<firstemptyspace+length;i++)
            {
                p->space[i]=number;//添加包裹
            }
            
            p->list[k][1]=firstemptyspace;

            ptr->start_place = firstemptyspace;  //更新货物的状态

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
            int starti = find_best_place(ptr->space,len); //从左边开始放
            for (int j = 0; j < len; j++) {
                ptr->space[j + starti] = len; 
            }
            ptr->biggest_slot = find_present_biggest_slot(ptr->space);
            ptr->empty_space -= len;
            //进入的包裹,会改变当前list存储包裹的状态
            ptr->list[ptr->package_num][0] = p->number;              
            ptr->list[ptr->package_num][1] = starti;
            ptr->list[ptr->package_num][2] = len;
            ptr->package_num++;//多了个包裹嘛
            strcpy(p->situation,"placed");
            p->start_place = starti;
            p->storage_place = cnt;
            return 1;
        }
        cnt++;
        ptr = ptr->next; // 继续遍历下一个 storage
    }
    printf("no more free slot remain\n");
    //没空间了 那么就该整理了 将同一行的货物并到一起 那么biggest_slot就是empty_space
    //整理需要一个函数
    collate(head_stor);  //整理函数
    ptr = head_stor;
    cnt = 0;
    while(ptr!=NULL){
        if (ptr->biggest_slot >= len) {
            int starti = find_best_place(ptr->space,len); //从左边开始放
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
        ptr = ptr->next; // 继续遍历下一个 storage
    }
    return 0;   //整理了也放不下, 那就干脆不放了,或者放外面,处理方法这里之后有时间可以展开讨论
}

int take_package_from_stor(package*p){
    int n = p->storage_place;
    storage*ptr = head_stor;
    for(int i =0;i<n;i++){
        ptr = ptr->next;
    }
    return 0;
    //通过package存储的内容 找到对应的货架 通过货架的list找到package的位置 之后移除该package 更新货架状态
    //找不到就返回 0 找到了就返回 1
}//S




