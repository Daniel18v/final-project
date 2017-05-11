#pragma once
#include "XAudio2.h"
#include <atlbase.h>
#include <memory>
#include <vector>

class XAudioGalaga
{
	//--------------------------------------------------------------------------------------
	// Callback structure
	//--------------------------------------------------------------------------------------
	struct StreamingVoiceContext : public IXAudio2VoiceCallback
	{
		StreamingVoiceContext(XAudioGalaga& audio) :
			_audio(audio)
		{
		}
		virtual ~StreamingVoiceContext()
		{
		}

		// IXAudio2VoiceCallback
		STDMETHOD_(void, OnVoiceProcessingPassStart)(UINT32) override {}
		STDMETHOD_(void, OnVoiceProcessingPassEnd)() override {}
		STDMETHOD_(void, OnStreamEnd)() override {}
		STDMETHOD_(void, OnBufferStart)(void*) override {}
		STDMETHOD_(void, OnBufferEnd)(void*) override {
			_audio.next();
		}
		STDMETHOD_(void, OnLoopEnd)(void*) override {}
		STDMETHOD_(void, OnVoiceError)(void*, HRESULT) override {}

	private:
		XAudioGalaga& _audio;
	};

public:
	XAudioGalaga();
	~XAudioGalaga();

	void RenderAudio(const std::vector<int>& samples);
	void next();

private:
	CComPtr<IXAudio2> _pMusicEngine;
	IXAudio2SourceVoice* _pSourceVoice;
	StreamingVoiceContext _voiceContext;
	IXAudio2MasteringVoice* _pMasteringVoice;

	static const int STREAMING_BUFFER_SIZE = 1920; // 2048
	static const int MAX_BUFFER_COUNT = 8;

	std::vector<uint8_t> _buffers[MAX_BUFFER_COUNT];
	int _lastPlayed;  // 0 ... MAX_BUFFER_COUNT - 1
	int _lastWritten; // 0 ... STREAMING_BUFFER_SIZE * MAX_BUFFER_COUNT - 2 (inc by 2)

#ifndef _NOFILEAPI
	FILE* _fp;
#endif // #ifndef _NOFILEAPI
};
