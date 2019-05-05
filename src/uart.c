#include "uart.h"

#define USART3_PORT GPIOB
#define USART3_PIN_RX LL_GPIO_PIN_11
#define USART3_PIN_TX LL_GPIO_PIN_10

void uart_init()
{
   LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOB);
   LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART3);
   LL_GPIO_InitTypeDef GPIO_InitStructure;
   LL_USART_InitTypeDef USART_InitStruct;
   LL_USART_ClockInitTypeDef USART_ClockInitStruct;
   LL_GPIO_StructInit(&GPIO_InitStructure);



   GPIO_InitStructure.Mode = LL_GPIO_MODE_ALTERNATE;
   GPIO_InitStructure.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
   GPIO_InitStructure.Speed = LL_GPIO_MODE_OUTPUT_50MHz;
   GPIO_InitStructure.Pull = LL_GPIO_PULL_DOWN;

   GPIO_InitStructure.Pin = USART3_PIN_TX;
   LL_GPIO_Init(GPIOB,&GPIO_InitStructure);

   GPIO_InitStructure.Mode = LL_GPIO_MODE_INPUT;
   GPIO_InitStructure.Pin = USART3_PIN_RX;
   LL_GPIO_Init(GPIOB,&GPIO_InitStructure);

   LL_USART_StructInit(&USART_InitStruct);

   USART_InitStruct.BaudRate            = 115200U;

   if(LL_USART_Init(USART3,&USART_InitStruct) != SUCCESS){
       while(1){
           GPIOC->ODR ^= (1<<13);
       };

   }


   // enable interrupts
   USART3->CR1 |= (USART_CR1_RXNEIE|USART_CR1_TXEIE| USART_CR1_RE|USART_CR1_TE|USART_CR1_UE);

   NVIC_EnableIRQ(USART3_IRQn);



}
