#include "mapping.h"

/*! 
 * 
 * @param 
 * @return None
 * */
void evaluateMapping(mapping *m) {
    int inputPort;
    bool inputPortState;
    int count=0;
    bool andGateIsPossible = false;
     
    m->evaluation = false;
    
    for(inputPort=0;inputPort<m->inputPortCount;inputPort++) {

        if (m->inputPorts[inputPort].enabled) {

            inputPortState = *m->inputPorts[inputPort].p;

            if (m->inputPorts[inputPort].invert) {
                inputPortState = !inputPortState;
            }

            switch(m->type) {
                case AND: {
                    if (andGateIsPossible == false) {
                        /*  For AND gate, evaluation must be initialised to true.
                            This should only be done if atleast one input port is enabled
                            to prevent outputting true when there is no input. */
                        andGateIsPossible = true;
                        m->evaluation = true;
                    }
                    m->evaluation = m->evaluation & inputPortState;
                    break;
                }
                case OR: {
                    m->evaluation = m->evaluation | inputPortState;
                    break;
                }
                case XOR: {
                    printf("Don't use XOR yet!\r\n");
                    break;
                }
                case THRESHOLD: {
                    if (inputPortState) {
                        count++;
                        // cap at max
                    } else {
                        count--;
                        // cap at min
                    }
                    break;
                }
                default: {
                    printf("Invalid mapping type...\r\n");
                    break;
                }
            }
        }
    }

    if(m->type==THRESHOLD) {
        /* Threshold comparisn with hysterisis */
        if (m->evaluation == true && count < m->offThreshold) {
            m->evaluation = false;
        } else if (m->evaluation==false && count >m->onThreshold) {
            m->evaluation = true;
        }
    }

    if(m->outputPort.invert) {
        m->evaluation = !m->evaluation;
    }
    
    if (m->outputPort.enabled==true) {
        *m->outputPort.p = m->evaluation;
    }
}

/*! 
 * 
 * @param 
 * @return None
 * */
void printMappingState(mapping *m) {
    int i;
    printf("inputPortCount:\t%d\r\n", m->inputPortCount);
    for (i=0;i<m->inputPortCount;i++) {
        printf("inputPorts[%i].p:\t%d\r\n",i, (int)*m->inputPorts[i].p);
        printf("inputPorts[%i].invert:\t%d\r\n",i, (int)m->inputPorts[i].invert);
        printf("inputPorts[%i].enabled:\t%d\r\n",i, (int)m->inputPorts[i].enabled);
    }
    printf("type:\t%d\r\n", (int)m->type);
    printf("evaluation:\t%d\r\n", (int)m->evaluation);
    printf("onThreshold:\t%d\r\n", m->onThreshold);
    printf("offThreshold:\t%d\r\n", m->offThreshold);
    printf("Output.p:\t%d\r\n", (int)*m->outputPort.p);
    printf("Output.invert:\t%d\r\n", (int)m->outputPort.invert);
    printf("Output.enabled:\t%d\r\n", (int)m->outputPort.enabled);
}