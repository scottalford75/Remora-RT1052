/***********************************************************************************************************************
 * This file was generated by the MCUXpresso Config Tools. Any manual edits made to this file
 * will be overwritten if the respective MCUXpresso Config Tools is used to update this file.
 **********************************************************************************************************************/

/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!GlobalInfo
product: Pins v12.0
processor: MIMXRT1052xxxxB
package_id: MIMXRT1052CVL5B
mcu_data: ksdk2_0
processor_version: 12.0.1
board: IMXRT1050-EVKB
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

#include "fsl_common.h"
#include "fsl_iomuxc.h"
#include "pin_mux.h"

/* FUNCTION ************************************************************************************************************
 * 
 * Function Name : BOARD_InitBootPins
 * Description   : Calls initialization functions.
 * 
 * END ****************************************************************************************************************/
void BOARD_InitBootPins(void) {
    BOARD_InitPins();
}

/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitPins:
- options: {callFromInitBoot: 'true', coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: B13, peripheral: ENET, signal: enet_ref_clk, pin_signal: GPIO_B1_10, software_input_on: Enable, hysteresis_enable: Disable, pull_up_down_config: Pull_Down_100K_Ohm,
    pull_keeper_select: Keeper, pull_keeper_enable: Disable, open_drain: Disable, speed: MHZ_50, drive_strength: R0_6, slew_rate: Fast}
  - {pin_num: E12, peripheral: ENET, signal: 'enet_rx_data, 0', pin_signal: GPIO_B1_04, software_input_on: Disable, hysteresis_enable: Disable, pull_up_down_config: Pull_Up_100K_Ohm,
    pull_keeper_select: Pull, pull_keeper_enable: Enable, open_drain: Disable, speed: MHZ_200, drive_strength: R0_5, slew_rate: Fast}
  - {pin_num: D12, peripheral: ENET, signal: 'enet_rx_data, 1', pin_signal: GPIO_B1_05, software_input_on: Disable, hysteresis_enable: Disable, pull_up_down_config: Pull_Up_100K_Ohm,
    pull_keeper_select: Pull, pull_keeper_enable: Enable, open_drain: Disable, speed: MHZ_200, drive_strength: R0_5, slew_rate: Fast}
  - {pin_num: C12, peripheral: ENET, signal: enet_rx_en, pin_signal: GPIO_B1_06, software_input_on: Disable, hysteresis_enable: Disable, pull_up_down_config: Pull_Up_100K_Ohm,
    pull_keeper_select: Pull, pull_keeper_enable: Enable, open_drain: Disable, speed: MHZ_200, drive_strength: R0_5, slew_rate: Fast}
  - {pin_num: C13, peripheral: ENET, signal: enet_rx_er, pin_signal: GPIO_B1_11, software_input_on: Disable, hysteresis_enable: Disable, pull_up_down_config: Pull_Up_100K_Ohm,
    pull_keeper_select: Pull, pull_keeper_enable: Enable, open_drain: Disable, speed: MHZ_200, drive_strength: R0_5, slew_rate: Fast}
  - {pin_num: B12, peripheral: ENET, signal: 'enet_tx_data, 0', pin_signal: GPIO_B1_07, software_input_on: Disable, hysteresis_enable: Disable, pull_up_down_config: Pull_Up_100K_Ohm,
    pull_keeper_select: Pull, pull_keeper_enable: Enable, open_drain: Disable, speed: MHZ_200, drive_strength: R0_5, slew_rate: Fast}
  - {pin_num: A12, peripheral: ENET, signal: 'enet_tx_data, 1', pin_signal: GPIO_B1_08, software_input_on: Disable, hysteresis_enable: Disable, pull_up_down_config: Pull_Up_100K_Ohm,
    pull_keeper_select: Pull, pull_keeper_enable: Enable, open_drain: Disable, speed: MHZ_200, drive_strength: R0_5, slew_rate: Fast}
  - {pin_num: A13, peripheral: ENET, signal: enet_tx_en, pin_signal: GPIO_B1_09, software_input_on: Disable, hysteresis_enable: Disable, pull_up_down_config: Pull_Up_100K_Ohm,
    pull_keeper_select: Pull, pull_keeper_enable: Enable, open_drain: Disable, speed: MHZ_200, drive_strength: R0_5, slew_rate: Fast}
  - {pin_num: G13, peripheral: GPIO1, signal: 'gpio_io, 10', pin_signal: GPIO_AD_B0_10}
  - {pin_num: C14, peripheral: ENET, signal: enet_mdc, pin_signal: GPIO_B1_14}
  - {pin_num: B14, peripheral: ENET, signal: enet_mdio, pin_signal: GPIO_B1_15}
  - {pin_num: B11, peripheral: LPUART4, signal: RX, pin_signal: GPIO_B1_01}
  - {pin_num: A11, peripheral: LPUART4, signal: TX, pin_signal: GPIO_B1_00}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitPins(void) {
  CLOCK_EnableClock(kCLOCK_Iomuxc);           

  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_10_GPIO1_IO10, 0U); 
  IOMUXC_SetPinMux(IOMUXC_GPIO_B1_00_LPUART4_TXD, 0U); 
  IOMUXC_SetPinMux(IOMUXC_GPIO_B1_01_LPUART4_RXD, 0U); 
  IOMUXC_SetPinMux(IOMUXC_GPIO_B1_12_LPUART5_TXD, 0U);
  IOMUXC_SetPinMux(IOMUXC_GPIO_B1_13_LPUART5_RXD, 0U);
  IOMUXC_SetPinMux(IOMUXC_GPIO_B1_04_ENET_RX_DATA00, 0U); 
  IOMUXC_SetPinMux(IOMUXC_GPIO_B1_05_ENET_RX_DATA01, 0U); 
  IOMUXC_SetPinMux(IOMUXC_GPIO_B1_06_ENET_RX_EN, 0U); 
  IOMUXC_SetPinMux(IOMUXC_GPIO_B1_07_ENET_TX_DATA00, 0U); 
  IOMUXC_SetPinMux(IOMUXC_GPIO_B1_08_ENET_TX_DATA01, 0U); 
  IOMUXC_SetPinMux(IOMUXC_GPIO_B1_09_ENET_TX_EN, 0U); 
  IOMUXC_SetPinMux(IOMUXC_GPIO_B1_10_ENET_REF_CLK, 1U); 
  IOMUXC_SetPinMux(IOMUXC_GPIO_B1_11_ENET_RX_ER, 0U); 
  IOMUXC_SetPinMux(IOMUXC_GPIO_B1_14_ENET_MDC, 0U); 
  IOMUXC_SetPinMux(IOMUXC_GPIO_B1_15_ENET_MDIO, 0U); 

  IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_04_ENET_RX_DATA00, 0xB0E9U); 
  IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_05_ENET_RX_DATA01, 0xB0E9U); 
  IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_06_ENET_RX_EN, 0xB0E9U); 
  IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_07_ENET_TX_DATA00, 0xB0E9U); 
  IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_08_ENET_TX_DATA01, 0xB0E9U); 
  IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_09_ENET_TX_EN, 0xB0E9U); 
  IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_10_ENET_REF_CLK, 0x31U); 
  IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_11_ENET_RX_ER, 0xB0E9U); 
}

/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
