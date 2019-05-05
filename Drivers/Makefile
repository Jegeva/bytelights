
CC=arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -g -mcpu=cortex-m3
AR=arm-none-eabi-ar

vpath %.c src

BUILD_DIR = build/

C_INCLUDES =  -Iinc \
-ISTM32F1xx_HAL_Driver/inc \
-ICMSIS/Device/ST/STM32F1xx/Include \
-ICMSIS/Include

CFLAGS = -ffreestanding -nostdlib
CFLAGS += -I./CMSIS/Include -Iinc -DARM_MATH_CM3
CFLAGS += -Iinc -DARM_MATH_CM3 $(C_INCLUDES) -DUSE_FULL_LL_DRIVER

SRCS = stm32f1xx_hal_adc.c\
	stm32f1xx_hal_adc_ex.c\
	stm32f1xx_hal.c\
	stm32f1xx_hal_can.c\
	stm32f1xx_hal_cec.c\
	stm32f1xx_hal_cortex.c\
	stm32f1xx_hal_crc.c\
	stm32f1xx_hal_dac.c\
	stm32f1xx_hal_dac_ex.c\
	stm32f1xx_hal_dma.c\
	stm32f1xx_hal_eth.c\
	stm32f1xx_hal_flash.c\
	stm32f1xx_hal_flash_ex.c\
	stm32f1xx_hal_gpio.c\
	stm32f1xx_hal_gpio_ex.c\
	stm32f1xx_hal_hcd.c\
	stm32f1xx_hal_i2c.c\
	stm32f1xx_hal_i2s.c\
	stm32f1xx_hal_irda.c\
	stm32f1xx_hal_iwdg.c\
	stm32f1xx_hal_mmc.c\
	stm32f1xx_hal_msp_template.c\
	stm32f1xx_hal_nand.c\
	stm32f1xx_hal_nor.c\
	stm32f1xx_hal_pccard.c\
	stm32f1xx_hal_pcd.c\
	stm32f1xx_hal_pcd_ex.c\
	stm32f1xx_hal_pwr.c\
	stm32f1xx_hal_rcc.c\
	stm32f1xx_hal_rcc_ex.c\
	stm32f1xx_hal_rtc.c\
	stm32f1xx_hal_rtc_ex.c\
	stm32f1xx_hal_sd.c\
	stm32f1xx_hal_smartcard.c\
	stm32f1xx_hal_spi.c\
	stm32f1xx_hal_spi_ex.c\
	stm32f1xx_hal_sram.c\
	stm32f1xx_hal_tim.c\
	stm32f1xx_hal_timebase_rtc_alarm_template.c\
	stm32f1xx_hal_timebase_tim_template.c\
	stm32f1xx_hal_tim_ex.c\
	stm32f1xx_hal_uart.c\
	stm32f1xx_hal_usart.c\
	stm32f1xx_hal_wwdg.c\
	stm32f1xx_ll_adc.c\
	stm32f1xx_ll_crc.c\
	stm32f1xx_ll_dac.c\
	stm32f1xx_ll_dma.c\
	stm32f1xx_ll_exti.c\
	stm32f1xx_ll_fsmc.c\
	stm32f1xx_ll_gpio.c\
	stm32f1xx_ll_i2c.c\
	stm32f1xx_ll_pwr.c\
	stm32f1xx_ll_rcc.c\
	stm32f1xx_ll_rtc.c\
	stm32f1xx_ll_sdmmc.c\
	stm32f1xx_ll_spi.c\
	stm32f1xx_ll_tim.c\
	stm32f1xx_ll_usart.c\
	stm32f1xx_ll_usb.c\
	stm32f1xx_ll_utils.c


OBJS = $(SRCS:%.c=$(BUILD_DIR)%.o)

all: libstm32f103c8.a

$(BUILD_DIR)%.o : STM32F1xx_HAL_Driver/src/%.c
	$(CC) $(CFLAGS) -c -o $@ $^

libstm32f103c8.a: $(OBJS)
	$(AR) -r $@ $(OBJS)

clean:
	rm -f $(OBJS) libstm32f103c8.a
