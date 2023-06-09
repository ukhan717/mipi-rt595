/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


/***********************************************************************************************************************
 * This file was generated by the MCUXpresso Config Tools. Any manual edits made to this file
 * will be overwritten if the respective MCUXpresso Config Tools is used to update this file.
 **********************************************************************************************************************/

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!GlobalInfo
product: Pins v9.0
processor: MIMXRT595S
package_id: MIMXRT595SFFOC
mcu_data: ksdk2_0
processor_version: 0.9.0
pin_labels:
- {pin_num: B11, pin_signal: PIO0_29/FC4_TXD_SCL_MISO_WS/CTIMER4_MAT1/I2S_BRIDGE_WS_OUT/SEC_PIO0_29, label: UART_TX, identifier: UART_TX}
- {pin_num: D14, pin_signal: PIO0_30/FC4_RXD_SDA_MOSI_DATA/CTIMER4_MAT2/I2S_BRIDGE_DATA_OUT/SEC_PIO0_30, label: UART_RX, identifier: UART_RX}
- {pin_num: J15, pin_signal: PIO0_7/FC1_SCK/SCT0_GPI4/SCT0_OUT4/CTIMER1_MAT0/I2S_BRIDGE_CLK_OUT/SEC_PIO0_7, label: I2S_CLK, identifier: I2S_CLK;I3S_CLK}
- {pin_num: H12, pin_signal: PIO0_8/FC1_TXD_SCL_MISO_WS/SCT0_GPI5/SCT0_OUT5/CTIMER1_MAT1/I2S_BRIDGE_WS_OUT/SEC_PIO0_8, label: I2S_TX, identifier: I3S_TX;I2S_TX}
- {pin_num: H17, pin_signal: PIO0_9/FC1_RXD_SDA_MOSI_DATA/SCT0_GPI6/SCT0_OUT6/CTIMER1_MAT2/I2S_BRIDGE_DATA_OUT/SEC_PIO0_9, label: I2S_RX, identifier: I3S_RX;I2S_RX}
- {pin_num: P2, pin_signal: PIO5_4/LCD_D9/DBI_D9/PDM_CLK01, label: DMIC_CLK, identifier: DMIC_CLK}
- {pin_num: P3, pin_signal: PIO5_8/LCD_D13/DBI_D13/PDM_DATA01, label: DMIC_DATA01, identifier: DMIC_DATA01}
- {pin_num: D16, pin_signal: PIO3_1/PDM_CLK23/PDM_DATA23/FC0_TXD_SCL_MISO_WS/I3C1_SCL, label: DMIC_DATA23, identifier: DMIC_DATA23}
- {pin_num: C16, pin_signal: PIO3_2/PDM_CLK45/PDM_DATA45/FC0_RXD_SDA_MOSI_DATA/I3C1_SDA, label: DMIC_DATA45, identifier: DMIC_DATA45}
- {pin_num: D15, pin_signal: PIO3_3/PDM_CLK67/PDM_DATA67/LCD_D23/FC0_CTS_SDA_SSEL0/I3C1_PUR/CMP0_OUT, label: DMIC_DATA67, identifier: DMIC_DATA67}
- {pin_num: C13, pin_signal: PIO0_23/FC3_RXD_SDA_MOSI_DATA/SCT0_GPI7/SCT0_OUT8/CTIMER3_MAT2/CTIMER0_MAT3/TRACEDATA_1/SEC_PIO0_23, label: I2S3, identifier: I2S3}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

#include "fsl_common.h"
#include "fsl_iopctl.h"
#include "fsl_inputmux.h"
#include "pin_mux.h"

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitBootPins
 * Description   : Calls initialization functions.
 *
 * END ****************************************************************************************************************/
void BOARD_InitBootPins(void)
{
    BOARD_InitPins();
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitPins:
- options: {callFromInitBoot: 'true', coreID: cm33, enableClock: 'true'}
- pin_list:
  - {pin_num: B11, peripheral: FLEXCOMM4, signal: TXD_SCL_MISO_WS, pin_signal: PIO0_29/FC4_TXD_SCL_MISO_WS/CTIMER4_MAT1/I2S_BRIDGE_WS_OUT/SEC_PIO0_29, direction: OUTPUT,
    pupdena: disabled, pupdsel: pullUp, ibena: enabled, slew_rate: normal}
  - {pin_num: D14, peripheral: FLEXCOMM4, signal: RXD_SDA_MOSI_DATA, pin_signal: PIO0_30/FC4_RXD_SDA_MOSI_DATA/CTIMER4_MAT2/I2S_BRIDGE_DATA_OUT/SEC_PIO0_30, direction: INPUT,
    pupdsel: pullUp, ibena: enabled}
  - {pin_num: J15, peripheral: FLEXCOMM1, signal: SCK, pin_signal: PIO0_7/FC1_SCK/SCT0_GPI4/SCT0_OUT4/CTIMER1_MAT0/I2S_BRIDGE_CLK_OUT/SEC_PIO0_7, identifier: I2S_CLK,
    direction: INPUT, pupdena: enabled, pupdsel: pullUp, ibena: enabled, drive: full}
  - {pin_num: H12, peripheral: FLEXCOMM1, signal: TXD_SCL_MISO_WS, pin_signal: PIO0_8/FC1_TXD_SCL_MISO_WS/SCT0_GPI5/SCT0_OUT5/CTIMER1_MAT1/I2S_BRIDGE_WS_OUT/SEC_PIO0_8,
    identifier: I2S_TX, direction: OUTPUT, pupdena: enabled, pupdsel: pullUp, ibena: enabled, drive: full}
  - {pin_num: H17, peripheral: FLEXCOMM1, signal: RXD_SDA_MOSI_DATA, pin_signal: PIO0_9/FC1_RXD_SDA_MOSI_DATA/SCT0_GPI6/SCT0_OUT6/CTIMER1_MAT2/I2S_BRIDGE_DATA_OUT/SEC_PIO0_9,
    identifier: I2S_RX, direction: INPUT, pupdena: enabled, pupdsel: pullUp, ibena: enabled, drive: full}
  - {pin_num: C13, peripheral: FLEXCOMM3, signal: RXD_SDA_MOSI_DATA, pin_signal: PIO0_23/FC3_RXD_SDA_MOSI_DATA/SCT0_GPI7/SCT0_OUT8/CTIMER3_MAT2/CTIMER0_MAT3/TRACEDATA_1/SEC_PIO0_23,
    direction: INPUT, ibena: enabled, drive: full}
  - {pin_num: P2, peripheral: DMIC0, signal: 'CLK, 01', pin_signal: PIO5_4/LCD_D9/DBI_D9/PDM_CLK01, pupdena: disabled, ibena: enabled}
  - {pin_num: P3, peripheral: DMIC0, signal: 'DATA, 01', pin_signal: PIO5_8/LCD_D13/DBI_D13/PDM_DATA01, pupdena: disabled, ibena: enabled}
  - {peripheral: FUSIONF1, signal: 'DSP_INTERRUPT, 23', pin_signal: DMAC1}
  - {pin_num: K2, peripheral: FLEXCOMM5, signal: SCK, pin_signal: PIO1_3/FC5_SCK/HS_SPI1_SCK, pupdena: enabled, pupdsel: pullUp, ibena: enabled, drive: full}
  - {pin_num: K1, peripheral: FLEXCOMM5, signal: TXD_SCL_MISO_WS, pin_signal: PIO1_4/FC5_TXD_SCL_MISO_WS/HS_SPI1_MISO, pupdena: enabled, pupdsel: pullUp, ibena: enabled,
    drive: full}
  - {pin_num: L2, peripheral: FLEXCOMM5, signal: RXD_SDA_MOSI_DATA, pin_signal: PIO1_5/FC5_RXD_SDA_MOSI_DATA/HS_SPI1_MOSI, pupdena: enabled, pupdsel: pullUp, ibena: enabled,
    drive: full}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
/* Function assigned for the Cortex-M33 */
void BOARD_InitPins(void)
{
    /* Enables the clock for the Input Mux */
    CLOCK_EnableClock(kCLOCK_InputMux);
    /* DMAC1 interrupt signal is selected for DSP interrupt input 23 */
    INPUTMUX_AttachSignal(INPUTMUX, 23U, kINPUTMUX_Dmac1ToDspInterrupt);

    const uint32_t port0_pin23_config = (/* Pin is configured as FC3_RXD_SDA_MOSI_DATA */
                                         IOPCTL_PIO_FUNC1 |
                                         /* Disable pull-up / pull-down function */
                                         IOPCTL_PIO_PUPD_DI |
                                         /* Enable pull-down function */
                                         IOPCTL_PIO_PULLDOWN_EN |
                                         /* Enables input buffer function */
                                         IOPCTL_PIO_INBUF_EN |
                                         /* Normal mode */
                                         IOPCTL_PIO_SLEW_RATE_NORMAL |
                                         /* Full drive enable */
                                         IOPCTL_PIO_FULLDRIVE_EN |
                                         /* Analog mux is disabled */
                                         IOPCTL_PIO_ANAMUX_DI |
                                         /* Pseudo Output Drain is disabled */
                                         IOPCTL_PIO_PSEDRAIN_DI |
                                         /* Input function is not inverted */
                                         IOPCTL_PIO_INV_DI);
    /* PORT0 PIN23 (coords: C13) is configured as FC3_RXD_SDA_MOSI_DATA */
    IOPCTL_PinMuxSet(IOPCTL, 0U, 23U, port0_pin23_config);

    const uint32_t port0_pin29_config = (/* Pin is configured as FC4_TXD_SCL_MISO_WS */
                                         IOPCTL_PIO_FUNC1 |
                                         /* Disable pull-up / pull-down function */
                                         IOPCTL_PIO_PUPD_DI |
                                         /* Enable pull-up function */
                                         IOPCTL_PIO_PULLUP_EN |
                                         /* Enables input buffer function */
                                         IOPCTL_PIO_INBUF_EN |
                                         /* Normal mode */
                                         IOPCTL_PIO_SLEW_RATE_NORMAL |
                                         /* Normal drive */
                                         IOPCTL_PIO_FULLDRIVE_DI |
                                         /* Analog mux is disabled */
                                         IOPCTL_PIO_ANAMUX_DI |
                                         /* Pseudo Output Drain is disabled */
                                         IOPCTL_PIO_PSEDRAIN_DI |
                                         /* Input function is not inverted */
                                         IOPCTL_PIO_INV_DI);
    /* PORT0 PIN29 (coords: B11) is configured as FC4_TXD_SCL_MISO_WS */
    IOPCTL_PinMuxSet(IOPCTL, 0U, 29U, port0_pin29_config);

    const uint32_t port0_pin30_config = (/* Pin is configured as FC4_RXD_SDA_MOSI_DATA */
                                         IOPCTL_PIO_FUNC1 |
                                         /* Disable pull-up / pull-down function */
                                         IOPCTL_PIO_PUPD_DI |
                                         /* Enable pull-up function */
                                         IOPCTL_PIO_PULLUP_EN |
                                         /* Enables input buffer function */
                                         IOPCTL_PIO_INBUF_EN |
                                         /* Normal mode */
                                         IOPCTL_PIO_SLEW_RATE_NORMAL |
                                         /* Normal drive */
                                         IOPCTL_PIO_FULLDRIVE_DI |
                                         /* Analog mux is disabled */
                                         IOPCTL_PIO_ANAMUX_DI |
                                         /* Pseudo Output Drain is disabled */
                                         IOPCTL_PIO_PSEDRAIN_DI |
                                         /* Input function is not inverted */
                                         IOPCTL_PIO_INV_DI);
    /* PORT0 PIN30 (coords: D14) is configured as FC4_RXD_SDA_MOSI_DATA */
    IOPCTL_PinMuxSet(IOPCTL, 0U, 30U, port0_pin30_config);

    const uint32_t port0_pin7_config = (/* Pin is configured as FC1_SCK */
                                        IOPCTL_PIO_FUNC1 |
                                        /* Enable pull-up / pull-down function */
                                        IOPCTL_PIO_PUPD_EN |
                                        /* Enable pull-up function */
                                        IOPCTL_PIO_PULLUP_EN |
                                        /* Enables input buffer function */
                                        IOPCTL_PIO_INBUF_EN |
                                        /* Normal mode */
                                        IOPCTL_PIO_SLEW_RATE_NORMAL |
                                        /* Full drive enable */
                                        IOPCTL_PIO_FULLDRIVE_EN |
                                        /* Analog mux is disabled */
                                        IOPCTL_PIO_ANAMUX_DI |
                                        /* Pseudo Output Drain is disabled */
                                        IOPCTL_PIO_PSEDRAIN_DI |
                                        /* Input function is not inverted */
                                        IOPCTL_PIO_INV_DI);
    /* PORT0 PIN7 (coords: J15) is configured as FC1_SCK */
    IOPCTL_PinMuxSet(IOPCTL, 0U, 7U, port0_pin7_config);

    const uint32_t port0_pin8_config = (/* Pin is configured as FC1_TXD_SCL_MISO_WS */
                                        IOPCTL_PIO_FUNC1 |
                                        /* Enable pull-up / pull-down function */
                                        IOPCTL_PIO_PUPD_EN |
                                        /* Enable pull-up function */
                                        IOPCTL_PIO_PULLUP_EN |
                                        /* Enables input buffer function */
                                        IOPCTL_PIO_INBUF_EN |
                                        /* Normal mode */
                                        IOPCTL_PIO_SLEW_RATE_NORMAL |
                                        /* Full drive enable */
                                        IOPCTL_PIO_FULLDRIVE_EN |
                                        /* Analog mux is disabled */
                                        IOPCTL_PIO_ANAMUX_DI |
                                        /* Pseudo Output Drain is disabled */
                                        IOPCTL_PIO_PSEDRAIN_DI |
                                        /* Input function is not inverted */
                                        IOPCTL_PIO_INV_DI);
    /* PORT0 PIN8 (coords: H12) is configured as FC1_TXD_SCL_MISO_WS */
    IOPCTL_PinMuxSet(IOPCTL, 0U, 8U, port0_pin8_config);

    const uint32_t port0_pin9_config = (/* Pin is configured as FC1_RXD_SDA_MOSI_DATA */
                                        IOPCTL_PIO_FUNC1 |
                                        /* Enable pull-up / pull-down function */
                                        IOPCTL_PIO_PUPD_EN |
                                        /* Enable pull-up function */
                                        IOPCTL_PIO_PULLUP_EN |
                                        /* Enables input buffer function */
                                        IOPCTL_PIO_INBUF_EN |
                                        /* Normal mode */
                                        IOPCTL_PIO_SLEW_RATE_NORMAL |
                                        /* Full drive enable */
                                        IOPCTL_PIO_FULLDRIVE_EN |
                                        /* Analog mux is disabled */
                                        IOPCTL_PIO_ANAMUX_DI |
                                        /* Pseudo Output Drain is disabled */
                                        IOPCTL_PIO_PSEDRAIN_DI |
                                        /* Input function is not inverted */
                                        IOPCTL_PIO_INV_DI);
    /* PORT0 PIN9 (coords: H17) is configured as FC1_RXD_SDA_MOSI_DATA */
    IOPCTL_PinMuxSet(IOPCTL, 0U, 9U, port0_pin9_config);

    const uint32_t port1_pin3_config = (/* Pin is configured as FC5_SCK */
                                        IOPCTL_PIO_FUNC1 |
                                        /* Enable pull-up / pull-down function */
                                        IOPCTL_PIO_PUPD_EN |
                                        /* Enable pull-up function */
                                        IOPCTL_PIO_PULLUP_EN |
                                        /* Enables input buffer function */
                                        IOPCTL_PIO_INBUF_EN |
                                        /* Normal mode */
                                        IOPCTL_PIO_SLEW_RATE_NORMAL |
                                        /* Full drive enable */
                                        IOPCTL_PIO_FULLDRIVE_EN |
                                        /* Analog mux is disabled */
                                        IOPCTL_PIO_ANAMUX_DI |
                                        /* Pseudo Output Drain is disabled */
                                        IOPCTL_PIO_PSEDRAIN_DI |
                                        /* Input function is not inverted */
                                        IOPCTL_PIO_INV_DI);
    /* PORT1 PIN3 (coords: K2) is configured as FC5_SCK */
    IOPCTL_PinMuxSet(IOPCTL, 1U, 3U, port1_pin3_config);

    const uint32_t port1_pin4_config = (/* Pin is configured as FC5_TXD_SCL_MISO_WS */
                                        IOPCTL_PIO_FUNC1 |
                                        /* Enable pull-up / pull-down function */
                                        IOPCTL_PIO_PUPD_EN |
                                        /* Enable pull-up function */
                                        IOPCTL_PIO_PULLUP_EN |
                                        /* Enables input buffer function */
                                        IOPCTL_PIO_INBUF_EN |
                                        /* Normal mode */
                                        IOPCTL_PIO_SLEW_RATE_NORMAL |
                                        /* Full drive enable */
                                        IOPCTL_PIO_FULLDRIVE_EN |
                                        /* Analog mux is disabled */
                                        IOPCTL_PIO_ANAMUX_DI |
                                        /* Pseudo Output Drain is disabled */
                                        IOPCTL_PIO_PSEDRAIN_DI |
                                        /* Input function is not inverted */
                                        IOPCTL_PIO_INV_DI);
    /* PORT1 PIN4 (coords: K1) is configured as FC5_TXD_SCL_MISO_WS */
    IOPCTL_PinMuxSet(IOPCTL, 1U, 4U, port1_pin4_config);

    const uint32_t port1_pin5_config = (/* Pin is configured as FC5_RXD_SDA_MOSI_DATA */
                                        IOPCTL_PIO_FUNC1 |
                                        /* Enable pull-up / pull-down function */
                                        IOPCTL_PIO_PUPD_EN |
                                        /* Enable pull-up function */
                                        IOPCTL_PIO_PULLUP_EN |
                                        /* Enables input buffer function */
                                        IOPCTL_PIO_INBUF_EN |
                                        /* Normal mode */
                                        IOPCTL_PIO_SLEW_RATE_NORMAL |
                                        /* Full drive enable */
                                        IOPCTL_PIO_FULLDRIVE_EN |
                                        /* Analog mux is disabled */
                                        IOPCTL_PIO_ANAMUX_DI |
                                        /* Pseudo Output Drain is disabled */
                                        IOPCTL_PIO_PSEDRAIN_DI |
                                        /* Input function is not inverted */
                                        IOPCTL_PIO_INV_DI);
    /* PORT1 PIN5 (coords: L2) is configured as FC5_RXD_SDA_MOSI_DATA */
    IOPCTL_PinMuxSet(IOPCTL, 1U, 5U, port1_pin5_config);

    const uint32_t port5_pin4_config = (/* Pin is configured as PDM_CLK01 */
                                        IOPCTL_PIO_FUNC4 |
                                        /* Disable pull-up / pull-down function */
                                        IOPCTL_PIO_PUPD_DI |
                                        /* Enable pull-down function */
                                        IOPCTL_PIO_PULLDOWN_EN |
                                        /* Enables input buffer function */
                                        IOPCTL_PIO_INBUF_EN |
                                        /* Normal mode */
                                        IOPCTL_PIO_SLEW_RATE_NORMAL |
                                        /* Normal drive */
                                        IOPCTL_PIO_FULLDRIVE_DI |
                                        /* Analog mux is disabled */
                                        IOPCTL_PIO_ANAMUX_DI |
                                        /* Pseudo Output Drain is disabled */
                                        IOPCTL_PIO_PSEDRAIN_DI |
                                        /* Input function is not inverted */
                                        IOPCTL_PIO_INV_DI);
    /* PORT5 PIN4 (coords: P2) is configured as PDM_CLK01 */
    IOPCTL_PinMuxSet(IOPCTL, 5U, 4U, port5_pin4_config);

    const uint32_t port5_pin8_config = (/* Pin is configured as PDM_DATA01 */
                                        IOPCTL_PIO_FUNC4 |
                                        /* Disable pull-up / pull-down function */
                                        IOPCTL_PIO_PUPD_DI |
                                        /* Enable pull-down function */
                                        IOPCTL_PIO_PULLDOWN_EN |
                                        /* Enables input buffer function */
                                        IOPCTL_PIO_INBUF_EN |
                                        /* Normal mode */
                                        IOPCTL_PIO_SLEW_RATE_NORMAL |
                                        /* Normal drive */
                                        IOPCTL_PIO_FULLDRIVE_DI |
                                        /* Analog mux is disabled */
                                        IOPCTL_PIO_ANAMUX_DI |
                                        /* Pseudo Output Drain is disabled */
                                        IOPCTL_PIO_PSEDRAIN_DI |
                                        /* Input function is not inverted */
                                        IOPCTL_PIO_INV_DI);
    /* PORT5 PIN8 (coords: P3) is configured as PDM_DATA01 */
    IOPCTL_PinMuxSet(IOPCTL, 5U, 8U, port5_pin8_config);
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
