#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#define PRU_BASEFREQ    	40000    		// PRU Base thread ISR update frequency (hz)
#define PRU_SERVOFREQ       1000            // PRU Servo thread ISR update freqency (hz)

#define STEPBIT     		22            	// bit location in DDS accum
#define STEP_MASK   		(1L<<STEPBIT)

#define JOINTS			    6				// Number of joints - set this the same as LinuxCNC HAL compenent
#define VARIABLES           2             	// Number of command values - set this the same as the LinuxCNC HAL compenent

#define PRU_DATA			0x64617461 		// "data" payload
#define PRU_READ          	0x72656164  	// "read" payload
#define PRU_WRITE         	0x77726974  	// "writ" payload
#define PRU_ACKNOWLEDGE		0x61636b6e		// "ackn" payload
#define PRU_ERR		        0x6572726f		// "erro" payload
#define PRU_ESTOP           0x65737470  	// "estp" payload
#define PRU_NVMPG			0x6D706764

#define BUFFER_SIZE			68

#define DATA_ERR_MAX		5

// Connection LED
#define LED_PORT 	GPIO3
#define LED_PIN 	0

// Step and Directions pins
#define STEP_PORT	GPIO1
#define X_STEP_PIN	22
#define X_DIR_PIN	17
#define Y_STEP_PIN	24
#define Y_DIR_PIN	31
#define Z_STEP_PIN	18
#define Z_DIR_PIN	25
#define A_STEP_PIN	27
#define A_DIR_PIN	21
#define B_STEP_PIN	16
#define B_DIR_PIN	23
#define C_STEP_PIN	20
#define C_DIR_PIN	19

// Inputs
#define FHA_PORT	GPIO3
#define FHA_PIN		26
#define FHB_PORT	GPIO4
#define FHB_PIN		30
#define SRO_PORT	GPIO3
#define SRO_PIN		19
#define SJR_PORT	GPIO4
#define SJR_PIN		20

#define IN01_PORT	GPIO4
#define IN01_PIN	26
#define IN02_PORT	GPIO4
#define IN02_PIN	27
#define IN03_PORT	GPIO4
#define IN03_PIN	21
#define IN04_PORT	GPIO4
#define IN04_PIN	24

#define IN05_PORT	GPIO4
#define IN05_PIN	28
#define IN06_PORT	GPIO4
#define IN06_PIN	25
#define IN07_PORT	GPIO4
#define IN07_PIN	29
#define IN08_PORT	GPIO4
#define IN08_PIN	23

#define IN09_PORT	GPIO4
#define IN09_PIN	31
#define IN10_PORT	GPIO3
#define IN10_PIN	18
#define IN11_PORT	GPIO3
#define IN11_PIN	21
#define IN12_PORT	GPIO3
#define IN12_PIN	20

#define CIN_PORT 	GPIO4
#define CIN_PIN		18
#define XIN_PORT 	GPIO4
#define XIN_PIN		17
#define YIN_PORT 	GPIO4
#define YIN_PIN		16
#define ZIN_PORT 	GPIO4
#define ZIN_PIN		15

#define AIN_PORT 	GPIO4
#define AIN_PIN		14
#define X100IN_PORT	GPIO4
#define X100IN_PIN	13
#define X10IN_PORT	GPIO4
#define X10IN_PIN	12
#define EP_PORT		GPIO4
#define EP_PIN		11

#define WHA_PORT	GPIO3
#define WHA_PIN		16
#define WHB_PORT	GPIO3
#define WHB_PIN		17
#define INDEX_PORT	GPIO3
#define INDEX_PIN	22

// Outputs
#define OUT1_PORT	GPIO4
#define OUT1_PIN	2
#define OUT2_PORT	GPIO4
#define OUT2_PIN	1
#define OUT3_PORT	GPIO4
#define OUT3_PIN	3
#define OUT4_PORT	GPIO4
#define OUT4_PIN	4
#define OUT5_PORT	GPIO4
#define OUT5_PIN	5
#define OUT6_PORT	GPIO4
#define OUT6_PIN	6
#define OUT7_PORT	GPIO4
#define OUT7_PIN	7
#define OUT8_PORT	GPIO4
#define OUT8_PIN	8
#define OUT9_PORT	GPIO4
#define OUT9_PIN	9
#define OUT10_PORT	GPIO4
#define OUT10_PIN	10

#endif
