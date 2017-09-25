#include"MY_pso.h"
#include<iostream>
#include<vector>

void MY_pso::SetConfig(int par, int ite, double w, double f) 
{
	config.population = par;
	config.iteration = ite;
	config.weight = w;
	config.factor = f;
}

int * MY_pso::Evolve() 
{
	int *best = new int[sut->parameter];
	std::vector<Particle> T;           //粒子群
	int *gBest = new int[sut->parameter]; //种群最优
	int fitbest = 0;
	int it = 1; //迭代次数
	//初始化粒子群
	for (int i = 0; i < config.population; ++i) 
	{
		Particle a(sut->parameter, sut->value);
		a.RandomInit();
		T.push_back(a);
	}
	//gBest = T[0]
	std::vector<Particle>::iterator x = T.begin();
	for (int c = 0; c < sut->parameter; c++) 
	{
		gBest[c] = (*x).position[c];
	}
	bool adap_inertia = false;
	if (adap_inertia == -1) 
	{
		adap_inertia = true;
		config.weight = 0.9;
	}
	//生成一个测试用例
	while (it < config.iteration) 
	{
		//计算每个粒子的适应度值，并更新pbest, gbest
		for (std::vector<Particle>::iterator i = T.begin(); i != T.end(); ++i) 
		{
			int fit = sut->FitnessValue((*i).position, 0);
			if (fit == sut->testcaseCoverMax) 
			{
				for (int c = 0; c < sut->parameter; ++c) 
				{
					best[c] = (*i).position[c];
				}
				delete[] gBest;
				for (std::vector<Particle>::iterator i = T.begin(); i != T.end(); ++i) 
				{
					i->clear();
				}
				return best;
			}
			//跟新pBest
			if (fit > (*i).fitness_pbest) 
			{
				(*i).Setpbest(fit);
			}
			//更新gBest
			if (fit > fitbest) 
			{
				fitbest = fit;
				for (int c = 0; c < sut->parameter; ++c) 
				{
					gBest[c] = (*i).position[c];
				}
			}
		} //end for
		
		

		for (std::vector<Particle>::iterator i = T.begin(); i != T.end(); ++i) 
		{
			(*i).velocityUpdate(config.weight, config.factor, gBest);
			(*i).positionUpdate();
		}

		//iteration++
		it++;
		
		//config.weight = 0.9;
		
		if (adap_inertia) 
		{
			config.weight = 0.9 - 0.5 * ((double)it / (double) config.iteration);
		}

	} // end while

	for (int k = 0; k < sut->parameter; ++k)  // best = gBest.position
	{
		best[k] = gBest[k];
	}
	delete[] gBest;
	for (vector<Particle>::iterator i = T.begin(); i != T.end(); i++) 
	{
		i->clear();
	}
	T.clear();
	return best;
}