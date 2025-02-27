#include <stdio.h>
#include <string.h>
char user_logged[20];
int user_type;
// 我打算建立全局变量记录用户名和用户类型 用户类型用数字表示
void welcome()
{
    printf("##################################################\n欢迎使用快递驿站管理系统\nWelcome to use Courier station management system\n##################################################\n");
}

bool findaccount(char account[],char password[],int* type)//输入account的值，如果有相应用户，password和type作为参数返回值，方便比对
{

}

bool fetch_user()
{
    printf("请输入用户账号\n");
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
    printf("请输入用户密码\n");
    fgets(password_input,39,stdin);
    if(strcmp(password_input,password_right)==0)
    {
        printf("密码正确\n欢迎 %s 登录\n您的身份是 ",account);
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
        printf("读取用户信息错误");
        
            break;
        }
        return true;
    }
    else{
        printf("密码错误");
        return false;
    }
}

void pickup_package()
{

}

void inquire_package()
{

}

void pickup_instead_package()//代取
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

