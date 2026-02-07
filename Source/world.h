#ifndef _WORLD_H_
#define _WORLD_H_

#include "global.h"
#include "region.h"

#define MAX_REGION  (2)

typedef struct {
    int regionCount;
    region regions[MAX_REGION];
} world;

extern world w;

void run(void);

#endif /* _WORLD_H_*/