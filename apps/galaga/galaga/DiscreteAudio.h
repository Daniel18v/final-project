#pragma once

#include "../emulator/timer.h"
#include "../emulator/discretesoundnode.h"
#include "../emulator/audioinput.h"
#include "../emulator/ladderdac.h"
#include "../emulator/opampfilter.h"
#include "../emulator/mixer.h"

namespace Galaga {

	// forward declarations:
	class GalagaMachine;

	class DiscreteAudio : public DiscreteDevice
	{
	public:
		DiscreteAudio(GalagaMachine& galagaMachine, uint sampleRate);
		virtual ~DiscreteAudio();

		void updateToCurrentTime() override;

		void process(UINT samplesPerUpdate, std::vector<int>& samples);

		Emulator::AudioInput& getAudioInput(int index)
		{
			assert (index < 3);
			switch (index)
			{
			case 0:
				return _input1;
			case 1:
				return _input2;
			case 2:
			default:
				return _input3;
			}
		}

		std::vector<int>& update(INT64 currentTime);

	private:
		void initAudioGraph();
		int updateToTime(INT64 currentTime);
		std::vector<int> generateSamples(int samples);

		GalagaMachine& _galagaMachine;
		Emulator::Mixer _mixer;
		Emulator::OpAmpFilter _opAmpFilter1;
		Emulator::OpAmpFilter _opAmpFilter2;
		Emulator::OpAmpFilter _opAmpFilter3;
		Emulator::LadderDAC _dac1;
		Emulator::LadderDAC _dac2;
		Emulator::LadderDAC _dac3;
		Emulator::AudioInput _input1;
		Emulator::AudioInput _input2;
		Emulator::AudioInput _input3;

		std::vector<int> _buffer;
		INT64 _lastSampleIndex;

		static const int SampleRate = 48000;
	};

} // namespace Galaga