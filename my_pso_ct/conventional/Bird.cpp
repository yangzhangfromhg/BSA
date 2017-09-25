
#include<cfloat>

#include"Bird.h"
#include"../utils/utils.h"

void Bird::clear() 
{
	delete[] position;
	delete[] pbest;
}

// initialize
void Bird::randomInit() 
{
	for (int i = 0; i < dimension; ++i) 
	{
		//double temp = get_uniform_rand(0, 1.0);
		position[i] = (int)(((double)(rand() % 1000) / 1000.0) * range[i]);
	}
	fitness_now = 0;
	for (int i = 0; i < dimension; ++i) 
	{
		pbest[i] = position[i];
	}
	fitness_pbest = 0;
}

void Bird::reflectWall() 
{
	

}

// normal distribution
double Bird::get_nor_distribution(double a, double b)
{
	std::normal_distribution<double> distribution(a, b);
	return distribution(gen);
}
// levy distribution
double Bird::get_levy_distribution(double beta)
{
	double num = tgamma(1+beta) * sin(pi * beta/2);
	double den = tgamma((1+beta)/2) * beta * std::pow(2, (beta-1)/2);
	
	double sigma_u = std::pow((num/den), 1/beta);
	
	double u = get_nor_distribution(0, sigma_u);
	
	double v = get_nor_distribution(0, 1);
	
	double z = u / std::pow(std::abs(v), 1/beta);
	
	return z;
}

//update pbest
void Bird::setPbest(int fit) 
{
	for (int i = 0; i < dimension; ++i) 
	{
		pbest[i] = position[i];
	}
	fitness_pbest = fit;
}

void Bird::forage(double c1, double c2,const int* gBest) 
{
	for (int i = 0; i < dimension; ++i) 
	{
		double temp_1 = ((double)(rand() % 1000) / 1000.0);
		double temp_2 = ((double)(rand() % 1000) / 1000.0);
		position[i] = int(position[i] +
			c1 * temp_1 * (pbest[i] - position[i]) +
			c2 * temp_2 * (gBest[i] - position[i]));
		
		//reflecting Walls
		if (position[i] >= range[i]) 
		{
			//position[i] = 2 * range[i] - position[i] - 1;
			position[i] = range[i] - 1;
		}
		if (position[i] < 0) 
		{
			position[i] = 0;
		}
	}
}

void Bird::vigilance(int sumFit, Bird* bird_k,const double * Bird_mean, int a1, int a2, int pop) 
{
	double rand_1 = ((double)(rand() % 1000) / 1000.0); // 0~1 random
	double rand_2 = ((double)(rand() % 1000) / 1000.0) * 2 - 1; // -1~1 random
	for (int i = 0; i < dimension; ++i) 
	{
		position[i] = int(position[i] + 
			rand_1 * (Bird_mean[i] - position[i]) * 
			a1 * exp(-(fitness_pbest * pop / (sumFit + DBL_MIN))) + 
			rand_2 * (bird_k->pbest[i] - position[i]) * 
			a2 * exp(((fitness_pbest - bird_k->fitness_pbest) / (abs(bird_k->fitness_pbest - fitness_pbest) + DBL_MIN))
				* (bird_k->fitness_pbest * pop / (sumFit + DBL_MIN))));
		
		//reflecting Walls
		if (position[i] >= range[i])
		{
			//position[i] = 2 * range[i] - position[i] - 1;
			position[i] = range[i] - 1;
		}
		if (position[i] < 0)
		{
			position[i] = 0;
		}
	}
}

void Bird::producer()
{
	for (int i = 0; i < dimension; ++i) 
	{
		//double temp = (double)(rand()%1000/1000.0);
		double temp = get_nor_distribution(0, 1.0);
		position[i] = int(position[i] + (1 + temp));
		
		//reflecting Walls
		if (position[i] >= range[i])
		{
			//position[i] = 2 * range[i] - position[i] - 1;
			position[i] = range[i] - 1;
		}
		if (position[i] < 0)
		{
			position[i] = 0;
		}
	}
}

void Bird::scrounge(int * bird_k_pbest, double FL) 
{
	for (int i = 0; i < dimension; ++i)
	{
		position[i] = int(position[i] + (bird_k_pbest[i] - position[i]) * FL);
		
		//reflecting Walls
		if (position[i] >= range[i])
		{
			//position[i] = 2 * range[i] - position[i] - 1;
			position[i] = range[i] - 1;
		}
		if (position[i] < 0)
		{
			position[i] = 0;
		}
	}
}