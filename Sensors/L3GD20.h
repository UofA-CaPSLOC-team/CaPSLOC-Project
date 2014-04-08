/*
 * L3GD20.h
 *
 *  Created on: Nov 25, 2013
 *      Author: curtis
 */

#ifndef L3GD20_H_
#define L3GD20_H_

#include <stdint.h>
#include <unistd.h>
#include <time.h>
#include "../WiringPi/wiringPiI2C.h"


/* Author: Curtis Hendrix.
 *
 * 2013/11/01 - Initial writing.
 *
 *
 * The L3GD20 is a 3 axis gyroscope, capable of up to 2000dps
 * sensitivity.  This class manages communication to and
 * operation of the L3GD20 gyroscope.  The sensitivity of the
 * gryoscope is described in dps, or degrees per second.
 * Counter-clockwise rotation produces positive values.
 */
class L3GD20
{
public:
	/*****************  Enumerations *****************/
	/* Register enumeration contains all the addressable registers
	 * on the L3GD20 gyroscope, corresponding to their name and address
	 * as shown in the datasheet. This is not meant to be a replacement
	 * for the datasheet; rather, it's intended to be used as supplemental
	 * information to refresh someone already familiar with the L3GD20.
	 */
	enum Register
	{
		/* Contains the device identification byte.  Value is 0xD4.
		 */
		WHO_AM_I = 0x0F,

		/* Bit 7:6 (DR1-DR0) control the output data rate selection. Default 00
		 * 		See Table 21.
		 * Bit 5:4 (BW1-BW0) control the bandwidth selection. Default 00
		 * 		See Table 21.
		 * Bit 3 (PD) controls the power state.  Default 0
		 * 		0: power down.  1: normal mode or sleep mode
		 * Bit 2 (Zen) is the Z axis enable. Default 1
		 * 		0: Z axis disabled.  1: Z axis enabled.
		 * Bit 1 (Yen) is the Y axis enable.  Default 1
		 * 		0: Y axis disabled.  1: Y axis enabled.
		 */
		CTRL_REG1 = 0x20,

		/* Bit 7:6 must be set to 0 for proper operation
		 * Bit 5:4 (HPM1-HPM0)  High-pass filter mode selection. Default 00
		 * 		See Table 25.
		 * Bit 3:0 (HPCF3-HPCF0) High pass filter frequency selection. Default 0000
		 * 		See Table 26 .
		 */
		CTRL_REG2 = 0x21,

		/* Bit 7 (I1_Int1) Interrupt enable on INT1 pin. Default 0
		 * 		0: disable.  1: enable.
		 * Bit 6 (I1_Boot) Boot status available on IN1. Default 0
		 * 		0: disable.  1: enable.
		 * Bit 5 (H_Lactive) Interrupt active configuration on IN1. Default 0
		 * 		0: high. 1: low.
		 * Bit 4 (PP_OD) Push-pull/Open drain. Default 0
		 * 		0: push-pull. 1: open drain.
		 * Bit 3 (I2_DRDY) Date-ready on DRDY/INT2.  Default 0.
		 * 		0: disable. 1: enable.
		 * Bit 2 (I2_WTM) FIFO watermark interrupt on DRDY/INT2. Default 0
		 * 		0: disable. 1: enable.
		 * Bit 1 (I2_ORun) FIFO overrun interrupt on DRDY/INT2. Default 0
		 * 		0: disable. 1: enable.
		 * Bit 0 (I2_Empty) FIFO empty interrupt on DRDY/INT2. Default 0
		 */
		CTRL_REG3 = 0x22,

		/* Bit 7 (BDU) Block data update. Default 0
		 * 		0: continuous update. 1: output registers not updated until MSb and LSb reading.
		 * Bit 6 (BLE) Big/little endian data selection.  Default 0
		 * 		0: data LSb @ lower address. 1: data MSb @ lower address.
		 * Bit 5:4 (FS1-FS0) Full scale selection.  Default 00
		 * 		00: 250dps. 01: 500dps. 10: 2000dps. 11: 2000dps
		 * Bit 3 Unimplemented
		 * Bit 2:1 must be set to 0 for proper operation
		 * Bit 0 (SIM) SPI serial interface mode selection. Default 0
		 * 		0: 4 wire interface. 1: 3 wire interface.
		 */
		CTRL_REG4 = 0x23,

		/* Bit 7 (BOOT) Reboot memory content.  Default 0
		 * 		0: normal mode. 1: reboot memory content
		 * Bit 6 (FIFO_EN) FIFO enable. Default 0
		 * 		0: FIFO disable. 1: FIFO enable
		 * Bit 5 Unimplemented
		 * Bit 4 (HPen) High pass filter enable. Default 0
		 * 		0: HPF disabled. 1: HPF enabled.  See figure 20.
		 * Bit 3:2 (INT1_Sel1-INT1_Sel0) INT1 selection configuration.  Default 00
		 * 		See Figure 20.
		 * Bit 1:0 (Out_Sel1-Out_Sel0) Out selection configuration. Default 00
		 * 		See Figure 20.
		 */
		CTRL_REG5 = 0x24,

		/* Bit 7:0 (Ref7-Ref0) Reference value for interrupt generation. Default 0x00
		 */
		REFERENCE = 0x25,

		/* Bit 7:0 (Temp7-Temp0) Temperature data (1LSB/deg - 8 bit resolution.
		 * 		Expressed as 2's complement.
		 */
		OUT_TMEP = 0x26,

		/* Bit 7 (ZYXOR) X, Y, Z axis data overrun. Default 0
		 * 		0: no overrun has occurred. 1: new data has overwritten the previous data before it was read.
		 * Bit 6 (ZOR) Z axis data overrun. Default 0
		 * 		0: no overrun has occurred. 1: new data for the Z axis has overwritten the previous data.
		 * Bit 5 (YOR) Y axis data overrun. Default 0
		 * 		0: no overrun has occurred. 1: new data for the Y axis has overwritten the previous data.
		 * Bit 4 (XOR) X axis data overrun. Default 0
		 * 		0: no overrun has occurred. 1: new data for the X axis has overwritten the previous data.
		 * Bit 3 (ZYXDA) X, Y, Z axis new data available. Default 0
		 *		0: a new set of data is not yet available. 1: a new set of data is available.
		 * Bit 2 (ZDA) Z axis new data available. Default 0
		 * 		0: new data for the Z axis is not yet available. 1: new data for the Z axis is available.
		 * Bit 1 (YDA) Y axis new data available. Default 0
		 * 		0: new data for the Y axis is not yet available. 1: new data for the Y axis is available.
		 * Bit 0 (XDA) X axis new data available. Default 0
		 * 		0: new data for the X axis is not yet available. 1: new data for the X axis is available.
		 */
		STATUS_REG = 0x27,

		/* Bit 7:0 lower byte of the X axis angular data rate
		 * 		data word is expressed as 2's complement.
		 */
		OUT_X_L = 0x28,

		/* Bit 7:0 upper byte of the X axis angular data rate
		 * 		data word is expressed as 2's complement.
		 */
		OUT_X_H = 0x29,

		/* Bit 7:0 lower byte of the Y axis angular data rate
		 * 		data word is expressed as 2's complement.
		 */
		OUT_Y_L = 0x2A,

		/* Bit 7:0 upper byte of the Y axis angular data rate
		 * 		data word is expressed as 2's complement
		 */
		OUT_Y_H = 0x2B,

		/* Bit 7:0 lower byte of the Z axis angular data rate
		 * 		data word is expressed as 2's complement
		 */
		OUT_Z_L = 0x2C,

		/* Bit 7:0 upper byte of the Z axis angular data rate
		 * 		data word is expressed as 2's complement
		 */
		OUT_Z_H = 0x2D,

		/* Bit 7:5 (FM2-FM0) FIFO mode selection. Default 00
		 * 		see Table 41.
		 * Bit 4:0 (TWM4-TWM0) FIFO threshold. Watermark level setting
		 */
		FIFO_CTRL_REG = 0x2E,

		/* Bit 7 (WTM) Watermark status
		 * 		0: FIFO filling is lower than WTM level. 1: FIFO filling is equal or higher than WTM level
		 * Bit 6 (OVRN) Overrun bit status
		 * 		0: FIFO is not completely filled. 1: FIFO is completely filled.
		 * Bit 5 (EMPTY) FIFO empty bit
		 * 		0: FIFO is not empty. 1: FIFO is empty.
		 * Bit 4:0 (FSS4-FSS1) FIFO stored data level
		 */
		FIFO_SRC_REG = 0x2F,

		/* Bit 7 (AND/OR) AND/OR combination of interrupt events. Default 0
		 * 		0: OR combination of interrupt events. 1: AND combination of interrupt events.
		 * Bit 6 (LIR) Latch interrupt request. Default 0
		 * 		0: interrupt request not latched. 1: interrupt request latched.
		 * 		cleared by reading INT1_SRC register
		 * Bit 5 (ZHIE) Enable interrupt generation on Z high event. Default 0
		 * 		0: disable interrupt request. 1: enable interrupt request on measured value higher than preset threshold
		 * Bit 4 (ZLIE) Enable interrupt generation on Z low event. Default 0
		 * 		0: disable interrupt request. 1: enable interrupt request on measured value higher than preset threshold
		 * Bit 3 (YHIE) Enable interrupt generation on Y high event. Default 0
		 * 		0: disable interrupt request. 1: enable interrupt request on measured value higher than preset threshold
		 * Bit 2 (YLIE) Enable interrupt generation on Y low event. Default 0
		 * 		0: disable interrupt request. 1: enable interrupt request on measured value higher than preset threshold
		 * Bit 1 (XHIE) Enable interrupt generation on X high event. Default 0
		 * 		0: disable interrupt request. 1: enable interrupt request on measured value higher than preset threshold
		 * Bit 0 (XLIE) Enable interrupt generation on X low event. Default 0
		 * 		0: disable interrupt request. 1: enable interrupt request on measured value higher than preset threshold
		 */
		INT1_CFG = 0x30,

		/* Read only register.  Reading at this address clears INT1_SRC IA bit (and eventually the interrupt signal
		 * on the INT1 pin) and allows the refresh of data in the INT1)SRC register if the latched option was chosen.
		 * Bit 7 Has no value.
		 * 		Read as 0
		 * Bit 6 (IA) Interrupt active. Default 0
		 * 		0: no interrupt has been generated. 1: one or more interrupts have been generated.
		 * Bit 5 (ZH) Z high. Default 0
		 * 		0: no interrupt. 1: Z high event has occurred.
		 * Bit 4 (ZL) Z low. Default 0
		 * 		0: no interrupt. 1: Z low event has occurred.
		 * Bit 3 (YH) Y high. Default 0
		 * 		0: no interrupt. 1: Y high event has occurred.
		 * Bit 2 (YL) Y low. Default 0
		 * 		0: no interrupt. 1: Y low event has occurred.
		 * Bit 1 (XH) X high. Default 0
		 * 		0: no interrupt. 1: X high event has occurred.
		 * Bit 0 (XL) X low. Default 0
		 * 		0: no interrupt. 1: X low event has occurred.
		 */
		INT1_SRC = 0x31,

		/* Bit 7 Unimplemented.
		 * Bit 6:0 (THSX14-THSX8) X high interrupt threshold. Default 0x00
		 */
		INT1_THS_XH = 0x32,

		/* Bit 7:0 (THSX7-THSX0) X low interrupt threshold. Default 0x00
		 */
		INT1_THS_XL = 0x33,

		/* Bit 7 Unimplemented.
		 * Bit 6:0 (THSY14-THSY8) Y high interrupt threshold. Default 0x00
		 */
		INT1_THS_YH = 0x34,

		/* Bit 7:0 (THSY7-THSY0) Y low interrupt threshold. Default 0x00
		 */
		INT1_THS_YL = 0x35,

		/* Bit 7 Unimplemented.
		 * Bit 6:0 (THSZ14-THSZ8) Z high interrupt threshold. Default 0x00
		 */
		INT1_THS_ZH = 0x36,

		/* Bit 7:0 (THSZ7-THSZ0) Z high interrupt threshold. Default 0x00
		 */
		INT1_THS_ZL = 0x37,

		/* Bit 7 (WAIT) Wait enabled. Default 0
		 * 		0: disable. 1: enable
		 * Bit 6:0 (D6-D0) Duration value. Default 0
		 * D6-D0 bits set the minimum duration of the interrupt event to be recognized.
		 * 		Duration steps and maximum values depend on the ODR chosen.
		 * WIAT = 0: the interrupt falls immediately if the signal crosses the selected threshold.
		 * WAIT = 1: if the signal crosses the selected threshold, the interrupt falls only after
		 * 		the duration has counted the number of samples at the selected data rate, written
		 * 		into the duration counter register.
		 */
		INT1_DURATION = 0x38,
	};
	/* FullScale enumeration contains the possible settings for full scale.
	 * The 2000dps selection is arbitrarily set at 0x10.  According to the
	 * datasheet, this can also be set at 0x11 for full scale range of 2000dps.
	 */
	enum FullScale
		{
			_250dps = 0x00,
			_500dps = 0x01,
			_2000dps = 0x02,
		};

	/****************** Constructors *****************/
	/* Default constructor.  Turns on the gyroscope with full scale value of 250dps.
	 * X, Y, Z axis are enabled.  The FIFO is disabled.
	 */
	L3GD20();

	bool isXAxisEnabled();

	bool isYAxisEnabled();

	bool isZAxisEnabled();

	bool isGyroscopeOn();

	bool isI2CHandleValid();

	uint8_t getI2CAddress();

	L3GD20::FullScale getSensitivity();

	uint16_t getXAxisRaw();

	uint16_t getYAxisRaw();

	uint16_t getZAxisRaw();

	float getXAxisRotationSpeed();

	float getYAxisRotationSpeed();

	float getZAxisRotationSpeed();

	void setSensitivity(FullScale fullScale);


private:
	const static uint8_t _defaultI2cAddress = 0x6B;
	uint8_t _i2cAddress;
	int _i2cHandle;
	FullScale _sensitivity;
	float _xAxisOffset;
	float _yAxisOffset;
	float _zAxisOffset;

	float getFullScaleDegreesPerSecond();

	void calibrate();

};


#endif /* L3GD20_H_ */
