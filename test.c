#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

#define STRIP_SZ 15

typedef struct {
    volatile uint8_t strip_bckup[STRIP_SZ];
    volatile uint16_t ccr_bckup[STRIP_SZ];
    void (*next_strip)();
} effect_typedef;

effect_typedef * curr_effect;


#define EFFECT_AMOUNT 1

effect_typedef * effect_list[EFFECT_AMOUNT];

uint8_t strip_step[STRIP_SZ];
uint32_t curr_fadestep_left;
void fadeinstep(uint8_t * tostrip,uint8_t to_init,uint32_t steps)
{
    if(to_init){
        curr_fadestep_left = steps;


    }

}


uint8_t effect3_inited = 0
void effect3_ns()
{

}


int main()
{
    effect_typedef effect3;






}
