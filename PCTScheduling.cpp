/*
 * PCTScheduling.cpp
 *
 *  Created on: Oct 3, 2016
 *      Author: haoli
 */

#include "PCTScheduling.h"
#include <sched.h>
VOID docount() {

	if(instructionIndex==changePointsIndex[pointIndex]){
		cout<<"change point:"<<dec<<instructionIndex<<"    ";
		cout<<"yield() is called"<<std::endl;
		PIN_Yield();
		pointIndex++;
//		struct sched_param param;
//		param.sched_priority = 20;
//		if (sched_setscheduler(0, SCHED_FIFO, &param)) {
//	      printf("errno = \n");
//		}
	}

	instructionIndex++;
}

// Pin calls this function every time a new instruction is encountered
VOID PCTScheduling(INS ins, VOID *v)
{
    // Insert a call to docount before every instruction, no arguments are passed
    INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)docount, IARG_END);
}
