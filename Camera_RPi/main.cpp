#include "socket.h"
#include <string>
#include <iostream>
#include <stdlib.h>
#include <sstream>

string response;
string url;
string arg;

bool state=true;
bool run=true;

stringstream x;
string s;

bool check_camera(){
    bool host_is;
    int status;
	struct addrinfo hints;
	struct addrinfo *serv;
	int sock;
	char *proto;
	char *host;
    
	host = "192.168.1.100";
	proto = "80";
    
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;		//don't care IPV4 or IPV6
	hints.ai_socktype = SOCK_STREAM;	//TCP
	hints.ai_flags = AI_PASSIVE;		//fill in my IP for me
    
	if((status = getaddrinfo(host, proto, &hints, &serv)) != 0) {
		fprintf(stderr, "getaddrinfo error:%s\n",gai_strerror(status));
		return -1;
	}
    
	sock = socket(serv->ai_family, serv->ai_socktype, serv->ai_protocol);
    
	if (sock == -1) {
		perror("socket");
		return -1;
	}
    
	if (connect(sock, serv->ai_addr, serv->ai_addrlen) == -1) {
		host_is=false;
	}
	else {
		host_is=true;
	}
	close(sock);
	freeaddrinfo(serv);
	return host_is;
}

int main(void){
    //sleep(23);
    if(check_camera()){
        system("xbmc-send -a \"PlayMedia(storage/videos/live1.strm)\"");
    }
    Socket sock1;
    sock1.create();
    sock1.bind(5000);
    sock1.listen();
    while (run) {
        Socket sock2;
        sock1.accept(sock2);
        sock2.recv(s);
        url=s.substr(0,s.find('\n'));
        if(url.substr(0,3)=="GET"){
            response="Success";
            url=url.substr(4,url.find(' ',4)-4);
            if(url.find('?')!=4294967295){
                arg=url.substr(url.find('?')+1);
                url=url.substr(0,url.find('?'));
            }
            
            if (url=="/on") {
                if(system("xbmc-send -a \"PlayMedia(storage/videos/live1.strm)\"")!=0){
                    response="fail";
                }
                state=1;
            }
            else if (url=="/w?") {
                string video;
                video="xbmc-send -a \"PlayMedia(";
                video.append(s.substr(7,(s.find(' ',7)-7)));
                video.append(")\"");
                if(system(video.c_str())!=0){
                    response="fail";
                }
                state=1;
            }
            else if (url=="/off") {
                if(system("xbmc-send -a \"PlayerControl(Stop)\"")!=0){
                    response="fail";
                }
                
                state=0;
            }
            else if (url=="/shutdown") {
                if(system("xbmc-send -a \"Shutdown\"")!=0){
                    response="fail";
                }
                state=0;
            }
            else if (url=="/exit") {
                run=false;
            }
            else if (url=="/time"){
                response="don't know :-/";
            }
            else if (url=="/state"){
                if (state) {
                    response="ON";
                }
                else {
                    response="OFF";
                }
            }
            else {
                response="no Cmd to this url";
                //cout<<"Unknown cmd";
                //cout.flush();
            }
            
            sock2.send("HTTP/1.0 200 OK\r\nConnection: close\r\nAccess-Control-Allow-Origin: *\r\nDate: Sat, 28 Nov 2009 04:36:25 GMT\r\nServer: RPi\r\nContent-Type: text/html\r\n\r\n"+response);
            
            //cout<<"repuest: "<<url <<" => "<<response<<endl;
            //cout.flush();
        }
    }
    sock1.close();
    return 0;
}
