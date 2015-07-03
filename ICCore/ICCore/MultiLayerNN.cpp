#include "stdafx.h"

#include "Math.h"
#include "MultiLayerNN.h"

#include "Partition.h"

//================================================
/////////////Constructors/Destructors/////////////
//================================================

MultiLayerNN::MultiLayerNN(const MultiLayerNN& B): mInputs(NULL), mInputCount(0), 
	mOutputsNumber(NULL), mLayerCount(0), mNeuronPerLayer(0), mK(0.15), mInertia(0.1)
{
	this->operator=(B);
}

MultiLayerNN::MultiLayerNN(int inputCount, int layerCount, int neuronPerLayer, int outputsNumber): 
	mInputCount(inputCount), mInputs(NULL), mLayerCount(layerCount), mNeuronPerLayer(neuronPerLayer), 
	mOutputsNumber(outputsNumber), mK(0.15), mInertia(0.1)
	{
		_CreateDefaultNN();
	}

void MultiLayerNN::_CreateDefaultNN()
{
	const_cast<double*>(mInputs) = new double[mInputCount];

	for(int i=0; i < (mLayerCount-1); ++i)
	{
		mNeuronMatrix.push_back(std::vector<BaseNeuron>());
		for(int j=0; j < mNeuronPerLayer; ++j)
		{
			BaseNeuron newNeuron(((i == 0)? (mInputCount): (mNeuronPerLayer)));
			mNeuronMatrix[i].push_back(newNeuron);
		}
	}

	mNeuronMatrix.push_back(std::vector<BaseNeuron>());
	for(int j=0; j < mOutputsNumber; ++j)
	{
		BaseNeuron newNeuron((((mLayerCount-1) == 0)? (mInputCount): (mNeuronPerLayer)));
		mNeuronMatrix[mLayerCount-1].push_back(newNeuron);
	}
}

MultiLayerNN::~MultiLayerNN()
{
	assert( this->mInputs != NULL);
	_ClearNN();
}

void MultiLayerNN::_ClearNN()
{
	delete[] this->mInputs;
	for(int i =0; i < mLayerCount; ++i)
		mNeuronMatrix[i].clear();
	mNeuronMatrix.clear();
}

//================================================
/////////////Serializable Interface	//////////////
//================================================

void MultiLayerNN::Serialize(std::string fileName, BaseFormat format) throw(std::exception)
{
	//Absolutly necesary because ANSI C++ virtual functions with overloading have a limitation
	//once the compiler gets a function in the hierarchy that is the same as the one asked
	//it will search the overloading inside that level, so if we dont do this the compiler will
	//tell that string cant be converted to ifstream(wont go to the parent for the overloading)
	try
	{
		ISerializable::Serialize(fileName, format);
	}catch(std::exception* e)
	{
		throw e;
	}
}

void MultiLayerNN::Serialize(std::ostream& fileStream, BaseFormat format) throw(std::exception)
{
	switch( format )//serialize the specified format
	{
	case BaseFormat::CSV:
		{
			//Serialize Network header

			//(mLayerCount) (Input number) (Number Perceptron Layer 1) ....
			fileStream<< mLayerCount<< ' ' << mInputCount;
			for(int i=0; i < mLayerCount; ++i)
				fileStream<< ' ' << ((i==(mLayerCount-1))? (mOutputsNumber) : (mNeuronPerLayer));
			
			//Serialize Network Weights
			for(int i=0; i < mLayerCount; ++i)//Layer i
			{
				fileStream<< std::endl;
				int CurrentPerceptronInLayer = (i==(mLayerCount-1))? mOutputsNumber : mNeuronPerLayer;
				for(int j=0; j < CurrentPerceptronInLayer ; ++j)//Perceptron j
				{
					for(int k=0; k < mNeuronMatrix[i][j].mWeightsCount ; ++k)//Weight k
					{
						if( !(j == 0 && k == 0) )	
							fileStream<<',';
						fileStream<< mNeuronMatrix[i][j].mWeights[k];
					}
					fileStream<< ','<<mNeuronMatrix[i][j].mBias;//Last Weight is the Bias
				}
			}

		}
		break;
	case BaseFormat::Own:
		{
			throw(new std::exception("MultiLayerNN::Serialize => Own format not Immplemented!!!"));
		}
		break;
	case BaseFormat::XML:
		{
			throw(new std::exception("MultiLayerNN::Serialize => XML format not Immplemented!!!"));
		}
		break;
	default:
			throw(new std::exception("MultiLayerNN::Serialize => format specified to serialize is not enumed!!!"));
	}

	return;
}

void MultiLayerNN::Load(std::string fileName, BaseFormat format) throw(std::exception)
{
	//Absolutly necesary because ANSI C++ virtual functions with overloading have a limitation
	//once the compiler gets a function in the hierarchy that is the same as the one asked
	//it will search the overloading inside that level, so if we dont do this the compiler will
	//tell that string cant be converted to ifstream(wont go to the parent for the overloading)
	try
	{
		ISerializable::Load(fileName, format);
	}catch( std::exception* e)
	{
		throw e;
	}
}

void MultiLayerNN::Load(std::istream& fileStream, BaseFormat format) throw(std::exception)
{
	switch( format )//loads the specified format
	{
	case BaseFormat::CSV:
		{
			//Load Network header

			int dummy;

			int LoadLayerCount = 0;
			int LoadInputCount = 0;
			int LoadOutputsNumber = 0;
			int LoadNeuronPerLayer = 0;

			//(mLayerCount) (Input number) (Number Perceptron Layer 1) ....
			fileStream>> LoadLayerCount>> LoadInputCount;
			for(int i=0; i < LoadLayerCount; ++i)
				fileStream>> ((i==(LoadLayerCount-1))? LoadOutputsNumber : LoadNeuronPerLayer);
			fileStream.ignore(1);//fix >> read to use std::getline

			if( mLayerCount != LoadLayerCount || mInputCount != LoadInputCount || 
				mOutputsNumber != LoadOutputsNumber || mNeuronPerLayer != LoadNeuronPerLayer)
			{
				_ClearNN();
				const_cast<int&>(mLayerCount)    = LoadLayerCount;    const_cast<int&>(mInputCount)     = LoadInputCount; 
				const_cast<int&>(mOutputsNumber) = LoadOutputsNumber; const_cast<int&>(mNeuronPerLayer) = LoadNeuronPerLayer;
				_CreateDefaultNN();
			}

			std::string line;
			//Load Network Weights
			for(int i=0; i < mLayerCount; ++i)//Layer i
			{
				std::getline(fileStream,line);
				int pos = 0;
				int CurrentPerceptronInLayer = (i==(mLayerCount-1))? mOutputsNumber : mNeuronPerLayer;
				for(int j=0; j < CurrentPerceptronInLayer ; ++j)//Perceptron j
				{
					int aux = std::string::npos;
					for(int k=0; k < mNeuronMatrix[i][j].mWeightsCount ; ++k)//Weight k
					{
						aux = line.find(',', pos);
						std::string value = line.substr( pos, aux-pos );
						pos = aux+1;
						mNeuronMatrix[i][j].mWeights[k] = atof(value.c_str());
					}
					aux = line.find(',', pos);
					std::string value = line.substr( pos, aux-pos );
					pos = aux+1;
					mNeuronMatrix[i][j].mBias = atof( value.c_str() );//Last Weight is the Bias
				}
			}

		}
		break;
	case BaseFormat::Own:
		{
			throw(new std::exception("MultiLayerNN::Load => Own format not Immplemented!!!"));
		}
		break;
	case BaseFormat::XML:
		{
			throw(new std::exception("MultiLayerNN::Load => XML format not Immplemented!!!"));
		}
		break;
	default:
		throw(new std::exception("MultiLayerNN::Load => format specified to load is not enumed!!!"));
	}

	return;
}

void MultiLayerNN::SerializeLearnData(std::string fileName, BaseFormat format) throw(std::exception)
{
	//Absolutly necesary because ANSI C++ virtual functions with overloading have a limitation
	//once the compiler gets a function in the hierarchy that is the same as the one asked
	//it will search the overloading inside that level, so if we dont do this the compiler will
	//tell that string cant be converted to ifstream(wont go to the parent for the overloading)
	try
	{
		ISerializableSupervisedLearning::SerializeLearnData(fileName, format);
	}catch(std::exception* e)
	{
		throw e;
	}
}

void MultiLayerNN::SerializeLearnData(std::ostream& fileStream, BaseFormat format) throw(std::exception)
{
	switch( format )//serialize the specified format
	{
	case BaseFormat::CSV:
		{
			throw(new std::exception("MultiLayerNN::Serialize => CSV format not Immplemented!!!"));
		}
		break;
	case BaseFormat::Own:
		{
			throw(new std::exception("MultiLayerNN::Serialize => Own format not Immplemented!!!"));
		}
		break;
	case BaseFormat::XML:
		{
			throw(new std::exception("MultiLayerNN::Serialize => XML format not Immplemented!!!"));
		}
		break;
	default:
			throw(new std::exception("MultiLayerNN::Serialize => format specified to serialize is not enumed!!!"));
	}

	return;
}

void MultiLayerNN::LoadLearnData(std::string fileName, BaseFormat format) throw(std::exception)
{
	//Absolutly necesary because ANSI C++ virtual functions with overloading have a limitation
	//once the compiler gets a function in the hierarchy that is the same as the one asked
	//it will search the overloading inside that level, so if we dont do this the compiler will
	//tell that string cant be converted to ifstream(wont go to the parent for the overloading)
	try
	{
		ISerializableSupervisedLearning::LoadLearnData(fileName, format);
	}catch( std::exception* e)
	{
		throw e;
	}
}

void MultiLayerNN::LoadLearnData(std::istream& fileStream, BaseFormat format) throw(std::exception)
{
	switch( format )//loads the specified format
	{
	case BaseFormat::CSV:
		{
		std::string line;
	
		std::getline(fileStream, line);
		const_cast<int&>(mInputCount) = std::count(line.begin(), line.end(), ',');
	
		do
		{
			LearnInfo info(mInputCount);

			int pos = 0;
			for(int i=0; i < mInputCount; ++i)
			{
				int aux = line.find(',', pos);
				std::string value = line.substr( pos, aux-pos );
				pos = aux+1;
				info.mInputs[i] = atof(value.c_str());
			}
		
			info.mOutput = atof( line.substr( pos, std::string::npos ).c_str() );

			this->mLearnBook.push_back(info);
		}while( std::getline(fileStream, line) );

		int max = 0;
		for(int i=0; i < mLearnBook.size(); ++i)
			(mLearnBook[i].mOutput > max)? (max = mLearnBook[i].mOutput): 0;

		const_cast<int&>(mOutputsNumber) = max + 1;//+1 because 0 is also a class
		return;
		}
		break;

	case BaseFormat::Own:
		{
			throw(new std::exception("MultiLayerNN::Load => Own format not Immplemented!!!"));
		}
		break;
	case BaseFormat::XML:
		{
			throw(new std::exception("MultiLayerNN::Load => XML format not Immplemented!!!"));
		}
		break;
	default:
		throw(new std::exception("MultiLayerNN::Load => format specified to load is not enumed!!!"));
	}

	return;
}

//================================================
///////		IGeneralICNetwork Methods	   ///////
//================================================

void MultiLayerNN::Learn(int iteration)
{
	this->Learn(0.0f, iteration, false, false);
}

void* MultiLayerNN::Process(double* Inputs, int InputCount)
{
	assert( InputCount == this->mInputCount );//if not Process not posible!

	//std::vector<double> Outputs(this->mOutputsNumber, 0.0);
	double* Outputs = new double[this->mOutputsNumber];

	int maxInputs = max(InputCount, mNeuronPerLayer);
	double* CurrentInputs = new double[maxInputs];
	double* NextInputs    = new double[maxInputs];
	std::copy(Inputs, Inputs+InputCount, CurrentInputs);
	std::fill(CurrentInputs+InputCount, CurrentInputs+maxInputs, 0.0);

	// Hiden Layers
	for(int i=0; i < (mLayerCount-1); ++i)
	{
		for(int j=0; j < mNeuronPerLayer ; ++j)
			NextInputs[j] = mNeuronMatrix[i][j].Process(CurrentInputs, mPartitionFunction);

		std::swap(CurrentInputs,NextInputs);
	}
	
	// Front Layer: Apply Sigmoid
	for(int j=0; j < mOutputsNumber; ++j)
		Outputs[j] = mNeuronMatrix[mLayerCount-1][j].Process(CurrentInputs, mPartitionFunction);

	delete[] NextInputs;
	delete[] CurrentInputs;

	return static_cast<void*>(Outputs);
}

//================================================
/////////////	 Neuron Methods		 /////////////
//================================================

std::vector<double> MultiLayerNN::Process()
{
	std::vector<double> Outputs(this->mOutputsNumber, 0.0);
	int maxInputs = max(mInputCount, mNeuronPerLayer);
	double* CurrentInputs = new double[maxInputs];
	double* NextInputs    = new double[maxInputs];
	std::copy(mInputs, mInputs+mInputCount, CurrentInputs);
	std::fill(CurrentInputs+mInputCount, CurrentInputs+maxInputs, 0.0);

	// Hiden Layers
	for(int i=0; i < (mLayerCount-1); ++i)
	{
		for(int j=0; j < mNeuronPerLayer ; ++j)
			NextInputs[j] = mNeuronMatrix[i][j].Process(CurrentInputs, mPartitionFunction);

		std::swap(CurrentInputs,NextInputs);
	}
	
	// Front Layer: Apply Sigmoid
	for(int j=0; j < mOutputsNumber; ++j)
		Outputs[j] = mNeuronMatrix[mLayerCount-1][j].Process(CurrentInputs, mPartitionFunction);

	delete[] NextInputs;
	delete[] CurrentInputs;

	return Outputs;
}

void MultiLayerNN::_ProcessAll(double** Y)
{
	double* CurrentInput = NULL;
	// Hiden Layers
	for(int i=0; i < (mLayerCount-1); ++i)
	{
		CurrentInput = (i == 0)? mInputs: Y[i-1];
		for(int j=0; j < mNeuronPerLayer ; ++j)
			Y[i][j] = mNeuronMatrix[i][j].Process( CurrentInput, mPartitionFunction);
	}
	
	// Front Layer: Apply Sigmoid
	CurrentInput = ((mLayerCount-1) == 0)? mInputs: Y[(mLayerCount-1)-1];
	for(int j=0; j < mOutputsNumber; ++j)
		Y[mLayerCount-1][j] = mNeuronMatrix[mLayerCount-1][j].Process(CurrentInput, mPartitionFunction);
}

double _InternalDeltaWeightDot(double* deltaLayer, std::vector<BaseNeuron> mNeuronLayer, int j)
{
	double tot = 0.0;
	for(int k=0;k < mNeuronLayer.size(); ++k)
		tot += deltaLayer[k] * mNeuronLayer[k].mWeights[j];
	return tot;
}

void MultiLayerNN::_LearnInicialization(double** &Y, double** &ro, double* &e, std::vector< std::vector< std::vector<double> > >& deltaWeights,
	std::vector< std::vector< std::vector<double> > >& deltaWeightsPrevious)
{
	e  = new double[mOutputsNumber];
	Y  = new double*[mLayerCount];
	ro = new double*[mLayerCount];
	for(int i=0; i < (mLayerCount-1); ++i)
	{
		Y[i] = new double[mNeuronPerLayer];
		ro[i] = new double[mNeuronPerLayer];
		std::fill(Y[i], Y[i]+mNeuronPerLayer, 0.0);
		std::fill(ro[i], ro[i]+mNeuronPerLayer, 0.0);
	}
	Y[mLayerCount-1] = new double[mOutputsNumber];
	ro[mLayerCount-1] = new double[mOutputsNumber];

	std::fill(Y[mLayerCount-1], Y[mLayerCount-1]+mOutputsNumber, 0.0);
	std::fill(ro[mLayerCount-1], ro[mLayerCount-1]+mOutputsNumber, 0.0);

	for(int i=0; i < (mLayerCount-1); ++i)
	{
		deltaWeightsPrevious[i].resize(mNeuronPerLayer);
		deltaWeights[i].resize(mNeuronPerLayer);
		for(int j=0; j < mNeuronPerLayer; ++j)
		{
			if( i == 0 )
			{
				deltaWeightsPrevious[i][j].resize(mInputCount+1, 0.0);
				deltaWeights[i][j].resize(mInputCount+1, 0.0);//Plus 1 for Bias
			}
			else
			{
				deltaWeightsPrevious[i][j].resize(mNeuronPerLayer+1, 0.0);
				deltaWeights[i][j].resize(mNeuronPerLayer+1, 0.0);//Plus 1 for Bias
			}
		}
	}
	deltaWeightsPrevious[mLayerCount-1].resize(mOutputsNumber);
	deltaWeights[mLayerCount-1].resize(mOutputsNumber);

	for(int j=0; j < mOutputsNumber; ++j)
		if( (mLayerCount-1) == 0 )
		{
			deltaWeightsPrevious[mLayerCount-1][j].resize(mInputCount+1, 0.0);
			deltaWeights[mLayerCount-1][j].resize(mInputCount+1, 0.0);//Plus 1 for Bias
		}
		else
		{
			deltaWeightsPrevious[mLayerCount-1][j].resize(mNeuronPerLayer+1, 0.0);
			deltaWeights[mLayerCount-1][j].resize(mNeuronPerLayer+1, 0.0);//Plus 1 for Bias
		}

}
	
void MultiLayerNN::_LearnFinish(double** &Y, double** &ro, double* &e, std::vector< std::vector< std::vector<double> > >& deltaWeights,
	std::vector< std::vector< std::vector<double> > >& deltaWeightsPrevious)
{
	for(int i=0; i < mLayerCount; ++i)
		delete[] Y[i];
	delete[] Y;
	for(int i=0; i < deltaWeights.size(); ++i)
	{
		for(int j=0; j < deltaWeights[i].size(); ++j)
		{
			deltaWeightsPrevious[i][j].clear();
			deltaWeights[i][j].clear();
		}
		deltaWeightsPrevious[i].clear();
		deltaWeights[i].clear();
	}
	deltaWeightsPrevious.clear();
	deltaWeights.clear();
	for(int i=0; i < mLayerCount; ++i)
		delete[] ro[i];
	delete[] ro;
	delete[] e;
}

void MultiLayerNN::Learn(double tolerance, int maxIteration, bool doFeedBack, bool doErrorFeedBack, std::ostream& feedbackStream)
{
	 //Create necesary variables
	double** Y  = NULL;
	double** ro = NULL;
	double*  e  = NULL;
	std::vector< std::vector< std::vector<double> > > deltaWeights(mLayerCount);
	std::vector< std::vector< std::vector<double> > > deltaWeightsPrevious(mLayerCount);
	_LearnInicialization(Y,ro,e,deltaWeights,deltaWeightsPrevious);//reserve initialize variables

	int* trivialOrder = new int[mLearnBook.size()];
	int* order        = new int[mLearnBook.size()];

	double errorsThreshold;

	double* aux = new double[mOutputsNumber];//to calculate output error with learning data

	// Learn iterations
	for(int k=0; k < maxIteration; ++k)
	{
		//Feedback percentage
		if( doFeedBack )
			if( k % (maxIteration/10) == 0 )
				feedbackStream<<(k / (double)maxIteration)*100<<"%"<<std::endl;

		//Disorder
		srand(std::time(NULL));
		for(int k=0; k < mLearnBook.size(); ++k)
			trivialOrder[k] = k;
		for(int k=0; k < mLearnBook.size(); ++k)
		{ 
			int index = -1;
			int tmp = -1;
			while( tmp == -1)
			{
				index = rand()%mLearnBook.size();
				tmp   = trivialOrder[index];
			}
			trivialOrder[index] = -1;
			order[k] = tmp;
		}

		errorsThreshold = 0;
		
		//Learn each patron
		for(int sample=0; sample < mLearnBook.size(); ++sample)
		{
			LearnInfo& info = mLearnBook[order[sample]];
			std::copy(info.mInputs.begin(), info.mInputs.end(), mInputs);
			_ProcessAll(Y);//Process the learn patron

			std::fill(aux,aux+mOutputsNumber, -1.0);
			aux[(int)info.mOutput] = 1.0;

			for(int j=0; j < mOutputsNumber; ++j)
				e[j] = mErrorFunction(Y[mLayerCount-1][j], aux[j]);   //Calcs error(sigmoid)

			_Retropropagate(Y,ro,e,deltaWeights,deltaWeightsPrevious);//Retropropagate

			//count errors in the prediction of the sample
			for(int j=0; j < mOutputsNumber; ++j)
			{
				e[j] = mErrorFunction(mSignFunction(Y[mLayerCount-1][j]), aux[j]);
				if( e[j] != 0 )
					e[j] = e[j]/e[j];
			}

			if(std::find(e,e+mOutputsNumber,1) != e+mOutputsNumber)
				++errorsThreshold;
		}

		errorsThreshold = this->TestAll();

		if( doErrorFeedBack )
			feedbackStream<<"Seasson "<<k+1<<" Mistakes~ "<<errorsThreshold*mLearnBook.size()<<std::endl;
		
		if( errorsThreshold == 0 || (errorsThreshold < tolerance) )//tolerance exit
		{
			if( doFeedBack )
				feedbackStream<<"Wanted Error tolerance reached\tAverage Error = "<<errorsThreshold<<" <= "<<tolerance<<std::endl;
				feedbackStream<<"Exit on iteration: "<<k+1<<std::endl;
			break;
		}

	}

	if( doFeedBack )
		feedbackStream<<"100%\tCompleted..."<<std::endl;//Feedback percentage

	_LearnFinish(Y,ro,e,deltaWeights,deltaWeightsPrevious);//free memory

	delete[] order;
	delete[] trivialOrder;
	delete[] aux;
	
	return;
}

void MultiLayerNN::_Retropropagate(double** Y, double** ro, double* e, std::vector< std::vector< std::vector<double> > >& deltaWeights,
	std::vector< std::vector< std::vector<double> > >& deltaWeightsPrevious)
{
	//Front Layer
	for(int j=0; j < mOutputsNumber; ++j)
	{
		int i = mLayerCount-1;
		ro[mLayerCount-1][j] = 0.5*e[j]*(1+Y[mLayerCount-1][j])*(1-Y[mLayerCount-1][j]);
		//ro[mLayerCount-1][j] = Y[mLayerCount-1][j]*(1-Y[mLayerCount-1][j])*(e[j]-Y[mLayerCount-1][j]);//Lippmann

		for(int d=0; d < mNeuronPerLayer; ++d)
			deltaWeights[mLayerCount-1][j][d] = mK * ro[mLayerCount-1][j] * Y[mLayerCount-2][d]   +  mInertia * (deltaWeights[mLayerCount-1][j][d]-deltaWeightsPrevious[mLayerCount-1][j][d]);
			//deltaWeights[mLayerCount-1][j][d] = mK * ro[mLayerCount-1][j] * Y[mLayerCount-2][d];//Lippmann
		deltaWeights[mLayerCount-1][j][deltaWeights[mLayerCount-1][j].size()-1] = mK * ro[mLayerCount-1][j] * (-1) +  mInertia * (deltaWeights[mLayerCount-1][j][deltaWeights[mLayerCount-1][j].size()-1]-deltaWeightsPrevious[mLayerCount-1][j][deltaWeights[mLayerCount-1][j].size()-1]);
		//deltaWeights[mLayerCount-1][j][deltaWeights[mLayerCount-1][j].size()-1] = mK * ro[mLayerCount-1][j] * (-1);//Lippmann
	}

	//Hiden Layer Minus First
	for(int i=(mLayerCount-2); i > 0 ; --i)
	{
		for(int j=0; j < mNeuronPerLayer; ++j)
		{
			ro[i][j] =  _InternalDeltaWeightDot(ro[i+1], mNeuronMatrix[i+1], j)*0.5*(1+Y[i][j])*(1-Y[i][j]);
			//ro[i][j] =  Y[i][j]*(1-Y[i][j])*_InternalDeltaWeightDot(ro[i+1], mNeuronMatrix[i+1], j);//Lippmann

			for(int d=0; d < mNeuronPerLayer; ++d)
				deltaWeights[i][j][d] = mK * ro[i][j] * Y[i-1][d] +  mInertia * (deltaWeights[i][j][d]-deltaWeightsPrevious[i][j][d]);
				//deltaWeights[i][j][d] = mK * ro[i][j] * Y[i-1][d];//Lippmann
			//deltaWeights[i][j][deltaWeights[i][j].size()-1] = mK * ro[i][j] * (-1);//Lippmann
			deltaWeights[i][j][deltaWeights[i][j].size()-1] = mK *ro[i][j] * (-1) +  mInertia * (deltaWeights[i][j][deltaWeights[i][j].size()-1]-deltaWeightsPrevious[i][j][deltaWeights[i][j].size()-1]);//Update Bias
		}
	}

	//First Layer
	for(int j=0; j < mNeuronPerLayer; ++j)
		{
			int i = 0;
			ro[0][j] =  _InternalDeltaWeightDot(ro[1], mNeuronMatrix[1], j)*0.5*(1+Y[0][j])*(1-Y[0][j]);
			//ro[0][j] =  Y[0][j]*(1-Y[0][j])*_InternalDeltaWeightDot(ro[1], mNeuronMatrix[1], j);//Lippmann

			for(int d=0; d < mInputCount; ++d)
				deltaWeights[0][j][d] = mK *ro[0][j] * mInputs[d] +  mInertia * (deltaWeights[0][j][d]-deltaWeightsPrevious[0][j][d]);
				//deltaWeights[0][j][d] = mK * ro[0][j] * mInputs[d];//Lippmann
			//deltaWeights[0][j][deltaWeights[0][j].size()-1] = mK * ro[0][j] * (-1);//Lippmann
			deltaWeights[0][j][deltaWeights[0][j].size()-1] = mK *ro[0][j] * (-1) +  mInertia * (deltaWeights[0][j][deltaWeights[0][j].size()-1]-deltaWeightsPrevious[0][j][deltaWeights[0][j].size()-1]);//Update Bias
		}
			

	// Update Weights
	for(int i=0; i < mLayerCount ; ++i)
	{
		int neuronByLayer = (i == mLayerCount-1)? mOutputsNumber : mNeuronPerLayer;
		for(int j=0; j < neuronByLayer; ++j)
		{
			int weightInLayer = (i == 0)? mInputCount : mNeuronPerLayer;
			for(int k=0; k < weightInLayer ; ++k)
			{
				deltaWeightsPrevious[i][j][k] = deltaWeights[i][j][k];//save previous
				mNeuronMatrix[i][j].mWeights[k] += deltaWeights[i][j][k];
			}
			deltaWeightsPrevious[i][j][weightInLayer] = deltaWeights[i][j][weightInLayer];//save previous
			mNeuronMatrix[i][j].mBias += deltaWeights[i][j][weightInLayer];
		}
	}

	return;
}

double MultiLayerNN::TestAll()
{
	double* e   = new double[mOutputsNumber];
	double* aux = new double[mOutputsNumber];//to calculate output error with learning data
	int errors  = 0;

	std::vector<double> Outputs(mOutputsNumber);

	//Test each patron
	for(int sample=0; sample < mLearnBook.size(); ++sample)
	{
		LearnInfo& info = mLearnBook[sample];
		std::copy(info.mInputs.begin(), info.mInputs.end(), mInputs);
			
		Outputs = Process();

		std::fill(aux,aux+mOutputsNumber, -1.0);
		aux[(int)info.mOutput] = 1.0;

		for(int j=0; j < mOutputsNumber; ++j)
		{
			e[j] = mErrorFunction(mSignFunction(Outputs[j]), aux[j]);
			if( e[j] != 0 )
				e[j] = e[j]/e[j];
		}

		if(std::find(e,e+mOutputsNumber,1) != e+mOutputsNumber)
			++errors;
	}

	delete[] aux;
	
	return ( errors/(double)mLearnBook.size() );
}

double MultiLayerNN::LearnCrossValidation(int maxIteration, int k, double& ErrorAverage, double& ErrorStandarDesviation, bool doFeedBack, std::ostream& feedbackStream) throw(std::exception)
{
	if( k >= mLearnBook.size() )
		throw (new std::exception("No se puede hacer leave-k-out con k mas grande o igual a la particion universal"));

	std::stringstream firstNetwork;//Save the first Network
	Serialize(firstNetwork);

	 //Create necesary variables
	double** Y  = NULL;
	double** ro = NULL;
	double*  e  = NULL;
	std::vector< std::vector< std::vector<double> > > deltaWeights(mLayerCount);
	std::vector< std::vector< std::vector<double> > > deltaWeightsPrevious(mLayerCount);
	_LearnInicialization(Y,ro,e,deltaWeights,deltaWeightsPrevious);//reserve initialize variables

	
	int PartitionsNumber = ceil(mLearnBook.size()/(double)k);

	double* aux = new double[mOutputsNumber];//to calculate output error with learning data

	int* errors  = new int[PartitionsNumber];
	std::fill(errors,errors+PartitionsNumber, 0);
	double errorsThreshold = 0;

	Partition validationPartition(mLearnBook);

	validationPartition.InitLeaveKOut(k);

	for(int curPart=0;;++curPart)//until no more leave-k-out partitions
	{
		std::vector<int> shuffle(validationPartition.mLearnPartition.size());//creates shuffe here could be optimized
		validationPartition.GetShuffle(shuffle);
		std::vector<LearnInfo*>& LearnData = validationPartition.mLearnPartition;

		Load(firstNetwork);//each partition start again

		if( doFeedBack )
			feedbackStream<<"Particion Numero: "<<curPart<<std::endl;

		for(int k=0; k < maxIteration; ++k)//iterates the learning of the patrons
		{

			//Learn each patron
			for(int sample=0; sample < LearnData.size(); ++sample)
			{
				LearnInfo& info = *LearnData[shuffle[sample]];
				std::copy(info.mInputs.begin(), info.mInputs.end(), mInputs);
				_ProcessAll(Y);

				std::fill(aux,aux+mOutputsNumber, -1.0);
				aux[(int)info.mOutput] = 1.0;

				for(int j=0; j < mOutputsNumber; ++j)
					e[j] = mErrorFunction(Y[mLayerCount-1][j], aux[j]);

				_Retropropagate(Y,ro,e,deltaWeights,deltaWeightsPrevious);
			}

		}

		std::vector<double> Outputs(mOutputsNumber);
		std::vector<LearnInfo*>& TestData = validationPartition.mTestPartition;

		//Test each patron
		for(int sample=0; sample < TestData.size(); ++sample)
		{
			LearnInfo& info = *TestData[sample];
			std::copy(info.mInputs.begin(), info.mInputs.end(), mInputs);
			
			Outputs = Process();

			std::fill(aux,aux+mOutputsNumber, -1.0);
			aux[(int)info.mOutput] = 1.0;

			for(int j=0; j < mOutputsNumber; ++j)
			{
				e[j] = mErrorFunction(mSignFunction(Outputs[j]), aux[j]);
				if( e[j] != 0 )
					e[j] = e[j]/e[j];
			}

			//Errors to check with tolerance
			double newErrors = 0.0;
			for(int j=0; j < mOutputsNumber; ++j)
				aux[j] = abs(e[j]);
			Math::Sum(aux,mOutputsNumber,newErrors);
			errorsThreshold += newErrors;

			if(std::find(e,e+mOutputsNumber,1) != e+mOutputsNumber)
				++errors[curPart];
		}

		//Next Partitions
		if( !validationPartition.NextLeaveKOut() )
			break;
	}

	//errors / (double)PartitionsNumber;
	//errorsThreshold / (double)PartitionsNumber;
	ErrorAverage = 0.0;
	Math::Sum(errors, PartitionsNumber,ErrorAverage);
	ErrorAverage /= (double)PartitionsNumber;

	ErrorStandarDesviation = 0;
	for(int i=0; i < PartitionsNumber; ++i)
		ErrorStandarDesviation += errors[i]*errors[i];
	ErrorStandarDesviation = sqrt(ErrorStandarDesviation)/(double)PartitionsNumber;

	_LearnFinish(Y,ro,e,deltaWeights,deltaWeightsPrevious);

	Load(firstNetwork);//Keep the first Network

	firstNetwork.clear();

	delete[] aux;
	delete[] errors;
	return 1.0;
	//return ( errors/(double)mLearnBook.size() );
}

MultiLayerNN& MultiLayerNN::operator =(const MultiLayerNN& B)
{
	// Equal Inputs
	if( this->mInputCount != B.mInputCount )
		{
			delete[] this->mInputs;
			
			const_cast<double*>(mInputs) = new double[B.mInputCount];
			const_cast<int&>(mInputCount) = B.mInputCount;
		}
	for(int i =0; i < mInputCount; ++i)
		mInputs[i] = B.mInputs[i];

	// Equal Weights Matrix
	if( this->mLayerCount != B.mLayerCount || this->mNeuronPerLayer != B.mNeuronPerLayer ||
		this->mOutputsNumber != B.mOutputsNumber)
		{
			for(int i =0; i < mInputCount; ++i)
				this->mNeuronMatrix[i].clear();
			this->mNeuronMatrix.clear();

			const_cast<int&>(mLayerCount)     = B.mLayerCount;
			const_cast<int&>(mNeuronPerLayer) = B.mNeuronPerLayer;
			const_cast<int&>(mOutputsNumber)  = B.mOutputsNumber;

			// Create & copy the new Weights Matrix
			for(int i=0; i < (mLayerCount-1); ++i)
			{
				mNeuronMatrix.push_back(std::vector<BaseNeuron>());
				for(int j=0; j < mNeuronPerLayer; ++j)
					mNeuronMatrix[i].push_back(B.mNeuronMatrix[i][j]);
			}

			mNeuronMatrix.push_back(std::vector<BaseNeuron>());
			for(int j=0; j < mOutputsNumber; ++j)
				mNeuronMatrix[mLayerCount-1].push_back(B.mNeuronMatrix[mLayerCount-1][j]);
		}

	// Equal Other values
	this->mK             = B.mK;
	this->mErrorFunction = B.mErrorFunction;
	this->mSignFunction  = B.mSignFunction;
	this->mLearnBook     = B.mLearnBook;

	return *this;
}

