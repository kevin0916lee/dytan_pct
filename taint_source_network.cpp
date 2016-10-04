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

#include "include/taint_source_network.h"
#include "include/dytan.h"
#include <iostream>

NetworkTaintSource::NetworkTaintSource(SyscallMonitor *syscallMonitor, bool observeEverything)
{
    monitor = new NetworkMonitor(syscallMonitor, observeEverything);
    monitor->activate();
    monitor->registerAddressDefault(networkReadDefault, this);
}

NetworkTaintSource::~NetworkTaintSource()
{
    delete monitor;
}

void NetworkTaintSource::addObserverForAll(taint_range_t type)
{
    switch (type) {
        case PerByte:
            monitor->registerCallbackForAll(networkReadCallbackPerByte, NULL);
            break;
        case PerRead:
            monitor->registerCallbackForAll(networkReadCallbackPerRead, NULL);
            break;
        default:
            cout << "Missing case!";
            abort();
    }
}

void NetworkTaintSource::addNetworkSource(string host_ip, string host_port, 
        taint_range_t type)
{
    switch(type) {
        case PerByte: 
            monitor->observeAddress(host_ip, host_port, networkReadCallbackPerByte, NULL);
            break;
        case PerRead:
            monitor->observeAddress(host_ip, host_port, networkReadCallbackPerRead, NULL);
            break;
        default:
            cout << "Missing case!";
            abort();
    }
}

void networkReadCallbackPerByte(NetworkAddress networkAddr, ADDRINT start, size_t length, void *v)
{
    int tag;

    assert(taintGen);
    bitset *s = bitset_init(NUMBER_OF_TAINT_MARKS);

    ADDRINT end = start + length;
    for(ADDRINT addr = start; addr < end; addr++) {
        tag = taintGen->nextTaintMark();
        bitset_set_bit(s, tag);
        memTaintMap[addr] = bitset_copy(s);
        bitset_reset(s);
    }
    bitset_free(s);

    ADDRINT currAddress = start;
    while (currAddress < end) {
        taintAssignmentLog << tag << " - [" << networkAddr.strAddress << "] -> " << std::hex << currAddress++ << "\n";
    }
    taintAssignmentLog.flush();

#ifdef TRACE
    if(tracing) {
    	logfile << "\t" << std::hex << start << "-" << std::hex << end - 1 << " <- read\n";
    	logfile.flush();
    }
#endif
}

void networkReadCallbackPerRead(NetworkAddress networkAddr, ADDRINT start, size_t length, void *v)
{
    int tag;
    bitset *s = bitset_init(NUMBER_OF_TAINT_MARKS);
    assert(taintGen);
    tag = taintGen->nextTaintMark();
    //taint entire buffer with 1 mark
    bitset_set_bit(s, tag);

    ADDRINT end = start + length;
    for(ADDRINT addr = start; addr < end; addr++) {
        memTaintMap[addr] = bitset_copy(s);
    }
    bitset_free(s);

    taintAssignmentLog << tag << " - [" << networkAddr.strAddress << "] -> " << std::hex << start << "-" << std::hex << end - 1<< "\n";
    taintAssignmentLog.flush();

#ifdef TRACE
    if(tracing) {
    	logfile << "\t" << std::hex << start << "-" << std::hex << end - 1 << " <- read(" << tag << ")\n";
    	logfile.flush();
    }
#endif
}

void networkReadDefault(NetworkAddress networkAddr, ADDRINT start, size_t length, void *v)
{
    printf("Read from  \n ");
    //clear taint marks

}

