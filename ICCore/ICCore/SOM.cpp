#include "stdafx.h"

#include "Math.h"
#include "SOM.h"

#include <map>


//================================================
/////////////Constructors/Destructors/////////////
//================================================

SOM::SOM(const SOM& B): mInputs(NULL), mInputCount(0), 
	mMapWidth(0), mMapHeight(0), mKGlobal(0.8), mKConvergence(0.1)
{
	this->operator=(B);
}

SOM::SOM(int inputCount, int mapWidth, int mapHeight):
	mInputCount(inputCount), mInputs(NULL), mMapWidth(mapWidth), mMapHeight(mapHeight), 
	mKGlobal(0.8), mKConvergence(0.1)
	{
		_CreateDefaultSOM();
	}

void SOM::_CreateDefaultSOM()
{
	const_cast<double*>(mInputs) = new double[mInputCount];

	for(int i=0; i < (mMapHeight*mMapWidth) ; ++i)
	{
		BaseNeuron Neuron(mInputCount);
		mNeuronMap.push_back( Neuron );
	}
}

SOM::~SOM()
{
	assert( this->mInputs != NULL);
	_ClearSOM();
}

void SOM::_ClearSOM()
{
	delete[] this->mInputs;
	mNeuronMap.clear();
}

//================================================
/////////////Serializable Interface	//////////////
//================================================

void SOM::Serialize(std::string fileName, BaseFormat format) throw(std::exception)
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

void SOM::Serialize(std::ostream& fileStream, BaseFormat format) throw(std::exception)
{
	switch( format )//serialize the specified format
	{
	case BaseFormat::CSV:
		{
			//Serialize SOM header
			
			//(mMapHeight) (mMapWidth) (Input number)
			fileStream<< mMapHeight<< ' ' << mMapWidth << ' ' << mInputCount;
			
			//Serialize SOM Weights
			for(int i=0; i < mMapHeight; ++i)//Matrix tipical i index
			{
				fileStream<< std::endl;
				for(int j=0; j < mMapWidth; ++j)//Matrix tipical i index
				{
					BaseNeuron& curNeuron = mNeuronMap[i*mMapWidth+j];
					for(int k=0; k < curNeuron.mWeightsCount ; ++k)//Weight k
					{
						if( !(j == 0 && k == 0) )	
							fileStream<<',';
						fileStream<< curNeuron.mWeights[k];
					}
					fileStream<< ','<<curNeuron.mBias;//Last Weight is the Bias
				}
			}

		}
		break;
	case BaseFormat::Own:
		{
			throw(new std::exception("SOM::Serialize => Own format not Immplemented!!!"));
		}
		break;
	case BaseFormat::XML:
		{
			throw(new std::exception("SOM::Serialize => XML format not Immplemented!!!"));
		}
		break;
	default:
			throw(new std::exception("SOM::Serialize => format specified to serialize is not enumed!!!"));
	}

	return;
}

void SOM::Load(std::string fileName, BaseFormat format) throw(std::exception)
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

void SOM::Load(std::istream& fileStream, BaseFormat format) throw(std::exception)
{
	switch( format )//loads the specified format
	{
	case BaseFormat::CSV:
		{
			//Load SOM header

			int dummy;

			int LoadMapHeight  = 0;
			int LoadMapWidth   = 0;
			int LoadInputCount = 0;
			
			//(mMapHeight) (mMapWidth) (Input number)
			fileStream>> LoadMapHeight>> LoadMapWidth >> LoadInputCount;

			fileStream.ignore(1);//fix >> read to use std::getline

			if( mMapHeight != LoadMapHeight || mMapWidth != LoadMapWidth || mInputCount != LoadInputCount )
			{
				_ClearSOM();
				const_cast<int&>(mMapHeight)    = LoadMapHeight;
				const_cast<int&>(mMapWidth)     = LoadMapWidth; 
				const_cast<int&>(mInputCount)   = LoadInputCount;
				_CreateDefaultSOM();
			}

			std::string line;
			//Load SOM Weights
			for(int i=0; i < mMapHeight; ++i)//Matrix tipical i index
			{
				std::getline(fileStream,line);
				int pos = 0;
				for(int j=0; j < mMapWidth; ++j)//Matrix tipical j index
				{
					BaseNeuron& curNeuron = mNeuronMap[i*mMapWidth+j];
					int aux = std::string::npos;
					for(int k=0; k < curNeuron.mWeightsCount ; ++k)//Weight k
					{
						aux = line.find(',', pos);
						std::string value = line.substr( pos, aux-pos );
						pos = aux+1;
						curNeuron.mWeights[k] = atof(value.c_str());
					}
					aux = line.find(',', pos);
					std::string value = line.substr( pos, aux-pos );
					pos = aux+1;
					curNeuron.mBias = atof( value.c_str() );//Last Weight is the Bias
				}
			}

		}
		break;
	case BaseFormat::Own:
		{
			throw(new std::exception("SOM::Load => Own format not Immplemented!!!"));
		}
		break;
	case BaseFormat::XML:
		{
			throw(new std::exception("SOM::Load => XML format not Immplemented!!!"));
		}
		break;
	default:
		throw(new std::exception("SOM::Load => format specified to load is not enumed!!!"));
	}

	return;
}

void SOM::SerializeLearnData(std::string fileName, BaseFormat format) throw(std::exception)
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

void SOM::SerializeLearnData(std::ostream& fileStream, BaseFormat format) throw(std::exception)
{
	switch( format )//serialize the specified format
	{
	case BaseFormat::CSV:
		{
			throw(new std::exception("SOM::Serialize => CSV format not Immplemented!!!"));
		}
		break;
	case BaseFormat::Own:
		{
			throw(new std::exception("SOM::Serialize => Own format not Immplemented!!!"));
		}
		break;
	case BaseFormat::XML:
		{
			throw(new std::exception("SOM::Serialize => XML format not Immplemented!!!"));
		}
		break;
	default:
			throw(new std::exception("SOM::Serialize => format specified to serialize is not enumed!!!"));
	}

	return;
}

void SOM::LoadLearnData(std::string fileName, BaseFormat format) throw(std::exception)
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

void SOM::LoadLearnData(std::istream& fileStream, BaseFormat format) throw(std::exception)
{
	switch( format )//loads the specified format
	{
	case BaseFormat::CSV:
		{
		std::string line;
	
		std::getline(fileStream, line);
		const_cast<int&>(mInputCount) = std::count(line.begin(), line.end(), ',');//because is not supervised we don't read the output
																				  //BUT IT SHOULD BE THERE SO WE CAN RE USE THIS FILE WITH THE 
		do																		  //SUPERVIZED METHODS
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
		
			info.mOutput = atof( line.substr( pos, std::string::npos ).c_str() );// USED HERE BECAUSE ALTHOUG WE ARE NOT SUPERVISED
																				 // WE CAN ALSO BE SUPERVISED

			this->mLearnBook.push_back(info);
		}while( std::getline(fileStream, line) );

		return;
		}
		break;

	case BaseFormat::Own:
		{
			throw(new std::exception("SOM::Load => Own format not Immplemented!!!"));
		}
		break;
	case BaseFormat::XML:
		{
			throw(new std::exception("SOM::Load => XML format not Immplemented!!!"));
		}
		break;
	default:
		throw(new std::exception("SOM::Load => format specified to load is not enumed!!!"));
	}

	return;
}

//================================================
///////		IGeneralICNetwork Methods	   ///////
//================================================


void SOM::Learn(int iteration)
{
	this->Learn(ceil(iteration/3.0f), ceil(iteration/3.0f), ceil(iteration/3.0f), false, false);
}

void* SOM::Process(double* Inputs, int InputCount)
{
	assert( InputCount == this->mInputCount );//if not Process not posible!

	BaseNeuron* pWinner = NULL;
	double MinDif = 99999;//numeric_limits<double>::max();
	double CurDif = 0;

	// Hiden Layers
	for(int i=0; i < (mMapWidth*mMapHeight); ++i)
	{
		Math::Norm(mNeuronMap[i].mWeights, Inputs, InputCount, CurDif);
		(CurDif < MinDif)? (pWinner = &(mNeuronMap[i]), MinDif = CurDif): 42;
	}


	return static_cast<void*>(pWinner);
}

//================================================
/////////////	 Neuron Methods		 /////////////
//================================================

BaseNeuron SOM::Process()
{
	BaseNeuron* pWinner = NULL;
	double MinDif = 999999;//std::numeric_limits<double>::max();
	double CurDif = 0;

	// Hiden Layers
	for(int i=0; i < (mMapWidth*mMapHeight); ++i)
	{
		Math::Norm(mNeuronMap[i].mWeights, mInputs, mInputCount, CurDif);
		(CurDif < MinDif)? (pWinner = &(mNeuronMap[i]), MinDif = CurDif): 42;
	}

	return *pWinner;
}

void SOM::_ProcessLearn(int &I, int &J)
{
	double MinDif = 9999999;//std::numeric_limits<double>::max();
	double CurDif = 0;

	for(int i=0; i < (mMapWidth*mMapHeight); ++i)
	{
		Math::Norm(mNeuronMap[i].mWeights, mInputs, mInputCount, CurDif);
		(CurDif < MinDif)? (MinDif = CurDif, I = i/mMapWidth, J = i%mMapWidth): 42;
	}
}
	
void SOM::Learn(int globalIterations, int transitionIterations, int fineIterations, bool doFeedBack, bool doErrorFeedBack, std::ostream& feedbackStream)
{
	 //Create necesary variables
	int* trivialOrder = new int[mLearnBook.size()];
	int* order        = new int[mLearnBook.size()];

	int affectRange = 0;

	int totIterations = globalIterations + transitionIterations + fineIterations;

	// Learn iterations
	for(int k=0; k < totIterations; ++k)
	{
		//Feedback percentage
		if( doFeedBack )
			if( k % (totIterations/10) == 0 )
				feedbackStream<<(k / (double)totIterations)*100<<"%"<<std::endl;

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

		//Set the current Stage learn value
		if( 0 <= k && k < globalIterations )
		{
			//affectRange = this->mMapHeight / 2;
			affectRange = max(this->mMapHeight, this->mMapWidth)/(float)2;
			mCurLearningState = LearnState::GlobalState;
		}else if( globalIterations <= k && k < (globalIterations+transitionIterations) )
		{
			affectRange = Math::LInterpolate(max(this->mMapHeight, this->mMapWidth) / 2, 1, (k-globalIterations)/(double)transitionIterations);
			mCurLearningState = LearnState::TransicionState;
		}else
		{
			affectRange = 0;
			mCurLearningState = LearnState::ConvergenceState;
		}
		
		//Learn each patron
		for(int sample=0; sample < mLearnBook.size(); ++sample)
		{
			LearnInfo& info = mLearnBook[order[sample]];
			std::copy(info.mInputs.begin(), info.mInputs.end(), mInputs);

			_LearnMethod(affectRange, k-globalIterations);//Actual Update Weights -> Learn
		}

		if( doErrorFeedBack )
			feedbackStream<<"Seasson "<<k+1<<std::endl;
	}

	if( doFeedBack )
		feedbackStream<<"100%\tCompleted..."<<std::endl;//Feedback percentage

	delete[] order;
	delete[] trivialOrder;
	
	return;
}

int SOM::_LearnMethod(int affectRange, int k)
{
	//BaseNeuron* pWinner;
	int winnerI, winnerJ;

	_ProcessLearn(winnerI, winnerJ);

	for(int i=max(winnerI-affectRange,0); i < min(winnerI+affectRange+1,mMapHeight); ++i)
	{
		for(int j=max(winnerJ-affectRange,0); j < min(winnerJ+affectRange+1,mMapWidth); ++j)
		{
			int difX = winnerI-i;
			int difY = winnerJ-j;

			BaseNeuron& curNeuron = mNeuronMap[i*mMapWidth+j];
			for(int k=0; k < curNeuron.mWeightsCount ; ++k)
				curNeuron.mWeights[k] += _n(difX,difY,k) * (mInputs[k]-curNeuron.mWeights[k]);
		}
	}

	return (winnerI*mMapWidth+winnerJ);
}

double SOM::_n(int difX, int difY, int iteration)
{
	switch( mCurLearningState )
	{
	case LearnState::GlobalState:
		return mKGlobal;
		break;
	case LearnState::TransicionState:
		return mKTransicion(difX,difY,iteration);
		break;
	case LearnState::ConvergenceState:
		return mKConvergence;
		break;
	}

	return 42;
}




void SOM::InitializeContinuousLearn(int globalIterations, int transitionIterations, int fineIterations, bool doFeedBack, bool doErrorFeedBack, std::ostream& feedbackStream)
{
	this->CurContinuousLearnSetup.GlobalIterations     = globalIterations;
	this->CurContinuousLearnSetup.TransitionIterations = transitionIterations;
	this->CurContinuousLearnSetup.FineIterations       = fineIterations;
	this->CurContinuousLearnSetup.DoFeedBack           = doFeedBack;
	this->CurContinuousLearnSetup.DoErrorFeedBack      = doErrorFeedBack;
	this->CurContinuousLearnSetup.pFeedbackStream       = &feedbackStream;
	this->CurContinuousLearnSetup.Curk                 = 0;
	this->CurContinuousLearnSetup.NeuronWinners.resize(mMapWidth*mMapHeight);
}



void SOM::ContinuousLearn()
{
	std::fill(CurContinuousLearnSetup.NeuronWinners.begin(), CurContinuousLearnSetup.NeuronWinners.end(), false);

		 //Create necesary variables
	int* trivialOrder = new int[mLearnBook.size()];
	int* order        = new int[mLearnBook.size()];

	int affectRange = 0;

	int totIterations = CurContinuousLearnSetup.GlobalIterations + CurContinuousLearnSetup.TransitionIterations + CurContinuousLearnSetup.FineIterations;
	int &k = CurContinuousLearnSetup.Curk;

	// Learn iterations
	if( k < totIterations )
	{
		//Feedback percentage
		if( this->CurContinuousLearnSetup.DoFeedBack )
			if( k % (totIterations/10) == 0 )
				*CurContinuousLearnSetup.pFeedbackStream<<(k / (double)totIterations)*100<<"%"<<std::endl;

		//Disorder
		srand(std::time(NULL));
		for(int w=0; w < mLearnBook.size(); ++w)
			trivialOrder[w] = w;
		for(int w=0; w < mLearnBook.size(); ++w)
		{ 
			int index = -1;
			int tmp = -1;
			while( tmp == -1)
			{
				index = rand()%mLearnBook.size();
				tmp   = trivialOrder[index];
			}
			trivialOrder[index] = -1;
			order[w] = tmp;
		}

		//Set the current Stage learn value
		if( 0 <= k && k < CurContinuousLearnSetup.GlobalIterations )
		{
			affectRange = max(this->mMapHeight, this->mMapWidth) /(float) 2;
			mCurLearningState = LearnState::GlobalState;
		}else if( CurContinuousLearnSetup.GlobalIterations <= k && k < (CurContinuousLearnSetup.GlobalIterations+CurContinuousLearnSetup.TransitionIterations) )
		{
			affectRange = Math::LInterpolate(max(this->mMapHeight, this->mMapWidth) / 2, 1, (k-CurContinuousLearnSetup.GlobalIterations)/(double)CurContinuousLearnSetup.TransitionIterations);
			mCurLearningState = LearnState::TransicionState;
		}else
		{
			affectRange = 0;
			mCurLearningState = LearnState::ConvergenceState;
		}
		
		//Learn each patron
		for(int sample=0; sample < mLearnBook.size(); ++sample)
		{
			LearnInfo& info = mLearnBook[order[sample]];
			std::copy(info.mInputs.begin(), info.mInputs.end(), mInputs);

			int PosWinner = _LearnMethod(affectRange, k-CurContinuousLearnSetup.GlobalIterations);//Actual Update Weights -> Learn
			CurContinuousLearnSetup.NeuronWinners[PosWinner] = true;
		}

		if( CurContinuousLearnSetup.DoErrorFeedBack )
			*CurContinuousLearnSetup.pFeedbackStream<<"Seasson "<<k+1<<std::endl;

		++k;

	}

	if( CurContinuousLearnSetup.DoFeedBack )
		*CurContinuousLearnSetup.pFeedbackStream<<"100%\tCompleted..."<<std::endl;//Feedback percentage

	delete[] order;
	delete[] trivialOrder;
	
	return;
}



void SOM::LearnAsclassificator(int globalIterations, int transitionIterations, int fineIterations, bool doFeedBack, bool doErrorFeedBack, std::ostream& feedbackStream)
{
	Learn(globalIterations, transitionIterations, fineIterations, doFeedBack, doErrorFeedBack, feedbackStream);

	std::vector<std::map<int,int>> bucket(mMapHeight*mMapWidth);

	//Labeling
	for(int sample=0; sample < mLearnBook.size(); ++sample)
	{
		LearnInfo& info = mLearnBook[sample];
		std::copy(info.mInputs.begin(), info.mInputs.end(), mInputs);

		int winnerI;
		int winnerJ;
		this->_ProcessLearn(winnerI,winnerJ);

		bucket[winnerI*mMapWidth+winnerJ][info.mOutput]++;
		//char* aux = new char[25];
		//::itoa(info.mOutput, aux, 10);
		//this->mNeuronMap[winnerI*mMapWidth+winnerJ].mLabel = std::string(aux);
		//delete[] aux;
	}

	char* aux = new char[25];
	for(int i=0; i < (mMapHeight*mMapWidth); ++i)
	{
		int curMax = -1;
		int curWinLabel;
		std::map<int,int>::iterator it = bucket[i].begin();
		while( it != bucket[i].end() )
		{
			if( it->second > curMax )
			{
				curMax      = it->second;
				curWinLabel = it->first;
			}
			++it;
		}
		::itoa(curWinLabel, aux, 10);
		this->mNeuronMap[i].mLabel = std::string(aux);
	}
	delete[] aux;

}

double SOM::TestAsclassificator()
{
	int errors = 0;
	//Test each sample
	for(int sample=0; sample < mLearnBook.size(); ++sample)
	{
		LearnInfo& info = mLearnBook[sample];
		std::copy(info.mInputs.begin(), info.mInputs.end(), mInputs);

		int winnerI;
		int winnerJ;
		this->_ProcessLearn(winnerI,winnerJ);

		char* aux = new char[25];
		::itoa(info.mOutput, aux, 10);

		if( this->mNeuronMap[winnerI*mMapWidth+winnerJ].mLabel != std::string(aux) )
			++errors;

		delete[] aux;
	}

	return ((errors/(double)mLearnBook.size())*100.0f);
}

SOM& SOM::operator =(const SOM& B)
{
	// Equal Neuron Matrix
	if( this->mMapHeight != B.mMapHeight || this->mMapWidth != B.mMapWidth || 
		this->mInputCount != B.mInputCount)
		{
			this->mNeuronMap.clear();

			const_cast<int&>(mMapHeight) = B.mMapHeight;
			const_cast<int&>(mMapWidth)  = B.mMapWidth;

			// Create & copy the new Weights Matrix
			for(int i=0; i < (mMapWidth*mMapHeight); ++i)
				mNeuronMap.push_back(B.mNeuronMap[i]);
		}

	// Equal Inputs
	if( this->mInputCount != B.mInputCount )
		{
			delete[] this->mInputs;
			
			const_cast<double*>(mInputs) = new double[B.mInputCount];
			const_cast<int&>(mInputCount) = B.mInputCount;
		}
	for(int i =0; i < mInputCount; ++i)
		mInputs[i] = B.mInputs[i];

	// Equal Other values
	this->mKGlobal      = B.mKGlobal;
	this->mKTransicion  = B.mKTransicion;
	this->mKConvergence = B.mKConvergence;
	this->mLearnBook    = B.mLearnBook;

	return *this;
}

