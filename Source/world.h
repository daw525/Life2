#ifndef _WORLD_H_
#define _WORLD_H_

#include "global.h"
#include "region.h"

bool initialiseRegions(void);
void tick(void);

#define MAX_REGION  (1) 
extern region regions[MAX_REGION];

#endif /* _WORLD_H_*/