//Socket_Server
#include "socket.h"
#include <string>
#include <iostream>
#include <stdlib.h>

using namespace std;

string response;
string url;
string x;
bool state=0;

int main(void){
    Socket sock1;
    sock1.create();
    sock1.bind(5000);
    sock1.listen();
    while (true) {
        Socket sock2;
        sock1.accept(sock2);
        string s;
        sock2.recv(s);
        if (s.at(0)=='G') {
            response="Success";
            url=s.substr(4,(s.find(' ',4)-4));
            
            if (url.substr(0,3)=="/on") {
                /*if(system("open /Applications/vlc.app")!=0){
                    response="fail";
                }*/
                usleep(3000000);
                state=1;
            }
            else if (url.substr(0,4)=="/off") {
                /*if(system("open")!=0){
                    response="fail";
                }*/
                state=0;
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