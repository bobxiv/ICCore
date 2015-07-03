#pragma once

class D3DXFRAME_Derived;

namespace XFileModel
{

	namespace XFileParser
	{

		class XParser;
		class XAnimationSet;

		class XAnimationParser: public XParser
			{
			private:

				//Specialization of XParser:ParseObject
				virtual BOOL ParseObject(ID3DXFileData* pDataObj,
									 ID3DXFileData* pParentObj,
									 DWORD Depth,
									 void** Data,
									 BOOL Reference);

			public:
				
				//Number of AnimationsSets loaded
				DWORD m_NumOfAnimationSets;
				//List of AnimationsSets loaded
				XAnimationSet* m_AnimationSets;

				XAnimationParser(): m_NumOfAnimationSets(0), m_AnimationSets(NULL)
					{}

				~XAnimationParser()
					{}

				//Load the Animations(AnimationSets) of the XFile
				//Parameter:
				//- Filename   the name of the file to get the animations [IN]
				BOOL Load(std::string Filename)
					{
					if( Parse(Filename.c_str(), NULL) == FALSE )
						return FALSE;
					
					return TRUE;
					}

				//Maps or links the Animations Data with his corresponding Frame(Bone)
				//The Name of Animation Bone will have to be equal to the Frame Name(normally this is the way)
				//Parameter:
				//- RootFrame	  main RootFrame from witch to search the animations corresponding Frame [IN]
				void Map(D3DXFRAME_Derived* RootFrame);

				//Updates an Animation(AnimationSet) changing the current total animation time elapsed
				//Parameter:
				//- AnimationName name of the animation to play [IN]
				//- Time		  new time of the animation [IN]
				void Update(const char* AnimationName, DWORD Time);

				XAnimationSet* FindAnimationSet(const char* AnimationName);

			};

	}

}