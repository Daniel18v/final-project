#pragma once

#ifndef WIN32
#	include <emscripten/bind.h>
#endif // #ifndef WIN32
#include "types.h"

#include "../emulator/emulator.h"
#include "../emulator/timer.h"
#include "audio.h"
#include "discreteaudio.h"
#include "screendevice.h"
#include "frame.h"
#include "galagaiohandler.h"
#include "namcosound.h"
#include <string>
#include <map>
#include <functional>
#include <vector>

namespace Galaga {

	class Cpu2;
	class Cpu3;
	class MainCpu;
	class Namco06;
	class ScreenDevice;
	class IN0LPort;
    class IN0HPort;
    class IN1LPort;
    class IN1HPort;
    class DSWAPort;
    class DSWBPort;

	class GalagaMachine
	{
	public:
		static readonly double ClockFrequency;
		static readonly uint AudioSampleRate;

        enum CabinetType
        {
            Upright,
            Cocktail
        };

	private:
		static readonly uint Quantum = 167; // us

        enum Difficulty
        {
            Medium = 0,
            Hard,
            Hardest,
            Easy,
        };

        enum Coinage
        {
            FreePlay = 0,
            TwoCoinsThreeCredits,
            ThreeCoinsOneCredit,
            OneCoinTwoCredits,
            FourCoinsOneCredit,
            OneCoinThreeCredits,
            TwoCoinsOneCredit,
            OneCoinOneCredit
        };

        enum InitialLives
        {
            Two = 0,
            Four,
            Three,
            Five
        };

        enum BonusLives
        {
            None = 0,
            OnetoFour_30K_100K_Every100K__Five_30K_and_100K_only,
            OnetoFour_20K_70K_Every70K__Five_30K_120K_Every120K,
            OnetoFour_20K_and_60K_only__Five_30K_and_150K_only,
            OnetoFour_20K_60K_Every60K__Five_20K_100K_Every100K,
            OnetoFour_30K_120K_Every120K__Five_30K_and_120K_only,
            OnetoFour_20K_80K_Every80K__Five_30K_150K_Every150K,
            OnetoFour_30K_and_80K_only__Five_30K_only,
        };

	public:
		GalagaMachine();
		virtual ~GalagaMachine();

		const ByteArray& ReadRom(const std::string& rom);
		Emulator::IInputPort* GetPortByTag(const std::string& tag);

		void Out0PortWrite(byte data);
		void Out1PortWrite(byte data);

		byte ReadDsw(int offset);
		void WriteLatch(int offset, byte value);

		void OnVblankStartCallback();

		std::vector<int> GetAudio() {
			return audio->GetAudio();
		}

#ifdef WIN32
		void Run(double us, Frame& videoFrame, AudioFrame& audioFrame);
#else
		void Run(double ms, emscripten::val onComplete);
#endif // #ifndef WIN32
		void set_InsertCoin(bool value);
        void set_Start1Player(bool value);
        void set_Start2Player(bool value);
        void set_MoveLeft(bool value);
        void set_MoveRight(bool value);
        void set_Button1(bool value);

		Emulator::AudioInput& getAudioInput(int index) {
			return discreteAudio->getAudioInput(index);
		}
		NamcoSound& getNamcoSound() {
			return *namcoSound;
		}
		long get_CurrentTime() {
			return scheduler.get_CurrentTime();
		}

	private:
		void OnCpu3InterruptCallback(INT64 param);
		void RegisterVblankCallback(Action callback);
		void Reset();
		void ResetLatch();

		void Draw(Frame& frame);
		void GetAudioSamples(AudioFrame& audioFrame);

#ifndef WIN32
		void DrawAndRotate();
#endif // #ifndef WIN32

	public:
        Emulator::Scheduler scheduler;
        Namco06* namco06xx;
        ScreenDevice* screenDevice;
		DiscreteAudio* discreteAudio;
		NamcoSound* namcoSound;
	private:
		Emulator::IRomManager& romManager;
        Frame frame;
        byte* ram;
        GalagaIOHandler ioHandler;
        MainCpu* mainCpu;
        Cpu2* cpu2;
        Cpu3* cpu3;
        Emulator::CTimer* cpu3InterruptCallbackTimer;
		Audio* audio;
        std::vector<Action> vblankCallbacks;
        int customMod;
		friend class Namco06;
#ifndef WIN32
		byte _screenBuffer[ScreenDevice::Height * ScreenDevice::Width * 4];
#endif // #ifndef WIN32

		// ports
		typedef std::map<std::string, Emulator::IInputPort*> MachinePorts;
        MachinePorts machinePorts;
        IN0LPort* in0lPort;
        IN0HPort* in0hPort;
        IN1LPort* in1lPort;
        IN1HPort* in1hPort;
        DSWAPort* dswaPort;
        DSWBPort* dswbPort;

		// js-only
		Frame _frame;
		AudioFrame _audioFrame;

	public:
		// settings
        bool twoPlayersGame;
        Difficulty difficulty;
        bool demoSound;
        bool freeze;
        bool rackTest;
        CabinetType cabinetType;
        Coinage coinage;
        InitialLives initialLives;
        BonusLives bonusLives;
	};

} // Galaga
