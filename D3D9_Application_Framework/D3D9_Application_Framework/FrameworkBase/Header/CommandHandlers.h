#pragma once

#ifndef COMMANDHANDLERS_H
#define COMMANDHANDLERS_H

#include "..\\..\\stdafx.h"
//#include <uiribbon.h>
//#include <UIRibbonPropertyHelpers.h>
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

//using namespace Ribbon;

namespace ConfigDialog
{

	using namespace Ribbon;

	//This handle is in charge of: Tab DirectX / Group Device Kind
	//Rules:
	//- Only one of the 3 ToggleButtons can be activated at once
	//- One Button must be activated
	class CRPCommandHandler: public IUICommandHandler/*Immplements IUICommandHandle*/
	{
		public:

			// Static method to create an instance of the object.
			__checkReturn static HRESULT CreateInstance(__deref_out IUICommandHandler **ppCommandHandler);

			// IUnknown methods.
			STDMETHODIMP_(ULONG) AddRef();
			STDMETHODIMP_(ULONG) Release();
			STDMETHODIMP QueryInterface(REFIID iid, void** ppv);
			
			// IUICommandHandler methods
			STDMETHOD(UpdateProperty)(UINT nCmdID,
									  __in REFPROPERTYKEY key,
									  __in_opt const PROPVARIANT* ppropvarCurrentValue,
									  __out PROPVARIANT* ppropvarNewValue);

			STDMETHOD(Execute)(UINT nCmdID,
							   UI_EXECUTIONVERB verb, 
							   __in_opt const PROPERTYKEY* key,
							   __in_opt const PROPVARIANT* ppropvarValue,
							   __in_opt IUISimplePropertySet* pCommandExecutionProperties);

		private:
			CRPCommandHandler(): m_cRef(1)
			{}

			// Control States
			BOOL m_HALEnable;
			BOOL m_REFEnable;
			BOOL m_WARPEnable;

			HRESULT m_hr;						// Error Handle

			LONG m_cRef;                        // Reference count.
	};

}

#endif