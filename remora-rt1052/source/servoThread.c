
#include "fsl_gpio.h"
#include "fsl_iomuxc.h"

#include "configuration.h"
#include "servoThread.h"




uint8_t noDataCount;
extern volatile bool cmdReceived;
extern volatile bool commsStatus;

void configServoThread()
{
	// Configure pins

	gpio_pin_config_t output_config = {
	  .direction = kGPIO_DigitalOutput,
	  .outputLogic = 0U,
	  .interruptMode = kGPIO_NoIntmode
	};

	// CONN LED [pad L5, GPIO_SD_B1_00, gpio 3, pin 0]
	IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_00_GPIO3_IO00, 0U);
	GPIO_PinInit(LED_PORT, LED_PIN, &output_config);
	GPIO_PinWrite(LED_PORT, LED_PIN, 1U);

}


void updateServoThread()
{
	monitorEthernet();
}


void monitorEthernet()
{
	// Monitor Ethernet communications
	if (cmdReceived)
	{
		noDataCount = 0;
		commsStatus = true;

		// turn CONN LED on (inverted logic)
		GPIO_PinWrite(LED_PORT, LED_PIN, 0U);
	}
	else
	{
		noDataCount++;
	}

	if (noDataCount > DATA_ERR_MAX)
	{
		noDataCount = 0;
		commsStatus = false;

		// turn CONN LED off (inverted logic)
		GPIO_PinWrite(LED_PORT, LED_PIN, 1U);
	}

	cmdReceived = false;
}
