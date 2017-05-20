#pragma once

#include <functional>
#include <vector>

// defines
#define null NULL
#define readonly const

// typedefs
typedef unsigned char byte;
typedef char sbyte;
typedef unsigned short ushort;
typedef unsigned int uint;

typedef std::function<void()> Action;
typedef std::vector<byte> ByteArray;

namespace Emulator
{
	class IDeviceInputLine;

	enum LineState {
        Clear, 			// clear (a fired or held) line
        Assert, 		// assert an interrupt immediately
        Hold, 			// hold interrupt line until acknowledged
        Pulse			// pulse interrupt line instantaneously (only for NMI, RESET)
    };

    class IOHandler
    {
	public:
        virtual void Write(int port, byte value) = 0;
        virtual byte Read(int port) = 0;
    };

	class IMemoryHandler
	{
	public:
        virtual bool WriteMemory(ushort address, byte value) = 0;
        virtual bool ReadMemory(ushort address, byte& value) = 0;
	};

    class IDevice
    {
	public:
		virtual void Reset() = 0;
		virtual INT64 get_ExecutionTimeInCurrentTimeslice() const = 0;
        virtual INT64 Execute(INT64 untilTime) = 0; // us
		virtual void stopTimeslice() = 0;
    };

    class IInputPort
    {
	public:
		virtual ~IInputPort() {}
        virtual byte Read() = 0;
    };

    class IProcessor
    {
	public:
		virtual ~IProcessor() {}
        virtual void Reset() = 0;
		virtual INT64 get_ExecutionTimeInCurrentTimeslice() const = 0;
        virtual INT64 Execute(INT64 us) = 0;
        virtual void AbortTimeslice() = 0;
		virtual IDeviceInputLine& get_IrqInputLine() = 0;
		virtual IDeviceInputLine& get_ResetInputLine() = 0;
		virtual IDeviceInputLine& get_NmiInputLine() = 0;
		virtual std::vector<IDeviceInputLine*> get_InputLines() = 0;
    };

    // suspension reasons for executing devices
    enum SuspendReason
    {
        SUSPEND_REASON_HALT = 0x0001,	// HALT line set (or equivalent)
        SUSPEND_REASON_RESET = 0x0002,	// RESET line set (or equivalent)
        SUSPEND_REASON_SPIN = 0x0004,	// currently spinning
        SUSPEND_REASON_TRIGGER = 0x0008,	// waiting for a trigger
        SUSPEND_REASON_DISABLE = 0x0010,	// disabled (due to disable flag)
        SUSPEND_REASON_TIMESLICE = 0x0020,	// waiting for the next timeslice
        SUSPEND_ANY_REASON = 0xffff		// all of the above
    };

    class IRomManager
    {
	public:
        virtual const ByteArray& ReadRom(const std::string& rom) = 0;
    };

} // Emulator
