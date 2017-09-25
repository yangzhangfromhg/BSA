#include"SUT.h"

#include<iostream>
#include<cmath>
using namespace std ;

// ----------------------------------------------------------------------------
//  ���㺯�����������±��0��ʼ
// ----------------------------------------------------------------------------
// ����C(n,m)
int SUT::cal_combine( int n , int m )   
{
	int ret = 1 ;
	int p = n ;
	for( int x=1 ; x<=m ; x++ , p-- )
	{
		ret = ret * p ;
		ret = ret / x ;
	}
	return ret ;
}
// ����c[]��������ϵ��ֵ����е���ţ���n��������ѡm������� (n,m)
int SUT::cal_combine2num( const int *c , const int n , const int m )    
{
	int ret = cal_combine( n , m ) ;
	for( int i=0 ; i<m ; i++ )
		ret -= cal_combine( n-c[i]-1 , m-i );
	ret--;
	return ret ;                
}
// �����ֵ����t���Ĳ�����ϣ��������c�У���n��������ѡm������� (n,m)
void SUT::cal_num2combine( int *c , int t , const int n , const int m )  
{
	t++;                        // ����+1
	int j=1 , k ;
	for( int i=0 ; i<m ; c[i++]=j++ )
		for( ; t>( k = cal_combine( n-j , m-i-1 ) ) ; t-=k , j++ )
			;
	for( int p=0 ; p<m ; p++ )   // ���-1
		c[p]--;
}

// ----------------------------------------------------------------------------
// SUT��ʼ��
// ----------------------------------------------------------------------------
// �Ӹ���
void SUT::SetSub( const vector<SUBINFO> sub )
{
	for( vector<SUBINFO>::const_iterator i = sub.begin() ; i != sub.end() ; i++ )
	{
		SUBINFO temp ;
		temp.subparameter = (*i).subparameter ;
		temp.subway = (*i).subway ;
		temp.subposition = new int[temp.subparameter] ;
		for( int k=0 ; k<temp.subparameter ; k++ )
			temp.subposition[k] = (*i).subposition[k] ;
		subInfo.push_back(temp);
	}
	// ��������
	for( vector<SUBINFO>::iterator i = subInfo.begin() ; i != subInfo.end() ; i++ )
	{
		(*i).coverIndex = coverMax ;
		(*i).coverSub = cal_combine( (*i).subparameter , (*i).subway );
		coverMax += (*i).coverSub ;
	}
	// һ������������า����
	testcaseCoverMax = coverMax ;
}

// ����
void SUT::SetSeed( const vector<int*> seed )
{
	for( vector<int*>::const_iterator i = seed.begin() ; i != seed.end() ; i++ )
	{
		int *temp = new int[parameter] ;
		for( int k=0 ; k<parameter ; k++ )
			temp[k] = (*i)[k] ;
		seedInfo.push_back(temp);
	}
}

// ----------------------------------------------------------------------------
// ����Ԥ����������Covered����Evolveʱ�ٿ��Ǽ���
// ----------------------------------------------------------------------------
void SUT::preforseed()
{
	for( vector<int*>::const_iterator i = seedInfo.begin() ; i != seedInfo.end() ; i++ )
		FitnessValue((*i),1);
}


// ----------------------------------------------------------------------------
// ����AllS�ռ䣬������
// ----------------------------------------------------------------------------
void SUT::GenerateS()
{
	// ��ʼ��
	if( AllS != 0 )
	{
		for( int i=0 ; i<coverMax ; i++ )
			delete[] AllS[i];
		delete[] AllS;
		AllS = 0;
	}
	SCount = 0 ;

	// ����coverMax��
	AllS = new MyByte*[coverMax] ;

	// ���ֵ����������в������������
	int *temp = new int[tway] ;      // ������¼
	int *temp_max = new int[tway];   // �������ֵ
	for( int k=0 ; k<tway ; k++ )  // ��ʼ��
	{
		temp[k] = k ;
		temp_max[k] = parameter - tway + k ;
	}

	int end = tway - 1 ;
	int ptr = end ;

	int already = 0 ;
	while( already < coverMain )
	{
		//
		// ����temp����ʱtemp[]����˲������
		//

		// �����漰��������ϸ���
		int allcomb = 1 ;
		for( int p=0 ; p<tway ; p++ )
			allcomb = allcomb * value[temp[p]] ;
		// �����µ�1��
		int column = (int)ceil((double)allcomb/(double)8) ; 
		AllS[already] = new MyByte[column];
		// ��ʼ����ȫΪ0
		for( int k=0 ; k<column ; k++ )
			AllS[already][k].allbyte = 0x00 ;

		// �����ܵ���ϸ���
		SCount += allcomb ;

		//
		// ����һ�����
		//
		temp[end] = temp[end] + 1 ;  // ĩλ��1
		ptr = end ;
		while( ptr > 0 )
		{
			if( temp[ptr] > temp_max[ptr] )  // ������λ�������ֵ
			{
				temp[ptr-1] = temp[ptr-1] + 1 ;   // ǰһλ��1
				ptr-- ;
			}
			else
				break ;
		}
		if( temp[ptr] <= temp_max[ptr])  // ����λֵ������󣬺���ÿλ��ǰһλ�����ϼ�1
		{
			for( int i=ptr+1 ; i<tway ; i++ ) 
				temp[i] = temp[i-1] + 1 ;
		}
		already++ ;
	}

	delete[] temp ;
	delete[] temp_max ;

	// �����Ӹ���
	if( subInfo.size() != 0 )
		GenerateSSub();

	// ��������
	if( seedInfo.size() !=0 )
		preforseed();

	// ��¼�ܵĴ����������
	SCountAll = SCount ;
}

// AllS��Sub����
void SUT::GenerateSSub()
{
	// ��ÿ��sub
	for( vector<SUBINFO>::iterator i = subInfo.begin() ; i != subInfo.end() ; i++ )
	{
		int subway = (*i).subway ;
		int subparameter = (*i).subparameter ;
		int *subposition = (*i).subposition ;

		// ���ֵ����������в������������
		int *subtemp = new int[subway] ;      // ������¼
		int *subtemp_max = new int[subway];   // �������ֵ
		for( int k=0 ; k<subway ; k++ )  // ��ʼ��
		{
			subtemp[k] = k ;
			subtemp_max[k] = subparameter - subway + k ;
		}

		int subend = subway - 1 ;
		int subptr = subend ;

		int subalready = 0 ;
		int subrow = (*i).coverIndex ;
		while( subalready < (*i).coverSub )
		{
			//
			// ����subtemp
			// subtemp[]�ǰ�[0,1,...]��ŵģ�subtemp[]�ĸ�ֵ������subposition[]�е�λ��
			//

			// �����漰��������ϸ���
			int suballcomb = 1 ;
			for( int p=0 ; p<subway ; p++ )
				suballcomb = suballcomb * value[subposition[subtemp[p]]] ;
			// �����µ�1��
			int subcolumn = (int)ceil((double)suballcomb/(double)8) ; 
			AllS[subrow] = new MyByte[subcolumn];
			// ��ʼ����ȫΪ0
			for( int k=0 ; k<subcolumn ; k++ )
				AllS[subrow][k].allbyte = 0x00 ;

			// �����ܵ���ϸ���
			SCount += suballcomb ;

			//
			// ����һ�����
			//
			subtemp[subend] = subtemp[subend] + 1 ;  // ĩλ��1
			subptr = subend ;
			while( subptr > 0 )
			{
				if( subtemp[subptr] > subtemp_max[subptr] )  // ������λ�������ֵ
				{
					subtemp[subptr-1] = subtemp[subptr-1] + 1 ;   // ǰһλ��1
					subptr-- ;
				}
				else
					break ;
			}
			if( subtemp[subptr] <= subtemp_max[subptr])  // ����λֵ������󣬺���ÿλ��ǰһλ�����ϼ�1
			{
				for( int i=subptr+1 ; i<subway ; i++ ) 
					subtemp[i] = subtemp[i-1] + 1 ;
			}
			// row++
			subrow++;
			subalready++ ;
		}
		delete[] subtemp ;
		delete[] subtemp_max ;

	} // end for each sub
}

// ----------------------------------------------------------------------------
// �����������test��δ������϶Լ�S���ܸ��ǵ���϶���
// ���FLAG=0ֻ���㲻�޸ģ�FLAG=1�򽫸��ǵ�����������Ϊ�Ѹ���
// ���룺һ����������test�����FLAG
// ��������������
// ----------------------------------------------------------------------------
int SUT::FitnessValue( const int *test , int FLAG )
{
	int num = 0 ;   // ����ֵ

	// ���ΰ��ֵ�������test�ĸ�����������Ȼ���AllS�����ж��Ƿ񸲸ǣ�O(C(par,tway))

	int *pos = new int[tway] ;      // ����������
	int *pos_max = new int[tway];   // �������ֵ
	for( int k=0 ; k<tway ; k++ )  // ��ʼ��
	{
		pos[k] = k ;
		pos_max[k] = parameter - tway + k ;
	}
	int end = tway - 1 ;
	int ptr = end ;

	int *sch = new int[tway] ;  // �洢ȡֵ�����

	for( int row = 0 ; row < coverMain ; row++ )
	{
		// �õ�������
		for( int k=0 ; k<tway ; k++ )
			sch[k] = test[pos[k]] ;
		// �ж��Ƿ񸲸�
		if( !Covered( pos , sch , FLAG ) )
			num++ ;

		// ����һ�¸�
		pos[end] = pos[end] + 1 ;  // ĩλ��1
		ptr = end ;
		while( ptr > 0 )
		{
			if( pos[ptr] > pos_max[ptr] )  // ������λ�������ֵ
			{
				pos[ptr-1] = pos[ptr-1] + 1 ;   // ǰһλ��1
				ptr-- ;
			}
			else
				break ;
		}
		if( pos[ptr] <= pos_max[ptr])  // ����λֵ������󣬺���ÿλ��ǰһλ�����ϼ�1
		{
			for( int i=ptr+1 ; i<tway ; i++ ) 
				pos[i] = pos[i-1] + 1 ;
		}
	}
	delete[] pos;
	delete[] pos_max;
	delete[] sch;

	if( subInfo.size() !=  0 )
		num += FitnessValueSub( test , FLAG );

	return num ;
}

// Sub����
int SUT::FitnessValueSub( const int *test , int FLAG )
{
	int subnum = 0 ;   // ����ֵ

	// ��ÿ��Sub
	for( vector<SUBINFO>::iterator i = subInfo.begin() ; i != subInfo.end() ; i++ )
	{
		int subway = (*i).subway ;
		int subparameter = (*i).subparameter ;
		int *subposition = (*i).subposition ;
		// ���ΰ��ֵ�������test�ĸ�����������Ȼ���AllS�����ж��Ƿ񸲸ǣ�O(C(subpar,subway))

		int *subpos = new int[subway] ;      // ����������
		int *subpos_max = new int[subway];   // �������ֵ
		for( int k=0 ; k<subway ; k++ )  // ��ʼ��
		{
			subpos[k] = k ;
			subpos_max[k] = subparameter - subway + k ;
		}
		int subend = subway - 1 ;
		int subptr = subend ;

		int *subsch = new int[subway] ;  // �洢ȡֵ�����

		for( int row = 0 ; row < (*i).coverSub ; row++ )
		{
			// �õ�������
			for( int k=0 ; k<subway ; k++ )
				subsch[k] = test[subposition[subpos[k]]] ;

			// �ж��Ƿ񸲸�
			if( !Covered( subpos , subsch , i , FLAG ) )
				subnum++ ;

			// ����һ�¸�
			subpos[subend] = subpos[subend] + 1 ;  // ĩλ��1
			subptr = subend ;
			while( subptr > 0 )
			{
				if( subpos[subptr] > subpos_max[subptr] )  // ������λ�������ֵ
				{
					subpos[subptr-1] = subpos[subptr-1] + 1 ;   // ǰһλ��1
					subptr-- ;
				}
				else
					break ;
			}
			if( subpos[subptr] <= subpos_max[subptr])  // ����λֵ������󣬺���ÿλ��ǰһλ�����ϼ�1
			{
				for( int i=subptr+1 ; i<subway ; i++ ) 
					subpos[i] = subpos[i-1] + 1 ;
			}
		}
		delete[] subpos;
		delete[] subpos_max;
		delete[] subsch;
	}

	return subnum ;
}

void SUT::FitnessValueForP(SUT * sut, std::vector<Bird> * swarm, int pop, int width, int FLAG, int id)
{
	int length_per_thread = (pop % SIZE == 0) ? pop / SIZE : pop + SIZE - 1 / SIZE;
	int index = id * length_per_thread;
	for(int i = index; i < index + length_per_thread; i++)
	{
		if(i < pop)
		{
			const int * test = (*swarm)[i].position;
			(*swarm)[i].fitness_now = sut->FitnessValue(test, 0);
		}
	}
}

void SUT::FitnessValueByParallel(std::vector<Bird> * swarm, const int pop, const int width)
{
	std::thread * ths;
	ths = new std::thread[SIZE];
	for(int c = 0; c < SIZE; c++)
	{
		ths[c] = std::thread(FitnessValueForP, this, swarm, pop, width, 0, c);
	}
	for(int c = 0; c < SIZE; c++)
	{
		ths[c].join();
	}
	delete[] ths;
}

// ----------------------------------------------------------------------------
// ����һ��ȡֵ����Ƿ񸲸ǣ�����pos�洢�����ֵ���ţ�sch�洢��Ӧȡֵ
// ���룺pos[tway]��sch[tway]��FLAG=1���ʾδ����ʱ��Ҫ����λ��1
// �����trueΪ�Ѹ��ǣ�falseΪδ����
// ----------------------------------------------------------------------------
bool SUT::Covered( const int *pos , const int *sch , int FLAG )
{
	// ͨ��position����õ��кţ�schema����õ��кţ���AllS�ó�������ӽ�O(1)

	bool ret = true ;  // ����ֵ

	// �洢������Ϣ
	int row = 0 ;
	int column = 0 ;       // ��һ��BYTE
	int column_bit = 0 ;   // ��һ��bit

	// ����row���±��0��ʼ����
	row = cal_combine2num( pos , parameter , tway );

	// ����column���±��0��ʼ����
	int Index = 0 ;
	int it = 0 ;
	for( int i=0 ; i<tway ; i++ )
	{
		it = sch[i] ;
		for( int j=i+1 ; j<tway ; j++ )
			it = value[pos[j]] * it ;
		Index += it ;
	}

	column = Index / 8 ;
	column_bit = Index % 8 ;

	MyByte byte = AllS[row][column] ;
	switch( column_bit )
	{
	case 0:
		if( !byte.bit.bit0 )   
		{
			ret = false ;
			if( FLAG==1 ) 
			{
				AllS[row][column].bit.bit0 = 1 ;
				SCount--;
			}
		}
		break ;
	case 1:
		if( !byte.bit.bit1 )   
		{
			ret = false ;
			if( FLAG==1 ) 
			{
				AllS[row][column].bit.bit1 = 1 ;
				SCount--;
			}
		}
		break ;
	case 2:
		if( !byte.bit.bit2 )   
		{
			ret = false ;
			if( FLAG==1 ) 
			{
				AllS[row][column].bit.bit2 = 1 ;
				SCount--;
			}
		}
		break ;
	case 3:
		if( !byte.bit.bit3 )   
		{
			ret = false ;
			if( FLAG==1 ) 
			{
				AllS[row][column].bit.bit3 = 1 ;
				SCount--;
			}
		}
		break ;
	case 4:
		if( !byte.bit.bit4 )   
		{
			ret = false ;
			if( FLAG==1 ) 
			{
				AllS[row][column].bit.bit4 = 1 ;
				SCount--;
			}
		}
		break ;
	case 5:
		if( !byte.bit.bit5 )   
		{
			ret = false ;
			if( FLAG==1 ) 
			{
				AllS[row][column].bit.bit5 = 1 ;
				SCount--;
			}
		}
		break ;
	case 6:
		if( !byte.bit.bit6 )   
		{
			ret = false ;
			if( FLAG==1 ) 
			{
				AllS[row][column].bit.bit6 = 1 ;
				SCount--;
			}
		}
		break ;
	case 7:
		if( !byte.bit.bit7 )   
		{
			ret = false ;
			if( FLAG==1 ) 
			{
				AllS[row][column].bit.bit7 = 1 ;
				SCount--;
			}
		}
		break ;
	}
	return ret ;
}

// Sub����
// ����pos�洢�Ӹ����ж�Ӧ�ֵ���ţ���ʵ����λ����sub.subposition�У�sch�洢��Ӧȡֵ
bool SUT::Covered( const int *pos , const int *sch , vector<SUBINFO>::const_iterator sub , int FLAG )
{
	int subway = (*sub).subway ;
	int subparameter = (*sub).subparameter ;
	int *subposition = (*sub).subposition ;

	// ͨ��position����õ��кţ�schema����õ��кţ���AllS�ó�������ӽ�O(1)

	bool subret = true ;  // ����ֵ

	// �洢������Ϣ
	int subrow = (*sub).coverIndex ;
	int subcolumn = 0 ;       // ��һ��BYTE
	int subcolumn_bit = 0 ;   // ��һ��bit

	// ����row���±��0��ʼ����
	subrow += cal_combine2num( pos , subparameter , subway );

	// ����column���±��0��ʼ����
	int subIndex = 0 ;
	int it = 0 ;
	for( int i=0 ; i<subway ; i++ )
	{
		it = sch[i] ;
		for( int j=i+1 ; j<subway ; j++ )
			it = value[subposition[pos[j]]] * it ;
		subIndex += it ;
	}

	subcolumn = subIndex / 8 ;
	subcolumn_bit = subIndex % 8 ;

	MyByte byte = AllS[subrow][subcolumn] ;
	switch( subcolumn_bit )
	{
	case 0:
		if( !byte.bit.bit0 )   
		{
			subret = false ;
			if( FLAG==1 ) 
			{
				AllS[subrow][subcolumn].bit.bit0 = 1 ;
				SCount--;
			}
		}
		break ;
	case 1:
		if( !byte.bit.bit1 )   
		{
			subret = false ;
			if( FLAG==1 ) 
			{
				AllS[subrow][subcolumn].bit.bit1 = 1 ;
				SCount--;
			}
		}
		break ;
	case 2:
		if( !byte.bit.bit2 )   
		{
			subret = false ;
			if( FLAG==1 ) 
			{
				AllS[subrow][subcolumn].bit.bit2 = 1 ;
				SCount--;
			}
		}
		break ;
	case 3:
		if( !byte.bit.bit3 )   
		{
			subret = false ;
			if( FLAG==1 ) 
			{
				AllS[subrow][subcolumn].bit.bit3 = 1 ;
				SCount--;
			}
		}
		break ;
	case 4:
		if( !byte.bit.bit4 )   
		{
			subret = false ;
			if( FLAG==1 ) 
			{
				AllS[subrow][subcolumn].bit.bit4 = 1 ;
				SCount--;
			}
		}
		break ;
	case 5:
		if( !byte.bit.bit5 )   
		{
			subret = false ;
			if( FLAG==1 ) 
			{
				AllS[subrow][subcolumn].bit.bit5 = 1 ;
				SCount--;
			}
		}
		break ;
	case 6:
		if( !byte.bit.bit6 )   
		{
			subret = false ;
			if( FLAG==1 ) 
			{
				AllS[subrow][subcolumn].bit.bit6 = 1 ;
				SCount--;
			}
		}
		break ;
	case 7:
		if( !byte.bit.bit7 )   
		{
			subret = false ;
			if( FLAG==1 ) 
			{
				AllS[subrow][subcolumn].bit.bit7 = 1 ;
				SCount--;
			}
		}
		break ;
	}
	return subret ;
}



