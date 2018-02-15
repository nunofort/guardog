
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

FILE: Expression.hpp

DESC: Expression pattern class

********************************************************************************/


#ifndef EXPRESSION_CLASS
#define EXPRESSION_CLASS

#include <pcre++.h>
#include <c++/version/string>
#include <c++/version/sstream>
#include <c++/version/iostream>

using namespace std;
using namespace pcrepp;

#include <time.h>
#include <sys/types.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/socket.h>
#include <regex.h>

class Expression {

public:
Expression(std::string regex);
~Expression() {}
bool match(std::string text);
int matches();
std::string getMatch(int i);
friend std::ostream& operator<<(std::ostream& os,const Expression& exp);

//friend Expression& operator=(const Expression& src);
//const Node& operator[](const Index& i) const;

private:
std::string s;
Pcre _regex; // MUST be a pointer to work!
//Or make Expression pointers outside!

};


Expression::Expression(std::string regex) 
: s(regex) {
_regex = Pcre(regex);
}

/*
C API
bool Expression::isValid(const char *rule)
{
  bool valid = true;
  regex_t cregex;
  int errcode;
  size_t errmsg_size;
  char errmsg[512];
  FILE *fp;
  fp = fopen(errfile,"a");
  // check regular expression integrity
  errcode = regcomp(&cregex, Pcre(rule), REG_EXTENDED);
  if (errcode != 0)
  {
     errmsg_size = regerror(errcode,  &cregex, errmsg, sizeof(errmsg));
     fprintf(fp, "ERROR: %s\n",errmsg);
     fprintf(fp, "ERROR: Shutting Down IDS.\n");
     fprintf(stderr,"ERROR: %s\n",errmsg);
     valid = false;
   }
   regfree(&cregex);
   fclose(fp);
   return valid;
}
*/

bool Expression::match(std::string text) {
//int i = _regex.search(text);
//return _regex.matched();
return _regex.search(text);
}

int Expression::matches() {
return _regex.matches();
}

std::string Expression::getMatch(int i) {
return _regex.get_match(i);
}

std::ostream& operator<<(std::ostream& os, const Expression& exp)
{
   os << exp.s;
   return os;
}

/*
Expression& operator=(const Expression& src)
{
   _regex = src._regex;
   return this;
}


unsigned int Expression::size() 
{
   return this._;
}

const Node& Expression::operator[](const Index& i) const
{
   //assert(i < newNode);
   return count[i];
}
*/

#endif
