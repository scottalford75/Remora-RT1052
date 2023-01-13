
#include "fsl_gpio.h"
#include "fsl_iomuxc.h"
#include "fsl_qtmr.h"

#include "configuration.h"
#include "remora.h"
#include "servoThread.h"

#include "nvmpg.h"

extern volatile rxData_t rxData;
extern volatile txData_t txData;
extern volatile bool cmdReceived;
extern volatile bool commsStatus;
extern uint8_t noDataCount;

#define confine(value, min, max) (((value) < (min))?(min):(((value) > (max))?(max):(value)))

/* The QTMR instance/channel used for board */
#define QTMR_BASEADDR              TMR1
#define QTMR_PWM_CHANNEL           kQTMR_Channel_0

/* QTMR Clock source divider for Ipg clock source, the value of two macros below should be aligned. */
#define QTMR_PRIMARY_SOURCE       (kQTMR_ClockDivide_32)
#define QTMR_CLOCK_SOURCE_DIVIDER (32U)


float pwmSP, pwmSPold;

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

	// VSD PWM -> Analog 0-10V
	IOMUXC_SetPinMux(IOMUXC_GPIO_B0_00_TMR1_TIMER0, 0U);
	IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_00_TMR1_TIMER0, 0x10B0U);

    qtmr_config_t qtmrConfig;

    QTMR_GetDefaultConfig(&qtmrConfig);
    qtmrConfig.primarySource = QTMR_PRIMARY_SOURCE;
    QTMR_Init(QTMR_BASEADDR, QTMR_PWM_CHANNEL, &qtmrConfig);
}


void updateServoThread()
{
	monitorEthernet();
	readInputs();
	setOutputs();
	updatePWM();
	updateNVMPG();
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


void updatePWM()
{
	pwmSP = confine(rxData.setPoint[0], 0.0, 100.0);

	if (pwmSP != pwmSPold)
	{
		setupPWM(pwmSP);
		pwmSPold = pwmSP;
	}
}


void setupPWM(float dutyCyclePercent)
{
	TMR_Type *base = QTMR_BASEADDR;
	qtmr_channel_selection_t channel = QTMR_PWM_CHANNEL;
	uint32_t pwmFreqHz = 100;
	bool outputPolarity = false;
	uint32_t srcClock_Hz = (CLOCK_GetFreq(kCLOCK_IpgClk) / QTMR_CLOCK_SOURCE_DIVIDER);

    uint32_t periodCount, highCount, lowCount;
    uint16_t reg;

    // stop the timer so we can force an output state if needed
    QTMR_StopTimer(QTMR_BASEADDR, QTMR_PWM_CHANNEL);

    // manage the Quad Timer inability to handle 0 and 100% conditions
    if (dutyCyclePercent == 0.0)
    {
    	// use the FORCE and VAL registers to output 0
    	base->CHANNEL[channel].SCTRL |= (TMR_SCTRL_FORCE_MASK | TMR_SCTRL_OEN_MASK | TMR_SCTRL_VAL(0));
    }
    else if (dutyCyclePercent == 100.0)
    {
    	// use the FORCE and VAL registers to output 1
    	base->CHANNEL[channel].SCTRL |= (TMR_SCTRL_FORCE_MASK | TMR_SCTRL_OEN_MASK | TMR_SCTRL_VAL(1));
    }
    else
    {
        /* Set OFLAG pin for output mode and force out a low on the pin */
        base->CHANNEL[channel].SCTRL |= (TMR_SCTRL_FORCE_MASK | TMR_SCTRL_OEN_MASK);

        /* Counter values to generate a PWM signal */
        periodCount = srcClock_Hz / pwmFreqHz;
        highCount   = (uint32_t) ((float)periodCount * dutyCyclePercent) / 100;
        lowCount    = periodCount - highCount;

        if (highCount > 0U)
        {
            highCount -= 1U;
        }
        if (lowCount > 0U)
        {
            lowCount -= 1U;
        }

        /* Setup the compare registers for PWM output */
        base->CHANNEL[channel].COMP1 = (uint16_t)lowCount;
        base->CHANNEL[channel].COMP2 = (uint16_t)highCount;

        /* Setup the pre-load registers for PWM output */
        base->CHANNEL[channel].CMPLD1 = (uint16_t)lowCount;
        base->CHANNEL[channel].CMPLD2 = (uint16_t)highCount;

        reg = base->CHANNEL[channel].CSCTRL;
        /* Setup the compare load control for COMP1 and COMP2.
         * Load COMP1 when CSCTRL[TCF2] is asserted, load COMP2 when CSCTRL[TCF1] is asserted
         */
        reg &= (uint16_t)(~(TMR_CSCTRL_CL1_MASK | TMR_CSCTRL_CL2_MASK));
        reg |= (TMR_CSCTRL_CL1(kQTMR_LoadOnComp2) | TMR_CSCTRL_CL2(kQTMR_LoadOnComp1));
        base->CHANNEL[channel].CSCTRL = reg;

        if (outputPolarity)
        {
            /* Invert the polarity */
            base->CHANNEL[channel].SCTRL |= TMR_SCTRL_OPS_MASK;
        }
        else
        {
            /* True polarity, no inversion */
            base->CHANNEL[channel].SCTRL &= ~(uint16_t)TMR_SCTRL_OPS_MASK;
        }

        reg = base->CHANNEL[channel].CTRL;
        reg &= ~(uint16_t)TMR_CTRL_OUTMODE_MASK;
        /* Count until compare value is  reached and re-initialize the counter, toggle OFLAG output
         * using alternating compare register
         */
        reg |= (TMR_CTRL_LENGTH_MASK | TMR_CTRL_OUTMODE(kQTMR_ToggleOnAltCompareReg));
        base->CHANNEL[channel].CTRL = reg;

        // restart the timer
        QTMR_StartTimer(QTMR_BASEADDR, QTMR_PWM_CHANNEL, kQTMR_PriSrcRiseEdge);
    }
}
