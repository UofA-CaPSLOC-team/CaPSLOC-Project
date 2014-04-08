/*
 * LSM303DLHC.h
 *
 *  Created on: Dec 1, 2013
 *      Author: curtis
 */

#ifndef LSM303_H_
#define LSM303_H_

#include <unistd.h>
#include <stdint.h>
#include "/opt/WiringPi/wiringPiI2C.h"

class LSM303
{
public:
	enum AccRegister
	{
		/* Bit 7:4 (ODR3-ODR0) data rate selection. Default 0
		 * 		See Table 19.
		 * Bit 3 (LPen) Low-power mode enable. Default 0
		 * 		0: normal mode. 1: low power mode
		 * Bit 2 (Zen) Z axis enable. Default 1
		 * 		0: Z axis disabled. 1: Z axis enabled.
		 * Bit 1 (Yen) Y axis enable. Default 1
		 * 		0: Y axis disabled. 1: Y axis enabled.
		 * Bit 0 (Xen) X axis enabled. Default 1
		 * 		0: X axis disabled. 1: X axis enabled.
		 */
		CTRL_REG1_A = 0x20,

		/* Bit 7:6 (HPM1-HPM0) High pass filter mode selection. Default 00
		 * 		See Table 23.
		 * Bit 5:4 (HPCF2-HPCF1) High pass filter cut-off frequency selection.
		 * Bit 3 (FDS) Filtered data selection. Default 0
		 * 		0: Internal filter bypassed. 1: Data from internal filter sent to output register and FIFO.
		 * Bit 2 (HPCLICK) High pass filter enabled for CLICK function. Default 0
		 * 		0: Filter bypassed. 1: Filter enabled.
		 * Bit 1 (HPIS2) High pass filter enabled for AOI function on Interrupt 2. Default 0
		 * 		0: Filter bypassed. 1: Filter enabled.
		 * Bit 0 (HPIS1) High pass filter enabled for AOI function on Interrupt 1. Default 0
		 * 		0: Filter bypassed. 1: Filter enabled.
		 */
		CTRL_REG2_A = 0x21,

		/* Bit 7 (I1_CLICK) CLICK interrupt on INT1. Default 0
		 * 		0: Disable. 1: Enable.
		 * Bit 6 (I1_A0I1) AOI1 interrupt on INT1. Default 0
		 * 		0: Disable. 1: Enable.
		 * Bit 5 (I1_AOI2) AOI2 interrupt on INT1. Default 0
		 * 		0: Disable. 1: Enable.
		 * Bit 4 (I1_DRDY1) DRDY1 interrupt on INT1. Default 0
		 * 		0: Disable. 1: Enable.
		 * Bit 3 (I1_DRDY2) DRDY2 interrupt on INT1. Default 0
		 * 		0: Disable. 1: Enable.
		 * Bit 2 (I1_WTM) FIFO watermark interrupt on INT1. Default 0
		 * 		0: Disable. 1: Enable.
		 * Bit 1 (I1_OVERRUN) FIFO overrun interrupt on INT1. Default 0
		 * 		0: Disable. 1: Enable.
		 * Bit 0 Unimplemented.
		 */
		CTRL_REG3_A = 0x22,

		/* Bit 7 (BDU) Block data update. Default 0
		 * 		0: Continuous update. 1: output registers not updated until MSB and LSB reading.
		 * Bit 6 (BLE) Big/little endian data selection. Default 0
		 * 		0: data LSB @ lower address. 1: data MSB @ lower address.
		 * Bit 5:4 (FS1-FS0) Full-scale selection. Default 00
		 * 		00: 2g. 01: 4g. 10: 8g. 11: 16g.
		 * Bit 3 (HR) High resolution output mode: Default 0
		 * 		0: high resolution disable. 1: high resolution enable.
		 * Bit 2:1 Set to 0
		 * Bit 0 (SIM) SPI serial interface mode selection. Default 0
		 * 		0: 4 wire interface. 1: 3 wire interface.
		 */
		CTRL_REG4_A = 0x23,

		/* Bit 7 (BOOT) Reboot memory content. Default 0
		 * 		0: normal mode. 1: reboot memory content.
		 * Bit 6 (FIFO_EN) FIFO enable. Default 0
		 * 		0: FIFO disable. 1: FIFO enable.
		 * Bit 5:4 Unimplemented.
		 * Bit 3 (LIR_INT1) Latch interrupt request on INT1_SRC register, with INT1_SRC register
		 * 	cleared by reading INT1_SRC itself. Default 0
		 * 		0: interrupt request not latched. 1: interrupt request latched.
		 * Bit 2 (D4D_INT1) 4D Enable: 4D detection is enabled on INT1 when bit 6 on INT1_CFG is set to 1.
		 * Bit 1 (LIR_INT2) Latch interrupt request on INT2_SRC register with INT2_SRC register
		 * 	cleared by reading INT2)SRC itself. Default 0
		 * 		0:" interrupt request not latched. 1: interrupt request latched.
		 * Bit 0 (D4D_INT2) 4D enable: 4D detection is enabled on INT2 when 6D bit on INT2_CFG is set to 1.
		 */
		CTRL_REG5_A = 0x24,

		/* Bit 7 (I2_CLICKen) CLICK interrupt on PAD2. Default 0
		 * 		0: disable. 1: enable.
		 * Bit 6 (I2_INT1) Interrupt 1 on PAD2. Default 0
		 * 		0: disable. 1: enable.
		 * Bit 5 (I2_INT2) Interrupt 2 on PAD2. Default 0
		 * 		0: disable. 1: enable.
		 * Bit 4 (BOOT_I1) Reboot memory content on PAD2. Default 0
		 * 		0: disable. 1: enable.
		 * Bit 3 (P2_ACT) Active function status on PAD2. Default 0
		 * 		0: disable. 1: enable.
		 * Bit 2 Unimplemented.
		 * Bit 1 (H_LACTIVE) Interrupt active high, low. Default 0
		 * 		0: active high. 1: active low.
		 */
		CTRL_REG6_A = 0x25,

		/* Bit 7:0 (REF7-REF0) Reference value for interrupt generation. Default 0
		 */
		REFERENCE_A = 0x26,

		/* Bit 7 (ZYXOR) X, Y, and Z axis data overrun. Default 0
		 * 		0: no overrun has occurred. 1: a new set of data has overwritten the previous ones.
		 * Bit 6 (ZOR) Z axis data overrun. Default 0
		 * 		0: no overrun has occurred. 1: a new data for the Z-axis has overwritten the previous one.
		 * Bit 5 (YOR) Y axis data overrun. Default 0
		 * 		0: no overrun has occurred. 1: a new data for the Y-axis has overwritten the previous one.
		 * Bit 4 (XOR) X axis data overrun. Default 0
		 * 		0: no overrun has occurred. 1: a new data for the X-axis has overwritten the previous one.
		 * Bit 3 (ZYXDA) X, Y, and Z axis new data available. Default 0
		 * 		0: a new set of data is not yet available. 1: anew set of data is available.
		 * Bit 2 (ZDA) Z axis new data available. Default 0
		 * 		0: new data for the Z-axis is not yet available. 1: new data for the Z-axis is available.
		 * Bit 1 (YDA) Y axis new data available. Default 0
		 * 		0: new data for the Y-axis is not yet available. 1: new data for the Y-axis is available.
		 * Bit 0 (XDA) X axis new data available. Default 0
		 * 		0: new data for the X-axis is not yet available. 1: new data for the X-axis is available.
		 */
		STATUS_REG_A = 0x27,

		/*Low byte of the X axis acceleration data*/
		OUT_X_L_A = 0x28,

		/*High byte of the X axis acceleration data, expressed as 2's complement*/
		OUT_X_H_A = 0x29,

		/*Low byte of the Y axis acceleration data*/
		OUT_Y_L_A = 0x2A,

		/*High byte of the Y axis acceleration data, expressed as 2's complement*/
		OUT_Y_H_A = 0x2B,

		/*Low byte of the Z axis acceleration data*/
		OUT_Z_L_A = 0x2C,

		/*High byte of the Z axis acceleration data, expressed as 2's complement*/
		OUT_Z_H_A = 0x2D,

		/* Bit 7 (FM1-FM0) FIFO mode selection. Default 00
		 * 		See Table 38
		 * Bit 5 (TR) Trigger selection. Default 0
		 * 		0: trigger event linked to trigger signal on INT1. 1: trigger event linked to trigger signal on INT2.
		 * Bit 4:0 (FTH4:0) Default 0
		 */
		FIFO_CTRL_REG_A = 0x2E,

		/*No documentation available in datasheet*/
		FIFO_SRC_REG_A = 0x2F,

		/* Bit 7 (AOI) And/OR combination of interrupt events. Default 0
		 * 		See Table 42
		 * Bit 6 (6D) 6-direction detection function enabled. Default 0
		 * 		See Table 42
		 * Bit 5 Enable interrupt generation on Z high event or on direction recognition. Default 0
		 * 		0: disable interrupt request. 1: enable interrupt request
		 * Bit 4 Enable interrupt generation on Z low event or on direction recognition. Default 0
		 * 		0: disable interrupt request. 1: enable interrupt request.
		 * Bit 3 Enable interrupt generation on Y high event or on direction recognition. Default 0
		 * 		0: disable interrupt request. 1: enable interrupt request
		 * Bit 2 Enable interrupt generation on Y low event or on direction recognition. Default 0
		 * 		0: disable interrupt request. 1: enable interrupt request.
		 * Bit 1 Enable interrupt generation on X high event or on direction recognition. Default 0
		 * 		0: disable interrupt request. 1: enable interrupt request
		 * Bit 0 Enable interrupt generation on X low event or on direction recognition. Default 0
		 * 		0: disable interrupt request. 1: enable interrupt request.
		 */
		INT1_CFG_A = 0x30,

		/* Bit 7 Set to 0
		 * Bit 6 (IA) Interrupt Active. Default 0
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
		 * Bit 0 (ZL) X low. Default 0
		 * 		0: no interrupt. 1: X low event has occurred.
		 */
		INT1_SRC_A = 0x31,

		/* Bit 7 Set to 0
		 * Bit 6:0 (THS6-THS0) Interrupt 1 threshold. Default 0x00.
		 */
		INT1_THS_A = 0x32,

		/* Bit 7 Set to 0
		*  Bit 6:0 (D6-D0) Duration value. Default 0x00.
		*/
		INT1_DURATION_A = 0x33,

		/* Bit 7 (AOI) AND/OR combination of interrupt events. Default 0
		 * 		See Table 51
		 * Bit 6 (6D) 6-direction detection function enabled. Default 0
		 * 		See Table 51
		 * Bit 5 (ZHIE) Enable interrupt generation on Z high event. Default 0
		 * 		0: disable interrupt request. 1: enable interrupt request on measured accel. value higher than preset threshold.
		 * Bit 4 (ZLIE) Enable interrupt generation on Z low event. Default 0
		 * 		0: disable interrupt request. 1: enable interrupt request on measured accel. value lower than preset threshold.
		 * Bit 3 (YHIE) Enable interrupt generation on U high event. Default 0
		 * 		0: disable interrupt request. 1: enable interrupt request on measured accel. value higher than preset threshold.
		 * Bit 2 (YLIE) Enable interrupt generation on Y low event. Default 0
		 * 		0: disable interrupt request. 1: enable interrupt request on measured accel. value lower than preset threshold.
		 * Bit 1 (XHIE) Enable interrupt generation on X high event. Default 0
		 * 		0: disable interrupt request. 1: enable interrupt request on measured accel. value higher than preset threshold.
		 * Bit 0 (XLIE) Enable interrupt generation on X low event. Default 0
		 * 		0: disable interrupt request. 1: enable interrupt request on measured accel. value lower than preset threshold.
		 */
		INT2_CFG_A = 0x34,

		/* Bit 7 Set to 0
		 * Bit 6 (IA) interrupt active. Default 0
		 * 		0: no interrupt has been generated. 1: one or more interrupts have been generated.
		 * Bit 5 (ZH) Z high. Default 0
		 * 		0: no interrupt. 1: Z high event has occurred.
		 * Bit 4 (ZL) Z low. Default 0
		 * 		0: no interrupt. 1: Z low event has occurred.
		 * Bit 3 (YH) Y high. Default 0
		 * 		0: no interrupt. 1: Z high event has occurred.
		 * Bit 2 (YL) Y low. Default 0
		 * 		0: no interrupt. 1: Z low event has occurred.
		 * Bit 1 (XH) X high. Default 0
		 * 		0: no interrupt. 1: Z high event has occurred.
		 * Bit 0 (XL) X low. Default 0
		 * 		0: no interrupt. 1: Z low event has occurred.
		 */
		INT2_SOURCE_A = 0x35,

		/* Bit 7 Set to 0
		 * Bit 6:0 (THS6-THS0) Interrupt 2 threshold. Default 0x00.
		 */
		INT2_THS_A = 0x36,

		/* Bit 7 Set to 0
		*  Bit 6:0 (D6-D0) Duration value. Default 0x00.
		*/
		INT2_DURATION_A = 0x37,

		/* Bit 7:6 Unimplemented
		 * Bit 5 (ZD) Enable interrupt double CLICK on Z axis. Default 0
		 * 		0: disable interrupt request. 1: enable interrupt request on measured accel. value higher than preset threshold.
		 * Bit 4 (ZS) Enable interrupt single CLICK on Z axis. Default 0
		 * 		0: disable interrupt request. 1: enable interrupt request on measured accel. value higher than preset threshold.
		 * Bit 5 (TD) Enable interrupt double CLICK on Y axis. Default 0
		 * 		0: disable interrupt request. 1: enable interrupt request on measured accel. value higher than preset threshold.
		 * Bit 4 (TS) Enable interrupt single CLICK on Y axis. Default 0
		 * 		0: disable interrupt request. 1: enable interrupt request on measured accel. value higher than preset threshold.
		 * Bit 5 (XD) Enable interrupt double CLICK on X axis. Default 0
		 * 		0: disable interrupt request. 1: enable interrupt request on measured accel. value higher than preset threshold.
		 * Bit 4 (XS) Enable interrupt single CLICK on X axis. Default 0
		 * 		0: disable interrupt request. 1: enable interrupt request on measured accel. value higher than preset threshold.
		 */
		CLICK_CFG_A = 0x38,

		/* Bit 7 Unimplemented.
		 * Bit 6 (IA) Interrupt active. Default 0
		 * 		0: no interrupt has been generated. 1: one or more interrupts have been generated.
		 * Bit 5 (DCLICK) Double CLICK-CLICK enable. Default 0
		 * 		0: double CLICK-CLICK detection disable. 1: double CLICK-CLICK detection enable.
		 * Bit 4 (SCLICK) Single CLICK-CLICK enable. Default 0
		 * 		0: single CLICK-CLICK detection disable. 1: single CLICK-CLICK detection enable.
		 * Bit 3 (Sign) CLICK-CLICK Sign.
		 * 		0: positive detection. 1: negative detection.
		 * Bit 2 (Z) Z CLICK-CLICK detection. Default 0
		 * 		0: no interrupt. 1: Z high event has occurred.
		 * Bit 1 (Y) Y CLICK-CLICK detection. Default 0
		 * 		0: no interrupt. 1: Y high event has occurred.
		 * Bit 0 (X) X CLICK-CLICK detection. Default 0
		 * 		0: no interrupt. 1: X high event has occurred.
		 */
		CLICK_SRC_A = 0x39,

		/* Bit 7 Unimplemented.
		 * Bit 6:0 (Ths6:Thos0) CLICK-CLICK threshold. Default 0x00
		 * 		1LSB = full-scale/128.  Bit 6:0 define the threshold which is used by the system to start the click detection. Unsigned.
		 */
		CLICK_THS_A = 0x3A,

		/* Bit 7 Unimplemented.
		 * Bit 6:0 (TLI6:TLI0) CLICK-CLICK time limit. Default 0x00
		 * 		1LSB = 1/ODR.  Defines the maximum time interval that can elapse between the start of the click detection procedure and when the acceleration goes back below the threshold.
		 */
		TIME_LIMIT_A = 0x3B,

		/* Bit 7:0 (TLA7-TLA0) CLICK-CLICK time latency. Default 0x00
		 * 		1LSB = 1/ODR TLA7 through TLA0 define the time interval that starts after the first click detection where the click detection procedure is disabled.
		 */
		TIME_LATENCY_A = 0x3C,

		/* Bit 7:0 (TW7:TW0) CLICK-CLICK time window
		 * 		1LSB =1/ODR. TW7 through TW0 define the maximum interval of time that can elapse after the end of the latency interval in which the click detection procedured can start.
		 */
		TIME_WINDOW_A = 0x3D
	};

	enum MagRegister
	{
		/* Bit 7(TEMP_EN) Temperature sensor enable. Default 0
		 * 		0: temperature sensor disabled. 1: temperature sensor enable.d
		 * Bit 6:5 Set to 0
		 * Bit 4:2 (DO2:DO0) data output rate bits.  Default 100
		 * 		See Table 72
		 * Bit 1:0 Set to 0
		 */
		CRA_REG_M = 0x00,

		/* Bit 7:5 (GN2:GN0) Gain configuration bits. The gain configuration is common for all channels
		 * 		See Table 75
		 * Bit 4:0 Set to 0.
		 */
		CRB_REG_M = 0x01,

		/* Bit 7:2 Set to 0
		 * Bit 1:0 (MD1-MD0) Mode selection bits. These bits select the operation mode of this device.
		 *		See Table 78
		 */
		MR_REG_M = 0x02,

		/* High byte of X axis. Expressed as 2's Complement.
		 */
		OUT_X_H_M = 0x03,

		/* Low byte of X axis.
		 */
		OUT_X_L_M = 0x04,

		/* High byte of Z axis. Expressed as 2's complement.
		 */
		OUT_Z_H_M = 0x05,

		/* Low byte of Z axis.
		 */
		OUT_Z_L_M = 0x06,

		/* High byte of Y axis. Expressed as 2's complement.
		 */
		OUT_Y_H_M = 0x07,

		/*Low byte of Y axis.
		 */
		OUT_Y_L_M = 0x08,

		/* Bit 7:2 Set to 0
		 * Bit 1 (LOCK) Data output register lock. Once a new set of measurements is available,
		 * 	this bit is set when the first magnetic file data register has been read.
		 * Bit 0 (DRDY) Data ready bit. This bit is set when a new set of measurements are available.
		 */
		SR_REG_M = 0x09,

		/* 0b01001000 */
		IRA_REG_M = 0x0A,

		/* 0b00110100 */
		IRB_REG_M = 0x0B,

		/* 0b00110011 */
		IRC_REG_M = 0x0C,

		/* Bit 7:0 (TEMP11-TEMP4) Top 8 bits of temperature data. Expressed as 2's complement.*/
		TEMP_OUT_H_M = 0x31,

		/* Bit 7:4 (TMEP3-TEMP0) Bottom 4 bits of temperature data.
		 * Bit 3:0 Unimplemented.
		 */
	TEMP_OUT_L_M = 0x32,
};

	enum AccelerometerFullScale
	{
		_2g = 0x00,
		_4g = 0x01,
		_8g = 0x10,
		_16g = 0x11
	};

	enum MagnetometerFullScale
	{
		_1d3gauss = 0x01,
		_1d9gauss = 0x02,
		_2d5gauss = 0x03,
		_4d0gauss = 0x04,
		_4d7gauss = 0x05,
		_5d6gauss = 0x06,
		_8d1gauss = 0x07,

	};
	LSM303();

	uint8_t accGetI2CAddress();
	int16_t accGetXAxisRaw();
	int16_t accGetYAxisRaw();
	int16_t accGetZAxisRaw();
	float accGetXAxisG();
	float accGetYAxisG();
	float accGetZAxisG();

	uint8_t magGetI2CAddress();
	int16_t magGetXAxisRaw();
	int16_t magGetYAxisRaw();
	int16_t magGetZAxisRaw();
	float magGetXAxisGaus();
	float magGetYAxisGaus();
	float magGetZAxisGaus();

private:
	const static uint8_t _magDefaultI2CAddress = 0x1E;
	const static uint8_t _accDefaultI2CAddress = 0x19;
	AccelerometerFullScale _accFullScale;
	MagnetometerFullScale _magFullScale;

	float _accxAxisOffset;
	float _accyAxisOffset;
	float _acczAxisOffset;

	int _magI2CHandle;
	int _accI2CHandle;

	void calibrate();
	void magUnlock();
};

#endif /* LSM303DLHC_H_ */
