//Socket_Server
#include "socket.h"
#include <string>
#include <iostream>
#include <sstream>

#define CAM_IP "192.168.1.100"

using namespace std;

string response;
string url;
string x;
string rec;
bool state=0;
bool off=false;
const string cam=CAM_IP;

bool check_cam(){
    /*struct sockaddr *sa=;
    socklen_t len;
    char hbuf[NI_MAXHOST];
    
    if (getnameinfo(sa, len, hbuf, sizeof(hbuf),NULL, 0, NI_NAMEREQD)){
        printf("could not resolve hostname");
        return 0;
    }
    else {
        return 1;
    }*/
    return 1;
}

bool check_tv(){
    Socket TV;
    TV.create();
    TV.connect("192.168.2.2", 80);
    TV.send("GET /jsonrpc?jsonrpc=2.0&method=\"Player.GetActivePlayers\"&id=1 HTTP/1.1\r\nAuthorization: Basic eGJtYzpyb290\r\nAccept: */*\r\nUser-Agent: curl\r\nContent-Type: application/json\r\nContent-Length: 0\r\n\r\n");
    TV.recv(rec);
    cout<<rec;
    cout.flush();
    return 1;
}

int main(void){
    Socket sock1;
    sock1.create();
    sock1.bind(5000);
    sock1.listen();
    while (!off) {
        Socket sock2;
        sock1.accept(sock2);
        string s;
        stringstream x;
        sock2.recv(s);
        x<<s;
        if (s.at(0)=='G') {
            response="Success";
            getline(x, url);
            url=s.substr(4,(s.find(' ',4)-4));
            
            if (url.substr(0,3)=="/on") {
                if(check_cam()){
                /*if(system("ssh root@192.168.1.102 \"xbmc-send -a 'PlayMedia(storage/videos/live1.strm)'\"")!=0){
                    response="fail";
                }*/
                state=1;
                }
                else {
                    response="could not connect to CAM";
                }
            }
            else if (url.substr(0,4)=="/off") {
                /*if(system("ssh root@192.168.1.102 \"xbmc-send -a 'PlayerControl(Stop)'\"")!=0){
                    response="fail";
                }*/
                state=0;
            }
            else if (url.substr(0,9)=="/shutdown") {
                /*if(system("ssh root@192.168.1.102 \"xbmc-send -a 'PlayerControl(Stop)'\"")!=0){
                 response="fail";
                 }*/
                state=0;
                off=true;
            }
            else if (url.substr(0,5)=="/time"){
                response="don't know :-/";
            }
            else if (url.substr(0,6)=="/state"){
                if (state) {
                    response="ON";
                }
                else {
                    response="OFF";
                }
            }
            else {
                response="no Cmd to this url";
                cout<<"Unknown cmd";
                cout.flush();
            }
        
        sock2.send("HTTP/1.0 200 OK\r\nConnection: close\r\nAccess-Control-Allow-Origin: *\r\nDate: Sat, 28 Nov 2009 04:36:25 GMT\r\nServer: RPi\r\nContent-Type: text/plain\r\n\r\n"+response);
            
            cout<<"repuest: "<<url <<" => "<<response<<endl;
            cout.flush();
        }
        else {
            sock2.send("HTTP/1.0 200 OK\r\nConnection: close\r\nAccess-Control-Allow-Origin: *\r\nDate: Sat, 28 Nov 2009 04:36:25 GMT\r\nServer: RPi\r\nContent-Type: text/plain\r\n\r\nNOT A GET METHOD");
        }
    }
    sock1.close();
    return 0;
}