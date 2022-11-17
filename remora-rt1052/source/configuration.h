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

#define LED_PORT 	GPIO3
#define LED_PIN 	0

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

#endif
