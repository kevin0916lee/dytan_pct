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

#ifndef TARGET_WINDOWS
#include <syscall.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <linux/net.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

#include <iostream>
#include <sstream>
#include <string>
#include <typeinfo>
#include <map>
#include <stdexcept>
#include <cassert>
#include <stdio.h>
#include <utility>


#include "include/monitor_network.h"

/**
 * This class uses the Observer design pattern. It is a participant in two 
 * instances of the pattern. According to the design pattern,
 * this class is a subject for TaintNetworkSource, while it is an
 * observer for the SyscallMonitor class.
 */

void socketcallNetworkCallback(INT32 num, syscall_arguments args, void *v);
void readNetworkCallback(INT32 num, syscall_arguments args, void *v);
void closeNetworkCallback(INT32 num, syscall_arguments args, void *v);

NetworkMonitor::NetworkMonitor(SyscallMonitor *monitor, bool observeAll)
{
    syscallMonitor = monitor;
    observeEverything = observeAll;
    defaultAddressObserver = NULL;
    allObserver = NULL;
    
}

NetworkMonitor::~NetworkMonitor()
{
  if(NULL != defaultAddressObserver) {
    delete defaultAddressObserver;
  }
    if(allObserver != NULL)
        delete allObserver;

}

void NetworkMonitor::activate()
{
#ifndef TARGET_WINDOWS
#if __WORDSIZE == 32
  syscallMonitor->addObserver(SYS_socketcall, socketcallNetworkCallback, this);
#endif
  syscallMonitor->addObserver(SYS_read, readNetworkCallback, this);
  syscallMonitor->addObserver(SYS_close, closeNetworkCallback, this);
#endif
}

/**
 * Registers the default callback function
 */
void NetworkMonitor::registerAddressDefault(NetworkMonitorCallback callback,
					 void *v)
{
  if(NULL != defaultAddressObserver) {
    delete defaultAddressObserver;
  }

  defaultAddressObserver = new pair<NetworkMonitorCallback, void *>(callback, v);
  
}

void NetworkMonitor::registerCallbackForAll(NetworkMonitorCallback callback, void *v)
{
	
    if(allObserver != NULL)
        delete allObserver;
    allObserver = new pair<NetworkMonitorCallback, void *>(callback, v);
	
}

/**
 * Adds a callback for the address addr
 */
void NetworkMonitor::observeAddress(string ip, string port,
				    NetworkMonitorCallback callback,
				    void *v)
{
#ifndef TARGET_WINDOWS
    NetworkAddress addr;
    addr.ip = inet_addr(ip.c_str());
    addr.port = atoi(port.c_str());
    addr.strAddress = ip + " : " + port;
    addressObservers[addr].push_back(pair<NetworkMonitorCallback, void *>(callback, v));
#endif
}

/**
 * Checks if the read/recv/recvfrom/recvmsg system call has taken place for the 
 * network address that we are monitoring. If yes, then
 * notify the TaintNetworkSource object
 */
void NetworkMonitor::notifyForRead(syscall_arguments args, 
        NetworkAddress& address, ADDRINT start, size_t bytesRead)
{
//    if(addressObservers.find(address) == addressObservers.end()) {
//        if(NULL != defaultAddressObserver) {
//            defaultAddressObserver->first(address,
//                    args,
//                    defaultAddressObserver->second);
//        }
//        return;
//    }
	
#ifndef TARGET_WINDOWS
    map<NetworkAddress, vector<pair<NetworkMonitorCallback, void *> > >::iterator it = addressObservers.find(address);
    NetworkAddress naddr = it->first;
    vector<pair<NetworkMonitorCallback, void *> > activeObservers = it->second;

    for(vector<pair<NetworkMonitorCallback, void *> >::iterator iter = activeObservers.begin(); 
            iter != activeObservers.end(); iter++) {
        (*iter).first(naddr, start, bytesRead, (*iter).second);
    }
#endif
}


void socketcallNetworkCallback(INT32 num, syscall_arguments args, void *v)
{
#ifndef TARGET_WINDOWS
    NetworkMonitor *networkMonitor = static_cast<NetworkMonitor *>(v);
    struct sockaddr *serv_addr;
    struct sockaddr_in *addr; 
    NetworkAddress address;
    int sock;
    unsigned long *sock_args = (unsigned long *)args.arg1;
    
    switch ((int)args.arg0) {
        case SYS_SOCKET:
            //pass
            break;
        case SYS_CONNECT: 
            {
                serv_addr = (struct sockaddr *)sock_args[1];
                addr = (struct sockaddr_in *)serv_addr;
                address.ip = addr->sin_addr.s_addr;
                address.port = ntohs(addr->sin_port);
                // check if the address in this call needs to be observed
                if (networkMonitor->addressObservers.find(address) != networkMonitor->addressObservers.end() && networkMonitor->observeEverything == true) {
                    sock = (int)sock_args[0];
                    // save socket number and address for future read calls
                    networkMonitor->socketToAddress[sock] = address;
                }
                
                //pass
            }
            break;
        case SYS_BIND:
        case SYS_LISTEN:
        case SYS_ACCEPT:
        case SYS_GETSOCKNAME:
        case SYS_GETPEERNAME:
        case SYS_SEND:
     //       cout << "send = " << (char *)sock_args[1] << endl;
            break;
        case SYS_RECVFROM:
            {
                sock = (int)sock_args[0];
                serv_addr = (struct sockaddr *)sock_args[4];
                addr = (struct sockaddr_in *)serv_addr;
                address.ip = addr->sin_addr.s_addr;
                address.port = ntohs(addr->sin_port);
                // check if the address in this call needs to be observed
                if (networkMonitor->addressObservers.find(address) != networkMonitor->addressObservers.end() && networkMonitor->observeEverything == true) {
                    // save socket number and address for future read calls
                    networkMonitor->socketToAddress[sock] = address;
                } 
            }
        case SYS_RECV:
            {
                sock = (int)sock_args[0];
                if (networkMonitor->socketToAddress.find(sock) == networkMonitor->socketToAddress.end() && networkMonitor->observeEverything == false) {
                    //TODO : do i need to clear taint marks for the read memory?
                    break;
                }

                address = networkMonitor->socketToAddress[sock];

                ADDRINT buf = sock_args[1];
                int bytesRead = args.ret;
                networkMonitor->notifyForRead(args, address, buf, bytesRead);
            }
            break;
        case SYS_SENDTO:
        case SYS_SHUTDOWN:
        case SYS_SETSOCKOPT:
        case SYS_GETSOCKOPT:
        case SYS_SENDMSG:
            break;
        default:
            cout << "Unhandled system call" << endl;
            abort();
    }
#endif
}

void closeNetworkCallback(INT32 syscall_num, syscall_arguments args, void *v)
{
    NetworkMonitor *networkMonitor = static_cast<NetworkMonitor *>(v);

    if((int)args.ret == -1) return;

    networkMonitor->socketToAddress.erase((int)args.arg0);
}

// ssize_t read(int fd, void *buf, size_t count);
void readNetworkCallback(INT32 num, syscall_arguments args, void *v)
{
    NetworkMonitor *networkMonitor = static_cast<NetworkMonitor *>(v);

    if(-1 == (int)args.ret) return;
    
    if (networkMonitor->socketToAddress.find((int)args.arg0) ==
            networkMonitor->socketToAddress.end()) {
        //TODO : clear taint marks in read buffer?
    } else {
        NetworkAddress address = networkMonitor->socketToAddress[(int)args.arg0];

        ADDRINT buf = args.arg1;
        int bytesRead = args.ret;
        networkMonitor->notifyForRead(args, address, buf, bytesRead);
    }
}
