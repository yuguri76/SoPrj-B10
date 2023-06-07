#include "login.h"
#include "utils.h"

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

int latest_login_date;

extern char base_dir[];

int process_login(char date[]) {
    int ret, day = checkDate(date);
    if (day < 0) {
        return day;
    }
    if ((ret = makeFile(date)) != 0) {
        latest_login_date = ret;
        return -3;
    } else {
        return day;
    }
}

int makeFile(char date[]) {
    FILE* fp;
    int day = checkDate(date);
    char date_str[FILENAME_MAX];
    sprintf(date_str, "%s%08d", base_dir, day);
    int ret = checkFile(day);
    if (ret != 0) {
        return ret; // 마지막 정산일 이후의 파일이 아님
    }

    fp = fopen(date_str, "r");
    if (fp == NULL) {
        fp = fopen(date_str, "w");
        if (fp == NULL) {
            printf("치명적 오류");
            exit(1);
        }
        else {
            printf("데이터 파일 위치 %s에 정산파일을 성공적으로 생성했습니다: \n%s\n", base_dir, date_str);
            return 0;
        }
    }
    fclose(fp);
    return -1;
}

int checkFile(int date) {
    struct dirent** namelist;
    int count;
    int idx;
    int max = 0;

    if ((count = scandir(base_dir, &namelist, NULL, alphasort)) == -1) {
        printf("오류: 프로그램이 데이터 파일 위치를 파악할 수 없습니다. 프로그램을 종료합니다.");
        exit(EXIT_FAILURE);
    }

    for (idx = 0; idx < count; idx++) {
        if (checkDate(namelist[idx]->d_name) > 0) {
            if (max < checkDate(namelist[idx]->d_name))
                max = checkDate(namelist[idx]->d_name);
        }
    }

    for (idx = 0; idx < count; idx++) {
        free(namelist[idx]);
    }
    free(namelist);

    if (max < date) {
        return 0; // 마지막 정산일 이후의 로그인 일 때
    }
    else {
        return max; // 마지막 정산일 이후의 로그인이 아닐 때
    }
}
