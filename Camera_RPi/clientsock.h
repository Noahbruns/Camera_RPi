//
//  clientsock.h
//  Camera_RPi
//
//  Created by Noah Bruns on 28.12.13.
//  Copyright (c) 2013 Noah Bruns. All rights reserved.
//

#ifndef Camera_RPi_clientsock_h
#define Camera_RPi_clientsock_h

#include <string>
#include "socket.h"
using namespace std;

class ClientSock : private Socket {
public:
    ClientSock(string host, int port);
    virtual ~ClientSock(){};
    const ClientSock& operator << (const string&) const;
    const ClientSock& operator >> (string&) const;
    void close() const;
};
#endif
