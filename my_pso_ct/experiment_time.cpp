#include<cstdlib>
#include<cstdio>
#include<iostream>
#include<io.h>

using namespace std;

int main(int argc, char * argv[])
{
	if(argc < 2)
	{
		std::cout << "parameter too little "<< std::endl;
		return 0;
	}
	int experiment_time = atoi(argv[1]);
	char * fitnessfile = argv[2];
	char * algorithm = argv[3];
	char * modelfile = argv[4];
	system("cls");
	
	if(_access(fitnessfile, 0) != -1)
	{
		if(remove(fitnessfile) == 0)
		{
			std::cout << "delete successfully" << std::endl;
		}
		else
		{
			std::cout << "fail to delete" << std::endl;
			return 0;
		}	
	}
	for(int i = 0; i < experiment_time; ++i)
	{
		char str3[50];
		sprintf(str3,"my_pso_ct.exe %s %s",algorithm,modelfile);	
		//std::cout << str3 << std::endl;
		const char * para_string = (const char *) str3;
		system(para_string);
	}	
	
	std::cout << "\nexperiment_time : "<< experiment_time
	          << "\nfitnessfile     : "<< fitnessfile
	          << "\nalgorithm       : "<< algorithm
	          << "\nmodelfile       : "<< modelfile << std::endl;
	return 0;
}