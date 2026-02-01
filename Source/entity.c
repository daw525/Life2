#include "entity.h"


void initialiseEntity(entity *e, int integrationTime, int flipTime) {
        int sample;
        e->input = false;
        e->output = false;
        e->previousOutput = false;
        for(sample=0;sample<MAX_SAMPLE;sample++) {
            e->inputs[sample] = false;
            e->outputs[sample] = false;
        }
        e->INTEGRATION_TIME = integrationTime;
        e->inputTimeTrue = 0;
        e->outputTimeTrue = 0;
        e->outputTimeFalse = 0;
        e->integrationTime = 0;
        e->error = 0;
        e->trueWeight = 0;
        e->falseWeight = 0;
        e->FLIP_TIME = flipTime;
        e->flipTime = 0; 
}

void processEntity(entity *e) {
    int samplePointer, sampleCount;

    e->previousOutput = e->output;

    /* Range check pointer first to avoid out of bounds array access */
    if (e->integrationTime >= e->INTEGRATION_TIME) {
        e->integrationTime = 0;
    }
    e->inputs[e->integrationTime] = e->input;
    e->outputs[e->integrationTime] = e->previousOutput;

    e->inputTimeTrue = 0;
    e->outputTimeTrue = 0;
    e->outputTimeFalse = 0;

    /*  This could be improved..
        Give greater weighting to the latest sample. Samples wash out over time. */
    sampleCount = e->INTEGRATION_TIME; 
    samplePointer = e->integrationTime;

    while (sampleCount > 0) {
        if (e->inputs[samplePointer]==true) {
            e->inputTimeTrue++;
        }
        if (e->outputs[samplePointer]==true) {
            e->outputTimeTrue++;
        } else {
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
    e->error = e->INTEGRATION_TIME - e->inputTimeTrue;

    if (e->error == 0) {
        /* if error is zero, then either outputTimeTrue or outputTimeFalse are INTEGRATION_TIME and the other one is zero */
        e->trueWeight = (float)e->outputTimeTrue;
        e->falseWeight = (float)e->outputTimeFalse;
    } else {
        /*  This could be improved..
            Could consider if trend is growing or shirinking..
            How fast is it changing... */
        e->trueWeight = (float)e->outputTimeTrue / e->error;
        e->falseWeight = (float)e->outputTimeFalse / e->error;
    } 

    e->output = e->trueWeight >= e->falseWeight;

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

void printEntityState(int time, int identifier, entity *e, bool withHeader, bool verbose) {
    int sample;

    if (withHeader) {
        printf("Time\t");
        printf("ID\t");
        printf("Input\t");
        printf("Output\t");
        printf("Prev. Output\t");

        if (verbose) {
            printf("integrationTime\t");
            printf("INTEGRATION_TIME\t");
            printf("inputTimeTrue\t");
            printf("outputTimeTrue\t");
            printf("outputTimeFalse\t");
            printf("error\t");
            printf("trueWeight\t");
            printf("falseWeight\t");
            printf("FLIP_TIME\t");
            printf("flipTime\t");
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

        for(sample=0;sample<e->INTEGRATION_TIME;sample++) {
            printf("%d\t", (int)e->inputs[sample]);
        }

        for(sample=0;sample<e->INTEGRATION_TIME;sample++) {
            printf("%d\t", (int)e->outputs[sample]);
        }
    }

    printf("\n");
}