/*
 * Copyright 2020-2021 NXP
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
product: Pins v10.0
processor: MIMXRT595S
package_id: MIMXRT595SFFOC
mcu_data: ksdk2_0
processor_version: 10.0.0
pin_labels:
- {pin_num: D6, pin_signal: PIO3_20/SD1_RESET_N/LCD_D21/CTIMER4_MAT2, label: SDIO_RST, identifier: SDIO_RST}
- {pin_num: D12, pin_signal: PIO0_31/FC4_CTS_SDA_SSEL0/SCT0_GPI0/SCT0_OUT6/CTIMER4_MAT3/FC3_SSEL2/SEC_PIO0_31, label: WL_RST, identifier: WL_RST}
- {pin_num: M14, pin_signal: PIO4_3/FC7_CTS_SDA_SSEL0, label: M2_3V3, identifier: M2_3V3}
- {pin_num: N15, pin_signal: PIO4_0/FC7_SCK/FREQME_GPIO_CLK/CLKOUT, label: SDC_3V3, identifier: SDC_3V3}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

#include "fsl_common.h"
#include "fsl_gpio.h"
#include "fsl_iopctl.h"
#include "pin_mux.h"

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitBootPins
 * Description   : Calls initialization functions.
 *
 * END ****************************************************************************************************************/
void BOARD_InitBootPins(void)
{
    BOARD_InitDEBUG_UARTPins();
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitDEBUG_UARTPins:
- options: {callFromInitBoot: 'true', coreID: cm33, enableClock: 'true'}
- pin_list:
  - {pin_num: N8, peripheral: FLEXCOMM12, signal: TXD_SCL_MISO, pin_signal: PIO4_30/LCD_D3/DBI_D3/FC12_TXD_SCL_MISO/FLEXIO_D10}
  - {pin_num: N10, peripheral: FLEXCOMM12, signal: RXD_SDA_MOSI, pin_signal: PIO4_31/LCD_D4/DBI_D4/FC12_RXD_SDA_MOSI/FLEXIO_D11, ibena: enabled}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitDEBUG_UARTPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
/* Function assigned for the Cortex-M33 */
void BOARD_InitDEBUG_UARTPins(void)
{

    const uint32_t port4_pin30_config = (/* Pin is configured as FC12_TXD_SCL_MISO */
                                         IOPCTL_PIO_FUNC6 |
                                         /* Disable pull-up / pull-down function */
                                         IOPCTL_PIO_PUPD_DI |
                                         /* Enable pull-down function */
                                         IOPCTL_PIO_PULLDOWN_EN |
                                         /* Disable input buffer function */
                                         IOPCTL_PIO_INBUF_DI |
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
    /* PORT4 PIN30 (coords: N8) is configured as FC12_TXD_SCL_MISO */
    IOPCTL_PinMuxSet(IOPCTL, 4U, 30U, port4_pin30_config);

    const uint32_t port4_pin31_config = (/* Pin is configured as FC12_RXD_SDA_MOSI */
                                         IOPCTL_PIO_FUNC6 |
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
    /* PORT4 PIN31 (coords: N10) is configured as FC12_RXD_SDA_MOSI */
    IOPCTL_PinMuxSet(IOPCTL, 4U, 31U, port4_pin31_config);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitPinsSD:
- options: {callFromInitBoot: 'false', coreID: cm33, enableClock: 'true'}
- pin_list:
  - {pin_num: T3, peripheral: USDHC0, signal: USDHC_CARD_DET, pin_signal: PIO2_9/SD0_CARD_DET_N/SCT0_OUT5/CTIMER1_MAT3/FC8_CTS_SDA_SSEL1/SMARTDMA_PIO9, pupdena: enabled,
    pupdsel: pullUp, ibena: enabled}
  - {pin_num: U4, peripheral: USDHC0, signal: 'USDHC_DATA, 0', pin_signal: PIO2_0/SD0_D0/SCT0_GPI2/SMARTDMA_PIO0, pupdena: enabled, pupdsel: pullUp, ibena: enabled}
  - {pin_num: T4, peripheral: USDHC0, signal: 'USDHC_DATA, 1', pin_signal: PIO2_1/SD0_D1/SCT0_GPI3/SMARTDMA_PIO1, pupdena: enabled, pupdsel: pullUp, ibena: enabled}
  - {pin_num: T7, peripheral: USDHC0, signal: 'USDHC_DATA, 2', pin_signal: PIO2_2/SD0_D2/SCT0_OUT0/SMARTDMA_PIO2, pupdena: enabled, pupdsel: pullUp, ibena: enabled}
  - {pin_num: U6, peripheral: USDHC0, signal: 'USDHC_DATA, 3', pin_signal: PIO2_3/SD0_D3/SCT0_OUT1/SMARTDMA_PIO3, pupdena: enabled, pupdsel: pullUp, ibena: enabled}
  - {pin_num: R6, peripheral: USDHC0, signal: USDHC_CMD, pin_signal: PIO1_31/SD0_CMD/SCT0_GPI1, pupdena: enabled, pupdsel: pullUp, ibena: enabled}
  - {pin_num: R5, peripheral: USDHC0, signal: USDHC_CLK, pin_signal: PIO1_30/SD0_CLK/SCT0_GPI0, ibena: enabled}
  - {pin_num: N15, peripheral: GPIO, signal: 'PIO4, 0', pin_signal: PIO4_0/FC7_SCK/FREQME_GPIO_CLK/CLKOUT, direction: OUTPUT, gpio_init_state: 'false'}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitPinsSD
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
/* Function assigned for the Cortex-M33 */
void BOARD_InitPinsSD(void)
{

    /* Enables the clock for the GPIO4 module */
    CLOCK_EnableClock(kCLOCK_HsGpio4);

    gpio_pin_config_t SDC_3V3_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 0U
    };
    /* Initialize GPIO functionality on pin PIO4_0 (pin N15)  */
    GPIO_PinInit(BOARD_INITPINSSD_SDC_3V3_GPIO, BOARD_INITPINSSD_SDC_3V3_PORT, BOARD_INITPINSSD_SDC_3V3_PIN, &SDC_3V3_config);

    const uint32_t port1_pin30_config = (/* Pin is configured as SD0_CLK */
                                         IOPCTL_PIO_FUNC1 |
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
    /* PORT1 PIN30 (coords: R5) is configured as SD0_CLK */
    IOPCTL_PinMuxSet(IOPCTL, 1U, 30U, port1_pin30_config);

    const uint32_t port1_pin31_config = (/* Pin is configured as SD0_CMD */
                                         IOPCTL_PIO_FUNC1 |
                                         /* Enable pull-up / pull-down function */
                                         IOPCTL_PIO_PUPD_EN |
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
    /* PORT1 PIN31 (coords: R6) is configured as SD0_CMD */
    IOPCTL_PinMuxSet(IOPCTL, 1U, 31U, port1_pin31_config);

    const uint32_t port2_pin0_config = (/* Pin is configured as SD0_D0 */
                                        IOPCTL_PIO_FUNC1 |
                                        /* Enable pull-up / pull-down function */
                                        IOPCTL_PIO_PUPD_EN |
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
    /* PORT2 PIN0 (coords: U4) is configured as SD0_D0 */
    IOPCTL_PinMuxSet(IOPCTL, 2U, 0U, port2_pin0_config);

    const uint32_t port2_pin1_config = (/* Pin is configured as SD0_D1 */
                                        IOPCTL_PIO_FUNC1 |
                                        /* Enable pull-up / pull-down function */
                                        IOPCTL_PIO_PUPD_EN |
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
    /* PORT2 PIN1 (coords: T4) is configured as SD0_D1 */
    IOPCTL_PinMuxSet(IOPCTL, 2U, 1U, port2_pin1_config);

    const uint32_t port2_pin2_config = (/* Pin is configured as SD0_D2 */
                                        IOPCTL_PIO_FUNC1 |
                                        /* Enable pull-up / pull-down function */
                                        IOPCTL_PIO_PUPD_EN |
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
    /* PORT2 PIN2 (coords: T7) is configured as SD0_D2 */
    IOPCTL_PinMuxSet(IOPCTL, 2U, 2U, port2_pin2_config);

    const uint32_t port2_pin3_config = (/* Pin is configured as SD0_D3 */
                                        IOPCTL_PIO_FUNC1 |
                                        /* Enable pull-up / pull-down function */
                                        IOPCTL_PIO_PUPD_EN |
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
    /* PORT2 PIN3 (coords: U6) is configured as SD0_D3 */
    IOPCTL_PinMuxSet(IOPCTL, 2U, 3U, port2_pin3_config);

    const uint32_t port2_pin9_config = (/* Pin is configured as SD0_CARD_DET_N */
                                        IOPCTL_PIO_FUNC1 |
                                        /* Enable pull-up / pull-down function */
                                        IOPCTL_PIO_PUPD_EN |
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
    /* PORT2 PIN9 (coords: T3) is configured as SD0_CARD_DET_N */
    IOPCTL_PinMuxSet(IOPCTL, 2U, 9U, port2_pin9_config);

    const uint32_t SDC_3V3 = (/* Pin is configured as PIO4_0 */
                              IOPCTL_PIO_FUNC0 |
                              /* Disable pull-up / pull-down function */
                              IOPCTL_PIO_PUPD_DI |
                              /* Enable pull-down function */
                              IOPCTL_PIO_PULLDOWN_EN |
                              /* Disable input buffer function */
                              IOPCTL_PIO_INBUF_DI |
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
    /* PORT4 PIN0 (coords: N15) is configured as PIO4_0 */
    IOPCTL_PinMuxSet(IOPCTL, BOARD_INITPINSSD_SDC_3V3_PORT, BOARD_INITPINSSD_SDC_3V3_PIN, SDC_3V3);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitPinsM2:
- options: {callFromInitBoot: 'false', coreID: cm33, enableClock: 'true'}
- pin_list:
  - {pin_num: C8, peripheral: USDHC1, signal: 'USDHC_DATA, 0', pin_signal: PIO3_10/SD1_D0/LCD_D11/CTIMER0_MAT2/FC10_RXD_SDA_MOSI, pupdena: enabled, pupdsel: pullUp,
    ibena: enabled}
  - {pin_num: C10, peripheral: USDHC1, signal: 'USDHC_DATA, 1', pin_signal: PIO3_11/SD1_D1/LCD_D12/CTIMER0_MAT3/FC10_CTS_SDA_SSELN0, pupdena: enabled, pupdsel: pullUp,
    ibena: enabled}
  - {pin_num: A6, peripheral: USDHC1, signal: 'USDHC_DATA, 2', pin_signal: PIO3_12/SD1_D2/LCD_D13/CTIMER_INP0/FC10_RTS_SCL_SSELN1, pupdena: enabled, pupdsel: pullUp,
    ibena: enabled}
  - {pin_num: B7, peripheral: USDHC1, signal: 'USDHC_DATA, 3', pin_signal: PIO3_13/SD1_D3/LCD_D14/CTIMER_INP1/FC10_SSELN2, pupdena: enabled, pupdsel: pullUp, ibena: enabled}
  - {pin_num: B8, peripheral: USDHC1, signal: USDHC_CMD, pin_signal: PIO3_9/SD1_CMD/LCD_D10/CTIMER0_MAT1/FC10_TXD_SCL_MISO, ibena: enabled}
  - {pin_num: A8, peripheral: USDHC1, signal: USDHC_CLK, pin_signal: PIO3_8/SD1_CLK/LCD_D9/CTIMER0_MAT0/FC10_SCK, ibena: enabled}
  - {pin_num: D6, peripheral: GPIO, signal: 'PIO3, 20', pin_signal: PIO3_20/SD1_RESET_N/LCD_D21/CTIMER4_MAT2, direction: OUTPUT, gpio_init_state: 'false'}
  - {pin_num: D12, peripheral: GPIO, signal: 'PIO0, 31', pin_signal: PIO0_31/FC4_CTS_SDA_SSEL0/SCT0_GPI0/SCT0_OUT6/CTIMER4_MAT3/FC3_SSEL2/SEC_PIO0_31, direction: OUTPUT,
    gpio_init_state: 'false'}
  - {pin_num: M14, peripheral: GPIO, signal: 'PIO4, 3', pin_signal: PIO4_3/FC7_CTS_SDA_SSEL0, direction: OUTPUT, gpio_init_state: 'false'}
  - {pin_num: E15, peripheral: CLKCTL, signal: 32KHZ_CLKOUT, pin_signal: PIO2_14/SCT0_OUT8/CTIMER_INP1/32KHZ_CLKOUT/SMARTDMA_PIO14/CMP0_A}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitPinsM2
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
/* Function assigned for the Cortex-M33 */
void BOARD_InitPinsM2(void)
{

    /* Enables the clock for the GPIO0 module */
    CLOCK_EnableClock(kCLOCK_HsGpio0);

    /* Enables the clock for the GPIO3 module */
    CLOCK_EnableClock(kCLOCK_HsGpio3);

    /* Enables the clock for the GPIO4 module */
    CLOCK_EnableClock(kCLOCK_HsGpio4);

    gpio_pin_config_t WL_RST_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 0U
    };
    /* Initialize GPIO functionality on pin PIO0_31 (pin D12)  */
    GPIO_PinInit(BOARD_INITPINSM2_WL_RST_GPIO, BOARD_INITPINSM2_WL_RST_PORT, BOARD_INITPINSM2_WL_RST_PIN, &WL_RST_config);

    gpio_pin_config_t SDIO_RST_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 0U
    };
    /* Initialize GPIO functionality on pin PIO3_20 (pin D6)  */
    GPIO_PinInit(BOARD_INITPINSM2_SDIO_RST_GPIO, BOARD_INITPINSM2_SDIO_RST_PORT, BOARD_INITPINSM2_SDIO_RST_PIN, &SDIO_RST_config);

    gpio_pin_config_t M2_3V3_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 0U
    };
    /* Initialize GPIO functionality on pin PIO4_3 (pin M14)  */
    GPIO_PinInit(BOARD_INITPINSM2_M2_3V3_GPIO, BOARD_INITPINSM2_M2_3V3_PORT, BOARD_INITPINSM2_M2_3V3_PIN, &M2_3V3_config);

    const uint32_t WL_RST = (/* Pin is configured as PIO0_31 */
                             IOPCTL_PIO_FUNC0 |
                             /* Disable pull-up / pull-down function */
                             IOPCTL_PIO_PUPD_DI |
                             /* Enable pull-down function */
                             IOPCTL_PIO_PULLDOWN_EN |
                             /* Disable input buffer function */
                             IOPCTL_PIO_INBUF_DI |
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
    /* PORT0 PIN31 (coords: D12) is configured as PIO0_31 */
    IOPCTL_PinMuxSet(IOPCTL, BOARD_INITPINSM2_WL_RST_PORT, BOARD_INITPINSM2_WL_RST_PIN, WL_RST);

    const uint32_t port2_pin14_config = (/* Pin is configured as 32KHZ_CLKOUT */
                                         IOPCTL_PIO_FUNC7 |
                                         /* Disable pull-up / pull-down function */
                                         IOPCTL_PIO_PUPD_DI |
                                         /* Enable pull-down function */
                                         IOPCTL_PIO_PULLDOWN_EN |
                                         /* Disable input buffer function */
                                         IOPCTL_PIO_INBUF_DI |
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
    /* PORT2 PIN14 (coords: E15) is configured as 32KHZ_CLKOUT */
    IOPCTL_PinMuxSet(IOPCTL, 2U, 14U, port2_pin14_config);

    const uint32_t port3_pin10_config = (/* Pin is configured as SD1_D0 */
                                         IOPCTL_PIO_FUNC1 |
                                         /* Enable pull-up / pull-down function */
                                         IOPCTL_PIO_PUPD_EN |
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
    /* PORT3 PIN10 (coords: C8) is configured as SD1_D0 */
    IOPCTL_PinMuxSet(IOPCTL, 3U, 10U, port3_pin10_config);

    const uint32_t port3_pin11_config = (/* Pin is configured as SD1_D1 */
                                         IOPCTL_PIO_FUNC1 |
                                         /* Enable pull-up / pull-down function */
                                         IOPCTL_PIO_PUPD_EN |
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
    /* PORT3 PIN11 (coords: C10) is configured as SD1_D1 */
    IOPCTL_PinMuxSet(IOPCTL, 3U, 11U, port3_pin11_config);

    const uint32_t port3_pin12_config = (/* Pin is configured as SD1_D2 */
                                         IOPCTL_PIO_FUNC1 |
                                         /* Enable pull-up / pull-down function */
                                         IOPCTL_PIO_PUPD_EN |
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
    /* PORT3 PIN12 (coords: A6) is configured as SD1_D2 */
    IOPCTL_PinMuxSet(IOPCTL, 3U, 12U, port3_pin12_config);

    const uint32_t port3_pin13_config = (/* Pin is configured as SD1_D3 */
                                         IOPCTL_PIO_FUNC1 |
                                         /* Enable pull-up / pull-down function */
                                         IOPCTL_PIO_PUPD_EN |
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
    /* PORT3 PIN13 (coords: B7) is configured as SD1_D3 */
    IOPCTL_PinMuxSet(IOPCTL, 3U, 13U, port3_pin13_config);

    const uint32_t SDIO_RST = (/* Pin is configured as PIO3_20 */
                               IOPCTL_PIO_FUNC0 |
                               /* Disable pull-up / pull-down function */
                               IOPCTL_PIO_PUPD_DI |
                               /* Enable pull-down function */
                               IOPCTL_PIO_PULLDOWN_EN |
                               /* Disable input buffer function */
                               IOPCTL_PIO_INBUF_DI |
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
    /* PORT3 PIN20 (coords: D6) is configured as PIO3_20 */
    IOPCTL_PinMuxSet(IOPCTL, BOARD_INITPINSM2_SDIO_RST_PORT, BOARD_INITPINSM2_SDIO_RST_PIN, SDIO_RST);

    const uint32_t port3_pin8_config = (/* Pin is configured as SD1_CLK */
                                        IOPCTL_PIO_FUNC1 |
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
    /* PORT3 PIN8 (coords: A8) is configured as SD1_CLK */
    IOPCTL_PinMuxSet(IOPCTL, 3U, 8U, port3_pin8_config);

    const uint32_t port3_pin9_config = (/* Pin is configured as SD1_CMD */
                                        IOPCTL_PIO_FUNC1 |
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
    /* PORT3 PIN9 (coords: B8) is configured as SD1_CMD */
    IOPCTL_PinMuxSet(IOPCTL, 3U, 9U, port3_pin9_config);

    const uint32_t M2_3V3 = (/* Pin is configured as PIO4_3 */
                             IOPCTL_PIO_FUNC0 |
                             /* Disable pull-up / pull-down function */
                             IOPCTL_PIO_PUPD_DI |
                             /* Enable pull-down function */
                             IOPCTL_PIO_PULLDOWN_EN |
                             /* Disable input buffer function */
                             IOPCTL_PIO_INBUF_DI |
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
    /* PORT4 PIN3 (coords: M14) is configured as PIO4_3 */
    IOPCTL_PinMuxSet(IOPCTL, BOARD_INITPINSM2_M2_3V3_PORT, BOARD_INITPINSM2_M2_3V3_PIN, M2_3V3);
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
