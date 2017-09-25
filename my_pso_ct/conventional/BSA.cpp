#include"BSA.h"

void BSA::SetConfig(int par, int ite, double c1, double c2, double a1, double a2, int FQ)
{
	config.population = par;
	config.iteration = ite;
	config.c1 = c1;
	config.c2 = c2;
	config.a1 = a1;
	config.a2 = a2;
	config.FQ = FQ;
}

void BSA::setMaxMinBirdSumFitBirdMean(std::vector<Bird> * T)
{
	unsigned int size = T->size();
	int max_index = 0;
	int min_index = 0;
	int max_fitness = T->front().fitness_now;
	int min_fitness = T->front().fitness_now;
	int sum_fitness = 0;
	double * temp = new double[sut->parameter]; //未初始化为0 调试注意
	for (unsigned int i = 0; i < size; ++i) 
	{
		int current_fitness = (*T)[i].fitness_now;
		int temp_fitness = (*T)[i].fitness_pbest;
		sum_fitness += temp_fitness;
		if (max_fitness < current_fitness)
		{
			max_fitness = current_fitness;
			max_index = i;
		}
		if (min_fitness > current_fitness)
		{
			min_fitness = current_fitness;
			min_index = i;
		}

		if (i == 0) 
		{
			for (int c = 0; c < sut->parameter; ++c)
			{
				temp[c] = (*T)[i].position[c];
			}
		}
		else 
		{
			for (int c = 0; c < sut->parameter; ++c)
			{
				temp[c] += (*T)[i].position[c];
			}
		}	
	}

	//设置成员变量
	minindex = min_index;
	maxindex = max_index;
	sumFit = sum_fitness;
	for (int c = 0; c < sut->parameter; ++c) 
	{
		temp[c] = temp[c] / size;
		bird_mean[c] = temp[c];
	}
	delete[] temp;
}

void BSA::clear() 
{
	delete[] bird_mean;
}

int* BSA::Evolve() 
{
	int *best =new int[sut->parameter];
	std::vector<Bird> T;                    // bird swarm
	int *gBest = new int[sut->parameter];   // the best one of swarm
	int bestfit = 0;

	//initial
	for (int i = 0; i < config.population; ++i) 
	{
		Bird b(sut->parameter, sut->value);
		b.randomInit();
		T.push_back(b);
	}

	/*test
	std::cout << "initial : ";
	for (int c = 0; c < sut->parameter; ++c) 
	{
		std::cout << T[0].position[c] << " ";
	}
	std::cout << std::endl;*/
	//gBest = T[0]
	std::vector<Bird>::iterator x = T.begin();
	for (int c = 0; c < sut->parameter; ++c) 
	{
		gBest[c] = (*x).position[c];
	}
	// iteration time
	int it = 0;

	// generate one test one time
	while (true) 
	{
		/*
		std::cout << "during iteration : " << it << " : ";
		for (int c = 0; c < sut->parameter; ++c)
		{
			std::cout << T[0].position[c] << " ";
		}
		std::cout << "\n";
		*/
		for (std::vector<Bird>::iterator i = T.begin(); i != T.end(); ++i) 
		{	
			int fit = sut->FitnessValue((*i).position, 0);
			(*i).fitness_now = fit;
			// if fitness(t) == coverMax, return
			if (fit == sut->testcaseCoverMax) 
			{
				for (int c = 0; c < sut->parameter; ++c) 
				{
					best[c] = (*i).position[c];
				}
				delete[] gBest;
				for (std::vector<Bird>::iterator i = T.begin(); i != T.begin(); ++i) 
				{
					i->clear();
				}
				T.clear();
				return best;
			}
			//update pBest
			if (fit > (*i).fitness_pbest) 
			{
				(*i).setPbest(fit);
			}
			//update gBest
			if (fit > bestfit) 
			{
				bestfit = fit;
				for (int c = 0; c < sut->parameter; c++) 
				{
					gBest[c] = (*i).position[c];
				}
			}
		}// end for
		
		//设置参数  minIndex maxIndex sumFit bird_mean
		setMaxMinBirdSumFitBirdMean(&T);
		
		// 终止条件
		if (it >= config.iteration) 
		{
			break;
		}
		//小鸟行为
		if (it % config.FQ != 0) 
		{		
			for (std::vector<Bird>::iterator i = T.begin(); i != T.end(); ++i)
			{
				double rand_bsa = ((double)(rand() % 1000) / 1000.0);
				double pro = ((double)(rand() % 1000) / 1000.0) * 0.2 + 0.4;
				if (rand_bsa < pro)
				{
					(*i).forage(config.c1, config.c2, gBest);
				}
				else
				{
					int k = int(((double)(rand() % 1000) / 1000.0) * double(config.population));
					(*i).vigilance(sumFit, &T[k], bird_mean, config.a1, config.a2, config.population);
				}
			}
		}
		else 
		{
			// producer and scrounge
			
			int choose = 0;
			if (minindex < 0.5 * config.population && maxindex < 0.5 * config.population) 
			{
				choose = 1;
			}
			else if (minindex > 0.5 * config.population && maxindex < 0.5 * config.population)
			{
				choose = 2;
			}
			else if (minindex < 0.5 * config.population && maxindex > 0.5 * config.population)
			{
				choose = 3;
			}
			else if (minindex > 0.5 * config.population && maxindex > 0.5 * config.population)
			{
				choose = 4;
			}
			if (choose < 3) 
			{
				for (int i = int(config.population * 0.5 + 1); i < config.population; ++i) 
				{
					T[i].producer();
				}
				//std::cout << "< 3 bird producer out" << std::endl;
				if (choose == 1) 
				{
					T[minindex].producer();
				}
				for (int i = 0; i <= int(config.population * 0.5); ++i)
				{
					double FL = ((double)(rand() % 1000) / 1000.0) * 0.4 + 0.5;
					if (choose == 2 || minindex != i) 
					{
						int k = int(((double)(rand() % 1000) / 1000.0) * config.population * 0.5) + int(config.population * 0.5);
						T[i].scrounge(T[k].pbest, FL);
					}
				}
				//std::cout << "< 3 bird scrounge out" << std::endl;
			}
			else 
			{	
				for (int i = 0; i <= int(config.population * 0.5); ++i)
				{
					T[i].producer();
				}
				//std::cout << ">= 3 bird producer out" << std::endl;
				if (choose == 4)
				{
					T[minindex].producer();
				}
				for (int i = int(config.population * 0.5) + 1; i < config.population; ++i)
				{
					double FL = ((double)(rand() % 1000) / 1000.0) * 0.4 + 0.5;
					if (choose == 3 || minindex != i)
					{
						int k = int(((double)(rand() % 1000) / 1000.0) * config.population * 0.5);
						T[i].scrounge(T[k].pbest, FL);
					}
				}
				//std::cout << ">= 3 bird scrounge out" << std::endl;
			}
		}// 行为终止
		//更新迭代次数
		it++;
	}//endl while

	for (int k = 0; k < sut->parameter; ++k) 
	{
		best[k] = gBest[k];
	}
	delete[] gBest;
	//delete[] bird_mean;
	for (std::vector<Bird>::iterator i = T.begin(); i != T.end(); ++i) 
	{
		i->clear();
	}
	T.clear();
	return best;
}

