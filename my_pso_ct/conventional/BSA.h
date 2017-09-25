#include<vector>
#include"../common/PSO.h"
#include"Bird.h"
#include"../utils/utils.h"

class BSA : public PSO 
{
public:
	BSA(SUT * s) 
	{
		sut = s;
		bird_mean = new double[sut->parameter];
	}
public:
	int * Evolve();
	void SetConfig(int par, int ite, double c1, double c2, double a1, double a2, int FQ);
	void setMaxMinBirdSumFitBirdMean(std::vector<Bird> * T);
	void clear();
public:
	int maxindex, minindex; //最好和最坏的小鸟索引
	int sumFit;             // 适应度值和
	double * bird_mean;       // 小鸟的平均位置
};