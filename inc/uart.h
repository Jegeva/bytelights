#ifndef __UART_H
#define __UART_H
#include "stm32f1xx_ll_usart.h"
#include "stm32f1xx_ll_gpio.h"
#include "stm32f1xx_ll_rcc.h"
#include "stm32f1xx_ll_bus.h"
extern volatile uint8_t message_available ;
extern volatile uint8_t strip[15];
extern volatile uint8_t hat_kick;

void uart_init();


#endif
