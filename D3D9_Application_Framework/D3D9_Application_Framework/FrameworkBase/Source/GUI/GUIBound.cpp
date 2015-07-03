#pragma once

#include "stdafx.h"
#include "..\\..\\Header\\GUI\\GUIObject.h"
#include "..\\..\\Header\\GUI\\GUIBound.h"


namespace GUIFramework
{

	GUIRectangle::GUIRectangle(GUIObject* obj)
		{
			Owner = obj;
			A.x=-0.5;A.y=0.5;
			B.x=0.5;B.y=-0.5;
		}

	GUIRectangle::GUIRectangle(GUIObject* obj, D3DXVECTOR2 UpperLeft, D3DXVECTOR2 LowerRight):
			  A(UpperLeft), B(LowerRight)
		{
			Owner = obj;  
		}

	D3DXVECTOR2 GUIRectangle::SetB(D3DXVECTOR2& P2)
		{
			D3DXVECTOR2 aux = B;
			B = P2;

			return aux;
		}

	D3DXVECTOR2 GUIRectangle::SetA(D3DXVECTOR2& P1)
		{
			D3DXVECTOR2 aux = A;
			A = P1;
			
			return aux;
		}

	bool GUIRectangle::IsInside(AppPoint P)
	{
			//Transform to real Coordinates
			AppPoint O = Owner->GetPosition();

			int w = Owner->GetWidth();
			int h = Owner->GetHeight();

				//RA and RB are the real coordinates of the bound in window space
			AppPoint RA(O.X+w*(A.x+0.5), O.Y+h*(A.y-0.5));
			AppPoint RB(O.X+w*(B.x+0.5), O.Y+h*(-B.y+0.5));

			if( P.X > RA.X && P.X < RB.X )
			{
				if( P.Y < RB.Y && P.Y > RA.Y )
					return true;
			}
			return false;
		}

}