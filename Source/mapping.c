#include "mapping.h"

/*! 
 * 
 * @param 
 * @return boolean failure indication (true = failure, false = success) 
 * */
bool initialisePort(port *p, bool *b, const bool invert) {
    bool failure;
    if ( (p==NULL) || (b==NULL) ) {
        failure = true;
    } else {
        failure = false;
        p->p=b;
        p->invert=invert;
    }
    return failure;
}

bool initialiseMapping(mapping *m, const mapType t) {
    int p;
    bool failure;
    if (m==NULL) {
        failure = true;
    } else {
        failure = false;
        m->type = t;
        m->count = 0;
        m->inputPortCount = 0;
        m->onThreshold = 0;
        m->offThreshold = 0;
        m->evaluation = 0;
        m->outputPort.p = NULL;
        m->outputPort.invert = false;
        for(p=0;p<MAX_INPUT_PORT;p++) {
            m->inputPorts[p].p = NULL;
            m->inputPorts[p].invert = false;
        }
    }
    return failure;
}

/*! 
 * 
 * @param 
 * @return boolean failure indication (true = failure, false = success) 
 * */
bool addPortToMapping(mapping *m, const port *p, const bool inputPort) {
    bool failure;
    if ( (m==NULL) || (p==NULL) ) {
        failure = true;
    } else {
        if (inputPort) {
            if (m->inputPortCount >= MAX_INPUT_PORT) {
                /* Array is already full */
                failure = true;
            } else {
                failure = false;
                /* Add to array then increment count. So first item is added to index 0, and then count goes up to 1 */
                m->inputPorts[m->inputPortCount].p = p->p;
                m->inputPorts[m->inputPortCount].invert = p->invert;
                m->inputPortCount++;
            }
        } else {
            failure = false;
            m->outputPort.p = p->p;
            m->outputPort.invert = p->invert;
        }
    }
    return failure;
}

/*! 
 * 
 * @param 
 * @return boolean failure indication (true = failure, false = success) 
 * */
bool setMappingThresholds(mapping *m, const int onThreshold, const int offThreshold) {
    bool failure;
    if ( m==NULL) {
        failure = true;
    } else {
        failure = false;
        m->onThreshold = onThreshold;
        m->offThreshold = offThreshold;
    }
    return failure;
}

/*! 
 * 
 * @param 
 * @return boolean failure indication (true = failure, false = success) 
 * */
bool evaluateMapping(mapping *m) {
    int inputPort;
    bool inputPortState;
    bool failure=false;

    if (m==NULL) {
        failure = true;
    } else {
        for (inputPort=0;inputPort<m->inputPortCount;inputPort++) {
            /* First check there are no NULL pointers */
            failure = failure | (m->inputPorts[inputPort].p == NULL);
        }
        
        failure = failure | (m->outputPort.p == NULL);

        if (failure==false) {
            m->count = 0;
    
            for(inputPort=0;inputPort<m->inputPortCount;inputPort++) {
                inputPortState = *m->inputPorts[inputPort].p;

                if (m->inputPorts[inputPort].invert) {
                    inputPortState = !inputPortState;
                }

                if (inputPortState) {
                    m->count++;
                    if (m->count > m->inputPortCount) {
                        m->count = m->inputPortCount;
                    }
                } else {
                    m->count--;
                    if (m->count < 0) {
                        m->count = 0;
                    }
                }
            }
            /* Threshold comparison with hysterisis */
            if (m->evaluation == true && m->count <= m->offThreshold) {
                m->evaluation = false;
            } else if (m->evaluation==false && m->count >= m->onThreshold) {
                m->evaluation = true;
            }
    
            if(m->outputPort.invert) {
                m->evaluation = !m->evaluation;
            }
            *m->outputPort.p = m->evaluation;
        }
    }
    return failure;
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
    }
    printf("type:\t%d\r\n", (int)m->type);
    printf("evaluation:\t%d\r\n", (int)m->evaluation);
    printf("count:\t%d\r\n", m->count);   
    printf("onThreshold:\t%d\r\n", m->onThreshold);
    printf("offThreshold:\t%d\r\n", m->offThreshold);
    printf("Output.p:\t%d\r\n", (int)*m->outputPort.p);
    printf("Output.invert:\t%d\r\n", (int)m->outputPort.invert);
}