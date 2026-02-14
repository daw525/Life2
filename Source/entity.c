#include "entity.h"

/*! 
 * 
 * @param 
 * @return None
 * */
void processEntity(entity *e) {
    int samplePointer, sampleCount;
    int uninitialisedSampleCount = 0;

    if (e->firstPass) {
        e->firstPass = false;
        e->output = e->input;
    }

    //e->previousOutput = e->output;

    /* Range check pointer first to avoid out of bounds array access */
    if (e->integrationTime >= e->INTEGRATION_TIME) {
        e->integrationTime = 0;
    }

    if(e->input == true) {
        e->inputs[e->integrationTime] = TRUE_SAMPLE;
    } else {
        e->inputs[e->integrationTime] = FALSE_SAMPLE;
    }

    if(e->output == true) {
        e->outputs[e->integrationTime] = TRUE_SAMPLE;
    } else {
        e->outputs[e->integrationTime] = FALSE_SAMPLE;
    }

    e->inputTimeTrue = 0;
    e->outputTimeTrue = 0;
    e->outputTimeFalse = 0;

    /*  This could be improved..
        Give greater weighting to the latest sample. Samples wash out over time. */
    sampleCount = e->INTEGRATION_TIME; 
    samplePointer = e->integrationTime;

    while (sampleCount > 0) {
        if (e->inputs[samplePointer]==TRUE_SAMPLE) {
            e->inputTimeTrue++;
        } else if (e->inputs[samplePointer]==NO_SAMPLE) {
            uninitialisedSampleCount++;
        }
        if (e->outputs[samplePointer]==TRUE_SAMPLE) {
            e->outputTimeTrue++;
        } 
        if (e->outputs[samplePointer]==FALSE_SAMPLE) {
            e->outputTimeFalse++;
        } 
        samplePointer++;
        if (samplePointer >= e->INTEGRATION_TIME) {
            samplePointer = 0;
        }
        sampleCount--;
    }

    /*  Requirement is to keep input TRUE for as long as possible
        if input was TRUE for full integration time, this is as good as it can get. Error will be zero in that case. */
    //e->error = e->INTEGRATION_TIME - e->inputTimeTrue - uninitialisedSampleCount;

    /* Instead of error - check which output state is closest to input true time */
    // if (e->inputTimeTrue - uninitialisedSampleCount - e->outputTimeTrue > e->inputTimeTrue - uninitialisedSampleCount - e->outputTimeFalse) {
    //     e->output = true;
    // } else {
    //     e->output = false;
    // }

    // if (e->error >= e->outputTimeTrue) {
    //     e->output = false;
    // } else if (e->error >= e->outputTimeFalse) {
    //     e->output = true;
    // }

    // if (e->error == 0) {
    //     /* if error is zero, then either outputTimeTrue or outputTimeFalse are INTEGRATION_TIME and the other one is zero */
    //     e->trueWeight = (float)e->outputTimeTrue;
    //     e->falseWeight = (float)e->outputTimeFalse;
    // } else {
    //     /*  This could be improved..
    //         Could consider if trend is growing or shirinking..
    //         How fast is it changing... */

    //     /* */
    //     e->trueWeight = (float)e->outputTimeTrue / e->error;
    //     e->falseWeight = (float)e->outputTimeFalse / e->error;
    //     /*  Integration time is 1, 1 cycle with input false...
    //         Error = 1...
    //         outputTrueTime = 0
    //         outputFalseTime = 1
    //         trueWeight = 0/1 = 0
    //         falseWeight = 1/1 = 1
    //         This is incorrect..
    //         We have only been false and had no reward, so weighting for true should be increasing...
    //         Swapping the test below to be true<false, instead of true>=false. This will flip the weighting*/
    // } 

    //e->output = e->trueWeight >= e->falseWeight;

    if (e->flipTime == e->FLIP_TIME) {
        /*  This could be improved..
            The flip could be held for variable number of cycles...*/
        e->output = !e->output;
        e->flipTime = 0;
    } else {
       e->flipTime++;
    }

    /* Just increment the pointer here, it will wrap around to zero next call if there is an overflow */
    e->integrationTime++;
}

/*! 
 * 
 * @param 
 * @return None
 * */
void printEntityState(int time, int identifier, entity *e, bool withHeader, bool verbose) {
    int sample;

    if (withHeader) {
        printf("T\t");
        printf("ID\t");
        printf("IP\t");
        printf("OP\t");
        printf("pOP\t");

        if (verbose) {
            printf("iT\t");
            printf("INT_Y\t");
            printf("ipTT\t");
            printf("opTT\t");
            printf("opFT\t");
            printf("E\t");
            printf("trueWeight\t");
            printf("falseWeight\t");
            printf("F_T\t");
            printf("flipT\t");
            printf("fP\t");
        }

        printf("\n");
    }

    printf("%d\t",time);
    printf("%d\t",identifier);
    printf("%d\t",(int)e->input);
    printf("%d\t",(int)e->output);
    printf("%d\t",(int)e->previousOutput);
    
    if (verbose) {
        printf("%d\t",e->integrationTime);
        printf("%d\t",e->INTEGRATION_TIME);
        printf("%d\t",e->inputTimeTrue);
        printf("%d\t",e->outputTimeTrue);
        printf("%d\t",e->outputTimeFalse);
        printf("%d\t",e->error);
        printf("%f\t",e->trueWeight);
        printf("%f\t",e->falseWeight);
        printf("%d\t",e->FLIP_TIME);
        printf("%d\t",e->flipTime);
        printf("%d\t",(int)e->firstPass);

        for(sample=0;sample<e->INTEGRATION_TIME;sample++) {
            printf("s[%i]: %d\t",sample, (int)e->inputs[sample]);
        }

        for(sample=0;sample<e->INTEGRATION_TIME;sample++) {
            printf("s[%i]: %d\t",sample, (int)e->outputs[sample]);
        }
    }

    printf("\n");
}