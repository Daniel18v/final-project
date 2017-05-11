#pragma once

#include "emulator.h"
#include <vector>

namespace Emulator
{
	class Cpu;
	class CTimer;

	typedef std::function<uint()> IrqCallback;

	class IDeviceInputLine
	{
	public:
		virtual void EmptyEventQueue(/*INT64 param*/) = 0;
		virtual LineState get_State() const = 0;
		virtual void SetState(LineState state, uint vector = USE_STORED_VECTOR) = 0;
		virtual uint DefaultIrqCallback() = 0;

        static const uint USE_STORED_VECTOR = 0xff000000;
	};

    class _DeviceInput : public IDeviceInputLine
    {
        static const size_t MaxQueueLength = 32;
        static const int DefaultIrqVector = 0x4001;

	protected:
		struct Event {
			Event() : state(LineState::Clear), vector(DefaultIrqVector) {}
			Event(LineState state, uint vector) : state(state), vector(vector) {}

			LineState state;
			uint vector;
		};

	public:
		typedef std::function<void(LineState state)> StatusChangedCallback;

        _DeviceInput(Cpu& cpu, StatusChangedCallback callback, uint storedVector);
		virtual ~_DeviceInput();

		// IDeviceInputLine
		virtual void EmptyEventQueue(/*INT64 param*/);
		virtual LineState get_State() const {
			return curstate;
		}
		virtual uint DefaultIrqCallback();

		virtual void HandleEvent(Event inputEvent) = 0;

		void onStateChanged(LineState state);

	protected:
		void PostState(LineState state, uint vector);

		Cpu& cpu;
        LineState curstate;	// most recently processed state
		uint curvector;		// most recently processed vector

	private:
		StatusChangedCallback callback;
		uint storedVector;	// most recently written vector
		std::vector<Event> eventQueue; // queue of pending events
		CTimer* emptyEventQueueTimer;
	};

	class IrqDeviceInputLine : public _DeviceInput
	{
	public:
        IrqDeviceInputLine(Cpu& cpu, StatusChangedCallback callback, uint storedVector);
		virtual void SetState(LineState state, uint vector = USE_STORED_VECTOR);
		virtual void HandleEvent(_DeviceInput::Event inputEvent);
		uint OnIrq();
	};

	class NmiDeviceInputLine : public _DeviceInput
	{
	public:
        NmiDeviceInputLine(Cpu& cpu, StatusChangedCallback callback, uint storedVector);
		virtual void SetState(LineState state, uint vector = USE_STORED_VECTOR);
		virtual void HandleEvent(_DeviceInput::Event inputEvent);
	};

	class HaltDeviceInputLine : public _DeviceInput
	{
	public:
        HaltDeviceInputLine(Cpu& cpu, StatusChangedCallback callback, uint storedVector);
		virtual void SetState(LineState state, uint vector = USE_STORED_VECTOR);
		virtual void HandleEvent(_DeviceInput::Event inputEvent);
	};

	class ResetDeviceInputLine : public _DeviceInput
	{
	public:
        ResetDeviceInputLine(Cpu& cpu, StatusChangedCallback callback, uint storedVector);
		virtual void SetState(LineState state, uint vector = USE_STORED_VECTOR);
		virtual void HandleEvent(_DeviceInput::Event inputEvent);
	};

} // namespace Emulator
