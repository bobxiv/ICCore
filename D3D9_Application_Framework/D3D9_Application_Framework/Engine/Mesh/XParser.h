#pragma once

//Standard templates for XFiles
extern unsigned char D3DRM_XTEMPLATES[];
#define D3DRM_XTEMPLATE_BYTES 3278

namespace XFileModel
{

	namespace XFileParser
	{

		//Abstract Class for Parsing X model files
		//Intended to be specialize for own X file templates
		class XParser
			{

			protected:

				//Executes at the begining of the Parse Function
				//Intended to be specialize
				virtual BOOL BeginParse(void** Data)
					{return TRUE;}

				//Executes at the ending of the Parse Function
				//Intended to be specialize
				virtual BOOL EndParse(void** Data)
					{return TRUE;}

				//Parses the data elements of a XFileData object
				//then calls ParseChildObject of this object
				//Intended to be specialize
				//Parameter:
				//- pDataObj	currently parsing object [IN]
				//- pParentObj	parent of the parsing object [IN]
				//- Depth		depth in the hierarchy [IN]
				//- Data		Data [IN/OUT]
				//- Reference	if it is a Reference XFileData [IN]
				virtual BOOL ParseObject(ID3DXFileData* pDataObj,
										 ID3DXFileData* pParentObj,
										 DWORD Depth,
										 void** Data,
										 BOOL Reference)throw(std::exception) = 0;

				//Parses the children of a XFileData object
				//For each calls ParseObject
				//Parameter:
				//- pDataObj		currently object, this node children will be call ParseObject [IN]
				//- Depth			depth in the hierarchy [IN]
				//- Data			Data [IN]
				//- ForceReference	if it will be forced to be a Reference XFileData [IN]
				virtual BOOL ParseChildObject(ID3DXFileData* pDataObj,
											  DWORD Depth,
											  void** Data,
											  BOOL ForceReference = FALSE);

			public:

				//Function to parse an X file
				//Parameter:
				//strFilename the name of the X file with extension
				//Data		  model data loaded
				BOOL Parse(LPCSTR strFilename,void** Data);

				//Function get the Global Unique ID of a XFileData object
				//Parameter:
				//pDataObj the XFileData element from whom to get the GUID
				GUID *GetObjectGUID(ID3DXFileData *pDataObj)
					{
					LPGUID pGuid = new GUID;
					if( FAILED( pDataObj->GetType(pGuid) ) )
						return NULL;
					return pGuid;
					}

				//Function get the Name of a XFileData object
				//Parameter:
				//pDataObj the XFileData element from whom to get the name
				char* GetObjectName(ID3DXFileData *pDataObj)
					{
					SIZE_T size=0;
					pDataObj->GetName(NULL,&size);
					char* str = new char[size];
					if( FAILED(pDataObj->GetName(str,&size)) )
						return NULL;
					return str;
					}
			};


	}

}