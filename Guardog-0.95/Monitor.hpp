
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

FILE: Monitor.hpp

DESC: Monitor Abstract class definition

********************************************************************************/

#ifndef MONITOR_CLASS
#define MONITOR_CLASS

#include "Guard.hpp"


class Monitor {
public:
Monitor() {}
virtual ~Monitor() {}
virtual void process(Guard& guard) = 0;
//Monitor& operator=(Monitor mon);

protected:
void error(std::string msg);
//void load_config(const char *conf);

};


void Monitor::error(std::string msg)
{
    //std::ofstream ofs;
    //ofs.open(filename,ios::binary|ios::app);
    cout << msg << endl;
    exit(0);
}

/*
Monitor& operator=(Monitor& mon) {
   return mon;
}
*/

/*
void Agent::load_config(const char *conf)
{
  FILE *in;
  char key[512];
  char *value;
  in = fopen(conf,"r");
  do
  {
        value = parser.read_config(in,key,'=');
        if ((key[0] != '#') && (value != NULL))
        {
           printf("# %s : %s\n",key,value);
           if (strcmp(key,"log") == 0)
                logfile = value;
           else if (strcmp(key,"err") == 0)
           {
                errfile = value;
           }
           else if (strcmp(key,"pos") == 0)
           {
                posfile = value;
                //strcpy(posfile,value);
           }
           else if (strcmp(key,"interval") == 0)
           {
                interval = atoi(value);
           }
           else if (strcmp(key,"regex") == 0)
           {
                guard.addRule(value);
           }
           //else if (is_numeric(key) == 1)
           else if (strcmp(key,"action") == 0)
           {
                guard.addMapping(atoi(value));
           }
           else if (strcmp(key,"exec") == 0)
           {
                guard.addAction(0,value);
           }

        }
  }
  while (!parser.real_end(in));
  fclose(in);
}
*/


#endif

