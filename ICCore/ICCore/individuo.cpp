#include "stdafx.h"

#include "individuo.h"

individuoPML::individuoPML() {}

individuoPML::~individuoPML() {}

void individuoPML::Set(int nin, int ncapas, int nperc[])
{
		w.resize(ncapas);
		w[0].resize(nperc[0]);
		int i, j, k;
		for(i=0; i<w[0].size(); i++) {
			w[0][i]=vec(nin+1);
			for(k=0; k<w[0][i].s; k++) 
				w[0][i][k]=double(rand()) * 2 / double(RAND_MAX) - 1;
		}
		for(int i=1; i<w.size(); i++) {
			w[i].resize( nperc[i] );
			for(j=0; j<w[i].size(); j++) {
				w[i][j]=vec(nperc[i-1]+1);
				for(k=0; k<w[i][j].s; k++) 
					w[i][j][k]=double(rand()) * 2 / double(RAND_MAX) - 1;
			}
		}
		
		v.resize(ncapas);
		v[0].resize(nperc[0]);
		for(i=0; i<v[0].size(); i++) {
			v[0][i]=vec(nin+1);
			for(k=0; k<v[0][i].s; k++) 
				v[0][i][k]=0;
		}
		for(int i=1; i<v.size(); i++) {
			v[i].resize( nperc[i] );
			for(j=0; j<v[i].size(); j++) {
				v[i][j]=vec(nperc[i-1]+1);
				for(k=0; k<v[i][j].s; k++) 
					v[i][j][k]=0;
			}
		}
		
		best.resize(ncapas);
		best[0].resize(nperc[0]);
		for(i=0; i<best[0].size(); i++) {
			best[0][i]=vec(nin+1);
			for(k=0; k<best[0][i].s; k++) 
				best[0][i][k]=w[0][i][k];
		}
		for(int i=1; i<best.size(); i++) {
			best[i].resize( nperc[i] );
			for(j=0; j<best[i].size(); j++) {
				best[i][j]=vec(nperc[i-1]+1);
				for(k=0; k<best[i][j].s; k++) 
					best[i][j][k]=w[i][j][k];
			}
		}
}
	
vector< vector <vec> > individuoPML::Get_w()
{
	return w;
}

void individuoPML::Set_best(vector< vector <vec> > wg)
{
	best = wg;
}

double individuoPML::GetFitness()
{
	pNN->setw(w);
	return pNN->nerror(*pP,false);
}

void individuoPML::UpdateV(float c1, float c2, vector< vector <vec> > wg)
{
	int i,j,k;
	for(i=0; i < v.size(); i++)
	{
		for(j=0; j < v[i].size(); j++)
		{
			for(k=0; k < v[i][j].s; k++)
			{
				double r1=(double(rand())/ double(RAND_MAX));
				double r2=(double(rand())/ double(RAND_MAX));
				v[i][j][k]= v[i][j][k] + c1*r1*(best[i][j][k]-w[i][j][k]) + c2*r2*(wg[i][j][k]-w[i][j][k]);
			}
		}
	}
}

void individuoPML::UpdateX()
{
	int i,j,k;
	for(i=0; i < w.size(); i++)
	{
		for(j=0; j < w[i].size(); j++)
		{
			for(k=0; k < w[i][j].s; k++)
				w[i][j][k]= w[i][j][k]+v[i][j][k];
		}
	}
}

bool individuoPML::printWeight()
{
	if(w.empty()) return false;
	unsigned int i, nc=w.size(), j, np, k, nw;
	for(i=0; i<nc; i++) {
		cout<<"Capa: "<<i<<endl;
		vector< vec >& wi = w[i];
		np = wi.size();
		for(j=0; j<np; j++) {
			cout<<"   Perceptron: "<<j<<endl;
			vec& wij = wi[j];
			nw=wij.s;
			for(k=0; k<nw; k++) cout<<"      w["<<k<<"] -> "<<wij[k]<<endl;
		}
	}
	return true;
}
