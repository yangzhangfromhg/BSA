#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<algorithm>

#include"common/SUT.h"
#include"conventional/MY_pso.h"
#include"conventional/BSA.h"

using namespace std;


// parameters
string alg;			// specified algorithm
string filename;	// input.model and output.ca
SUT *sut;			// the software under test

int parseOptions(int argcf, char **argvf)
{
	if (argcf != 3) {
		cout << "Invalid parameter. Failed." << endl;
		return 0;
	}

	// get algorithm
	alg = argvf[1];
	string algs[11] = { "cpso","MY_pso" , "cclpso", "capso", "cdmspso", "dpso", "dtvac", "dclpso", "dapso", "ddmspso", "bsa"};
	if (find(algs, algs + 11, alg) == algs + 11)
	{
		cout << "Invalid parameter: algorithm. Failed." << endl;
		return 0;
	}

	// get SUT from filename
	filename = argvf[2];
	ifstream infile(filename + ".model");

	if (infile.is_open())
	{
		string line;

		// parameter
		getline(infile, line);
		int par = atoi(line.substr(line.rfind("parameter") + 10).c_str());

		// value
		int *val = new int[par];
		getline(infile, line);
		string vs = line.substr(line.rfind("value") + 6);
		stringstream ssin(vs);
		int i = 0;
		while (ssin.good() && i < par)
		{
			ssin >> val[i];
			i++;
		}

		// t-way
		getline(infile, line);
		int tway = atoi(line.substr(line.rfind("tway") + 5).c_str());

		// initialize SUT
		sut = new SUT(par, val, tway);

		if (getline(infile, line)) 
		{
			std::vector<SUBINFO> subs;
			int variable = atoi(line.substr(line.rfind("variable") + 9).c_str());
			for (int c = 0; c < variable; c++) 
			{
				SUBINFO sub;
				//subparameter
				getline(infile, line);
				sub.subparameter = atoi(line.substr(line.rfind("subparameter")+ 13).c_str());
				//subtway
				getline(infile, line);
				sub.subway = atoi(line.substr(line.rfind("subway")+ 8).c_str());
				// subposition
				int *  subposition = new int[sub.subparameter];
				getline(infile, line);
				std::string subv = line.substr(line.rfind("subposition") + 12);
				std::stringstream ss(subv);
				int k = 0;
				while (ss.good() && k < sub.subparameter) 
				{
					ss >> subposition[k];
					k++;
				}
				sub.subposition = subposition;
				subs.push_back(sub);
			}
			sut->SetSub(subs);
		}


		return 1;
	}
	else
	{
		cout << "Unable to open model file. Failed" << endl;
		return 0;
	}

}

int main(int argc, char* argv[]) 
{
	srand((unsigned int)time(0));

	if (!parseOptions(argc, argv))
		return 0;

	// initialize the PSO generator
	PSO *pso;
	if (alg == "MY_pso")
	{
		pso = new MY_pso(sut);
	}
	else if (alg == "bsa") 
	{
		pso = new BSA(sut);
	}
	else
	{
		cout << "Algorithm error. Failed." << endl;
		return 0;
	}

	// set default configuration
	if (alg == "cpso" || alg == "cclpso" || alg == "cdmspso" || alg == "MY_pso")
		pso->SetConfig(80, 250, 0.9, 1.3);
	else if (alg == "ctvac" || alg == "capso")
		pso->SetConfig(80, 250);
	else if (alg == "dpso" || alg == "dclpso" || alg == "ddmspso")
		pso->SetConfig(80, 250, 0.5, 1.3, 0.5, 0.3, 0.7);
	else if (alg == "dtvac" || alg == "dapso")
		pso->SetConfig(80, 250, 0.5, 0.3, 0.7);
	else if (alg == "bsa")
		pso->SetConfig(900, 500, 2.0, 2.0, 1.0, 1.0, 5);
	
	
	

	//system("pause");
	// run
	pso->PSOEvolve();

	// output the result
	cout << "size: " << pso->PSO_SIZE << ", time: " << pso->PSO_TIME << endl;

	// write to file
	pso->writeThisResult(filename + ".ca", alg);
	return 0;
}