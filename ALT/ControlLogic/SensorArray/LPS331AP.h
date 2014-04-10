/*
 * LPS331AP.h
 *
 *  Created on: Nov 28, 2013
 *      Author: curtis
 */

#ifndef LPS331AP_H_
#define LPS331AP_H_

#include <stdint.h>
#include <cmath>
#include "/opt/WiringPi/wiringPiI2C.h"

class LPS331AP
{
public:
	/* The Register enumeration contains all the addressable
	 * registers on the LPS331AP.  The documentation for each
	 * register in the enumeration is not intended to replace
	 * the datasheet.  Rather, it's intended to provide a
	 * quick and easy reference for those who are already
	 * familiar with the LPS331AP.  If there is a discrepancy
	 * between the data sheet and the comments below, believe
	 * the datasheet.
	 */
	enum Register
	{
		/* Bit 7:0 (REFL7-REFL0) is the lower byte of the reference pressure. Default 0x00
		 */
		REF_P_XL = 0x08,

		/* Bit 7:0 (REFL15-REFL8) is the middle byte of the reference pressure.  Default 0x00
		 */
		REF_P_L = 0x09,

		/* Bit 7:0 (REFL23-REFL16) is the top byte of the reference pressure.  Default 0x00
		 */
		REF_P_H = 0x0A,

		/* Bit 7 (RFU) Unkown function.  Always set to 0.
		 * Bit 6:4 (AVGt2-AVGT0) Temperature resolution configuration.  Default 000
		 * 		See Table 16.
		 * Bit 3:0 (AVGP3-AVGP0) Pressure resolution configuration.  Default 000
		 * 		See Table 15.
		 */
		RES_CONF = 0x10,

		/* Read only register that contains the device identifier. Set to 0xBB.
		 */
		WHO_AM_I = 0x0F,

		/* Bit 7 (PD) Power down control. Default 0
		 * 		0: power-down mode. 1: active mode.
		 * Bit 6:4 (ODR2-ODR0) Output data rate selection. PD must be 0 before changing ODR. Default 000
		 * 		See Table 18
		 * Bit 3 (DIFF_EN) Interrupt circuit enable. Default 0
		 * 		0: interrupt generation disabled. 1: interrupt circuit enabled.
		 * Bit 2 (BDU) Block data update. Default 0
		 * 		0: continuous update. 1: output registers not updated until MSB and LSB reading.
		 * Bit 1 (DELTA_EN) Delta pressure enable.  Default 0
		 * 		0: disable. 1: delta pressure registers enabled.
		 * Bit 0 (SIM) SPI Serial Interface Mode selection. Default 0
		 * 		0: 4-wire interface. 1: 3-wire interface.
		 */
		CTRL_REG1 = 0x20,

		/* Bit 7 (BOOT) Reboot memory content. Default 0
		 * 		0: Normal mode. 1: Reboot memory content.
		 * Bit 6:3 RESERVED
		 * Bit 2 (SWRESET) Software reset. Default 0
		 * 		0: Normal mode. 1: Software reset.
		 * Bit 1 (AUTO_ZERO) Auto zero enable. Default 0
		 * 		0: Normal mode. 1: Autozero enable.
		 * Bit 0 (ONE_SHOT) One shot enable. Default 0
		 * 		0: waiting for start of conversion. 1: start for a new dataset.
		 */
		CTRL_REG2 = 0x21,

		/* Bit 7 (INT_H_L) Interrupt active high, low. Default 0
		 * 		0: Active high. 1: Active low.
		 * Bit 6 (PP_OD) Push-pull/open drain selection on interrupt pads. Default 0
		 * 		0: Push-pull. 1: Open drain.
		 * Bit 5:3 (INT2_S2-INT2_S1) Data signal on INT2 pad control bits. Default 000
		 * 		See Table 19.
		 * Bit 2:0 (INT1_S3-INT1-S1) Data signal on INT1 pad control bits. Default 000
		 * 		See table 19.
		 */
		CTRL_REG3 = 0x22,

		/* Bit 7:3 RESERVED
		 * Bit 2 (LIR) Latch interrupt request into INT_SOURCE register. Default 0
		 * 		0: interrupt request not latched. 1: interrupt request latched.
		 * Bit 1 (PL_E) Enable interrupt generation on differential pressure low event. Default 0
		 * 		0: disable interrupt request. 1: enable interrupt request on measured differential pressure value lower than preset threshold.
		 * Bit 0 (PH_E) Enable interrupt generation on differential pressure high event. Default 0
		 * 		0: disable interrupt request. 1: enable interrupt request on measured differential pressure value higher than preset threshold.
		 */
		INTERRUPT_CFG = 0x23,

		/* Bit 7:3 Set to 0
		 * Bit 2 (IA) Interrupt Active)
		 * 		0: no interrupt has been generated. 1: one or more interrupt events have been generated.
		 * Bit 1 (PL) Differential pressure Low.
		 * 		0: no interrupt has been generated. 1: low differential pressure event has occurred.
		 * Bit 0 (PH) Differential pressure High.
		 * 		0: no interrupt has been generated. 1: high differential pressure event has occurred.
		 */
		INT_SOURCE = 0x24,

		/* Bit 7:0 (THS7-THS0) Threshold pressure LSB. Default 0x00
		 */
		THS_P_L = 0x25,

		/* Bit 7:0 (THS15-THS8) Threshold pressure MSB. Default 0x00
		 */
		THS_P_H = 0x26,

		/* Bit 7:6 Set to 0
		 * Bit 5 (P_OR) Pressure data overrun. Default 0
		 * 		0: no overrun has occurred. 1: new data for pressure has overwritten the previous one.
		 * Bit 4 (T_OR) Temperature data overrun. Default 0
		 * 		0: no overrun has occurred. 1: new data for temperature has overwritten the previous one.
		 * Bit 3:2 Set to 0
		 * Bit 1 (P_DA) Pressure data available. Default 0
		 * 		0: new data for pressure is not yet available. 1: new data for pressure is available.
		 * Bit 0 (T_DA) Temperature data available. Default 0
		 * 		0: new data for temperature is not yet available. 1: new data for temperature is available.
		 */
		STATUS_REG = 0x27,

		/* Bit 7:0 (POUT7:POUT0) Pressure data LSB
		 */
		PRESS_OUT_XL = 0x28,

		/* Bit 7:0 (POUT15-POUT0) Pressure data middle byte.
		 */
		PRESS_OUT_L = 0x29,

		/* Bit 7:0 (POUT23:POUT16) Pressure data MSB.
		 * Pressure output data: POUT(mbar) = ((PRESS_OUT_H << 16)||(PRESS_OUT_L << 8)||(PRESS_OUT_XL))/4096
		 */
		PRESS_OUT_H = 0x2A,

		/* Bit 7:0 (TOUT7-TOUT0) Temperature data LSB
		 */
		TEMP_OUT_L = 0x2B,

		/* Bit 7:0 (TOUT15-TOUT8) Temperature data MSB.
		 * T(degC) = 42.5+((TEMP_OUT_H << 8)||(TEMP_OUT_L))/480
		 */
		TEMP_OUT_H = 0x2C,

		/* Bit 7:1 Reserved
		 * Bit 0 (SELMAIN) Current of operational amplifier selector
		 * 		0: high current during pressure acquisition and low current during temperature acquisition. 1: always high current.
		 */
		AMP_CTRL = 0x2D,

		/* Bit 7:0 (DELTA7-DELTA0) Delta pressure register for one point calibration.
		 */
		DELTA_PRESS_XL = 0x3C,

		/* Bit 7:0 (DELTA15-DELTA8) Delta pressure register for one point calibration.
		 */
		DELTA_PRESS_L = 0x3D,
	};

	/******************** Constructors ******************/
	/*Default constructor. Initializes the pressure sensor with pressure and temperature
	 * enabled, no interrupts, and pressure and temperature output data rate of 25hz.
	 */
	LPS331AP();

	/* Gets the raw pressure from the sensor. Returns ONLY the bit pattern from the
	 * 3 PRESS_OUT registers, and returns them as an unsigned 32 bit.  Note that
	 * the bottom 24 bits contain the actual data.  The top byte does contain valid data.
	 */
	uint32_t getPressureRaw();

	/* Gets the raw temperature from the sensor.  Returns ONLY the bit pattern from
	 * the 2 TEMP_OUT registers, and returns them as an unsigned 16 bit.
	 */
	int16_t getTemperatureRaw();

	/* Gets the pressure from the sensor.  Units are in mBar.
	 */
	float getPressure();

	/* Gets the temperature from the sensor. Units are degrees celcius.
	 */
	float getTemperature();

	float getAltitudeInFeet();

	float getAltitudeInMeters();

	private:
		const static uint8_t _defaultI2cAddress= 0x5D;
		int _i2cHandle;
};


#endif /* LPS331AP_H_ */
