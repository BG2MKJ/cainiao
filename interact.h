#include <stdio.h>
#include <string.h>
char user_logged[20];
int user_type;
// �Ҵ��㽨��ȫ�ֱ�����¼�û������û����� �û����������ֱ�ʾ
void welcome()
{
    printf("##################################################\n��ӭʹ�ÿ����վ����ϵͳ\nWelcome to use Courier station management system\n##################################################\n");
}

bool findaccount(char account[],char password[],int* type)//����account��ֵ���������Ӧ�û���password��type��Ϊ��������ֵ������ȶ�
{

}

bool fetch_user()
{
    printf("�������û��˺�\n");
    char account[40];
    int type=0;
    fgets(account,39,stdin);
    char password_right[40];
    char password_input[40];
    if(findaccount(account,password_right,&type)==0)
    {
        printf("can't find account");
        return false;
    }
    printf("�������û�����\n");
    fgets(password_input,39,stdin);
    if(strcmp(password_input,password_right)==0)
    {
        printf("������ȷ\n��ӭ %s ��¼\n��������� ",account);
        strcpy(user_logged,account);
        user_type=type;
        switch (user_type)
        {
        case 1:
            printf("");
            break;
        case 2:
            printf("");
            break;
        case 3:
            printf("");
            break;
        case 4:
            printf("");
            break;
        case 5:
            printf("");
            break;
           
        
        default:
        printf("��ȡ�û���Ϣ����");
        
            break;
        }
        return true;
    }
    else{
        printf("�������");
        return false;
    }
}

void pickup_package()
{

}

void inquire_package()
{

}

void pickup_instead_package()//��ȡ
{

}

void send_package()
{

}

void seelog()
{

}

void inquirestorage_storage()
{
    
}

