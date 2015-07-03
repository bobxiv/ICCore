#pragma once

#ifndef BASENEURON_H
#define BASENEURON_H

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

class IC_API BaseNeuron
{
private:
	BaseNeuron(): mWeights(NULL), mWeightsCount(0)
	{}

public:
	//================================================
	/////////////Constructors/Destructors/////////////
	//================================================

	BaseNeuron(const BaseNeuron& B);
	explicit BaseNeuron(int weightsCount);
	~BaseNeuron();

	//================================================
	/////////////Serializable Interface	//////////////
	//================================================

	void Serialize(std::string fileName);
	void Serialize(std::ofstream& fileStream);
	void Load(std::string fileName);
	void Load(std::ifstream& fileStream);

	//================================================
	/////////////	 Neuron Methods		 /////////////
	//================================================

	double Process(double* Inputs, std::function<double(double)> SignFunction);

	double Process(double* Inputs);

	BaseNeuron& operator =(const BaseNeuron& B);

	//================================================
	/////////////		Properties		//////////////
	//================================================

	//Number of weights
	const int  mWeightsCount;

	//Array of weights that affect a respective neuron input like:
	//
	//		mWeights[i]*mInputs[i]
	double *const mWeights;

	//The Bias that is apply to the neuron like:
	//		mBias*-1
	double mBias;

	//The label of this neuron
	//Used in the auto-organized map to make a clasificator with it
	std::string mLabel;
};


#endif