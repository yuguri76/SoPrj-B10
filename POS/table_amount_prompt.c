//
//  table_amount_prompt.c
//  core
//
//  Created by 이승윤 on 2023/06/05.
//

#include "table_amount_prompt.h"
#include "common_prompts.h"

extern int table_amount;

void table_amount_prompt(void) {
    while (table_amount < 0) {
        printf("POS / 테이블 개수 입력> ");
        table_amount = read_amount();
        if (table_amount == -1) {
            printf("오류 : 개수를 입력해주세요.\n");
        } else if (table_amount == -2) {
            printf("오류 : 개수는 다음과 같은 형식으로 입력할 수 있습니다. <횡공백류열0><개수><횡공백류열0>\n");
        } else if (table_amount == -3) {
            printf("오류 : 개수에 숫자가 아닌 것이 포함되어 있습니다. 개수는 숫자로만 입력할 수 있습니다.\n");
        } else if (table_amount == -4) {
            printf("오류 : 개수의 첫글자가 0입니다. 개수는 0으로 시작할 수 없습니다.\n");
        } else if (table_amount == -5) {
            printf("오류 : 개수는 1이상 20이하의 숫자여야 합니다.\n");
        }
    }
}
