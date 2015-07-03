////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////					   User App			    	  //////////////////////////////////////
///////////////////////////////////					Derived from App		      //////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "FrameworkBase\\Header\\App.h"

#include "FrameworkBase\\Header\\ProPrimitives\\Quad.h"
//#include "MyGUI.h"


#include "CommonVertexClases.h"//We use VertexColor for the grid


// The render clases used with the IC methods
#include "Grid.h"
#include "SOMRenderer.h"

//ICCore include
#include <IC.h>


//Singleton Class
//The user App, this is where you have to put your code.
//This clase derives from the base App class. The responsibility
//from the base App is to initialize input devices, graphic devices,
//manage the camera, the console, the initial specification panel, etc
//
//The specifics of your App are responsibility from this clase
//
//IMPORTANT: the only overhead of this framework is the virtuals calls
//			 that the base class makes from this class
class MyApp: public App
{
	private:
			//Singleton Variable
		static MyApp* Singleton;

			//To catch error results
		HRESULT hr;

		//-----------------------
		//	Your States
		//-----------------------

			//YOUR CODE HERE
			//THIS IS AN EXAMPLE
		float Xmin;
		float Xmax;
		float Ymin;
		float Ymax;

		GUIFramework::GUIBasicEffect BasicEffect;

		HMENU hPopupMenu;
		DWORD ID_POPUP_MLNN;
		DWORD ID_POPUP_SOM;
		DWORD ID_POPUP_RBF;

		LPDIRECT3DVERTEXDECLARATION9 pQuadVertexDeclaration;//To be set for the GUI

		//------------------------
		//	Your Private Methods
		//------------------------


		//-----------------------

	protected:

		MyApp(void): App(), CurrentDisplayMethod(DisplayMethod::MLNN), pCurNetwork(NULL){}

		MyApp(const MyApp&){}

		MyApp& operator= (const MyApp&){}

	public:
			//The singleton getter
		static MyApp* GetSingleton();

			//-----------------------
			//	Virtual Functions
			//-----------------------

		//Execute after getting the window and the devices,
		//makes the user app initializations
		virtual void Initialize();

		//Execute after Initialize, loads all the nesesary 
		//DirectX and user resources
		virtual void LoadContent();

		//Execute before quitting the app, it just unloads
		//the resources loaded in LoadContent(NO OTHER)
		virtual void UnloadContent();

		//Execute 1 time per loop, it updates all
		//
		//- elapsedTime the elapsed time between this frame and the previous
		virtual void Update(const double elapsedTime);

		//Execute 1 time per loop, it renders the scene
		//
		//- elapsedTime the elapsed time between this frame and the previous
		virtual void Render(const double elapsedTime);

		//------------------------
		//	Your Public Methods
		//------------------------

			//YOUR CODE HERE

		/*void CalculateGridAndCreateBuffers(float delta);

		void CreateGridVertexBuffers();

		void CreateGridIndexBuffers();

		void CreateGrid();*/

		//------------------------
		//	Your Public Properties
		//------------------------

			//YOUR VARIABLES HERE

		IGeneralICNetwork* pCurNetwork;

		SOM*          pMySOM;
		struct SOMConfig
		{
			SOMConfig(): GlobalIterations(0), TransitionIterations(0), FineIterations(0)
			{}
			int GlobalIterations;
			int TransitionIterations;
			int FineIterations;
		};
		SOMConfig MySOMConfig;

		SOMRenderer*  pMySOMRenderer;

		MultiLayerNN*                pMyNNetwork;

		LPDIRECT3DVERTEXDECLARATION9 pGridDeclaration;

		Grid*                        pGrid;

		enum DisplayMethod{ MLNN, RBF, SOM };
		//The IC technic that is currentply displaying
		DisplayMethod CurrentDisplayMethod;

		struct LearnState
		{
			LearnState(): Learning(false), Refresh(true){}

			bool Learning;//When the NN should be learning
			bool Refresh; //When the NN change(but not because of the learning process)
						  //by some way and the grid need to be re-calculated
		};

		LearnState State;

		int Seasson;

		float currentDelta;
	
};
