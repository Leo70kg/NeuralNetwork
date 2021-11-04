#ifndef __COLLECTIVES_H__   
#define __COLLECTIVES_H__

#include <cstddef>
#include <mpi.h>

void InitCollectives();
void RingAllreduce(float* data, size_t length, float* output);
void RingAllgather(float* data, size_t length, float** output);


#endif
