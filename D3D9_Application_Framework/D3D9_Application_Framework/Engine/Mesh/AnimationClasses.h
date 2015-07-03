#pragma once 

#include <d3dx9.h>

namespace XFileModel
{

	namespace XFileParser
	{

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////	  KeyFrame Animation	 ////////////////////////////////////////
		/////////////////////////////////////////		Transformations      ////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////

		class XAnimationVectorKey
			{
			public:
				XAnimationVectorKey(): m_Time(0){}
				float m_Time;//Length time
				D3DXVECTOR3 m_VecKey;

			};

		class XAnimationQuaternionKey
			{
			public:
				XAnimationQuaternionKey(): m_Time(0){}
				float m_Time;//Length time
				D3DXQUATERNION m_QuatKey;
			};

		class XAnimationMatrixKey
			{
			public:
				XAnimationMatrixKey(): m_Time(0){}
				float m_Time;//Length time
				D3DXMATRIX m_MatKey;
			};

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////      KeyFrame Animation DataStructures	     ////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////


		//Animaiton Data
		class XAnimation
			{
			public:
				char* m_Name;//Bone Name
				//Bone
				D3DXFRAME* m_Bone;

				XAnimation* m_Next;//Another XAnimation linked with this

				//Transformations of the KeyFrame Animaiton:
				//------------------------------------------
				DWORD m_NumOfQuaternionKey;
				XAnimationQuaternionKey* m_QuaternionKeys;
				DWORD m_NumOfVectorKey;
				XAnimationVectorKey* m_VectorKeys;
				DWORD m_NumOfTranslationKey;
				XAnimationVectorKey* m_TranslationKeys;
				DWORD m_NumOfMatrixKey;
				XAnimationMatrixKey* m_MatrixKeys;

				XAnimation(): m_NumOfQuaternionKey(0), m_NumOfVectorKey(0), m_NumOfTranslationKey(0),
					m_NumOfMatrixKey(0), m_Bone(NULL), m_Next(NULL), m_QuaternionKeys(NULL),
					m_VectorKeys(NULL), m_TranslationKeys(NULL), m_MatrixKeys(NULL), m_Name(NULL){}
				~XAnimation(){}
			};

		//Group of Animations
		//Could be linked to another AnimationSet
		class XAnimationSet
			{
			public:
				char* m_Name;//Name of the Animation Group

				DWORD m_Length;//Total Length of the Animations

				XAnimationSet* m_Next;//Another AnimationSet linked with this

				//Quantity of Animations
				DWORD m_NumOfAnimations;
				//Animations Data
				XAnimation* m_Animations;

				XAnimationSet(): m_Length(0), m_NumOfAnimations(0), m_Next(NULL), 
					m_Animations(NULL), m_Name(NULL){}
				~XAnimationSet(){}
			};

	}

}