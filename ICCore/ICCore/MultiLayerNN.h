#pragma once

#ifndef MULTILAYERNN_H
#define MULTILAYERNN_H

#ifdef IC_EXPORTS
#define IC_API __declspec(dllexport)
#else
#define IC_API __declspec(dllimport)
#endif


#include <string>
#include <iostream>
#include <fstream>
#include <exception>
#include <functional>
#include <vector>

#include "IGeneralICNetwork.h"

#include "ISerializable.h"
#include "BaseNeuron.h"

//Multilayer neural Network
class IC_API MultiLayerNN: public ISerializableSupervisedLearning, public IGeneralICNetwork
{
private:

	MultiLayerNN(): mInputs(NULL), mInputCount(0), 
		mOutputsNumber(NULL), mLayerCount(0), mNeuronPerLayer(0), mK(0.15)
	{}

protected:
	void _CreateDefaultNN();
	void _ClearNN();
	void _ProcessAll(double**);
	void _LearnInicialization(double** &Y, double** &ro, double* &e, std::vector< std::vector< std::vector<double> > >& deltaWeights,
		std::vector< std::vector< std::vector<double> > >& deltaWeightsPrevious);
	void _LearnFinish(double** &Y, double** &ro, double* &e, std::vector< std::vector< std::vector<double> > >& deltaWeights,
		std::vector< std::vector< std::vector<double> > >& deltaWeightsPrevious);
	void _Retropropagate(double** Y, double** ro, double* e, std::vector< std::vector< std::vector<double> > >& deltaWeights,
		std::vector< std::vector< std::vector<double> > >& deltaWeightsPrevious);
public://PRUEBA
	struct LearnInfo
	{
		LearnInfo(const LearnInfo& B)
		{	
			mInputs = B.mInputs;	
			mOutput = B.mOutput;
		}
		LearnInfo(int count)
		{	mInputs.resize(count,0);	}

		std::vector<double> mInputs;
		double              mOutput;
	};

	std::vector<LearnInfo> mLearnBook;
	
public:

	friend class Partition;

	//================================================
	/////////////Constructors/Destructors/////////////
	//================================================

	MultiLayerNN(const MultiLayerNN& B);
	explicit MultiLayerNN(int inputCount, int layerCount, int neruronPerLayer, int outputsNumber);
	virtual ~MultiLayerNN();

	//================================================
	/////////////Serializable Interface	//////////////
	//================================================

	virtual void Serialize(std::string fileName, BaseFormat format = BaseFormat::CSV);
	virtual void Serialize(std::ostream& fileStream, BaseFormat format = BaseFormat::CSV);
	virtual void Load(std::string fileName, BaseFormat format = BaseFormat::CSV);
	virtual void Load(std::istream& fileStream, BaseFormat format = BaseFormat::CSV);

	virtual void SerializeLearnData(std::string fileName, BaseFormat format = BaseFormat::CSV);
	virtual void SerializeLearnData(std::ostream& fileStream, BaseFormat format = BaseFormat::CSV);
	virtual void LoadLearnData(std::string fileName, BaseFormat format = BaseFormat::CSV);
	virtual void LoadLearnData(std::istream& fileStream, BaseFormat format = BaseFormat::CSV);

	//================================================
	///////		IGeneralICNetwork Methods	   ///////
	//================================================

	//Learn from the learn data loaded
	virtual void Learn(int iteration);

	//Calculates the output of the network with the Inputs assigned
	//Uses the Inputs that are pass be parameter. 
	//This Process is fundamental for the NetworkNet beacause it make
	//concurrent safe the process of the net.
	//
	//return: (std::vector<double>*) -> the output from the front layer of the network inside a vector of doubles
	virtual void* Process(double* Inputs, int InputCount);

	//================================================
	/////////////	 Neuron Methods		 /////////////
	//================================================

	//Calculates the output of the network with the Inputs assigned
	//
	//return: the output from the front layer of the network
	std::vector<double> Process();

	//Learn from the learn data loaded from a file by Load
	//Tolerance means: 0 learn all the patrons, 0.5 learn half the patrons, 1 do nothing
	void Learn(double tolerance, int maxIteration, bool doFeedBack = false, bool doErrorFeedBack = false, std::ostream& feedbackStream = std::cout);

	//Learn from the learn data loaded from a file by Load
	//Using a leave-k-out learn and validation. The input k determines
	//the leave-k-out partitions
	//
	//Returns a value in range [0,1] that is the average of errors in tests 
	//So return of 0 means GREAT, return of 1 mean WORST
	double LearnCrossValidation(int maxIteration, int k, double& ErrorAverage, double& ErrorStandarDesviation, bool doFeedBack = false, std::ostream& feedbackStream = std::cout);

	//Test all the learn data with the network. Similar to the
	//cross-validation but simpler just test against all the cases, no partitions
	//
	//Returns a value in range [0,1] that is the average of errors in tests 
	//So return of 0 means GREAT, return of 1 mean WORST
	double TestAll();

	MultiLayerNN& operator =(const MultiLayerNN& B);

	//================================================
	/////////////		Properties		//////////////
	//================================================

	//The learning quickness const
	double mK;

	//The inertia of the learning
	double mInertia;

	//Number of inputs X to the neuron
	const int  mInputCount;

	//X values that enter the neuron
	double *const mInputs;

	//Number of neurons in the last layer, ie the number of groups in a classifier
	const int  mOutputsNumber;

	//Number of layers in the network
	const int  mLayerCount;

	//Number of neurons per layer. 
	//The exception is the last layer where there are the number of outputs neurons
	const int  mNeuronPerLayer;

	//Matrix of neurons:
	//
	//		mNeuronMatrix[i][j]
	//		Where: - i is the layer
	//			   - j is j neuron inside i layer
	std::vector< std::vector<BaseNeuron> > mNeuronMatrix;

	//The sign function that will be apply to the internal neurons
	//Syntax: double (double result)
	//			where:
	//				-result is output from that neuron
	//
	//Note: Can be apply as a lambda function
	std::function<double(double)> mSignFunction;

	//The sign function that will be apply to the final layer of the network
	//Must give a return value [-1,1] continuous, Sigmoid is a posibility
	//Syntax: double (double result)
	//			where:
	//				-result is output from a neuron from the last layer
	//
	//Note: Can be apply as a lambda function
	std::function<double(double)> mPartitionFunction;

	//The function to get the result class in a unique value, an index 
	//enumerating the result class 
	//Syntax: double (double value, int clases)
	//			where:
	//				-value  a value from a output neuron
	//				-clases max number of output clases in the network
	//Note: Can be apply as a lambda function
	std::function<int(std::vector<double>,int)> mCodeFunction;

	//The error function that will calculate the error in the weights
	//in the learning process
	//Syntax: double (double y,double yd)
	//			where:
	//				-y  is the actual result
	//				-yd is the desired result
	//
	//Note: Can be apply as a lambda function
	std::function<double(double,double)> mErrorFunction;
};

#endif