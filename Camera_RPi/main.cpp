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

int main(void){
    sleep(26);
    system("xbmc-send -a \"PlayMedia(storage/videos/live1.strm)\"");
    Socket sock1;
    sock1.create();
    sock1.bind(5000);
    sock1.listen();
    while (run) {
        Socket sock2;
        sock1.accept(sock2);
        x.str("");
        sock2.recv(s);
        x<<s;
        url="";
        getline(x,url);
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
