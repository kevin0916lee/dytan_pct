/**

 Copyright 2007
 Georgia Tech Research Corporation
 Atlanta, GA  30332-0415
 All Rights Reserved

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are
 met:

 * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.

 * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided
 * with the distribution.

 * Neither the name of the Georgia Tech Research Coporation nor the
 * names of its contributors may be used to endorse or promote
 * products derived from this software without specific prior
 * written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 **/

#include "include/taint_source_path.h"
#include "include/dytan.h"

#include "include/struct_dat.h"
#include <sys/types.h>
#include <unistd.h>
//#include <stdlib.h>
#include <ctype.h>
//#include <stdio.h>

#ifdef TARGET_WINDOWS
typedef struct _IO_STATUS_BLOCK {
	union {
		unsigned long Status;
		void *Pointer;
	};
	unsigned long *Information;
}IO_STATUS_BLOCK, *PIO_STATUS_BLOCK;
#endif

void pathSourceReadDefault(string, syscall_arguments, void *);
void pathSourceReadCallbackPerByte(string, syscall_arguments, void *);
void pathSourceReadCallbackPerRead(string, syscall_arguments, void *);
void pathSourceReadCallbackPerOffset(string, syscall_arguments, void *);

PathTaintSource::PathTaintSource(SyscallMonitor *syscallMonitor,
		bool observeEverything) {
	monitor = new PathMonitor(syscallMonitor, observeEverything);
	monitor->activate();

	monitor->registerDefault(pathSourceReadDefault, this);
}

PathTaintSource::~PathTaintSource() {
	delete monitor;
}

void PathTaintSource::addObserverForAll(taint_range_t type) {
	switch (type) {
	case PerByte:
		monitor->registerCallbackForAll(pathSourceReadCallbackPerByte, NULL);
		break;
	case PerRead:
		monitor->registerCallbackForAll(pathSourceReadCallbackPerRead, NULL);
		break;
	case PerOffset:
		monitor->registerCallbackForAll(pathSourceReadCallbackPerOffset, NULL);
		break;
	default:
		printf("Missing case\n");
		abort();
	}
}

void PathTaintSource::addPathSource(string pathname, taint_range_t type) {
	switch (type) {
	case PerByte: {
		monitor->observePath(pathname, pathSourceReadCallbackPerByte, NULL);
		break;
	}
	case PerRead: {
		monitor->observePath(pathname, pathSourceReadCallbackPerRead, NULL);
		break;
	}
	case PerOffset: {
		monitor->observePath(pathname, pathSourceReadCallbackPerOffset, NULL);
		break;
	}
	default:
		printf("Missing case\n");
		abort();
	}

}

/**************************************************************/

// ssize_t read(int fd, void *buf, size_t count);
void pathSourceReadCallbackPerByte(string pathname, syscall_arguments args,
		void *v) {
//  TaintGenerator *gen = static_cast<TaintGenerator *>(v);
#ifdef TARGET_WINDOWS
	char *buf = (char *) args.arg5;
	PIO_STATUS_BLOCK ib = (PIO_STATUS_BLOCK)args.arg4;
	int ret = (int)ib->Information;
	/*
	 log << "IO_STATUS_BLOCK:";
	 log << std::hex;
	 char *p = (char *)ib;
	 for (int i = 0; i < 16; i++) {
	 log << " ";
	 log << (int)p[i];
	 }
	 log << std::dec;
	 log << "\n";
	 */
#else
	char *buf = (char *) args.arg1;
	int ret = args.ret;
#endif
	int tag = -1;

	ADDRINT start = (ADDRINT) buf;
	ADDRINT end = start + ret;

	logfile << "pathSourceReadCallbackPerByte:" << pathname << "," << ret << "\n";

	//bail if nothing was actually assigned to memory
	if (ret <= 0)
		return;

	assert(taintGen);
	bitset *s = bitset_init(NUMBER_OF_TAINT_MARKS);

#ifdef TARGET_WINDOWS
	off_t curr = 0;
#else
	off_t currentOffset = lseek(args.arg0, 0, SEEK_CUR);
	off_t curr = currentOffset - args.ret;
#endif

	taintAssignmentLog << "Tainted from file " << pathname << "\n";
	for (ADDRINT addr = start; addr < end; addr++) {
		tag = taintGen->nextTaintMark();
		bitset_set_bit(s, tag);
		memTaintMap[addr] = bitset_copy(s);
		bitset_reset(s);
		char *p = (char *) addr;
		taintAssignmentLog << std::dec << tag << " - " << pathname << "["
				<< curr++ << "] -> " << std::hex << addr << ": " << (int) *p;
		if (isprint(*p))
			taintAssignmentLog << " '" << *p << "'";
		taintAssignmentLog << "\n";
	}
	bitset_free(s);
	taintAssignmentLog.flush();

#ifdef TRACE
	if (tracing) {
		logfile << "\t" << std::hex << start << "-" << std::hex << end - 1
				<< " <- read\n";
		logfile.flush();
	}
#endif
}

void pathSourceReadCallbackPerRead(string pathname, syscall_arguments args,
		void *v) {
//  TaintGenerator *gen = static_cast<TaintGenerator *>(v);

	char *buf = (char *) args.arg1;
	int ret = args.ret;
	int tag;

	ADDRINT start = (ADDRINT) buf;
	ADDRINT end = start + ret;

	//bail if nothing was actually assigned to memory
	if (ret <= 0)
		return;

	assert(taintGen);
	bitset *s = bitset_init(NUMBER_OF_TAINT_MARKS);
	tag = taintGen->nextTaintMark();
	bitset_set_bit(s, tag);

	for (ADDRINT addr = start; addr < end; addr++) {
		memTaintMap[addr] = bitset_copy(s);
	}
	bitset_free(s);

#ifdef TARGET_WINDOWS  
	off_t currentOffset = 0;
#else
	off_t currentOffset = lseek(args.arg0, 0, SEEK_CUR);
#endif

	taintAssignmentLog << "Tainted from file " << pathname << "\n";
	taintAssignmentLog << tag << " - " << pathname << "["
			<< currentOffset - args.ret << "-" << currentOffset << "] -> "
			<< std::hex << start << "-" << std::hex << end - 1 << "\n";
	taintAssignmentLog.flush();

#ifdef TRACE
	if (tracing) {
		logfile << "\t" << std::hex << start << "-" << std::hex << end - 1
				<< " <- read(" << tag << ")\n";
		logfile.flush();
	}
#endif

}

void pathSourceReadDefault(string pathname, syscall_arguments args, void *v) {
}

void pathSourceReadCallbackPerOffset(string pathname, syscall_arguments args,
		void *v) {
	static int count = -1;
	int flag = 0;
	int index = 0;
//  TaintGenerator *gen = static_cast<TaintGenerator *>(v);
#ifdef TARGET_WINDOWS
	char *buf = (char *) args.arg5;
	PIO_STATUS_BLOCK ib = (PIO_STATUS_BLOCK)args.arg4;
	int ret = (int)ib->Information;

#else
	char *buf = (char *) args.arg1;
	int ret = args.ret;
#endif
	int tag = -1;

	ADDRINT start = (ADDRINT) buf;
	ADDRINT end = start + ret;

	logfile << "pathSourceReadCallbackPerOffset:" << pathname << "," << ret << "\n";

	//bail if nothing was actually assigned to memory
	if (ret <= 0)
		return;

	assert(taintGen);
	bitset *s = bitset_init(NUMBER_OF_TAINT_MARKS);

#ifdef TARGET_WINDOWS
	off_t curr = 0;
#else
	off_t currentOffset = lseek(args.arg0, 0, SEEK_CUR);
	off_t curr = currentOffset - args.ret;
#endif
	taintAssignmentLog << "Tainted from file " << pathname << "\n";
	for (ADDRINT addr = start; addr < end; addr++) {
		count++;
		if ((count < datFile.start[index]) || (count > datFile.end[index])) {
			continue;
		}
		if (count == datFile.start[index])
			flag = 1;
		if (count == datFile.end[index])
			index++;
		if (flag) {
			tag = taintGen->nextTaintMark();
			flag = 0;
		}

		bitset_set_bit(s, tag);
		memTaintMap[addr] = bitset_copy(s);
		bitset_reset(s);
		char *p = (char *) addr;
		taintAssignmentLog << std::dec << tag << " - " << pathname << "["
				<< curr++ << "] -> " << std::hex << addr << ": " << (int) *p;
		if (isprint(*p))
			taintAssignmentLog << " '" << *p << "'";
		taintAssignmentLog << "\n";
	}
	bitset_free(s);
	taintAssignmentLog.flush();

#ifdef TRACE
	if (tracing) {
		logfile << "\t" << std::hex << start << "-" << std::hex << end - 1
				<< " <- read\n";
		logfile.flush();
	}
#endif
}

