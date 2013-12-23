//
//  socket.h
//  socket server
//
//  Created by Noah Bruns on 26.01.13.
//  Copyright (c) 2013 Noah Bruns. All rights reserved.
//  UNIX/Linux
#ifndef SOCKET_H_
#define SOCKET_H_

#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

const int MAXCONNECTIONS = 5;
const int MAXRECV = 1024;

class Socket {
private:
    int m_sock;
    sockaddr_in m_addr;
public:
    Socket();
    virtual ~Socket();
    
    bool create();
    bool UDP_create();
    bool bind(const int port);
    bool listen() const;
    bool accept(Socket&) const;
    bool connect(const string host, const int port);
    bool send (const string) const;
    int recv(string&) const;
    bool UDP_send (const string, const string, const int port) const;
    int UDP_recv(string&) const;
    bool close() const;
    void cleanup() const;
    bool is_valid() const {return m_sock!=-1;};
};
#endif