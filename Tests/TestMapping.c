#include "./Unity/unity.h"
#include "../Source/mapping.h"

typedef struct {
    bool testEnabled;
    mapping m;
    bool expectedEvaluation;
    bool expectedOutput;
    bool output;
} testCase;

static bool POINTER_TO_TRUE_INPUT = true;
static bool POINTER_TO_FALSE_INPUT = false;

void setUp(void) {
     
}

void tearDown(void) {

}


void test_evaluateMappingAndGateOneInput(void) {
    #define MAX_TEST_CASE (10)
    testCase tests[MAX_TEST_CASE];
    int testCase;
    #define MAX_MESSAGE (10)
    char message[MAX_MESSAGE];

    /* No inputs, expect evaluation to false */
    tests[0].testEnabled = true;
    tests[0].expectedEvaluation = false;
    tests[0].expectedOutput = false; 
    tests[0].output = !(tests[0].expectedOutput); /* Initiaise to inverse of expected result so there is a change to observe */
    tests[0].m.inputPortCount = 0;
    tests[0].m.inputPorts[0].p = &POINTER_TO_FALSE_INPUT;
    tests[0].m.inputPorts[0].invert = false;
    tests[0].m.inputPorts[0].enabled = false;
    tests[0].m.outputPort.p = &tests[0].output;
    tests[0].m.outputPort.invert = false;
    tests[0].m.outputPort.enabled = true,
    tests[0].m.type = AND;
    tests[0].m.evaluation = !(tests[0].expectedEvaluation); /* Initiaise to inverse of expected result so there is a change to observe */
    tests[0].m.onThreshold = 0;
    tests[0].m.offThreshold = 999;

    /* One input, false, expect evaluation to false */
    tests[1].testEnabled = true;
    tests[1].expectedEvaluation = false; 
    tests[1].expectedOutput = false; 
    tests[1].output = !(tests[1].expectedOutput); /* Initiaise to inverse of expected result so there is a change to observe */
    tests[1].m.inputPortCount = 1;
    tests[1].m.inputPorts[0].p = &POINTER_TO_FALSE_INPUT;
    tests[1].m.inputPorts[0].invert = false;
    tests[1].m.inputPorts[0].enabled = true;
    tests[1].m.outputPort.p = &tests[1].output;
    tests[1].m.outputPort.invert = false;
    tests[1].m.outputPort.enabled = true,
    tests[1].m.type = AND;
    tests[1].m.evaluation = !(tests[1].expectedEvaluation); /* Initiaise to inverse of expected result so there is a change to observe */
    tests[1].m.onThreshold = 0;
    tests[1].m.offThreshold = 999;

    /* One input, true, expect evaluation to true */
    tests[2].testEnabled = true;
    tests[2].expectedEvaluation = true;
    tests[2].expectedOutput = true; 
    tests[2].output = !(tests[2].expectedOutput);  /* Initiaise to inverse of expected result so there is a change to observe */
    tests[2].m.inputPortCount = 1;
    tests[2].m.inputPorts[0].p = &POINTER_TO_TRUE_INPUT;
    tests[2].m.inputPorts[0].invert = false;
    tests[2].m.inputPorts[0].enabled = true;
    tests[2].m.outputPort.p = &tests[2].output;
    tests[2].m.outputPort.invert = false;
    tests[2].m.outputPort.enabled = true,
    tests[2].m.type = AND;
    tests[2].m.evaluation = !(tests[2].expectedEvaluation); /* Initiaise to inverse of expected result so there is a change to observe */
    tests[2].m.onThreshold = 0;
    tests[2].m.offThreshold = 999;

    /* One input (disabled), true, expect evaluation to false */
    tests[3].testEnabled = true;
    tests[3].expectedEvaluation = false;
    tests[3].expectedOutput = false; 
    tests[3].output = !(tests[3].expectedOutput);  /* Initiaise to inverse of expected result so there is a change to observe */
    tests[3].m.inputPortCount = 1;
    tests[3].m.inputPorts[0].p = &POINTER_TO_TRUE_INPUT;
    tests[3].m.inputPorts[0].invert = false;
    tests[3].m.inputPorts[0].enabled = false;
    tests[3].m.outputPort.p = &tests[3].output;
    tests[3].m.outputPort.invert = false;
    tests[3].m.outputPort.enabled = true,
    tests[3].m.type = AND;
    tests[3].m.evaluation = !(tests[3].expectedEvaluation); /* Initiaise to inverse of expected result so there is a change to observe */
    tests[3].m.onThreshold = 0;
    tests[3].m.offThreshold = 999;

    /* One input (inverted), true, expect evaluation to false */
    tests[4].testEnabled = true;
    tests[4].expectedEvaluation = false;
    tests[4].expectedOutput = false; 
    tests[4].output = !(tests[4].expectedOutput);  /* Initiaise to inverse of expected result so there is a change to observe */
    tests[4].m.inputPortCount = 1;
    tests[4].m.inputPorts[0].p = &POINTER_TO_TRUE_INPUT;
    tests[4].m.inputPorts[0].invert = true;
    tests[4].m.inputPorts[0].enabled = true;
    tests[4].m.outputPort.p = &tests[4].output;
    tests[4].m.outputPort.invert = false;
    tests[4].m.outputPort.enabled = true,
    tests[4].m.type = AND;
    tests[4].m.evaluation = !(tests[4].expectedEvaluation); /* Initiaise to inverse of expected result so there is a change to observe */
    tests[4].m.onThreshold = 0;
    tests[4].m.offThreshold = 999;

    /* One input (disabled and inverted), true, expect evaluation to false */
    tests[5].testEnabled = true;
    tests[5].expectedEvaluation = false;
    tests[5].expectedOutput = false; 
    tests[5].output = !(tests[5].expectedOutput);  /* Initiaise to inverse of expected result so there is a change to observe */
    tests[5].m.inputPortCount = 1;
    tests[5].m.inputPorts[0].p = &POINTER_TO_TRUE_INPUT;
    tests[5].m.inputPorts[0].invert = true;
    tests[5].m.inputPorts[0].enabled = false;
    tests[5].m.outputPort.p = &tests[5].output;
    tests[5].m.outputPort.invert = false;
    tests[5].m.outputPort.enabled = true,
    tests[5].m.type = AND;
    tests[5].m.evaluation = !(tests[5].expectedEvaluation); /* Initiaise to inverse of expected result so there is a change to observe */
    tests[5].m.onThreshold = 0;
    tests[5].m.offThreshold = 999;

    /* One input, false. Output inverted. Expect evaluation to true */
    tests[6].testEnabled = true;
    tests[6].expectedEvaluation = true;
    tests[6].expectedOutput = true; 
    tests[6].output = !(tests[6].expectedOutput);  /* Initiaise to inverse of expected result so there is a change to observe */
    tests[6].m.inputPortCount = 1;
    tests[6].m.inputPorts[0].p = &POINTER_TO_FALSE_INPUT;
    tests[6].m.inputPorts[0].invert = false;
    tests[6].m.inputPorts[0].enabled = true;
    tests[6].m.outputPort.p = &tests[6].output;
    tests[6].m.outputPort.invert = true;
    tests[6].m.outputPort.enabled = true,
    tests[6].m.type = AND;
    tests[6].m.evaluation = !(tests[6].expectedEvaluation); /* Initiaise to inverse of expected result so there is a change to observe */
    tests[6].m.onThreshold = 0;
    tests[6].m.offThreshold = 999;

    /* One input, true. Output inverted. Expect evaluation to false */
    tests[7].testEnabled = true;
    tests[7].expectedEvaluation = false;
    tests[7].expectedOutput = false; 
    tests[7].output = !(tests[7].expectedOutput);  /* Initiaise to inverse of expected result so there is a change to observe */
    tests[7].m.inputPortCount = 1;
    tests[7].m.inputPorts[0].p = &POINTER_TO_TRUE_INPUT;
    tests[7].m.inputPorts[0].invert = false;
    tests[7].m.inputPorts[0].enabled = true;
    tests[7].m.outputPort.p = &tests[7].output;
    tests[7].m.outputPort.invert = true;
    tests[7].m.outputPort.enabled = true,
    tests[7].m.type = AND;
    tests[7].m.evaluation = !(tests[7].expectedEvaluation); /* Initiaise to inverse of expected result so there is a change to observe */
    tests[7].m.onThreshold = 0;
    tests[7].m.offThreshold = 999;

    /* One input, false, expect evaluation to false. Output disabled so expect output unchanged from initial state */
    tests[8].testEnabled = true;
    tests[8].expectedEvaluation = false; 
    tests[8].expectedOutput = true; 
    tests[8].output = (tests[8].expectedOutput);
    tests[8].m.inputPortCount = 1;
    tests[8].m.inputPorts[0].p = &POINTER_TO_FALSE_INPUT;
    tests[8].m.inputPorts[0].invert = false;
    tests[8].m.inputPorts[0].enabled = true;
    tests[8].m.outputPort.p = &tests[8].output;
    tests[8].m.outputPort.invert = false;
    tests[8].m.outputPort.enabled = false,
    tests[8].m.type = AND;
    tests[8].m.evaluation = !(tests[8].expectedEvaluation);
    tests[8].m.onThreshold = 0;
    tests[8].m.offThreshold = 999;

    /* One input, true, expect evaluation to true. Output disabled so expect output unchanged from initial state */
    tests[9].testEnabled = true;
    tests[9].expectedEvaluation = true;
    tests[9].expectedOutput = false; 
    tests[9].output = (tests[9].expectedOutput);
    tests[9].m.inputPortCount = 1;
    tests[9].m.inputPorts[0].p = &POINTER_TO_TRUE_INPUT;
    tests[9].m.inputPorts[0].invert = false;
    tests[9].m.inputPorts[0].enabled = true;
    tests[9].m.outputPort.p = &tests[9].output;
    tests[9].m.outputPort.invert = false;
    tests[9].m.outputPort.enabled = false,
    tests[9].m.type = AND;
    tests[9].m.evaluation = !(tests[9].expectedEvaluation); /* Initiaise to inverse of expected result so there is a change to observe */
    tests[9].m.onThreshold = 0;
    tests[9].m.offThreshold = 999;

    for(testCase=0;testCase<MAX_TEST_CASE;testCase++) {
        if (tests[testCase].testEnabled == true) {
            sprintf(message,"Test %i\0",testCase);
            evaluateMapping(&tests[testCase].m);
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
    RUN_TEST(test_evaluateMappingAndGateOneInput);
    return UNITY_END();
}