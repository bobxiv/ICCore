#pragma once

#ifndef PROCONFIGDIALOG_H
#define PROCONFIGDIALOG_H

#include "..\\..\\MyApp.h"

#include <atlbase.h>
#include <atlcom.h>
#include <initguid.h>

#include "CommandHandlers.h"

//namespace ConfigDialog
//{
//	namespace Ribbon
//	{
//		#include <uiribbon.h>
//
//		#pragma comment(lib, "propsys.lib")
//		#include <UIRibbonPropertyHelpers.h>
//	}
//}

// The current Ribbon definitions
#include "..\\..\\RibbonRes.h"

namespace ConfigDialog
{
 	using namespace Ribbon;

	class CConfigDialogApplication: public IUIApplication, public IUICommandHandler
		{
		private:

			static CConfigDialogApplication* Singleton;

		protected:
			//Render Plataform: handles -> HAL/REF/WARP
			IUICommandHandler* m_pRPCommandHandler;

			
			BOOL m_VSyncEnable;
			BOOL m_FTSEnable;

			HRESULT m_hr;							// Error Handle
			LONG m_cRef;                            // Reference count.

			CConfigDialogApplication();

		public:

			//Gets the Singleton Application
			static __checkReturn CConfigDialogApplication* GetSingleton();

			// IUnknown method implementations

				STDMETHODIMP_(ULONG) AddRef();

				STDMETHODIMP_(ULONG) Release();

				STDMETHODIMP QueryInterface(REFIID iid, void** ppv);

			// IUIApplication Implementation

				STDMETHOD(OnViewChanged)(UINT32 nViewID, 
										 __in UI_VIEWTYPE typeID,
										 __in IUnknown* pView,
										 UI_VIEWVERB verb,
										 INT32 uReasonCode);

				STDMETHOD(OnCreateUICommand)(UINT32 nCmdID, 
											 __in UI_COMMANDTYPE typeID,
											 __deref_out IUICommandHandler** ppCommandHandler); 

				STDMETHOD(OnDestroyUICommand)(UINT32 commandId, 
											  __in UI_COMMANDTYPE typeID,
											  __in_opt IUICommandHandler* pCommandHandler);

			// IUICommandhandler Implementation
				
				STDMETHODIMP Execute(UINT nCmdID,												/*Control ID*/
									UI_EXECUTIONVERB verb,										/*Action or Event*/
									__in_opt const PROPERTYKEY* key,							/*Property Changing*/
									__in_opt const PROPVARIANT* ppropvarValue,					/*Value of the propery changing*/
									__in_opt IUISimplePropertySet* pCommandExecutionProperties);

				STDMETHODIMP UpdateProperty(UINT nCmdID,									 /*Control ID*/
											__in REFPROPERTYKEY key,						 /*Property Key Affected*/
											__in_opt const PROPVARIANT* ppropvarCurrentValue,/*Current Value*/
											__out PROPVARIANT* ppropvarNewValue);			 /*New Value*/

		};

	//Initialize Ribbon Framework and Application
	bool InitRibbon(HWND hDialogWnd);
	//Destroys Ribbon Framework and Application
	bool DestroyRibbon();


		//DE = Dialog Event 
	bool DE_InitializeDialog(HWND hDlg);

		//CE = Control Event	|	 One per (Control,Event) pair
	void CE_SetDisplayModeClick(HWND hDlg);

	void CE_ResolutionSelectionChange(HWND hDlg, HWND control);

	void CE_BackBufferSelectionChange(HWND hDlg, HWND control);

	void CE_DepthStencilSelectionChange(HWND hDlg, HWND control);

		//Auxiliary Function
	//Checks and updates the current display mode
	void UpdateLabelCurrentDisplayMode(D3DDISPLAYMODE currentDisplayMode, HWND hDlg);
	//Checks and updates the buffers/display mode compatibility
	void UpdateLabelCompatibility(HWND hDlg);

}

#endif