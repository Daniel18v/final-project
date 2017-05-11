#include "types.h"
#include "galagamachine.h"
#include "../emulator/deviceinput.h"
#include "../emulator/timer.h"
#include "cpu2.h"
#include "cpu3.h"
#include "maincpu.h"
#include "namco06.h"
#include "audio.h"
#include "frame.h"
#include "galagainputports.h"
#include "galagaroms.h"
#include <algorithm>

namespace Galaga {

	// static
	readonly double GalagaMachine::ClockFrequency = 3.072 * 1e6; // 3.072 MHz
	readonly uint GalagaMachine::AudioSampleRate = 48000; // 48 KHz
	GalagaRoms s_galagaRoms;

	GalagaMachine::GalagaMachine() :
		scheduler(Quantum),
		screenDevice(null),
		romManager(s_galagaRoms),
		frame(ScreenDevice::Width, ScreenDevice::Height),
#pragma warning (disable: 4355) // 'this' : used in base member initializer list
		ioHandler(*this),
#pragma warning (default: 4355)
        vblankCallbacks(),
		customMod(0),
		machinePorts(),
		_frame(ScreenDevice::Width, ScreenDevice::Height), // js-only
		_audioFrame() // js-only
	{
		ram = new byte[65536];
		memset(ram, 0, 65536);

        twoPlayersGame = true;
        difficulty = Difficulty::Easy;
        demoSound = true;
        freeze = false;
        rackTest = false;
        cabinetType = CabinetType::Upright;
        coinage = Coinage::FreePlay;
        initialLives = InitialLives::Three;
        bonusLives = BonusLives::OnetoFour_20K_70K_Every70K__Five_30K_120K_Every120K;

        machinePorts["IN0L"] = in0lPort = new IN0LPort(*this);
        machinePorts["IN0H"] = in0hPort = new IN0HPort(*this);
        machinePorts["IN1L"] = in1lPort = new IN1LPort(*this);
        machinePorts["IN1H"] = in1hPort = new IN1HPort(*this);
        machinePorts["DSWA"] = dswaPort = new DSWAPort(*this);
        machinePorts["DSWB"] = dswbPort = new DSWBPort(*this);

        screenDevice = new ScreenDevice(*this, ram);
        mainCpu = new MainCpu(*this, ram, 64 * 1024, ioHandler);
        cpu2 = new Cpu2(*this, ram, 64 * 1024, ioHandler);
        cpu3 = new Cpu3(*this, ram, 64 * 1024, ioHandler);
		cpu3InterruptCallbackTimer = new Emulator::CTimer("OnCpu3InterruptCallback", scheduler,
			[this](INT64 param, INT64 currentTime) { OnCpu3InterruptCallback(param); });
        namco06xx = new Namco06(*this);
		discreteAudio = new DiscreteAudio(*this, AudioSampleRate);
		namcoSound = new NamcoSound(*this);
		audio = new Audio(*this);

        Reset();
	}

	GalagaMachine::~GalagaMachine()
	{
		machinePorts.clear();
		delete dswbPort;
		delete dswaPort;
		delete in1hPort;
		delete in1lPort;
		delete in0hPort;
		delete in0lPort;

		delete namcoSound;
		delete discreteAudio;
		delete audio;
		delete namco06xx;
		delete cpu3InterruptCallbackTimer;
		delete cpu3;
		delete cpu2;
		delete mainCpu;
		delete screenDevice;
		delete [] ram;
	}

	void GalagaMachine::Draw(Frame& frame)
    {
        frame.Clear();
        screenDevice->DrawStars(frame);
        screenDevice->DrawTiles(frame);
        screenDevice->DrawSprites(frame);
    }

	void GalagaMachine::GetAudioSamples(AudioFrame& audioFrame)
	{
		audioFrame.Set(audio->GetAudio());
	}

	Emulator::IInputPort* GalagaMachine::GetPortByTag(const std::string& tag)
    {
		MachinePorts::iterator it = machinePorts.find(tag);
		if (machinePorts.end() == it) {
			ASSERT(false);
			return null;
		}
		return it->second;
	}

	void GalagaMachine::OnCpu3InterruptCallback(INT64 param)
    {
        int scanline = (int)param;

        cpu3->NmiLinePulse();

        scanline = scanline + 128;
        if (scanline >= 272) {
            scanline = 64;
        }

        cpu3InterruptCallbackTimer->Start(screenDevice->TimeUntilScanline(scanline), false, scanline);
    }

    void GalagaMachine::OnVblankStartCallback()
    {
        std::for_each(std::begin(vblankCallbacks), std::end(vblankCallbacks), [](Action a) {
            a();
        });

        if (mainCpu->get_IsInterruptEnabled())
        {
            mainCpu->OnVblankStart();
        }

        if (cpu2->get_IsInterruptEnabled())
        {
            cpu2->OnVblankStart();
        }
    }

    void GalagaMachine::Out0PortWrite(byte data)
    {
        //set_led_status(1, data & 1);
        //set_led_status(0, data & 2);
        //coin_counter_w(1, ~data & 4);
        //coin_counter_w(0, ~data & 8);
    }

    void GalagaMachine::Out1PortWrite(byte data)
    {
        //coin_lockout_global_w(data & 1);
    }

	byte GalagaMachine::ReadDsw(int offset)
    {
        byte dswa = machinePorts["DSWA"]->Read();
        byte dswb = machinePorts["DSWB"]->Read();

        byte bit0 = (dswb >> offset) & 1;
        byte bit1 = (dswa >> offset) & 1;
        return (bit0 | (bit1 << 1));
    }

    const ByteArray& GalagaMachine::ReadRom(const std::string& rom)
    {
        return romManager.ReadRom(rom);
    }

    void GalagaMachine::RegisterVblankCallback(Action callback)
    {
        vblankCallbacks.push_back(callback);
    }

	void GalagaMachine::Reset()
    {
        ResetLatch();
        cpu3InterruptCallbackTimer->Start(screenDevice->TimeUntilScanline(64), false, 64);
    }

    void GalagaMachine::ResetLatch()
    {
        for (int i = 0; i < 8; i++)
        {
            WriteLatch(i, 0);
        }
    }

    void GalagaMachine::WriteLatch(int offset, byte value)
    {
        int bit = (value & 1);
        switch (offset)
        {
            case 0x00:	// IRQ1
                mainCpu->set_IsInterruptEnabled(bit != 0);
                if (0 == bit) {
                    mainCpu->get_Processor().get_IrqInputLine().SetState(Emulator::LineState::Clear);
                }
                break;

            case 0x01:	// IRQ2
                cpu2->set_IsInterruptEnabled(bit != 0);
                if (0 == bit) {
                    cpu2->get_Processor().get_IrqInputLine().SetState(Emulator::LineState::Clear);
                }
                break;

            case 0x02:	// NMION
                cpu3->set_IsInterruptEnabled(bit == 0);
                break;

            case 0x03:	// RESET
                cpu2->get_Processor().get_ResetInputLine().SetState(
					bit != 0 ? Emulator::LineState::Clear : Emulator::LineState::Assert);
                cpu3->get_Processor().get_ResetInputLine().SetState(
					bit != 0 ? Emulator::LineState::Clear : Emulator::LineState::Assert);
                break;

            case 0x04:	// n.c.
                break;

            case 0x05:	// MOD 0
                customMod = (customMod & ~0x01) | (bit << 0);
                break;

            case 0x06:	// MOD 1
                customMod = (customMod & ~0x02) | (bit << 1);
                break;

            case 0x07:	// MOD 2
                customMod = (customMod & ~0x04) | (bit << 2);
                break;
        }
    }

#ifndef WIN32
	void GalagaMachine::DrawAndRotate()
	{
		Draw(_frame);

		const int BitmapPixelWidth = ScreenDevice::Height;
		const int BitmapPixelHeight = ScreenDevice::Width;

		UINT* srcPixels = _frame.Pixels;
		int i = 0;
		for (int x = 0; x < BitmapPixelHeight; x++)
		{
			int j = (BitmapPixelWidth - 1) * frame.Width + x;
			for (int y = BitmapPixelWidth - 1; y >= 0; y--)
			{
				_screenBuffer[i++] = (byte)((srcPixels[j] & 0xff0000) >> 16);
				_screenBuffer[i++] = (byte)((srcPixels[j] & 0xff00) >> 8);
				_screenBuffer[i++] = (byte)(srcPixels[j] & 0xff);
				_screenBuffer[i++] = 255;// (byte)((srcPixels[j] & 0xff000000) >> 24);
				j -= frame.Width;
			}
		}
	}

	void GalagaMachine::Run(double ms, emscripten::val onComplete)
	{
		//printf("run for: %f ms\n", ms);

		double us = ms * 1000.0;
		long currentTime = scheduler.get_CurrentTime();
		while (scheduler.get_CurrentTime() < (currentTime + us))
		{
			scheduler.Timeslice();
		}

		DrawAndRotate();
		
		_audioFrame.Set(audio->GetAudio());

		//printf("update: %ld\n", scheduler.get_CurrentTime());

		onComplete(
			emscripten::typed_memory_view(ScreenDevice::Height * ScreenDevice::Width * 4, _screenBuffer),
			emscripten::typed_memory_view(_audioFrame.Size, _audioFrame.Samples));
	}

#else

	void GalagaMachine::Run(double us, Frame& videoFrame, AudioFrame& audioFrame)
	{
		us = MIN(us, 75000); // no more than 75ms

		long currentTime = scheduler.get_CurrentTime();
		while (scheduler.get_CurrentTime() < (currentTime + us))
		{
			scheduler.Timeslice();
		}

		// video
		Draw(videoFrame);

		// audio
		//audio->update(scheduler.get_CurrentTime());
		GetAudioSamples(audioFrame);
	}
#endif // #ifndef WIN32

	void GalagaMachine::set_InsertCoin(bool value)
	{
		in0hPort->InsertCoin = value;
	}
    void GalagaMachine::set_Start1Player(bool value)
	{
		in0lPort->Start1Player = value;
	}
    void GalagaMachine::set_Start2Player(bool value)
	{
		in0lPort->Start2Player = value;
	}
    void GalagaMachine::set_MoveLeft(bool value)
	{
		in1lPort->MoveLeft = value; 
	}
    void GalagaMachine::set_MoveRight(bool value)
	{
		in1lPort->MoveRight = value;
	}
    void GalagaMachine::set_Button1(bool value)
	{
		in0lPort->Button1 = value; 
	}

} // Galaga