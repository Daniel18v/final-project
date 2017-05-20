#pragma once

#include "../emulator/emulator.h"
#include "../emulator/deviceinput.h"
#include <exception>
#include <string>
#include <vector>

namespace Z80 {

#define A reg[7]
#define B reg[0]
#define C reg[1]
#define D reg[2]
#define E reg[3]
#define F reg[6]
#define H reg[4]
#define L reg[5]
#define c get_Carry()
#define s get_SignFlag()
#define z get_ZeroFlag()
#define h get_HFlag()
#define n get_NFlag()
#define p get_PVFlag()
#define v get_PVFlag()
#define BC get_BC()
#define DE get_DE()
#define HL get_HL()
#define HX get_HX()
#define HY get_HY()
#define LX get_LX()
#define LY get_LY()
#define Get_SZHVC_add_flag(operand, result, carry) (flag_SZHVC_add[operand][result][carry])
#define Get_SZHVC_sub_flag(operand, result, carry) (flag_SZHVC_sub[operand][result][carry])

	class Z80ProcessorImpl;

	enum InterruptMode
    {
        Mode0,
        Mode1,
        Mode2
    };

	class Z80ProcessorImpl  : public Emulator::IProcessor
	{
		typedef void (/*__thiscall*/ Z80ProcessorImpl::*Action)();

		struct Instruction
		{
			enum Prefix
			{
				None,
				CB,
				DD,
				ED,
				FD,
				DDCB,
				FDCB
			};

			enum Register
			{
				B_reg_index,
				C_reg_index,
				D_reg_index,
				E_reg_index,
				H_reg_index,
				L_reg_index,
				F_reg_index,
				A_reg_index
			};

			Prefix prefix;
			ushort opcode;
			int length;
			Register _register;
			byte displacement;
			byte immediate;
			ushort immediateExtended;
			Action execute;
			int cycles;
		};

        readonly static byte B_index = 0;
        readonly static byte C_index = 1;
        readonly static byte D_index = 2;
        readonly static byte E_index = 3;
        readonly static byte H_index = 4;
        readonly static byte L_index = 5;
        readonly static byte F_index = 6;
        readonly static byte A_index = 7;

        readonly static byte S_mask = 0x80;
        readonly static byte Z_mask = 0x40;
        readonly static byte H_mask = 0x10;
        readonly static byte PV_mask = 0x04;
        readonly static byte N_mask = 0x02;
        readonly static byte C_mask = 0x01;

        enum DecodeAction
        {
            None,
            Immediate,
            ImmediateExtended,
            Displacement,
            PrefixedDisplacement,
            DisplacementImmediate,
            DisplacementRegistry,
            Register
        };

        struct OpcodeInfo
        {
            OpcodeInfo(DecodeAction decodeAction, Action interpreterFunc)
            {
                this->decodeAction = decodeAction;
                this->interpreterFunc = interpreterFunc;
            }

            DecodeAction decodeAction;
            Action interpreterFunc;
        };

        static readonly byte cc_op[];
        static readonly byte cc_cb[];
        static readonly byte cc_ed[];
        static readonly byte cc_xy[];
        static readonly byte cc_xycb[];

        // extra cycles if jr/jp/call taken and 'interrupt latency' on rst 0-7
        static readonly byte cc_ex[];
        static readonly bool parity[];

        static byte flag_SZHVC_add[256][256][2];
        static byte flag_SZHVC_sub[256][256][2];

        static byte flag_SZ[256];
        static byte flag_SZP[256];
        static byte flag_SZ_BIT[256];
        static byte flag_SZHV_dec[256];
        static byte flag_SZHV_inc[256];

        static readonly byte rstMask[];

	public:
		Z80ProcessorImpl(double clockFrequency, byte ram[], Emulator::IOHandler& ioHandler,
			const std::string tag, Emulator::IMemoryHandler* memoryHandler, Emulator::Cpu& cpu);
		~Z80ProcessorImpl();

		// Emulator::IProcessor
		virtual void Reset();
		virtual INT64 get_ExecutionTimeInCurrentTimeslice() const;
		virtual INT64 Execute(INT64 us);
		virtual void TakeInterrupt();
		virtual void AbortTimeslice();
		virtual Emulator::IDeviceInputLine& get_IrqInputLine() {
			return irqLine;
		}
		virtual Emulator::IDeviceInputLine& get_ResetInputLine() {
			return resetLine;
		}
		virtual Emulator::IDeviceInputLine& get_NmiInputLine() {
			return nmiLine;
		}
		virtual std::vector<Emulator::IDeviceInputLine*> get_InputLines();
		virtual uint get_DefaultIrqVector() const {
			return 0x000000ff;
		}

	private:
		byte ReadMemory(ushort addr) const {
            byte value;
            if (null == memoryHandler || !memoryHandler->ReadMemory(addr, value))
            {
                value = ram[addr];
            }
            return value;
		} 
		ushort ReadMemory16(ushort addr) const {
            // code review: speed up!
            byte low = ReadMemory(addr);
            byte hi = ReadMemory((ushort)(addr + 1));
            return (ushort)((hi << 8) | low);
		} 
		void WriteMemory(ushort addr, byte value) {
            if (null == memoryHandler || !memoryHandler->WriteMemory(addr, value))
            {
                ram[addr] = value;
            }
		} 
		void WriteMemory16(ushort addr, ushort value) {
            // code review: speed up!
            byte low = (byte)(value & 0xff);
            byte hi = (byte)(value >> 8);
            WriteMemory(addr, low);
            WriteMemory((ushort)(addr + 1), hi);
		} 

        void SetAdcOpFlags(ushort op1, ushort op2, int result)
        {
            this->F = (byte)(
                ((result >> 16) & C_mask) |
                ((result >> 8) & S_mask) |
                (((result ^ op1 ^ op2) >> 8) & H_mask) |
                (((result & 0xffff) == 0) ? Z_mask : 0) |
                (((op2 ^ op1 ^ 0x8000) & (op2 ^ result) & 0x8000) >> 13)
            );
        } 

        void SetAddOpFlags(ushort op1, ushort op2, int result)
        {
            this->F = (byte)(
                (this->F & (S_mask | Z_mask | PV_mask)) |
                ((result >> 16) & C_mask) |
                (((result ^ op1 ^ op2) >> 8) & H_mask)
            );
        } 

        void SetSbcOpFlags(ushort op1, ushort op2, int result)
        {
            this->F = (byte)(
                N_mask |
                ((result >> 16) & C_mask) |
                ((0 == (result & 0xffff)) ? Z_mask : 0) |
                (((op1 ^ result ^ op2) >> 8) & H_mask) |
                ((((op2 ^ op1) & (op1 ^ result)) & 0x8000) >> 13)
            );
        }

		Instruction FetchDecode(ushort pc);

		void OnNmiLineChanged(Emulator::LineState state) {
            // mark an NMI pending
            if (this->nmiState == Emulator::LineState::Clear && state != Emulator::LineState::Clear) {
				this->isNmiPending = true;
            }
			this->nmiState = state;
		}

		byte get_A() const {
			return reg[7];
		}
		void set_A(byte value) {
			reg[7] = value;
		}
		byte get_B() const {
			return reg[0];
		}
		void set_B(byte value) {
			reg[0] = value;
		}
		byte get_C() const {
			return reg[1];
		}
		void set_C(byte value) {
			reg[1] = value;
		}
		byte get_D() const {
			return reg[2];
		}
		void set_D(byte value) {
			reg[2] = value;
		}
		byte get_E() const {
			return reg[3];
		}
		void set_E(byte value) {
			reg[3] = value;
		}
		byte get_H() const {
			return reg[4];
		}
		void set_H(byte value) {
			reg[4] = value;
		}
		byte get_L() const {
			return reg[5];
		}
		void set_L(byte value) {
			reg[5] = value;
		}
		ushort get_BC() const {
			return ((reg[0] << 8) | reg[1]);
		}
		void set_BC(ushort value) {
			reg[0] = (value >> 8) & 0xff;
			reg[1] = (value & 0xff);
		}
		ushort get_DE() const {
			return ((reg[2] << 8) | reg[3]);
		}
		void set_DE(ushort value) {
			reg[2] = (value >> 8) & 0xff;
			reg[3] = (value & 0xff);
		}
		ushort get_HL() const {
			return ((reg[4] << 8) | reg[5]);
		}
		void set_HL(ushort value) {
			reg[4] = (value >> 8) & 0xff;
			reg[5] = (value & 0xff);
		}
		void set_LX(byte value) {
			IX = (value & 0xff) | (IX & 0xff00);
		}
		void set_HX(byte value) {
			IX = ((value << 8) & 0xff00) | (IX & 0xff);
		}
		void set_LY(byte value) {
			IY = (value & 0xff) | (IY & 0xff00);
		}
		void set_HY(byte value) {
			IY = ((value << 8) & 0xff00) | (IY & 0xff);
		}
		bool get_ZeroFlag() const {
			return 0 != (reg[6] & 0x40);
		};
		void set_ZeroFlag(bool value) {
			if (value) {
				reg[6] |= 0x40;
			}
			else {
				reg[6] &= ~0x40;
			}
		};
		byte get_Carry() const {
			return 0 != (reg[6] & 0x01);
		};
		void set_Carry(bool value) {
			if (value) {
				reg[6] |= 0x01;
			}
			else {
				reg[6] &= ~0x01;
			}
		};
		bool get_SignFlag() const {
			return 0 != (reg[6] & 0x80);
		};
		void set_SignFlag(bool value) {
			if (value) {
				reg[6] |= 0x80;
			}
			else {
				reg[6] &= ~0x80;
			}
		};
		bool get_HFlag() const {
			return 0 != (reg[6] & 0x10);
		};
		void set_HFlag(bool value) {
			if (value) {
				reg[6] |= 0x10;
			}
			else {
				reg[6] &= ~0x10;
			}
		};
		bool get_NFlag() const {
			return 0 != (reg[6] & 0x02);
		};
		void set_NFlag(bool value) {
			if (value) {
				reg[6] |= 0x02;
			}
			else {
				reg[6] &= ~0x02;
			}
		};
		bool get_PVFlag() const {
			return 0 != (reg[6] & 0x04);
		};
		void set_PVFlag(bool value) {
			if (value) {
				reg[6] |= 0x04;
			}
			else {
				reg[6] &= ~0x04;
			}
		};
		byte get_HX() const {
			return (IX >> 8);
		}
		byte get_LX() const {
			return (IX & 0xff);
		}
		byte get_HY() const {
			return (IY >> 8);
		}
		byte get_LY() const {
			return (IY & 0xff);
		}

	private:
        void Noni()
        {
            // do nothing
            //throw new std::exception("invalid z80 instruction");
			return;
        }

        void Adc_a_r()
        {
            byte d = this->reg[(int)this->instr._register];
            byte result = (byte)((this->A + this->c + d) & 0xff);
            this->F = Get_SZHVC_add_flag(this->A, result, this->c);
            this->A = result;
        }

        void Adc_a_n()
        {
            byte result = (byte)((this->A + this->c + this->instr.immediate) & 0xff);
            this->F = Get_SZHVC_add_flag(this->A, result, this->c);
            this->A = result;
        }

        void Adc_a_hx()
        {
            byte result = (byte)((this->A + this->c + this->HX) & 0xff);
            this->F = Get_SZHVC_add_flag(this->A, result, this->c);
            this->A = result;
        }

        void Adc_a_lx()
        {
            byte result = (byte)((this->A + this->c + this->LX) & 0xff);
            this->F = Get_SZHVC_add_flag(this->A, result, this->c);
            this->A = result;
        }

        void Adc_a_hy()
        {
            byte result = (byte)((this->A + this->c + this->HY) & 0xff);
            this->F = Get_SZHVC_add_flag(this->A, result, this->c);
            this->A = result;
        }

        void Adc_a_ly()
        {
            byte result = (byte)((this->A + this->c + this->LY) & 0xff);
            this->F = Get_SZHVC_add_flag(this->A, result, this->c);
            this->A = result;
        }

        void Adc_a_hl_ind()
        {
            byte val = ReadMemory(this->HL);
            byte result = (byte)((this->A + this->c + val) & 0xff);
            this->F = Get_SZHVC_add_flag(this->A, result, this->c);
            this->A = result;
        }

        void Adc_a_ix_d_ind()
        {
            ushort addr = (ushort)(this->IX + this->instr.displacement);
            byte val = ReadMemory(addr);
            byte result = (byte)((this->A + this->c + val) & 0xff);
            this->F = Get_SZHVC_add_flag(this->A, result, this->c);
            this->A = result;
        }

        void Adc_a_iy_d_ind()
        {
            ushort addr = (ushort)(this->IY + this->instr.displacement);
            byte val = ReadMemory(addr);
            byte result = (byte)((this->A + this->c + val) & 0xff);
            this->F = Get_SZHVC_add_flag(this->A, result, this->c);
            this->A = result;
        }

        void Adc_hl_bc()
        {
            int result = this->HL + this->c + this->BC;
            SetAdcOpFlags(this->HL, this->BC, result);
            this->set_HL((ushort)(result & 0xffff));
        }

        void Adc_hl_de()
        {
            int result = this->HL + this->c + this->DE;
            SetAdcOpFlags(this->HL, this->DE, result);
            this->set_HL((ushort)(result & 0xffff));
        }

        void Adc_hl_hl()
        {
            int result = this->HL + this->c + this->HL;
            SetAdcOpFlags(this->HL, this->HL, result);
            this->set_HL((ushort)(result & 0xffff));
        }

        void Adc_hl_sp()
        {
            int result = this->HL + this->c + this->SP;
            SetAdcOpFlags(this->HL, this->SP, result);
            this->set_HL((ushort)(result & 0xffff));
        }

        void Add_a_r()
        {
            byte d = this->reg[(int)this->instr._register];
            byte result = (byte)((this->A + d) & 0xff);
            this->F = Get_SZHVC_add_flag(this->A, result, 0);
            this->A = result;
        }

        void Add_a_n()
        {
            byte result = (byte)((this->A + this->instr.immediate) & 0xff);
            this->F = Get_SZHVC_add_flag(this->A, result, 0);
            this->A = result;
        }

        void Add_a_hx()
        {
            byte result = (byte)((this->A + this->HX) & 0xff);
            this->F = Get_SZHVC_add_flag(this->A, result, 0);
            this->A = result;
        }

        void Add_a_lx()
        {
            byte result = (byte)((this->A + this->LX) & 0xff);
            this->F = Get_SZHVC_add_flag(this->A, result, 0);
            this->A = result;
        }

        void Add_a_hy()
        {
            byte result = (byte)((this->A + this->HY) & 0xff);
            this->F = Get_SZHVC_add_flag(this->A, result, 0);
            this->A = result;
        }

        void Add_a_ly()
        {
            byte result = (byte)((this->A + this->LY) & 0xff);
            this->F = Get_SZHVC_add_flag(this->A, result, 0);
            this->A = result;
        }

        void Add_a_hl_ind()
        {
            byte val = ReadMemory(this->HL);
            byte result = (byte)((this->A + val) & 0xff);
            this->F = Get_SZHVC_add_flag(this->A, result, 0);
            this->A = result;
        }

        void Add_a_ix_d_ind()
        {
            ushort addr = (ushort)(this->IX + this->instr.displacement);
            byte val = ReadMemory(addr);
            byte result = (byte)((this->A + val) & 0xff);
            this->F = Get_SZHVC_add_flag(this->A, result, 0);
            this->A = result;
        }

        void Add_a_iy_d_ind()
        {
            ushort addr = (ushort)(this->IY + this->instr.displacement);
            byte val = ReadMemory(addr);
            byte result = (byte)((this->A + val) & 0xff);
            this->F = Get_SZHVC_add_flag(this->A, result, 0);
            this->A = result;
        }

        void Add_hl_bc()
        {
            int result = this->HL + this->BC;
            SetAddOpFlags(this->HL, this->BC, result);
            this->set_HL((ushort)(result & 0xffff));
        }

        void Add_hl_de()
        {
            int result = this->HL + this->DE;
            SetAddOpFlags(this->HL, this->DE, result);
            this->set_HL((ushort)(result & 0xffff));
        }

        void Add_hl_hl()
        {
            int result = this->HL + this->HL;
            SetAddOpFlags(this->HL, this->HL, result);
            this->set_HL((ushort)(result & 0xffff));
        }

        void Add_hl_sp()
        {
            int result = this->HL + this->SP;
            SetAddOpFlags(this->HL, this->SP, result);
            this->set_HL((ushort)(result & 0xffff));
        }

        void Add_ix_bc()
        {
            int result = this->IX + this->BC;
            SetAddOpFlags(this->IX, this->BC, result);
            this->IX = (ushort)(result & 0xffff);
        }

        void Add_ix_de()
        {
            int result = this->IX + this->DE;
            SetAddOpFlags(this->IX, this->DE, result);
            this->IX = (ushort)(result & 0xffff);
        }

        void Add_ix_ix()
        {
            int result = this->IX + this->IX;
            SetAddOpFlags(this->IX, this->IX, result);
            this->IX = (ushort)(result & 0xffff);
        }

        void Add_ix_sp()
        {
            int result = this->IX + this->SP;
            SetAddOpFlags(this->IX, this->SP, result);
            this->IX = (ushort)(result & 0xffff);
        }

        void Add_iy_bc()
        {
            int result = this->IY + this->BC;
            SetAddOpFlags(this->IY, this->BC, result);
            this->IY = (ushort)(result & 0xffff);
        }

        void Add_iy_de()
        {
            int result = this->IY + this->DE;
            SetAddOpFlags(this->IY, this->DE, result);
            this->IY = (ushort)(result & 0xffff);
        }

        void Add_iy_iy()
        {
            int result = this->IY + this->IY;
            SetAddOpFlags(this->IY, this->IY, result);
            this->IY = (ushort)(result & 0xffff);
        }

        void Add_iy_sp()
        {
            int result = this->IY + this->SP;
            SetAddOpFlags(this->IY, this->SP, result);
            this->IY = (ushort)(result & 0xffff);
        }

        void And_a_r()
        {
            this->A = (byte)(this->A & this->reg[(int)this->instr._register]);
            this->F = (byte)(flag_SZP[this->A] | H_mask);
        }

        void And_a_n()
        {
            this->A = (byte)(this->A & this->instr.immediate);
            this->F = (byte)(flag_SZP[this->A] | H_mask);
        }

        void And_a_hx()
        {
            this->A = (byte)(this->A & this->HX);
            this->F = (byte)(flag_SZP[this->A] | H_mask);
        }

        void And_a_lx()
        {
            this->A = (byte)(this->A & this->LX);
            this->F = (byte)(flag_SZP[this->A] | H_mask);
        }

        void And_a_hy()
        {
            this->A = (byte)(this->A & this->HY);
            this->F = (byte)(flag_SZP[this->A] | H_mask);
        }

        void And_a_ly()
        {
            this->A = (byte)(this->A & this->LY);
            this->F = (byte)(flag_SZP[this->A] | H_mask);
        }

        void And_a_hl_ind()
        {
            byte val = ReadMemory(this->HL);
            this->A = (byte)(this->A & val);
            this->F = (byte)(flag_SZP[this->A] | H_mask);
        }

        void And_a_ix_d_ind()
        {
            ushort addr = (ushort)(this->IX + this->instr.displacement);
            byte val = ReadMemory(addr);
            this->A = (byte)(this->A & val);
            this->F = (byte)(flag_SZP[this->A] | H_mask);
        }

        void And_a_iy_d_ind()
        {
            ushort addr = (ushort)(this->IY + this->instr.displacement);
            byte val = ReadMemory(addr);
            this->A = (byte)(this->A & val);
            this->F = (byte)(flag_SZP[this->A] | H_mask);
        }

        void Bit_bit_r()
        {
            byte val = this->reg[(int)this->instr._register];
            int bit = (this->instr.opcode >> 3) & 0x07;
            this->F = (byte)(this->c | H_mask | flag_SZ_BIT[val & (1 << bit)]);
        }

        void Bit_bit_hl_ind()
        {
            byte val = ReadMemory(this->HL);
            int bit = (this->instr.opcode >> 3) & 0x07;
            this->F = (byte)(this->c | H_mask | flag_SZ_BIT[val & (1 << bit)]);
        }

        void Bit_bit_ix_d_ind()
        {
            ushort addr = (ushort)(this->IX + this->instr.displacement);
            byte val = ReadMemory(addr);
            int bit = (this->instr.opcode >> 3) & 0x07;
            this->F = (byte)(this->c | H_mask | flag_SZ_BIT[val & (1 << bit)]);
        }

        void Bit_bit_iy_d_ind()
        {
            ushort addr = (ushort)(this->IY + this->instr.displacement);
            byte val = ReadMemory(addr);
            int bit = (this->instr.opcode >> 3) & 0x07;
            this->F = (byte)(this->c | H_mask | flag_SZ_BIT[val & (1 << bit)]);
        }

        void Call(ushort addr)
        {
            WriteMemory(--this->SP, (byte)(this->PC >> 8));
            WriteMemory(--this->SP, (byte)(this->PC & 0xff));
            this->PC = addr;
        }

        void Call_nn()
        {
            Call(this->instr.immediateExtended);
        }

        void Call_c_nn()
        {
            if (this->c != 0)
            {
                Call(this->instr.immediateExtended);
                this->cyclesLeft -= cc_ex[this->instr.opcode];
            }
        }

        void Call_nc_nn()
        {
            if (this->c == 0)
            {
                Call(this->instr.immediateExtended);
                this->cyclesLeft -= cc_ex[this->instr.opcode];
            }
        }

        void Call_m_nn()
        {
            if (this->s)
            {
                Call(this->instr.immediateExtended);
                this->cyclesLeft -= cc_ex[this->instr.opcode];
            }
        }

        void Call_p_nn()
        {
            if (! this->s)
            {
                Call(this->instr.immediateExtended);
                this->cyclesLeft -= cc_ex[this->instr.opcode];
            }
        }

        void Call_z_nn()
        {
            if (this->z)
            {
                Call(this->instr.immediateExtended);
                this->cyclesLeft -= cc_ex[this->instr.opcode];
            }
        }

        void Call_nz_nn()
        {
            if (! this->z)
            {
                Call(this->instr.immediateExtended);
                this->cyclesLeft -= cc_ex[this->instr.opcode];
            }
        }

        void Call_pe_nn()
        {
            if (this->p)
            {
                Call(this->instr.immediateExtended);
                this->cyclesLeft -= cc_ex[this->instr.opcode];
            }
        }

        void Call_po_nn()
        {
            if (!this->p)
            {
                Call(this->instr.immediateExtended);
                this->cyclesLeft -= cc_ex[this->instr.opcode];
            }
        }

        void Ccf()
        {
            this->set_HFlag(this->c != 0);
            this->set_Carry(this->c == 0);
            this->set_NFlag(false);
        }

        void Cp_a_r()
        {
            byte d = this->reg[(int)this->instr._register];
            byte result = (byte)((this->A - d) & 0xff);
            this->F = Get_SZHVC_sub_flag(this->A, result, 0);
        }

        void Cp_a_n()
        {
            byte d = this->instr.immediate;
            byte result = (byte)((this->A - d) & 0xff);
            this->F = Get_SZHVC_sub_flag(this->A, result, 0);
        }

        void Cp_a_hx()
        {
            byte d = this->HX;
            byte result = (byte)((this->A - d) & 0xff);
            this->F = Get_SZHVC_sub_flag(this->A, result, 0);
        }

        void Cp_a_lx()
        {
            byte d = this->LX;
            byte result = (byte)((this->A - d) & 0xff);
            this->F = Get_SZHVC_sub_flag(this->A, result, 0);
        }

        void Cp_a_hy()
        {
            byte d = this->HY;
            byte result = (byte)((this->A - d) & 0xff);
            this->F = Get_SZHVC_sub_flag(this->A, result, 0);
        }

        void Cp_a_ly()
        {
            byte d = this->LY;
            byte result = (byte)((this->A - d) & 0xff);
            this->F = Get_SZHVC_sub_flag(this->A, result, 0);
        }

        void Cp_a_hl_ind()
        {
            byte val = ReadMemory(this->HL);
            byte result = (byte)((this->A - val) & 0xff);
            this->F = Get_SZHVC_sub_flag(this->A, result, 0);
        }

        void Cp_a_ix_d_ind()
        {
            ushort addr = (ushort)(this->IX + this->instr.displacement);
            byte val = ReadMemory(addr);
            byte result = (byte)((this->A - val) & 0xff);
            this->F = Get_SZHVC_sub_flag(this->A, result, 0);
        }

        void Cp_a_iy_d_ind()
        {
            ushort addr = (ushort)(this->IY + this->instr.displacement);
            byte val = ReadMemory(addr);
            byte result = (byte)((this->A - val) & 0xff);
            this->F = Get_SZHVC_sub_flag(this->A, result, 0);
        }

        // compare and decrement
        void Cpd()
        {
            byte val = ReadMemory(this->HL);
            byte result = (byte)(this->A - val);
            this->Dec_hl();
            this->Dec_bc();
            this->F = (byte)(this->c | flag_SZ[result] | ((this->A ^ val ^ result) & H_mask) |
                N_mask | (this->BC != 0 ? PV_mask : 0));
        }

        // compare and decrement, repeat
        void Cpdr()
        {
            byte val = ReadMemory(this->HL);
            byte result = (byte)(this->A - val);
            this->Dec_hl();
            this->Dec_bc();

            this->F = (byte)(this->c | flag_SZ[result] | ((this->A ^ val ^ result) & H_mask) |
                N_mask | (this->BC != 0 ? PV_mask : 0));

            if (!this->z && (this->BC != 0))
            {
                // repeat
                this->PC -= 2;
                cyclesLeft -= cc_ex[this->instr.opcode];
            }
        }

        // compare and increment
        void Cpi()
        {
            byte val = ReadMemory(this->HL);
            byte result = (byte)(this->A - val);
            this->Inc_hl();
            this->Dec_bc();
            this->F = (byte)(this->c | flag_SZ[result] | ((this->A ^ val ^ result) & H_mask) |
                N_mask | (this->BC != 0 ? PV_mask : 0));
        }

        // compare and increment, repeat
        void Cpir()
        {
            byte val = ReadMemory(this->HL);
            byte result = (byte)(this->A - val);
            this->Inc_hl();
            this->Dec_bc();
            this->F = (byte)(this->c | flag_SZ[result] | ((this->A ^ val ^ result) & H_mask) |
                N_mask | (this->BC != 0 ? PV_mask : 0));
            
            if (!this->z && (this->BC != 0))
            {
                // repeat
                this->PC -= 2;
                cyclesLeft -= cc_ex[this->instr.opcode];
            }
        }

        // complement
        void Cpl()
        {
            this->A ^= 0xff;
            this->set_HFlag(true);
            this->set_NFlag(true);
        }

        // decimal adjust
        void Da_a()
        {
            // Code review!

            int result = this->A;
            int tmp = 0;
            if (this->n)
            {
                tmp = this->F & 0x13; // & (CFLAG|NFLAG|HFLAG)
                if (this->c != 0 || this->A > 0x99)
                {
                    result -= 0x160;
                }

                if (this->h || (this->A & 0x0f) > 9)
                {
                    if ((this->A & 0x0F) > 5)
                    {
                        tmp &= 0xef; // ~HFLAG;
                    }
                    result = (result & 0xFF00) | ((result - 6) & 0xFF); 
                }
            }
            else
            {
                tmp = (this->F & 0x01) | (((this->A & 0x0F) > 9) ? 0x10 : 0x00); // 0x10:HFLAG
                if (((tmp | this->F) & 0x10) != 0)
                {
                    result += 6;
                }

                if ((this->c != 0) || ((result & 0x1F0) > 0x90))
                {
                    result += 0x60;
                }
            }
            
            this->A = (byte)(result & 0xff);
            this->F = (byte)(flag_SZP[this->A] | tmp | ((result >> 8) & 0x01));
        }

        void Dec_r()
        {
            int registerIndex = (this->instr.opcode & 0x38) >> 3;
            byte m = this->reg[registerIndex];
            byte result = (byte)(m - 1);
            this->reg[registerIndex] = result;
            this->F = (byte)(this->c | flag_SZHV_dec[result]);
        }

        void Dec_hl_ind()
        {
            byte val = ReadMemory(this->HL);
            byte result = (byte)(val - 1);
            WriteMemory(this->HL, result);
            this->F = (byte)(this->c | flag_SZHV_dec[result]); 
        }

        void Dec_ix_d_ind()
        {
            ushort addr = (ushort)(this->IX + this->instr.displacement);
            byte val = ReadMemory(addr);
            byte result = (byte)(val - 1);
            WriteMemory(addr, result);
            this->F = (byte)(this->c | flag_SZHV_dec[result]); 
        }

        void Dec_iy_d_ind()
        {
            ushort addr = (ushort)(this->IY + this->instr.displacement);
            byte val = ReadMemory(addr);
            byte result = (byte)(val - 1);
            WriteMemory(addr, result);
            this->F = (byte)(this->c | flag_SZHV_dec[result]); 
        }

        void Dec_bc()
        {
			set_BC(get_BC() - 1);
        }

        void Dec_de()
        {
			set_DE(get_DE() - 1);
        }

        void Dec_hl()
        {
			set_HL(get_HL() - 1);
        }

        void Dec_sp()
        {
            this->SP--;
        }

        void Dec_ix()
        {
            this->IX--;
        }

        void Dec_hx()
        {
            byte val = this->HX;
            byte result = (byte)(val - 1);
            this->set_HX(result);
            this->F = (byte)(this->c | flag_SZHV_dec[result]);
        }

        void Dec_lx()
        {
            byte val = this->LX;
            byte result = (byte)(val - 1);
            this->set_LX(result);
            this->F = (byte)(this->c | flag_SZHV_dec[result]);
        }

        void Dec_iy()
        {
            this->IY--;
        }

        void Dec_hy()
        {
            byte val = this->HY;
            byte result = (byte)(val - 1);
            this->set_HY(result);
            this->F = (byte)(this->c | flag_SZHV_dec[result]);
        }

        void Dec_ly()
        {
            byte val = this->LY;
            byte result = (byte)(val - 1);
            this->set_LY(result);
            this->F = (byte)(this->c | flag_SZHV_dec[result]);
        }

        void Di()
        {
            this->IFF1 = this->IFF2 = false;
        }

        void Djnz_d()
        {
            if (--this->B != 0)
            {
                this->PC += (ushort)(sbyte)(this->instr.displacement);
                cyclesLeft -= cc_ex[this->instr.opcode];
            }
        }

        void Ei()
        {
            this->IFF1 = this->IFF2 = true;
            this->AfterEI = true;
        }

        void Ex_sp_ind_hl()
        {
            byte low = ReadMemory(this->SP);
            byte hi = ReadMemory((ushort)(this->SP + 1));
            WriteMemory(this->SP, this->L);
            WriteMemory((ushort)(this->SP + 1), this->H);
            this->L = low;
            this->H = hi;
        }

        void Ex_sp_ind_ix()
        {
            ushort mem = ReadMemory16(this->SP);
            WriteMemory16(this->SP, this->IX);
            this->IX = mem;
        }

        void Ex_sp_ind_iy()
        {
            ushort mem = ReadMemory16(this->SP);
            WriteMemory16(this->SP, this->IY);
            this->IY = mem;
        }

        void Ex_af1_af2()
        {
            // (AF) <-> (AF')

            byte tmp = this->reg[A_index];
            this->reg[A_index] = this->altReg[A_index];
            this->altReg[A_index] = tmp;

            tmp = this->reg[F_index];
            this->reg[F_index] = this->altReg[F_index];
            this->altReg[F_index] = tmp;
        }

        void Ex_de_hl()
        {
            ushort tmp = this->DE;
            this->set_DE(this->HL);
            this->set_HL(tmp);
        }

        void Exx()
        {
            // (BC) <-> (BC'), (DE) <-> (DE'), (HL) <-> (HL')

            byte tmp = this->reg[B_index];
            this->reg[B_index] = this->altReg[B_index];
            this->altReg[B_index] = tmp;

            tmp = this->reg[C_index];
            this->reg[C_index] = this->altReg[C_index];
            this->altReg[C_index] = tmp;

            tmp = this->reg[D_index];
            this->reg[D_index] = this->altReg[D_index];
            this->altReg[D_index] = tmp;

            tmp = this->reg[E_index];
            this->reg[E_index] = this->altReg[E_index];
            this->altReg[E_index] = tmp;

            tmp = this->reg[H_index];
            this->reg[H_index] = this->altReg[H_index];
            this->altReg[H_index] = tmp;

            tmp = this->reg[L_index];
            this->reg[L_index] = this->altReg[L_index];
            this->altReg[L_index] = tmp;
        }

        void Halt()
        {
            this->halt = true;
            this->PC--;
            if ((this->irqLine.get_State() == Emulator::LineState::Clear) && (this->cyclesLeft > 0L))
            {
                INT64 num = (this->cyclesLeft + 3L) / 4L;
                this->R = (byte)(this->R + num);
                this->cyclesLeft -= 4L * num;
            }
        }

        void LeaveHalt()
        {
            if (this->halt)
            {
                this->halt = false;
                this->PC++;
            }
        }

        void Im_0()
        {
            this->interruptMode = InterruptMode::Mode0;
        }

        void Im_1()
        {
            this->interruptMode = InterruptMode::Mode1;
        }

        void Im_2()
        {
            this->interruptMode = InterruptMode::Mode2;
        }

        void In_a_n()
        {
            this->A = ioHandler.Read(this->instr.immediate);
        }

        void In_r_c()
        {
            int registerIndex = (this->instr.opcode & 0x38) >> 3;
            byte val = ioHandler.Read(this->C);
            this->reg[registerIndex] = val;
            this->F = (byte)((this->F & C_mask) | flag_SZP[val]);
        }

        void In_0_c()
        {
            byte val = ioHandler.Read(this->C);
            this->F = (byte)((this->F & C_mask) | flag_SZP[val]);
        }

        void Inc_r()
        {
            int registerIndex = (this->instr.opcode & 0x38) >> 3;
            byte m = this->reg[registerIndex];
            byte result = (byte)(m + 1);
            this->reg[registerIndex] = result;
            this->F = (byte)(this->c | flag_SZHV_inc[result]);
        }

        void Inc_bc()
        {
			set_BC(get_BC() + 1);
        }

        void Inc_de()
        {
			set_DE(get_DE() + 1);
        }

        void Inc_hl()
        {
			set_HL(get_HL() + 1);
        }

        void Inc_sp()
        {
            this->SP++;
        }

        void Inc_ix()
        {
            this->IX++;
        }

        void Inc_hx()
        {
            byte val = this->HX;
            byte result = (byte)(val + 1);
            this->set_HX(result);
            this->F = (byte)(this->c | flag_SZHV_inc[result]);
        }

        void Inc_lx()
        {
            byte val = this->LX;
            byte result = (byte)(val + 1);
            this->set_LX(result);
            this->F = (byte)(this->c | flag_SZHV_inc[result]);
        }

        void Inc_iy()
        {
            this->IY++;
        }

        void Inc_hy()
        {
            byte val = this->HY;
            byte result = (byte)(val + 1);
            this->set_HY(result);
            this->F = (byte)(this->c | flag_SZHV_inc[result]);
        }

        void Inc_ly()
        {
            byte val = this->LY;
            byte result = (byte)(val + 1);
            this->set_LY(result);
            this->F = (byte)(this->c | flag_SZHV_inc[result]);
        }

        void Inc_hl_ind()
        {
            byte val = ReadMemory(this->HL);
            byte result = (byte)(val + 1);
            WriteMemory(this->HL, result);
            this->F = (byte)(this->c | flag_SZHV_inc[result]);
        }

        void Inc_ix_d_ind()
        {
            ushort addr = (ushort)(this->IX + this->instr.displacement);
            byte val = ReadMemory(addr);
            byte result = (byte)(val + 1);
            WriteMemory(addr, result);
            this->F = (byte)(this->c | flag_SZHV_inc[result]);
        }

        void Inc_iy_d_ind()
        {
            ushort addr = (ushort)(this->IY + this->instr.displacement);
            byte val = ReadMemory(addr);
            byte result = (byte)(val + 1);
            WriteMemory(addr, result);
            this->F = (byte)(this->c | flag_SZHV_inc[result]);
        }

        void Ind()
        {
            byte val = ioHandler.Read(this->C);
            this->B--;
            WriteMemory(this->HL, val);
            this->Dec_hl();

            // code review
            byte f = flag_SZ[this->B];
            int t = ((this->C - 1) & 0xff) + val;
            if ((val & S_mask) != 0)
            {
                f |= N_mask;
            }

            if ((t & 0x100) != 0)
            {
                f |= (byte)(H_mask | C_mask);
            }

            this->F = f | (byte)(flag_SZP[t & 0x07 ^ this->B] & PV_mask);
        }

        void Indr()
        {
            byte val = ioHandler.Read(this->C);
            WriteMemory(this->HL, val);
            this->Dec_hl();
            this->B--;

            // code review
            byte f = flag_SZ[this->B];
            int t = ((this->C - 1) & 0xff) + val;
            if ((val & S_mask) != 0)
            {
                f |= N_mask;
            }

            if ((t & 0x100) != 0)
            {
                f |= (byte)(H_mask | C_mask);
            }

            this->F = f | (byte)(flag_SZP[t & 0x07 ^ this->B] & PV_mask);
            
            if (this->B != 0)
            {
                // repeat
                this->PC -= 2;
                cyclesLeft -= cc_ex[this->instr.opcode];
            }
        }

        void Ini()
        {
            byte val = ioHandler.Read(this->C);
            WriteMemory(this->HL, val);
            this->B--;
            this->Inc_hl();

            // code review
            byte f = flag_SZ[this->B];
            int t = ((this->C + 1) & 0xff) + val;
            if ((val & C_mask) != 0)
            {
                f |= N_mask;
            }

            if ((t & 0x100) != 0)
            {
                f |= (byte)(H_mask | C_mask);
            }

            this->F = f | (byte)(flag_SZP[t & 0x07 ^ this->B] & PV_mask);
        }

        void Inir()
        {
            byte val = ioHandler.Read(this->C);
            WriteMemory(this->HL, val);
            this->Inc_hl();
            this->B--;

            // code review
            byte f = flag_SZ[this->B];
            int t = ((this->C + 1) & 0xff) + val;
            if ((val & C_mask) != 0)
            {
                f |= N_mask;
            }

            if ((t & 0x100) != 0)
            {
                f |= (byte)(H_mask | C_mask);
            }

            this->F = f | (byte)(flag_SZP[t & 0x07 ^ this->B] & PV_mask);
            
            if (this->B != 0)
            {
                // repeat
                this->PC -= 2;
                cyclesLeft -= cc_ex[this->instr.opcode];
            }
        }

        void Jp_nn()
        {
            this->PC = this->instr.immediateExtended;
        }

        void Jp_hl_ind()
        {
            this->PC = this->HL;
        }

        void Jp_ix_ind()
        {
            this->PC = this->IX;
        }

        void Jp_iy_ind()
        {
            this->PC = this->IY;
        }

        void Jp_c_nn()
        {
            if (this->c != 0)
            {
                this->PC = this->instr.immediateExtended;
                cyclesLeft -= cc_ex[this->instr.opcode];
            }
        }

        void Jp_nc_nn()
        {
            if (this->c == 0)
            {
                this->PC = this->instr.immediateExtended;
                cyclesLeft -= cc_ex[this->instr.opcode];
            }
        }

        void Jp_m_nn()
        {
            if (this->s)
            {
                this->PC = this->instr.immediateExtended;
                cyclesLeft -= cc_ex[this->instr.opcode];
            }
        }

        void Jp_p_nn()
        {
            if (! this->s)
            {
                this->PC = this->instr.immediateExtended;
                cyclesLeft -= cc_ex[this->instr.opcode];
            }
        }

        void Jp_z_nn()
        {
            if (this->z)
            {
                this->PC = this->instr.immediateExtended;
                cyclesLeft -= cc_ex[this->instr.opcode];
            }
        }

        void Jp_nz_nn()
        {
            if (! this->z)
            {
                this->PC = this->instr.immediateExtended;
                cyclesLeft -= cc_ex[this->instr.opcode];
            }
        }

        void Jp_pe_nn()
        {
            if (this->p)
            {
                this->PC = this->instr.immediateExtended;
                cyclesLeft -= cc_ex[this->instr.opcode];
            }
        }

        void Jp_po_nn()
        {
            if (! this->p)
            {
                this->PC = this->instr.immediateExtended;
                cyclesLeft -= cc_ex[this->instr.opcode];
            }
        }

        void Jr_d()
        {
            this->PC += (ushort)(sbyte)(this->instr.displacement);
        }

        void Jr_c_d()
        {
            if (this->c != 0)
            {
                this->PC += (ushort)(sbyte)(this->instr.displacement);
                cyclesLeft -= cc_ex[this->instr.opcode];
            }
        }

        void Jr_nc_d()
        {
            if (this->c == 0)
            {
                this->PC += (ushort)(sbyte)(this->instr.displacement);
                cyclesLeft -= cc_ex[this->instr.opcode];
            }
        }

        void Jr_z_d()
        {
            if (this->z)
            {
                this->PC += (ushort)(sbyte)(this->instr.displacement);
                cyclesLeft -= cc_ex[this->instr.opcode];
            }
        }

        void Jr_nz_d()
        {
            if (! this->z)
            {
                this->PC += (ushort)(sbyte)(this->instr.displacement);
                cyclesLeft -= cc_ex[this->instr.opcode];
            }
        }

        void Ld_i_a()
        {
            this->I = this->A;
        }

        void Ld_mr_a()
        {
            this->R = this->A;
        }

        void Ld_a_i()
        {
            this->A = this->I;
            this->F = (byte)(this->c | flag_SZ[this->A] | (this->IFF2 ? PV_mask : 0));
        }

        void Ld_a_mr()
        {
            this->A = this->R;
            this->F = (byte)(this->c | flag_SZ[this->A] | (this->IFF2 ? PV_mask : 0));
        }

        void Ld_a_r()
        {
            this->A = this->reg[(int)this->instr._register];
        }

        void Ld_a_n()
        {
            this->A = this->instr.immediate;
        }

        void Ld_a_bc_ind()
        {
            this->A = ReadMemory(this->BC);
        }

        void Ld_a_de_ind()
        {
            this->A = ReadMemory(this->DE);
        }

        void Ld_a_hl_ind()
        {
            this->A = ReadMemory(this->HL);
        }

        void Ld_a_ix_d_ind()
        {
            ushort addr = (ushort)(this->IX + this->instr.displacement);
            this->A = ReadMemory(addr);
        }

        void Ld_a_hx()
        {
            this->A = this->HX;
        }

        void Ld_a_lx()
        {
            this->A = this->LX;
        }

        void Ld_a_iy_d_ind()
        {
            ushort addr = (ushort)(this->IY + this->instr.displacement);
            this->A = ReadMemory(addr);
        }

        void Ld_a_hy()
        {
            this->A = this->HY;
        }

        void Ld_a_ly()
        {
            this->A = this->LY;
        }

        void Ld_a_nn_ind()
        {
            this->A = ReadMemory(this->instr.immediateExtended);
        }

        void Ld_b_r()
        {
            this->B = this->reg[(int)this->instr._register];
        }

        void Ld_b_n()
        {
            this->B = this->instr.immediate;
        }

        void Ld_b_hl_ind()
        {
            this->B = ReadMemory(this->HL);
        }

        void Ld_b_ix_d_ind()
        {
            ushort addr = (ushort)(this->IX + this->instr.displacement);
            this->B = ReadMemory(addr);
        }

        void Ld_b_iy_d_ind()
        {
            ushort addr = (ushort)(this->IY + this->instr.displacement);
            this->B = ReadMemory(addr);
        }

        void Ld_b_hx()
        {
            this->B = this->HX;
        }

        void Ld_b_lx()
        {
            this->B = this->LX;
        }

        void Ld_b_hy()
        {
            this->B = this->HY;
        }

        void Ld_b_ly()
        {
            this->B = this->LY;
        }

        void Ld_c_r()
        {
            this->C = this->reg[(int)this->instr._register];
        }

        void Ld_c_n()
        {
            this->C = this->instr.immediate;
        }

        void Ld_c_hl_ind()
        {
            this->C = ReadMemory(this->HL);
        }

        void Ld_c_ix_d_ind()
        {
            ushort addr = (ushort)(this->IX + this->instr.displacement);
            this->C = ReadMemory(addr);
        }

        void Ld_c_hx()
        {
            this->C = this->HX;
        }

        void Ld_c_lx()
        {
            this->C = this->LX;
        }

        void Ld_c_iy_d_ind()
        {
            ushort addr = (ushort)(this->IY + this->instr.displacement);
            this->C = ReadMemory(addr);
        }

        void Ld_c_hy()
        {
            this->C = this->HY;
        }

        void Ld_c_ly()
        {
            this->C = this->LY;
        }

        void Ld_d_r()
        {
            this->D = this->reg[(int)this->instr._register];
        }

        void Ld_d_n()
        {
            this->D = this->instr.immediate;
        }

        void Ld_d_hl_ind()
        {
            this->D = ReadMemory(this->HL);
        }

        void Ld_d_ix_d_ind()
        {
            ushort addr = (ushort)(this->IX + this->instr.displacement);
            this->D = ReadMemory(addr);
        }

        void Ld_d_hx()
        {
            this->D = this->HX;
        }

        void Ld_d_lx()
        {
            this->D = this->LX;
        }

        void Ld_d_iy_d_ind()
        {
            ushort addr = (ushort)(this->IY + this->instr.displacement);
            this->D = ReadMemory(addr);
        }

        void Ld_d_hy()
        {
            this->D = this->HY;
        }

        void Ld_d_ly()
        {
            this->D = this->LY;
        }

        void Ld_e_r()
        {
            this->E = this->reg[(int)this->instr._register];
        }

        void Ld_e_n()
        {
            this->E = this->instr.immediate;
        }

        void Ld_e_hl_ind()
        {
            this->E = ReadMemory(this->HL);
        }

        void Ld_e_ix_d_ind()
        {
            ushort addr = (ushort)(this->IX + this->instr.displacement);
            this->E = ReadMemory(addr);
        }

        void Ld_e_hx()
        {
            this->E = this->HX;
        }

        void Ld_e_lx()
        {
            this->E = this->LX;
        }

        void Ld_e_iy_d_ind()
        {
            ushort addr = (ushort)(this->IY + this->instr.displacement);
            this->E = ReadMemory(addr);
        }

        void Ld_e_hy()
        {
            this->E = this->HY;
        }

        void Ld_e_ly()
        {
            this->E = this->LY;
        }

        void Ld_h_r()
        {
            this->H = this->reg[(int)this->instr._register];
        }

        void Ld_h_n()
        {
            this->H = this->instr.immediate;
        }

        void Ld_h_hl_ind()
        {
            this->H = ReadMemory(this->HL);
        }

        void Ld_h_ix_d_ind()
        {
            ushort addr = (ushort)(this->IX + this->instr.displacement);
            this->H = ReadMemory(addr);
        }

        void Ld_h_iy_d_ind()
        {
            ushort addr = (ushort)(this->IY + this->instr.displacement);
            this->H = ReadMemory(addr);
        }

        void Ld_l_r()
        {
            this->L = this->reg[(int)this->instr._register];
        }

        void Ld_l_n()
        {
            this->L = this->instr.immediate;
        }

        void Ld_l_hl_ind()
        {
            this->L = ReadMemory(this->HL);
        }

        void Ld_l_ix_d_ind()
        {
            ushort addr = (ushort)(this->IX + this->instr.displacement);
            this->L = ReadMemory(addr);
        }

        void Ld_l_iy_d_ind()
        {
            ushort addr = (ushort)(this->IY + this->instr.displacement);
            this->L = ReadMemory(addr);
        }

        void Ld_bc_nn_ind()
        {
            this->set_BC(ReadMemory16(this->instr.immediateExtended));
        }

        void Ld_bc_nn()
        {
            this->set_BC(this->instr.immediateExtended);
        }

        void Ld_de_nn_ind()
        {
            this->set_DE(ReadMemory16(this->instr.immediateExtended));
        }

        void Ld_de_nn()
        {
            this->set_DE(this->instr.immediateExtended);
        }

        void Ld_hl_nn_ind()
        {
            this->set_HL(ReadMemory16(this->instr.immediateExtended));
        }

        void Ld_hl_nn()
        {
            this->set_HL(this->instr.immediateExtended);
        }

        void Ld_sp_nn_ind()
        {
            this->SP = ReadMemory16(this->instr.immediateExtended);
        }

        void Ld_sp_hl()
        {
            this->SP = this->HL;
        }

        void Ld_sp_ix()
        {
            this->SP = this->IX;
        }

        void Ld_sp_iy()
        {
            this->SP = this->IY;
        }

        void Ld_sp_nn()
        {
            this->SP = this->instr.immediateExtended;
        }

        void Ld_hx_n()
        {
            this->set_HX(this->instr.immediate);
        }

        void Ld_hx_a()
        {
            this->set_HX(this->A);
        }

        void Ld_hx_b()
        {
            this->set_HX(this->B);
        }

        void Ld_hx_c()
        {
            this->set_HX(this->C);
        }

        void Ld_hx_d()
        {
            this->set_HX(this->D);
        }

        void Ld_hx_e()
        {
            this->set_HX(this->E);
        }

        void Ld_hx_hx()
        {
            // do nothing
        }

        void Ld_hx_lx()
        {
            this->set_HX(this->LX);
        }

        void Ld_lx_n()
        {
            this->set_LX(this->instr.immediate);
        }

        void Ld_lx_a()
        {
            this->set_LX(this->A);
        }

        void Ld_lx_b()
        {
            this->set_LX(this->B);
        }

        void Ld_lx_c()
        {
            this->set_LX(this->C);
        }

        void Ld_lx_d()
        {
            this->set_LX(this->D);
        }

        void Ld_lx_e()
        {
            this->set_LX(this->E);
        }

        void Ld_lx_hx()
        {
            this->set_LX(this->HX);
        }

        void Ld_lx_lx()
        {
            // do nothing
        }

        void Ld_ix_nn_ind()
        {
            this->IX = ReadMemory16(this->instr.immediateExtended);
        }

        void Ld_ix_nn()
        {
            this->IX = this->instr.immediateExtended;
        }

        void Ld_hy_n()
        {
            this->set_HY(this->instr.immediate);
        }

        void Ld_hy_a()
        {
            this->set_HY(this->A);
        }

        void Ld_hy_b()
        {
            this->set_HY(this->B);
        }

        void Ld_hy_c()
        {
            this->set_HY(this->C);
        }

        void Ld_hy_d()
        {
            this->set_HY(this->D);
        }

        void Ld_hy_e()
        {
            this->set_HY(this->E);
        }

        void Ld_hy_hy()
        {
            // do nothing
        }

        void Ld_hy_ly()
        {
            this->set_HY(this->LY);
        }

        void Ld_ly_n()
        {
            this->set_LY(this->instr.immediate);
        }

        void Ld_ly_a()
        {
            this->set_LY(this->A);
        }

        void Ld_ly_b()
        {
            this->set_LY(this->B);
        }

        void Ld_ly_c()
        {
            this->set_LY(this->C);
        }

        void Ld_ly_d()
        {
            this->set_LY(this->D);
        }

        void Ld_ly_e()
        {
            this->set_LY(this->E);
        }

        void Ld_ly_hy()
        {
            this->set_LY(this->HY);
        }

        void Ld_ly_ly()
        {
            // do nothing
        }

        void Ld_iy_nn_ind()
        {
            this->IY = ReadMemory16(this->instr.immediateExtended);
        }

        void Ld_iy_nn()
        {
            this->IY = this->instr.immediateExtended;
        }

        void Ld_hl_ind_r()
        {
            WriteMemory(this->HL, this->reg[(int)this->instr._register]);
        }

        void Ld_hl_ind_n()
        {
            WriteMemory(this->HL, this->instr.immediate);
        }

        void Ld_bc_ind_a()
        {
            WriteMemory(this->BC, this->A);
        }

        void Ld_de_ind_a()
        {
            WriteMemory(this->DE, this->A);
        }

        void Ld_nn_ind_a()
        {
            WriteMemory(this->instr.immediateExtended, this->A);
        }

        void Ld_nn_ind_bc()
        {
            WriteMemory16(this->instr.immediateExtended, this->BC);
        }

        void Ld_nn_ind_de()
        {
            WriteMemory16(this->instr.immediateExtended, this->DE);
        }

        void Ld_nn_ind_hl()
        {
            WriteMemory16(this->instr.immediateExtended, this->HL);
        }

        void Ld_nn_ind_sp()
        {
            WriteMemory16(this->instr.immediateExtended, this->SP);
        }

        void Ld_nn_ind_ix()
        {
            WriteMemory16(this->instr.immediateExtended, this->IX);
        }

        void Ld_nn_ind_iy()
        {
            WriteMemory16(this->instr.immediateExtended, this->IY);
        }

        void Ld_ix_d_ind_r()
        {
            ushort addr = (ushort)(this->IX + this->instr.displacement);
            WriteMemory(addr, this->reg[(int)this->instr._register]);
        }

        void Ld_ix_d_ind_n()
        {
            ushort addr = (ushort)(this->IX + this->instr.displacement);
            WriteMemory(addr, this->instr.immediate);
        }

        void Ld_iy_d_ind_r()
        {
            ushort addr = (ushort)(this->IY + this->instr.displacement);
            WriteMemory(addr, this->reg[(int)this->instr._register]);
        }

        void Ld_iy_d_ind_n()
        {
            ushort addr = (ushort)(this->IY + this->instr.displacement);
            WriteMemory(addr, this->instr.immediate);
        }

        void Ldd()
        {
            byte val = ReadMemory(this->HL);
            WriteMemory(this->DE, val);
            this->Dec_de();
            this->Dec_hl();
            this->Dec_bc();
            this->set_HFlag(false);
            this->set_NFlag(false);
            this->set_PVFlag(this->BC != 0);
        }

        void Lddr()
        {
            WriteMemory(this->DE, ReadMemory(this->HL));
            this->Dec_de();
            this->Dec_hl();
            this->Dec_bc();

            this->set_HFlag(false);
            this->set_NFlag(false);
            this->set_PVFlag(false);

            if (this->BC != 0)
            {
                // repeat
                this->PC -= 2;
                cyclesLeft -= cc_ex[this->instr.opcode];
            }
        }

        void Ldi()
        {
            WriteMemory(this->DE, ReadMemory(this->HL));
            this->Inc_de();
            this->Inc_hl();
            this->Dec_bc();
            this->set_HFlag(false);
            this->set_NFlag(false);
            this->set_PVFlag(this->BC != 0);
        }

        void Ldir()
        {
            WriteMemory(this->DE, ReadMemory(this->HL));
            this->Inc_de();
            this->Inc_hl();
            this->Dec_bc();
            this->set_HFlag(false);
            this->set_NFlag(false);
            this->set_PVFlag(false);

            if (this->BC != 0)
            {
                // repeat
                this->PC -= 2;
                cyclesLeft -= cc_ex[this->instr.opcode];
            }
        }

        void Neg()
        {
            byte val = this->A;
            this->A = (byte)(-val);
            this->F = (byte)(Get_SZHVC_sub_flag(0, this->A, 0) | N_mask);
        }

        void Nop()
        {
            // do nothing
        }

        void Or_a_r()
        {
            this->A = (byte)(this->A | this->reg[(int)this->instr._register]);
            this->F = flag_SZP[this->A];
        }

        void Or_a_n()
        {
            this->A = (byte)(this->A | this->instr.immediate);
            this->F = flag_SZP[this->A];
        }

        void Or_a_hx()
        {
            this->A = (byte)(this->A | this->HX);
            this->F = flag_SZP[this->A];
        }

        void Or_a_lx()
        {
            this->A = (byte)(this->A | this->LX);
            this->F = flag_SZP[this->A];
        }

        void Or_a_hy()
        {
            this->A = (byte)(this->A | this->HY);
            this->F = flag_SZP[this->A];
        }

        void Or_a_ly()
        {
            this->A = (byte)(this->A | this->LY);
            this->F = flag_SZP[this->A];
        }

        void Or_a_hl_ind()
        {
            this->A = (byte)(this->A | ReadMemory(this->HL));
            this->F = flag_SZP[this->A];
        }

        void Or_a_ix_d_ind()
        {
            ushort addr = (ushort)(this->IX + this->instr.displacement);
            byte val = ReadMemory(addr);
            this->A = (byte)(this->A | val);
            this->F = flag_SZP[this->A];
        }

        void Or_a_iy_d_ind()
        {
            ushort addr = (ushort)(this->IY + this->instr.displacement);
            byte val = ReadMemory(addr);
            this->A = (byte)(this->A | val);
            this->F = flag_SZP[this->A];
        }

        void Out_n_a()
        {
            ioHandler.Write(this->instr.immediate, this->A);
        }

        void Out_c_r()
        {
            int registerIndex = (this->instr.opcode & 0x38) >> 3;
            ioHandler.Write(this->C, this->reg[registerIndex]);
        }

        void Out_c_0()
        {
            ioHandler.Write(this->C, 0);
        }

        void Outd()
        {
            byte val = ReadMemory(this->HL);
            this->B--;
            ioHandler.Write(this->C, val);
            this->Dec_hl();

            // code review
            this->F = flag_SZ[this->B];
            int t = this->L + val;
            if ((val & S_mask) != 0)
            {
                this->F |= N_mask;
            }

            if ((t & 0x100) != 0)
            {
                this->F |= (byte)(H_mask | C_mask);
            }

            this->F |= (byte)(flag_SZP[t & 0x07 ^ this->B] & PV_mask);
        }

        void Otdr()
        {
            byte val = ReadMemory(this->HL);
            this->B--;
            ioHandler.Write(this->C, val);
            this->Dec_hl();

            // code review
            this->F = flag_SZ[this->B];
            int t = this->L + val;
            if ((val & S_mask) != 0)
            {
                this->F |= N_mask;
            }

            if ((t & 0x100) != 0)
            {
                this->F |= (byte)(H_mask | C_mask);
            }

            this->F |= (byte)(flag_SZP[t & 0x07 ^ this->B] & PV_mask);

            if (this->B != 0)
            {
                // repeat
                this->PC -= 2;
                cyclesLeft -= cc_ex[this->instr.opcode];
            }
        }

        void Outi()
        {
            byte val = ReadMemory(this->HL);
            this->B--;
            ioHandler.Write(this->C, val);
            this->Inc_hl();

            // code review
            this->F = flag_SZ[this->B];
            int t = this->L + val;
            if ((val & S_mask) != 0)
            {
                this->F |= N_mask;
            }

            if ((t & 0x100) != 0)
            {
                this->F |= (byte)(H_mask | C_mask);
            }

            this->F |= (byte)(flag_SZP[t & 0x07 ^ this->B] & PV_mask);
        }

        void Otir()
        {
            byte val = ReadMemory(this->HL);
            this->B--;
            ioHandler.Write(this->C, val);
            this->Inc_hl();

            // code review
            this->F = flag_SZ[this->B];
            int t = this->L + val;
            if ((val & S_mask) != 0)
            {
                this->F |= N_mask;
            }

            if ((t & 0x100) != 0)
            {
                this->F |= (byte)(H_mask | C_mask);
            }

            this->F |= (byte)(flag_SZP[t & 0x07 ^ this->B] & PV_mask);

            if (this->B != 0)
            {
                // repeat
                this->PC -= 2;
                cyclesLeft -= cc_ex[this->instr.opcode];
            }
        }

        void Pop_af()
        {
            this->F = ReadMemory(this->SP++);
            this->A = ReadMemory(this->SP++);
        }

        void Pop_bc()
        {
            this->set_BC(ReadMemory16(this->SP));
            this->SP += 2;
        }

        void Pop_de()
        {
            this->set_DE(ReadMemory16(this->SP));
            this->SP += 2;
        }

        void Pop_hl()
        {
            this->set_HL(ReadMemory16(this->SP));
            this->SP += 2;
        }

        void Pop_ix()
        {
            this->IX = ReadMemory16(this->SP);
            this->SP += 2;
        }

        void Pop_iy()
        {
            this->IY = ReadMemory16(this->SP);
            this->SP += 2;
        }

        void Push_af()
        {
            WriteMemory(--this->SP, this->A);
            WriteMemory(--this->SP, this->F);
        }

        void Push_bc()
        {
            this->SP -= 2;
            WriteMemory16(this->SP, this->BC);
        }

        void Push_de()
        {
            this->SP -= 2;
            WriteMemory16(this->SP, this->DE);
        }

        void Push_hl()
        {
            this->SP -= 2;
            WriteMemory16(this->SP, this->HL);
        }

        void Push_ix()
        {
            this->SP -= 2;
            WriteMemory16(this->SP, this->IX);
        }

        void Push_iy()
        {
            this->SP -= 2;
            WriteMemory16(this->SP, this->IY);
        }

        void Push_pc()
        {
            this->SP -= 2;
            WriteMemory16(this->SP, this->PC);
        }

        void Res_bit_r()
        {
            int bit = (this->instr.opcode >> 3) & 0x07;
            this->reg[(int)this->instr._register] &= rstMask[bit];
        }

        void Res_bit_hl_ind()
        {
            int bit = (this->instr.opcode >> 3) & 0x07;
            byte val = ReadMemory(this->HL);
            WriteMemory(this->HL, (byte)(val & rstMask[bit]));
        }

        byte Res_bit_ix()
        {
            ushort addr = (ushort)(this->IX + this->instr.displacement);
            byte val = ReadMemory(addr);
            int bit = (this->instr.opcode >> 3) & 0x07;
            byte result = (byte)(val & rstMask[bit]);
            WriteMemory(addr, result);
            return result;
        }

        byte Res_bit_iy()
        {
            ushort addr = (ushort)(this->IY + this->instr.displacement);
            byte val = ReadMemory(addr);
            int bit = (this->instr.opcode >> 3) & 0x07;
            byte result = (byte)(val & rstMask[bit]);
            WriteMemory(addr, result);
            return result;
        }

        void Res_bit_ix_d_ind()
        {
            Res_bit_ix();
        }

        void Res_bit_ix_d_ind_Ld_a()
        {
            this->A = Res_bit_ix();
        }

        void Res_bit_ix_d_ind_Ld_b()
        {
            this->B = Res_bit_ix();
        }

        void Res_bit_ix_d_ind_Ld_c()
        {
            this->C = Res_bit_ix();
        }

        void Res_bit_ix_d_ind_Ld_d()
        {
            this->D = Res_bit_ix();
        }

        void Res_bit_ix_d_ind_Ld_e()
        {
            this->E = Res_bit_ix();
        }

        void Res_bit_ix_d_ind_Ld_h()
        {
            this->H = Res_bit_ix();
        }

        void Res_bit_ix_d_ind_Ld_l()
        {
            this->L = Res_bit_ix();
        }

        void Res_bit_iy_d_ind()
        {
            Res_bit_iy();
        }

        void Res_bit_iy_d_ind_Ld_a()
        {
            this->A = Res_bit_iy();
        }

        void Res_bit_iy_d_ind_Ld_b()
        {
            this->B = Res_bit_iy();
        }

        void Res_bit_iy_d_ind_Ld_c()
        {
            this->C = Res_bit_iy();
        }

        void Res_bit_iy_d_ind_Ld_d()
        {
            this->D = Res_bit_iy();
        }

        void Res_bit_iy_d_ind_Ld_e()
        {
            this->E = Res_bit_iy();
        }

        void Res_bit_iy_d_ind_Ld_h()
        {
            this->H = Res_bit_iy();
        }

        void Res_bit_iy_d_ind_Ld_l()
        {
            this->L = Res_bit_iy();
        }

        void Ret()
        {
            this->PC = ReadMemory16(this->SP);
            this->SP += 2;
        }

        void Ret_c()
        {
            if (this->c != 0)
            {
                Ret();
                cyclesLeft -= cc_ex[this->instr.opcode];
            }
        }

        void Ret_nc()
        {
            if (this->c == 0)
            {
                Ret();
                cyclesLeft -= cc_ex[this->instr.opcode];
            }
        }

        void Ret_m()
        {
            if (this->s)
            {
                Ret();
                cyclesLeft -= cc_ex[this->instr.opcode];
            }
        }

        void Ret_p()
        {
            if (! this->s)
            {
                Ret();
                cyclesLeft -= cc_ex[this->instr.opcode];
            }
        }

        void Ret_z()
        {
            if (this->z)
            {
                Ret();
                cyclesLeft -= cc_ex[this->instr.opcode];
            }
        }

        void Ret_nz()
        {
            if (! this->z)
            {
                Ret();
                cyclesLeft -= cc_ex[this->instr.opcode];
            }
        }

        void Ret_pe()
        {
            if (this->p)
            {
                Ret();
                cyclesLeft -= cc_ex[this->instr.opcode];
            }
        }

        void Ret_po()
        {
            if (! this->p)
            {
                Ret();
                cyclesLeft -= cc_ex[this->instr.opcode];
            }
        }

        void Reti()
        {
            // Return from interrupt
            Ret();

            // according to http://www.msxnet.org/tech/z80-documented.pdf
            this->IFF1 = this->IFF2;
        }

        void Retn()
        {
            // Return from non maskable interrupt
            Ret();

            this->IFF1 = this->IFF2;
        }

        void Rl_a()
        {
            byte newCarry = (byte)((this->A & 0x80) >> 7);
            this->A = (byte)(((this->A & 0x7f) << 1) | this->c);
            this->F = (byte)((this->F & (S_mask | Z_mask | PV_mask)) | newCarry);
        }

        void Rl_r()
        {
            byte val = this->reg[(int)this->instr._register];
            byte newCarry = (byte)((val & 0x80) >> 7);
            byte result = (byte)(((val & 0x7f) << 1) | this->c);
            this->reg[(int)this->instr._register] = result;
            this->F = (byte)(flag_SZP[result] | newCarry);
        }

        void Rl_hl_ind()
        {
            byte val = ReadMemory(this->HL);
            byte newCarry = (byte)((val & 0x80) >> 7);
            byte result = (byte)(((val & 0x7f) << 1) | this->c);
            WriteMemory(this->HL, result);
            this->F = (byte)(flag_SZP[result] | newCarry);
        }

        byte Rl_ind(ushort addr)
        {
            byte val = ReadMemory(addr);
            byte newCarry = (byte)((val & 0x80) >> 7);
            byte result = (byte)(((val & 0x7f) << 1) | this->c);
            WriteMemory(addr, result);
            this->F = (byte)(flag_SZP[result] | newCarry);
            return result;
        }

        void Rl_ix_d_ind()
        {
            Rl_ind((ushort)(this->IX + this->instr.displacement));
        }

        void Rl_ix_d_ind_Ld_a()
        {
            this->A = Rl_ind((ushort)(this->IX + this->instr.displacement));
        }

        void Rl_ix_d_ind_Ld_b()
        {
            this->B = Rl_ind((ushort)(this->IX + this->instr.displacement));
        }

        void Rl_ix_d_ind_Ld_c()
        {
            this->C = Rl_ind((ushort)(this->IX + this->instr.displacement));
        }

        void Rl_ix_d_ind_Ld_d()
        {
            this->D = Rl_ind((ushort)(this->IX + this->instr.displacement));
        }
        void Rl_ix_d_ind_Ld_e()
        {
            this->E = Rl_ind((ushort)(this->IX + this->instr.displacement));
        }

        void Rl_ix_d_ind_Ld_h()
        {
            this->H = Rl_ind((ushort)(this->IX + this->instr.displacement));
        }

        void Rl_ix_d_ind_Ld_l()
        {
            this->L = Rl_ind((ushort)(this->IX + this->instr.displacement));
        }

        void Rl_iy_d_ind()
        {
            Rl_ind((ushort)(this->IY + this->instr.displacement));
        }

        void Rl_iy_d_ind_Ld_a()
        {
            this->A = Rl_ind((ushort)(this->IY + this->instr.displacement));
        }

        void Rl_iy_d_ind_Ld_b()
        {
            this->B = Rl_ind((ushort)(this->IY + this->instr.displacement));
        }

        void Rl_iy_d_ind_Ld_c()
        {
            this->C = Rl_ind((ushort)(this->IY + this->instr.displacement));
        }

        void Rl_iy_d_ind_Ld_d()
        {
            this->D = Rl_ind((ushort)(this->IY + this->instr.displacement));
        }

        void Rl_iy_d_ind_Ld_e()
        {
            this->E = Rl_ind((ushort)(this->IY + this->instr.displacement));
        }

        void Rl_iy_d_ind_Ld_h()
        {
            this->H = Rl_ind((ushort)(this->IY + this->instr.displacement));
        }

        void Rl_iy_d_ind_Ld_l()
        {
            this->L = Rl_ind((ushort)(this->IY + this->instr.displacement));
        }

        byte Rlc_ind(ushort addr)
        {
            byte val = ReadMemory(addr);
            byte newCarry = (byte)((val & 0x80) >> 7);
            byte result = (byte)(((val & 0x7f) << 1) | newCarry);
            WriteMemory(addr, result);
            this->F = (byte)(flag_SZP[result] | newCarry);
            return result;
        }

        void Rlc_ix_d_ind()
        {
            Rlc_ind((ushort)(this->IX + this->instr.displacement));
        }

        void Rlc_ix_d_ind_Ld_a()
        {
            this->A = Rlc_ind((ushort)(this->IX + this->instr.displacement));
        }

        void Rlc_ix_d_ind_Ld_b()
        {
            this->B = Rlc_ind((ushort)(this->IX + this->instr.displacement));
        }

        void Rlc_ix_d_ind_Ld_c()
        {
            this->C = Rlc_ind((ushort)(this->IX + this->instr.displacement));
        }

        void Rlc_ix_d_ind_Ld_d()
        {
            this->D = Rlc_ind((ushort)(this->IX + this->instr.displacement));
        }

        void Rlc_ix_d_ind_Ld_e()
        {
            this->E = Rlc_ind((ushort)(this->IX + this->instr.displacement));
        }

        void Rlc_ix_d_ind_Ld_h()
        {
            this->H = Rlc_ind((ushort)(this->IX + this->instr.displacement));
        }

        void Rlc_ix_d_ind_Ld_l()
        {
            this->L = Rlc_ind((ushort)(this->IX + this->instr.displacement));
        }

        void Rlc_iy_d_ind()
        {
            Rlc_ind((ushort)(this->IY + this->instr.displacement));
        }

        void Rlc_iy_d_ind_Ld_a()
        {
            this->A = Rlc_ind((ushort)(this->IY + this->instr.displacement));
        }

        void Rlc_iy_d_ind_Ld_b()
        {
            this->B = Rlc_ind((ushort)(this->IY + this->instr.displacement));
        }

        void Rlc_iy_d_ind_Ld_c()
        {
            this->C = Rlc_ind((ushort)(this->IY + this->instr.displacement));
        }

        void Rlc_iy_d_ind_Ld_d()
        {
            this->D = Rlc_ind((ushort)(this->IY + this->instr.displacement));
        }

        void Rlc_iy_d_ind_Ld_e()
        {
            this->E = Rlc_ind((ushort)(this->IY + this->instr.displacement));
        }

        void Rlc_iy_d_ind_Ld_h()
        {
            this->H = Rlc_ind((ushort)(this->IY + this->instr.displacement));
        }

        void Rlc_iy_d_ind_Ld_l()
        {
            this->L = Rlc_ind((ushort)(this->IY + this->instr.displacement));
        }

        void Rlc_a()
        {
            byte newCarry = (byte)((this->A & 0x80) >> 7);
            this->A = (byte)(((this->A & 0x7f) << 1) | newCarry);
            this->F = (byte)((this->F & (S_mask | Z_mask | PV_mask)) | newCarry);
        }

        void Rlc_r()
        {
            byte val = this->reg[(int)this->instr._register];
            byte newCarry = (byte)((val & 0x80) >> 7);
            byte result = (byte)(((val & 0x7f) << 1) | newCarry);
            this->reg[(int)this->instr._register] = result;
            this->F = (byte)(flag_SZP[result] | newCarry);
        }

        void Rlc_hl_ind()
        {
            byte val = ReadMemory(this->HL);
            byte newCarry = (byte)((val & 0x80) >> 7);
            byte result = (byte)(((val & 0x7f) << 1) | newCarry);
            WriteMemory(this->HL, result);
            this->F = (byte)(flag_SZP[result] | newCarry);
        }

        void Rld_a_hl()
        {
            byte val = ReadMemory(this->HL);
            byte a = this->A;
            WriteMemory(this->HL, (byte)(((val & 0x0f) << 4) | (a & 0x0f)));
            this->A = (byte)((a & 0xf0) | (val >> 4));
            this->F = (byte)(flag_SZP[this->A] | this->c);
        }

        void Rr_a()
        {
            byte newCarry = (byte)(this->A & 0x01);
            this->A = (byte)((this->A >> 1) | (this->c << 7));
            this->F = (byte)((this->F & (S_mask | Z_mask | PV_mask)) | newCarry);
        }

        void Rr_r()
        {
            byte val = this->reg[(int)this->instr._register];
            byte newCarry = (byte)(val & 0x01);
            byte result = (byte)((val >> 1) | (this->c << 7));
            this->reg[(int)this->instr._register] = result;
            this->F = (byte)(flag_SZP[result] | newCarry);
        }

        void Rr_hl_ind()
        {
            byte val = ReadMemory(this->HL);
            byte newCarry = (byte)(val & 0x01);
            byte result = (byte)((val >> 1) | (this->c << 7));
            WriteMemory(this->HL, result);
            this->F = (byte)(flag_SZP[result] | newCarry);
        }

        byte Rr_ind(ushort addr)
        {
            byte val = ReadMemory(addr);
            byte newCarry = (byte)(val & 0x01);
            byte result = (byte)((val >> 1) | (this->c << 7));
            WriteMemory(addr, result);
            this->F = (byte)(flag_SZP[result] | newCarry);
            return result;
        }

        void Rr_ix_d_ind()
        {
            Rr_ind((ushort)(this->IX + this->instr.displacement));
        }

        void Rr_ix_d_ind_Ld_a()
        {
            this->A = Rr_ind((ushort)(this->IX + this->instr.displacement));
        }

        void Rr_ix_d_ind_Ld_b()
        {
            this->B = Rr_ind((ushort)(this->IX + this->instr.displacement));
        }

        void Rr_ix_d_ind_Ld_c()
        {
            this->C = Rr_ind((ushort)(this->IX + this->instr.displacement));
        }

        void Rr_ix_d_ind_Ld_d()
        {
            this->D = Rr_ind((ushort)(this->IX + this->instr.displacement));
        }

        void Rr_ix_d_ind_Ld_e()
        {
            this->E = Rr_ind((ushort)(this->IX + this->instr.displacement));
        }

        void Rr_ix_d_ind_Ld_h()
        {
            this->H = Rr_ind((ushort)(this->IX + this->instr.displacement));
        }

        void Rr_ix_d_ind_Ld_l()
        {
            this->L = Rr_ind((ushort)(this->IX + this->instr.displacement));
        }

        void Rr_iy_d_ind()
        {
            Rr_ind((ushort)(this->IY + this->instr.displacement));
        }

        void Rr_iy_d_ind_Ld_a()
        {
            this->A = Rr_ind((ushort)(this->IY + this->instr.displacement));
        }

        void Rr_iy_d_ind_Ld_b()
        {
            this->B = Rr_ind((ushort)(this->IY + this->instr.displacement));
        }

        void Rr_iy_d_ind_Ld_c()
        {
            this->C = Rr_ind((ushort)(this->IY + this->instr.displacement));
        }

        void Rr_iy_d_ind_Ld_d()
        {
            this->D = Rr_ind((ushort)(this->IY + this->instr.displacement));
        }

        void Rr_iy_d_ind_Ld_e()
        {
            this->E = Rr_ind((ushort)(this->IY + this->instr.displacement));
        }

        void Rr_iy_d_ind_Ld_h()
        {
            this->H = Rr_ind((ushort)(this->IY + this->instr.displacement));
        }

        void Rr_iy_d_ind_Ld_l()
        {
            this->L = Rr_ind((ushort)(this->IY + this->instr.displacement));
        }

        void Rrc_a()
        {
            byte newCarry = (byte)(this->A & 0x01);
            this->A = (byte)((this->A >> 1) | (newCarry << 7));
            this->F = (byte)((this->F & (S_mask | Z_mask | PV_mask)) | newCarry);
        }

        void Rrc_r()
        {
            byte val = this->reg[(int)this->instr._register];
            byte newCarry = (byte)(val & 0x01);
            byte result = (byte)((val >> 1) | (newCarry << 7));
            this->reg[(int)this->instr._register] = result;
            this->F = (byte)(flag_SZP[result] | newCarry);
        }

        void Rrc_hl_ind()
        {
            byte val = ReadMemory(this->HL);
            byte newCarry = (byte)(val & 0x01);
            byte result = (byte)((val >> 1) | (newCarry << 7));
            WriteMemory(this->HL, result);
            this->F = (byte)(flag_SZP[result] | newCarry);
        }

        byte Rrc_ind(ushort addr)
        {
            byte val = ReadMemory(addr);
            byte newCarry = (byte)(val & 0x01);
            byte result = (byte)((val >> 1) | (newCarry << 7));
            WriteMemory(addr, result);
            this->F = (byte)(flag_SZP[result] | newCarry);
            return result;
        }

        void Rrc_ix_d_ind()
        {
            Rrc_ind((ushort)(this->IX + this->instr.displacement));
        }

        void Rrc_ix_d_ind_Ld_a()
        {
            this->A = Rrc_ind((ushort)(this->IX + this->instr.displacement));
        }

        void Rrc_ix_d_ind_Ld_b()
        {
            this->B = Rrc_ind((ushort)(this->IX + this->instr.displacement));
        }

        void Rrc_ix_d_ind_Ld_c()
        {
            this->C = Rrc_ind((ushort)(this->IX + this->instr.displacement));
        }

        void Rrc_ix_d_ind_Ld_d()
        {
            this->D = Rrc_ind((ushort)(this->IX + this->instr.displacement));
        }

        void Rrc_ix_d_ind_Ld_e()
        {
            this->E = Rrc_ind((ushort)(this->IX + this->instr.displacement));
        }

        void Rrc_ix_d_ind_Ld_h()
        {
            this->H = Rrc_ind((ushort)(this->IX + this->instr.displacement));
        }

        void Rrc_ix_d_ind_Ld_l()
        {
            this->L = Rrc_ind((ushort)(this->IX + this->instr.displacement));
        }

        void Rrc_iy_d_ind()
        {
            Rrc_ind((ushort)(this->IY + this->instr.displacement));
        }

        void Rrc_iy_d_ind_Ld_a()
        {
            this->A = Rrc_ind((ushort)(this->IY + this->instr.displacement));
        }

        void Rrc_iy_d_ind_Ld_b()
        {
            this->B = Rrc_ind((ushort)(this->IY + this->instr.displacement));
        }

        void Rrc_iy_d_ind_Ld_c()
        {
            this->C = Rrc_ind((ushort)(this->IY + this->instr.displacement));
        }

        void Rrc_iy_d_ind_Ld_d()
        {
            this->D = Rrc_ind((ushort)(this->IY + this->instr.displacement));
        }

        void Rrc_iy_d_ind_Ld_e()
        {
            this->E = Rrc_ind((ushort)(this->IY + this->instr.displacement));
        }

        void Rrc_iy_d_ind_Ld_h()
        {
            this->H = Rrc_ind((ushort)(this->IY + this->instr.displacement));
        }

        void Rrc_iy_d_ind_Ld_l()
        {
            this->L = Rrc_ind((ushort)(this->IY + this->instr.displacement));
        }

        void Rrd_a_hl()
        {
            byte val = ReadMemory(this->HL);
            byte a = this->A;
            WriteMemory(this->HL, (byte)(((a & 0x0f) << 4) | (val >> 4)));
            this->A = (byte)((a & 0xf0) | (val & 0x0f));
            this->F = (byte)(flag_SZP[this->A] | this->c);
        }

        void Rst()
        {
            byte pc = (byte)(this->instr.opcode & 0x38);
            WriteMemory(--this->SP, (byte)(this->PC >> 8));
            WriteMemory(--this->SP, (byte)(this->PC & 0xff));
            this->PC = pc; // jump to page-0 address
        }

        void Sbc_a_r()
        {
            byte d = this->reg[(int)this->instr._register];
            byte result = (byte)((this->A - d - this->c) & 0xff);
            this->F = Get_SZHVC_sub_flag(this->A, result, this->c);
            this->A = result;
        }

        void Sbc_a_n()
        {
            byte d = this->instr.immediate;
            byte result = (byte)((this->A - d - this->c) & 0xff);
            this->F = Get_SZHVC_sub_flag(this->A, result, this->c);
            this->A = result;
        }

        void Sbc_a_hx()
        {
            byte d = this->HX;
            byte result = (byte)((this->A - d - this->c) & 0xff);
            this->F = Get_SZHVC_sub_flag(this->A, result, this->c);
            this->A = result;
        }

        void Sbc_a_lx()
        {
            byte d = this->LX;
            byte result = (byte)((this->A - d - this->c) & 0xff);
            this->F = Get_SZHVC_sub_flag(this->A, result, this->c);
            this->A = result;
        }

        void Sbc_a_hy()
        {
            byte d = this->HY;
            byte result = (byte)((this->A - d - this->c) & 0xff);
            this->F = Get_SZHVC_sub_flag(this->A, result, this->c);
            this->A = result;
        }

        void Sbc_a_ly()
        {
            byte d = this->LY;
            byte result = (byte)((this->A - d - this->c) & 0xff);
            this->F = Get_SZHVC_sub_flag(this->A, result, this->c);
            this->A = result;
        }

        void Sbc_a_hl_ind()
        {
            byte val = ReadMemory(this->HL);
            byte result = (byte)((this->A - val - this->c) & 0xff);
            this->F = Get_SZHVC_sub_flag(this->A, result, this->c);
            this->A = result;
        }

        void Sbc_a_ix_d_ind()
        {
            ushort addr = (ushort)(this->IX + this->instr.displacement);
            byte val = ReadMemory(addr);
            byte result = (byte)((this->A - val - this->c) & 0xff);
            this->F = Get_SZHVC_sub_flag(this->A, result, this->c);
            this->A = result;
        }

        void Sbc_a_iy_d_ind()
        {
            ushort addr = (ushort)(this->IY + this->instr.displacement);
            byte val = ReadMemory(addr);
            byte result = (byte)((this->A - val - this->c) & 0xff);
            this->F = Get_SZHVC_sub_flag(this->A, result, this->c);
            this->A = result;
        }

        void Sbc_hl_bc()
        {
            ushort d = this->BC;
            int result = this->HL - d - this->c;
            SetSbcOpFlags(this->HL, d, result);
            this->set_HL(result & 0xffff);
        }

        void Sbc_hl_de()
        {
            ushort d = this->DE;
            int result = this->HL - d - this->c;
            SetSbcOpFlags(this->HL, d, result);
            this->set_HL(result & 0xffff);
        }

        void Sbc_hl_hl()
        {
            ushort d = this->HL;
            int result = this->HL - d - this->c;
            SetSbcOpFlags(this->HL, d, result);
            this->set_HL(result & 0xffff);
        }

        void Sbc_hl_sp()
        {
            ushort d = this->SP;
            int result = this->HL - d - this->c;
            SetSbcOpFlags(this->HL, d, result);
            this->set_HL(result & 0xffff);
        }

        void Scf()
        {
            this->F = (byte)((this->F & (S_mask | Z_mask | PV_mask)) | C_mask);
        }

        void Set_bit_r()
        {
            int bit = (this->instr.opcode >> 3) & 0x07;
            this->reg[(int)this->instr._register] |= (byte)(1 << bit);
        }

        void Set_bit_hl_ind()
        {
            int bit = (this->instr.opcode >> 3) & 0x07;
            WriteMemory(this->HL, (byte)(ReadMemory(this->HL) | (1 << bit)));
        }

        byte Set_bit_ix()
        {
            ushort addr = (ushort)(this->IX + this->instr.displacement);
            int bit = (this->instr.opcode >> 3) & 0x07;
            byte val = ReadMemory(addr);
            byte result = (byte)(val | (byte)(1 << bit));
            WriteMemory(addr, result);
            return result;
        }

        byte Set_bit_iy()
        {
            ushort addr = (ushort)(this->IY + this->instr.displacement);
            int bit = (this->instr.opcode >> 3) & 0x07;
            byte val = ReadMemory(addr);
            byte result = (byte)(val | (byte)(1 << bit));
            WriteMemory(addr, result);
            return result;
        }

        void Set_bit_ix_d_ind()
        {
            Set_bit_ix();
        }

        void Set_bit_ix_d_ind_Ld_a()
        {
            this->A = Set_bit_ix();
        }

        void Set_bit_ix_d_ind_Ld_b()
        {
            this->B = Set_bit_ix();
        }

        void Set_bit_ix_d_ind_Ld_c()
        {
            this->C = Set_bit_ix();
        }

        void Set_bit_ix_d_ind_Ld_d()
        {
            this->D = Set_bit_ix();
        }

        void Set_bit_ix_d_ind_Ld_e()
        {
            this->E = Set_bit_ix();
        }

        void Set_bit_ix_d_ind_Ld_h()
        {
            this->H = Set_bit_ix();
        }

        void Set_bit_ix_d_ind_Ld_l()
        {
            this->L = Set_bit_ix();
        }

        void Set_bit_iy_d_ind()
        {
            Set_bit_iy();
        }

        void Set_bit_iy_d_ind_Ld_a()
        {
            this->A = Set_bit_iy();
        }

        void Set_bit_iy_d_ind_Ld_b()
        {
            this->B = Set_bit_iy();
        }

        void Set_bit_iy_d_ind_Ld_c()
        {
            this->C = Set_bit_iy();
        }

        void Set_bit_iy_d_ind_Ld_d()
        {
            this->D = Set_bit_iy();
        }

        void Set_bit_iy_d_ind_Ld_e()
        {
            this->E = Set_bit_iy();
        }

        void Set_bit_iy_d_ind_Ld_h()
        {
            this->H = Set_bit_iy();
        }

        void Set_bit_iy_d_ind_Ld_l()
        {
            this->L = Set_bit_iy();
        }

        void Sla_r()
        {
            byte val = this->reg[(int)this->instr._register];
            byte newCarry = (byte)((val & 0x80) >> 7);
            byte result = (byte)((val & 0x7f) << 1);
            this->reg[(int)this->instr._register] = result;
            this->F = (byte)(flag_SZP[result] | newCarry);
        }

        void Sla_hl_ind()
        {
            byte val = ReadMemory(this->HL);
            byte newCarry = (byte)((val & 0x80) >> 7);
            byte result = (byte)((val & 0x7f) << 1);
            WriteMemory(this->HL, result);
            this->F = (byte)(flag_SZP[result] | newCarry);
        }

        byte Sla_ind(ushort addr)
        {
            byte val = ReadMemory(addr);
            byte newCarry = (byte)((val & 0x80) >> 7);
            byte result = (byte)((val & 0x7f) << 1);
            WriteMemory(addr, result);
            this->F = (byte)(flag_SZP[result] | newCarry);
            return result;
        }

        void Sla_ix_d_ind()
        {
            Sla_ind((ushort)(this->IX + this->instr.displacement));
        }

        void Sla_ix_d_ind_Ld_a()
        {
            this->A = Sla_ind((ushort)(this->IX + this->instr.displacement));
        }

        void Sla_ix_d_ind_Ld_b()
        {
            this->B = Sla_ind((ushort)(this->IX + this->instr.displacement));
        }

        void Sla_ix_d_ind_Ld_c()
        {
            this->C = Sla_ind((ushort)(this->IX + this->instr.displacement));
        }

        void Sla_ix_d_ind_Ld_d()
        {
            this->D = Sla_ind((ushort)(this->IX + this->instr.displacement));
        }

        void Sla_ix_d_ind_Ld_e()
        {
            this->E = Sla_ind((ushort)(this->IX + this->instr.displacement));
        }

        void Sla_ix_d_ind_Ld_h()
        {
            this->H = Sla_ind((ushort)(this->IX + this->instr.displacement));
        }

        void Sla_ix_d_ind_Ld_l()
        {
            this->L = Sla_ind((ushort)(this->IX + this->instr.displacement));
        }

        void Sla_iy_d_ind()
        {
            Sla_ind((ushort)(this->IY + this->instr.displacement));
        }

        void Sla_iy_d_ind_Ld_a()
        {
            this->A = Sla_ind((ushort)(this->IY + this->instr.displacement));
        }

        void Sla_iy_d_ind_Ld_b()
        {
            this->B = Sla_ind((ushort)(this->IY + this->instr.displacement));
        }

        void Sla_iy_d_ind_Ld_c()
        {
            this->C = Sla_ind((ushort)(this->IY + this->instr.displacement));
        }

        void Sla_iy_d_ind_Ld_d()
        {
            this->D = Sla_ind((ushort)(this->IY + this->instr.displacement));
        }

        void Sla_iy_d_ind_Ld_e()
        {
            this->E = Sla_ind((ushort)(this->IY + this->instr.displacement));
        }

        void Sla_iy_d_ind_Ld_h()
        {
            this->H = Sla_ind((ushort)(this->IY + this->instr.displacement));
        }

        void Sla_iy_d_ind_Ld_l()
        {
            this->L = Sla_ind((ushort)(this->IY + this->instr.displacement));
        }
        
        void Sra_r()
        {
            byte val = this->reg[(int)this->instr._register];
            byte newCarry = (byte)(val & 0x01);
            byte result = (byte)((val >> 1) | (val & 0x80));
            this->reg[(int)this->instr._register] = result;
            this->F = (byte)(flag_SZP[result] | newCarry);
        }

        void Sra_hl_ind()
        {
            byte val = ReadMemory(this->HL);
            byte newCarry = (byte)(val & 0x01);
            byte result = (byte)((val >> 1) | (val & 0x80));
            WriteMemory(this->HL, result);
            this->F = (byte)(flag_SZP[result] | newCarry);
        }

        byte Sra_ind(ushort addr)
        {
            byte val = ReadMemory(addr);
            byte newCarry = (byte)(val & 0x01);
            byte result = (byte)((val >> 1) | (val & 0x80));
            WriteMemory(addr, result);
            this->F = (byte)(flag_SZP[result] | newCarry);
            return result;
        }

        void Sra_ix_d_ind()
        {
            Sra_ind((ushort)(this->IX + this->instr.displacement));
        }

        void Sra_ix_d_ind_Ld_a()
        {
            this->A = Sra_ind((ushort)(this->IX + this->instr.displacement));
        }

        void Sra_ix_d_ind_Ld_b()
        {
            this->B = Sra_ind((ushort)(this->IX + this->instr.displacement));
        }

        void Sra_ix_d_ind_Ld_c()
        {
            this->C = Sra_ind((ushort)(this->IX + this->instr.displacement));
        }

        void Sra_ix_d_ind_Ld_d()
        {
            this->D = Sra_ind((ushort)(this->IX + this->instr.displacement));
        }

        void Sra_ix_d_ind_Ld_e()
        {
            this->E = Sra_ind((ushort)(this->IX + this->instr.displacement));
        }

        void Sra_ix_d_ind_Ld_h()
        {
            this->H = Sra_ind((ushort)(this->IX + this->instr.displacement));
        }

        void Sra_ix_d_ind_Ld_l()
        {
            this->L = Sra_ind((ushort)(this->IX + this->instr.displacement));
        }

        void Sra_iy_d_ind()
        {
            Sra_ind((ushort)(this->IY + this->instr.displacement));
        }

        void Sra_iy_d_ind_Ld_a()
        {
            this->A = Sra_ind((ushort)(this->IY + this->instr.displacement));
        }

        void Sra_iy_d_ind_Ld_b()
        {
            this->B = Sra_ind((ushort)(this->IY + this->instr.displacement));
        }

        void Sra_iy_d_ind_Ld_c()
        {
            this->C = Sra_ind((ushort)(this->IY + this->instr.displacement));
        }

        void Sra_iy_d_ind_Ld_d()
        {
            this->D = Sra_ind((ushort)(this->IY + this->instr.displacement));
        }

        void Sra_iy_d_ind_Ld_e()
        {
            this->E = Sra_ind((ushort)(this->IY + this->instr.displacement));
        }

        void Sra_iy_d_ind_Ld_h()
        {
            this->H = Sra_ind((ushort)(this->IY + this->instr.displacement));
        }

        void Sra_iy_d_ind_Ld_l()
        {
            this->L = Sra_ind((ushort)(this->IY + this->instr.displacement));
        }

        void Sll_r()
        {
            byte val = this->reg[(int)this->instr._register];
            byte newCarry = (byte)((val & 0x80) >> 7);
            byte result = (byte)(((val & 0x7f) << 1) | 0x01); // undocumented
            this->reg[(int)this->instr._register] = result;
            this->F = (byte)(flag_SZP[result] | newCarry);
        }

        void Sll_hl_ind()
        {
            byte val = ReadMemory(this->HL);
            byte newCarry = (byte)((val & 0x80) >> 7);
            byte result = (byte)(((val & 0x7f) << 1) | 0x01); // undocumented
            WriteMemory(this->HL, result);
            this->F = (byte)(flag_SZP[result] | newCarry);
        }

        byte Sll_ind(ushort addr)
        {
            byte val = ReadMemory(addr);
            byte newCarry = (byte)((val & 0x80) >> 7);
            byte result = (byte)(((val & 0x7f) << 1) | 0x01); // undocumented
            WriteMemory(addr, result);
            this->F = (byte)(flag_SZP[result] | newCarry);
            return result;
        }

        void Sll_ix_d_ind()
        {
            Sll_ind((ushort)(this->IX + this->instr.displacement));
        }

        void Sll_ix_d_ind_Ld_a()
        {
            this->A = Sll_ind((ushort)(this->IX + this->instr.displacement));
        }

        void Sll_ix_d_ind_Ld_b()
        {
            this->B = Sll_ind((ushort)(this->IX + this->instr.displacement));
        }

        void Sll_ix_d_ind_Ld_c()
        {
            this->C = Sll_ind((ushort)(this->IX + this->instr.displacement));
        }

        void Sll_ix_d_ind_Ld_d()
        {
            this->D = Sll_ind((ushort)(this->IX + this->instr.displacement));
        }

        void Sll_ix_d_ind_Ld_e()
        {
            this->E = Sll_ind((ushort)(this->IX + this->instr.displacement));
        }

        void Sll_ix_d_ind_Ld_h()
        {
            this->H = Sll_ind((ushort)(this->IX + this->instr.displacement));
        }

        void Sll_ix_d_ind_Ld_l()
        {
            this->L = Sll_ind((ushort)(this->IX + this->instr.displacement));
        }

        void Sll_iy_d_ind()
        {
            Sll_ind((ushort)(this->IY + this->instr.displacement));
        }

        void Sll_iy_d_ind_Ld_a()
        {
            this->A = Sll_ind((ushort)(this->IY + this->instr.displacement));
        }

        void Sll_iy_d_ind_Ld_b()
        {
            this->B = Sll_ind((ushort)(this->IY + this->instr.displacement));
        }

        void Sll_iy_d_ind_Ld_c()
        {
            this->C = Sll_ind((ushort)(this->IY + this->instr.displacement));
        }

        void Sll_iy_d_ind_Ld_d()
        {
            this->D = Sll_ind((ushort)(this->IY + this->instr.displacement));
        }

        void Sll_iy_d_ind_Ld_e()
        {
            this->E = Sll_ind((ushort)(this->IY + this->instr.displacement));
        }

        void Sll_iy_d_ind_Ld_h()
        {
            this->H = Sll_ind((ushort)(this->IY + this->instr.displacement));
        }

        void Sll_iy_d_ind_Ld_l()
        {
            this->L = Sll_ind((ushort)(this->IY + this->instr.displacement));
        }

        void Srl_r()
        {
            byte val = this->reg[(int)this->instr._register];
            byte newCarry = (byte)(val & 0x01);
            byte result = (byte)(val >> 1);
            this->reg[(int)this->instr._register] = result;
            this->F = (byte)(flag_SZP[result] | newCarry);
        }

        void Srl_hl_ind()
        {
            byte val = ReadMemory(this->HL);
            byte newCarry = (byte)(val & 0x01);
            byte result = (byte)(val >> 1);
            WriteMemory(this->HL, result);
            this->F = (byte)(flag_SZP[result] | newCarry);
        }

        byte Srl_ind(ushort addr)
        {
            byte val = ReadMemory(addr);
            byte newCarry = (byte)(val & 0x01);
            byte result = (byte)(val >> 1);
            WriteMemory(addr, result);
            this->F = (byte)(flag_SZP[result] | newCarry);
            return result;
        }

        void Srl_ix_d_ind()
        {
            Srl_ind((ushort)(this->IX + this->instr.displacement));
        }

        void Srl_ix_d_ind_Ld_a()
        {
            this->A = Srl_ind((ushort)(this->IX + this->instr.displacement));
        }

        void Srl_ix_d_ind_Ld_b()
        {
            this->B = Srl_ind((ushort)(this->IX + this->instr.displacement));
        }

        void Srl_ix_d_ind_Ld_c()
        {
            this->C = Srl_ind((ushort)(this->IX + this->instr.displacement));
        }

        void Srl_ix_d_ind_Ld_d()
        {
            this->D = Srl_ind((ushort)(this->IX + this->instr.displacement));
        }

        void Srl_ix_d_ind_Ld_e()
        {
            this->E = Srl_ind((ushort)(this->IX + this->instr.displacement));
        }

        void Srl_ix_d_ind_Ld_h()
        {
            this->H = Srl_ind((ushort)(this->IX + this->instr.displacement));
        }

        void Srl_ix_d_ind_Ld_l()
        {
            this->L = Srl_ind((ushort)(this->IX + this->instr.displacement));
        }

        void Srl_iy_d_ind()
        {
            Srl_ind((ushort)(this->IY + this->instr.displacement));
        }

        void Srl_iy_d_ind_Ld_a()
        {
            this->A = Srl_ind((ushort)(this->IY + this->instr.displacement));
        }

        void Srl_iy_d_ind_Ld_b()
        {
            this->B = Srl_ind((ushort)(this->IY + this->instr.displacement));
        }

        void Srl_iy_d_ind_Ld_c()
        {
            this->C = Srl_ind((ushort)(this->IY + this->instr.displacement));
        }

        void Srl_iy_d_ind_Ld_d()
        {
            this->D = Srl_ind((ushort)(this->IY + this->instr.displacement));
        }

        void Srl_iy_d_ind_Ld_e()
        {
            this->E = Srl_ind((ushort)(this->IY + this->instr.displacement));
        }

        void Srl_iy_d_ind_Ld_h()
        {
            this->H = Srl_ind((ushort)(this->IY + this->instr.displacement));
        }

        void Srl_iy_d_ind_Ld_l()
        {
            this->L = Srl_ind((ushort)(this->IY + this->instr.displacement));
        }

        void Sub_a_r()
        {
            byte d = this->reg[(int)this->instr._register];
            byte result = (byte)((this->A - d) & 0xff);
            this->F = (byte)(Get_SZHVC_sub_flag(this->A, result, 0) | N_mask);
            this->A = result;
        }

        void Sub_a_n()
        {
            byte result = (byte)((this->A - this->instr.immediate) & 0xff);
            this->F = (byte)(Get_SZHVC_sub_flag(this->A, result, 0) | N_mask);
            this->A = result;
        }

        void Sub_a_hx()
        {
            byte result = (byte)((this->A - this->HX) & 0xff);
            this->F = (byte)(Get_SZHVC_sub_flag(this->A, result, 0) | N_mask);
            this->A = result;
        }

        void Sub_a_lx()
        {
            byte result = (byte)((this->A - this->LX) & 0xff);
            this->F = (byte)(Get_SZHVC_sub_flag(this->A, result, 0) | N_mask);
            this->A = result;
        }

        void Sub_a_hy()
        {
            byte result = (byte)((this->A - this->HY) & 0xff);
            this->F = (byte)(Get_SZHVC_sub_flag(this->A, result, 0) | N_mask);
            this->A = result;
        }

        void Sub_a_ly()
        {
            byte result = (byte)((this->A - this->LY) & 0xff);
            this->F = (byte)(Get_SZHVC_sub_flag(this->A, result, 0) | N_mask);
            this->A = result;
        }

        void Sub_a_hl_ind()
        {
            byte val = ReadMemory(this->HL);
            byte result = (byte)((this->A - val) & 0xff);
            this->F = (byte)(Get_SZHVC_sub_flag(this->A, result, 0) | N_mask);
            this->A = result;
        }

        void Sub_a_ix_d_ind()
        {
            ushort addr = (ushort)(this->IX + this->instr.displacement);
            byte val = ReadMemory(addr);
            byte result = (byte)((this->A - val) & 0xff);
            this->F = (byte)(Get_SZHVC_sub_flag(this->A, result, 0) | N_mask);
            this->A = result;
        }

        void Sub_a_iy_d_ind()
        {
            ushort addr = (ushort)(this->IY + this->instr.displacement);
            byte val = ReadMemory(addr);
            byte result = (byte)((this->A - val) & 0xff);
            this->F = (byte)(Get_SZHVC_sub_flag(this->A, result, 0) | N_mask);
            this->A = result;
        }

        void Xor_a_r()
        {
            this->A = (byte)(this->A ^ this->reg[(int)this->instr._register]);
            this->F = flag_SZP[this->A];
        }

        void Xor_a_n()
        {
            this->A = (byte)(this->A ^ this->instr.immediate);
            this->F = flag_SZP[this->A];
        }

        void Xor_a_hx()
        {
            this->A = (byte)(this->A ^ this->HX);
            this->F = flag_SZP[this->A];
        }

        void Xor_a_lx()
        {
            this->A = (byte)(this->A ^ this->LX);
            this->F = flag_SZP[this->A];
        }

        void Xor_a_hy()
        {
            this->A = (byte)(this->A ^ this->HY);
            this->F = flag_SZP[this->A];
        }

        void Xor_a_ly()
        {
            this->A = (byte)(this->A ^ this->LY);
            this->F = flag_SZP[this->A];
        }

        void Xor_a_hl_ind()
        {
            byte val = ReadMemory(this->HL);
            this->A = (byte)(this->A ^ val);
            this->F = flag_SZP[this->A];
        }

        void Xor_a_ix_d_ind()
        {
            ushort addr = (ushort)(this->IX + this->instr.displacement);
            byte val = ReadMemory(addr);
            this->A = (byte)(this->A ^ val);
            this->F = flag_SZP[this->A];
        }

        void Xor_a_iy_d_ind()
        {
            ushort addr = (ushort)(this->IY + this->instr.displacement);
            byte val = ReadMemory(addr);
            this->A = (byte)(this->A ^ val);
            this->F = flag_SZP[this->A];
        }

	private:
		void InitFlagArrays();
		void ResetStatus();

        static OpcodeInfo fetchDecodeTable[7][256];

		// processor status
        ushort IX;
        ushort IY;
        ushort SP;
        ushort PC;

        byte reg[8];
        byte altReg[8];
        byte I;
        byte R;

        bool IFF1;
        bool IFF2;
        InterruptMode interruptMode;
        Emulator::LineState nmiState;
        bool isNmiPending;
        uint irqVector;
        bool AfterEI;

		Emulator::IrqDeviceInputLine irqLine;
		Emulator::NmiDeviceInputLine nmiLine;
		Emulator::HaltDeviceInputLine haltLine;
		Emulator::ResetDeviceInputLine resetLine;

		//
		//Emulator::Cpu& cpu;
		std::string tag;
        double clockFrequency; // Hz
        bool halt;
        byte* ram; // 64K
        Emulator::IOHandler& ioHandler;
        Emulator::IMemoryHandler* memoryHandler;
		bool aborted;
        INT64 cyclesLeft;
		INT64 cyclesInCurrentTimeslice;
		Instruction instr; // for the sake of speed, reuse the same Instruction
	};

} // Z80