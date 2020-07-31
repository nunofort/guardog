
ABOUT
------

The intend of this project is to develop a simple but powerfull  Intrusion Dectection System (IDS) by inspecting messages from Log files, Network packets, or from other sources. It uses Perl Regular Expressions to check for any bad messages.   


INSTALL
--------

INSTALL pcre++ library and headers
ON UBUNTU: apt-get install libpcre++-dev

make a link to the c++ version source headers
ln -s /usr/include/c++/4.4 /usr/include/c++/version


USAGE
-------

./guardog -c guard.conf

with verbose option
./guardog -v -c guard.conf


REGEX
-----
regex = main regular expression match

%1 = expression for substring[1] to match
%2 = expression for substring[2] to match 
%3 = ..

ACTION
-------

action=action for first substring expression
action=action for second substring expression
action=..



CHANGES
---------

0.95
    
    - FIX Segmentation fault on config load
    - NEGATE REGEX support
    - MULTIPLE TIMES MATCH TEXT for actions 
    - POSITION Keyword option for actions
    - DATE and TIME Keyword option for actions
    - SIMULATE support for actions
    - CONFIG POSITION=0 support for LOG ROTATE FILES
    - CONVERT some C routines to C++ STL 
    - set program log file option 
    - set program exit 
    - more debugging information
    - some minor fixes
    - some code cleanups
