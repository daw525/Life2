#include "entity.h"
#include <math.h>

static void log_weights(float *weights, int n) {
    float sum = 0.0;
    int i;

    for (i = 0; i < n; i++) {
        weights[i] = (float)1.0 / (1.0 + log((double)i + 2));
        sum += weights[i];
    }

    for (i = 0; i < n; i++) {
        weights[i] /= sum;
        //printf("%f\n",weights[i]);
    }
}

/*! 
 * Requirement 1: The software shall attempt to keep its input TRUE.
 * 
 * Design ideas:
 * 1.  Are external conditions good or bad? (is input mainly true or mainly false?).
 * 1a. Count number of cycles where "input is true". >= 50% is mainly true?
 * 
 * 2.  Are external conditions getting better, getting worse, how fast are they changing?
 * 2a. Apply weighting to samples. Most recent sample is weighted more than oldest sample.
 *     First attempt at doing this using logarithmic weighting (normalised).
 * 
 * 2b. If external conditions are getting worse then change output state, else keep current output state.
 *     TBD
 * 
 * 3.  How volatile (how long has it been changing like this?)
 * 3a. TBD
 * 
 * 4.  Do my outputs appear to influence the environment?
 * 4a. Count number of cycles where "output is true"?
 * 
 * @param pointer to entity
 * @return None
 * */
void processEntity(entity *e) {
    int samplePointer, sampleCount;
    int uninitialisedSampleCount = 0;
    float weights[MAX_SAMPLE]={0.0};
    float inputThreshold;
    int i;

    /* Pre-calculate sample weighitngs */
    log_weights(weights, e->INTEGRATION_TIME);

    /* Reciprocate on first pass */
    if (e->firstPass) {
        e->firstPass = false;
        e->output = e->input;
    }

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

    sampleCount = 0; 
    samplePointer = e->integrationTime;

    while (sampleCount < e->INTEGRATION_TIME) {
        if (e->inputs[samplePointer]==TRUE_SAMPLE) {
            e->inputTimeTrue = e->inputTimeTrue + weights[sampleCount];
        } else if (e->inputs[samplePointer]==NO_SAMPLE) {
            uninitialisedSampleCount++;
        }
        if (e->outputs[samplePointer]==TRUE_SAMPLE) {
            e->outputTimeTrue = e->outputTimeTrue + weights[sampleCount];
        } 
        samplePointer--;
        if (samplePointer < 0) {
            samplePointer = e->INTEGRATION_TIME-1;
        }
        sampleCount++;
    }

    /* Need to lower threshold appropriately until enough samples have been captured */
    /* Just testing this out for now...*/
    inputThreshold = 0;
    //printf("INT: %d\tUNINIT: %d\n",e->INTEGRATION_TIME,uninitialisedSampleCount);
    for (i=0;i<e->INTEGRATION_TIME-uninitialisedSampleCount;i++) {
        //printf("weight: %f\n", weights[i]);        
        inputThreshold = inputThreshold + weights[i];
        //printf("IPT: %f\n", inputThreshold);
    }

    inputThreshold = inputThreshold * 0.5;
    //printf("IPT 0.5: %f\n", inputThreshold);

    if (e->inputTimeTrue < inputThreshold) {
        e->output = !e->output;
    }


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
        //printf("pOP\t");

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
            //printf("flipT\t");
            //printf("fP\t");
        }

        printf("\n");
    }

    printf("%d\t",time);
    printf("%d\t",identifier);
    printf("%d\t",(int)e->input);
    printf("%d\t",(int)e->output);
    //printf("%d\t",(int)e->previousOutput);
    
    if (verbose) {
        printf("%d\t",e->integrationTime);
        printf("%d\t",e->INTEGRATION_TIME);
        printf("%f\t",e->inputTimeTrue);
        printf("%f\t",e->outputTimeTrue);
        printf("%f\t",e->outputTimeFalse);
        printf("%d\t",e->error);
        printf("%f\t",e->trueWeight);
        printf("%f\t",e->falseWeight);
        //printf("%d\t",e->FLIP_TIME);
        //printf("%d\t",e->flipTime);
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