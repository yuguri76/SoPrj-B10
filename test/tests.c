#include <stdlib.h>
#include <string.h>
#include <unity.h>
#include <utils.h>

void setUp(void) {
    
}
void tearDown(void) {
    
}

char* trim_str(char* str) {
    int len = strlen(str);
    char* string = malloc(sizeof(char) * len);
    strcpy(string, str);
    trim(string);
    return string;
}

void test_check_correct_command(void) {
    TEST_ASSERT_EQUAL_INT(0, is_correct_command("0"));
    TEST_ASSERT_EQUAL_INT(0, is_correct_command("back"));
    TEST_ASSERT_EQUAL_INT(1, is_correct_command("1"));
    TEST_ASSERT_EQUAL_INT(1, is_correct_command("one"));
    TEST_ASSERT_EQUAL_INT(2, is_correct_command("2"));
    TEST_ASSERT_EQUAL_INT(2, is_correct_command("two"));
    TEST_ASSERT_EQUAL_INT(3, is_correct_command("3"));
    TEST_ASSERT_EQUAL_INT(3, is_correct_command("three"));
    TEST_ASSERT_EQUAL_INT(4, is_correct_command("4"));
    TEST_ASSERT_EQUAL_INT(4, is_correct_command("four"));
    
    TEST_ASSERT_EQUAL_INT(-1, is_correct_command("asd"));
    TEST_ASSERT_EQUAL_INT(-1, is_correct_command("6"));
    TEST_ASSERT_EQUAL_INT(-1, is_correct_command("seven"));
}

void test_trim(void) {
    TEST_ASSERT_EQUAL_STRING("", trim_str(""));
    TEST_ASSERT_EQUAL_STRING("", trim_str("      "));
    TEST_ASSERT_EQUAL_STRING("", trim_str("\t\t\t\t\t\t\t"));
    TEST_ASSERT_EQUAL_STRING("", trim_str(" \t\t\t \t\t    \t\t \t"));
    TEST_ASSERT_EQUAL_STRING("abcd123", trim_str("abcd123"));
    TEST_ASSERT_EQUAL_STRING("abc /t/td123", trim_str("abc /t/td123"));
    TEST_ASSERT_EQUAL_STRING("abc /t/td123", trim_str("abc /t/td123     "));
    TEST_ASSERT_EQUAL_STRING("abc /t/td123", trim_str("abc /t/td123\t\t\t"));
    TEST_ASSERT_EQUAL_STRING("abc /t/td123", trim_str("abc /t/td123\t \t "));
    TEST_ASSERT_EQUAL_STRING("abc /t/td123", trim_str("   abc /t/td123"));
    TEST_ASSERT_EQUAL_STRING("abc /t/td123", trim_str("\t\tabc /t/td123"));
    TEST_ASSERT_EQUAL_STRING("abc /t/td123", trim_str("\t\tabc /t/td123"));
    TEST_ASSERT_EQUAL_STRING("!!@!@", trim_str(" !!@!@ "));
    TEST_ASSERT_EQUAL_STRING("abc!", trim_str(" \t\t\t \t\t  abc!  \t\t \t"));
}

void test_isAlpha(void) {
    TEST_ASSERT_EQUAL_INT(1, is_alpha('a'));
    TEST_ASSERT_EQUAL_INT(0, is_alpha('1'));
    TEST_ASSERT_EQUAL_INT(0, is_alpha('!'));
    TEST_ASSERT_EQUAL_INT(0, is_alpha(' '));
    TEST_ASSERT_EQUAL_INT(0, is_alpha('\t'));
}

void test_is_contain_non_number(void) {
    TEST_ASSERT_EQUAL_INT(0, is_contain_non_number("12498"));
    TEST_ASSERT_EQUAL_INT(1, is_contain_non_number("a123s"));
    TEST_ASSERT_EQUAL_INT(1, is_contain_non_number("-1="));
    TEST_ASSERT_EQUAL_INT(1, is_contain_non_number("가나13"));
}

//void test_isNum
 
int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_trim);
    RUN_TEST(test_isAlpha);
    RUN_TEST(test_check_correct_command);
    RUN_TEST(test_is_contain_non_number);
    return UNITY_END();
}
