#include "stdafx.h"

#include "Adaline.h"

//================================================
/////////////Constructors/Destructors/////////////
//================================================

Adaline::Adaline(int inputCount): mInputCount(inputCount), mInputs(new double[inputCount]), mWeights(new double[inputCount]),
								  mSignFunction(NULL), mErrorFunction(NULL)
{}

Adaline::~Adaline()
{
	delete[] this->mInputs;
	delete[] this->mWeights;
}

//================================================
/////////////Serializable Interface	//////////////
//================================================

void Adaline::Serialize(std::string fileName, BaseFormat format) throw(std::exception)
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


void Adaline::Serialize(std::ostream& fileStream, BaseFormat format) throw(std::exception)
{
	switch( format )//serialize the specified format
	{
	case BaseFormat::CSV:
		{
			throw(new std::exception("Adaline::Serialize => CSV format not Immplemented!!!"));
		}
		break;
	case BaseFormat::Own:
		{
			fileStream<<"Weights:"<<std::endl;
			fileStream<<"--------"<<std::endl;
			fileStream<<"Count: "<<this->mInputCount<<std::endl;
			for(int i=0; i < mInputCount; ++i)
				fileStream<<this->mWeights[i]<<" ";
		}
	case BaseFormat::XML:
		{
			throw(new std::exception("Adaline::Serialize => XML format not Immplemented!!!"));
		}
		break;
	default:
			throw(new std::exception("Adaline::Serialize => format specified to serialize is not enumed!!!"));
	}

	return;

	
}

void Adaline::Load(std::string fileName, BaseFormat format) throw(std::exception)
{
	//Absolutly necesary because ANSI C++ virtual functions with overloading have a limitation
	//once the compiler gets a function in the hierarchy that is the same as the one asked
	//it will search the overloading inside that level, so if we dont do this the compiler will
	//tell that string cant be converted to ifstream(wont go to the parent for the overloading)
	try
	{
		ISerializable::Load(fileName, format);
	}catch(std::exception* e)
	{
		throw e;
	}
}

void Adaline::Load(std::istream& fileStream, BaseFormat format) throw(std::exception)
{
	switch( format )//loads the specified format
	{
	case BaseFormat::CSV:
		{
			throw(new std::exception("Adaline::Load => CSV format not Immplemented!!!"));
		}
		break;
	case BaseFormat::Own:
		{
			//		The Weight info if any

			char dummy[256];
			fileStream.getline(dummy,256);//"Weights:"
			fileStream.getline(dummy,256);//"--------"
			int aux;
			fileStream>>dummy>>aux;//"Count: " mInputCount
			if( aux != this->mInputCount )
			{
				delete[] this->mInputs;
				delete[] this->mWeights;

				const_cast<double*>(mInputs)  = new double[aux];
				const_cast<double*>(mWeights) = new double[aux];
				const_cast<int&>(this->mInputCount) = aux;
			}

			for(int i=0; i < mInputCount; ++i)
				fileStream>>this->mWeights[i];

			//		The LearnBool info if any

			fileStream.getline(dummy,256);//""
			fileStream.getline(dummy,256);//"LearnBook:"
			fileStream.getline(dummy,256);//"----------"
			fileStream>>dummy>>aux;//"Count: " mLearnBook.size()

			for(int i=0; i < aux; ++i)
			{
				LearnInfo info(mInputCount);
				for(int j=0; j < mInputCount; ++j)
					fileStream>>info.mInputs[j];
				fileStream>>info.mOutput;
				mLearnBook.push_back(info);
			}
		}
		break;
	case BaseFormat::XML:
		{
			throw(new std::exception("Adaline::Load => XML format not Immplemented!!!"));
		}
		break;
	default:
		throw(new std::exception("Adaline::Load => format specified to load is not enumed!!!"));
	}

	return;
}

//================================================
/////////////	 Neuron Methods		 /////////////
//================================================

double Adaline::Process()
{
	double tot = 0.0;
	for(int i=0; i < mInputCount; ++i)
		tot += mInputs[i] * mWeights[i];

	tot -= mBias;//the Bias is process with a -1 like his input 
				 //OUR CONVENTION

	return mSignFunction(tot);
}

void Adaline::Learn(double tolerance, int maxIteration)
{
	int* trivialOrder = new int[mLearnBook.size()];
	int* order        = new int[mLearnBook.size()];
	for(int k=0; k < maxIteration; ++k)
	{
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

		//Learn
		for(int i=0; i < mLearnBook.size(); ++i)
		{
			LearnInfo& info = mLearnBook[order[i]];

			for(int j=0; j < this->mInputCount; ++j)
				this->mInputs[j] = info.mInputs[j];
			double y = Process();
			double error = mErrorFunction(y, info.mOutput);
			/*if( abs(error) < tolerance )
			{
				delete[] trivialOrder;
				delete[] order;
				return;
			}*/

			for(int j=0; j < this->mInputCount; ++j)
				this->mWeights[j] += mK*error*mInputs[j];
			this->mBias -= mK*error;
		}
	}

	delete[] trivialOrder;
	delete[] order;
	
	return;
}
