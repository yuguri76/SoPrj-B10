#include "add_product_prompts.h"
#include "common_prompts.h"
#include "main_prompts.h"
#include "sell_prompts.h"
#include "utils.h"
#include "product.h"
#include "settlement.h"

#include <stdlib.h>
#include <string.h>

extern Table *tables; // 테이블
extern Product_Array all_products; // 모든 상품들의 목록을 포함한 구조체
extern int date;

void main_prompt() {
    while (1) {
        printf("<메인 메뉴>\n");
        printf("\t1. 판매 관리\n");
        printf("\t2. 날짜별 정산\n");
        printf("\t3. 상품 관리\n");
        printf("\t4. 종료\n");

        printf("POS / 메인 메뉴 - 번호 선택 > ");
        int ret = command_prompt_b(4, 0);
        if (ret == 1) {
            if (all_products.length) {
                sell_prompt();
            } else {
                printf("오류 : 상품추가를 먼저 해주세요.\n");
            }
        } else if (ret == 2) {
            settlement_prompt();
        } else if (ret == 3) {
            product_management_prompt();
        } else if (ret == 4) {
            if (exit_check() == 0) {
                write_product_file();
                save_settlement();
                return;
            } else {
                printf("오류 : 모든 테이블에 결제가 끝나지 않아 종료가 불가능합니다.\n");
            }
        }
    }
}

int exit_check() {
    for (int i = 0; i < 4; i++) {
        Table *table = &tables[i];
        for (int j = 0; j < table->length; j++) {
            if (table->products[j].amount != 0) {
                return -1;
            }
        }
    }
    return 0;
}
