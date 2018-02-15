#LIB_FLAGS = -L. -lpcre++ -lParser -lExpression -lLogMonitor -lCommandAction -lGuard -lAgent 
LIB_FLAGS = -lpcre++ 
#-L -lrawtcp

all:   main

clean:
			@echo "Removing Application Binary..."
			@#rm -f librawtcp.so
			rm -f guardog

main:
			@#@echo "Compiling rawtcp..."
			@#gcc -c rawtcp.c -o librawtcp.so
			@#chmod 755 librawtcp.so
			@echo "Compiling Application..."
			g++ $(LIB_FLAGS) main.cpp -o guardog

