
#include "fsl_gpio.h"
#include "fsl_iomuxc.h"

#include "configuration.h"
#include "remora.h"
#include "baseThread.h"


extern volatile rxData_t rxData;
extern volatile txData_t txData;

int8_t stepPin[JOINTS];
int8_t dirPin[JOINTS];

bool step;
float frequencyScale;
bool isEnabled[JOINTS];
bool isForward[JOINTS];
bool isStepping[JOINTS];
int32_t frequencyCommand[JOINTS];
int32_t rawCount[JOINTS];
int32_t DDSaccumulator[JOINTS];
int32_t DDSaddValue[JOINTS];


void configBaseThread()
{
	// Configure pins

	gpio_pin_config_t output_config = {
	  .direction = kGPIO_DigitalOutput,
	  .outputLogic = 1U,
	  .interruptMode = kGPIO_NoIntmode
	};

	// Joint 0 - X
	GPIO_PinInit(STEP_PORT, X_STEP_PIN, &output_config);
	GPIO_PinInit(STEP_PORT, X_DIR_PIN, &output_config);
	stepPin[0] = X_STEP_PIN;
	dirPin[0] = X_DIR_PIN;

	// Joint 1 - Y
	GPIO_PinInit(STEP_PORT, Y_STEP_PIN, &output_config);
	GPIO_PinInit(STEP_PORT, Y_DIR_PIN, &output_config);
	stepPin[1] = Y_STEP_PIN;
	dirPin[1] = Y_DIR_PIN;

	// Joint 2 - Z
	GPIO_PinInit(STEP_PORT, Z_STEP_PIN, &output_config);
	GPIO_PinInit(STEP_PORT, Z_DIR_PIN, &output_config);
	stepPin[2] = Z_STEP_PIN;
	dirPin[2] = Z_DIR_PIN;

	// Joint 3 - A
	GPIO_PinInit(STEP_PORT, A_STEP_PIN, &output_config);
	GPIO_PinInit(STEP_PORT, A_DIR_PIN, &output_config);
	stepPin[3] = A_STEP_PIN;
	dirPin[3] = A_DIR_PIN;

	// Joint 4 - B
	GPIO_PinInit(STEP_PORT, B_STEP_PIN, &output_config);
	GPIO_PinInit(STEP_PORT, B_DIR_PIN, &output_config);
	stepPin[4] = B_STEP_PIN;
	dirPin[4] = B_DIR_PIN;

	//Joint 5 - C
	GPIO_PinInit(STEP_PORT, C_STEP_PIN, &output_config);
	GPIO_PinInit(STEP_PORT, C_DIR_PIN, &output_config);
	stepPin[5] = C_STEP_PIN;
	dirPin[5] = C_DIR_PIN;

	frequencyScale = (float)(1 << STEPBIT) / (float)PRU_BASEFREQ;

}


void updateBaseThread()
{
	// ISR runns at double PRU_BASEFREQ, make steps in one cycle and reset in the next
	if (!step)
	{
		makeSteps();
		step = true;
	}
	else
	{
		resetSteps();
		step = false;
	}
}


void makeSteps()
{
	int8_t i;
	int32_t stepNow;

	// loop through the step generators
	for (i = 0; i <= JOINTS; i++)
	{
		stepNow = 0;
		isEnabled[i] = ((rxData.jointEnable & (1 << i)) != 0);

		if (isEnabled[i] == true)
		{
			frequencyCommand[i] = rxData.jointFreqCmd[i];
			DDSaddValue[i] = frequencyCommand[i] * frequencyScale;
			stepNow = DDSaccumulator[i];
			DDSaccumulator[i] += DDSaddValue[i];
			stepNow ^= DDSaccumulator[i];
			stepNow &= (1L << STEPBIT);
			rawCount[i] = DDSaccumulator[i] >> STEPBIT;

			if (DDSaddValue[i] > 0)
			{
				isForward[i] = true;
			}
			else
			{
				isForward[i] = false;
			}

			if (stepNow)
			{
				GPIO_PinWrite(STEP_PORT, dirPin[i], isForward[i]);
				GPIO_PinWrite(STEP_PORT, stepPin[i], 1);
				txData.jointFeedback[i] = DDSaccumulator[i];
				isStepping[i] = true;
			}
		}
	}
}


void resetSteps()
{
	int8_t i;

	for (i = 0; i <= JOINTS; i++)
	{
		if (isStepping[i])
		{
			GPIO_PinWrite(STEP_PORT, stepPin[i], 0);
		}
	}
}
