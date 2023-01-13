#include "board.h"
#include "fsl_lpuart.h"
#include "fsl_lpuart_edma.h"
#include "fsl_dmamux.h"

#include "remora.h"

#define NVMPG_LPUART                LPUART5
#define LPUART_CLK_FREQ           	BOARD_DebugConsoleSrcFreq()
#define LPUART_TX_DMA_CHANNEL       0U
#define LPUART_RX_DMA_CHANNEL       1U
#define LPUART_TX_DMA_REQUEST       kDmaRequestMuxLPUART5Tx
#define LPUART_RX_DMA_REQUEST       kDmaRequestMuxLPUART5Rx
#define LPUART_DMAMUX_BASEADDR 		DMAMUX
#define LPUART_DMA_BASEADDR    		DMA0

extern mpgData_t mpgData;
extern uint8_t mpgTxData[53];
extern uint8_t mpgRxData;
extern volatile bool mpgReceived;
extern volatile bool mpgSerial;

uint16_t mask;
bool buttonState;

lpuart_edma_handle_t g_lpuartEdmaHandle;
edma_handle_t g_lpuartTxEdmaHandle;
edma_handle_t g_lpuartRxEdmaHandle;
//lpuart_transfer_t ;
lpuart_transfer_t sendXfer;
//lpuart_transfer_t receiveXfer;



/* LPUART user callback */
//void LPUART_UserCallback(LPUART_Type *base, lpuart_edma_handle_t *handle, status_t status, void *userData);


void LPUART_UserCallback(LPUART_Type *base, lpuart_edma_handle_t *handle, status_t status, void *userData)
{
    if (kStatus_LPUART_TxIdle == status)
    {
        // Transmit callback
    }

    if (kStatus_LPUART_RxIdle == status)
    {
    	// Receive callback
    	//mpgSerial = true;
    	//LPUART_ReceiveEDMA(NVMPG_LPUART, &g_lpuartEdmaHandle, &receiveXfer);
    }
}


void initNVMPG()
{
    lpuart_config_t lpuartConfig;
    edma_config_t config;

    LPUART_GetDefaultConfig(&lpuartConfig);
    lpuartConfig.baudRate_Bps = 115200;
    lpuartConfig.enableTx     = true;
    lpuartConfig.enableRx     = true;

    LPUART_Init(NVMPG_LPUART, &lpuartConfig, LPUART_CLK_FREQ);

    /* Init DMAMUX */
    DMAMUX_Init(LPUART_DMAMUX_BASEADDR);

    /* Set channel for LPUART */
    DMAMUX_SetSource(LPUART_DMAMUX_BASEADDR, LPUART_TX_DMA_CHANNEL, LPUART_TX_DMA_REQUEST);
    DMAMUX_SetSource(LPUART_DMAMUX_BASEADDR, LPUART_RX_DMA_CHANNEL, LPUART_RX_DMA_REQUEST);
    DMAMUX_EnableChannel(LPUART_DMAMUX_BASEADDR, LPUART_TX_DMA_CHANNEL);
    DMAMUX_EnableChannel(LPUART_DMAMUX_BASEADDR, LPUART_RX_DMA_CHANNEL);

    /* Init the EDMA module */
    EDMA_GetDefaultConfig(&config);
    EDMA_Init(LPUART_DMA_BASEADDR, &config);
    EDMA_CreateHandle(&g_lpuartTxEdmaHandle, LPUART_DMA_BASEADDR, LPUART_TX_DMA_CHANNEL);
    EDMA_CreateHandle(&g_lpuartRxEdmaHandle, LPUART_DMA_BASEADDR, LPUART_RX_DMA_CHANNEL);

    /* Create LPUART DMA handle. */
    LPUART_TransferCreateHandleEDMA(NVMPG_LPUART, &g_lpuartEdmaHandle, LPUART_UserCallback, NULL, &g_lpuartTxEdmaHandle, &g_lpuartRxEdmaHandle);

	sendXfer.data     		= mpgTxData;
	sendXfer.dataSize 		= sizeof(mpgTxData) - 1;
}


void updateNVMPG()
{
	bool rxStatus;

	// Poll the LPUART to see if a button push byte has been received
	rxStatus = (bool)(LPUART_GetStatusFlags(NVMPG_LPUART)  & (uint32_t)kLPUART_RxFifoEmptyFlag);

	if (!rxStatus)
	{
		// data received, RxFifo is not empty
		LPUART_ReadBlocking(NVMPG_LPUART, &mpgRxData, 1);
		mpgSerial = true;
	}


	if (mpgSerial)
	{
		// get the button number from the low nibble, subtract 2 (buttons start from #2), NVMPG start at bit 26 in the uint64_t output structure
		mask = 1 << ((mpgRxData & 0x0f) - 2);

		// button state is from the high nibble, x0_ is button down (logical 1), x8_ is button up (logical 0)
		buttonState = (mpgRxData & 0x80);

		if (buttonState)
		{
			txData.NVMPGinputs &= ~mask;
		}
		else
		{
			txData.NVMPGinputs |= mask;
		}

		mpgRxData = 0;
		mpgSerial = false;
	}

	if (mpgReceived)
	{
		// copy the data to txData buffer
		for (int i = 0; i < 53; i++)
		{
			mpgTxData[i] =  mpgData.payload[i+4];
		}

	    LPUART_SendEDMA(NVMPG_LPUART, &g_lpuartEdmaHandle, &sendXfer);

		mpgReceived = false;
	}
}
