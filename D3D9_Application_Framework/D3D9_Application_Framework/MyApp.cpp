#pragma once

#include "stdafx.h"
#include "MyApp.h"

	//Singleton Variable Initialization
MyApp* MyApp::Singleton = NULL;

MyApp* pMyApp = MyApp::GetSingleton();//create the unique instance


MyApp* MyApp::GetSingleton()
	{
	if( !MyApp::Singleton )
		MyApp::Singleton = new MyApp();

	return MyApp::Singleton;	
	}


