#include <stdio.h>
#include <string.h>
#include <stdlib.h>   // for system()

#define N 100

struct birth {
    int year;
    int month;
    int day;
};

struct person {
    char ID[19];
    char name[20];
    struct birth birthday;
    int flag;   // 1:校验正确, 0:校验错误
};

// 读取文件，返回记录数
int read(struct person p[]) {
    FILE *fp = fopen("person.txt", "r");
    if (fp == NULL) {
        printf("无法打开 person.txt\n");
        return 0;
    }
    int i = 0;
    while (i < N && fscanf(fp, "%s %s", p[i].ID, p[i].name) == 2) {
        i++;
    }
    fclose(fp);
    return i;
}

// 校验身份证校验位，正确返回1，错误返回0
int checkID(char *ID) {
    int weight[] = {7, 9, 10, 5, 8, 4, 2, 1, 6, 3, 7, 9, 10, 5, 8, 4, 2};
    char check[] = "10X98765432";
    int sum = 0;
    for (int i = 0; i < 17; i++) {
        if (ID[i] < '0' || ID[i] > '9') return 0;
        sum += (ID[i] - '0') * weight[i];
    }
    int mod = sum % 11;
    char last = ID[17];
    if (last >= 'a' && last <= 'z') last -= 32;
    return (last == check[mod]) ? 1 : 0;
}

// 从身份证中提取出生日期
struct birth get_birth(char *ID) {
    struct birth b;
    b.year = (ID[6] - '0') * 1000 + (ID[7] - '0') * 100 + (ID[8] - '0') * 10 + (ID[9] - '0');
    b.month = (ID[10] - '0') * 10 + (ID[11] - '0');
    b.day = (ID[12] - '0') * 10 + (ID[13] - '0');
    return b;
}

// 填充所有人的出生日期和校验标志
void get_all_person(struct person p[], int n) {
    for (int i = 0; i < n; i++) {
        p[i].birthday = get_birth(p[i].ID);
        p[i].flag = checkID(p[i].ID);
    }
}

// 显示人员信息，flag=1显示正确，flag=0显示错误
void display_person(struct person p[], int n, int flag) {
    int count = 0;
    if (flag == 1) {
        printf("\n身份证号码            姓名      出生日期\n");
        printf("----------------------------------------\n");
        for (int i = 0; i < n; i++) {
            if (p[i].flag == 1) {
                printf("%-18s  %-8s  %d年%d月%d日\n",
                       p[i].ID, p[i].name,
                       p[i].birthday.year, p[i].birthday.month, p[i].birthday.day);
                count++;
            }
        }
    } else {
        printf("\n身份证号码            姓名\n");
        printf("--------------------------------\n");
        for (int i = 0; i < n; i++) {
            if (p[i].flag == 0) {
                printf("%-18s  %-8s\n", p[i].ID, p[i].name);
                count++;
            }
        }
    }
    printf("--------------------------------\n");
    printf("共 %d 人\n\n", count);
}

// 比较两个生日
int birth_cmp(struct birth birth1, struct birth birth2) {
    if (birth1.year != birth2.year)
        return birth1.year - birth2.year;
    if (birth1.month != birth2.month)
        return birth1.month - birth2.month;
    return birth1.day - birth2.day;
}

// 按出生日期排序（年龄从大到小）
void birth_sort(struct person p[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (birth_cmp(p[j].birthday, p[j+1].birthday) > 0) {
                struct person tmp = p[j];
                p[j] = p[j+1];
                p[j+1] = tmp;
            }
        }
    }
}

// 在正确信息中查找同名人数
int search(struct person p[], int n, char *name) {
    int cnt = 0;
    for (int i = 0; i < n; i++) {
        if (p[i].flag == 1 && strcmp(p[i].name, name) == 0) {
            cnt++;
        }
    }
    return cnt;
}

// 将正确信息写入文件
void save(struct person p[], int n) {
    FILE *fp = fopen("person_checked.txt", "w");
    if (fp == NULL) {
        printf("无法创建文件 person_checked.txt\n");
        return;
    }
    for (int i = 0; i < n; i++) {
        if (p[i].flag == 1) {
            fprintf(fp, "%-18s %-8s %d年%d月%d日\n",
                    p[i].ID, p[i].name,
                    p[i].birthday.year, p[i].birthday.month, p[i].birthday.day);
        }
    }
    fclose(fp);
    printf("正确信息已保存到 person_checked.txt\n");
}

// 菜单显示
int prompt(void) {
    int cmd = 0;
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

int main(void) {
    // 自动将控制台代码页设置为简体中文 GBK
    system("chcp 936 > nul");

    struct person p[N];
    int n = read(p);
    if (n == 0) {
        printf("没有读取到任何数据，程序退出。\n");
        return 1;
    }
    get_all_person(p, n);

    int cmd;
    while (1) {
        cmd = prompt();
        switch (cmd) {
            case 1:
                display_person(p, n, 1);
                break;
            case 2:
                display_person(p, n, 0);
                break;
            case 3: {
                struct person correct[N];
                int cnt = 0;
                for (int i = 0; i < n; i++) {
                    if (p[i].flag == 1) {
                        correct[cnt++] = p[i];
                    }
                }
                if (cnt == 0) {
                    printf("没有正确的身份证信息。\n");
                } else {
                    birth_sort(correct, cnt);
                    display_person(correct, cnt, 1);
                }
                break;
            }
            case 4: {
                char name[20];
                printf("请输入要查找的姓名：");
                scanf("%s", name);
                int num = search(p, n, name);
                printf("有 %d 个人名为 %s\n\n", num, name);
                break;
            }
            case 0:
                save(p, n);
                printf("程序退出。\n");
                return 0;
            default:
                printf("无效输入，请重新选择。\n");
                break;
        }
    }
    return 0;
}