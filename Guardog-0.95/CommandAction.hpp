
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

FILE: CommandAction.hpp

DESC: Command Type Alert Action

********************************************************************************/


#ifndef COMMANDACTION_CLASS
#define COMMANDACTION_CLASS

#include <c++/version/string>
#include <c++/version/sstream>
#include <c++/version/iostream>

using namespace std;

//extern "C++" {
#include "Action.hpp"
//}

#include <time.h>
#include <sys/types.h>
#include <stdlib.h>

//Command Type Alert Action

class CommandAction : public Action {
public:
CommandAction();
~ CommandAction() {}
CommandAction(std::string cmd);
//void addMatch(std::string match);
std::string get();
std::string execute(unsigned long pos,Expression *ex);
void simulate(bool sim);


private:
std::string exec;

//std::vector<std::string> matches;


};



CommandAction::CommandAction() {}

CommandAction::CommandAction(std::string cmd) 
{
  s = cmd; 
  exec = cmd;
}

std::string CommandAction::get() {
return exec;
}

/*
void CommandAction::addMatch(std::string match) {
matches.push_back(match);
}
*/

std::string CommandAction::execute(unsigned long pos,Expression *ex) {
//cout << "PROCESS Action: " << exec << endl;
//cout << "REGEX MATCH: " << *ex << endl;

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

std::string cmd = exec;
//GET MATCHES FROM EXPRESSION!
if (ex->matches() >= 1) {
//cout << ex->matches() << endl;
size_t foundHere;
//int len = cmd.length();
for (int i = 0; i < ex->matches(); i++)
{
     std::string m = ex->getMatch(i);
     std::ostringstream o;
     o << "%" << i;
     //cout << o.str() << "=[" << m << "]" << endl;
     Pcre re(o.str());
     //std::string re = o.str();

     foundHere = 0;
     while ((foundHere = cmd.find(o.str(),foundHere)) != string::npos) {
     cmd = re.replace(cmd,m);
     //cmd = re.replace(cmd,m);
     //cmd = cmd.replace(s,m);
     //cout << "REPLACE: " << cmd << endl;
     }
}
}
std::ostringstream os;
os << pos;
Pcre re("%POSITION%");
cmd = re.replace(cmd,os.str());

std::ostringstream os_timenow;
os_timenow << hour << ":" << min << ":" << sec;
std::ostringstream os_datenow;
os_datenow << year << "/" << mon << "/" << day;
Pcre retime("%TIME%");
cmd = retime.replace(cmd,os_timenow.str());
Pcre redate("%DATE%");
cmd = redate.replace(cmd,os_datenow.str());

if (this->sim)
cout << "EXECUTE: " << cmd << endl;
else
//SYSTEM execute command function
system(cmd.c_str());

return cmd;
}


void CommandAction::simulate(bool sim) {
   this->sim = sim;
}

#endif

