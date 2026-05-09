#include <stdio.h>
#include <string.h>
#define N 100
struct birth
{
    int     year;
    int     month;
    int     day;
};
struct person
{
    char    ID[19];
    char    name[20];
    struct  birth  birthday;
    int flag;//校验位是否正确, 1代表正确，0代表错误
};

int read(struct person p[])
{
   
}

int checkID(char *ID)
{

}

struct birth get_birth(char *ID)
{

}

void get_all_person(struct person p[], int n)
{

}

void display_person(struct person p[], int n, int flag)
{

}

int birth_cmp(struct birth birth1, struct birth birth2)
{

}

void birth_sort(struct person p[], int n)
{

}

int search(struct person p[], int n, char *name)
{

}

void save(struct person p[], int n)
{

}

int prompt(void)
{
    int cmd=0;
    printf("-------------------\n");
    printf("1.  公民信息显示（不含错误信息）\n");
    printf("2.  错误信息查询\n");
    printf("3.  按出生日期排序（不含错误信息）\n");
    printf("4.  重名查询（不含错误信息）\n");
    printf("0.  退出\n");
    printf("-------------------\n");
    printf("请输入你的选择(0-4):");
    scanf("%d", &cmd);
    return cmd;
}

int main(void)
{
    struct person p[N];
    int cmd, n;
    while (1)
    {   cmd = prompt();


    } 
    return 0;
}
