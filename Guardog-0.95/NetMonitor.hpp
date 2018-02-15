
/*******************************************************************************

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

 Copyright (C) 2011 Nuno Fortes(nunofort@gmail.com)

FILE: NetMonitor.hpp

DESC: Network Monitoring

********************************************************************************/

#include <c++/version/string>
#include <c++/version/sstream>
#include <c++/version/iostream>
#include <c++/version/fstream>

using namespace std;
using namespace pcrepp;

#include <time.h>
#include <sys/types.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/socket.h>
#include <regex.h>

//extern "C++" {
#include "Monitor.hpp"
//}
#include "Rawtcp.hpp"

/*
//linkage problem!
extern "C" {
#include "rawtcp.h"
}
*/

class NetMonitor : public Monitor {
public:
NetMonitor(std::string block_filename,int port);
virtual ~NetMonitor() {}
void process(Guard& guard);

private:
void load_blocked_ips(std::string filename);
void check_blocked_network();
int is_private(const char *host);
void view_ips();

int _port; // port to listen
std::string ip; // ip address
char tmpip[64]; // temporary source ip storage

std::string ipsfile;

vector<std::string> ips;

};


NetMonitor::NetMonitor(std::string block_filename,int port) 
: _port(port) 
{
ipsfile = block_filename;
load_blocked_ips(block_filename);

cout << "[+] Network Monitoring..." << port << endl;
}


int NetMonitor::is_private(const char *host)
{
   int found, i;
   char CLASS_A[] = "10.";
   char CLASS_B[] = "172."; // 172.16. -> 172.31.
   char CLASS_C[] = "192.168.";
   found = 1;
   for (i = 0; i < strlen(CLASS_A); i++)
        if (host[i] != CLASS_A[i]) found = 0;
   if (found == 1) return 1;
   found = 1;
   for (i = 0; i < strlen(CLASS_C); i++)
        if (host[i] != CLASS_C[i]) found = 0;
   if (found == 1)
        return 1;
   else
        return 0;
}

void NetMonitor::load_blocked_ips(std::string filename)
{
  std::string line;
  std::ifstream ifs_ips(filename.c_str(),ios::binary|ios::in);
  if (ifs_ips) {
  do {
  ifs_ips >> line;
  ips.push_back(line);
  }
  while (!ifs_ips.eof());
  ifs_ips.close();
  }
    //cout << "Error Opening File " << filename << endl;
    //exit(0);
}

void NetMonitor::view_ips()
{
  for (int i = 0; i < ips.size(); i++)
        cout << ips[i] << endl;
}

void NetMonitor::check_blocked_network()
//void NetMonitor::check_ip(std::string ip)
{
  char blockcheck[] = "iptables -n -L INPUT | grep %ip";
  char blockcmd[] = "iptables -A INPUT -p tcp -s %ip --dport 80 -j DROP";
  Pcre net("^([0-9]{1,3}).([0-9]{1,3})[0-9.]+$");
  char num[32];
  std::string cmd;
  int found;
  vector<std::string> nums;
  vector<int> counts;
  for (int i = 0; i < ips.size(); i++)
  {
   net.search(ips[i]);
   strcpy(num,net.get_match(0).c_str());
   strcat(num,net.get_match(1).c_str());
   for (int i = 0; i < sizeof(nums); i++)
   {
        if (strcmp(num,nums[i].c_str()) == 0)
        {
                counts[i]++;
                found = 1;
                break;
        }
   }
   if (found == 0)
   {
      nums.push_back(num);
      counts.push_back(1);
   }
  }
  int percent;
  Pcre ipex("%ip");
  for (int i = 0; i < nums.size(); i++)
  {
        percent = (int)((counts[i] * 100) / sizeof(ips));
        cout << nums[i] << " = " << counts[i] << " = " << percent << "%\n";
        if (percent > 50)
        {
                cmd = ipex.replace(string(blockcmd),nums[i]);
                system(cmd.c_str());
        }
  }
}

void NetMonitor::process(Guard& guard)
{
  Rawtcp *rawtcp = new Rawtcp;
  int pid, pid_child, state_child;
  int i, socket, newsock, len;
  char message[MESSAGE_SIZE];
  std::string msg;
  //char *message1;
  //struct ConfigOptions *info1;
  //socket = tcp_openserver(_port);
  socket = rawtcp->open();
  //sock = socket;
  //printf("%s %u\n",_ip, _port);
  //newsock = tcp_serverlisten(socket);
  while(1)
  {
        if (rawtcp->datavail(socket, 1000))
        //if (tcp_dataReady(newsock, 1000))
        {
                msg = rawtcp->_read(socket);
                //len = tcp_read(newsock,message,sizeof(message));
                pid = fork();
                if (pid == 0)
                {
                   // NEW PROCESS
                   //printf("child born: %u\n",pid);
                   // Allocate a new message block for the new proccess
                   ////////message1 = (char *) malloc(sizeof(message));
                   //info1 = (struct ConfigOptions *) malloc(sizeof(struct ConfigOptions));
                   ////////memcpy(message1, message, sizeof(message));
                   //memcpy(info1, info, sizeof(info));
                   //message1 = strdup(message);
                   // TAKE IP ADDRESS OF THE HOST
                   //strcpy(info1->tmpip,(char*)inet_ntoa(source.sin_addr.s_addr));
                   //std::string msg(message1);
                   if (guard.getVerbose())
                   cout << "MONITOR MESSAGE: " << msg << endl;
                   guard.checkMessage(0,msg);

                   ///////free(message1); // END THE PROCCESS MESSAGE
                   exit(0); // END PROCESS
                }
                else
                {
                   pid_child = wait( &state_child);
                   //printf("child died: %u\n",pid_child);
                }
        }

  }
  rawtcp->close(socket);
  //tcp_close(newsock);
  //fclose(out);
}






