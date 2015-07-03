// ICConsoleApp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "ICConsoleApp.h"
#include <IC.h>


char res = 'N';

bool Preguntar(std::string what)
{
	std::cout<<what<<"[Y/N]";

	do
	{
	std::cin>>res;
	}while( !(res == 'Y' || res == 'N') );

	return ((res == 'Y')? true : false);
}

//Process Lean Info and saves the results in a file names "MyConcentCallc"
void SaveConcentResult(MultiLayerNN& ConcentNN)
{
	std::ofstream file("MyConcentCallc.csv");

	if( !file.is_open() )
		throw( new std::exception("No se puede guardar en MyConcentCallc.csv") );

	std::vector<double> Outputs;
	for(int sample=0; sample < ConcentNN.mLearnBook.size(); ++sample)
		{
			MultiLayerNN::LearnInfo& info = ConcentNN.mLearnBook[sample];
			std::copy(info.mInputs.begin(), info.mInputs.end(), ConcentNN.mInputs);
			Outputs = ConcentNN.Process();

			file<<ConcentNN.mInputs[0]<<','<<ConcentNN.mInputs[1];
			file<<','<<ConcentNN.mCodeFunction(Outputs, ConcentNN.mOutputsNumber)<<std::endl;
		}

	file.close();
}


int _tmain(int argc, _TCHAR* argv[])
{
	//Debug Folder
	#ifdef _DEBUG
		if( !SetCurrentDirectoryA("..\\ICCore\\ICCore\\Resource\\") )
		{
			std::cerr<<"No se puede establecer el directorio de archivos deseado!"<<std::endl;
			system("pause");
			exit(1);
		}
	#else
		/*if( !SetCurrentDirectoryA("Resource\\") )
		{
			std::cerr<<"No se puede establecer el directorio de archivos deseado!"<<std::endl;
			system("pause");
			exit(1);
		}*/
	#endif
		SetCurrentDirectoryA("..\\ICCore\\ICCore\\Resource\\");


		//Entrenados todos con la misma configuracion que esta abajo
		// Particion de entrenamiento: universo   Particion de test: universo
	//SOM mySOM(2,4,1);//XOR			0% de error
	SOM mySOM(2,4,4);//concent			5-8.28% de error
	//SOM mySOM(2,8,8);//concent		7.7% de error
	//SOM mySOM(2,16,16);//concent		2.4% de error
	//SOM mySOM(2,16*16,1);//concent	5.16% de error
	//SOM mySOM(2,4,4);//clouds			19% de error
	//SOM mySOM(2,16,1);//clouds		23% de error
	//SOM mySOM(2,16,16);//clouds		13~14% de error
	//SOM mySOM(5,8,8);//phoneme		24% de error
	//SOM mySOM(5,16,16);//phoneme		20% de error
	//SOM mySOM(5,16*16,1);//phoneme	22% de error
	//SOM mySOM(4,8,8);//iris			2-3.33% de error
	//SOM mySOM(4,16,16);//iris			0% de error
	
	try
	{
		//mySOM.LoadLearnData("iris");
		//mySOM.LoadLearnData("phoneme");
		//mySOM.LoadLearnData("clouds");
		mySOM.LoadLearnData("concent");
		//mySOM.LoadLearnData("XOR");
	}
	catch(std::exception* e)
	{
		std::cerr<<e->what()<<std::endl;
		delete e;
		system("pause");
		exit(1);
	}

	mySOM.mKGlobal = 0.2;
	mySOM.mKTransicion = [](int difX, int difY, int n) -> double
		{
			if( (difX == 0) && (difY == 0) )
				return 0.1f;
			return 0.1/(sqrt((double)(difX*difX+difY*difY)));
		};
	mySOM.mKConvergence = 0.01;
	//mySOM.LearnAsclassificator(100,1000,200,true,true);
	mySOM.LearnAsclassificator(100,300,200,true,true);

	/*mySOM.mInputs[0] = -1.0f; mySOM.mInputs[1] = -1.0f;
	BaseNeuron N1 = mySOM.Process();
	mySOM.mInputs[0] = -1.0f; mySOM.mInputs[1] = 1.0f;
	BaseNeuron N2 = mySOM.Process();
	mySOM.mInputs[0] = 1.0f; mySOM.mInputs[1] = -1.0f;
	BaseNeuron N3 = mySOM.Process();
	mySOM.mInputs[0] = 1.0f; mySOM.mInputs[1] = 1.0f;
	BaseNeuron N4 = mySOM.Process();*/

	double percentage = mySOM.TestAsclassificator();
	std::cout<<"Error en Auto-Organizativo:"<<std::endl;
	std::cout<<percentage<<'%'<<std::endl;

	/*mySOM.mInputs[0] = 0; mySOM.mInputs[1] = 0;
	BaseNeuron neu = mySOM.Process();
	mySOM.mInputs[0] = 0.5; mySOM.mInputs[1] = 0.5;
	neu = mySOM.Process();
	mySOM.mInputs[0] = 1.0; mySOM.mInputs[1] = 1.0;
	neu = mySOM.Process();*/

	mySOM.Serialize("MiPrueba");

	system("pause");





	MultiLayerNN MLNN(2,3,8,2);

	MLNN.mSignFunction = [] (double val)->double {
											if( val > 0.0 )
												return 1.0;
											else
												return -1.0;
											};
	MLNN.mErrorFunction = [] (double y,double yd)->double 
											{
												return (yd-y);
											};

	MLNN.mPartitionFunction = [] (double val)->double {
											double aux = std::pow(2.71, -val);
											return ( (1-aux) / (double)(1+aux) );
											};

	MLNN.mCodeFunction = [] (std::vector<double> res, int n)->double {
											double delta = 2/(double)n;
											std::vector<int> aux(res.size());
											for(int i=0; i < res.size() ; ++i)
											{
												aux[i] = int( (res[i]+1) / delta );
												if( aux[i] > 0 )
													aux[i] /= aux[i];
											}
											if( std::count(aux.begin(), aux.end(), 1) != 1 )
												return -1;
											std::vector<int>::iterator it = std::find(aux.begin(), aux.end(), 1);
											return it-aux.begin();
											};

	try
	{
		MLNN.LoadLearnData("concent");
	}
	catch(std::exception* e)
	{
		std::cerr<<std::endl<<e->what()<<std::endl<<std::endl;
		delete e;
		system("pause");
		exit(1);
	}

	MLNN.mK = 0.1f;
	MLNN.mInertia = 0.1f;

	MLNN.Learn(0.0f, 1300, true,true);// 3.36% de error tarda una banda

	std::cout<<"Error en Multicapa:"<<std::endl;
	percentage = MLNN.TestAll();
	std::cout<<percentage<<std::endl;

	system("pause");




	/*
	std::cout<<"Adaline:"<<std::endl<<std::endl;

	Adaline N1(2);
	N1.mBias = 0.1f;

	N1.mK = 0.1f;

	std::cout<<"Cargando datos OR:"<<std::endl;
	
	try
	{
		N1.Load("OR",Adaline::BaseFormat::Own);
		//N1.Load("clases");
	}
	catch(std::exception* e)
	{
		std::cerr<<e->what()<<std::endl;
		delete e;
		system("pause");
		exit(1);
	}
	
	N1.mSignFunction = [] (double val)->double {
											if( val > 0.0 )
												return 1.0;
											else
												return -1.0;
											};
	N1.mErrorFunction = [] (double y,double yd)->double 
											{
												return (yd-y);
											};

	srand(std::time(NULL));
	//Weights in range [0,0.1]
	N1.mWeights[0]    = (rand()/(double)RAND_MAX)/(double)10;N1.mWeights[1] = (rand()/(double)RAND_MAX)/(double)10;
	N1.mBias = 0.1f;

	std::cout<<"Aprendiendo... Supervisado"<<std::endl;
	N1.Learn(0.000000001f,100);

	std::cout<<"Probando valores:"<<std::endl;
	N1.mInputs[0]     = -1;N1.mInputs[1]  = -1;
	std::cout<<"Input: 0 0"<<std::endl;
	std::cout<<"Procesing: "<<((N1.Process() == -1)? 0: 1)<<std::endl;

	N1.mInputs[0]     = -1;N1.mInputs[1]  = 1;
	std::cout<<"Input: 0 1"<<std::endl;
	std::cout<<"Procesing: "<<((N1.Process() == -1)? 0: 1)<<std::endl;

	N1.mInputs[0]     = 1;N1.mInputs[1]  = -1;
	std::cout<<"Input: 1 0"<<std::endl;
	std::cout<<"Procesing: "<<((N1.Process() == -1)? 0: 1)<<std::endl;

	N1.mInputs[0]     = 1;N1.mInputs[1]  = 1;
	std::cout<<"Input: 1 1"<<std::endl;
	std::cout<<"Procesing: "<<((N1.Process() == -1)? 0: 1)<<std::endl;

	std::cout<<std::endl<<"Multilayer Neural Network:"<<std::endl<<std::endl;
	std::cout<<"2 entradas, 3 capas, 20 neuronas por capa, 2 salidas"<<std::endl;
	//MultiLayerNN MLNN(2,3,20,2);//Para concent
	MultiLayerNN MLNN(2,3,3,2);//Para XOR

	for(int i=0;i < MLNN.mInputCount; ++i)
		MLNN.mInputs[i] = (rand()/(double)RAND_MAX) - 0.5;

	MLNN.mSignFunction = N1.mSignFunction;

	MLNN.mErrorFunction = N1.mErrorFunction;

	MLNN.mPartitionFunction = [] (double val)->double {
											double aux = std::pow(2.71, -val);
											return ( (1-aux) / (double)(1+aux) );
											};

	MLNN.mCodeFunction = [] (std::vector<double> res, int n)->double {
											double delta = 2/(double)n;
											std::vector<int> aux(res.size());
											for(int i=0; i < res.size() ; ++i)
											{
												aux[i] = int( (res[i]+1) / delta );
												if( aux[i] > 0 )
													aux[i] /= aux[i];
											}
											if( std::count(aux.begin(), aux.end(), 1) != 1 )
												return -1;
											std::vector<int>::iterator it = std::find(aux.begin(), aux.end(), 1);
											return it-aux.begin();
											};

	double ErrorAverage           = 0;
	double ErrorStandarDesviation = 0;
	double errors      = 0;
	int leaveKout      = 0;
	double tolerance   = 0;
	int iterations     = 0;
	std::vector<double> Outputs;

	#pragma region Clase

	std::cout<<"Cargando datos Clase:"<<std::endl;
	std::cout<<"3 entradas, 3 capas, 20 neuronas por capa, 2 salidas"<<std::endl;
	MultiLayerNN ClaseNN(3,3,20,2);
	ClaseNN.mErrorFunction     = MLNN.mErrorFunction;
	ClaseNN.mSignFunction      = MLNN.mSignFunction;
	ClaseNN.mCodeFunction      = MLNN.mCodeFunction;
	ClaseNN.mPartitionFunction = MLNN.mPartitionFunction;

	std::cout<<std::endl<<"Cargando datos Concent:"<<std::endl;
	try
	{
		ClaseNN.LoadLearnData("clases");
	}
	catch(std::exception* e)
	{
		std::cerr<<std::endl<<e->what()<<std::endl<<std::endl;
		delete e;
		system("pause");
		exit(1);
	}

	ClaseNN.mK = 0.1f;
	std::cout<<"Ingrese la tasa de aprendizaje(recomienda 0.1):"<<std::endl;
	std::cin>> ClaseNN.mK;

	std::cout<<"Ingrese la incercia de aprendizaje(recomienda 0.1):"<<std::endl;
	std::cin>> ClaseNN.mInertia;

	tolerance = 0.0001;
	std::cout<<"Ingrese la tolerancia de error para aprendizaje(recomienda 0.0001):"<<std::endl;
	std::cin>> tolerance;

	iterations = 2000;
	std::cout<<"Ingrese el numero de iteraciones de aprendizaje(recomienda 2000):"<<std::endl;
	std::cin>> iterations;

	std::cout<<std::endl<<"Aprendiendo... (Supervisado)"<<std::endl;
	ClaseNN.Learn(tolerance, iterations, true,true);
	//MLNN.Load("XOR_Entrenado");
	//double err = MLNN.LearnCrossValidation(tolerance, iterations, 1, true);
	//MLNN.Load("XOR_Entrenado");
	//MLNN.Serialize(std::cout);
	//system("pause");

	ClaseNN.TestAll();

	std::cout<<std::endl<<"Probando valores Clase:"<<std::endl;
	ClaseNN.mInputs[0]     = -1.0;MLNN.mInputs[1]  = -1.0;
	//MLNN.mInputs[0]     = 0.0;MLNN.mInputs[1]  = 0.0;//Lippmann
	Outputs = MLNN.Process();
	std::cout<<"Input: 0 0"<<std::endl;
	std::cout<<"Procesing: output 1 = "<<Outputs[0]<<"   output 2 = "<<Outputs[1]<<std::endl;//Deberia dar 0
	std::cout<<"0 xor 0 = "<<MLNN.mCodeFunction(Outputs,2)<<std::endl;
	ClaseNN.mInputs[0]     = -1.0;MLNN.mInputs[1]  = 1.0;
	//MLNN.mInputs[0]     =  0.0;MLNN.mInputs[1]  = 1.0;//Lippmann
	Outputs = MLNN.Process();
	std::cout<<"Input: 0 1"<<std::endl;
	std::cout<<"Procesing: output 1 = "<<Outputs[0]<<"   output 2 = "<<Outputs[1]<<std::endl;//Deberia dar 1
	std::cout<<"0 xor 1 = "<<MLNN.mCodeFunction(Outputs,2)<<std::endl;
	MLNN.mInputs[0]     = 1.0;MLNN.mInputs[1]  = -1.0;
	//MLNN.mInputs[0]     = 1.0;MLNN.mInputs[1]  = 0.0;//Lippmann
	Outputs = MLNN.Process();
	std::cout<<"Input: 1 0"<<std::endl;
	std::cout<<"Procesing: output 1 = "<<Outputs[0]<<"   output 2 = "<<Outputs[1]<<std::endl;//Deberia dar 1
	std::cout<<"1 xor 0 = "<<MLNN.mCodeFunction(Outputs,2)<<std::endl;
	MLNN.mInputs[0]     = 1.0;MLNN.mInputs[1]  = 1.0;
	Outputs = MLNN.Process();
	std::cout<<"Input: 1 1"<<std::endl;
	std::cout<<"Procesing: output 1 = "<<Outputs[0]<<"   output 2 = "<<Outputs[1]<<std::endl;//Deberia dar 0
	std::cout<<"1 xor 1 = "<<MLNN.mCodeFunction(Outputs,2)<<std::endl;

	std::cout<<std::endl<<"Esta conforme con los resultados?[Y/N]";
	res = 'N';
	std::cin>>res;
	if( res == 'Y' )
		MLNN.Serialize("XOR_Entrenado");

	#pragma endregion

	#pragma region XOR

	std::cout<<"Cargando datos XOR:"<<std::endl;
	try
	{
		MLNN.LoadLearnData("XOR");
	}
	catch(std::exception* e)
	{
		std::cerr<<std::endl<<e->what()<<std::endl<<std::endl;
		delete e;
		system("pause");
		exit(1);
	}

	MLNN.mK = 0.1f;
	std::cout<<"Ingrese la tasa de aprendizaje(recomienda 0.1):"<<std::endl;
	std::cin>> MLNN.mK;

	std::cout<<"Ingrese la incercia de aprendizaje(recomienda 0.1):"<<std::endl;
	std::cin>> MLNN.mInertia;

	tolerance = 0.0001;
	std::cout<<"Ingrese la tolerancia de error para aprendizaje(recomienda 0.0001):"<<std::endl;
	std::cin>> tolerance;

	iterations = 2000;
	std::cout<<"Ingrese el numero de iteraciones de aprendizaje(recomienda 2000):"<<std::endl;
	std::cin>> iterations;

	std::cout<<std::endl<<"Aprendiendo... (Supervisado)"<<std::endl;
	MLNN.Learn(tolerance, iterations, true,true);
	//MLNN.Load("XOR_Entrenado");
	//double err = MLNN.LearnCrossValidation(tolerance, iterations, 1, true);
	//MLNN.Load("XOR_Entrenado");
	//MLNN.Serialize(std::cout);
	//system("pause");

	std::cout<<std::endl<<"Probando valores XOR:"<<std::endl;
	MLNN.mInputs[0]     = -1.0;MLNN.mInputs[1]  = -1.0;
	//MLNN.mInputs[0]     = 0.0;MLNN.mInputs[1]  = 0.0;//Lippmann
	Outputs = MLNN.Process();
	std::cout<<"Input: 0 0"<<std::endl;
	std::cout<<"Procesing: output 1 = "<<Outputs[0]<<"   output 2 = "<<Outputs[1]<<std::endl;//Deberia dar 0
	std::cout<<"0 xor 0 = "<<MLNN.mCodeFunction(Outputs,2)<<std::endl;
	MLNN.mInputs[0]     = -1.0;MLNN.mInputs[1]  = 1.0;
	//MLNN.mInputs[0]     =  0.0;MLNN.mInputs[1]  = 1.0;//Lippmann
	Outputs = MLNN.Process();
	std::cout<<"Input: 0 1"<<std::endl;
	std::cout<<"Procesing: output 1 = "<<Outputs[0]<<"   output 2 = "<<Outputs[1]<<std::endl;//Deberia dar 1
	std::cout<<"0 xor 1 = "<<MLNN.mCodeFunction(Outputs,2)<<std::endl;
	MLNN.mInputs[0]     = 1.0;MLNN.mInputs[1]  = -1.0;
	//MLNN.mInputs[0]     = 1.0;MLNN.mInputs[1]  = 0.0;//Lippmann
	Outputs = MLNN.Process();
	std::cout<<"Input: 1 0"<<std::endl;
	std::cout<<"Procesing: output 1 = "<<Outputs[0]<<"   output 2 = "<<Outputs[1]<<std::endl;//Deberia dar 1
	std::cout<<"1 xor 0 = "<<MLNN.mCodeFunction(Outputs,2)<<std::endl;
	MLNN.mInputs[0]     = 1.0;MLNN.mInputs[1]  = 1.0;
	Outputs = MLNN.Process();
	std::cout<<"Input: 1 1"<<std::endl;
	std::cout<<"Procesing: output 1 = "<<Outputs[0]<<"   output 2 = "<<Outputs[1]<<std::endl;//Deberia dar 0
	std::cout<<"1 xor 1 = "<<MLNN.mCodeFunction(Outputs,2)<<std::endl;

	std::cout<<std::endl<<"Esta conforme con los resultados?[Y/N]";
	res = 'N';
	std::cin>>res;
	if( res == 'Y' )
		MLNN.Serialize("XOR_Entrenado");

	#pragma endregion

	std::cout<<std::endl<<std::endl;
	std::cout<<"--------------------------------------------------------------------"<<std::endl;
	system("pause");

	#pragma region Concentric
	{
	std::cout<<"Concent Neural Network:"<< std::endl;
	std::cout<<"-----------------------"<< std::endl;
	std::cout<<"2 entradas, 3 capas, 20 neuronas por capa, 2 salidas"<<std::endl;
	MultiLayerNN ConcentNN(2,3,5,2);
	ConcentNN.mErrorFunction     = MLNN.mErrorFunction;
	ConcentNN.mSignFunction      = MLNN.mSignFunction;
	ConcentNN.mCodeFunction      = MLNN.mCodeFunction;
	ConcentNN.mPartitionFunction = MLNN.mPartitionFunction;

	std::cout<<std::endl<<"Cargando datos Concent:"<<std::endl;
	try
	{
		ConcentNN.LoadLearnData("concent");
	}
	catch(std::exception* e)
	{
		std::cerr<<std::endl<<e->what()<<std::endl<<std::endl;
		delete e;
		system("pause");
		exit(1);
	}

	ConcentNN.mK = 0.1f;
	std::cout<<"Ingrese la tasa de aprendizaje(recomienda 0.1):"<<std::endl;
	std::cin>> ConcentNN.mK;

	std::cout<<"Ingrese la incercia de aprendizaje(recomienda 0.1):"<<std::endl;
	std::cin>> ConcentNN.mInertia;

	tolerance = 0.0001;
	std::cout<<"Ingrese la tolerancia de error para aprendizaje(recomienda 0.0001):"<<std::endl;
	std::cin>> tolerance;

	iterations = 2000;
	std::cout<<"Ingrese el numero de iteraciones de aprendizaje(recomienda 2000):"<<std::endl;
	std::cin>> iterations;

	std::cout<<std::endl<<"Aprendiendo... (Supervisado)"<<std::endl;
	ConcentNN.Learn(tolerance, iterations, true,true);
	//ConcentNN.Load("concent_Entrenado");
	//ConcentNN.LearnCrossValidation(2,500,ErrorAverage,ErrorStandarDesviation,true);

	std::cout<<"ErrorAverage = "<<ErrorAverage<<std::endl;
	std::cout<<"ErrorStandarDesviation = "<<ErrorStandarDesviation<<std::endl;
	system("pause");

	std::cout<<"Guardando los resultados de procesar mLearnBook con nuestra red en un archivo llamdo: \"MyConcentCallc.csv\"..."<<std::endl;
	//SaveConcentResult(ConcentNN);

	errors = ConcentNN.TestAll();
	std::cout<<"Indice de errores con validacion simple : "<<errors     <<std::endl;
	if( Preguntar("Hacer validacion cruzada?") )
	{
		std::cout<<"\tIngrese k... ";
		std::cin>>leaveKout;
		std::cout<<std::endl<<"\tIngrese iteraciones(recomienda 1)... ";
		std::cin>>iterations;
		try
		{
			ConcentNN.LearnCrossValidation(iterations,leaveKout,ErrorAverage,ErrorStandarDesviation,true);
		}catch(std::exception* e)
		{
			std::cerr<<std::endl<<e->what()<<std::endl<<std::endl;
			delete e;
			system("pause");
			exit(1);
		}
		std::cout<<"Promedio de Errores = "<< ErrorAverage<<std::endl;
		std::cout<<"Desviacion Estandar de Errores = "<< ErrorStandarDesviation<<std::endl;
	}
	

	std::cout<<std::endl<<"Probando valores Concentric:"<<std::endl;
	ConcentNN.mInputs[0]     = 0.201659414; ConcentNN.mInputs[1]  = 0.712270393;
	Outputs = ConcentNN.Process();
	std::cout<<"Input: 0.201659414 0.712270393\tOutput: "<< ConcentNN.mCodeFunction(Outputs,Outputs.size())<<std::endl;
	//std::cout<<"Procesing: output 1 = "<<Outputs[0]<<"   output 2 = "<<Outputs[1]<<std::endl;//Deberia dar 1
	ConcentNN.mInputs[0]     = 0.434624638; ConcentNN.mInputs[1]  = 0.737467967;
	Outputs = ConcentNN.Process();
	std::cout<<"Input: 0.434624638 0.737467967\tOutput: "<< ConcentNN.mCodeFunction(Outputs,Outputs.size())<<std::endl;
	//std::cout<<"Procesing: output 1 = "<<Outputs[0]<<"   output 2 = "<<Outputs[1]<<std::endl;//Deberia dar 0
	ConcentNN.mInputs[0]     = 0.270127153; ConcentNN.mInputs[1]  = 0.350168737;
	Outputs = ConcentNN.Process();
	std::cout<<"Input: 0.270127153 0.350168737\tOutput: "<< ConcentNN.mCodeFunction(Outputs,Outputs.size())<<std::endl;
	//std::cout<<"Procesing: output 1 = "<<Outputs[0]<<"   output 2 = "<<Outputs[1]<<std::endl;//Deberia dar 0
	ConcentNN.mInputs[0]     = 0.344335413; ConcentNN.mInputs[1]  = 0.861386822;
	Outputs = ConcentNN.Process();
	std::cout<<"Input: 0.344335413 0.861386822\tOutput: "<< ConcentNN.mCodeFunction(Outputs,Outputs.size())<<std::endl;
	//std::cout<<"Procesing: output 1 = "<<Outputs[0]<<"   output 2 = "<<Outputs[1]<<std::endl;//Deberia dar 1


	do
	{
		std::cout<<"Ingrese valores de consulta: ";
		std::cin>>ConcentNN.mInputs[0];std::cout<<' '; std::cin>>ConcentNN.mInputs[1];std::cout<<std::endl;

		Outputs = ConcentNN.Process();
		std::cout<<"Input: "<<ConcentNN.mInputs[0]<<' '<<ConcentNN.mInputs[1]<<
			"\tOutput: "<< ConcentNN.mCodeFunction(Outputs,Outputs.size())<<std::endl;
	}while( Preguntar("Seguir?") );

	if( Preguntar("Esta conforme con los resultados?") )
	{
		ConcentNN.Serialize("concent_Entrenado");
		std::cout<<"Archivo de pesos guardado en concent_Entrenado.csv..."<<std::endl;
	}

	}
	#pragma endregion

	std::cout<<std::endl<<std::endl;
	std::cout<<"--------------------------------------------------------------------"<<std::endl;
	system("pause");

	#pragma region Iris
	{
	std::cout<<"Iris Neural Network:"<< std::endl;
	std::cout<<"--------------------"<< std::endl;
	std::cout<<"4 entradas, 3 capas, 40 neuronas por capa, 3 salidas"<<std::endl;

	MultiLayerNN IrisNN(4,3,20,3);
	IrisNN.mErrorFunction     = MLNN.mErrorFunction;
	IrisNN.mSignFunction      = MLNN.mSignFunction;
	IrisNN.mCodeFunction      = MLNN.mCodeFunction;
	IrisNN.mPartitionFunction = MLNN.mPartitionFunction;

	std::cout<<std::endl<<"Cargando datos Iris:"<<std::endl;
	try
	{
		IrisNN.LoadLearnData("iris");
	}
	catch(std::exception* e)
	{
		std::cerr<<std::endl<<e->what()<<std::endl<<std::endl;
		delete e;
		system("pause");
		exit(1);
	}

	IrisNN.mK = 0.1f;
	std::cout<<"Ingrese la tasa de aprendizaje(recomienda 0.1):"<<std::endl;
	std::cin>> IrisNN.mK;

	std::cout<<"Ingrese la incercia de aprendizaje(recomienda 0.1):"<<std::endl;
	std::cin>> IrisNN.mInertia;

	tolerance = 0.0001;
	std::cout<<"Ingrese la tolerancia de error para aprendizaje(recomienda 0.0001):"<<std::endl;
	std::cin>> tolerance;

	iterations = 2000;
	std::cout<<"Ingrese el numero de iteraciones de aprendizaje(recomienda 2000):"<<std::endl;
	std::cin>> iterations;

	std::cout<<std::endl<<"Aprendiendo... (Supervisado)"<<std::endl;
	IrisNN.Learn(tolerance, iterations, true);
	//IrisNN.Load("iris_Entrenado");

	errors = IrisNN.TestAll();
	std::cout<<"Indice de errores con validacion simple : "<<errors     <<std::endl;
	if( Preguntar("Hacer validacion cruzada?") )
	{
		std::cout<<"\tIngrese k... ";
		std::cin>>leaveKout;
		std::cout<<std::endl<<"\tIngrese iteraciones(recomienda 1)... ";
		std::cin>>iterations;
		try
		{
			IrisNN.LearnCrossValidation(iterations,leaveKout,ErrorAverage,ErrorStandarDesviation,true);
		}catch(std::exception* e)
		{
			std::cerr<<std::endl<<e->what()<<std::endl<<std::endl;
			delete e;
			system("pause");
			exit(1);
		}
		std::cout<<"Promedio de Errores = "<< ErrorAverage<<std::endl;
		std::cout<<"Desviacion Estandar de Errores = "<< ErrorStandarDesviation<<std::endl;
	}
	

	std::cout<<std::endl<<"Probando valores Iris:"<<std::endl;
	IrisNN.mInputs[0]     = 5.1; IrisNN.mInputs[1]  = 3.5; IrisNN.mInputs[2]     = 1.4; IrisNN.mInputs[3]  = 0.2;
	Outputs = IrisNN.Process();
	std::cout<<"Input: 5.1 3.5 1.4 0.2\tOutput: "<< IrisNN.mCodeFunction(Outputs,Outputs.size())<<std::endl;
	//std::cout<<"Procesing: output 1 = "<<Outputs[0]<<"   output 2 = "<<Outputs[1]<<"   output 3 = "<<Outputs[2]<<std::endl;//Deberia dar 0
	IrisNN.mInputs[0]     = 4.9; IrisNN.mInputs[1]  = 3; IrisNN.mInputs[2]     = 1.4; IrisNN.mInputs[3]  = 0.2;
	Outputs = IrisNN.Process();
	std::cout<<"Input: 4.9 3 1.4 0.2\tOutput: "<< IrisNN.mCodeFunction(Outputs,Outputs.size())<<std::endl;
	//std::cout<<"Procesing: output 1 = "<<Outputs[0]<<"   output 2 = "<<Outputs[1]<<"   output 3 = "<<Outputs[2]<<std::endl;//Deberia dar 0
	IrisNN.mInputs[0]     = 6.2; IrisNN.mInputs[1]  = 2.2; IrisNN.mInputs[2]     = 4.5; IrisNN.mInputs[3]  = 1.5;
	Outputs = IrisNN.Process();
	std::cout<<"Input: 6.2 2.2 4.5 1.5\tOutput: "<< IrisNN.mCodeFunction(Outputs,Outputs.size())<<std::endl;
	//std::cout<<"Procesing: output 1 = "<<Outputs[0]<<"   output 2 = "<<Outputs[1]<<"   output 3 = "<<Outputs[2]<<std::endl;//Deberia dar 1
	IrisNN.mInputs[0]     = 7.4; IrisNN.mInputs[1]  = 2.8; IrisNN.mInputs[2]     = 6.1; IrisNN.mInputs[3]  = 1.9;
	Outputs = IrisNN.Process();
	std::cout<<"Input: 7.4 2.8 6.1 1.9\tOutput: "<< IrisNN.mCodeFunction(Outputs,Outputs.size())<<std::endl;
	//std::cout<<"Procesing: output 1 = "<<Outputs[0]<<"   output 2 = "<<Outputs[1]<<"   output 3 = "<<Outputs[2]<<std::endl;//Deberia dar 2

	do
	{
		std::cout<<"Ingrese valores de consulta: ";
		std::cin>>IrisNN.mInputs[0];std::cout<<' '; std::cin>>IrisNN.mInputs[1];std::cout<<' ';
		std::cin>>IrisNN.mInputs[2];std::cout<<' '; std::cin>>IrisNN.mInputs[3];std::cout<<std::endl;

		Outputs = IrisNN.Process();
		std::cout<<"Input: "<<IrisNN.mInputs[0]<<' '<<IrisNN.mInputs[1]<<' '<<IrisNN.mInputs[2]<<' '
			     <<IrisNN.mInputs[3]<<"\tOutput: "<< IrisNN.mCodeFunction(Outputs,Outputs.size())<<std::endl;
	}while( Preguntar("Seguir?") );

	if( Preguntar("Esta conforme con los resultados?") )
	{
		IrisNN.Serialize("iris_Entrenado");
		std::cout<<"Archivo de pesos guardado en iris_Entrenado.csv..."<<std::endl;
	}

	}
	#pragma endregion

	std::cout<<std::endl<<std::endl;
	std::cout<<"--------------------------------------------------------------------"<<std::endl;
	system("pause");
	*/

	return 0;
}

