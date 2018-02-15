
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netdb.h>
#include <errno.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netinet/ip.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define HEADER_SIZE		52
#define MESSAGE_SIZE		8192

//struct iphdr *ipdata;
//struct tcphdr *tcpdata;
//char *msgdata;
//int sock;

class Rawtcp {
public:
Rawtcp() {}
int open();
void close(int sock);
int datavail(int sock, int waitusec);
std::string _read(int sock); 
int _write(int sock, std::string msg); 

};


int Rawtcp::open()
{
  int sock;
  sock = socket (AF_INET, SOCK_RAW, IPPROTO_TCP);
  return sock;
}

void Rawtcp::close(int sock)
{
  close(sock);
}

int Rawtcp::datavail(int sock, int waitusec)
{
    fd_set set;
    struct timeval tv;
    int retval;
    FD_ZERO(&set);
    FD_SET(sock, &set);
    tv.tv_sec = 0; // wait sec
    tv.tv_usec = waitusec; // wait usec
    retval = select(sock+1, &set, NULL, &set, &tv);
    return retval;
}

std::string Rawtcp::_read(int sock) 
{
   int len,i;
   char buffer[MESSAGE_SIZE];
   char *msgdata;
   int bufsize = sizeof(buffer);
   bzero(buffer, bufsize);
   len = read(sock, buffer, bufsize);
   msgdata = buffer+HEADER_SIZE;
   msgdata[len-HEADER_SIZE-1] = '\0';
   /*ipdata = (struct iphdr*) buffer;
   tcpdata = (struct tcphdr*)(buffer+sizeof(struct iphdr));
   // resolve some data assigned formats!
   tcpdata->dest = *(buffer+sizeof(struct iphdr)+3) & 0xFFFF;
   tcpdata->source = *(buffer+sizeof(struct iphdr)+1) & 0xFFFF;
   msgdata = buffer+HEADER_SIZE;
   msgdata[len-HEADER_SIZE] = '\0';*/
   return msgdata;
}

int Rawtcp::_write(int sock, std::string msg) 
{
   int len;
   int bufsize = msg.length();
   //len = write(sock, buffer, HEADER_SIZE+strlen(msgdata));
   len = write(sock, msg.c_str(), bufsize);
   return len;
}


