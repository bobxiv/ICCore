#pragma once

#ifndef INDIVIDUO_H
#define INDIVIDUO_H

#include "vector"

using namespace std;

//Interfaz de Individuo
class IndividuoBase
{
public:
	IndividuoBase();

	~IndividuoBase();

	void Set(int nin,int ncapas,int nperc[]);
	
	//Devuelve la posicion
	vector< vector < vector<double> > > Get_w();

	//Setea la mejor selecion para este individuo
	void Set_best(vector< vector < vector<double> > >wg);

	double GetFitness();

	//Actualiza la velocidad
	void UpdateV(float c1,float c2,vector< vector <vec> >wg);

	//Actualiza la posicion
	void UpdateX();
};


class individuoPML {
private:
	//Matriz de pesos de la red neuronal, w[i][j][k] -> i es la capa, j es el perceptron, k es el peso dentro del perceptron
	vector< vector <vec> >w;

	//Matriz de velocidad de los pesos de la red neuronal, w[i][j][k] -> i es la capa, j es el perceptron, k es el peso dentro del perceptron
	vector< vector <vec> >v;

	//La mejor selecion de pesos para la red para este individuo
	vector< vector <vec> >best;
	
public:

	static PML *pNN;

	static Particion *pP;

	individuoPML();

	~individuoPML();

	void Set(int nin,int ncapas,int nperc[]);
	
	//Devuelve la posicion
	vector< vector <vec> > Get_w();

	//Setea la mejor selecion para este individuo
	void Set_best(vector< vector <vec> >wg);

	//Actualiza la velocidad
	void UpdateV(float c1,float c2,vector< vector <vec> >wg);

	//Actualiza la posicion
	void UpdateX();

	double GetFitness();

	bool printWeight();
	
};

#endif

