#pragma once
#include <vector>
#include "../emulator/emulator.h"
#include "../emulator/discretesoundnode.h"
#include "../emulator/timer.h"

/* 8 voices max */
#define MAX_VOICES 8

#define MAX_VOLUME 16

/* a position of waveform sample */
#define WAVEFORM_POSITION(n)	(((n) >> chip->f_fracbits) & 0x1f)

namespace Galaga {

class GalagaMachine;

class NamcoSound
{
	/* this structure defines the parameters for a channel */
	struct sound_channel
	{
		UINT32 frequency;
		UINT32 counter;
		INT32 volume;
		INT32 waveform_select;
	};

	struct namco_sound
	{
		/* data about the sound system */
		sound_channel channel_list[MAX_VOICES];
		sound_channel *last_channel;
		std::vector<UINT8> soundregs;
		const UINT8* wavedata;
		std::vector<INT16> waveforms;

		/* global sound parameters */
		INT32 num_voices;
		INT32 sound_enable;
		int namco_clock;
		int f_fracbits;

		/* decoded waveform table */
		INT16 *waveform[MAX_VOLUME];
	};

public:
	NamcoSound(GalagaMachine& galagaMachine);

	std::vector<int>& update(INT64 currentTime);

	void WriteMemory(ushort offset, byte data);
	std::vector<int> UpdateMono(int samples);

private:
	static void BuildDecodedWaveform(namco_sound* chip, const UINT8* rgnbase);
	static void UpdateNamcoWaveform(namco_sound* chip, int offset, UINT8 data);

	void updateToCurrentTime();
	int updateToTime(INT64 time);

	/* generate sound by oversampling */
	inline UINT32 UpdateOne(namco_sound* chip, stream_sample_t* buffer, int length, const INT16* wave, UINT32 counter, UINT32 freq)
	{
		while (length-- > 0)
		{
			*buffer++ += wave[WAVEFORM_POSITION(counter)];
			counter += freq;
		}

		return counter;
	}

private:
	static const int SampleRate = 192000;

	GalagaMachine& galagaMachine;
	static const uint romLength = 512;
	byte rom[romLength];
	namco_sound _namcoSound;

	std::vector<int> _buffer;
	INT64 _lastSampleIndex;
};

} // Galaga
