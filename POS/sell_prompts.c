//
//  sell.c
//  POS
//
//  Created by 이승윤 on 2023/04/24.
//

#include "common_prompts.h"
#include "sell_prompts.h"
#include "payment_prompts.h"
#include "types.h"
#include "utils.h"

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


extern Table *tables; // 테이블
extern Product_Array all_products;

void sell_prompt() {
    int table_num;

    while (1) {
        table_num =-1;
        printf("<테이블 리스트>\n");
        printf("\t1. 1번 테이블\n");
        printf("\t2. 2번 테이블\n");
        printf("\t3. 3번 테이블\n");
        printf("\t4. 4번 테이블\n");
        printf("\t0. 돌아가기\n");

        printf("POS / 판매관리(테이블 관리) - 테이블 번호 선택 > ");
        
        int ret = command_prompt(4);
        if (ret == 0) {
            return;
        } else if (ret != -1) {
            table_management_prompt(ret);
        }
    }
}

void table_management_prompt(int table_num) {
    while(1) {
        printf("<%d번 테이블>\n", table_num);
        printf("\t1. 주문내역 출력\n");
        printf("\t2. 상품 주문\n");
        printf("\t3. 상품 취소\n");
        printf("\t4. 결제\n");
        printf("\t5. 테이블 합치기\n");
        printf("\t0. 돌아가기\n");

        printf("POS / %d번 테이블 - 테이블 번호 선택 > ", table_num);
        
        int ret = command_prompt(4);
        if (ret == 0) {
            return;
        } else if (ret == 1) {
            if (is_empty_table(table_num)) {
                printf("오류 : 주문된 상품이 없습니다.\n");
            } else {
                print_receipt(table_num);
            }
        } else if (ret == 2) {
            order_product(table_num);
        } else if (ret == 3) {
            if (is_empty_table(table_num)) {
                printf("오류 : 주문된 상품이 없습니다.\n");
            } else {
                cancel_order(table_num);
            }
        } else if (ret == 4) {
            if (is_empty_table(table_num)) {
                printf("오류 : 주문된 상품이 없습니다.\n");
            } else {
                process_payment(table_num);
            }
        }
    }
}

// 주문내역출력
void print_receipt(int table_num) {
    Table *table = &tables[table_num - 1];
    int total_price = 0;
    
    printf("<주문 내역>\n");
    printf("\t테이블 번호 : %d번\n", table_num);
    printf("\t주문 내역 : \n");
    
    // 주문 내역 출력
    for (int i = 0; i < table->length; i++) {
        if (table->products[i].amount == 0) continue;
        printf("\t%d.%s\t%lld\t%lld\n", i+1, table->products[i].name, table->products[i].price, table->products[i].amount);
        total_price += table->products[i].price * table->products[i].amount;
    }
    // 합계 금액 출력
    printf("\t합계 : %d원\n", total_price);
}

void order_product(int table_num) {
    int i;
    Product *product_to_order = NULL;
    
    while(1) {
        printf("<상품 주문>\n");
        printf("\t1. 상품명 입력\n");
        printf("\t0. 돌아가기\n");
        printf("POS / (상품 주문) - 번호 선택 > ");
        
        int ret = command_prompt(1);
        if (ret == 0) {
            return;
        } else if (ret == 1) {
            break;
        }
    }
    
    printf("<상품 주문>");
    printf("\t테이블 번호 %d번\n", table_num);
    printf("\t상품 목록:\n");
    for (i = 0; i < all_products.length; i++)
    {
        printf("%d. %s\t%lld\n", i + 1, all_products.products[i].name, all_products.products[i].price);
    }

    // 상품명 입력받기
    while (1) {
        printf("POS /(상품 주문) - 상품명 입력 > ");
        char *input = read_line();
        trim(input);
        remove_all_space(input);
        to_lower(input);
        for (i = 0; i < all_products.length; i++) {
            char comparing[16];
            strcpy(comparing, all_products.products[i].name);
            remove_all_space(comparing);
            to_lower(comparing);
            if (strcmp(input, comparing) == 0) {
                product_to_order = &all_products.products[i];
            }
        }

        if (product_to_order != NULL) {
            break;
        }

        printf("오류 : 상품목록에 없는 상품명입니다. 상품목록에 있는 상품명을 입력해주세요\n");
    }

    // 상품수량 입력받기
    int temp_amount;
    printf("상품을 몇개 주문 하시겠습니까?\n");
    while (1) {
        printf("POS /(상품 주문) - 상품 개수 입력 > ");
        temp_amount = read_amount();
        if (temp_amount == -1) {
            printf("오류 : 개수를 입력해주세요.\n");
        }
        else if (temp_amount == -2) {
            printf("오류 : 개수는 다음과 같은 형식으로 입력할 수 있습니다. <횡공백류열0><개수><횡공백류열0>\n");
        }
        else if (temp_amount == -3) {
            printf("오류 : 개수에 숫자가 아닌 것이 포함되어 있습니다. 개수는 숫자로만 입력할 수 있습니다.\n");
        }
        else if (temp_amount == -4) {
            printf("오류 : 개수의 첫글자가 0입니다. 개수는 0으로 시작할 수 없습니다.\n");
        }
        else if (temp_amount == -5) {
            printf("오류 : 개수는 1이상 20이하의 숫자여야 합니다.");
        }
        else 
            break;
    }
    
    while(1) {
        printf("정말로 주문하시겠습니까?\n");
        printf("\t1. 주문\n");
        printf("\t0. 돌아가기\n");
        printf("POS / 상품 주문 - 번호 선택 > ");
        int ret = command_prompt(1);
        if (ret == 0) {
            return;
        } else if (ret == 1) {

            break;
        }
    }

    // 주문목록에 추가
    add_order(table_num, product_to_order, temp_amount);
}

void add_order(int table_num, Product *order_product, int order_quantity) {
    Table *table = &tables[table_num - 1];
    int is_already_existing_order = 0; //테이블 내에 상품이 있는지 확인 .. 있으면 1, 없으면 0
    
    for (int j = 0; j < table->length; j++) {
        char tname[16];
        strcpy(tname, table->products[j].name);
        if (!strcmp(order_product->name, tname)) // 테이블 내에 상품이 이미 있음
        {    //한개 시켰다가 취소해서 0인경우 ?  ... 상관없음
            is_already_existing_order = 1;
            all_products.products[j].amount += order_quantity;
            table->products[j].amount += order_quantity; //주문한 상품의 개수 더해주기
            break;
        }
    }
    if (!is_already_existing_order) {
        if (table->length == 0) {  //주문내역이 아예 비어있음
            if ((table->products = malloc(sizeof(Product))) == NULL) { //메모리 부족으로 malloc 호출 실패하면 종료됨
                printf("오류 : 메모리 문제로 주문에 실패하였습니다. 이전 메뉴로 돌아갑니다.\n");
                return;
            }
            strcpy(table->products[0].name, order_product->name); //상품명 저장
            table->products[0].price = order_product->price; //가격 저장
            table->products[0].amount = order_quantity; //개수 저장
            table->length++; //다음 인덱스를 가리킴
        } else {
            void* realloced = realloc(table->products, (table->length + 1) * sizeof(Product));
            if (realloced == NULL) {
                printf("오류 : 메모리 문제로 주문에 실패하였습니다. 이전 메뉴로 돌아갑니다.\n");
                return;
            } else {
                table->products = realloced;
            }
            strcpy(table->products[table->length].name, order_product->name); //상품명 저장
            table->products[table->length].price = order_product->price; //가격 저장
            table->products[table->length].amount = order_quantity; //개수 저장
            table->length++;
        }
    }
}

void combine_Table(int table_num)
{
    bool loop1 = true;
    while (loop1)
    {
        printf("<%d번 테이블>", table_num);
        printf("1. 테이블 합치기\n");
        printf("0. 돌아가기\n");
        printf("POS / 번호 입력 : ");
        command_prompt(2);

        char choice[10];
        scanf("%s", choice);

        if (choice[0] == '0' || strcmp(choice, "back"))
        {
            loop1 = false;
        }
        else if (choice[0] == '1' || strcmp(choice, "one") == 0)
        {
            bool loop2 = true;
            while (true)
            {
                printf("<%d번 테이블>", table_num);
                printf("가능한 테이블 목록:\n");
                for (int i = 0; i < table_amount; i++)
                {
                    if (tables[i].status != kCombined)
                    {
                        printf("%d. %d번 테이블\n", i + 1, i + 1);
                    }
                }
                printf("\nPOS / 합칠 테이블 번호 입력: ");

                int selected_table = read_amount();

                if (selected_table < 1 || selected_table > table_amount || tables[selected_table - 1].status == kCombined)
                {
                    printf("존재하지 않는 테이블입니다.\n");
                    continue;
                }

                bool loop3 = true;
                while (loop3)
                {
                    printf("정말로 합치시겠습니까??\n");
                    printf("1. 합치기\n");
                    printf("0. 돌아가기\n");
                    printf("POS / 번호 입력 : ");
                    command_prompt(2);

                    char merge_choice[10];
                    scanf("%s", merge_choice);

                    if (merge_choice[0] == '0' || strcmp(merge_choice, "back") == 0)
                    {
                        loop3 = false;
                    }
                    else if (merge_choice[0] == '1' || strcmp(merge_choice, "one") == 0)
                    {
                        break;
                    }
                    else
                    {
                        printf("올바르지 않은 명령어입니다. 1 또는 0을 입력해주세요.\n");
                        continue;
                    }
                }
            }

            bool loop4 = true;
            while (loop4)
            {
                printf("계속하시겠습니까?\n");
                printf("1. 계속하기n");
                printf("0. 그만두기\n");
                printf("POS / 번호 입력 : ");
                command_prompt(2);

                char continue_choice[10];
                scanf("%s", continue_choice);

                if (continue_choice[0] == '1' || strcmp(continue_choice, "one") == 0)
                {
                    break;
                }
                else if (continue_choice[0] == '0' || strcmp(continue_choice, "back") == 0)
                {
                    return 0;
                }
                else
                {
                    printf("올바르지 않은 명령어입니다. 1 또는 0을 입력해주세요.\n");
                    continue;
                }
            }
        }
        else
        {
            printf("올바르지 않은 명령어입니다.\n");
            continue;
        }
    }
    return;
}