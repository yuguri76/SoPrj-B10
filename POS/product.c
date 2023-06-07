#include "product.h"
#include "types.h"
#include "utils.h"

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <dirent.h>
#include <sys/stat.h>

#define PRODUCTFILE "product.txt"

extern Table *tables; // 테이블
extern int table_amount;
extern Product_Array all_products; // 모든 상품들의 목록을 포함한 구조체
extern char base_dir[];

int read_product_file(void){
    
    char datafile_dir[FILENAME_MAX];
    sprintf(datafile_dir ,"%s%s", base_dir, PRODUCTFILE);
    
    int ret = mkdir(base_dir, 0777);
    if (ret == 0) {
        printf("오류: 데이터 폴더 %s가 없습니다.\n", base_dir);//base_dir -> datafile_dir
        printf("데이터 폴더를 새로 생성했습니다.\n");
    } if(ret == -1 && errno != EEXIST) {
        printf("오류: 데이터 폴더 %s가 없습니다.\n", datafile_dir);
        printf("오류: 데이터 폴더를 생성하지 못했습니다. 프로그램을 종료합니다.\n");
        exit(EXIT_FAILURE);
    }
    int c;
    int err = 0;
    FILE* fp;
    fpos_t p;
    
//    strcpy(datafile_dir, base_dir);
//    strcat(datafile_dir, PRODUCTFILE);
    
    struct dirent** namelist;
    int count;
    
    if ((count = scandir(base_dir, &namelist, NULL, alphasort)) == -1) {
        printf("오류: 프로그램이 데이터 파일 위치를 파악할 수 없습니다. 프로그램을 종료합니다.");
        exit(EXIT_FAILURE);
    }
    
    int file_exist = 0;
    for (int idx = 0; idx < count; idx++) {
        if (strcmp(namelist[idx]->d_name, PRODUCTFILE) == 0) {
            file_exist = 1;
            break;
        }
    }
    
    if (!file_exist) {
        printf("오류 : 데이터 파일 위치 %s에 상품파일이 없습니다.\n", base_dir);
        fp = fopen(datafile_dir, "w+");
        if (fp) {
            printf("데이터 파일 위치에 빈 상품파일을 새로 생성했습니다:\n%s\n", datafile_dir);
        } else {
            printf("오류 : 데이터 파일 위치에 상품파일을 생성하지 못했습니다. 프로그램을 종료합니다.\n");
            exit(EXIT_FAILURE);
        }
        fclose(fp);
    }
    
    if ((fp = fopen(datafile_dir, "r+")) == NULL) {
        printf("오류 : 상품파일 %s에 대한 입출력 권한이 없습니다. 프로그램을 종료합니다.\n", datafile_dir);
        exit(EXIT_FAILURE);
    }
    
    table_amount = read_table_amounts_line(fp);

    while(1){
        fgetpos(fp, &p);//잘못된 입력 출력하기 위해
        switch(c = read_product_line(fp)){//각 행을 읽어서 올바른 입력인지 파악
            case 2: // 마지막 행
                fclose(fp);
                if(err) {
                    return -1;
                }
                return 0;
            case 1: // 상품레코드
            case 0: // 횡공백류열
                break;
            case -1:
                err = 1;
                fsetpos(fp, &p);
                printf("오류 : 상품명과 상품가격이 레코드 형식으로 작성되지 않았습니다. 상품레코드 형식은 <횡공백류열0><상품명><횡공백류열tab><상품가격><횡공백류열0>입니다.\n");
                while((c = fgetc(fp)) != '\n'){ //c?
                    if(c == EOF){
                        printf("\n");
                        fclose(fp);
                        return -1;
                    }
                    putchar(c);
                }
                putchar(c);
                break;

            case -2:
                err = 1;
                fsetpos(fp, &p);
                printf("오류 : 상품명에 알파벳이 아닌 것이 포함되어 있습니다. 상품명은 알파벳으로만 입력할 수 있습니다.\n");
                while((c = fgetc(fp)) != '\n'){
                    if(c == EOF){
                        printf("\n");
                        fclose(fp);
                        return -1;
                    }
                    putchar(c);
                }
                putchar(c);
                break;

            case -3:
                err = 1;
                fsetpos(fp, &p);
                printf("오류 : 상품명 길이가 15글자를 초과했습니다. 상품명은 1~15글자의 알파벳 및 표준공백(스페이스바)으로만 설정할 수 있습니다.\n");
                while((c = fgetc(fp)) != '\n'){
                    if(c == EOF){
                        printf("\n");
                        fclose(fp);
                        return -1;
                    }
                    putchar(c);
                }
                putchar(c);
                break;

            case -4:
                err = 1;
                fsetpos(fp, &p);
                printf("오류 : 상품가격에 숫자가 아닌 것이 포함되어 있습니다. 상품가격은 숫자로만 입력할 수 있습니다.\n");
                while((c = fgetc(fp)) != '\n'){
                    if(c == EOF){
                        printf("\n");
                        fclose(fp);
                        return -1;
                    }
                    putchar(c);
                }
                putchar(c);
                break;

            case -5:
                err = 1;
                fsetpos(fp, &p);
                printf("오류 : 상품가격의 첫글자가 0입니다. 상품 가격은 0으로 시작할 수 없습니다.\n");
                while((c = fgetc(fp)) != '\n'){
                    if(c == EOF){
                        printf("\n");
                        fclose(fp);
                        return -1;
                    }
                    putchar(c);
                }
                putchar(c);
                break;

            case -6:
                err = 1;
                fsetpos(fp, &p);
                printf("오류 : 상품가격은 100으로 나누었을 때 나머지가 0이 아닙니다. 상품가격은 100단위로 정해야합니다.\n");
                while((c = fgetc(fp)) != '\n'){
                    if(c == EOF){
                        printf("\n");
                        fclose(fp);
                        return -1;
                    }
                    putchar(c);
                }
                putchar(c);
                break;

            case -7:
                err = 1;
                fsetpos(fp, &p);
                printf("오류 : 상품가격이 100만원 이상입니다. 상품가격은 100만원보다 작게 정해야합니다.\n");
                while((c = fgetc(fp)) != '\n'){
                    if(c == EOF){
                        printf("\n");
                        fclose(fp);
                        return -1;
                    }
                    putchar(c);
                }
                putchar(c);
                break;
        }
    }
}

int read_product_line(FILE* fp){
    int length, tab = 0;
    char c, name[16];
    int price = 0;

    while((c=getc(fp)) == ' ' || c == '\t') //횡공백류열 제거
        ;

    if(c == '\n')//횡공백류열만 입력한 행
        return 0;
    if(c == EOF)//파일 끝
        return 2;

    if(!isalpha(c))//상품명에 알파벳이 아닌 글자 포함
        return -2;
    ungetc(c, fp);

    for(length=0; length<16; length++){//최대 15글자까지 읽음
        c = getc(fp);
        if(c == '\t'){ // \t읽으면 break
            tab = 1;
            break;
        }
        if(isalpha(c) || c == ' '){ //상품명은 알파벳 or 표준공백
            name[length] = c;
        }else{ //위의 둘이 아니라면 에러
            return -2;
        }
    }

    if(length == 16) // 길이가 16이 되었는데
        if(isalpha(c))//마지막 문자가 알파벳이면 길이 초과
            return -3;

    for(;name[length-1] == ' ';length--)
        ;//상품명 뒤에오는 표준공백 삭제
    name[length] = '\0';

    if(!tab){ //앞에서 탭을 못발견했으면
        while((c=getc(fp)) != '\t'){//탭나올때까지 읽고
            if(c == '\n' || c == EOF)//탭보다 먼저 나오면 에러
                return -1;
        }
    }

    while((c=getc(fp)) == ' ' || c == '\t')//횡공백류열 제거
        ;
    if(!isdigit(c))//상품가격이 숫자가 아니면 에러
        return -4;
    if(c == '0')//첫글자가 0이면 에러
        return -5;
    ungetc(c, fp);

    for(length=0; length<7; length++){
        c = getc(fp);
        if(isdigit(c)){ //상품가격은 숫자
            price *= 10;
            price += c-'0';
        }else
            break;
    }
    if(length == 7) // 길이가 8이 되었는데
        if(isdigit(c))//마지막 문자가 숫자이면 길이 초과
            return -7;
    if(c != ' ' && c != '\t' && c != '\n' && c != EOF)
        return -4;//상품가격 읽다가 위 네 조건 아니고 빠져나오면 에러
    ungetc(c, fp);
    if(price%100)//가격이 100단위가 아님
        return -6;

    while((c=getc(fp)) == ' ' || c == '\t')//횡공백류열 제거
        ;
    if(c != '\n' && c != EOF)//이 두 개가 아닌 다른게 나오면
        return -1;            //에러
    else{
        add_product(name, price);//모든 에러 넘겼으니 상품추가
        if(c=='\n')//다음 행
            return 1;
        else        //마지막 행
            return 2;
    }
}

int productcmp(char* name){//상품명 인수로 받아서 all_product.list랑 동치비교
    int i, j, k;
    for(i=0; i<all_products.length; i++){
        for(j = k = 0; name[j] || all_products.products[i].name[k]; ){
            if(name[j] == ' '){//공백 무시
                j++;
                continue;
            }
            if(all_products.products[i].name[k] == ' '){//공백 무시
                k++;
                continue;
            }
            if(tolower(name[j])!=tolower(all_products.products[i].name[k]))//대소문자 구분 X
                break;
            j++;
            k++;
            }
        if(!name[j] && !all_products.products[i].name[k]){
            return i;//둘 다 \0까지 도달하면 인덱스 반환
        }
    }
    return -1;//동치인 인덱스 발견하지 못하면 -1 반환
}

void add_product(char* name, int price){
    int idx;
    
    if ((idx=productcmp(name))>=0) { //productcmp로 동치비교를 해서 동치인 인덱스 반환
        if (all_products.products[idx].amount > 0) {
            printf("오류 : 오늘 이미 판매된적 있는 상품입니다. 상품정보를 변경할 수 없습니다.\n");
            return;
        }
        //sprintf(all_products.products[idx].name, "%s", name);
        strcpy(all_products.products[idx].name, name);//동치인 Product 있으면
        all_products.products[idx].price = price;    //가격 이름 갱신
        return;
    } else {// 동치인 상품명 없으면
        
        all_products.products = realloc(all_products.products, (all_products.length + 1)* sizeof(Product));
        if(!all_products.products){//할당 실패
            printf("오류 : 메모리 부족으로 상품 추가에 실패하였습니다.\n");
            return;
        }
        //sprintf(all_products.products[all_products.length].name,"%s", name);
        strcpy(all_products.products[all_products.length].name, name);
        all_products.products[all_products.length].price = price;
        all_products.products[all_products.length].amount = 0;
        all_products.length++;//all_product.list에 Product 할당
    }
}

void write_product_file(void){
    int i;
    FILE* fp;
    
    char datafile_dir[FILENAME_MAX];
    sprintf(datafile_dir ,"%s%s", base_dir, PRODUCTFILE);

    fp = fopen(datafile_dir, "w");
    
    if(!fp){
        printf("오류 : 데이터 파일 위치에 상품파일을 생성하지 못했습니다. 프로그램을 종료합니다.\n"); //???
        exit(EXIT_FAILURE);
    }
    
    // 테이블 개수
    fprintf(fp, "%d\n", table_amount);
    
    // 상품
    for(i=0; i<all_products.length; i++){
        fprintf(fp, "%-15s\t\t%lld\n", all_products.products[i].name, all_products.products[i].price);
    }
    fclose(fp);
}

int read_table_amounts_line(FILE* fp) {
    char tmp;
    int amount;
    
    // EOF Check
    if ((tmp = getc(fp)) == EOF) {
        return -1;
    } else {
        ungetc(tmp, fp);
    }

    char* line = read_line_f(fp);
    if (strlen(line) == 0){
        printf("오류 : 테이블 개수가 입력되어있지 않습니다.\n");
        printf("%s\n", line);
        exit(EXIT_FAILURE);
    } else if (is_contain_non_number(line)) {
        printf("오류 : 테이블 개수는 숫자로만 입력될 수 있습니다.\n");
        printf("%s\n", line);
        exit(EXIT_FAILURE);
    } else if (line[0] == '0') {
        if (strlen(line) == 1) {
            printf("오류 : 테이블 개수는 0일 수 없습니다.\n");
        } else {
            printf("오류 : 테이블 개수는 0으로 시작될 수 없습니다.\n");
        }
        printf("%s\n", line);
        exit(EXIT_FAILURE);
    } else if ((amount = atoi(line)) > 20) {
        printf("오류 : 테이블 개수는 20을 넘을 수 없습니다.\n");
        printf("%s\n", line);
        exit(EXIT_FAILURE);
    }
    free(line);
    return amount;
}
