#pragma once
#include"../common/PSO.h"
#include"Particle.h"

class MY_pso : public PSO 
{
public:
	MY_pso(SUT * s) 
	{
		sut = s;
	}
public:
	int * Evolve();
	void SetConfig(int par, int ite, double w, double f);
};