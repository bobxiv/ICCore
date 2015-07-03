#pragma once

#include "stdafx.h"
#include "..\\..\\Header\\ProPrimitives\\Point.h"

namespace ProPrimitive
{

	AppPoint& AppPoint::operator=(const AppPoint& B)
		{
			this->X = B.X;
			this->Y = B.Y;
			return (*this);
		}

	bool AppPoint::operator==(const AppPoint& B) const
	{
		return (this->X == B.X && this->Y == B.Y);
	}

	AppRectangle& AppRectangle::operator=(const AppRectangle& B)
		{
			this->P1 = B.P1;
			this->P2 = B.P2;
			return (*this);
		}

	bool AppRectangle::operator==(const AppRectangle& B) const
	{
		return (this->P1 == B.P1 && this->P2 == B.P2);
	}

}

