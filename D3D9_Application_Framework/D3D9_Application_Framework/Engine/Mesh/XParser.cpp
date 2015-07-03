#pragma once

#include "stdafx.h"

#include "XParser.h"

namespace XFileModel
{

	namespace XFileParser
	{

		BOOL XParser::Parse(LPCSTR strFilename,void** Data)
			{
			HRESULT hr=0;

			ID3DXFile* pXFile=NULL;
			//Creates an X File
			hr = D3DXFileCreate(&pXFile);
			if( FAILED(hr) )
				return FALSE;

			//Registers the Standard X File Templates
			if(FAILED(pXFile->RegisterTemplates(
					 (LPVOID)D3DRM_XTEMPLATES,   
					  D3DRM_XTEMPLATE_BYTES))) 
			{
				pXFile->Release();
				return FALSE;
			}

			ID3DXFileEnumObject* pXFileEnumObj;
			//Creates an EnumObject of the specific X File to start the parsing of the roots
			hr = pXFile->CreateEnumObject(strFilename,D3DXF_FILELOAD_FROMFILE,&pXFileEnumObj);
			if( FAILED(hr) )
				{
				pXFile->Release();
				return FALSE;
				}

			SIZE_T childCount=0;
			hr = pXFileEnumObj->GetChildren(&childCount);
			if( FAILED(hr) )
				{
				pXFileEnumObj->Release();
				pXFile->Release();
				return FALSE;
				}

			//Executes the BeginParse, interesting if specialized
			if( BeginParse( Data ) == FALSE )
				{
				pXFileEnumObj->Release();
				pXFile->Release();
				return FALSE;
				}

			//Actual Parsing of the root XFileData
			for( int child=0;child < childCount; ++child )
				{

				ID3DXFileData* pFileData;

				hr = pXFileEnumObj->GetChild( child, &pFileData);
				if( FAILED(hr) )
					{
					pXFileEnumObj->Release();
					pXFile->Release();
					return FALSE;
					}

				//Call to Parse this XFileData object
				if( ParseObject(pFileData, NULL, 0, Data, false) == FALSE )
					{
					pFileData->Release();
					break;
					}

				pFileData->Release();

				}

			//Executes the EndParse, interesting if specialized
			if( EndParse( Data ) == FALSE )
				{
				pXFileEnumObj->Release();
				pXFile->Release();
				return FALSE;
				}

			pXFileEnumObj->Release();
			pXFile->Release();

			return TRUE;
			}


		BOOL XParser::ParseChildObject(ID3DXFileData* pDataObj,
									  DWORD Depth,
									  void** Data,
									  BOOL ForceReference)
			{
			HRESULT hr=0;

			SIZE_T childCount=0;
			hr = pDataObj->GetChildren(&childCount);
			if( FAILED(hr) )
				return FALSE;

			//Parses all the children of the XFileData object
			for( int child = 0; child <  childCount; ++child)
				{
				ID3DXFileData* pChildObj = NULL;
				hr = pDataObj->GetChild(child, &pChildObj);
				if( FAILED(hr) )
					return FALSE;

				//Special call if it is a refenrence
				if( pChildObj->IsReference() )
					{
						if( ParseObject(pChildObj, pDataObj, Depth+1, Data, true) == FALSE )
						{
						pChildObj->Release();
						break;
						}
					}else
					{

					if( ParseObject(pChildObj, pDataObj, Depth+1, Data, ForceReference) == FALSE )
						{
						pChildObj->Release();
						break;
						}
					}

				pChildObj->Release();
				}

			return TRUE;
			}

	}

}