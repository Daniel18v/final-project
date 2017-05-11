


#define VERBOSE 1
#define LOG(x)  do { if (VERBOSE) logerror x; } while (0)

#define M1_MASTER_CLOCK (XTAL_8MHz)
#define M1_DUART_CLOCK  (XTAL_3_6864MHz)

#include "cpu/m6809/m6809.h"
#include "machine/i8279.h"

#include "video/awpvid.h"       //Fruit Machines Only
#include "machine/6821pia.h"
#include "machine/mc68681.h"
#include "machine/meters.h"
#include "machine/roc10937.h"   // vfd
#include "machine/steppers.h"   // stepper motor
#include "sound/ay8910.h"
#include "sound/2413intf.h"
#include "sound/okim6376.h"
#include "machine/nvram.h"

class maygay1b_state : public driver_device
{
public:
	maygay1b_state(const machine_config &mconfig, device_type type, const char *tag)
		: driver_device(mconfig, type, tag),
		m_maincpu(*this, "maincpu"),
		m_vfd(*this, "vfd"),
		m_ay(*this, "aysnd"),
		m_msm6376(*this, "msm6376"),
		m_duart68681(*this, "duart68681"),
		m_sw1_port(*this, "SW1"),
		m_sw2_port(*this, "SW2"),
		m_s2_port(*this, "STROBE2"),
		m_s3_port(*this, "STROBE3"),
		m_s4_port(*this, "STROBE4"),
		m_s5_port(*this, "STROBE5"),
		m_s6_port(*this, "STROBE6"),
		m_s7_port(*this, "STROBE7")
	{}

	required_device<cpu_device> m_maincpu;
	optional_device<s16lf01_t> m_vfd;
	required_device<ay8910_device> m_ay;
	optional_device<okim6376_device> m_msm6376;
	required_device<mc68681_device> m_duart68681;
	required_ioport m_sw1_port;
	required_ioport m_sw2_port;
	required_ioport m_s2_port;
	required_ioport m_s3_port;
	required_ioport m_s4_port;
	required_ioport m_s5_port;
	required_ioport m_s6_port;
	required_ioport m_s7_port;

	UINT8 m_lamppos;
	int m_lamp_strobe;
	int m_old_lamp_strobe;
	int m_lamp_strobe2;
	int m_old_lamp_strobe2;
	int m_RAMEN;
	int m_ALARMEN;
	int m_PSUrelay;
	int m_WDOG;
	int m_SRSEL;
	int m_NMIENABLE;
	int m_meter;
	TIMER_DEVICE_CALLBACK_MEMBER( maygay1b_nmitimer_callback );
	UINT8 m_Lamps[256];
	int m_optic_pattern;
	DECLARE_WRITE8_MEMBER(scanlines_w);
	DECLARE_WRITE8_MEMBER(lamp_data_w);
	DECLARE_WRITE8_MEMBER(lamp_data_2_w);
	DECLARE_READ8_MEMBER(kbd_r);
	DECLARE_WRITE8_MEMBER(reel12_w);
	DECLARE_WRITE8_MEMBER(reel34_w);
	DECLARE_WRITE8_MEMBER(reel56_w);
	DECLARE_WRITE8_MEMBER(m1_latch_w);
	DECLARE_WRITE8_MEMBER(latch_ch2_w);
	DECLARE_READ8_MEMBER(latch_st_hi);
	DECLARE_READ8_MEMBER(latch_st_lo);
	DECLARE_WRITE8_MEMBER(m1ab_no_oki_w);
	DECLARE_WRITE8_MEMBER(m1_pia_porta_w);
	DECLARE_WRITE8_MEMBER(m1_pia_portb_w);
	DECLARE_WRITE8_MEMBER(m1_lockout_w);
	DECLARE_WRITE8_MEMBER(m1_meter_w);
	DECLARE_READ8_MEMBER(m1_meter_r);
	DECLARE_READ8_MEMBER(m1_firq_clr_r);
	DECLARE_READ8_MEMBER(m1_firq_trg_r);
	DECLARE_WRITE_LINE_MEMBER(duart_irq_handler);
	DECLARE_READ8_MEMBER(m1_duart_r);
	DECLARE_DRIVER_INIT(m1);
	virtual void machine_start();
	virtual void machine_reset();
	void m1_stepper_reset();
};
