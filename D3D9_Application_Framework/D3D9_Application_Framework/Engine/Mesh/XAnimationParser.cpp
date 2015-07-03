#pragma once
#include "stdafx.h"

#include "XParser.h"
#include "XAnimationParser.h"

//Clases for KeyFrame Animation DataStructure
#include "AnimationClasses.h"

#include "XFrameParser.h"//Uses FindFrame in Map

namespace XFileModel
{

	namespace XFileParser
	{

		BOOL XAnimationParser::ParseObject(ID3DXFileData* pDataObj,
											  ID3DXFileData* pParentObj,
											  DWORD Depth,
											  void** Data,
											  BOOL Reference)
		{
		HRESULT hr=0;
		//Gets the GUID to know what is this XFileData object
		GUID* pGuid = GetObjectGUID(pDataObj);

		//if it is an AnimationSet
		if( *pGuid == TID_D3DRMAnimationSet )
			{
			XAnimationSet* pNewAnimationSet = new XAnimationSet();
			pNewAnimationSet->m_Name = GetObjectName(pDataObj);
			pNewAnimationSet->m_Next = m_AnimationSets;//Link this new AnimationSet to the main list
			m_AnimationSets = pNewAnimationSet;
			m_NumOfAnimationSets++;
			}

		//if it is an Animation Data
		if( *pGuid == TID_D3DRMAnimation && m_AnimationSets )
			{
			XAnimation* pNewAnimation = new XAnimation();
			pNewAnimation->m_Next = m_AnimationSets->m_Animations;
			m_AnimationSets->m_Animations = pNewAnimation;//Link this new Animation Data AnimationSet
			m_AnimationSets->m_NumOfAnimations++;
			}

		//if it is Frame(Bone)
		if( *pGuid == TID_D3DRMFrame && m_AnimationSets && m_AnimationSets->m_Animations
				&& Reference)
			{
			if( *GetObjectGUID(pParentObj) == TID_D3DRMAnimation )//if inside an Animation Data
				m_AnimationSets->m_Animations->m_Name = GetObjectName(pDataObj);
			}

		//if it is a KeyFrame of the Animation
		if( *pGuid == TID_D3DRMAnimationKey && m_AnimationSets && m_AnimationSets->m_Animations )
			{
			XAnimation* CurAnim = m_AnimationSets->m_Animations;
			DWORD* pData = NULL;
			SIZE_T size;
			pDataObj->Lock(&size,(LPCVOID*)&pData);
			
			//Read the XFile Template Properties
			DWORD Type = *pData++;
			DWORD NumOfKeys = *pData++;

			#pragma region Loads KeyFrame Data
			switch(Type)
				{
				case 0://Rotacion
					{
					delete[] CurAnim->m_QuaternionKeys;
					CurAnim->m_NumOfQuaternionKey = NumOfKeys;
					CurAnim->m_QuaternionKeys = new XAnimationQuaternionKey[NumOfKeys];

					for(int i=0; i < NumOfKeys ; ++i)
						{
						CurAnim->m_QuaternionKeys[i].m_Time = *pData++;
						if( CurAnim->m_QuaternionKeys[i].m_Time > m_AnimationSets->m_Length )
							m_AnimationSets->m_Length = CurAnim->m_QuaternionKeys[i].m_Time;

						pData++;//Esto se que es 4*4 = 16 bits, asi que no me molesto en leerlo

						CurAnim->m_QuaternionKeys[i].m_QuatKey.w = *pData++;
						CurAnim->m_QuaternionKeys[i].m_QuatKey.x = *pData++;
						CurAnim->m_QuaternionKeys[i].m_QuatKey.y = *pData++;
						CurAnim->m_QuaternionKeys[i].m_QuatKey.z = *pData++;
						}
					}
					break;
				case 1: //Escalado
					{
					delete[] CurAnim->m_VectorKeys;
					CurAnim->m_NumOfVectorKey = NumOfKeys;
					CurAnim->m_VectorKeys = new XAnimationVectorKey[NumOfKeys];

					for(int i=0; i < NumOfKeys ; ++i)
						{
						CurAnim->m_VectorKeys[i].m_Time = *pData++;
						if( CurAnim->m_QuaternionKeys[i].m_Time > m_AnimationSets->m_Length )
							m_AnimationSets->m_Length = CurAnim->m_QuaternionKeys[i].m_Time;

						pData++;//Esto se que es 4*4 = 16 bits, asi que no me molesto en leerlo
						D3DXVECTOR3* vecPtr = (D3DXVECTOR3*)pData;
						CurAnim->m_VectorKeys[i].m_VecKey = *vecPtr;

						pData += 3;
						}
					}
					break;

				case 2: //Traslacion
					{
					delete[] CurAnim->m_TranslationKeys;
					CurAnim->m_NumOfVectorKey = NumOfKeys;
					CurAnim->m_TranslationKeys = new XAnimationVectorKey[NumOfKeys];

					for(int i=0; i < NumOfKeys ; ++i)
						{
						CurAnim->m_TranslationKeys[i].m_Time = *pData++;
						if( CurAnim->m_TranslationKeys[i].m_Time > m_AnimationSets->m_Length )
							m_AnimationSets->m_Length = CurAnim->m_TranslationKeys[i].m_Time;

						pData++;//Esto se que es 4*4 = 16 bits, asi que no me molesto en leerlo
						D3DXVECTOR3* vecPtr = (D3DXVECTOR3*)pData;
						CurAnim->m_VectorKeys[i].m_VecKey = *vecPtr;

						pData += 3;
						}
					}
					break;

				case 4: // Matriz de Transformacion
					{
					delete[] CurAnim->m_MatrixKeys;
					CurAnim->m_NumOfMatrixKey = NumOfKeys;
					CurAnim->m_MatrixKeys = new XAnimationMatrixKey[NumOfKeys];

					for(int i=0; i < NumOfKeys ; ++i)
						{
						CurAnim->m_MatrixKeys[i].m_Time = *pData++;
						if( CurAnim->m_MatrixKeys[i].m_Time > m_AnimationSets->m_Length )
							m_AnimationSets->m_Length = CurAnim->m_MatrixKeys[i].m_Time;

						pData++;//Esto se que es 4*4 = 16 bits, asi que no me molesto en leerlo
						D3DXMATRIX* matPtr = (D3DXMATRIX*)pData;
						CurAnim->m_MatrixKeys[i].m_MatKey = *matPtr;

						pData += 16;
						}
					}
					break;
				}
			#pragma endregion

			pDataObj->Unlock();
			}
				
		//Parse Children
		return ParseChildObject(pDataObj,Depth,Data,Reference);
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////	  Utility Functions	     ////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void XAnimationParser::Map(D3DXFRAME_Derived* RootFrame)
			{

			XAnimationSet* AnimSet = this->m_AnimationSets;

			while( AnimSet != NULL )// for each AnimationSet
				{
				XAnimation* Anim = AnimSet->m_Animations;
				while( Anim != NULL )// for each Animation Data
					{
						Anim->m_Bone = D3DXFRAME_Derived::FindFrame(RootFrame,Anim->m_Name);//find corresponding Bone and link

					Anim = Anim->m_Next;
					}
				AnimSet = AnimSet->m_Next;
				}

			return;
			}


		void XAnimationParser::Update(const char* AnimationName, DWORD Time)
			{

			XAnimationSet* AnimSet;

			//Find Request AnimationSet
			AnimSet = FindAnimationSet(AnimationName);

			if( AnimSet == NULL )
				return;

			//For each Animation Update the Bone Transformation to Time
			XAnimation* Anim = AnimSet->m_Animations;
			for(int i=0; i < AnimSet->m_NumOfAnimations ;++i)
				{
				
				#pragma region Update Bone Transformation with Current KeyFrames
					//For each Bone Animation with Quaternions
				for(int r=0; r < Anim->m_NumOfQuaternionKey ;++r)
					{
						//Between the 2 keyframes
					if( (Anim->m_QuaternionKeys[r].m_Time <= Time) && (r+1 < Anim->m_NumOfQuaternionKey) 
						&& (Anim->m_QuaternionKeys[r+1].m_Time > Time) )
						{
						XAnimationQuaternionKey& CurQuatKey  = Anim->m_QuaternionKeys[r];
						XAnimationQuaternionKey& NextQuatKey = Anim->m_QuaternionKeys[r+1];
					
						D3DXMATRIX RotMatrix;
						D3DXMatrixRotationQuaternion(&RotMatrix, &CurQuatKey.m_QuatKey);

						DWORD Time1 = CurQuatKey.m_Time;
						DWORD Time2 = NextQuatKey.m_Time;

						float S = (float)(Time - Time1)/(float)(Time2 - Time1);//En Time = Time 1 S es 0, en Time = Time2 S es 1
						
						Anim->m_Bone->TransformationMatrix *= RotMatrix * S;//Interpolado Linealmente
						}
					}
					//For each Bone Animation with Matrixs 
				for(int r=0; r < Anim->m_NumOfMatrixKey ;++r)
					{
						//Between the 2 keyframes
					if( (Anim->m_MatrixKeys[r].m_Time <= Time) && (r+1 < Anim->m_NumOfMatrixKey) 
						&& (Anim->m_MatrixKeys[r+1].m_Time > Time) )
						{
						XAnimationMatrixKey& CurMatrixKey  = Anim->m_MatrixKeys[r];
						XAnimationMatrixKey& NextMatrixKey = Anim->m_MatrixKeys[r+1];

						//Linear Interpolation between 2 key matrices:
						//
						// T = k1 + s*( k2-k1 ) = (1-s)*K1 + s*K2   
						//
						//			where:	- T  is the new transformation of the matrix
						//					- Ki is the i key transformation
						//					- s  is the coeficient of progress in the current keyframe
						//						(linear coeficient)
					
						DWORD Time1 = CurMatrixKey.m_Time;
						DWORD Time2 = NextMatrixKey.m_Time;

						float S = (float)(Time - Time1)/(float)(Time2 - Time1);//if Time = Time1 -> S = 0
																			   //if Time = Time2 -> S = 1

						Anim->m_Bone->TransformationMatrix = (1-S) * CurMatrixKey.m_MatKey + 
																		S * NextMatrixKey.m_MatKey;
						}
					}
				//Faltan los otros tipos de movimientos
				#pragma endregion

				Anim = Anim->m_Next;
				}

			return;
			}

		XAnimationSet* XAnimationParser::FindAnimationSet(const char* AnimationName)
		{
			XAnimationSet* AnimSet = this->m_AnimationSets;

			//searches the AnimationSet with Name equal to AnimationName
			if( AnimationName )
				{
				while(AnimSet != NULL)
					{
					if( strcmp(AnimSet->m_Name,AnimationName) == 0)
						break;
					AnimSet = AnimSet->m_Next;
					}
				}

			return AnimSet;
		}

	}
}