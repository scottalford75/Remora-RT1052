/*
Remora firmware for Novusun RT1052 based CNC controller boards
to allow use with LinuxCNC.

Copyright (C) Scott Alford aka scotta

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; If not, see <http://www.gnu.org/licenses/>.
*/


#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"

#include "ethernet.h"
#include "lwip/timeouts.h"
#include "enet_ethernetif.h"

#include "nvmpg.h"

#include "fsl_gpio.h"
#include "fsl_iomuxc.h"


#include "configuration.h"
#include "remora.h"

#include "threads.h"
#include "baseThread.h"
#include "servoThread.h"


// boolean
volatile bool PRUreset;
bool threadsRunning = false;
volatile bool cmdReceived = false;
volatile bool mpgReceived = false;
volatile bool mpgSerial = false;
volatile bool commsStatus = false;
uint8_t noDataCount;

// unions for RX, TX and MPG data
rxData_t rxBuffer;
volatile rxData_t rxData;
volatile txData_t txData;
mpgData_t mpgData;
uint8_t mpgTxData[53] = {'\0'};
uint8_t mpgRxData;

// state machine
enum State {
    ST_SETUP = 0,
    ST_START,
    ST_IDLE,
    ST_RUNNING,
    ST_STOP,
    ST_RESET,
    ST_WDRESET
};



int main(void)
{
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    initEthernet();
    initNVMPG();

    enum State currentState;
    enum State prevState;

    currentState = ST_SETUP;
    prevState = ST_RESET;

    PRINTF("\r\n Remora RT1052 firmware for Digital Dream / Novusun CNC controllers starting\r\n");

    while (1)
    {
    	switch(currentState){
			  case ST_SETUP:
				  // do setup tasks
				  if (currentState != prevState)
				  {
					  PRINTF("\n ## Entering SETUP state\n\n");
				  }
				  prevState = currentState;

				  createThreads();
				  configServoThread();
				  configBaseThread();
				  startThreads();
				  udpServer_init();

				  currentState = ST_START;
				  break;

			  case ST_START:
				  // do start tasks
				  if (currentState != prevState)
				  {
					  PRINTF("\n ## Entering START state\n");
				  }
				  prevState = currentState;

				  if (!threadsRunning)
				  {
					  // Start the threads
					  PRINTF("\n  Starting the BASE thread\n");
					  //baseThread->startThread();

					  PRINTF("\n  Starting the SERVO thread\n");
					  //servoThread->startThread();

					  threadsRunning = true;
				  }

				  currentState = ST_IDLE;

				  break;


			  case ST_IDLE:
				  // do something when idle
				  if (currentState != prevState)
				  {
					  PRINTF("\n ## Entering IDLE state\n");
				  }
				  prevState = currentState;

				  //wait for data before changing to running state
				  if (commsStatus)
				  {
					  currentState = ST_RUNNING;
				  }

				  break;

			  case ST_RUNNING:
				  // do running tasks
				  if (currentState != prevState)
				  {
					  PRINTF("\n ## Entering RUNNING state\n");
				  }
				  prevState = currentState;

				  if (commsStatus == false)
				  {
					  currentState = ST_RESET;
				  }

				  break;

			  case ST_STOP:
				  // do stop tasks
				  if (currentState != prevState)
				  {
					  PRINTF("\n ## Entering STOP state\n");
				  }
				  prevState = currentState;
				  currentState = ST_STOP;

				  break;

			  case ST_RESET:
				  // do reset tasks
				  if (currentState != prevState)
				  {
					  PRINTF("\n ## Entering RESET state\n");
				  }
				  prevState = currentState;

				  // set all of the rxData buffer to 0
				  // rxData.rxBuffer is volatile so need to do this the long way. memset cannot be used for volatile
				  PRINTF("  Resetting rxBuffer\n");
				  {
					  int n = sizeof(rxData.rxBuffer);
					  while(n-- > 0)
					  {
						  rxData.rxBuffer[n] = 0;
					  }
				  }

				  currentState = ST_IDLE;
				  break;

			  case ST_WDRESET:
				  // force a reset
				  //HAL_NVIC_SystemReset();
				  break;
    	}

    	EthernetTasks();
    }
}

