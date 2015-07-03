#include "stdafx.h"

#include "MyGUI.h"

#include "MyApp.h"

//#include "FrameworkBase\\Header\\GUI\\Controls.h"
#include <Commdlg.h>


namespace GUIFramework
{

	#pragma region EventHandlers Forward Declarations

	bool On_mButtonRight_Click(int x, int y);

	bool On_mButtonLeft_Click(int x, int y);

	bool On_mPlayToggle_Click(int x, int y);

	bool On_mSave_Click(int x, int y);

	bool On_mLoad_Click(int x, int y);

	#pragma endregion

		//Singleton Variable Initialization
	MyGUI* MyGUI::Singleton = NULL;

	MyGUI* MyGUI::GetSingleton()
		{
		if( !Singleton )
			Singleton = new MyGUI();

		return Singleton;	
		}

	//YOUR CODE HERE

	/*void MyGUI::ResolveEvents()
		{
		return;
		}*/

	void MyGUI::Load()
		{

		pPublicDevice = this->pDevice;//Need the Device Public to be use with the controls

			//Initialize you controls, and hang then from the GUI Tree
		int W;
		int H;
		this->GetFrameDimensions(W,H);

		mRightButton.SetPosition(AppPoint(W-140,20));
		mRightButton.SetCaption(std::wstring(L">>"));
		mRightButton.AddHandler(Button::HandlerType::OnMouseClickHType, (bool (*)(void))&On_mButtonRight_Click);
		this->Controls.push_back(&mRightButton);

		mLeftButton.SetPosition(AppPoint(W-120-165,20));
		mLeftButton.SetCaption(std::wstring(L"<<"));
		mLeftButton.AddHandler(Button::HandlerType::OnMouseClickHType, (bool (*)(void))&On_mButtonLeft_Click);
		this->Controls.push_back(&mLeftButton);
		
		mDetalle.SetPosition(AppPoint(W-120-90,50));
		mDetalle.SetCaption(L"0.05");
		this->Controls.push_back(&mDetalle);

		mPlayToggle.SetPosition(AppPoint(W-120-90,90));
		mPlayToggle.SetCaption(std::wstring(L"Play"));
		mPlayToggle.AddHandler(Button::HandlerType::OnMouseClickHType, (bool (*)(void))&On_mPlayToggle_Click);
		this->Controls.push_back(&mPlayToggle);

		mSave.SetPosition(AppPoint(W-120-90,130+10));
		mSave.SetCaption(std::wstring(L"Save"));
		mSave.AddHandler(Button::HandlerType::OnMouseClickHType, (bool (*)(void))&On_mSave_Click);
		this->Controls.push_back(&mSave);

		mLoad.SetPosition(AppPoint(W-120-90,160+10));
		mLoad.SetCaption(std::wstring(L"Load"));
		mLoad.AddHandler(Button::HandlerType::OnMouseClickHType, (bool (*)(void))&On_mLoad_Click);
		this->Controls.push_back(&mLoad);
	
		}

	void MyGUI::UnLoad()
		{
			//Make any nessesary unitialization

		return;
		}

	void MyGUI::RenderGUI(const double elapsedTime)
		{
				//Our GUI will need alpha blending
			pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
			pDevice->SetRenderState(D3DRS_SRCBLEND        , D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND       , D3DBLEND_INVSRCALPHA);

				//Makes the rendering and the presentation of the backbuffer
			GUI::RenderGUI(elapsedTime);
			return;
		}

	#pragma region EventHandlers Section

	bool On_mButtonRight_Click(int x, int y)
	{
		MyApp* mainApp = MyApp::GetSingleton();
		float newDelta = mainApp->currentDelta - 0.01;
		mainApp->currentDelta = newDelta;

		if( newDelta < 0.01 )
		{
			MessageBoxA(NULL,"No se puede crear una grilla mas pequeña!", "Limite de Grilla", MB_OK|MB_ICONASTERISK);
		}
		else
		{
			delete mainApp->pGrid;
			mainApp->pGrid = new Grid(newDelta, MyGUI::GetSingleton()->pPublicDevice, mainApp->pGridDeclaration);
			mainApp->State.Refresh = true;

			wchar_t txt[40];
			if( newDelta < 0.0001 )
				swprintf(txt, L"%.5f", newDelta);
			else
			{
				if( newDelta < 0.001 )
					swprintf(txt, L"%.4f", newDelta);
					else
					{
						if( newDelta < 0.01 )
							swprintf(txt, L"%.3f", newDelta);
						else
							swprintf(txt, L"%.2f", newDelta);
					}
			}

			std::wstring stringTxt(txt);

			MyGUI::GetSingleton()->mDetalle.SetCaption(stringTxt);

		}

		return true;
	}

	bool On_mButtonLeft_Click(int x, int y)
	{
		MyApp* mainApp = MyApp::GetSingleton();
		float newDelta = mainApp->currentDelta + 0.01;
		mainApp->currentDelta = newDelta;

		if( newDelta > 0.2 )
		{
			MessageBoxA(NULL,"No se puede crear una grilla mas grande!", "Limite de Grilla", MB_OK|MB_ICONASTERISK);
		}else
		{
			delete mainApp->pGrid;
			mainApp->pGrid = new Grid(newDelta, MyGUI::GetSingleton()->pPublicDevice, mainApp->pGridDeclaration);
			mainApp->State.Refresh = true;

			wchar_t txt[40];
			if( newDelta < 0.0001 )
				swprintf(txt, L"%.5f", newDelta);
			else
			{
				if( newDelta < 0.001 )
					swprintf(txt, L"%.4f", newDelta);
					else
					{
						if( newDelta < 0.01 )
							swprintf(txt, L"%.3f", newDelta);
						else
							swprintf(txt, L"%.2f", newDelta);
					}
			}

			std::wstring stringTxt(txt);

			MyGUI::GetSingleton()->mDetalle.SetCaption(stringTxt);
		}

		return true;
	}

	bool On_mPlayToggle_Click(int x, int y)
	{
		if( MyApp::GetSingleton()->State.Learning == true )
		{
			MyApp::GetSingleton()->State.Learning = false;
			MyGUI::GetSingleton()->mPlayToggle.SetCaption(L"Play");
		}else
		{
			MyApp::GetSingleton()->State.Learning = true;
			MyGUI::GetSingleton()->mPlayToggle.SetCaption(L"Stop");
		}
		return true;
	}

	bool On_mSave_Click(int x, int y)
	{
		LPOPENFILENAMEA fileName = new OPENFILENAMEA;
		memset(fileName, 0, sizeof(OPENFILENAMEA));
		fileName->lpstrFilter = "CSV\0*.csv\0Text\0*.txt\0XML\0*.xml\0\0";
		fileName->nFilterIndex = 1;
		fileName->lpstrTitle  = "Guardar Estructura Entrenada";
		fileName->lStructSize = sizeof(OPENFILENAMEA);
		fileName->hwndOwner = NULL;
		fileName->lpstrFile = new char[512]; fileName->lpstrFile[0]='\0';
		fileName->nMaxFile = 512;
		fileName->lpstrFileTitle = new char[512]; fileName->lpstrFileTitle[0]='\0';
		fileName->nMaxFileTitle = 512;

		char* curDir = new char[512];
		GetCurrentDirectoryA(512, curDir);
		fileName->lpstrInitialDir = curDir;

		bool res = GetSaveFileNameA(fileName);
		if( res != 0 )
		{
			try
			{
				std::string aux(fileName->lpstrFile);
				int posEnd = aux.rfind('\\');
				aux = aux.substr(0, posEnd);
				SetCurrentDirectoryA(aux.c_str());

				switch( MyApp::GetSingleton()->CurrentDisplayMethod )
				{
				case MyApp::DisplayMethod::MLNN:
					MyApp::GetSingleton()->pMyNNetwork->Serialize(std::string(fileName->lpstrFileTitle));
					break;
				case MyApp::DisplayMethod::SOM:
					MyApp::GetSingleton()->pMySOM->Serialize(std::string(fileName->lpstrFileTitle));
					break;
				case MyApp::DisplayMethod::RBF:
					break;
				}
			}catch(std::exception* e)
			{
				MessageBoxA(NULL, "Error", e->what(), MB_ICONERROR|MB_OK);
			}
		}
		else
		{
			DWORD error = CommDlgExtendedError();
			if( error != 0 )//if 0 then just pressed cancel
				MessageBoxA(NULL, "Error", "No se a podido cargar el archivo", MB_ICONERROR|MB_OK);
		}

		delete[] fileName->lpstrFile;
		delete[] fileName->lpstrInitialDir;
		delete[] fileName->lpstrFileTitle;
		delete fileName;

		return true;
	}

	bool On_mLoad_Click(int x, int y)
	{
		LPOPENFILENAMEA fileName = new OPENFILENAMEA;
		memset(fileName, 0, sizeof(OPENFILENAMEA));
		fileName->lpstrFilter = "CSV\0*.csv\0Text\0*.txt\0XML\0*.xml\0\0";
		fileName->nFilterIndex = 1;
		fileName->lpstrTitle  = "Abrir Estructura Entrenada";
		fileName->lStructSize = sizeof(OPENFILENAMEA);
		fileName->hwndOwner = NULL;
		fileName->lpstrFile = new char[512]; fileName->lpstrFile[0]='\0';
		fileName->nMaxFile = 512;
		fileName->lpstrFileTitle = new char[512]; fileName->lpstrFileTitle[0]='\0';
		fileName->nMaxFileTitle = 512;
		
		char* curDir = new char[512];
		GetCurrentDirectoryA(512, curDir);
		fileName->lpstrInitialDir = curDir;

		bool res = GetOpenFileNameA(fileName);
		if( res != 0 )
		{
			try
			{
				std::string aux(fileName->lpstrFile);
				int posEnd = aux.rfind('\\');
				aux = aux.substr(0, posEnd);
				SetCurrentDirectoryA(aux.c_str());

				switch( MyApp::GetSingleton()->CurrentDisplayMethod )
				{
				case MyApp::DisplayMethod::MLNN:
					MyApp::GetSingleton()->pMyNNetwork->Load(std::string(fileName->lpstrFileTitle));
					break;
				case MyApp::DisplayMethod::SOM:
					MyApp::GetSingleton()->pMySOM->Load(std::string(fileName->lpstrFileTitle));
					break;
				case MyApp::DisplayMethod::RBF:
					break;
				}
				
				MyApp::GetSingleton()->State.Refresh = true;
			}catch(std::exception* e)
			{
				MessageBoxA(NULL, "Error", e->what(), MB_ICONERROR|MB_OK);
			}
		}
		else
		{
			DWORD error = CommDlgExtendedError();
			if( error != 0 )//if 0 then just pressed cancel
				MessageBoxA(NULL, "Error", "No se a podido cargar el archivo", MB_ICONERROR|MB_OK);
		}

		delete[] fileName->lpstrFile;
		delete[] fileName->lpstrInitialDir;
		delete[] fileName->lpstrFileTitle;
		delete fileName;

		return true;
	}

	#pragma endregion

}