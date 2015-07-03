#include "stdafx.h"

#include "Partition.h"
//================================================
/////////////Constructors/Destructors/////////////
//================================================

Partition::Partition(std::vector<MultiLayerNN::LearnInfo>& LearnBook)
{
	mUniverse = LearnBook;
}

Partition::~Partition()
{
	mUniverse.clear();

	mLearnPartition.clear();

	mTestPartition.clear();

	mIndexUsed.clear();
}

//================================================
/////////////	Partition Methods	 /////////////
//================================================

void Partition::InitLeaveKOut(unsigned int k)
{
	assert( k < mUniverse.size());//whouldnt have sense fi other way

	mK = k;

	srand( time(NULL) );

	mIndexUsed.resize(mUniverse.size());
	for(int i=0; i < mIndexUsed.size() ; ++i)
		mIndexUsed[i] = i;

	mTestPartition.resize(mK, NULL);//first test cases
	for(int i=0; i < mK ; ++i)
	{
		int index = rand() % mIndexUsed.size();
		mTestPartition[i] = &mUniverse[mIndexUsed[index]];
		mIndexUsed.erase(mIndexUsed.begin()+index);
	}

	mLearnPartition.resize(mUniverse.size()-k, NULL);//first learn cases
	for(int i=0; i < mIndexUsed.size() ; ++i)
		mLearnPartition[i] = &mUniverse[mIndexUsed[i]];

	return;
}

bool Partition::NextLeaveKOut()
{
	if( mIndexUsed.size() == 0 )
		return false;//end

	int partitionSize = min(mIndexUsed.size(),mK);//if we dont have k elements left..

	for(int i=0; i < mTestPartition.size() ; ++i)//get me test cases, and the rest to learn
		mLearnPartition.push_back( mTestPartition[i] );//put the previous test into learn

	if(partitionSize < mK)
		mTestPartition.resize(partitionSize);

	for(int i=0; i < partitionSize ; ++i)//get me test cases, and the rest to learn
	{
		int index = rand() % mIndexUsed.size();//get a index for test

		//mLearnPartition.push_back( mTestPartition[i] );//put the previous test into learn

		mTestPartition[i] = &mUniverse[mIndexUsed[index]];//put new test from the universe

		mLearnPartition.erase(std::find(mLearnPartition.begin(),mLearnPartition.end(),mTestPartition[i]));//erase cur test case

		mIndexUsed.erase( mIndexUsed.begin()+index );//erase this index
	}

	return true;//we are not finish
}

void Partition::GetShuffle(std::vector<int>& shuffle)
{
	int* trivialOrder = new int[shuffle.size()];

	for(int k=0; k < shuffle.size(); ++k)
		trivialOrder[k] = k;
	for(int k=0; k < shuffle.size(); ++k)
	{ 
		int index = -1;
		int tmp   = -1;
		while( tmp == -1)
		{
			index = rand()%shuffle.size();
			tmp   = trivialOrder[index];
		}
		trivialOrder[index] = -1;
		shuffle[k]          = tmp;
	}

	delete[] trivialOrder;
}