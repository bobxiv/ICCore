#pragma once

#ifndef SOM_H
#define SOM_H

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

//double TransitionGaussian(int difX, int difY, int n)
//	{
//		double exp = -(difX*difX + difY*difY)/(double)(2*2);
//		return pow(2.71,exp);
//	};


//Self Organized Map
class IC_API SOM: public ISerializableSupervisedLearning, public IGeneralICNetwork
{
private:

	SOM(): mInputs(NULL), mInputCount(0), 
		mMapWidth(0), mMapHeight(0), mKGlobal(0.8), mKConvergence(0.1)
	{}

protected:
	void _CreateDefaultSOM();
	void _ClearSOM();
	void _ProcessLearn(int &I, int &J);
	int  _LearnMethod(int affectRange, int k);
	double _n(int difX, int difY, int iteration);
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

	enum LearnState{GlobalState, TransicionState, ConvergenceState};
	LearnState mCurLearningState;

	std::vector<LearnInfo> mLearnBook;
	
public:

	friend class Partition;

	//================================================
	/////////////Constructors/Destructors/////////////
	//================================================

	SOM(const SOM& B);
	explicit SOM(int inputCount, int mapWidth, int mapHeight);
	virtual ~SOM();

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

	//Calculates the output of the SOM with the Inputs assigned
	//Uses the Inputs that are pass be parameter. 
	//This Process is fundamental for the SOM beacause it make
	//concurrent safe the process of the map.
	//
	//return: (BaseNeuron*) -> the winning Neuron
	virtual void* Process(double* Inputs, int InputCount);

	//================================================
	/////////////	 Neuron Methods		 /////////////
	//================================================

	//Calculates the output of the SOM with the Inputs assigned
	//
	//return: the winning Neuron
	BaseNeuron Process();

	//Learn from the learn data loaded from a file by Load
	//The process have 3 stages: Global Stage -> Transition Stage -> Fine Stage
	void Learn(int globalIterations, int transitionIterations, int fineIterations, bool doFeedBack = false, bool doErrorFeedBack = false, std::ostream& feedbackStream = std::cout);

	struct ContinuousLearnSetup
	{
		ContinuousLearnSetup(): GlobalIterations(0), TransitionIterations(0), FineIterations(0), DoFeedBack(false), DoErrorFeedBack(false), pFeedbackStream(NULL), Curk(0)
		{}
		int GlobalIterations;
		int TransitionIterations;
		int FineIterations;
		bool DoFeedBack;
		bool DoErrorFeedBack;
		std::ostream* pFeedbackStream;
		int Curk;
		std::vector<bool> NeuronWinners;
	};
	ContinuousLearnSetup CurContinuousLearnSetup;

	//Initialize the variables to make a ContinuousLearn
	//See there for more info
	void InitializeContinuousLearn(int globalIterations, int transitionIterations, int fineIterations, bool doFeedBack = false, bool doErrorFeedBack = false, std::ostream& feedbackStream = std::cout);

	//Learn from the learn data loaded from a file by Load
	//This call is intended to advance k iterations in time and return
	//so we can train a 'little' and then return to show the progress...
	//Note that this is necesary because Transition Stage must know the total iterations
	//so we cannot train 1 iteration at a time like Multi Layer NN
	//The process have 3 stages: Global Stage -> Transition Stage -> Fine Stage
	void ContinuousLearn();

	//Learn from the learn data loaded from a file by Load, and then put labels
	//So we can use this map as a clasificator
	//The LearnData must have an output value
	void LearnAsclassificator(int globalIterations, int transitionIterations, int fineIterations, bool doFeedBack = false, bool doErrorFeedBack = false, std::ostream& feedbackStream = std::cout);

	//Test the process of the map with all the data from the LearnData
	//So we can use this map as a clasificator
	double TestAsclassificator();

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
	//double TestAll();

	SOM& operator =(const SOM& B);

	//================================================
	/////////////		Properties		//////////////
	//================================================

	//The learning quickness const for Global Stage
	double mKGlobal;

	//The learning quickness function for Transicion Stage
	//Syntax: double (int difX, int difY, int n)
	//			where:
	//				-n  is current iteration inside the transition stage
	//
	//Note: Can be apply as a lambda function
	std::function<double(int difX, int difY, int n)> mKTransicion;

	//The learning quickness const for Convergence Stage
	double mKConvergence;

	//Number of inputs X to the map
	const int  mInputCount;

	//X values that enter the map
	double *const mInputs;

	//Number of Neuron per row
	const int  mMapWidth;

	//Number of Neuron per column
	const int  mMapHeight;

	//Linear Matrix of map of neurons:		ONLY SQUARE MAP
	//
	//		mNeuronMap[i*mMapWidth+j]
	//		Where: - i is the row
	//			   - j is the column
	std::vector<BaseNeuron> mNeuronMap;

};

#endif