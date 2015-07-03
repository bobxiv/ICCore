#pragma once

#ifndef ADALINE_H
#define ADALINE_H

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

#include "ISerializable.h"

//Adaptative Linear Neuron
class IC_API Adaline: public ISerializable
{
protected:

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

	//================================================
	/////////////Constructors/Destructors/////////////
	//================================================

	explicit Adaline(int inputCount);
	virtual ~Adaline();

	//================================================
	/////////////Serializable Interface	//////////////
	//================================================
	
	virtual void Serialize(std::string fileName, BaseFormat format = BaseFormat::CSV);
	virtual void Serialize(std::ostream& fileStream, BaseFormat format = BaseFormat::CSV);
	virtual void Load(std::string fileName, BaseFormat format = BaseFormat::CSV);
	virtual void Load(std::istream& fileStream, BaseFormat format = BaseFormat::CSV);
	
	//================================================
	/////////////	 Neuron Methods		 /////////////
	//================================================

	double Process();

	void Learn(double tolerance, int maxIteration);

	//================================================
	/////////////		Properties		//////////////
	//================================================

	//The learning quickness const
	double mK;

	//Number of inputs X to the neuron
	const int  mInputCount;

	//X values that enter the neuron
	double *const mInputs;
	//Array of weights that affect a respective neuron input like:
	//
	//		mWeights[i]*mInputs[i]
	double *const mWeights;

	//The Bias that is apply to the neuron like:
	//		mBias*-1
	double mBias;

	//The sign function that will be apply to the result
	//Syntax: double (double result)
	//			where:
	//				-result is output from the neuron
	//
	//Note: Can be apply as a lambda function
	std::function<double(double)> mSignFunction;

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