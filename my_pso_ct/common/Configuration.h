#pragma once

#define MAX 999999

struct Configuration
{
	unsigned int random ;   // 随机数种子

	// 算法参数
	int population ;
	int iteration ;
	double weight ;
    double factor ;
	double c1, c2;
	double a1, a2;
	int FQ;
};