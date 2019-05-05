#include "interrupts.h"

//#define ECHO_ON

extern volatile uint8_t message_available ;

extern volatile uint8_t strip[15];
extern volatile uint8_t bass_kick;
extern volatile uint8_t hat_kick;

int curr_rank = 0;

extern volatile uint8_t doblink;

void  USART3_IRQHandler(void)
{
    if(USART3->SR & USART_SR_TXE){
        // sent data
        GPIOC->ODR ^= (1<<13);
        USART3->CR1 &= ~USART_CR1_TXEIE; // disable interrupts.h
        //   USART3->CR1 &= ~ USART_CR1_TE;
        //USART3->DR = 0; // clear interrupt

    }
    if(USART3->SR & USART_SR_RXNE){
        // received data
        uint8_t r = USART3->DR;

        if( r=='b' || r=='h' || r=='c'){
            //command
            if(r == 'b'){
                strip[0] = 'b';
                message_available = 1;doblink=1;
                return;
            }
            if(r == 'h'){
                strip[0] = 'h';
                message_available = 1;
                return;
            }
            if(r == 'c'){
                curr_rank = 0;
                message_available=0;
                return;
            }
        } else {
            //data
            strip[curr_rank]=r;
            curr_rank++;
            if(curr_rank==15){
                curr_rank = 0;
                message_available = 1;
            }
            return;
        }
#ifdef ECHO_ON
        USART3->DR = r; // echo
        USART3->CR1 |= USART_CR1_TXEIE; // en. tx interrupt
#endif
    }



}
