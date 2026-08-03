// EXT Interrupt;

#include <stdint.h>

//CLOCK Registers;
#define RCC_BASE 0x40021000
#define RCC_AHB2ENR (*(volatile uint32_t*)(RCC_BASE + 0x4C))
#define RCC_APB2ENR (*(volatile uint32_t*)(RCC_BASE + 0x60))


//GPIOB Registers;
#define GPIOB_BASE 0x48000400
#define GPIOB_MODER (*(volatile uint32_t*)(GPIOB_BASE + 0x00))
#define GPIOB_OTYPER (*(volatile uint32_t*)(GPIOB_BASE + 0x04))
#define GPIOB_OSPEEDR (*(volatile uint32_t*)(GPIOB_BASE + 0x08))
#define GPIOB_PUPDR (*(volatile uint32_t*)(GPIOB_BASE + 0x0C))
#define GPIOB_ODR (*(volatile uint32_t *)(GPIOB_BASE + 0x14))

//GPIOC Registers;
#define GPIOC_BASE 0x48000800
#define GPIOC_MODER (*(volatile uint32_t*)(GPIOC_BASE + 0x00))
#define GPIOC_PUPDR (*(volatile uint32_t*)(GPIOC_BASE + 0x0C))
#define GPIOC_IDR (*(volatile uint32_t*)(GPIOC_BASE + 0x10))

//EXT Interrupt Registers
#define EXTI_BASE 0x40010400
#define EXTI_IMR1 (*(volatile uint32_t*)(EXTI_BASE + 0x00))
#define EXTI_RTSR1 (*(volatile uint32_t*)(EXTI_BASE + 0x08))
#define EXTI_FTSR1 (*(volatile uint32_t*)(EXTI_BASE + 0x0C))
#define EXTI_PR1 (*(volatile uint32_t*)(EXTI_BASE + 0x14))
#define NVIC_ISER0 (*(volatile uint32_t*)0xE000E100)
#define NVIC_ISER1 (*(volatile uint32_t*)0xE000E104)
#define SYSCFG_BASE 0x40010000
#define SYSCFG_EXTICR4 (*(volatile uint32_t*)(SYSCFG_BASE + 0x14))


void Delay(uint32_t count){
	for(uint32_t i =0;i<count;i++);
}


void LED_Blink(){

	RCC_AHB2ENR |= (1<<1);// GPIOB Clock
	//MODE Register [5:4] decides function for Port B's 2nd pin
	GPIOB_MODER &= ~(3<<4);
	GPIOB_MODER |= (1<<4);
	GPIOB_OTYPER &= ~(1<<2);// Default PUSH/PULL
	GPIOB_OSPEEDR &= ~(3 << 4);
	GPIOB_OSPEEDR |= (1 << 4);//01 for Medium;
	GPIOB_PUPDR &= ~(3 << 4);//No PullUP/PullDown;
	//while(1){
		GPIOB_ODR &= ~(1<<2);
		//Delay(500000);
		//GPIOB_ODR |= (1<<2);
		//Delay(500000);
	//}

}

void EXT_INT(){

	SYSCFG_EXTICR4 &= ~(0xF << 4);   // Clear EXTI13 field
	SYSCFG_EXTICR4 |=  (0x2 << 4);   // Port C
	//User Button is connected to PC13
	RCC_AHB2ENR |= (1<<2);//GPIOC Clock
	//[27:26] in MODE Register for Function select;
	GPIOC_MODER &=~(3<<26);// 00 for input mode
	GPIOC_PUPDR &=~(3<<26);// NO PULLUP/PULLDOWN;

	EXTI_IMR1 |=(1<<13);// Enabling EXTI13, Like Opening the GATE
	EXTI_RTSR1 |= (1 << 13);   // Rising Trigger enabled
	EXTI_FTSR1 |= (1 << 13);	//Falling Trigger enabled

	NVIC_ISER1 |= (1<<8);// Enabling EXTI15_10


	/*
	 while(1){
		if(GPIOC_IDR &(1<<13)){
			GPIOB_ODR |= (1<<2);
		}
		else{
			GPIOB_ODR &=~(1<<2);
		}
	}
	 */

}

//This function is called Automatically when Interrupt occurs.

void EXTI15_10_IRQHandler()
{
	if(EXTI_PR1 & (1<<13)){ // Checking the Pending register

		GPIOB_ODR |= (1 << 2);// Toggling
		Delay(500000);
		GPIOB_ODR &= ~(1<<2);
		//For clearing pending bit, we have to write 1;
		EXTI_PR1 |=(1<<13);
	}
}

int main(void)
{

    RCC_APB2ENR |= (1<<0);// SYSCONF clock
    LED_Blink();
    EXT_INT();
    //    	EXTI15_10_IRQHandler();
    while(1){

    }

}

