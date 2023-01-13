
#include "fsl_debug_console.h"
#include "fsl_pit.h"

#include "configuration.h"
#include "baseThread.h"
#include "servoThread.h"


pit_config_t pitConfig;

void createThreads(void)
{
    // The Periodic Interrupt Timer (PIT) module is used to trigger the threads
    CLOCK_SetMux(kCLOCK_PerclkMux, 1U);
    CLOCK_SetDiv(kCLOCK_PerclkDiv, 0U);
    PIT_GetDefaultConfig(&pitConfig);
    PIT_Init(PIT, &pitConfig);

    // PIT channel 0 - BASE thread
    PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, CLOCK_GetFreq(kCLOCK_OscClk)/(2 * PRU_BASEFREQ));
    PIT_EnableInterrupts(PIT, kPIT_Chnl_0, kPIT_TimerInterruptEnable);

    // PIT channel 1 - SERVO thread
    PIT_SetTimerPeriod(PIT, kPIT_Chnl_1, CLOCK_GetFreq(kCLOCK_OscClk)/PRU_SERVOFREQ);
    PIT_EnableInterrupts(PIT, kPIT_Chnl_1, kPIT_TimerInterruptEnable);

    EnableIRQ(PIT_IRQn);
}

void startThreads(void)
{
    PIT_StartTimer(PIT, kPIT_Chnl_0);
    PIT_StartTimer(PIT, kPIT_Chnl_1);
}


void PIT_IRQHandler(void)
{
	volatile uint32_t status;

    status = PIT_GetStatusFlags(PIT, kPIT_Chnl_0);
	if(status & (uint32_t)kPIT_TimerFlag)
	{
		PIT_ClearStatusFlags(PIT, kPIT_Chnl_0, kPIT_TimerFlag);
		__DSB();

		updateBaseThread();
	}

    status = PIT_GetStatusFlags(PIT, kPIT_Chnl_1);
	if(status & (uint32_t)kPIT_TimerFlag)
	{
		PIT_ClearStatusFlags(PIT, kPIT_Chnl_1, kPIT_TimerFlag);
		__DSB();

		updateServoThread();
	}

}

