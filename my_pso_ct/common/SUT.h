#pragma once

#include<vector>
#include<iostream>
#include<thread>
#include"../conventional/Bird.h"
using namespace std;

#define SIZE 4

// �ṹ���� : λ����
typedef unsigned char byte ;
struct Bits
{
	byte bit0:1;
	byte bit1:1;
	byte bit2:1;
	byte bit3:1;
	byte bit4:1;
	byte bit5:1;
	byte bit6:1;
	byte bit7:1;
};
union MyByte
{
	byte allbyte ;
	struct Bits bit;
};

// �Ӹ�����Ϣ
struct SUBINFO
{
	// subway��ʾǿ�ȣ�subparameter��ʾ����������subposition[subparameter]��ʾ����λ��
	int subway ;
	int subparameter ;
	int *subposition ;
	// ���ڳ�ʼ��ʱ��ֵ�������AllS��λ��
	int coverIndex ;  //  ��AllS�����±�
	int coverSub ;    //  C( subparameter , subway )
};

class SUT
{
	// ���캯��
public:
	SUT( int p , const int *v , int t )
	{
		parameter = p ;
		value = new int[p] ;
		for( int i = 0 ; i < p ; i++ )
			value[i] = v[i] ;
		tway = t ;

		AllS = 0 ;
		coverMain = cal_combine( p , t ) ;  	
	    coverMax = coverMain ;
		testcaseCoverMax = coverMax ;
	};
	~SUT()
	{
		delete[] value ;
		if( AllS != 0 )
		{
			for( int i=0 ; i<coverMax ; i++ )
				delete[] AllS[i];
			delete[] AllS;
			AllS = 0;
		}
		for( vector<SUBINFO>::iterator i = subInfo.begin() ; i != subInfo.end() ; i++ )
			delete[] (*i).subposition;
		subInfo.clear();
		for( vector<int*>::iterator i = seedInfo.begin() ; i != seedInfo.end() ; i++ )
			delete[] (*i) ;
		seedInfo.clear();
	}

// ����
public:
	// CA����
	int parameter ;
	int *value ;
	int tway ;
	
	// �Ӹ���
	vector<SUBINFO> subInfo ;

	// ���� 
	vector<int*> seedInfo ;
	
protected:
	// �洢����Ӧ�������������Լ���¼��δ���������
	MyByte **AllS ;    // ��λΪ0��ʾδ����

public:
	int coverMain ;    // ��¼Main������C(parameter,tway)
	int coverMax ;     // ��¼AllS��������Main+AllSub
	int SCountAll ;    // �ܵĴ����������
	int SCount ;       // δ���������
	int testcaseCoverMax ;   // һ��������������ܸ��������
	
// ����
public:
	// ������Ϣ��ʼ��
	void SetSub( const vector<SUBINFO> sub );
	void SetSeed( const vector<int*> seed ); 

	// ����AllS
    void GenerateS();
	void GenerateSSub();

	// ����fitness
	int FitnessValue( const int *test , int FLAG );
	int FitnessValueSub( const int *test , int FLAG );
	static void FitnessValueForP(SUT * sut, std::vector<Bird> * swarm, int pop, int width, int FLAG, int id);
	void FitnessValueByParallel(std::vector<Bird> * swarm, const int pop, const int width);

	// �ж��Ƿ񸲸�
    bool Covered( const int *pos , const int *sch , int FLAG );
	bool Covered( const int *pos , const int *sch , vector<SUBINFO>::const_iterator sub , int FLAG );

	// ����Ԥ����
	void preforseed();

private:
	// private �����ú���
	int cal_combine( int n , int m );
	int cal_combine2num( const int *c , const int n , const int m );    
	void cal_num2combine( int *c , int t , const int n , const int m );
};







