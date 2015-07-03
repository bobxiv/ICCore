#include "stdafx.h"

#include <iostream>
#include <fstream>

void NormalCircle(int N)
{
	std::ofstream fileStream("NormalCircle.csv");

	if( !fileStream.is_open() )
		throw (new std::exception( "NormalCircle -> Could not create the file" ) );

	float dAng   = (2*3.14f)/(float)N;
	float curAng = 0.0f;
	for(int i=0; i < N ; ++i)//not N+1 because the circle is cyclic
	{
		fileStream<<cos(curAng)<<','<<sin(curAng)<<','<<42<<std::endl;
		curAng += dAng;
	}

	fileStream.close();
}

void NormalT(int N) 
{
	std::ofstream fileStream("NormalT.csv");

	if( !fileStream.is_open() )
		throw (new std::exception( "NormalT -> Could not create the file" ) );

	double a[2];

	double b=0,d=0;

	if(fileStream.good())
	{

		for (int i=0;i<=(N/2);i++)
		{
			b =(float)i/(float)(N/2);

			for(int j=0;j<=(N/2);j++)
			{

				d =(float)j/(float)(N/2);

				//a[0]=-1*(1-b)+1*(b);
				a[0]= -1 + 2*b;

				//a[1]=-1*(1-d)+1*(d);
				a[1]= -1 + 2*d;

				//Generates:
				//
				//					|										  ------
				//					|						   or
				if( abs(a[0])<=0.25 &&  (a[1]<=0.5 && a[1]>=-1) || abs(a[0])<=1 &&  (a[1]<=1 && a[1]>=0.5) ) 
					fileStream<<a[0]<<','<<a[1]<<','<<42<<std::endl;
			}
		}
	}

	fileStream.close();
}

