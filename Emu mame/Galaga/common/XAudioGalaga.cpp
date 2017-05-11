#include "..\common\xaudiogalaga.h"

XAudioGalaga::XAudioGalaga() :
	_pMusicEngine(),
	_pSourceVoice(nullptr),
	_voiceContext(*this),
	_pMasteringVoice(nullptr),
	_buffers(),
	_lastPlayed(0),
	_lastWritten(0)
{
#ifndef _NOFILEAPI
	//_fp = nullptr;
	//char buff[MAX_PATH];
	//sprintf_s(buff, MAX_PATH, "c:\\temp\\xaudio2.txt");
	//fopen_s(&_fp, buff, "w");
#endif // #ifndef _NOFILEAPI

	// Create XAudio2 and a single mastering voice on the default audio device
	HRESULT hr = ::XAudio2Create(&_pMusicEngine, 0, XAUDIO2_DEFAULT_PROCESSOR);
	if (SUCCEEDED(hr))
	{
		hr = _pMusicEngine->CreateMasteringVoice(&_pMasteringVoice, XAUDIO2_DEFAULT_CHANNELS);
	}

	if (SUCCEEDED(hr))
	{
		WAVEFORMATEX wfx;
		ZeroMemory(&wfx, sizeof(WAVEFORMATEX));
		wfx.cbSize = 0;
		wfx.wFormatTag = WAVE_FORMAT_PCM;
		wfx.nChannels = 1;
		wfx.nSamplesPerSec = 48000;
		wfx.wBitsPerSample = 16;
		wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.wBitsPerSample / 8;
		wfx.nBlockAlign = wfx.wBitsPerSample / 8;

		hr = _pMusicEngine->CreateSourceVoice(&_pSourceVoice, (WAVEFORMATEX*)&wfx,
			0, 1.0f, &_voiceContext, NULL, NULL);
		if (SUCCEEDED(hr)) {
			hr = _pSourceVoice->Start(0, 0);
		}
	}

	// Create an array of read buffers.
	for (size_t j = 0; j < MAX_BUFFER_COUNT; ++j)
	{
		_buffers[j].resize(STREAMING_BUFFER_SIZE);
	}
	_lastPlayed = 2;//MAX_BUFFER_COUNT - 1;
	_lastWritten = MAX_BUFFER_COUNT * STREAMING_BUFFER_SIZE - 2;

	next();
}

XAudioGalaga::~XAudioGalaga()
{
#ifndef _NOFILEAPI
	//if (_fp != nullptr) {
	//	fclose(_fp);
	//}
#endif // #ifndef _NOFILEAPI

	if (_pMasteringVoice != nullptr) {
		_pMasteringVoice->DestroyVoice();
	}

	if (_pSourceVoice != nullptr) {
		_pSourceVoice->DestroyVoice();
	}
}

void XAudioGalaga::RenderAudio(const std::vector<int>& samples)
{
#ifndef _NOFILEAPI
	//if (_fp != nullptr) {
	//	fprintf(_fp, "[%08x] { RenderAudio %d samples where _lastWritten was %d\n", ::GetCurrentThreadId(), samples.size(), _lastWritten);
	//}
#endif // #ifndef _NOFILEAPI

	for (size_t i = 0; i < samples.size(); i++)
	{
		int sample = samples[i];
		INT16 v;
		if (sample < -32768)
			v = -32768;
		else if (sample > 32767)
			v = 32767;
		else
			v = sample;
		UINT16 uv = (UINT16)(v);

		int next = (2 + _lastWritten) % (MAX_BUFFER_COUNT * STREAMING_BUFFER_SIZE);
		int iBuffer = next / STREAMING_BUFFER_SIZE;
		int iOffset = next % STREAMING_BUFFER_SIZE;
		if (iBuffer == _lastPlayed) {
			// overflow
			return;
		}

		_buffers[iBuffer][iOffset] = uv & 0xff;
		_buffers[iBuffer][iOffset + 1] = (uv >> 8) & 0xff;
		_lastWritten = next;
	}
}

void XAudioGalaga::next()
{
#ifndef _NOFILEAPI
	//if (_fp != nullptr) {
	//	fprintf(_fp, "[%08x] { Submit next buffer where _lastPlayed is %d\n", ::GetCurrentThreadId(), _lastPlayed);
	//}
#endif // #ifndef _NOFILEAPI

	_lastPlayed = (_lastPlayed + 1) % MAX_BUFFER_COUNT;

	XAUDIO2_BUFFER buf = { 0 };
	buf.AudioBytes = _buffers[_lastPlayed].size();
	buf.pAudioData = _buffers[_lastPlayed].data();
	HRESULT hr = _pSourceVoice->SubmitSourceBuffer(&buf);

#ifndef _NOFILEAPI
	//if (_fp != nullptr) {
	//	fprintf(_fp, "[%08x] } Next buffer submitted; _lastPlayed:%d hr:%08x\n", ::GetCurrentThreadId(), _lastPlayed, hr);
	//}
#endif // #ifndef _NOFILEAPI
}
