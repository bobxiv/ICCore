////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////				Sound Primitive Manager			  //////////////////////////////////////
///////////////////////////////////			Uses DirectSound(deprecated but		  //////////////////////////////////////
///////////////////////////////////			absolutely low level interfaze)		  //////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <vector>
#include <map>
#include <Windows.h>
#include <MMSystem.h>
#include <dsound.h>

class Wave;
class SoundCue;

class SoundBuffersMG
{
	private:
		HRESULT hr;
	public:
		SoundBuffersMG(): pDSound(NULL){}
		~SoundBuffersMG(void);

			//Variables, not protected because dont want to impose 
			//the way to work

		//Main Sound device
		LPDIRECTSOUND8 pDSound;
		//Normal Output Buffers
		std::vector<LPDIRECTSOUNDBUFFER> OutputBuffers;

		//SoundDictionary, a database for Name-SoundCue
		std::map<std::string,SoundCue*> SoundDictionary;

			//Basic Functionality, it is in the user to respect 
			//encapsulation or not

		//Initialize the Direct Sound, before this 
		//the Manager is unusable
		bool Initialize(const HWND hWnd);

		//Creates an Output Buffer and appends it to the
		//Output Buffers vector
		LPDIRECTSOUNDBUFFER CreateOutputBuffer(const long int BufferBytes, const long int SamplesPerSec, 
			const int BitsPerSample, const int Channels, void* pData);
		
	};

class Wave
{
	public:
		WAVEFORMATEX wfmex; // WAVE's format
		DSBUFFERDESC dsbd;  // local direct sound buffer description
		UCHAR *pData;		// file loads here

		Wave(): pData(NULL){}

		~Wave();

		bool Open(const LPTSTR strFileName, const WAVEFORMATEX* wavFmt, const DWORD dwFlags);
	};

class SoundCue
{
	private:
		LPDIRECTSOUND          pDs;
		//DWORD                m_dwDSBufferSize;
		Wave*				   m_lpWaveFile;
		LPDIRECTSOUNDBUFFER    m_lpDSBuffer;
		//DWORD                m_dwNumBuffers;
		//DWORD                m_dwCreationFlags;
	public:
		SoundCue(LPDIRECTSOUND pDsound): m_lpWaveFile(NULL), m_lpDSBuffer(NULL),
										pDs(pDsound){}

		SoundCue(LPDIRECTSOUND pDsound, Wave* pWave);

		SoundCue(const SoundCue& B);

		SoundCue& operator=(const SoundCue& B);

		~SoundCue();
		
		//bool inUse;
		bool Open(LPTSTR strFileName);

		bool Play();

		void Pause();

		void Stop();
	};

