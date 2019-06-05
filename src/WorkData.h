#ifndef WORKDATA_H
#define WORKDATA_H

#include "CL/cl.h"

typedef struct /*__attribute__ ((packed))*/ WorkData {
	cl_uint index;
	cl_uchar cornerValues; 
	cl_char used;
	cl_float x, y, z;
};
/*
typedef __attribute__ ((packed)) struct
_WorkData
{
	//Initializing with a value causes problems.
	cl_uint index;
	cl_char edgeHasSignChange;
	cl_char used;
	cl_float x, y, z;
}WorkData;
*/
#endif
