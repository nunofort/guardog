
// INSTALL pcre++ library and headers
// ON UBUNTU: apt-get install libpcre++-dev

//make a link to the c++ version source headers
//ln -s /usr/include/c++/4.4 /usr/include/c++/version

//#define __CPLUS_VERSION	"4.4"

#define __PROGRAM_VERSION	"0.95"

#include "Agent.hpp"

void about(const char *bin)
{
   printf("Guardog %s    Intrusion Dectection System (IDS)\n",__PROGRAM_VERSION);
   printf("                by Nuno Fortes (nunofort@gmail.com)\n\n");
   printf("syntax: %s -c [config_file]\n\n",bin);
   printf("syntax: %s -p [config_file] (print only info)\n\n",bin);
   printf("syntax: %s -t [config_file] [test_msg] (test msg against regex config)\n\n",bin);
}

int main(int argc, char *argv[])
{
   Agent agent;
   int interval;
   bool verbose = false;
   if (argc > 1) {
   for (int i = 1; i < argc; i++) {
   if (strcmp(argv[i],"-p") == 0) {
        agent.initialize(argv[i+1]);
	agent.print_only();
        i++;
   }
   else if (strcmp(argv[i],"-t") == 0) {
        agent.initialize(argv[i+1]);
	agent.test_msg(argv[i+2]);
        i+=2;
   }
   else if (strcmp(argv[i],"-c") == 0) {
        agent.setVerbose(verbose);
        agent.initialize(argv[i+1]);
        if (agent.getMode() == 2) {
	  if (fork() != 0) exit(1); // START RUN LIKE A DAEMON!
	  while (1)
	  {
	    interval = agent.start();
	    if (interval > 0) sleep(interval);
	    // TODO: SEND A KILL SIGNAL to STOP THIS GUY!
	  }
	}
        else interval = agent.start();
        i++;
   }
   else if (strcmp(argv[i],"-v") == 0) {
        verbose = true;   
   }
   else {
        cout << "invalid parameter: " << argv[i] << endl;
   }
   }
   }
   else 
   about(argv[0]); 
   return 0;
}


//g++ -lpcre++ main.cpp -o main

