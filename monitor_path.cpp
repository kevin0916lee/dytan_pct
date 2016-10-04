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

#ifdef TARGET_WINDOWS
typedef struct _UNICODE_STRING {
    unsigned short Length;
    unsigned short MaximumLength;
    char*  Buffer;
} UNICODE_STRING;
typedef UNICODE_STRING *PUNICODE_STRING;

typedef struct _OBJECT_ATTRIBUTES {
    unsigned long Length;
    unsigned long RootDirectory;
    PUNICODE_STRING ObjectName;
    unsigned long Attributes;
    void* SecurityDescriptor;
    void* SecurityQualityOfService;
} OBJECT_ATTRIBUTES;
typedef OBJECT_ATTRIBUTES *POBJECT_ATTRIBUTES;

#define SYS_open 66
#define SYS_read 273
#define SYS_close 50
#else
#include <syscall.h>
#endif
#include "include/dytan.h"
#include "include/monitor_path.h"

extern std::ofstream logfile;

void openCallback(INT32, syscall_arguments, void *);
void closeCallback(INT32, syscall_arguments, void *);
void readCallback(INT32, syscall_arguments, void *);

PathMonitor::PathMonitor(SyscallMonitor *monitor, bool observeAll)
{
    syscallMonitor = monitor;
    observeEverything = observeAll;
    defaultObserver = NULL;
    allObserver = NULL;
}

PathMonitor::~PathMonitor()
{
  if(NULL != defaultObserver) {
    delete defaultObserver;
  }
  if(allObserver != NULL)
      delete allObserver;
}

void PathMonitor::activate()
{
  syscallMonitor->addObserver(SYS_open, openCallback, this);
  syscallMonitor->addObserver(SYS_close, closeCallback, this);
  syscallMonitor->addObserver(SYS_read, readCallback, this);
}

void PathMonitor::observePath(string pathname,
			 PathMonitorCallback callback,
			 void *v)
{
  observers[pathname].push_back(pair<PathMonitorCallback, void *>(callback, v));
}

void PathMonitor::registerCallbackForAll(PathMonitorCallback callback, void *v)
{
    if(allObserver != NULL)
        delete allObserver;
    allObserver = new pair<PathMonitorCallback, void *>(callback, v);
}

void PathMonitor::registerDefault(PathMonitorCallback callback, void *v)
{
  if(NULL != defaultObserver) {
    delete defaultObserver;
  }
  defaultObserver = new pair<PathMonitorCallback, void *>(callback, v);
}
 
void PathMonitor::addActiveFileDescriptor(int fd, string pathname)
{
  activeFileDescriptors[fd] = pathname;
}

void PathMonitor::removeActiveFileDescriptor(int fd)
{
  activeFileDescriptors.erase(fd);
}

/**
 * called by the listener for read system call. checks if the
 * read has happened on the file that we are interested in (defaultObserver).
 * if that is the case, it calls the callback for PathTaint 
 */
void PathMonitor::notifyForRead(syscall_arguments args)
{
  int fd = (int) args.arg0;

  if(activeFileDescriptors.find(fd) == activeFileDescriptors.end()) {
    return;
  }

  string pathname = activeFileDescriptors[fd];
  
  if(observers.find(pathname) == observers.end()) {
      
      if(observeEverything == true) {
          if(allObserver != NULL)
              allObserver->first(pathname, args, allObserver->second);
          return;
      }
      
      if(NULL != defaultObserver) {
      defaultObserver->first(pathname, args, defaultObserver->second);
    }
    return;
  }

  vector<pair<PathMonitorCallback, void *> > activeObservers = observers[pathname];

  for(vector<pair<PathMonitorCallback, void *> >::iterator iter = activeObservers.begin(); iter != activeObservers.end(); iter++) {
    (*iter).first(pathname, args, (*iter).second);
  }
  
}

/*********************************************/

// int open(const char *pathname, int flags, mode_t mode);
// friend function that listens for the open system call. This
// is called by the system call monitor. Adds the file descriptor
// of the file that is opened
void openCallback(INT32 syscall_num,
		  syscall_arguments args,
		  void *v)
{
  PathMonitor *pathMonitor = static_cast<PathMonitor *>(v);
 
  if((int)args.ret == -1) return;

#ifdef TARGET_WINDOWS
  log << "system call " << syscall_num << " handled\n";
  log << "num: " << args.num <<"\n";
  log << std::hex;
  log << "arg0: " << args.arg0 <<"\n";
  log << "arg1: " << args.arg1 <<"\n";
  log << "arg2: " << args.arg2 <<"\n";
  log << "arg3: " << args.arg3 <<"\n";
  log << "arg4: " << args.arg4 <<"\n";
  log << "arg5: " << args.arg5 <<"\n";
  log << std::dec;
  log << "ret: " << args.ret <<"\n";
  log << "err: " << args.err <<"\n";

  POBJECT_ATTRIBUTES pObj = (POBJECT_ATTRIBUTES)args.arg2;
/*
  log << std::hex;
  log << "*arg2:";

  char *q = (char *)args.arg2;
  for (int i = 0; i < 64; i++) {
	  log << " ";
	  log << (unsigned int)*q;
	  q++;
  }
  log << "\n";
  
  log << std::hex;
  log << "Buffer:";

  char *p = (char *)pObj->ObjectName->Buffer;
  for (int i = 0; i < 32; i++) {
	  log << " ";
	  log << (unsigned int)*p;
	  p++;
  }
  log << "\n";
  log << std::dec;
*/
  char buf[1024];
  sprintf(buf, "%S", pObj->ObjectName->Buffer);
  log << "ObjectName: " << buf << "\n";
  log.flush();
  int *pHandle = (int*)args.arg0;
  pathMonitor->addActiveFileDescriptor((int)*pHandle, string(buf));
#else
  pathMonitor->addActiveFileDescriptor((int) args.ret, string((const char *)args.arg0));
#endif
}

// int close(int fd);
// friend function that listens for the close call. called by
// system call monitor. removes file descriptor of the file
// being close
void closeCallback(INT32 syscall_num,
		   syscall_arguments args,
		   void *v)
{
  PathMonitor *pathMonitor = static_cast<PathMonitor *>(v);
 
  if((int)args.ret == -1) return;

  pathMonitor->removeActiveFileDescriptor((int) args.arg0);
}

// ssize_t read(int fd, void *buf, size_t count);
// friend function that listens for the read call. called by
// system call monitor. 
void readCallback(INT32 syscall_num,
		  syscall_arguments args,
		  void *v)
{
  PathMonitor *pathMonitor = static_cast<PathMonitor *>(v);

  if((int)args.ret == -1) return;
  
  pathMonitor->notifyForRead(args);

}
