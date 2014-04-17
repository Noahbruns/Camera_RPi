#include "socket.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits>
#include <iostream>

string response;
string url;
string arg;

bool state=false;
bool run=true;

string s;

string Commanddo(string cmd) {
    
    string data;
    FILE * stream;
    const int max_buffer = 256;
    char buffer[max_buffer];
    cmd.append(" 2>&1");
    
    stream = popen(cmd.c_str(), "r");
    if (stream) {
        while (!feof(stream))
            if (fgets(buffer, max_buffer, stream) != NULL) data.append(buffer);
        pclose(stream);
    }
    return data;
}

bool check(string src, char needle){
    if(src.find(needle)!=std::numeric_limits<unsigned long>::max()){
        //needle found
        return 0;
    }
    else {
        return 1;
    }
}

bool check_camera(){
    string cmd=Commanddo("ping -c 1 -t 1 192.168.178.99");
    unsigned long place = cmd.find(" packet loss");
    
    if(place == numeric_limits<unsigned long>::max()){
        return false;
    }
    if (cmd.substr(place-4,4)=="100%") {
        return false;
    }
    else {
        return true;
    }
}

string IR_cmd(string cmd){
    string ret=Commanddo("irsend SEND_ONCE LG "+cmd);
    return ret;
}


int main(int argc, char* argv[]){
    if (argc==1) {
        sleep(25);
    }
    
    while (!check_camera()) {
        sleep(5);
    }
    
    Commanddo("xbmc-send -a \"PlayMedia(storage/videos/live1.strm)\"");
    
    state=true;
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
            arg="";
            url=url.substr(4,url.find(' ',4)-4);
            if(url.find('?')!=std::numeric_limits<unsigned long>::max()){
                arg=url.substr(url.find('?')+1);
                url=url.substr(0,url.find('?'));
            }
            
            if (url=="/on") {
                Commanddo("xbmc-send -a \"PlayMedia(storage/videos/live1.strm)\"");
                state=1;
                sleep(10);
            }
            else if (url=="/w") {
                string video;
                video="xbmc-send -a \"PlayMedia(";
                video.append(s.substr(7,(s.find(' ',7)-7)));
                video.append(")\"");
                Commanddo(video.c_str());
                state=1;
            }
            
            else if (url=="/ir") {
                if (!check(arg, '&') || !check(arg, '|')) {
                    response="not allowed";
                }
                else {
                    response=IR_cmd(arg);
                }
            }
            
            else if (url=="/off") {
                Commanddo("xbmc-send -a \"PlayerControl(Stop)\"");
                
                state=0;
            }
            else if (url=="/shutdown") {
                Commanddo("xbmc-send -a \"Shutdown\"");
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
