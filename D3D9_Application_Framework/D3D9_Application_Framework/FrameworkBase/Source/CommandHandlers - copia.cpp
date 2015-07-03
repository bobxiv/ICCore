#include "stdafx.h"

#include "..\\Header\\CommandHandlers.h"

#include "..\\Header\\ProConfigDialog.h"

namespace ConfigDialog
{
	extern IUIFramework* pRibbonFramework;

	// Static method to create an instance of the object.
	__checkReturn HRESULT CRPCommandHandler::CreateInstance(__deref_out IUICommandHandler **ppCommandHandler)
	{
		if (!ppCommandHandler)
		{
			return E_POINTER;
		}

		*ppCommandHandler = NULL;

		HRESULT hr = S_OK;

		CRPCommandHandler* pCommandHandler = new CRPCommandHandler();

		if (pCommandHandler != NULL)
		{
			*ppCommandHandler = static_cast<IUICommandHandler *>(pCommandHandler);
		}
		else
		{
			hr = E_OUTOFMEMORY;
		}

		return hr;
	}

	// IUnknown method implementations.
	STDMETHODIMP_(ULONG) CRPCommandHandler::AddRef()
	{
		return InterlockedIncrement(&m_cRef);
	}

	STDMETHODIMP_(ULONG) CRPCommandHandler::Release()
	{
		LONG cRef = InterlockedDecrement(&m_cRef);
		if (cRef == 0)
		{
			delete this;
		}

		return cRef;
	}

	STDMETHODIMP CRPCommandHandler::QueryInterface(REFIID iid, void** ppv)
	{
		if (iid == __uuidof(IUnknown))
		{
			*ppv = static_cast<IUnknown*>(this);
		}
		else if (iid == __uuidof(IUICommandHandler))
		{
			*ppv = static_cast<IUICommandHandler*>(this);
		}
		else 
		{
			*ppv = NULL;
			return E_NOINTERFACE;
		}

		AddRef();
		return S_OK;
	}

	// IUICommandhandler Implementation
	STDMETHODIMP CRPCommandHandler::Execute(UINT nCmdID,									/*Control ID*/
								UI_EXECUTIONVERB verb,										/*Action or Event*/
								__in_opt const PROPERTYKEY* key,							/*Property Changing*/
								__in_opt const PROPVARIANT* ppropvarValue,					/*Value of the propery changing*/
								__in_opt IUISimplePropertySet* pCommandExecutionProperties)
	{
		UNREFERENCED_PARAMETER(pCommandExecutionProperties);
		UNREFERENCED_PARAMETER(ppropvarValue);
		m_hr = S_OK;

		#if _DEBUG
		////////////////////////////////
		///// Output the Execution /////
		////////////////////////////////
		char s[60];
		if( verb == UI_EXECUTIONVERB_EXECUTE )
			sprintf(s,"VERB -> EXECUTE\n");
		else if( verb == UI_EXECUTIONVERB_PREVIEW )
			sprintf(s,"VERB -> PREVIEW\n");
		else
			sprintf(s,"VERB -> CANCELPREVIEW\n");
		OutputDebugStringA(s);
		////////////////////////////////
		/////   Output the cmdID   /////
		////////////////////////////////
		if( nCmdID == cmdHAL )
			sprintf(s,"Control -> HAL\n");
		else if( nCmdID == cmdREF )
			sprintf(s,"Control -> REF\n");
		else if( nCmdID == cmdWARP )
			sprintf(s,"Control -> WARP\n");
		OutputDebugStringA(s);
		////////////////////////////////
		#endif
		
		switch (verb)//Action switch
		{
		case UI_EXECUTIONVERB_EXECUTE:
		case UI_EXECUTIONVERB_PREVIEW:
		case UI_EXECUTIONVERB_CANCELPREVIEW:
			switch( nCmdID )//Control switch
			{
				case cmdHAL:
					{
						if( key != NULL || *key ==  UI_PKEY_BooleanValue )
						{
							m_HALEnable = !m_HALEnable;

							PROPVARIANT var;
							if( !m_HALEnable )//inposible to deactivate, only selecting another
							{
								m_HALEnable = true;
								InitPropVariantFromBoolean(true, &var);
								m_hr = pRibbonFramework->SetUICommandProperty(cmdHAL, UI_PKEY_BooleanValue, var);  
								if( FAILED(m_hr) ) return m_hr;
							}
								
							InitPropVariantFromBoolean(false, &var);
							m_hr = pRibbonFramework->SetUICommandProperty(cmdREF, UI_PKEY_BooleanValue, var);  
							if( FAILED(m_hr) ) return m_hr;
							m_hr = pRibbonFramework->SetUICommandProperty(cmdWARP, UI_PKEY_BooleanValue, var);  
							if( FAILED(m_hr) ) return m_hr;

							#if _DEBUG
								PROPVARIANT varHAL;
								PROPVARIANT varREF;
								PROPVARIANT varWARP;
								pRibbonFramework->GetUICommandProperty(cmdHAL, UI_PKEY_BooleanValue, &varHAL);
								pRibbonFramework->GetUICommandProperty(cmdREF, UI_PKEY_BooleanValue, &varREF);
								pRibbonFramework->GetUICommandProperty(cmdWARP, UI_PKEY_BooleanValue, &varWARP);
								sprintf(s,"HAL: %i REF: %i   WARP: %i\n", varHAL.boolVal, varREF.boolVal, varWARP.boolVal);
								OutputDebugStringA(s);
							#endif

							if (FAILED(m_hr))
								return m_hr;
						}
					}
					break;

				case cmdREF:
					{
						if( key != NULL || *key ==  UI_PKEY_BooleanValue )
						{
						m_REFEnable = !m_REFEnable;

						PROPVARIANT var;
						if( !m_REFEnable )//inposible to deactivate, only selecting another
						{
							m_REFEnable = true;
							InitPropVariantFromBoolean(true, &var);
							m_hr = pRibbonFramework->SetUICommandProperty(cmdREF, UI_PKEY_BooleanValue, var);  
							if( FAILED(m_hr) ) return m_hr;
						}
							
						InitPropVariantFromBoolean(false, &var);
						m_hr = pRibbonFramework->SetUICommandProperty(cmdHAL, UI_PKEY_BooleanValue, var);  
						if( FAILED(m_hr) ) return m_hr;
						m_hr = pRibbonFramework->SetUICommandProperty(cmdWARP, UI_PKEY_BooleanValue, var);  
						if( FAILED(m_hr) ) return m_hr;

						#if _DEBUG
							PROPVARIANT varHAL;
							PROPVARIANT varREF;
							PROPVARIANT varWARP;
							pRibbonFramework->GetUICommandProperty(cmdHAL, UI_PKEY_BooleanValue, &varHAL);
							pRibbonFramework->GetUICommandProperty(cmdREF, UI_PKEY_BooleanValue, &varREF);
							pRibbonFramework->GetUICommandProperty(cmdWARP, UI_PKEY_BooleanValue, &varWARP);
							sprintf(s,"HAL: %i REF: %i   WARP: %i\n", varHAL.boolVal, varREF.boolVal, varWARP.boolVal);
							OutputDebugStringA(s);
						#endif

						if (FAILED(m_hr))
							return m_hr;
						}
					}
					break;

				case cmdWARP:
					{
						if( key != NULL || *key ==  UI_PKEY_BooleanValue )
						{
							m_WARPEnable = !m_WARPEnable;

							PROPVARIANT var;
							if( !m_WARPEnable )//inposible to deactivate, only selecting another
							{
								m_WARPEnable = true;
								InitPropVariantFromBoolean(true, &var);
								m_hr = pRibbonFramework->SetUICommandProperty(cmdWARP, UI_PKEY_BooleanValue, var);  
								if( FAILED(m_hr) ) return m_hr;
							}

							InitPropVariantFromBoolean(false, &var);
							m_hr = pRibbonFramework->SetUICommandProperty(cmdHAL, UI_PKEY_BooleanValue, var);  
							if( FAILED(m_hr) ) return m_hr;
							m_hr = pRibbonFramework->SetUICommandProperty(cmdREF, UI_PKEY_BooleanValue, var);  
							if( FAILED(m_hr) ) return m_hr;

							#if _DEBUG
								PROPVARIANT varHAL;
								PROPVARIANT varREF;
								PROPVARIANT varWARP;
								pRibbonFramework->GetUICommandProperty(cmdHAL, UI_PKEY_BooleanValue, &varHAL);
								pRibbonFramework->GetUICommandProperty(cmdREF, UI_PKEY_BooleanValue, &varREF);
								pRibbonFramework->GetUICommandProperty(cmdWARP, UI_PKEY_BooleanValue, &varWARP);
								sprintf(s,"HAL: %i REF: %i   WARP: %i\n", varHAL.boolVal, varREF.boolVal, varWARP.boolVal);
								OutputDebugStringA(s);
							#endif

							if (FAILED(m_hr))
								return m_hr;
						}
					}
					break;

			}//End ControlID switch

		}//End Action/Execution Switch

		#if _DEBUG
			OutputDebugStringA("\n---------\n");
		#endif

		return m_hr;
	}

	STDMETHODIMP CRPCommandHandler::UpdateProperty(UINT nCmdID,					 /*Control ID*/
								__in REFPROPERTYKEY key,						 /*Property Key Affected*/
								__in_opt const PROPVARIANT* ppropvarCurrentValue,/*Current Value*/
								__out PROPVARIANT* ppropvarNewValue)			 /*New Value*/
	{
		UNREFERENCED_PARAMETER(nCmdID);
		UNREFERENCED_PARAMETER(key);
		UNREFERENCED_PARAMETER(ppropvarCurrentValue);
		UNREFERENCED_PARAMETER(ppropvarNewValue);

		//HRESULT m_hr = E_FAIL;
		m_hr = S_OK;

		return m_hr;
	}


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////


}