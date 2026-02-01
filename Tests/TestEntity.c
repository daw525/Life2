#include "./Unity/unity.h"
#include "../Source/entity.h"

typedef struct {
    entity actual;
    entity expected;
} testCase;

testCase test;

void setUp(void) {
    int sample;
    test.actual.input = true;
    test.actual.output = true;
    test.actual.previousOutput = true;
    for (sample = 0; sample < MAX_SAMPLE; sample++)
    {
        test.actual.inputs[sample] = true;
        test.actual.outputs[sample] = true;
    }
    test.actual.INTEGRATION_TIME = 0x55AA;
    test.actual.integrationTime = 0x55AA;
    test.actual.inputTimeTrue = 0x55AA;
    test.actual.outputTimeTrue = 0x55AA;
    test.actual.outputTimeFalse = 0x55AA;
    test.actual.error = 0x55AA;
    test.actual.trueWeight = 9.999;
    test.actual.falseWeight = 99.999;
    test.actual.FLIP_TIME = 0x55AA;
    test.actual.flipTime = 0x55AA;

    test.expected.input = false;
    test.expected.output = false;
    test.expected.previousOutput = false;
    for (sample = 0; sample < MAX_SAMPLE; sample++)
    {
        test.expected.inputs[sample] = false;
        test.expected.outputs[sample] = false;
    }
    test.expected.INTEGRATION_TIME = 0;
    test.expected.integrationTime = 0;
    test.expected.inputTimeTrue = 0;
    test.expected.outputTimeTrue = 0;
    test.expected.outputTimeFalse = 0;
    test.expected.error = 0;
    test.expected.trueWeight = 0;
    test.expected.falseWeight = 0;
    test.expected.FLIP_TIME = 0;
    test.expected.flipTime = 0;
}

void tearDown(void) {

}

void test_initialiseEntityPrivateState(void) {
    int sample;
    initialiseEntity(&test.actual,test.expected.INTEGRATION_TIME,test.expected.FLIP_TIME);

    TEST_ASSERT_EQUAL(test.expected.input,test.actual.input);
    TEST_ASSERT_EQUAL(test.expected.output,test.actual.output);
    TEST_ASSERT_EQUAL(test.expected.previousOutput,test.actual.previousOutput);
    for (sample = 0; sample < MAX_SAMPLE; sample++)
    {
        TEST_ASSERT_EQUAL(test.expected.inputs[sample],test.actual.inputs[sample]);
        TEST_ASSERT_EQUAL(test.expected.outputs[sample],test.actual.outputs[sample]);
    }
    TEST_ASSERT_EQUAL(test.expected.INTEGRATION_TIME,test.actual.INTEGRATION_TIME);
    TEST_ASSERT_EQUAL(test.expected.integrationTime,test.actual.integrationTime);
    TEST_ASSERT_EQUAL(test.expected.inputTimeTrue,test.actual.inputTimeTrue);
    TEST_ASSERT_EQUAL(test.expected.outputTimeTrue,test.actual.outputTimeTrue);
    TEST_ASSERT_EQUAL(test.expected.outputTimeFalse,test.actual.outputTimeFalse);
    TEST_ASSERT_EQUAL(test.expected.error,test.actual.error);
    TEST_ASSERT_EQUAL_FLOAT(test.expected.trueWeight,test.actual.trueWeight);
    TEST_ASSERT_EQUAL_FLOAT(test.expected.falseWeight,test.actual.falseWeight);
    TEST_ASSERT_EQUAL(test.expected.FLIP_TIME,test.actual.FLIP_TIME);
    TEST_ASSERT_EQUAL(test.expected.flipTime,test.actual.flipTime);

}

void test_initaliseEntityWithInRangeParameters(void) {

    /* Test in range parameters */
    test.expected.INTEGRATION_TIME = 5;
    test.expected.FLIP_TIME = 3;

    /* Test out of range parameters */
    /* This should fail as no input validation has been added yet */
    test.expected.INTEGRATION_TIME = (MAX_SAMPLE + 1);
    test.expected.FLIP_TIME = -1;

    initialiseEntity(&test.actual,test.expected.INTEGRATION_TIME,test.expected.FLIP_TIME);
 
    TEST_ASSERT_EQUAL_INT(test.expected.INTEGRATION_TIME,test.actual.INTEGRATION_TIME);
    TEST_ASSERT_EQUAL_INT(test.expected.FLIP_TIME,test.actual.FLIP_TIME);
}

void test_initaliseEntityWithOutOfRangeParameters(void) {

    /* This should fail as no input validation has been added yet */
    test.expected.INTEGRATION_TIME = (MAX_SAMPLE + 1);
    test.expected.FLIP_TIME = -1;

    initialiseEntity(&test.actual,test.expected.INTEGRATION_TIME,test.expected.FLIP_TIME);

    TEST_ASSERT_GREATER_OR_EQUAL_INT(0,test.actual.INTEGRATION_TIME);
    TEST_ASSERT_LESS_THAN_INT(MAX_SAMPLE,test.actual.INTEGRATION_TIME);

    TEST_ASSERT_GREATER_OR_EQUAL_INT(0,test.actual.FLIP_TIME);
    TEST_ASSERT_LESS_THAN_INT(100,test.actual.FLIP_TIME);
    /* No maximum defined for flip time. Currently testing it can't go negative */
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_initialiseEntityPrivateState);
    RUN_TEST(test_initaliseEntityWithInRangeParameters);
    RUN_TEST(test_initaliseEntityWithOutOfRangeParameters);
    return UNITY_END();
}