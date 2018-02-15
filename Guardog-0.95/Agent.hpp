
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

FILE: Agent.hpp

DESC: Agent main program class

********************************************************************************/

#include <pcre++.h>
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
//#include <regex.h>

//extern "C++" {
//#include "Monitor.hpp"
#include "LogMonitor.hpp"
#include "NetMonitor.hpp"
//#include "Parser.hpp"
//#include "Guard.hpp"
//}


class Agent
{
public:
Agent();
void print_config();
void print_only();
void test_msg(const char *msg);
int start();
void initialize(const char *conf);
void setVerbose(bool verbose);
int getMode();

private:

int is_numeric(const char *text);
void error(std::string msg);
void trim(string& str);
void load_config(const char *conf);

     int mode;
     bool verbose;

     std::string errfile;
     std::string logfile;
     std::string posfile;
     std::string ipsfile;
     std::string cntfile;
     int interval; // seconds
     int port;

     Monitor *monitor;
     //Parser parser;
     Guard guard;

};



Agent::Agent() 
: mode(1), port(0),verbose(false) { 

  //initialize(config);
}

void Agent::setVerbose(bool verbose) {
this->verbose = verbose;
}

void Agent::error(std::string msg)
{
    //std::ofstream ofs;
    //ofs.open(filename,ios::binary|ios::app);
    cout << msg << endl;
    exit(0);	
}

int Agent::is_numeric(const char *text)
{
  int i, ii;
  char DECNUM[] = {'0','1','2','3','4','5','6','7','8','9'};
  int found = 0;
  for (i = 0; text[i] != '\0'; i++)
  {
        for (ii = 0; ii < sizeof(DECNUM); ii++)
        if (text[i] == DECNUM[ii]) found++;
  }
  return (found == strlen(text));
}

void Agent::trim(string& str)
{
  string::size_type pos = str.find_last_not_of(' ');
  if(pos != string::npos) {
    str.erase(pos + 1);
    pos = str.find_first_not_of(' ');
    if(pos != string::npos) str.erase(0, pos);
  }
  else str.erase(str.begin(), str.end());
}

void Agent::load_config(const char *conf)
{
  //FILE *in;
  char key[80];
  char *value;
  char *tmp;

  size_t foundHere = 0;
  char line[1024];
  std::string str_line;
  std::string str_key;
  std::string str_value;
  std::ifstream ifs_config(conf);
  if (ifs_config) {
  //in = fopen(conf,"r");
  //if (!in) { this->error( "No Config File Found!" ); }
  do
  {

  // C++ stream
  ifs_config.getline(line,512,'\n');
  str_line = line;
  //cout << str_line << endl;
  foundHere = 1;
  foundHere = str_line.find("=",foundHere);
  if (foundHere != string::npos) {  
        str_key = str_line.substr(0,foundHere);
        str_value = str_line.substr(foundHere+1);
        this->trim(str_key);
        this->trim(str_value);
        //cout << "[" << str_key << "=" << str_value << "]" << endl;
        strcpy(key,str_key.c_str());
        value = strdup(str_value.c_str());
        //cout << "[" << key << "][" << value << "]" << endl;

        // C Parser
        //value = parser.read_config(in,key,'=');
	if ((key[0] != '#') && (value != NULL)) 
	{
           if (verbose) printf("LOAD %s=[%s]\n",key,value);
	   if (strcmp(key,"mode") == 0) {
                if (strcmp(value,"normal") == 0)
                   mode = 1;
                else if (strcmp(value,"daemon") == 0)
                   mode = 2;
                else
                   mode = 1;
	   } else if (strcmp(key,"prolog") == 0) {
		guard.setLogfile(value);
                guard.init();
	   } else if (strcmp(key,"exiton") == 0) {
		guard.setExit(atoi(value));
	   } else if (strcmp(key,"log") == 0) {
		this->logfile = value;
	   //} else if (strcmp(key,"err") == 0) {
		//errfile = value;	
	   }
	   else if (strcmp(key,"pos") == 0)
	   {
                posfile = value;
		//strcpy(posfile,value);	
	   }
	   else if (strcmp(key,"ips") == 0)
	   {
		ipsfile = value;	
	   }
	   else if (strcmp(key,"cnt") == 0)
	   {
		cntfile = value;	
	   }
	   else if (strcmp(key,"port") == 0)
	   {
		port = atoi(value);	
	   }
/*
	   else if (strcmp(key,"ip") == 0)
	   {
		strcpy(ip,value);	
	   }
*/
	   else if (strcmp(key,"interval") == 0)
	   {
		interval = atoi(value);	
	   }
	   else if (strcmp(key,"regex") == 0)
	   {
		guard.addExpression(value);
	   }
           else if (key[0] == '%') {
                std::string skey(key);
                std::string s(skey,1);
                //cout << "IDX:" << key << " : " << s << endl;
                if (is_numeric(s.c_str()) == 1) {
                // ADD MATCH INDEX NUMBER
                guard.addMatch(atoi(s.c_str()));
                //guard.addMatch(s.toInt());
                //cout << "SUBREGEX:" << value << endl;
                //ADD SUB REG EXPRESSION
                guard.addExpression(value);
                }
           }
	   else if (strcmp(key,"action") == 0)
	   {
		guard.addAction(0,value);
	   }

	}
  }
  }
  while (!ifs_config.eof());
  //while (!parser.real_end(in));
  //fclose(in);
  ifs_config.close();
  }
}

int Agent::getMode() {
   return mode;
}

void Agent::print_config()
{
  int i;
  cout << "ERR FILE: "<< errfile << endl;
  cout << "LOG FILE: "<< logfile << endl;
  cout << "POS FILE: "<< posfile << endl;
  //printf("IPS FILE: %s\n",ipsfile);
  //printf("    PORT: %u\n",port);
  //printf("      IP: %u\n",ip);
  cout << "INTERVAL: " << interval << endl;
  cout << guard << endl;

}

void Agent::print_only()
{ 
  printf("[+] Print Config Options...\n"); 
  this->print_config();
}


void Agent::initialize(const char *conf)
{ 
  //cout << "Initializing..." << endl;
  //cout << "[+] Loading Config..." << endl;
  load_config(conf);

  //printf("[+] Print Config Options...\n"); 
  //print_config();
  //exit(0);
  //guard.init();
}

void Agent::test_msg(const char *msg)
{
  printf("[+] Checking Message...\n"); 
  //monitor = new TestMonitor(msg);
  //monitor->process(guard);
  guard.checkMessage(0,msg);
}

int Agent::start()
{
  if (port == 0) { 
  //cout << "[+] Log File Monitoring...[" << logfile << "](" << posfile << ")" << endl;
  monitor = new LogMonitor(logfile,posfile);
  } else {
  //printf("[+] Network Monitoring...\n"); 
  monitor = new NetMonitor(ipsfile,port);
  }
  guard.setVerbose(this->verbose);
  monitor->process(guard);
  //printf("[+] DONE!\n"); 
  return interval;
} 




