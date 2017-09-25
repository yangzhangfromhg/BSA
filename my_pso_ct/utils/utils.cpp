#include"utils.h"

double get_uniform_rand(double start, double end)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<double> dis(start, end);
	double ret = dis(gen);
	return ret;
}

double get_normal_rand(double start, double end)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::normal_distribution<double> dis(start, end);
	double ret = dis(gen);
	return ret;
}