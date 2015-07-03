#pragma once

#ifndef IGENERALICNETWORK_H
#define IGENERALICNETWORK_H

#ifdef IC_EXPORTS
#define IC_API __declspec(dllexport)
#else
#define IC_API __declspec(dllimport)
#endif

#include <vector>

//General interface for all Networks
class IC_API IGeneralICNetwork
{
public:
	//Learn from the learn data loaded
	virtual void Learn(int iteration) = 0;

	//Calculates the output of the network with the Inputs assigned
	//Uses the Inputs that are pass be parameter. 
	//Concurrent safe the process of the net.
	//
	//return: the output from the network, should be cast to the actual value!... God knows that I had to make this void* monster...
	virtual void* Process(double* Inputs, int InputCount) = 0;
};

#endif