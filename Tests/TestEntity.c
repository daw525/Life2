#include "./Unity/unity.h"
#include "../Source/entity.h"

typedef struct {
    entity actual;
    entity expected;
} testCase;

testCase test;

void setUp(void) {

}

void tearDown(void) {

}


int main(void) {
    UNITY_BEGIN();

    return UNITY_END();
}