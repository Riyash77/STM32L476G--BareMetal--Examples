//PWM Based LED Testing

#include <stdint.h>

//CLOCK Registers;
#define RCC_BASE 0x40021000
#define RCC_AHB2ENR (*(volatile uint32_t*)(RCC_BASE + 0x4C))
#define RCC_APB2ENR (*(volatile uint32_t*)(RCC_BASE + 0x60))
#define RCC_APB1ENR (*(volatile uint32_t*)(RCC_BASE + 0x58))
#define RCC_CCIPR (*(volatile uint32_t*)(RCC_BASE + 0x88))

//GPIOB Registers;
#define GPIOB_BASE 0x48000400
#define GPIOB_MODER (*(volatile uint32_t*)(GPIOB_BASE + 0x00))
#define GPIOB_OTYPER (*(volatile uint32_t*)(GPIOB_BASE + 0x04))
#define GPIOB_OSPEEDR (*(volatile uint32_t*)(GPIOB_BASE + 0x08))
#define GPIOB_PUPDR (*(volatile uint32_t*)(GPIOB_BASE + 0x0C))
#define GPIOB_ODR (*(volatile uint32_t*)(GPIOB_BASE + 0x14))
#define GPIOB_AFRL (*(volatile uint32_t*)(GPIOB_BASE + 0x20))


//LPTIM1 Registers;
#define LPTIM1_BASE 0x40007C00
#define LPTIM1_ISR (*(volatile uint32_t *)(LPTIM1_BASE + 0x00))
#define LPTIM1_ICR (*(volatile uint32_t *)(LPTIM1_BASE + 0x04))
#define LPTIM1_IER (*(volatile uint32_t *)(LPTIM1_BASE + 0x08))
#define LPTIM1_CFGR (*(volatile uint32_t *)(LPTIM1_BASE + 0x0C))
#define LPTIM1_CR (*(volatile uint32_t *)(LPTIM1_BASE + 0x10))
#define LPTIM1_CMP (*(volatile uint32_t *)(LPTIM1_BASE + 0x18))
#define LPTIM1_ARR (*(volatile uint32_t *)(LPTIM1_BASE + 0x1C))
#define LPTIM1_CNT (*(volatile uint32_t *)(LPTIM1_BASE + 0x20))


#define NVIC_ISER1 (*(volatile uint32_t*)0xE000E104)


void Delay(uint32_t count){
	for(uint32_t i =0;i<count;i++);
}


void LED_Blink(){

	RCC_AHB2ENR |= (1<<1);// GPIOB Clock
	//MODE Register [5:4] decides function for Port B's 2nd pin
	GPIOB_MODER &= ~(3<<4);
	GPIOB_MODER |= (2<<4);// 10 for AF;
	GPIOB_AFRL &= ~(0xF <<8);
	GPIOB_AFRL |=(1<<8);// 0001 for AF1


}

void LPTIM1_PWN(){

	RCC_APB1ENR |= (1<<31);//Low power timer 1 clock enable
	RCC_CCIPR &= ~(3 << 18);   // LPTIM1SEL = 00 (PCLK)
	LPTIM1_CR |= (1<<0);// LPTIM enable
	LPTIM1_CFGR &= ~(1 << 0);      // CKSEL = 0 (Internal clock)
	LPTIM1_CFGR &= ~(7 << 9);      // PRESC = DIV1
	LPTIM1_CFGR &= ~(1 << 14);     // COUNTMODE = Internal counter
	LPTIM1_CFGR &= ~(1<<20);// WAVE = 0 for PWM mode
	LPTIM1_CFGR &= ~(1<<21);// WAVE POL = 0 for same output polarity
	LPTIM1_ARR = 999;
	//while(!(LPTIM1_ISR & (1<<4)));// Waiting for ARR OK
	LPTIM1_ICR |= (1<<4);
	LPTIM1_CMP = 25;
	//while(!(LPTIM1_ISR & (1<<3)));//Waiting for CMP OK
	LPTIM1_ICR |= (1<<3);
	LPTIM1_CR |= (1<<2);// Count start continuously;
	while(1)
	{
	    if(LPTIM1_CNT < LPTIM1_CMP)
	    {
	        GPIOB_ODR |= (1<<2);
	    }
	    else
	    {
	        GPIOB_ODR &= ~(1<<2);
	    }
	}


}

/*
 * The Timer increments the CNT irrespective of While loop...
 */

int main(void)
{

	RCC_APB2ENR |= (1<<0);
	LED_Blink();
	LPTIM1_PWN();

}
