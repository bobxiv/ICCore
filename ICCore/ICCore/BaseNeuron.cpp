#include "stdafx.h"

#include "BaseNeuron.h"
#include "Partition.h"

//================================================
/////////////Constructors/Destructors/////////////
//================================================

BaseNeuron::BaseNeuron(const BaseNeuron& B): mWeights(NULL), mWeightsCount(0)
{
	this->operator=(B);
}

BaseNeuron::BaseNeuron(int weightsCount): mWeights(new double[weightsCount]), mWeightsCount(weightsCount)//, mBias(*(mWeights+weightsCount))
{
	srand(time(NULL));
	for(int i=0; i < mWeightsCount ; ++i)
		mWeights[i] = (rand()/(double)RAND_MAX)-0.5;//Range: [-0.5,0.5]
	mBias = (rand()/(double)RAND_MAX)-0.5;//Range: [-0.5,0.5]
}

BaseNeuron::~BaseNeuron()
{
	assert( this->mWeights != NULL );
	delete[] this->mWeights;
}

//================================================
/////////////	 Neuron Methods		 /////////////
//================================================

double BaseNeuron::Process(double* Inputs, std::function<double(double)> SignFunction)
{
	double tot = 0.0;
	for(int i=0; i < mWeightsCount; ++i)
		tot += Inputs[i] * mWeights[i];

	tot -= mBias;//the Bias is process with a -1 like his input 
				 //OUR CONVENTION

	return SignFunction(tot);
}

double BaseNeuron::Process(double* Inputs)
{
	double tot = 0.0;
	for(int i=0; i < mWeightsCount; ++i)
		tot += Inputs[i] * mWeights[i];

	tot -= mBias;//the Bias is process with a -1 like his input 
				 //OUR CONVENTION

	return tot;
}

BaseNeuron& BaseNeuron::operator=(const BaseNeuron& B)
	{
		if( mWeightsCount != B.mWeightsCount )
		{
			delete[] this->mWeights;

			const_cast<double*>(mWeights) = new double[B.mWeightsCount];
			const_cast<int&>(mWeightsCount) = B.mWeightsCount;
		}
		for(int i =0; i < mWeightsCount; ++i)
			mWeights[i] = B.mWeights[i];
		mBias = B.mBias;
		mLabel = B.mLabel;
		return *this;
	}