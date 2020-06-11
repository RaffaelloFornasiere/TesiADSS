#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string>
#include <iostream>
#include <sys/types.h> /* for pid_t */
#include <sys/wait.h> /* for wait */
#include <signal.h>

int childProc(const char *command);

int main(int argc, char *argv[])
{
	const char* command = "./main 	-c ../../../files/InpuFiles/Circuits/adder16.vhdl \
									-l ../../../files/InputFiles/Cell_Libraries/my_cell_library.hs \
									-f ../../../files/ipc/ipc2 \
									-p 100 \
									-p 20 \
									-p 30 \
									-r 0.7 \
									-s 1";

	pid_t process = fork();
	if(process == 0)
	{		
		int res = childProc(command);
		std::cerr << "childProc: " << res;
		return 0;
	}
	else if (childProc < 0)
	{
		perror("can't fork");
		exit(1);
	}
/*
	int n;
	std::cout << "inserisci un numero" << std::endl;
	std::cin >> n;
	std::cout << "numero inserito: " << n << std::endl;
*/
	std::cout << "all ok" << std::endl;

	waitpid(process, 0, 0);
	return 0;
}

int childProc(const char *command)
{
	return system(command);	
}