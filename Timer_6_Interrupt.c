
//TIMER INTERRUPT ;

#include <stdint.h>
//CLOCK Registers;
#define RCC_BASE 0x40021000
#define RCC_AHB2ENR (*(volatile uint32_t*)(RCC_BASE + 0x4C))
#define RCC_APB2ENR (*(volatile uint32_t*)(RCC_BASE + 0x60))
#define RCC_APB1ENR (*(volatile uint32_t*)(RCC_BASE + 0x58))


//GPIOB Registers;
#define GPIOB_BASE 0x48000400
#define GPIOB_MODER (*(volatile uint32_t*)(GPIOB_BASE + 0x00))
#define GPIOB_OTYPER (*(volatile uint32_t*)(GPIOB_BASE + 0x04))
#define GPIOB_OSPEEDR (*(volatile uint32_t*)(GPIOB_BASE + 0x08))
#define GPIOB_PUPDR (*(volatile uint32_t*)(GPIOB_BASE + 0x0C))
#define GPIOB_ODR (*(volatile uint32_t *)(GPIOB_BASE + 0x14))

// TIM6 Registers
#define TIM6_BASE 0x40001000
#define TIM6_CR1 (*(volatile uint32_t *)(TIM6_BASE + 0x00))
#define TIM6_DIER (*(volatile uint32_t *)(TIM6_BASE + 0x0C))
#define TIM6_SR (*(volatile uint32_t *)(TIM6_BASE + 0x10))
#define TIM6_CNT (*(volatile uint32_t *)(TIM6_BASE + 0x24))
#define TIM6_PSC (*(volatile uint32_t *)(TIM6_BASE + 0x28))
#define TIM6_ARR (*(volatile uint32_t *)(TIM6_BASE + 0x2C))

#define NVIC_ISER1 (*(volatile uint32_t*)0xE000E104)


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

void TIM6_Init(){

	RCC_APB1ENR |= (1<<4);
	//For 5 seconds;
	TIM6_PSC = 3999;
	TIM6_ARR = 4999;
	TIM6_DIER |=(1<<0);//Update Interrupt Enable;
	TIM6_CR1 |= (1<<0);// Counter Enable;
	NVIC_ISER1|=(1<<22);//Enabling TIM 6 Interrupt IRQ->(32-63);
	TIM6_CNT;

}

//ISR
void TIM6_DACUNDER_IRQHandler(void){

	if(TIM6_SR &(1<<0)){  // Checking UIF(Timer Overflow)
		TIM6_SR &= ~(1<<0);// Clearing;
		GPIOB_ODR ^= (1 << 2); // Turning OFF
	}
}



int main(void)
{

	RCC_APB2ENR |= (1<<0);
	LED_Blink();
	TIM6_Init();

	while(1){
			// // // // //
	}

}
