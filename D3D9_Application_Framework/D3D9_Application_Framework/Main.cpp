#include "stdafx.h"
#include "MyApp.h"

#include <rmxftmpl.h>

using namespace ProPrimitive;

extern MyApp* pMyApp;//YOUR APP(ALREADY DECLEARE AND DEFINE IN App.cpp)

//Matrices de Transformacion
D3DXMATRIXA16 View;
D3DXMATRIXA16 Proyection;

#include "GenerateFigures.h"

//Important: if want to use PIX with this .exe then define PIX_ENABLE in the proyect
//or put the next commented line in "stdafx.h"
//#define PIX_ENABLE

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
	{
		NormalCircle(75);
		NormalT(75);

	//App::GetSingleton() GETS THE UNIQUE INSTANCE OF YOUR APP, BUT pMyApp ALREADY HAVE IT

		//Set the state of the app, if desired, before calling the main loop
	//Example: MyApp->SetWindowStyle(CS_HREDRAW|CS_VREDRAW,WS_OVERLAPPED,NULL); This is the default

		//Set the creation setup of the Application
	pMyApp->CreationSetup.HasDirectSoundManager = false;//true;
	pMyApp->SetTitle(L"IC Graficador de Entrenamiento: MLNN");
		
	pMyApp->ToggleConsole();
		
	pMyApp->Run(hInstance,nCmdShow,(App::InputDevice)(App::InputDevice::KeyboardInput|App::InputDevice::MouseInput));//ENTERS THE MAIN LOOP
	
	delete pMyApp;//FREE YOUR APP
	return 0;
	}


	//APP BEHAVE FUNCTIONS

void MyApp::Initialize()
	{
	//INITIALIZE YOUR APPLICATION(here you already have all the devices initialized)
	Camera.SetProjection(D3DX_PI/4,0.001f,100.0f);
	Camera.Position(*(new D3DXVECTOR3(0,0,-1)));
	Camera.Upper(*(new D3DXVECTOR3(1,0,0)));
	Camera.LookAt(*(new D3DXVECTOR3(0,0,0)) );

	//pD3dDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	pD3dDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	pD3dDev->SetRenderState(D3DRS_LIGHTING, false);
	pD3dDev->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(255,255,255));
	pD3dDev->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	pD3dDev->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	pD3dDev->SetSamplerState(0, D3DSAMP_ADDRESSW, D3DTADDRESS_WRAP);
	pD3dDev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	pD3dDev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	pD3dDev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

	//pD3dDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	//pD3dDev->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	//pD3dDev->SetRenderState(D3DRS_SRCBLEND , D3DBLEND_SRCALPHA);
	//pD3dDev->SetRenderState(D3DRS_DESTBLEND , D3DBLEND_INVSRCALPHA);

	ID_POPUP_MLNN = 0xFFFFF;
	ID_POPUP_SOM  = 0xFFFFF1;
	ID_POPUP_RBF  = 0xFFFFF2;

	hPopupMenu = CreatePopupMenu();
	InsertMenu(hPopupMenu, 0, MF_BYPOSITION | MF_STRING, ID_POPUP_SOM , L"SOM");
	InsertMenu(hPopupMenu, 0, MF_BYPOSITION | MF_STRING, ID_POPUP_RBF , L"RBF");
	InsertMenu(hPopupMenu, 0, MF_BYPOSITION | MF_STRING, ID_POPUP_MLNN, L"MLNN");
	SetForegroundWindow(this->hWnd);
	
	
	return;
	}

void MyApp::LoadContent()
	{
	//LOAD YOUR CONTENT
		//pMySOM      = new ::SOM(2,1,5);		 //dummy inicialization
		//pMySOM      = new ::SOM(2,5,5);			 //dummy inicialization
		//pMySOM      = new ::SOM(2,4,4);			 //Con concent se van las 16 neuronas(4x4) a el centro... debe ser lo que muchas tiran se concentran
		pMySOM      = new ::SOM(2,25,1);
		//pMySOM      = new ::SOM(2,16,1);
		//pMySOM      = new ::SOM(2,4,1);			 //Resuelve XOR perfecto, mapa 2x2 oscila mucho
		pMyNNetwork = new MultiLayerNN(2,3,10,2);//dummy inicialization
		pCurNetwork = pMyNNetwork;
		try
		{
			std::cerr<< "Cargando archivo entrenamiento de Red Neuronal Multicapa..." << std::endl;
			pMyNNetwork->LoadLearnData("XOR");
			std::cerr<< "Archivo cargado"<< std::endl;

			std::cerr<<"Cargando archivo entrenamiento de SOM..."<<std::endl;
			//pMySOM->LoadLearnData     ("clouds");
			//pMySOM->LoadLearnData     ("concent");
			//pMySOM->LoadLearnData("XOR");
			//pMySOM->LoadLearnData     ("NormalSquare");
			//pMySOM->LoadLearnData     ("NormalCircle");
			pMySOM->LoadLearnData     ("NormalT");
			//pMySOM->LoadLearnData     ("t");
			std::cerr<<"Archivo cargado"<<std::endl;
			
		}catch(std::exception* e)
		{
			this->Error(e->what());
			if( this->HasConsole() )
				system("pause");
				
			this->Exit(1);
		}

		pMyNNetwork->mSignFunction = [] (double val)->double {
											if( val > 0.0 )
												return 1.0;
											else
												return -1.0;
											};
		pMyNNetwork->mErrorFunction = [] (double y,double yd)->double 
											{
												return (yd-y);
											};

		pMyNNetwork->mPartitionFunction = [] (double val)->double {
											double aux = std::pow(2.71, -val);
											return ( (1-aux) / (double)(1+aux) );
											};

		pMyNNetwork->mCodeFunction = [] (std::vector<double> res, int n)->double {
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

		pMyNNetwork->mInertia = 0.1f;
		pMyNNetwork->mK = 0.1f;

		pMySOM->mKGlobal = 0.5f;//0.2;
		//Gaussian
		pMySOM->mKTransicion = [](int difX, int difY, int n) -> double
		{
			if( (difX == 0) && (difY == 0) )
				return 0.3f;

			double exp = -(difX*difX + difY*difY)/(double)(2*(2*2));//with norm deriv 2 in x and y
			return (0.3f*pow(2.71,exp));
		};
		//Inv Distance
		/*pMySOM->mKTransicion = [](int difX, int difY, int n) -> double
		{
			if( (difX == 0) && (difY == 0) )
				return 0.2f;
			return 0.2/(sqrt((double)(difX*difX+difY*difY)));
		};*/
		pMySOM->mKConvergence = 0.01;//0.1;

		currentDelta = 0.05;

		Xmin = -1;
		Xmax = 1;
		Ymin = -1;
		Ymax = 1;
		
		Seasson = 0;

		hr = this->pD3dDev->CreateVertexDeclaration(VertexColor::VertexElement, &pGridDeclaration);
		if( FAILED(hr) )
		{
			this->Error("Vertex Declaration for Grid imposible to create");
			if( this->HasConsole() )
				system("pause");
			this->Exit(1);
		}

		std::cerr<< "Creando Renderizador de Red Neuronal Multicapa..." << std::endl;
		pGrid = new Grid(currentDelta, this->pD3dDev, this->pGridDeclaration);

		
		hr = this->pD3dDev->CreateVertexDeclaration(ProPrimitive::QuadVertexElement, &pQuadVertexDeclaration);
		if( FAILED(hr) )
		{
			this->Error("Vertex Declaration for Grid imposible to create");
			if( this->HasConsole() )
				system("pause");
			this->Exit(1);
		}

		//pGrid->GetGridIndexBuffers(pGridAdjacency);

		BasicEffect.Initialize();

		std::cerr<< "Creando Renderizador de SOM..." << std::endl;
		pMySOMRenderer = new SOMRenderer(pMySOM, 0.1f, 0.1f, BasicEffect, this->pD3dDev);

		//pDev = this->pD3dDev;
		//pMyNNetwork->Learn(0.0f, 2000, false, false, std::cout, RenderTraining);

		
	return;
	}

void MyApp::UnloadContent()
	{
	//FREE HERE THE RESOURCES LOADED IN LoadContent
		pGridDeclaration->Release();

		delete pMySOMRenderer;
		delete pMySOM;
		delete pMyNNetwork;
		delete pGrid;
	return;
	}

void MyApp::Update(const double elapsedTime)
	{
	//PUT HERE YOUR LOOPs UPDATE CODE
	Keyboard.UpdateState();
	Mouse.UpdateState();

	if( Mouse.ReleaseRightClick() )//Menu Contextual de selecion de Tipo de Metodo de IC
	{							   //que se desea visualizar/entrenar
		
		LPPOINT lpCursor = new POINT();
		::GetCursorPos(lpCursor);
		DWORD res = TrackPopupMenu(hPopupMenu, TPM_NONOTIFY | TPM_RETURNCMD  | TPM_LEFTBUTTON | TPM_BOTTOMALIGN | TPM_LEFTALIGN, lpCursor->x, lpCursor->y, 0, this->hWnd, NULL);
		delete lpCursor;
		if( res != 0 )
		{
			if( res ==  this->ID_POPUP_MLNN)
			{
				this->SetTitle(L"IC Graficador de Entrenamiento: MLNN");
				CurrentDisplayMethod = DisplayMethod::MLNN;
				pCurNetwork = this->pMyNNetwork;
			}
			if( res ==  this->ID_POPUP_RBF )
			{
				this->SetTitle(L"IC Graficador de Entrenamiento: Not Implemented");
				CurrentDisplayMethod = DisplayMethod::RBF;
				pCurNetwork = NULL;
			}
			if( res ==  this->ID_POPUP_SOM )
			{
				this->SetTitle(L"IC Graficador de Entrenamiento: SOM");
				CurrentDisplayMethod = DisplayMethod::SOM;
				pCurNetwork = this->pMySOM;

				//The SOM train must know the large of transition training so we need to make 
				//a settup number of iterations to train
				std::cerr<<"Ingresar iteraciones Globales: ";
				std::cin>>MySOMConfig.GlobalIterations;
				std::cerr<<"Ingresar iteraciones de Transicion: ";
				std::cin>>MySOMConfig.TransitionIterations;
				std::cerr<<"Ingresar iteraciones de Convergencia: ";
				std::cin>>MySOMConfig.FineIterations;
				pMySOM->InitializeContinuousLearn(MySOMConfig.GlobalIterations,MySOMConfig.TransitionIterations,MySOMConfig.FineIterations, true, true, std::cerr);
			}
		}
	}
	
	//Entrenamiento y/o Refresco del Metodo
	if( State.Learning || State.Refresh )
	{

		if( State.Learning )
		{
			switch(CurrentDisplayMethod)
			{
			case DisplayMethod::MLNN:
				{
					pCurNetwork->Learn(400);//Learn N Seasson
				}
				break;
			case DisplayMethod::SOM:
				{
					this->pMySOM->ContinuousLearn();
				}
				break;
			}
			Seasson++;
			std::cerr<<"Epoca: "<<Seasson<<std::endl;
		}

		if( State.Refresh )
			State.Refresh = false;

		switch( CurrentDisplayMethod )//Actualizar el renderizador del metodo actual
		{
		case DisplayMethod::SOM:
			{
			pMySOMRenderer->Update();
			}
			break;
		case DisplayMethod::MLNN:
			{
				VertexColor* Grid = NULL;
				try
				{
					Grid = pGrid->LockGridVertexs();
				}catch(std::exception* e)
				{
					this->Error(e->what());
					if( this->HasConsole() )
						system("pause");
					this->Exit(1);
				}
				int GridBroad = pGrid->GetGridBroad();
		
				//Update grid
				Concurrency::parallel_for(size_t(0),size_t(pGrid->GetVertexCount()), [&](int value)
				{
					double* Outputs;
					int i = int(value / GridBroad);
					int j = value % GridBroad;
					int index = i*GridBroad+j;
					double* Inputs = new double[2];
					Inputs[0] = Xmin + Grid[index].x*(Xmax-Xmin);
					Inputs[1] = Ymin + Grid[index].y*(Ymax-Ymin);

					//pMyNNetwork->mInputs[0] = Grid[index].x; ESTO NO SE PUEDE HACER 
					//pMyNNetwork->mInputs[1] = Grid[index].y; POR PROBLEMA DE CONCURRENCIA
					//Outputs = pMyNNetwork->Process();
					Outputs = static_cast< double* >(pMyNNetwork->Process(Inputs,2));// Concurrent Safe!!!
					delete[] Inputs;
				
					Grid[index].Color.r = (Outputs[0]+1)/(double)2;
					Grid[index].Color.g = (Outputs[1]+1)/(double)2;
					Grid[index].Color.b = 0.0f;

					delete[] Outputs;
				});

				try
				{
					pGrid->UnlockGridVertexs();
				}catch(std::exception* e)
				{
					this->Error(e->what());
					if( this->HasConsole() )
						system("pause");
					this->Exit(1);
				}
			}
			break;
		case DisplayMethod::RBF:
			break;
		}

	}
	
	Camera.UpdateView();
	return;
	}

void MyApp::Render(const double elapsedTime)
	{
	//PUT HERE YOUR LOOPs RENDER CODE
		pD3dDev->Clear(0,0,D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,D3DCOLOR_XRGB(0,150,255),1.0f,1.0f);


	
	
	pD3dDev->BeginScene();

		switch(this->CurrentDisplayMethod)
		{
		case DisplayMethod::MLNN:
			{
				hr = pD3dDev->SetVertexDeclaration(pGridDeclaration);
				if( FAILED(hr) )
					{
						this->Error("Imposible to set the vertex declaration");
						if( this->HasConsole() )
							system("pause");
						this->Exit(1);
					}

				try
				{
					pGrid->Draw(BasicEffect);
				}catch(std::exception* e)
				{
					this->Error(e->what());
					if( this->HasConsole() )
						system("pause");
					this->Exit(1);
				}
			}
			break;
		case DisplayMethod::SOM:
			{
				try
				{
					pMySOMRenderer->Draw();
				}catch(std::exception* e)
				{
					this->Error(e->what());
					if( this->HasConsole() )
						system("pause");
					this->Exit(1);
				}
			}
			break;
		}

	pD3dDev->EndScene();

	//pD3dDev->Present(NULL,NULL,NULL,NULL);It is presented inside MyGUI

	pD3dDev->SetVertexDeclaration(pQuadVertexDeclaration);//So the GUI can render the controls...
	
	return;
	}

