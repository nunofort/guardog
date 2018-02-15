
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

FILE: Guard.hpp

DESC: Guard Main check and action

********************************************************************************/

#ifndef GUARD_CLASS
#define GUARD_CLASS

#include <pcre++.h>
#include <c++/version/string>
#include <c++/version/sstream>
#include <c++/version/iostream>
#include <c++/version/fstream>
#include <c++/version/vector>

using namespace std;
using namespace pcrepp;

#include <time.h>
#include <sys/types.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/socket.h>
#include <regex.h>

//extern "C++" {
#include "Expression.hpp"
#include "Action.hpp"
#include "CommandAction.hpp"
//}

// Application Log File
const char GUARD_LOG_FILE[] = "guardog.log";

class Guard
{
public:
Guard();
Guard(Guard& g) {}
void init();
void setVerbose(bool verbose);
bool getVerbose();
void setExit(bool exiton);
void setLogfile(const std::string& logfile);
void addExpression(std::string ex);
void addAction(int rule_idx, std::string a);
void addMapping(unsigned idx);
void addMatch(unsigned idx);
void checkMessage(unsigned long position,std::string msg);
friend std::ostream& operator<<(std::ostream& os,const Guard& guard);

private:
void alertAction(std::string msg, int num);

std::vector<Expression*> regex;
std::vector<Action*> action; // Must be pointer to work!
std::vector<unsigned> emap;
std::vector<unsigned> idxmatch;
std::vector<bool> cond;
std::string logfile;
bool exiton;

std::ofstream log_stream;
bool verbose;
unsigned long filepos;
};



Guard::Guard() : verbose(false), exiton(true), filepos(0) {

this->logfile = GUARD_LOG_FILE;

}

void Guard::init() {
log_stream.open(this->logfile.c_str(),ios::binary|ios::ate|ios::app);

log_stream << "GUARD: Initializing..." << endl;

}

void Guard::setVerbose(bool verbose) {
  this->verbose = verbose;
}

bool Guard::getVerbose() {
 return this->verbose;
}

void Guard::setExit(bool exiton) {
  this->exiton = exiton;
}

void Guard::setLogfile(const std::string& logfile) {
  this->logfile = logfile;
}

void Guard::addExpression(std::string ex) {

//Expression *exp = new Expression(ex);
//if (exp.isValid()) {
//TODO: validate regex


std::string load_key("load:");
std::string neg_key("!");
std::string s(ex,0,5);
//cout << "KEY: " << s << endl;
if (s == load_key) { 
std::string s(ex,5);
//LOAD FROM EXTERNAL SOURCE
//not available yet!
}
else {
if (this->verbose)
cout << "LOADING REGEX :" << ex << endl;

std::string s(ex,0,1);
if (s == neg_key) {
   cond.push_back(false);
   ex = ex.substr(1);
} else
   cond.push_back(true);
 
log_stream << "GUARD: Adding Expression...[" << regex.size() << "]" << endl;
log_stream << ex << endl;
regex.push_back(new Expression(ex));

}

}

void Guard::addAction(int rule_idx, std::string cmd) {

std::string cmd_key("cmd:"); 
std::string sim_key("sim:"); 
std::string load_key("load:"); 
std::string shutdown_key("shutdown");
std::string reboot_key("shutdown");

std::vector<string> keys;
keys.push_back(cmd_key);
keys.push_back(sim_key);
keys.push_back(load_key);
keys.push_back(shutdown_key);
keys.push_back(reboot_key);

for (int i = 0; i < keys.size(); i++) {
int len_key = keys[i].length();
std::string s(cmd,0,len_key);
//cout << "KEY: " << s << endl;
if (s == keys[i]) {
if (s == shutdown_key)
   //not used yet!
   cout << "SHUTing down!..." << endl;
else if (s == reboot_key)
   //not used yet!
   cout << "REBOOTing!..." << endl;
else {
bool sim = false;
if (s == sim_key) {
   cout << "SIMulating ..." << endl;
   sim = true;
}
//EXECUTE COMMAND
std::string s(cmd,len_key);
if (this->verbose)
cout << "LOADING ACTION " << keys[i] << " " << s << endl;
CommandAction *ca = new CommandAction(s);
ca->simulate(sim);
log_stream << "GUARD: Adding Action...[" << cmd.size() << "]" << endl;
log_stream << cmd << endl;
action.push_back(ca);
}
}
}

/*
std::string s(cmd,0,4);
cout << "KEY: " << s << endl;
if (s == cmd_key) {
std::string s(cmd,4);
cout << "LOADING COMMAND: " << s << endl;
action.push_back(new CommandAction(s));
}
std::string s(cmd,0,5);
cout << "KEY: " << s << endl;
if (s == load_key) {
std::string s(cmd,5);
cout << "LOADING LOAD: " << s << endl;
action.push_back(new CommandAction(s));
}
//if (cmd == "shutdown") { 
//if (cmd == "reboot") { 
*/

/*
//C API
//if (strcmp(cmd,"shutdown") == 0) {
//if (strcmp(cmd,"reboot") == 0) {
//if (strcmp(cmd,"") == 0) {
char s[255];
strncpy(s,cmd,4);
if (strcmp(s,"cmd:") == 0) {
strncpy(s,cmd,255);
cout << "CMD: " << s << endl;
action.push_back(new CommandAction(s));
}
//strncpy(s,cmd,5);
//if (strcmp(s,"load:") == 0)
*/

}

void Guard::addMatch(unsigned idx) {
if (this->verbose)
cout << "LOAD INDEX MATCH: " << idx << endl;
log_stream << "GUARD: Adding Sub Expression..." << "[" << idx << "]" << endl;
idxmatch.push_back(idx);
}

void Guard::alertAction(std::string msg, int num)
{
struct tm *tm_time;
time_t t;
int day,mon,year,hour,min,sec;
t = time(&t);
tm_time = localtime(&t);
day = tm_time->tm_mday;
mon = tm_time->tm_mon+1;
year = 1900+tm_time->tm_year;
hour = tm_time->tm_hour;
min = tm_time->tm_min;
sec = tm_time->tm_sec;

   std::ostringstream oss;
   oss << year << "/" << mon << "/" << day << "#" << hour << ":" << min << ":" << sec;

   num--;
   if (action.size() > num) {
   if (this->verbose)
   cout << "ALERT ACTION[" << num << "]:" << *action[num] << endl;

   // SAVING TO LOG FILE THE ALERT INFORMATION
   log_stream << "GUARD: !!! ALERT !!! " <<  oss.str() << endl;
   log_stream << msg << endl;

   log_stream << "GUARD: EXECUTE ACTION[" << (num+1) << "]..." << *action[num] << endl;
   std::string cmd = action[num]->execute(this->filepos,regex[0]); 
   log_stream << cmd << endl;
   }
}


void Guard::checkMessage(unsigned long position,std::string msg)
{
  int i, ii;
/*
  regex_t cregex; // C API
  size_t nmatch = 10; // MUST to request number of regmatch_t to allocate
  regmatch_t pmatch[nmatch];
  int errcode;
*/
   if (this->verbose)
   cout << "CHECK MSG:" << msg << endl;
   //cout << position << endl;
   //for (i = 0; i < regex.size(); i++)
   //{
        //C style
        //errcode = regcomp(&cregex, exp, REG_EXTENDED);
        //if (errcode == 0)
        //{
           //MAIN REGEX CHECK
           if (regex[0]->match(msg) == true)
           //if ((errcode = regexec(&cregex, msg, nmatch, pmatch, 0)) == 0)
           {
           
           //cout << "REGEX DETECTED: " << *regex[0] << endl;
           for (ii = 0; ii < idxmatch.size(); ii++) {
           if (regex[0]->matches() > idxmatch[ii]) { 
           std::string m = regex[0]->getMatch(idxmatch[ii]);
           //GET FROM INDEX SUB STRING MATCH
           if (this->verbose)
           cout << position << ": CHECK SUBMSG[" << idxmatch[ii] << "]: " << m << endl;
           //for (i = 1; i < regex.size(); i++) {
           i=ii+1;
           if (this->verbose)
           cout << "CHECK SUBREGEX[" << i << "]: " << *regex[i] << endl;
           if (((cond[i] == true) && (regex[i]->match(m) == true)) ||
               ((cond[i] == false) && (regex[i]->match(m) == false))) {
           //if (regex[i]->match(m) == true) {
           if (this->verbose) {
           cout << "!REGEX FOUND[" <<i<<"]: " << *regex[i] << endl;
           cout << "!!! GUARD ALERT !!!"<< m << endl;
           }
           log_stream << "GUARD : REGEX[" << i << "]: " << *regex[i] << endl;
           log_stream << "GUARD : MATCH[" << i << "]: " << m << endl;
           this->filepos = position;
           //printf("MATCH EXP: %u\n\n",i);
           this->alertAction(msg, i);
           //alert_action(msg, pmatch, nmatch, i);
           //exit is ONLY FOR TESTING, should be continued..here!
           //set exiton!
           if (this->exiton) {
              log_stream  << "GUARD : Exiting..." << endl;
              exit(0);
           }
           //break;
           }
           //}
           }
           else cout << "MATCH OUT OF RANGE!" << endl;
           }
           }
        //}
        // Free Regex Allocated Memory
        //regfree(&cregex);
    //}
}

std::ostream& operator<<(std::ostream& os, const Guard& guard)
{
   os << "REGEX: " << endl;
   for (int i = 0; i < guard.regex.size(); i++) {
      os << i << ": " << *guard.regex[i] << endl;
   }
   os << "ACTION: " << endl;
   for (int i = 0; i < guard.action.size(); i++) {
      os << i << ": " << *guard.action[i] << endl;
   }
   return os;
}


#endif

