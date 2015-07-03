#ifndef ENJAMBRE_H
#define ENJAMBRE_H
#include "utiles.h"
#include "vec.h"
#include "individuo.h"
using namespace std;

template <typename T>
class Enjambre{
private:

	int cant;

	//Array de Individuos
	vector<T> x;

	//Fitness de cada individuo
	vector<double>f;

	//Fitness en 
	vector<double>fbest;

	//Apunta a el mejor individuo del vector x de individuos
	T* yglobal;

	//El fitness del individuo que apunta yglobal
	double yglob;

public:

	Enjambre(int cantindv,int nin,int ncapas,int nperc[]);

	~Enjambre();

	void Evaluar();

	void Run(PML &nn,const  Particion &P,unsigned int maxiter,double tol);
	
};

////////////////////////////////////////////////////////////////////////////////////
////////////////////////	Implementacion		////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

template< typename T >
Enjambre::Enjambre(int cantindv, int nin, int ncapas, int nperc[])
{
	cant=cantindv;
	x.resize(cant);
	for(int i=0;i<cant;i++) 
		x[i].set(nin,ncapas,nperc);
	fbest.resize(cant);
	for(int i=0;i<cant;i++) 
		fbest[i]=999999999;
	f.resize(cant);
	for(int i=0;i<cant;i++) 
		f[i]=999999999;
	yglob=999999999;
	yglobal=&x[0];
}

template< typename T >
Enjambre::~Enjambre()
{
	x.clear();
	f.clear();
	fbest.clear();
}

template< typename T >
void Enjambre::Evaluar()
{
	for(int h=0; h < cant ;h++)
		f[h] = x[h].GetFitness();
//	int index=0;
//	double minn=f[0];
//	for(int h=0;h<cant;h++){
//		if(f[h]<minn) {
//			minn=f[h];
//			index= h;
//		}
//	}
//	yglob = minn;
//	yglobal = &x[index];
}

template< typename T >
void Enjambre::Run(PML &nn, const  Particion &P, unsigned int maxiter, double tol)
{
	unsigned int iter=0;
	do
	{
		//cout<<yglob<<endl;
		if(yglob < tol) { 
			//nn.setw(yglobal->get_w());
			//cout<<"Salio por acà"<<endl;
			break;
		}
		Evaluar(nn,P);
		for(int h=0; h < cant ;h++)
		{
			if(f[h] < fbest[h])
			{
				x[h].set_best( x[h].get_w() );
				fbest[h] = f[h];
			}
			if(fbest[h] < yglob)
			{
				yglobal = &x[h];
				yglob   = fbest[h];
			}
		}
		//(*yglobal).printWeight();
		for(int h=0; h < cant ;h++)
		{
			x[h].UpdateV(2.0,2.3,(*yglobal).get_w());
			x[h].UpdateX();
		}
		//(*yglobal).printWeight();
		//int aux;	
		//cin>>aux;
		iter++;
	}while(iter<maxiter); 
		
}


#endif

