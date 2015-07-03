#ifndef PARTITION_H
#define PARTITION_H

#ifdef IC_EXPORTS
#define IC_API __declspec(dllexport)
#else
#define IC_API __declspec(dllimport)
#endif


#include <string>
//#include <iostream>
//#include <fstream>
#include <exception>
//#include <functional>
#include <vector>

#include "MultiLayerNN.h"


//Partition for Cross-Validation
class IC_API Partition
{
private:

	std::vector<MultiLayerNN::LearnInfo> mUniverse;

	std::vector<int> mIndexUsed;

	//The current k of the partition
	int mK;

public:

	//================================================
	/////////////Constructors/Destructors/////////////
	//================================================

	//Generates partitions in base of the LearnBook info passed
	Partition(std::vector<MultiLayerNN::LearnInfo>& LearnBook);
	~Partition();

	//================================================
	/////////////	Partition Methods	 /////////////
	//================================================

	//Initialize the Leave-k-Out partition algorithm
	//and fills the arrays with the first partition
	//
	//Further must call NextLeaveKOut to get next partition
	void InitLeaveKOut(unsigned int k);

	//Fills the arrays with the next partition
	//If  returns false the Leave-k-Out partitions are finished
	bool NextLeaveKOut();

	//Recibes shuffle vector of a set size
	//and fills it with shuffle indexs
	//
	//With indexs in the range: [0,shuffle.size()-1]
	void GetShuffle(std::vector<int>& shuffle);

	//================================================
	/////////////		Properties		//////////////
	//================================================

	//Current Partition to learn
	std::vector<MultiLayerNN::LearnInfo*> mLearnPartition;

	//Current Partition to Test
	std::vector<MultiLayerNN::LearnInfo*> mTestPartition;

};

#endif