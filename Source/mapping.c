#include "mapping.h"

void initialiseMapping(mapping *m, mapType type, bool *output, bool invertOutput) {
    int inputPort;
    m->evaluation = false;
    m->type = type;
    m->outputPort.p = output;
    m->outputPort.invert = invertOutput;
    m->outputPort.enabled = true;
    m->inputPortCount = 0;
    /* Initialise all input ports to disabled */
    for (inputPort=0;inputPort<MAX_INPUT_PORT;inputPort++) {
        m->inputPorts[inputPort].p = NULL;
        m->inputPorts[inputPort].invert = false;
        m->inputPorts[inputPort].enabled = false;
    }
    m->offThreshold = 0;
    m->onThreshold = 0;
}

/*!
 *
 *
 * @param
 * @return 0 = success; 1 = failure
 */

bool addInputPortToMapping(mapping *m, bool *input, const bool invert) {
    if (m->inputPortCount >=MAX_INPUT_PORT) {
        return true;
    } else {
        m->inputPorts[m->inputPortCount].p = input;
        m->inputPorts[m->inputPortCount].invert = invert;
        m->inputPorts[m->inputPortCount].enabled = true;
        m->inputPortCount++;
        return false;
    }
}

void setMappingThresholds(mapping *m, int onThresdhold, int offThreshold) {
    m->onThreshold = onThresdhold;
    m->offThreshold = offThreshold;
}

void evaluateMapping(mapping *m) {
    int inputPort;
    bool inputPortState;
    int count=0;

    if (m->type == AND) {
        /* For an AND gate it needs to be initialised to TRUE */
        m->evaluation = true;
    } else if (m->type == THRESHOLD) {
        /* Leave in current state for hysterisis */
        m->evaluation = m->evaluation;
    } else {
        m->evaluation = false;
    }

    for(inputPort=0;inputPort<m->inputPortCount;inputPort++) {

        if (m->inputPorts[inputPort].enabled) {

            inputPortState = *m->inputPorts[inputPort].p;

            if (m->inputPorts[inputPort].invert) {
                inputPortState = !inputPortState;
            }

            switch(m->type) {
                case AND: {
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
    *m->outputPort.p = m->evaluation; 
}

void printMappingState(mapping *m) {
    int i;
    printf("inputPortCount: %d\r\n", m->inputPortCount);
    for (i=0;i<m->inputPortCount;i++) {
        printf("inputPorts[%i]: %d\r\n",i, (int)*m->inputPorts[i].p);
    }
    printf("evaluation: %d\r\n", (int)m->evaluation);
    printf("onThreshold: %d\r\n", m->onThreshold);
    printf("offThreshold: %d\r\n", m->offThreshold);
}