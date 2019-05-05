#include "stm32f1xx.h"
#include "stm32f1xx_ll_rcc.h"
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_system.h"
#include "stm32f1xx_ll_exti.h"
#include "stm32f1xx_ll_cortex.h"
#include "stm32f1xx_ll_utils.h"
#include "stm32f1xx_ll_pwr.h"
#include "stm32f1xx_ll_dma.h"
#include "stm32f1xx_ll_gpio.h"
#include "stm32f1xx_ll_tim.h"

#define ARM_MATH_CM3
#include <arm_math.h>

#include "mersenne.h"
#include "interrupts.h"
#include "uart.h"


#define FAST	200
#define FASTER	50
#define SLOWER	400


uint8_t wheel_r[360]={255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,250,246,242,238,233,229,225,221,216,212,208,204,199,195,191,187,182,178,174,170,165,161,157,153,148,144,140,136,131,127,123,119,114,110,106,102,97,93,89,85,80,76,72,68,63,59,55,51,46,42,38,34,29,25,21,17,12,8,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,8,12,17,21,25,29,34,38,42,46,51,55,59,63,68,72,76,80,85,89,93,97,102,106,110,114,119,123,127,131,136,140,144,148,153,157,161,165,170,174,178,182,187,191,195,199,204,208,212,216,221,225,229,233,238,242,246,250,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255};
uint8_t wheel_g[360]={0,4,8,12,17,21,25,29,34,38,42,46,51,55,59,63,68,72,76,80,85,89,93,97,102,106,110,114,119,123,127,131,136,140,144,148,153,157,161,165,170,174,178,182,187,191,195,199,204,208,212,216,221,225,229,233,238,242,246,250,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,250,246,242,238,233,229,225,221,216,212,208,204,199,195,191,187,182,178,174,170,165,161,157,153,148,144,140,136,131,127,123,119,114,110,106,102,97,93,89,85,80,76,72,68,63,59,55,51,46,42,38,34,29,25,21,17,12,8,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint8_t wheel_b[360]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,8,12,17,21,25,29,34,38,42,46,51,55,59,63,68,72,76,80,85,89,93,97,102,106,110,114,119,123,127,131,136,140,144,148,153,157,161,165,170,174,178,182,187,191,195,199,204,208,212,216,221,225,229,233,238,242,246,250,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,250,246,242,238,233,229,225,221,216,212,208,204,199,195,191,187,182,178,174,170,165,161,157,153,148,144,140,136,131,127,123,119,114,110,106,102,97,93,89,85,80,76,72,68,63,59,55,51,46,42,38,34,29,25,21,17,12,8,4
};





void
delay ( void )
{
	volatile int count = 1000 * 30;	while ( count-- ) ;
}

volatile uint8_t message_available ;

volatile uint8_t strip[15];
volatile int16_t strip_trans[15];

volatile uint8_t strip_bckup[15];
volatile uint16_t ccr_backup[15];
volatile uint8_t bass_kick;
volatile uint8_t hat_kick;

uint8_t effect1_inited =0;

#define EFFECT_PWM_STEP  0x101
#define EFFECT_HHAT_STEP 256
//#define EFFECT_PWM_STEP  512




void backup_state()
{
    int i;
    for(i=0;i<15;i++){
        strip_bckup[i] = strip[i];
    }
    ccr_backup[0]  = TIM1->CCR1 ;ccr_backup[1]  = TIM1->CCR2 ;ccr_backup[2] = TIM1->CCR3 ;
    ccr_backup[3]  = TIM2->CCR1 ;ccr_backup[4]  = TIM2->CCR2 ;ccr_backup[5] = TIM2->CCR3 ;ccr_backup[6]  = TIM2->CCR4;
    ccr_backup[7]  = TIM3->CCR1 ;ccr_backup[8]  = TIM3->CCR2 ;ccr_backup[9] = TIM3->CCR3 ;ccr_backup[10] = TIM3->CCR4;
    ccr_backup[11] = TIM4->CCR1 ;ccr_backup[12] = TIM4->CCR2;ccr_backup[13] = TIM4->CCR3 ;ccr_backup[14] = TIM4->CCR4;

}

void restore_state()
{
    int i;
    for(i=0;i<15;i++){
        strip[i] = strip_bckup[i];
    }
    TIM1->CCR1= ccr_backup[0]   ;TIM1->CCR2=ccr_backup[1]   ;TIM1->CCR3=ccr_backup[2] ;
    TIM2->CCR1= ccr_backup[3]   ;TIM2->CCR2=ccr_backup[4]   ;TIM2->CCR3=ccr_backup[5] ;TIM2->CCR4=ccr_backup[6] ;
    TIM3->CCR1= ccr_backup[7]   ;TIM3->CCR2=ccr_backup[8]   ;TIM3->CCR3=ccr_backup[9] ;TIM3->CCR4=ccr_backup[10] ;
    TIM4->CCR1= ccr_backup[11]  ;TIM4->CCR2=ccr_backup[12]  ;TIM4->CCR3=ccr_backup[13];TIM4->CCR4=ccr_backup[14] ;
}


typedef struct {
    volatile uint8_t strip_bckup[15];
    volatile uint16_t ccr_bckup[15];
    void (*next_strip)();
} effect_typedef;

int blink(){
    backup_state();
    TIM1->CCR1 =0xffff;    TIM1->CCR2 =0xffff;    TIM1->CCR3 =0xffff;    TIM1->CCR4 =0xffff;
    TIM2->CCR1 =0xffff;    TIM2->CCR2 =0xffff;    TIM2->CCR3 =0xffff;    TIM2->CCR4 =0xffff;
    TIM3->CCR1 =0xffff;    TIM3->CCR2 =0xffff;    TIM3->CCR3 =0xffff;    TIM3->CCR4 =0xffff;
    TIM4->CCR1 =0xffff;    TIM4->CCR2 =0xffff;    TIM4->CCR3 =0xffff;    TIM4->CCR4 =0xffff;
    delay();delay();delay();delay();delay();delay();delay();delay();delay();delay();delay();delay();delay();delay();delay();delay();
    restore_state();
}

#define NBR_OF_EFFECTS 10



int e8 =0;
void effect8(){ // fade to red
    e8++;
    TIM1->CCR2 =  e8 * EFFECT_PWM_STEP;
    TIM2->CCR3 =  e8 * EFFECT_PWM_STEP;
    TIM3->CCR2 =  e8 * EFFECT_PWM_STEP;
    TIM3->CCR3 =  e8 * EFFECT_PWM_STEP;
    TIM4->CCR3 =  e8 * EFFECT_PWM_STEP;

    e8 = e8%255;
    delay();
}
int e7 =0;
void effect7(){ // fade to red
    e7++;
    TIM1->CCR1 =  e7 * EFFECT_PWM_STEP;
    TIM2->CCR2 =  e7 * EFFECT_PWM_STEP;
    TIM3->CCR1 =  e7 * EFFECT_PWM_STEP;
    TIM3->CCR4 =  e7 * EFFECT_PWM_STEP;
    TIM4->CCR2 =  e7 * EFFECT_PWM_STEP;

    e7 = e7%255;
    delay();
}

int e6 =0;
void effect6(){ // fade to red
    e6++;
    TIM1->CCR3 =  e6 * EFFECT_PWM_STEP;
    TIM2->CCR1 =  e6 * EFFECT_PWM_STEP;
    TIM2->CCR4 =  e6 * EFFECT_PWM_STEP;
    TIM4->CCR1 =  e6 * EFFECT_PWM_STEP;
    TIM4->CCR4 =  e6 * EFFECT_PWM_STEP;

    e6 = e6%255;
    delay();
}

int deg3 = 0;
void effect5(){ // rota wheel 4 umbrellas periph, central white
    deg3++;

    TIM1->CCR1 =  wheel_r[deg3 % 360] * EFFECT_PWM_STEP; // b
    TIM1->CCR2 =  wheel_r[deg3 % 360] * EFFECT_PWM_STEP; // g
    TIM1->CCR3 =  wheel_r[deg3 % 360] * EFFECT_PWM_STEP; // r

    TIM2->CCR1 =  wheel_r[deg3 % 360] * EFFECT_PWM_STEP;
    TIM2->CCR2 =  wheel_b[deg3 % 360] * EFFECT_PWM_STEP;
    TIM2->CCR3 =  wheel_g[deg3 % 360] * EFFECT_PWM_STEP;

    TIM2->CCR4 =  wheel_r[(deg3+90) % 360] * EFFECT_PWM_STEP;
    TIM3->CCR1 =  wheel_b[(deg3+90) % 360] * EFFECT_PWM_STEP;
    TIM3->CCR2 =  wheel_g[(deg3+90) % 360] * EFFECT_PWM_STEP;

    TIM3->CCR3 = wheel_g[(deg3+180) % 360] * EFFECT_PWM_STEP;
    TIM3->CCR4 = wheel_b[(deg3+180) % 360] * EFFECT_PWM_STEP;
    TIM4->CCR1 = wheel_r[(deg3+180) % 360] * EFFECT_PWM_STEP;

    TIM4->CCR2 = wheel_b[(deg3+270) % 360] * EFFECT_PWM_STEP;
    TIM4->CCR3 = wheel_g[(deg3+270) % 360] * EFFECT_PWM_STEP;
    TIM4->CCR4 = wheel_r[(deg3+270) % 360] * EFFECT_PWM_STEP;

    deg3 = deg3%360;
    delay();
}

int deg2 = 0;
void effect4(){ // solid wheel 5 umbrellas
    deg2++;

    TIM1->CCR1 =  wheel_b[deg2 % 360] * EFFECT_PWM_STEP;
    TIM1->CCR2 =  wheel_g[deg2 % 360] * EFFECT_PWM_STEP;
    TIM1->CCR3 =  wheel_r[deg2 % 360] * EFFECT_PWM_STEP;

    TIM2->CCR1 =  wheel_r[deg2 % 360] * EFFECT_PWM_STEP;

    TIM2->CCR2 =  wheel_b[deg2 % 360] * EFFECT_PWM_STEP;
    TIM2->CCR3 =  wheel_g[deg2 % 360] * EFFECT_PWM_STEP;

    TIM2->CCR4 =  wheel_r[deg2 % 360] * EFFECT_PWM_STEP;
    TIM3->CCR1 =  wheel_b[deg2 % 360] * EFFECT_PWM_STEP;
    TIM3->CCR2 =  wheel_g[deg2 % 360] * EFFECT_PWM_STEP;

    TIM3->CCR3 = wheel_g[deg2 % 360] * EFFECT_PWM_STEP;
    TIM3->CCR4 = wheel_b[deg2 % 360] * EFFECT_PWM_STEP;
    TIM4->CCR1 = wheel_r[deg2 % 360] * EFFECT_PWM_STEP;

    TIM4->CCR2 = wheel_b[deg2 % 360] * EFFECT_PWM_STEP;
    TIM4->CCR3 = wheel_g[deg2 % 360] * EFFECT_PWM_STEP;
    TIM4->CCR4 = wheel_r[deg2 % 360] * EFFECT_PWM_STEP;

    deg2 = deg2%360;
    delay();
}



int deg = 0;
void effect3(){ // fucked wheel
    deg++;

    TIM1->CCR1 =  wheel_r[deg % 360] * EFFECT_PWM_STEP;
    TIM1->CCR2 =  wheel_r[deg % 360] * EFFECT_PWM_STEP;
    TIM1->CCR3 =  wheel_r[deg % 360] * EFFECT_PWM_STEP;

    TIM2->CCR1 =  wheel_r[deg % 360] * EFFECT_PWM_STEP;
    TIM2->CCR2 =  wheel_g[deg % 360] * EFFECT_PWM_STEP;
    TIM2->CCR3 =  wheel_b[deg % 360] * EFFECT_PWM_STEP;

    TIM2->CCR4 =  wheel_r[deg % 360] * EFFECT_PWM_STEP;
    TIM3->CCR1 =  wheel_g[deg % 360] * EFFECT_PWM_STEP;
    TIM3->CCR2 =  wheel_b[deg % 360] * EFFECT_PWM_STEP;

    TIM3->CCR3 = wheel_r[deg % 360] * EFFECT_PWM_STEP;
    TIM3->CCR4 = wheel_g[deg % 360] * EFFECT_PWM_STEP;
    TIM4->CCR1 = wheel_b[deg % 360] * EFFECT_PWM_STEP;

    TIM4->CCR2 = wheel_r[deg % 360] * EFFECT_PWM_STEP;
    TIM4->CCR3 = wheel_g[deg % 360] * EFFECT_PWM_STEP;
    TIM4->CCR4 = wheel_b[deg % 360] * EFFECT_PWM_STEP;

    deg = deg%360;
}


void effect2()
{
    TIM1->CCR1 = genrand();    TIM1->CCR2 = genrand();    TIM1->CCR3 = genrand();    TIM1->CCR4 = genrand();
    TIM2->CCR1 = genrand();    TIM2->CCR2 = genrand();    TIM2->CCR3 = genrand();    TIM2->CCR4 = genrand();
    TIM3->CCR1 = genrand();    TIM3->CCR2 = genrand();    TIM3->CCR3 = genrand();    TIM3->CCR4 = genrand();
    TIM4->CCR1 = genrand();    TIM4->CCR2 = genrand();    TIM4->CCR3 = genrand();    TIM4->CCR4 = genrand();
    delay();delay();delay();delay();delay();
}

void effect1()
{
    if(effect1_inited == 0){
        int i =0;
        TIM1->CCR1 = i;        i+=EFFECT_PWM_STEP; TIM1->CCR2 = i; i+=EFFECT_PWM_STEP; TIM1->CCR3 = i; i+=EFFECT_PWM_STEP; //TIM1->CCR4 = i; i+=EFFECT_PWM_STEP;
        TIM2->CCR1 = i;        i+=EFFECT_PWM_STEP; TIM2->CCR2 = i; i+=EFFECT_PWM_STEP; TIM2->CCR3 = i; i+=EFFECT_PWM_STEP; TIM2->CCR4 = i; i+=EFFECT_PWM_STEP;
        TIM3->CCR1 = i;        i+=EFFECT_PWM_STEP; TIM3->CCR2 = i; i+=EFFECT_PWM_STEP; TIM3->CCR3 = i; i+=EFFECT_PWM_STEP; TIM3->CCR4 = i; i+=EFFECT_PWM_STEP;
        TIM4->CCR1 = i;        i+=EFFECT_PWM_STEP; TIM4->CCR2 = i; i+=EFFECT_PWM_STEP; TIM4->CCR3 = i; i+=EFFECT_PWM_STEP; TIM4->CCR4 = i;
        effect1_inited =1;
    } else {
        TIM1->CCR1 +=  EFFECT_PWM_STEP;    TIM1->CCR2 +=  EFFECT_PWM_STEP;    TIM1->CCR3 +=  EFFECT_PWM_STEP;    TIM1->CCR4 +=  EFFECT_PWM_STEP;
        TIM2->CCR1 +=  EFFECT_PWM_STEP;    TIM2->CCR2 +=  EFFECT_PWM_STEP;    TIM2->CCR3 +=  EFFECT_PWM_STEP;    TIM2->CCR4 +=  EFFECT_PWM_STEP;
        TIM3->CCR1 +=  EFFECT_PWM_STEP;    TIM3->CCR2 +=  EFFECT_PWM_STEP;    TIM3->CCR3 +=  EFFECT_PWM_STEP;    TIM3->CCR4 +=  EFFECT_PWM_STEP;
        TIM4->CCR1 +=  EFFECT_PWM_STEP;    TIM4->CCR2 +=  EFFECT_PWM_STEP;    TIM4->CCR3 +=  EFFECT_PWM_STEP;    TIM4->CCR4 +=  EFFECT_PWM_STEP;
    }
}

void effect11(){
    effect7();
    effect6();
}

void effect10(){
    effect8();
    effect6();
}

void effect9(){
    effect8();
    effect7();
}

void (*eff_fun_ptr[NBR_OF_EFFECTS])() = {
    effect5,
    effect1,
    //    effect2,
    effect3,
    effect4,
    effect6,
    effect7,
    effect8,
    effect9,
    effect10,
    effect11
};

GPIO_TypeDef * gpio_banks[15] = {
    GPIOA,    GPIOA,    GPIOA,
    GPIOB,    GPIOB,    GPIOB,    GPIOB,
    GPIOB,    GPIOB,    GPIOA,    GPIOA,
    GPIOA,    GPIOA,    GPIOA,    GPIOA
};

volatile uint32_t * gpio_odr[15] = {
    & (GPIOA->ODR),& (GPIOA->ODR),& (GPIOA->ODR),
    & (GPIOB->ODR),& (GPIOB->ODR),& (GPIOB->ODR),& (GPIOB->ODR),
    & (GPIOB->ODR),& (GPIOB->ODR),& (GPIOA->ODR),& (GPIOA->ODR),
    & (GPIOA->ODR),& (GPIOA->ODR),& (GPIOA->ODR),& (GPIOA->ODR)
};

uint32_t gpio_pinmask[15] = {
    (1<<8),(1<<9),(1<<10),
    (1<<6),(1<<7),(1<<8),(1<<9),
    (1<<1),(1<<0),(1<<7),(1<<6),
    (1<<3),(1<<2),(1<<1),(1<<0)
};
uint32_t gpio_llinitpin[15] = {
    (LL_GPIO_PIN_8),(LL_GPIO_PIN_9),(LL_GPIO_PIN_10),
    (LL_GPIO_PIN_6),(LL_GPIO_PIN_7),(LL_GPIO_PIN_8),(LL_GPIO_PIN_9),
    (LL_GPIO_PIN_1),(LL_GPIO_PIN_0),(LL_GPIO_PIN_7),(LL_GPIO_PIN_6),
    (LL_GPIO_PIN_3),(LL_GPIO_PIN_2),(LL_GPIO_PIN_1),(LL_GPIO_PIN_0)
};

TIM_TypeDef * gpio_timer[15] = {
    TIM1,TIM1,TIM1,
    TIM4,TIM4,TIM4,TIM4,
    TIM3,TIM3,TIM3,TIM3,
    TIM2,TIM2,TIM2,TIM2
};

uint32_t strip_channel[5][3] = {
    {LL_TIM_CHANNEL_CH1,LL_TIM_CHANNEL_CH2,LL_TIM_CHANNEL_CH3    }    ,
    {LL_TIM_CHANNEL_CH1,LL_TIM_CHANNEL_CH2,LL_TIM_CHANNEL_CH3    }    ,
    {LL_TIM_CHANNEL_CH4,LL_TIM_CHANNEL_CH4,LL_TIM_CHANNEL_CH3    }    ,
    {LL_TIM_CHANNEL_CH2,LL_TIM_CHANNEL_CH1,LL_TIM_CHANNEL_CH4    }    ,
    {LL_TIM_CHANNEL_CH3,LL_TIM_CHANNEL_CH2,LL_TIM_CHANNEL_CH1    }

};

uint32_t tim_ordered_channels[4] = {
    LL_TIM_CHANNEL_CH1,LL_TIM_CHANNEL_CH2,LL_TIM_CHANNEL_CH3,LL_TIM_CHANNEL_CH4
};



void
gpio_init ( )
{
    LL_APB2_GRP1_DisableClock(LL_APB2_GRP1_PERIPH_USART1);
    LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_USB);
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_AFIO);
    //AFIO -> MAPR |= AFIO_MAPR_USART1_REMAP;
    while(! (AFIO -> MAPR &  AFIO_MAPR_USART1_REMAP)){
        LL_GPIO_AF_EnableRemap_USART1();
        LL_GPIO_AF_DisableRemap_TIM1();
    }
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM1);
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM3);
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM4);
    //enable clocks
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOB);
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOC);
    LL_GPIO_InitTypeDef GPIO_InitStructure;
    LL_TIM_InitTypeDef TIM_InitStruct;
    LL_TIM_OC_InitTypeDef TIM_OC_InitStruct_TIM;
    // set for Alternate function af
    GPIO_InitStructure.Mode = LL_GPIO_MODE_ALTERNATE;//
    //GPIO_InitStructure.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStructure.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStructure.Speed = LL_GPIO_MODE_OUTPUT_50MHz;
    GPIO_InitStructure.Pull = LL_GPIO_PULL_DOWN;
    /*
    for(int i=0;i<15;i++){
        GPIO_InitStructure.Pin = gpio_llinitpin[i];
        LL_GPIO_Init(gpio_banks[i],&GPIO_InitStructure);
    }
    */
    GPIO_InitStructure.Pin =
        LL_GPIO_PIN_8|LL_GPIO_PIN_9|LL_GPIO_PIN_10|
        LL_GPIO_PIN_7|LL_GPIO_PIN_6|
        LL_GPIO_PIN_3|LL_GPIO_PIN_2|LL_GPIO_PIN_1|LL_GPIO_PIN_0
        ;
    LL_GPIO_Init(GPIOA,&GPIO_InitStructure);
    GPIO_InitStructure.Pin =
        LL_GPIO_PIN_6|LL_GPIO_PIN_7|LL_GPIO_PIN_8|
        LL_GPIO_PIN_9|
        LL_GPIO_PIN_0|LL_GPIO_PIN_1;
    LL_GPIO_Init(GPIOB,&GPIO_InitStructure);
    //keep the blinker
    GPIO_InitStructure.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStructure.Pin =  LL_GPIO_PIN_13;
    LL_GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIOC->ODR=0;
    LL_TIM_StructInit(&TIM_InitStruct);
    LL_TIM_Init(TIM2,&TIM_InitStruct);    LL_TIM_Init(TIM3,&TIM_InitStruct);    LL_TIM_Init(TIM4,&TIM_InitStruct);    LL_TIM_Init(TIM1,&TIM_InitStruct);
    LL_TIM_OC_StructInit( &TIM_OC_InitStruct_TIM);
    TIM_OC_InitStruct_TIM.OCMode = LL_TIM_OCMODE_PWM1;
    TIM_OC_InitStruct_TIM.CompareValue = 0xffff;
    TIM_OC_InitStruct_TIM.OCState=LL_TIM_OCSTATE_ENABLE;
    int v = 0;
    int i;
    for(i=0;i<4;i++){
        LL_TIM_OC_Init(TIM1,tim_ordered_channels[i],&TIM_OC_InitStruct_TIM);
        LL_TIM_OC_Init(TIM2,tim_ordered_channels[i],&TIM_OC_InitStruct_TIM);
        LL_TIM_OC_Init(TIM3,tim_ordered_channels[i],&TIM_OC_InitStruct_TIM);
        LL_TIM_OC_Init(TIM4,tim_ordered_channels[i],&TIM_OC_InitStruct_TIM);
    }
    TIM1 -> BDTR |= ( TIM_BDTR_MOE | TIM_BDTR_AOE );
// gotcha advanced timer ... lost a sunday on this...
    TIM1 -> CNT =0;    TIM2 -> CNT =0;    TIM3 -> CNT =0;    TIM4 -> CNT =0;
    TIM1->EGR |= TIM_EGR_UG;   TIM2->EGR |= TIM_EGR_UG;   TIM3->EGR |= TIM_EGR_UG;   TIM4->EGR |= TIM_EGR_UG;
    TIM1->CR1 |= TIM_CR1_ARPE; TIM2->CR1 |= TIM_CR1_ARPE; TIM3->CR1 |= TIM_CR1_ARPE; TIM4->CR1 |= TIM_CR1_ARPE;
    TIM1->CR1 |= TIM_CR1_CEN;  TIM2->CR1 |= TIM_CR1_CEN;  TIM3->CR1 |= TIM_CR1_CEN;   TIM4->CR1 |= TIM_CR1_CEN;
}

void
led_on ( void )
{
	GPIOC->ODR = (1<<13);
    //  GPIOA->ODR |= ( (1<<8)|(1<<9)|(1<<10) ) ;

}

void
led_off ( void )
{
	GPIOC->ODR ^= (1<<13);
    //  GPIOA->ODR ^= ( (1<<8)|(1<<9)|(1<<10) ) ;
}


void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);

   if(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_2)
  {
      while(1);
  }
  LL_RCC_HSE_Enable();

   /* Wait till HSE is ready */
  while(LL_RCC_HSE_IsReady() != 1)
  {

  }
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE_DIV_1, LL_RCC_PLL_MUL_9);
  LL_RCC_PLL_Enable();

   /* Wait till PLL is ready */
  while(LL_RCC_PLL_IsReady() != 1)
  {

  }
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {

  }
  LL_Init1msTick(72000000);
  LL_SYSTICK_SetClkSource(LL_SYSTICK_CLKSOURCE_HCLK);
  LL_SetSystemCoreClock(72000000);
}

volatile uint8_t doblink = 0;

void
main ( void )
{
    RCC -> CFGR &= ~RCC_CFGR_MCO_Msk;
    SystemClock_Config();
    SystemCoreClockUpdate();
    sgenrand(0xcafebabe);
	gpio_init ();
    uart_init();
    GPIOC->ODR |= (1<<13);
    int i=0;

    TIM1->CCR1 = TIM1->CCR2 = TIM1->CCR3 = TIM1->CCR4 = 0;
    TIM2->CCR1 = TIM2->CCR2 = TIM2->CCR3 = TIM2->CCR4 = 0;
    TIM3->CCR1 = TIM3->CCR2 = TIM3->CCR3 = TIM3->CCR4 = 0;
    TIM4->CCR1 = TIM4->CCR2 = TIM4->CCR3 = TIM4->CCR4 = 0;
    void (*curr_eff_fun_ptr)() = eff_fun_ptr[0];
	for ( ;; ) {
        curr_eff_fun_ptr();
	    delay ();
        //  if( (genrand() & 0xffff)< 255) blink();
        i++;
        if(( (i & 0xcff) == 0)){
            curr_eff_fun_ptr = eff_fun_ptr[genrand() % NBR_OF_EFFECTS ];
        }
        if(doblink){
            blink();doblink=0;}

	}
}

/* THE END */
