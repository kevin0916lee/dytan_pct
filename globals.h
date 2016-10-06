/*
 * globals.h
 *
 *  Created on: Oct 3, 2016
 *      Author: haoli
 */
#include <set>
#include <fstream>
#ifndef GLOBALS_H_
#define GLOBALS_H_
extern unsigned int numberOfThreads;
extern unsigned int numberOfInstructions;
extern std::set<int> changePoints;
extern int* changePointsIndex;
extern int pointIndex;
extern int instructionIndex;
extern std::ofstream memory_trace_out;
extern std::ofstream pct_out;
extern std::ofstream rtn_out;

#endif /* GLOBALS_H_ */
