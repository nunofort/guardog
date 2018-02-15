
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

FILE: LogMonitor.hpp

DESC: Log File Monitoring

********************************************************************************/

#include <c++/version/string>
#include <c++/version/sstream>
#include <c++/version/iostream>
#include <c++/version/fstream>

using namespace std;

#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/socket.h>
//#include <regex.h>
#include <string.h>

#include "Monitor.hpp"
//#include "Parser.hpp"

class LogMonitor : public Monitor {
public:
LogMonitor(std::string log, std::string pos);
virtual ~LogMonitor() {}
void process(Guard& guard);

private:
std::string clean(const char *text);
void loadPosition(std::string filename);
void savePosition(std::string filename);

std::string posfile;
std::string logfile;
std::ifstream ifs;
unsigned long position;
};



LogMonitor::LogMonitor(std::string log, std::string pos) 
: logfile(log), posfile(pos) { 

//cout << "[+] Log File Monitoring...[" << logfile << "](" << posfile << ")" << endl;

//loadPosition(posfile);
}


void LogMonitor::loadPosition(std::string filename)
{
  //cout << filename << ": Loading position..." ;

  std::ifstream ifs_pos(filename.c_str(),ios::binary|ios::in);
  //ifs_pos.open(filename.c_str(),ios::in);
  if (!ifs_pos) {
    cout << "Error Opening File " << filename << endl;
    exit(0);
  }
  ifs_pos >> position;
  ifs_pos.close();
}

void LogMonitor::savePosition(std::string filename)
{
  //cout << filename << ": Saving position..." << pos << endl;

  std::ofstream ofs_pos(filename.c_str());
  //ofs_pos.open(filename.c_str(),ios::binary|ios::out);
  if (!ofs_pos) {
    cout << "Error Opening File " << filename << endl;
    exit(0);
  }
  ofs_pos << position;
  ofs_pos.close();
}

std::string LogMonitor::clean(const char *text) {
int idx_start = 0;
int idx_end = strlen(text)-1;
//char tmp[512];
while ((text[idx_start] != '\0') && ((text[idx_start] == ' ') || (text[idx_start] == '\t') || (text[idx_start] == '\n') || (text[idx_start] == '\r'))) {
  idx_start++;
}
while ((idx_end != 0) && 
       ((text[idx_end] == ' ') || (text[idx_end] == '\t') || (text[idx_end] == '\n') || (text[idx_end] == '\r'))) {
  idx_end--;
}
std::string s(text,idx_start,idx_end-idx_start+1);
/*
while (text[idx_old] != '\0') {
  tmp[idx_new] = text[idx_old];
  idx_new++;
  idx_old++;
}
tmp[idx_new] = '\0';
text = strdup(tmp);
*/
return s;
}

//TODO: CHECK FILE TIMESTAMP for update then read from start(0)
//eg. ls -la /var/log/apache2/error.log| awk '{print $6,$7;}
void LogMonitor::process(Guard& guard)
{
//Parser parser;
//FILE *fptr;
  std::string msg("oops");
  char message[1024];
  if (strcmp(posfile.c_str(),"0") == 0)
     position = 0;
  else 
     loadPosition(posfile);
  
  //startpos = parser.read_position(posfile);
  //cout << "START AT POSITION: " << position << endl; 

  std::ifstream ifs_log(logfile.c_str());
  ifs_log.seekg( position, ios::beg );
  //ifs_log.getline(message,512,'\n');
  //cout << message << endl;
  //ifs_log.close();
  //ifs_log.seekg(startpos);
  //ifs_log.seekg(0,ios::end);
/*
  fptr = fopen(logfile.c_str(),"r");
  fseek(fptr, position, SEEK_SET);
*/
  int i = 0;
  do
  {
   /*
   //C parser code
   parser.get_line(fptr,message,sizeof(message));
   message[sizeof(message)-1] = '\0';
   */
   ifs_log.getline(message,512,'\n'); 
   //if (message[0] != '\n') {
   msg = this->clean(message);
   //cout << message << endl;
   //std::string msg(message);
   //if (msg != "") {
   if (msg.length() > 0) {
   if (guard.getVerbose())
   cout << "MONITOR MESSAGE["<<i<<"]: " << msg << endl;
   guard.checkMessage(ifs_log.tellg(),msg);
   i++;
   }
  }
  while (!ifs_log.eof());
  //while (!parser.real_end(fptr));
  if (strcmp(posfile.c_str(),"0") != 0) {
  //position = ftell(fptr);
  position = ifs_log.tellg();
  savePosition(posfile);
  //cout << "END AT POSITION: " << position << endl; 
  //parser.save_position(posfile,endpos);
  }
  ifs_log.close();
  //fclose(fptr);
}



