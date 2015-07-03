#pragma once

#include "stdafx.h"
#include "..\\Header\\SoundBuffers.h"

SoundBuffersMG::~SoundBuffersMG(void)
	{
	OutputBuffers.clear();
	SoundDictionary.clear();

	if( pDSound )
		pDSound->Release();
	}

bool SoundBuffersMG::Initialize(const HWND hWnd)
	{
	
	hr = DirectSoundCreate8(NULL, &pDSound, NULL);
	if( FAILED(hr) )
		return false;

	hr = pDSound->SetCooperativeLevel( hWnd, DSSCL_NORMAL);
	if( FAILED(hr) )
		return false;

	return true;
	}

LPDIRECTSOUNDBUFFER SoundBuffersMG::CreateOutputBuffer(const long int BufferBytes, const long int SamplesPerSec=8000, 
													   const int BitsPerSample=32, const int Channels=1, void* pData=NULL)
	{
	DSBUFFERDESC desc;
	desc.dwSize = sizeof(DSBUFFERDESC);
	desc.dwFlags = DSBCAPS_CTRLFREQUENCY|DSBCAPS_CTRLVOLUME;
	desc.guid3DAlgorithm = DS3DALG_DEFAULT; 
	desc.dwBufferBytes = BufferBytes;
	desc.dwReserved = 0;

	WAVEFORMATEX waveFmt; 
	waveFmt.cbSize = sizeof(WAVEFORMATEX);
	waveFmt.nChannels = Channels;
	waveFmt.nSamplesPerSec = SamplesPerSec;
	waveFmt.wBitsPerSample = BitsPerSample;
	waveFmt.nBlockAlign = (waveFmt.nChannels*waveFmt.wBitsPerSample)/8;
	waveFmt.nAvgBytesPerSec = waveFmt.nSamplesPerSec*waveFmt.nBlockAlign;
	waveFmt.wFormatTag = WAVE_FORMAT_PCM;

	desc.lpwfxFormat = &waveFmt;

	LPDIRECTSOUNDBUFFER pStaticBuffer;	
	hr =  pDSound->CreateSoundBuffer( &desc, &pStaticBuffer,NULL);
	if( FAILED(hr) )
		return NULL;

	if( pData != NULL )
	{
		LPVOID pBufferData = NULL;
		DWORD dataSize = 0;
		pStaticBuffer->Lock(0, NULL, &pBufferData, &dataSize, NULL, NULL, DSBLOCK_ENTIREBUFFER);

		memcpy(pBufferData, (LPVOID)pData, desc.dwBufferBytes);

		pStaticBuffer->Unlock(pBufferData, desc.dwBufferBytes, NULL, NULL);
	}

	OutputBuffers.push_back(pStaticBuffer);
	
	return pStaticBuffer;
	}

Wave::~Wave()
{
	if( !pData  )
		delete[] pData;
}

bool Wave::Open(const LPTSTR strFileName, const WAVEFORMATEX* wavFmt, const DWORD dwFlags) {

	HMMIO hWav; // WAVE's handle
	MMCKINFO child, parent;
	
	LPDIRECTSOUNDBUFFER wavFileBuffer; // temporary buffer
	LPDIRECTSOUNDBUFFER *p_wavFileBuffer;
	
	p_wavFileBuffer = &wavFileBuffer;

	hWav = mmioOpen(strFileName, NULL, MMIO_READ | MMIO_ALLOCBUF);

	if (hWav==NULL) {
		//ErrorBox("Could not find / load file at mmioOpen()");
		return false;
	}

	parent.fccType = mmioFOURCC('W', 'A', 'V', 'E');

	// descend to riff
	if (mmioDescend(hWav, &parent, NULL, MMIO_FINDRIFF)) {
		//ErrorBox("Could not descend to RIFF, file is corrupt or not a WAVE file.");
		mmioClose(hWav, 0);
		return false;
	}

	// descend to WAVEfmt
	child.ckid = mmioFOURCC('f', 'm', 't', ' ');
	if (mmioDescend(hWav, &child,  &parent, 0)) {
		//ErrorBox("Could not descend to WAVEfmt, file is corrupt or not a WAVE file.");
		mmioClose(hWav, 0);
		return false;
	}

	// open <strong class="highlight">WAV</strong> file for reading, check to make sure format is correct
	if (mmioRead(hWav, (char*)&wfmex, sizeof(WAVEFORMATEX)) != sizeof(WAVEFORMATEX)) {
		//ErrorBox("Error reading <strong class="highlight">WAV</strong> format, file is corrupt or not a WAVE file.");
		mmioClose(hWav, 0);
		return false;
	}

	if (wfmex.wFormatTag != WAVE_FORMAT_PCM) {
		//ErrorBox("WAVE file is not of PCM format.");
		mmioClose(hWav, 0);
		return false;
	}

	if (mmioAscend(hWav, &child, 0)) {
		//ErrorBox("Error ascending to WAVE data.");
		mmioClose(hWav, 0);
		return false;
	}

	// descend to data
	child.ckid = mmioFOURCC('d', 'a', 't', 'a');
	if (mmioDescend(hWav, &child, &parent, MMIO_FINDCHUNK)) {
		//ErrorBox("Error descending to the WAVE data level.");
		mmioClose(hWav, 0);
		return false;
	}

	// verification complete, now <strong class="highlight">read</strong> information

	// allocate memory and load pointer
	if (!(pData = new UCHAR [child.cksize])) {
		//ErrorBox("Insufficient memory to load WAVE file.");
		mmioClose(hWav, 0);
		return false;
	}

	// <strong class="highlight">read</strong> WAVE data

	long lBytesRead;
	lBytesRead = mmioRead(hWav, (char*)pData, child.cksize);
	if (lBytesRead<0) {
		//ErrorBox("Unable to <strong class="highlight">read</strong> WAVE file data.");
		mmioClose(hWav, 0);
		return false;
	}

	mmioClose(hWav, 0);

	// place our information on our buffer, prepare our description for direct sound
	dsbd.dwSize = sizeof(DSBUFFERDESC);
	dsbd.dwBufferBytes = child.cksize; // size of buffer is equal to the wave's size
	dsbd.dwFlags = DSBCAPS_STATIC | 
				DSBCAPS_LOCSOFTWARE |     // Force loading in software so we can't loose it.
			    DSBCAPS_GLOBALFOCUS |     // Don't mute on 'lostfocus'
			    DSBCAPS_CTRLPAN | 
			    DSBCAPS_CTRLVOLUME | 
			    DSBCAPS_CTRLFREQUENCY;
	dsbd.lpwfxFormat = &wfmex;
	dsbd.dwReserved = NULL;
	dsbd.guid3DAlgorithm = DS3DALG_DEFAULT;

	return true;
}

SoundCue::SoundCue(LPDIRECTSOUND pDsound, Wave* pWave): m_lpWaveFile(pWave), m_lpDSBuffer(NULL),
										pDs(pDsound)
{
	HRESULT hr = pDs->CreateSoundBuffer(&m_lpWaveFile->dsbd, &m_lpDSBuffer, NULL);
}

SoundCue::SoundCue(const SoundCue& B)
{
	*this = B;
}

SoundCue::~SoundCue()
{
	if( m_lpWaveFile )
		delete m_lpWaveFile;
	if( m_lpDSBuffer )
		m_lpDSBuffer->Release();
	
}

SoundCue& SoundCue::operator=(const SoundCue& B)
{
	this->m_lpWaveFile = B.m_lpWaveFile;
	this->m_lpDSBuffer = B.m_lpDSBuffer;
	this->pDs = B.pDs;

	return *this;
}

bool SoundCue::Open(LPTSTR strFileName)
{
	if( !m_lpWaveFile )
		m_lpWaveFile = new Wave();

	WAVEFORMATEX fmt;
	m_lpWaveFile->Open(strFileName, &fmt, NULL);

	HRESULT hr = pDs->CreateSoundBuffer(&m_lpWaveFile->dsbd, &m_lpDSBuffer, NULL);
	LPVOID pData;
	DWORD dataSize = 0;
	m_lpDSBuffer->Lock(0, NULL, &pData, &dataSize, NULL, NULL, DSBLOCK_ENTIREBUFFER);

	memcpy(pData, (LPVOID)m_lpWaveFile->pData, m_lpWaveFile->dsbd.dwBufferBytes);

	m_lpDSBuffer->Unlock(pData, m_lpWaveFile->dsbd.dwBufferBytes, NULL, NULL);

	if (hr==DSERR_INVALIDPARAM) 
	{
		delete m_lpWaveFile;
		m_lpWaveFile = NULL;
		return false;
	}

	return true;
}

bool SoundCue::Play()
{
	HRESULT hr = m_lpDSBuffer->Play(0,0,DSBPLAY_LOOPING);
	if( FAILED(hr) )
		return false;
	return true;
}

void SoundCue::Pause()
{
	m_lpDSBuffer->Stop();
}

void SoundCue::Stop()
{
	m_lpDSBuffer->Stop();
	m_lpDSBuffer->SetCurrentPosition(0);
}

