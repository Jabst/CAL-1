﻿#ifndef _CONNECTION_
#define _CONNECTION_

#include <cstdio>
#include <cstdlib>
#include <cstring>

#if defined(__APPLE__) || defined(__unix__)
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#else
#include <winsock2.h>
//#include "windows.h"//added by me
#endif

#include <string>
#include <iostream>

using namespace std;

class Connection {
 public:
  Connection(short port);

  bool sendMsg(string msg);
  string readLine();
 private: 
#if defined(__APPLE__) || defined(__unix__)
  int sock;
#else
  SOCKET sock;
#endif
};

#endif
