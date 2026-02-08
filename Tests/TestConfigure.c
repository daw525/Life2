#include "./Unity/unity.h"
#include "../Source/configure.h"


#define MAX_TEST_CASE (22)

#define MAX_STRING (10)
typedef struct {
    char stringInt[MAX_STRING];
    int expectedReturnValue;
    bool expectedFailure;
    int actualReturnValue;
    bool actualFailure;
} unityTestCase;

unityTestCase unityTestCases[MAX_TEST_CASE] = { 
                                                /* Testing zero inputs*/
                                                {"0",0,false,999,true},
                                                {"00",0,false,999,true},
                                                {"000",0,false,999,true},
                                                {"0000",999,true,999,false},    /* Too many characters */
                                                /* Testing negative inputs - negative values are not supported */
                                                {"-0",999,true,999,false},
                                                {"-1",999,true,999,false},
                                                {"-100",999,true,999,false},
                                                {"-1000",999,true,999,false},
                                                /* Testing max inputs*/
                                                {"9",9,false,0,true},
                                                {"99",99,false,0,true},
                                                {"999",999,false,0,true},
                                                {"9999",999,true,999,false},
                                                /* Leading zeros */
                                                {"001",1,false,888,true},
                                                {"010",10,false,888,true},
                                                {"100",100,false,888,true},
                                                {"1010",888,true,888,false},
                                                /* Invalid data */
                                                {"A",222,true,222,false},
                                                {"1A",555,true,555,false},
                                                {"11A",777,true,777,false},
                                                {"111A",0,true,0,false},    /* Invalid character even after 3 good characters */
                                                /* Specific scenarios from real world failures */
                                                {"10",10,false,0,true},  /* This was being calculated as 1 instead of 10 */
                                                {"",10,true,10,false}    /* Null string was passing but should fail*/

};

void setUp(void) {
     
}

void tearDown(void) {

}

void test_convertStringToInt(void) {
    int testCase;
    #define MAX_MESSAGE (10)
    char message[MAX_MESSAGE];

    for(testCase=0;testCase<MAX_TEST_CASE;testCase++) {
        sprintf(message,"Test %i\0",testCase);
        unityTestCases[testCase].actualFailure = convertStringToInt(unityTestCases[testCase].stringInt, &unityTestCases[testCase].actualReturnValue);
        TEST_ASSERT_EQUAL_MESSAGE(unityTestCases[testCase].expectedFailure, unityTestCases[testCase].actualFailure,message);
        TEST_ASSERT_EQUAL_MESSAGE(unityTestCases[testCase].expectedReturnValue, unityTestCases[testCase].actualReturnValue,message);
    }
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_convertStringToInt);
    return UNITY_END();
}