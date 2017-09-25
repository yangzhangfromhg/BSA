#include<cmath>
#include<random>
#include<ctime>

#define pi 3.1415926

class Bird 
{
public:
	Bird(int dim, int * ran) 
	{
		dimension = dim;
		range = ran;

		position = new int[dimension];
		pbest = new int[dimension];
	}

public:
	int dimension;   //the number of dimensions
	int * range;       //the number of values of each dimension
	
	std::mt19937 gen;
	//current position, 
	int * position;
	
	int fitness_now;

	//the best personal position (pBest) and its fitness value
	int *pbest;
	int fitness_pbest;

	//clear
	void clear();
public:
	// randomly initialize
	void randomInit();
	// reflect
	void reflectWall();
	// normal distribution
	double get_nor_distribution(double a, double b);
	double get_levy_distribution(double beta);
	// update pbest
	void setPbest(int fit);
	// forage
	void forage(double c1, double c2, const int * gBest);
	// vigilance
	void vigilance(int sumFit, Bird* bird_k,const double * bird_mean, int a1, int a2, int pop);
	// producer
	void producer();
	//scrounge
	void scrounge(int* bird_k_pbest,double FL);
};