#include<iostream>
#include<random>
#include<chrono>
#include<cmath>
#include<ctime>
#include<functional>
#define pi 3.1415926


std::mt19937 generator;

double get_normal_distribution(double a, double b)
{	
	std::normal_distribution<double> distribution(a, b);	
	return distribution(generator);
}

double levy(double beta)
{
	
	double num = tgamma(1+beta)*sin(pi * beta/2);
	double den = tgamma((1+beta)/2)*beta*std::pow(2, (beta-1)/2);
	
	double sigma_u = std::pow((num/den), 1/beta);
	
	double u = get_normal_distribution(0, sigma_u);
	
	double v = get_normal_distribution(0, 1);
	
	double z = u / std::pow(std::abs(v), 1/beta);
	
	return z;
}

int main(int argc, char * argv[])
{	
	for(int i = 0; i < 10; i++)
	{
		std::cout << " normal distribution : " << get_normal_distribution(0.0, 1.0);
		std::cout << "  levy : " << levy(1.5) << std::endl;
	}
	return 0;
}