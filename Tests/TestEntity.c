#include "./Unity/unity.h"
#include "../Source/entity.h"

void setUp(void) {

}

void tearDown(void) {

}

void test_MyTest(void) {
    TEST_ASSERT_EQUAL_INT(1,1);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_MyTest);
    return UNITY_END();
}