//
//  socket.cpp
//  socketclient
//
//  Created by Noah Bruns on 29.01.13.
//  Copyright (c) 2013 Noah Bruns. All rights reserved.
//

#include <cstdlib>
#include <iostream>
#include <cstring>
#include "socket.h"

using namespace std;

Socket::Socket():m_sock(0) {}

Socket::~Socket() {
    if (is_valid()) {
        ::close(m_sock);
    }
}

bool Socket::create() {
    m_sock = ::socket(AF_INET, SOCK_STREAM, 0);
    if (m_sock<0) {
        cout<<"Fehler beim erstellen des Sockets"<<endl;
        exit(1);
    }
    int y=1;
    setsockopt(m_sock, SOL_SOCKET, SO_REUSEADDR, &y, sizeof(int));
    return true;
}

bool Socket::UDP_create() {
    m_sock=::socket(AF_INET, SOCK_DGRAM, 0);
    if (m_sock<0) {
        cout<<"Fehler beim erstellen des Sockets"<<endl;
        exit(1);
    }
    return true;
}


bool Socket::bind(const int port) {
    if (!is_valid()) {
        return false;
    }
    m_addr.sin_family=AF_INET;
    m_addr.sin_addr.s_addr=INADDR_ANY;
    m_addr.sin_port=htons(port);
    
    int bind_return = ::bind(m_sock, (struct sockaddr *) &m_addr, sizeof(m_addr));
    if(bind_return==-1){
        return false;
    }
    return true;
}

bool Socket::listen() const{
    if (!is_valid()) {
        return false;
    }
    
    int listen_return = ::listen(m_sock, MAXCONNECTIONS);
    if (listen_return==-1) {
        return false;
    }
    return true;
}

bool Socket::accept(Socket& new_socket) const {
    int addr_length=sizeof(m_addr);
    new_socket.m_sock = ::accept(m_sock, (sockaddr *) &m_addr, (socklen_t *) &addr_length);
    if (new_socket.m_sock<=0) {
        return false;
    }
    else{
        return true;
    }
}

bool Socket::connect(const string host, const int port) {
    if (!is_valid()) {
        return false;
    }
    struct hostent *host_info;
    unsigned long addr;
    memset(&m_addr, 0, sizeof(m_addr));
    if ((addr= inet_addr(host.c_str())) != INADDR_NONE) {
        memcpy((char *) &m_addr.sin_addr, &addr, sizeof(addr));
    }
    else {
        host_info= gethostbyname(host.c_str());
        if (NULL == host_info) {
            cout<<"Unbekannter Server"<<endl;
            exit(1);
        }
        memcpy((char *) &m_addr.sin_addr, host_info->h_addr, host_info->h_length);
    }
    m_addr.sin_family = AF_INET;
    m_addr.sin_port=htons(port);
    
    int status =::connect(m_sock, (sockaddr *) &m_addr, sizeof(m_addr));
    
    if (status==0) {
        return true;
    }
    else {
        return false;
    }
}

bool Socket::send(const string s) const{
    int status = ::send(m_sock, s.c_str(), s.size(), 0);
    if (status== -1) {
        return false;
    }
    else {
        return true;
    }
}

int Socket::recv(string& s) const{
    char buf[MAXRECV + 1];
    s="";
    memset(buf, 0, MAXRECV+1);
    int status = ::recv(m_sock, buf, MAXRECV, 0);
    if (status>0 || status!=-1){
        s=buf;
        return status;
    }
    else {
        cout<<"Fehler in Socket::recv"<<endl;
        exit(1);
        return true;
    }
}

void Socket::cleanup() const{}

bool Socket::close() const{
    ::close(m_sock);
    cleanup();
    return true;
}