
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

FILE: Action.hpp

DESC: Action Abstract Class Definition

********************************************************************************/


#ifndef ACTION_CLASS
#define ACTION_CLASS


//enum { ACTION_COMMAND,ACTION_HALT,ACTION_REBOOT,ACTION_SHUTDOWN };

//Alert Action
class Action {
public:
Action();
Action(std::string act); 
virtual ~ Action() {}
virtual std::string get() = 0;
//virtual void addMatch(std::string match) = 0;
void simulate(bool sim);
virtual std::string execute(unsigned long pos,Expression *ex) = 0;
friend std::ostream& operator<<(std::ostream& os,const Action& action);

protected:
std::string s;

bool sim;
};


Action::Action() {}

Action::Action(std::string act) 
: s(act) {}

void Action::simulate(bool sim) {
  this->sim = sim;
}

std::ostream& operator<<(std::ostream& os, const Action& action)
{
   os << action.s;
   return os;
}


#endif

