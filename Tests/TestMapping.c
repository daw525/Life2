#include "./Unity/unity.h"
#include "../Source/mapping.h"

typedef struct {
    bool testEnabled;
    mapping m;
    bool expectedEvaluation;
    bool expectedOutput;
    bool output;
    bool expectedFailure;
} testCase;

static bool POINTER_TO_TRUE_INPUT = true;
static bool POINTER_TO_FALSE_INPUT = false;

void setUp(void) {
     
}

void tearDown(void) {

}

void test_evaluateMappingThresholdInput(void) {
    #define MAX_TEST_CASE (4)
    testCase tests[MAX_TEST_CASE];
    int testCase;
    #define MAX_MESSAGE (10)
    char message[MAX_MESSAGE];

    port tempPort;

    testCase = 0;

    /* One input, true, expect evaluation to true */
    tests[testCase].testEnabled = true;
    initialiseMapping(&tests[testCase].m, THRESHOLD);
    initialisePort(&tempPort, &tests[testCase].output, false);
    addPortToMapping(&tests[testCase].m, &tempPort, false);
    
    initialisePort(&tempPort, &POINTER_TO_TRUE_INPUT, false);
    addPortToMapping(&tests[testCase].m, &tempPort, true);
    
    setMappingThresholds(&tests[testCase].m, 1, 1);
    tests[testCase].expectedEvaluation = true;
    tests[testCase].expectedOutput = true; 
    tests[testCase].output = !(tests[testCase].expectedOutput); /* Initiaise to inverse of expected result so there is a change to observe */
    tests[testCase].expectedFailure = false;

    testCase++;

    /* One input, false, expect evaluation to false */
    tests[testCase].testEnabled = true;
    initialiseMapping(&tests[testCase].m, THRESHOLD);
    initialisePort(&tempPort, &tests[testCase].output, false);
    addPortToMapping(&tests[testCase].m, &tempPort, false);
    initialisePort(&tempPort, &POINTER_TO_FALSE_INPUT, false);
    addPortToMapping(&tests[testCase].m, &tempPort, true);
    setMappingThresholds(&tests[testCase].m, 1, 1);
    tests[testCase].expectedEvaluation = false;
    tests[testCase].expectedOutput = false; 
    tests[testCase].output = !(tests[testCase].expectedOutput); /* Initiaise to inverse of expected result so there is a change to observe */
    tests[testCase].expectedFailure = false;

    testCase++;

    /* Two inputs, true and false, expect evaluation to false */
    tests[testCase].testEnabled = true;
    initialiseMapping(&tests[testCase].m, THRESHOLD);
    initialisePort(&tempPort, &tests[testCase].output, false);
    addPortToMapping(&tests[testCase].m, &tempPort, false);
    initialisePort(&tempPort, &POINTER_TO_TRUE_INPUT, false);
    addPortToMapping(&tests[testCase].m, &tempPort, true);
    initialisePort(&tempPort, &POINTER_TO_FALSE_INPUT, false);
    addPortToMapping(&tests[testCase].m, &tempPort, true);
    setMappingThresholds(&tests[testCase].m, 2, 2);
    tests[testCase].expectedEvaluation = false;
    tests[testCase].expectedOutput = false; 
    tests[testCase].output = !(tests[testCase].expectedOutput); /* Initiaise to inverse of expected result so there is a change to observe */
    tests[testCase].expectedFailure = false;

    testCase++;

    /* Two inputs, true and false, expect evaluation to true */
    tests[testCase].testEnabled = true;
    initialiseMapping(&tests[testCase].m, THRESHOLD);
    initialisePort(&tempPort, &tests[testCase].output, false);
    addPortToMapping(&tests[testCase].m, &tempPort, false);
    initialisePort(&tempPort, &POINTER_TO_TRUE_INPUT, false);
    addPortToMapping(&tests[testCase].m, &tempPort, true);
    initialisePort(&tempPort, &POINTER_TO_TRUE_INPUT, false);
    addPortToMapping(&tests[testCase].m, &tempPort, true);
    setMappingThresholds(&tests[testCase].m, 2, 2);
    tests[testCase].expectedEvaluation = true;
    tests[testCase].expectedOutput = true; 
    tests[testCase].output = !(tests[testCase].expectedOutput); /* Initiaise to inverse of expected result so there is a change to observe */
    tests[testCase].expectedFailure = false;

    for(testCase=0;testCase<MAX_TEST_CASE;testCase++) {
        if (tests[testCase].testEnabled == true) {
            sprintf(message,"Test %i\0",testCase);
            TEST_ASSERT_EQUAL_MESSAGE(tests[testCase].expectedFailure,evaluateMapping(&tests[testCase].m),message);
            /* Test evaluation first to prove decision logic */
            TEST_ASSERT_EQUAL_MESSAGE(tests[testCase].expectedEvaluation, tests[testCase].m.evaluation,message);
            /* Test output next as this can be enabled or disabled */
            TEST_ASSERT_EQUAL_MESSAGE(tests[testCase].expectedOutput, tests[testCase].output,message);
            /* Then confirm that no other items were changed */
            /* To be completed */
        }
    }
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_evaluateMappingThresholdInput);
    return UNITY_END();
}