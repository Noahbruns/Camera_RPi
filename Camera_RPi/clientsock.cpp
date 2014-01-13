//
//  clientsock.cpp
//  Camera_RPi
//
//  Created by Noah Bruns on 28.12.13.
//  Copyright (c) 2013 Noah Bruns. All rights reserved.
//

#include "clientsock.h"
#include "socket.h"
using namespace std;

ClientSock::ClientSock(string host, int port){
    if (!Socket::create()) {
        //throw SockExcept("Fehler bei Socket::create");
    }
    if (Socket::connect(host,port)) {
        //throw SockExcept("Fehler bei Socket::connect");
    }
}

const ClientSock& ClientSock::operator<<(const string& s) const {
    if (! Socket::send(s)) {
        //throw SockExcept("Fehler beim senden");
    }
    return *this;
}

const ClientSock& ClientSock::operator >> (string& s) const {
    if (!S) {
        <#statements#>
    }
}