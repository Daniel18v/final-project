#include "types.h"
#include "audio.h"
#include "galagamachine.h"

namespace Galaga {

	Audio::Audio(GalagaMachine& galagaMachine) :
		_galagaMachine(galagaMachine),
		_buffer()
	{
		_timer = new Emulator::CTimer("UpdateSound", galagaMachine.scheduler,
			[this](INT64, INT64 currentTime)
			{
				update(currentTime);
			});
		_timer->Start(/*21334*/20000, true); // 50HZ = 20ms
	}

	Audio::~Audio()
	{
		delete _timer;
	}

	void Audio::update(INT64 currentTime)
	{
		auto& namcoSamples = _galagaMachine.namcoSound->update(currentTime);
		auto& discreteSamples = _galagaMachine.discreteAudio->update(currentTime);

		size_t size = std::max(namcoSamples.size() / 4, discreteSamples.size());

		//ASSERT(namcoSamples.size() == 4 * discreteSamples.size());
		if (discreteSamples.size() < size) {
			discreteSamples.resize(size);
		}
		if (namcoSamples.size() < size * 4) {
			namcoSamples.resize(size * 4);
		}

//#ifndef WIN32
//		size = 1024;
//#endif // #ifndef WIN32

		std::vector<int> samples;
		samples.resize(size);
		for (size_t i = 0; i < namcoSamples.size(); i++)
		{
			samples[i / 4] += namcoSamples[i];
		}

		for (size_t i = 0; i < size; i++)
		{
			samples[i] = (samples[i] + 7) / 8 + (((discreteSamples[i] * 230) / 256) + 1) / 2;
		}

		_buffer.insert(_buffer.end(), samples.begin(), samples.end());
		
		namcoSamples.clear();
		discreteSamples.clear();
	}
}