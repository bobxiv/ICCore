#line 1 "CommandHandlers - copia.h"
#pragma once




















namespace ConfigDialog
{

	using namespace Ribbon;

	
	
	
	
	class CRPCommandHandler: public IUICommandHandler
	{
		public:

			
			__checkReturn static HRESULT CreateInstance(__deref_out IUICommandHandler **ppCommandHandler);

			
			STDMETHODIMP_(ULONG) AddRef();
			STDMETHODIMP_(ULONG) Release();
			STDMETHODIMP QueryInterface(REFIID iid, void** ppv);
			
			
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

			
			BOOL m_HALEnable;
			BOOL m_REFEnable;
			BOOL m_WARPEnable;

			HRESULT m_hr;						

			LONG m_cRef;                        
	};

}

#line 72 "CommandHandlers - copia.h"
