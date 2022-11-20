
#include "fsl_gpio.h"
#include "fsl_iomuxc.h"

#include "configuration.h"
#include "remora.h"
#include "servoThread.h"


extern volatile rxData_t rxData;
extern volatile txData_t txData;

uint8_t noDataCount;
extern volatile bool cmdReceived;
extern volatile bool commsStatus;

void configServoThread()
{
	// Configure pins

	gpio_pin_config_t output_config = {
	  .direction = kGPIO_DigitalOutput,
	  .outputLogic = 1U,
	  .interruptMode = kGPIO_NoIntmode
	};

	gpio_pin_config_t input_config = {
	  .direction = kGPIO_DigitalInput,
	  .outputLogic = 0U,
	  .interruptMode = kGPIO_NoIntmode
	};

	// Connection LED
	GPIO_PinInit(LED_PORT, LED_PIN, &output_config);

	// Inputs
	GPIO_PinInit(FHA_PORT, FHA_PIN, &input_config);
	GPIO_PinInit(FHB_PORT, FHB_PIN, &input_config);
	GPIO_PinInit(SRO_PORT, SRO_PIN, &input_config);
	GPIO_PinInit(SJR_PORT, SJR_PIN, &input_config);

	GPIO_PinInit(IN01_PORT, IN01_PIN, &input_config);
	GPIO_PinInit(IN02_PORT, IN02_PIN, &input_config);
	GPIO_PinInit(IN03_PORT, IN03_PIN, &input_config);
	GPIO_PinInit(IN04_PORT, IN04_PIN, &input_config);

	GPIO_PinInit(IN05_PORT, IN05_PIN, &input_config);
	GPIO_PinInit(IN06_PORT, IN06_PIN, &input_config);
	GPIO_PinInit(IN07_PORT, IN07_PIN, &input_config);
	GPIO_PinInit(IN08_PORT, IN08_PIN, &input_config);

	GPIO_PinInit(IN09_PORT, IN09_PIN, &input_config);
	GPIO_PinInit(IN10_PORT, IN10_PIN, &input_config);
	GPIO_PinInit(IN11_PORT, IN11_PIN, &input_config);
	GPIO_PinInit(IN12_PORT, IN12_PIN, &input_config);

	GPIO_PinInit(XIN_PORT, XIN_PIN, &input_config);
	GPIO_PinInit(YIN_PORT, YIN_PIN, &input_config);
	GPIO_PinInit(ZIN_PORT, ZIN_PIN, &input_config);
	GPIO_PinInit(AIN_PORT, AIN_PIN, &input_config);

	//GPIO_PinInit(BIN_PORT, BIN_PIN, &input_config);
	GPIO_PinInit(CIN_PORT, CIN_PIN, &input_config);
	//GPIO_PinInit(X1IN_PORT, X1IN_PIN, &input_config);
	GPIO_PinInit(X10IN_PORT, X10IN_PIN, &input_config);

	GPIO_PinInit(X100IN_PORT, X100IN_PIN, &input_config);
	GPIO_PinInit(EP_PORT, EP_PIN, &input_config);
	GPIO_PinInit(INDEX_PORT, INDEX_PIN, &input_config);

	// Outputs
	GPIO_PinInit(OUT1_PORT, OUT1_PIN, &output_config);
	GPIO_PinInit(OUT2_PORT, OUT2_PIN, &output_config);
	GPIO_PinInit(OUT3_PORT, OUT3_PIN, &output_config);
	GPIO_PinInit(OUT4_PORT, OUT4_PIN, &output_config);
	GPIO_PinInit(OUT5_PORT, OUT5_PIN, &output_config);
	GPIO_PinInit(OUT6_PORT, OUT6_PIN, &output_config);
	GPIO_PinInit(OUT7_PORT, OUT7_PIN, &output_config);
	GPIO_PinInit(OUT8_PORT, OUT8_PIN, &output_config);
	GPIO_PinInit(OUT9_PORT, OUT9_PIN, &output_config);
	GPIO_PinInit(OUT10_PORT, OUT10_PIN, &output_config);
}


void updateServoThread()
{
	monitorEthernet();
	readInputs();
	setOutputs();
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


void readInputs()
{
	// Read inputs
	txData.inputs = 0;

	// inputs are inverted
	txData.inputs |= !GPIO_PinRead(FHA_PORT, FHA_PIN) << 0;
	txData.inputs |= !GPIO_PinRead(FHB_PORT, FHB_PIN) << 1;
	txData.inputs |= !GPIO_PinRead(SRO_PORT, SRO_PIN) << 2;
	txData.inputs |= !GPIO_PinRead(SJR_PORT, SJR_PIN) << 3;

	txData.inputs |= !GPIO_PinRead(IN01_PORT, IN01_PIN) << 4;
	txData.inputs |= !GPIO_PinRead(IN02_PORT, IN02_PIN) << 5;
	txData.inputs |= !GPIO_PinRead(IN03_PORT, IN03_PIN) << 6;
	txData.inputs |= !GPIO_PinRead(IN04_PORT, IN04_PIN) << 7;

	txData.inputs |= !GPIO_PinRead(IN05_PORT, IN05_PIN) << 8;
	txData.inputs |= !GPIO_PinRead(IN06_PORT, IN06_PIN) << 9;
	txData.inputs |= !GPIO_PinRead(IN07_PORT, IN07_PIN) << 10;
	txData.inputs |= !GPIO_PinRead(IN08_PORT, IN08_PIN) << 11;

	txData.inputs |= !GPIO_PinRead(IN09_PORT, IN09_PIN) << 12;
	txData.inputs |= !GPIO_PinRead(IN10_PORT, IN10_PIN) << 13;
	txData.inputs |= !GPIO_PinRead(IN11_PORT, IN11_PIN) << 14;
	txData.inputs |= !GPIO_PinRead(IN12_PORT, IN12_PIN) << 15;

	txData.inputs |= !GPIO_PinRead(XIN_PORT, XIN_PIN) << 16;
	txData.inputs |= !GPIO_PinRead(YIN_PORT, YIN_PIN) << 17;
	txData.inputs |= !GPIO_PinRead(ZIN_PORT, ZIN_PIN) << 18;
	txData.inputs |= !GPIO_PinRead(AIN_PORT, AIN_PIN) << 19;

	//txData.inputs |= !GPIO_PinRead(BIN_PORT, BIN_PIN) << 20; // not available
	txData.inputs |= !GPIO_PinRead(CIN_PORT, CIN_PIN) << 21; // EC300 = R1IN
	//txData.inputs |= !GPIO_PinRead(X1IN_PORT, X1IN_PIN) << 22; //not available
	txData.inputs |= !GPIO_PinRead(X10IN_PORT, X10IN_PIN) << 23;

	txData.inputs |= !GPIO_PinRead(X100IN_PORT, X100IN_PIN) << 24;
	txData.inputs |= !GPIO_PinRead(EP_PORT, EP_PIN) << 25;
	txData.inputs |= !GPIO_PinRead(INDEX_PORT, INDEX_PIN) << 26;
}

void setOutputs()
{
	// Outputs
	bool output;

	output = rxData.outputs & (1 << 0);
	GPIO_PinWrite(OUT1_PORT, OUT1_PIN, output);

	output = rxData.outputs & (1 << 1);
	GPIO_PinWrite(OUT2_PORT, OUT2_PIN, output);

	output = rxData.outputs & (1 << 2);
	GPIO_PinWrite(OUT3_PORT, OUT3_PIN, output);

	output = rxData.outputs & (1 << 3);
	GPIO_PinWrite(OUT4_PORT, OUT4_PIN, output);

	output = rxData.outputs & (1 << 4);
	GPIO_PinWrite(OUT5_PORT, OUT5_PIN, output);

	output = rxData.outputs & (1 << 5);
	GPIO_PinWrite(OUT6_PORT, OUT6_PIN, output);

	output = rxData.outputs & (1 << 6);
	GPIO_PinWrite(OUT7_PORT, OUT7_PIN, output);

	output = rxData.outputs & (1 << 7);
	GPIO_PinWrite(OUT8_PORT, OUT8_PIN, output);

	output = rxData.outputs & (1 << 8);
	GPIO_PinWrite(OUT9_PORT, OUT9_PIN, output);

	output = rxData.outputs & (1 << 9);
	GPIO_PinWrite(OUT10_PORT, OUT10_PIN, output);

}
