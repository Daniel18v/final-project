/*****************************************************************************************

    Maygay M1 A/B driver, (under heavy construction !!!)

    M.A.M.E Core Copyright Nicola Salmoria and the MAME Team,
    used under license from http://mamedev.org

    This only loads the basic stuff - there needs to be more done to make this run.

    The sound ROM + OKIM6376 is on the game plug-in board, so not all games have it
    (although in some cases it is just missing)


    Gladiators
    ----------

    Produttore
        MayGay

    N.revisione
        M1A

    CPU
        on main board:
            1x TMP82C79P-2
            1x S22EA-EF68B21P
            1x EP840034.A-P-80C51AVW
            1x MC68681P
            1x S22EB-EF68B09P
            1x YM2149F
            2x oscillator 8.000MHz
            1x oscillator 12.000MHz

        on piggyback (SA5-029D):
            1x OKIM6376

    ROMs
        on main board:
            1x GAL16V8

        on piggyback (SA5-029D):
            1x AM27C512
            2x M27C4001
            1x GAL16V8

    Note
        on main board:
            1x 26 pins dual line connector (serial pot?)
            1x 2 legs connector (speaker)
            2x 15 legs connector (coin mech, switch matrix)
            3x 10 legs connector (meters, reel index, triacs)
            1x 11 legs connector (spare stepper motors)
            1x 20 legs connector (stepper motors)
            1x 19 legs connector (aux display)
            1x 9 legs connector (lamps)
            1x 17 legs connector (P3)
            1x 24 legs connector (lamps)
            1x 14 legs connector (power supply)
            1x 8 legs connector (control port)
            1x trimmer (volume)
            1x battery (2.4V 100mAh)
            9x red leds
            1x pushbutton
            2x 8 switches dip

        on piggyback (SA5-029D):
            1x 5 legs connector
            3x trimmer


        TODO: I/O is generally a nightmare, probably needs a rebuild at the address level.
              Inputs need a sort out.
              Some games require dongles for security, need to figure this out.
******************************************************************************************/
#include "emu.h"
#include "includes/maygay1b.h"

#include "maygay1b.lh"

///////////////////////////////////////////////////////////////////////////
// called if board is reset ///////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

void maygay1b_state::m1_stepper_reset()
{
	int pattern = 0,i;
	for ( i = 0; i < 6; i++)
	{
		stepper_reset_position(i);
		if ( stepper_optic_state(i) ) pattern |= 1<<i;
	}
	m_optic_pattern = pattern;
}

void maygay1b_state::machine_reset()
{
	m_vfd->reset(); // reset display1
	m1_stepper_reset();
}

///////////////////////////////////////////////////////////////////////////

// IRQ from Duart (hopper?)
WRITE_LINE_MEMBER(maygay1b_state::duart_irq_handler)
{
	m_maincpu->set_input_line(M6809_IRQ_LINE,  state?ASSERT_LINE:CLEAR_LINE);
	LOG(("6809 irq%d \n",state));
}

// FIRQ, related to the sample playback?
READ8_MEMBER( maygay1b_state::m1_firq_trg_r )
{
	static int i = 0xff;
	i ^= 0xff;
	m_maincpu->set_input_line(M6809_FIRQ_LINE, HOLD_LINE);
	LOG(("6809 firq\n"));
	return i;
}

READ8_MEMBER( maygay1b_state::m1_firq_clr_r )
{
	static int i = 0xff;
	i ^= 0xff;
	m_maincpu->set_input_line(M6809_FIRQ_LINE, CLEAR_LINE);
	LOG(("6809 firq clr\n"));
	return i;
}

// NMI is periodic? or triggered by a write?
TIMER_DEVICE_CALLBACK_MEMBER( maygay1b_state::maygay1b_nmitimer_callback )
{
	if (m_NMIENABLE)
	{
		LOG(("6809 nmi\n"));
		m_maincpu->set_input_line(INPUT_LINE_NMI, HOLD_LINE);
	}
}



/***************************************************************************
    6821 PIA
***************************************************************************/

// some games might differ..
WRITE8_MEMBER(maygay1b_state::m1_pia_porta_w)
{
//  printf("m1_pia_porta_w %02x\n",data);

	m_vfd->por(!(data & 0x40));
	m_vfd->data(data & 0x10);
	m_vfd->sclk(data & 0x20);
}

WRITE8_MEMBER(maygay1b_state::m1_pia_portb_w)
{
	int i;
	for (i=0; i<8; i++)
		if ( data & (1 << i) )      output_set_indexed_value("triac", i, data & (1 << i));
}

// input ports for M1 board ////////////////////////////////////////

INPUT_PORTS_START( maygay_m1 )
	PORT_START("SW1")
	PORT_DIPNAME( 0x01, 0x00, "SW101" ) PORT_DIPLOCATION("SW1:01")
	PORT_DIPSETTING(    0x00, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x01, DEF_STR( On  ) )
	PORT_DIPNAME( 0x02, 0x00, "SW102" ) PORT_DIPLOCATION("SW1:02")
	PORT_DIPSETTING(    0x00, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x02, DEF_STR( On  ) )
	PORT_DIPNAME( 0x04, 0x00, "SW103" ) PORT_DIPLOCATION("SW1:03")
	PORT_DIPSETTING(    0x00, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x04, DEF_STR( On  ) )
	PORT_DIPNAME( 0x08, 0x00, "SW104" ) PORT_DIPLOCATION("SW1:04")
	PORT_DIPSETTING(    0x00, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x08, DEF_STR( On  ) )
	PORT_DIPNAME( 0x10, 0x00, "SW105" ) PORT_DIPLOCATION("SW1:05")
	PORT_DIPSETTING(    0x00, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x10, DEF_STR( On  ) )
	PORT_DIPNAME( 0x20, 0x00, "SW106" ) PORT_DIPLOCATION("SW1:06")
	PORT_DIPSETTING(    0x00, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x20, DEF_STR( On  ) )
	PORT_DIPNAME( 0x40, 0x00, "SW107" ) PORT_DIPLOCATION("SW1:07")
	PORT_DIPSETTING(    0x00, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x40, DEF_STR( On  ) )
	PORT_DIPNAME( 0x80, 0x00, "AntiFraud Protection" ) PORT_DIPLOCATION("SW1:08")
	PORT_DIPSETTING(    0x80, DEF_STR( Off  ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )

	PORT_START("SW2")
	PORT_DIPNAME( 0x01, 0x00, "SW201" ) PORT_DIPLOCATION("SW2:01")
	PORT_DIPSETTING(    0x00, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x01, DEF_STR( On  ) )
	PORT_DIPNAME( 0x02, 0x00, "SW202" ) PORT_DIPLOCATION("SW2:02")
	PORT_DIPSETTING(    0x00, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x02, DEF_STR( On  ) )
	PORT_DIPNAME( 0x04, 0x00, "SW203" ) PORT_DIPLOCATION("SW2:03")
	PORT_DIPSETTING(    0x00, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x04, DEF_STR( On  ) )
	PORT_DIPNAME( 0x08, 0x00, "SW204" ) PORT_DIPLOCATION("SW2:04")
	PORT_DIPSETTING(    0x00, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x08, DEF_STR( On  ) )
	PORT_DIPNAME( 0x10, 0x00, "SW205" ) PORT_DIPLOCATION("SW2:05")
	PORT_DIPSETTING(    0x00, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x10, DEF_STR( On  ) )
	PORT_DIPNAME( 0x20, 0x00, "SW206" ) PORT_DIPLOCATION("SW2:06")
	PORT_DIPSETTING(    0x00, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x20, DEF_STR( On  ) )
	PORT_DIPNAME( 0x40, 0x00, "SW207" ) PORT_DIPLOCATION("SW2:07")
	PORT_DIPSETTING(    0x00, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x40, DEF_STR( On  ) )
	PORT_DIPNAME( 0x80, 0x00, "SW208" ) PORT_DIPLOCATION("SW2:08")
	PORT_DIPSETTING(    0x00, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x80, DEF_STR( On  ) )

	PORT_START("STROBE2")
	PORT_BIT( 0x01, IP_ACTIVE_HIGH, IPT_OTHER) PORT_NAME("17")
	PORT_BIT( 0x02, IP_ACTIVE_HIGH, IPT_OTHER) PORT_NAME("18")
	PORT_BIT( 0x04, IP_ACTIVE_HIGH, IPT_OTHER) PORT_NAME("19")
	PORT_BIT( 0x08, IP_ACTIVE_HIGH, IPT_OTHER) PORT_NAME("20")
	PORT_BIT( 0x10, IP_ACTIVE_HIGH, IPT_OTHER) PORT_NAME("21")
	PORT_BIT( 0x20, IP_ACTIVE_HIGH, IPT_OTHER) PORT_NAME("22")
	PORT_BIT( 0x40, IP_ACTIVE_HIGH, IPT_OTHER) PORT_NAME("23")
	PORT_BIT( 0x80, IP_ACTIVE_HIGH, IPT_OTHER) PORT_NAME("24")

	PORT_START("STROBE3")
	PORT_BIT(0x01, IP_ACTIVE_HIGH, IPT_OTHER) PORT_NAME("25")
	PORT_BIT(0x02, IP_ACTIVE_HIGH, IPT_OTHER) PORT_NAME("Hi")
	PORT_BIT(0x04, IP_ACTIVE_HIGH, IPT_OTHER) PORT_NAME("Lo")
	PORT_BIT(0x08, IP_ACTIVE_HIGH, IPT_OTHER) PORT_NAME("28")
	PORT_BIT(0x10, IP_ACTIVE_HIGH, IPT_OTHER) PORT_NAME("29")
	PORT_BIT(0x20, IP_ACTIVE_HIGH, IPT_OTHER) PORT_NAME("30")
	PORT_BIT(0x40, IP_ACTIVE_HIGH, IPT_INTERLOCK) PORT_NAME("Rear Door") PORT_TOGGLE
	PORT_BIT(0x80, IP_ACTIVE_HIGH, IPT_INTERLOCK) PORT_NAME("Cashbox Door")  PORT_CODE(KEYCODE_Q) PORT_TOGGLE

	PORT_START("STROBE4")
	PORT_BIT(0x01, IP_ACTIVE_HIGH, IPT_BUTTON1) PORT_NAME("Hi2")
	PORT_BIT(0x02, IP_ACTIVE_HIGH, IPT_SERVICE) PORT_NAME("Refill Key") PORT_CODE(KEYCODE_R) PORT_TOGGLE
	PORT_BIT(0x04, IP_ACTIVE_HIGH, IPT_SPECIAL)//50p Tube
	PORT_BIT(0x08, IP_ACTIVE_HIGH, IPT_SPECIAL)//100p Tube rear
	PORT_BIT(0x10, IP_ACTIVE_HIGH, IPT_SPECIAL)//100p Tube front
	PORT_BIT(0x20, IP_ACTIVE_HIGH, IPT_UNUSED)
	PORT_BIT(0x40, IP_ACTIVE_HIGH, IPT_UNUSED)
	PORT_BIT(0x80, IP_ACTIVE_HIGH, IPT_UNUSED)

	PORT_START("STROBE5")
	PORT_BIT(0x01, IP_ACTIVE_HIGH, IPT_OTHER) PORT_NAME("49")
	PORT_BIT(0x02, IP_ACTIVE_HIGH, IPT_OTHER) PORT_NAME("50")
	PORT_BIT(0x04, IP_ACTIVE_HIGH, IPT_BUTTON3) PORT_NAME("Cancel")
	PORT_BIT(0x08, IP_ACTIVE_HIGH, IPT_BUTTON4) PORT_NAME("Hold 1")
	PORT_BIT(0x10, IP_ACTIVE_HIGH, IPT_BUTTON5) PORT_NAME("Hold 2")
	PORT_BIT(0x20, IP_ACTIVE_HIGH, IPT_BUTTON6) PORT_NAME("Hold 3")
	PORT_BIT(0x40, IP_ACTIVE_HIGH, IPT_BUTTON7) PORT_NAME("Hold 4")
	PORT_BIT(0x80, IP_ACTIVE_HIGH, IPT_START1)

	PORT_START("STROBE6")
	PORT_SERVICE_NO_TOGGLE(0x01,IP_ACTIVE_HIGH)
	PORT_BIT(0x02, IP_ACTIVE_HIGH, IPT_OTHER) PORT_NAME("58")
	PORT_BIT(0x04, IP_ACTIVE_HIGH, IPT_OTHER) PORT_NAME("59")
	PORT_BIT(0x08, IP_ACTIVE_HIGH, IPT_OTHER) PORT_NAME("60")
	PORT_BIT(0x10, IP_ACTIVE_HIGH, IPT_OTHER) PORT_NAME("61")
	PORT_BIT(0x20, IP_ACTIVE_HIGH, IPT_OTHER) PORT_NAME("62")
	PORT_BIT(0x40, IP_ACTIVE_HIGH, IPT_OTHER) PORT_NAME("63")
	PORT_BIT(0x80, IP_ACTIVE_HIGH, IPT_OTHER) PORT_NAME("64")

	PORT_START("STROBE7")
	PORT_BIT(0x01, IP_ACTIVE_HIGH, IPT_OTHER) PORT_NAME("65")
	PORT_BIT(0x02, IP_ACTIVE_HIGH, IPT_OTHER) PORT_NAME("66")
	PORT_BIT(0x04, IP_ACTIVE_HIGH, IPT_OTHER) PORT_NAME("67")
	PORT_BIT(0x08, IP_ACTIVE_HIGH, IPT_OTHER) PORT_NAME("68")
	PORT_BIT(0x10, IP_ACTIVE_HIGH, IPT_OTHER) PORT_NAME("69")
	PORT_BIT(0x20, IP_ACTIVE_HIGH, IPT_OTHER) PORT_NAME("70")
	PORT_BIT(0x40, IP_ACTIVE_HIGH, IPT_OTHER) PORT_NAME("72")
	PORT_BIT(0x80, IP_ACTIVE_HIGH, IPT_OTHER) PORT_NAME("73")

INPUT_PORTS_END

void maygay1b_state::machine_start()
{
	int i;

// setup 8 mechanical meters ////////////////////////////////////////////
	MechMtr_config(machine(),8);

// setup 6 default 96 half step reels ///////////////////////////////////
	for ( i = 0; i < 6; i++ )
	{
		stepper_config(machine(), i, &starpoint_interface_48step);
	}

}
WRITE8_MEMBER(maygay1b_state::reel12_w)
{
	stepper_update(0, data & 0x0F );
	stepper_update(1, (data>>4) & 0x0F );

	if ( stepper_optic_state(0) ) m_optic_pattern |=  0x01;
	else                          m_optic_pattern &= ~0x01;
	if ( stepper_optic_state(1) ) m_optic_pattern |=  0x02;
	else                          m_optic_pattern &= ~0x02;

	awp_draw_reel(0);
	awp_draw_reel(1);
}

WRITE8_MEMBER(maygay1b_state::reel34_w)
{
	stepper_update(2, data & 0x0F );
	stepper_update(3, (data>>4) & 0x0F );

	if ( stepper_optic_state(2) ) m_optic_pattern |=  0x04;
	else                          m_optic_pattern &= ~0x04;
	if ( stepper_optic_state(3) ) m_optic_pattern |=  0x08;
	else                          m_optic_pattern &= ~0x08;

	awp_draw_reel(2);
	awp_draw_reel(3);
}

WRITE8_MEMBER(maygay1b_state::reel56_w)
{
	stepper_update(4, data & 0x0F );
	stepper_update(5, (data>>4) & 0x0F );

	if ( stepper_optic_state(4) ) m_optic_pattern |=  0x10;
	else                          m_optic_pattern &= ~0x10;
	if ( stepper_optic_state(5) ) m_optic_pattern |=  0x20;
	else                          m_optic_pattern &= ~0x20;

	awp_draw_reel(4);
	awp_draw_reel(5);
}

READ8_MEMBER(maygay1b_state::m1_duart_r)
{
	return ~(m_optic_pattern);
}

WRITE8_MEMBER(maygay1b_state::m1_meter_w)
{
	int i;
	for (i=0; i<8; i++)
	{
		if ( data & (1 << i) )
		{
			MechMtr_update(i, data & (1 << i) );
			m_meter = data;
		}
	}
}

WRITE8_MEMBER(maygay1b_state::m1_latch_w)
{
	switch ( offset )
	{
		case 0: // m_RAMEN
		m_RAMEN = (data & 1);
		break;
		case 1: // AlarmEn
		m_ALARMEN = (data & 1);
		break;
		case 2: // Enable
		//printf("nmi enable %02x\n",data);
		m_NMIENABLE = (data & 1);
		break;
		case 3: // RTS
		{
		}
		break;
		case 4: // PSURelay
		m_PSUrelay = (data & 1);
		break;
		case 5: // WDog
		m_WDOG = (data & 1);
		break;
		case 6: // Srsel
		// this is the ROM banking?
		printf("rom bank %02x\n",data);
		m_SRSEL = (data & 1);
		break;
	}
}

WRITE8_MEMBER(maygay1b_state::latch_ch2_w)
{
	m_msm6376->write(space, 0, data&0x7f);
	m_msm6376->ch2_w(data&0x80);
}

//A strange setup this, the address lines are used to move st to the right level
READ8_MEMBER(maygay1b_state::latch_st_hi)
{
	m_msm6376->st_w(1);
	return 0;
}

READ8_MEMBER(maygay1b_state::latch_st_lo)
{
	m_msm6376->st_w(0);
	return 0;
}

READ8_MEMBER(maygay1b_state::m1_meter_r)
{
	//TODO: Can we just return the AY port A data?
	return m_meter;
}
WRITE8_MEMBER(maygay1b_state::m1_lockout_w)
{
	int i;
	for (i=0; i<6; i++)
	{
		coin_lockout_w(machine(), i, data & (1 << i) );
	}
}

static ADDRESS_MAP_START( m1_memmap, AS_PROGRAM, 8, maygay1b_state )
	AM_RANGE(0x0000, 0x1fff) AM_RAM AM_SHARE("nvram")

	AM_RANGE(0x2000, 0x2000) AM_WRITE(reel12_w)
	AM_RANGE(0x2010, 0x2010) AM_WRITE(reel34_w)
	AM_RANGE(0x2020, 0x2020) AM_WRITE(reel56_w)

	// there is actually an 8279 and an 8051 (which I guess is the MCU?).
	AM_RANGE(0x2030, 0x2030) AM_DEVREADWRITE("i8279", i8279_device, data_r, data_w )
	AM_RANGE(0x2031, 0x2031) AM_DEVREADWRITE("i8279", i8279_device, status_r, cmd_w)

	//8051
	AM_RANGE(0x2040, 0x2040) AM_DEVREADWRITE("i8279_2", i8279_device, data_r, data_w )
	AM_RANGE(0x2041, 0x2041) AM_DEVREADWRITE("i8279_2", i8279_device, status_r, cmd_w)
//  AM_RANGE(0x2050, 0x2050)// SCAN on M1B

	AM_RANGE(0x2070, 0x207f) AM_DEVREADWRITE("duart68681", mc68681_device, read, write )

	AM_RANGE(0x2090, 0x2091) AM_DEVWRITE("aysnd", ay8910_device, data_address_w)
	AM_RANGE(0x20B0, 0x20B0) AM_READ(m1_meter_r)

	AM_RANGE(0x20A0, 0x20A3) AM_DEVWRITE("pia", pia6821_device, write)
	AM_RANGE(0x20A0, 0x20A3) AM_DEVREAD("pia", pia6821_device, read)

	AM_RANGE(0x20C0, 0x20C7) AM_WRITE(m1_latch_w)

	AM_RANGE(0x2400, 0x2401) AM_DEVWRITE("ymsnd", ym2413_device, write)
	AM_RANGE(0x2404, 0x2405) AM_READ(latch_st_lo)
	AM_RANGE(0x2406, 0x2407) AM_READ(latch_st_hi)

	AM_RANGE(0x2410, 0x2410) AM_READ(m1_firq_clr_r)

	AM_RANGE(0x2412, 0x2412) AM_READ(m1_firq_trg_r) // firq, sample playback?

	AM_RANGE(0x2420, 0x2421) AM_WRITE(latch_ch2_w ) // oki

	AM_RANGE(0x2800, 0xffff) AM_ROM
ADDRESS_MAP_END


/*************************************
 *
 *  8279 display/keyboard driver
 *
 *************************************/

WRITE8_MEMBER( maygay1b_state::scanlines_w )
{
	m_lamp_strobe = data;
}

WRITE8_MEMBER( maygay1b_state::lamp_data_w )
{
	//The two A/B ports are merged back into one, to make one row of 8 lamps.

	if (m_old_lamp_strobe != m_lamp_strobe)
	{
		// Because of the nature of the lamping circuit, there is an element of persistance
		// As a consequence, the lamp column data can change before the input strobe without
		// causing the relevant lamps to black out.

		for (int i = 0; i < 8; i++)
		{
			output_set_lamp_value((8*m_lamp_strobe)+i, ((data  & (1 << i)) !=0));
		}
		m_old_lamp_strobe = m_lamp_strobe;
	}

}

READ8_MEMBER( maygay1b_state::kbd_r )
{
	ioport_port * portnames[] = { m_sw1_port, m_s2_port, m_s3_port, m_s4_port, m_s5_port, m_s6_port, m_s7_port, m_sw2_port};
	return (portnames[m_lamp_strobe&0x07])->read();
}

WRITE8_MEMBER( maygay1b_state::lamp_data_2_w )
{
	//The two A/B ports are merged back into one, to make one row of 8 lamps.

	if (m_old_lamp_strobe2 != m_lamp_strobe2)
	{
		// Because of the nature of the lamping circuit, there is an element of persistance
		// As a consequence, the lamp column data can change before the input strobe without
		// causing the relevant lamps to black out.

		for (int i = 0; i < 8; i++)
		{
			output_set_lamp_value((8*m_lamp_strobe)+i+128, ((data  & (1 << i)) !=0));
		}
		m_old_lamp_strobe2 = m_lamp_strobe2;
	}

}

// machine driver for maygay m1 board /////////////////////////////////



MACHINE_CONFIG_START( maygay_m1, maygay1b_state )

	MCFG_CPU_ADD("maincpu", M6809, M1_MASTER_CLOCK/2)
	MCFG_CPU_PROGRAM_MAP(m1_memmap)

	MCFG_MC68681_ADD("duart68681", M1_DUART_CLOCK)
	MCFG_MC68681_IRQ_CALLBACK(WRITELINE(maygay1b_state, duart_irq_handler))
	MCFG_MC68681_INPORT_CALLBACK(READ8(maygay1b_state, m1_duart_r))

	MCFG_DEVICE_ADD("pia", PIA6821, 0)
	MCFG_PIA_WRITEPA_HANDLER(WRITE8(maygay1b_state, m1_pia_porta_w))
	MCFG_PIA_WRITEPB_HANDLER(WRITE8(maygay1b_state, m1_pia_portb_w))

	MCFG_S16LF01_ADD("vfd",0)
	MCFG_SPEAKER_STANDARD_MONO("mono")
	MCFG_SOUND_ADD("aysnd", YM2149, M1_MASTER_CLOCK)
	MCFG_AY8910_PORT_A_WRITE_CB(WRITE8(maygay1b_state, m1_meter_w))
	MCFG_AY8910_PORT_B_WRITE_CB(WRITE8(maygay1b_state, m1_lockout_w))
	MCFG_SOUND_ROUTE(ALL_OUTPUTS, "mono", 1.0)

	MCFG_SOUND_ADD("ymsnd", YM2413, M1_MASTER_CLOCK/4)

	MCFG_SOUND_ADD("msm6376", OKIM6376, M1_MASTER_CLOCK/4) //?
	MCFG_SOUND_ROUTE(ALL_OUTPUTS, "mono", 1.0)

	MCFG_TIMER_DRIVER_ADD_PERIODIC("nmitimer", maygay1b_state, maygay1b_nmitimer_callback, attotime::from_hz(75)) // freq?
	MCFG_DEVICE_ADD("i8279", I8279, M1_MASTER_CLOCK/4)    // unknown clock
	MCFG_I8279_OUT_SL_CB(WRITE8(maygay1b_state, scanlines_w))   // scan SL lines
	MCFG_I8279_OUT_DISP_CB(WRITE8(maygay1b_state, lamp_data_w))     // display A&B
	MCFG_I8279_IN_RL_CB(READ8(maygay1b_state, kbd_r))           // kbd RL lines
	MCFG_DEVICE_ADD("i8279_2", I8279, M1_MASTER_CLOCK/4)        // unknown clock
	MCFG_I8279_OUT_DISP_CB(WRITE8(maygay1b_state, lamp_data_2_w))       // display A&B

	MCFG_NVRAM_ADD_0FILL("nvram")

	MCFG_DEFAULT_LAYOUT(layout_maygay1b)
MACHINE_CONFIG_END








WRITE8_MEMBER(maygay1b_state::m1ab_no_oki_w)
{
	popmessage("write to OKI, but no OKI rom");
}

DRIVER_INIT_MEMBER(maygay1b_state,m1)
{
	//AM_RANGE(0x2420, 0x2421) AM_WRITE(latch_ch2_w ) // oki
	// if there is no OKI region disable writes here, the rom might be missing, so alert user

	UINT8 *okirom = memregion( "msm6376" )->base();

	if (!okirom) {
		m_maincpu->space(AS_PROGRAM).install_write_handler(0x2420, 0x2421, write8_delegate(FUNC(maygay1b_state::m1ab_no_oki_w), this));
	}
	// print out the rom id / header info to give us some hints
	// note this isn't always correct, alley cat has 'Calpsyo' still in the ident string?
	{
		UINT8 *cpu = memregion( "maincpu" )->base();
		int base = 0xff20;
		for (int i=0;i<14;i++)
		{
			for (int j=0;j<16;j++)
			{
				UINT8 rom = cpu[base];

				if ((rom>=0x20) && (rom<0x7f))
				{
					printf("%c", rom);
				}
				else
				{
					printf("*");
				}

				base++;
			}
			printf("\n");
		}
	}
}
