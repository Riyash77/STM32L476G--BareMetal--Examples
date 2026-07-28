
#include <stdint.h>

#define RCC_BASE 0x40021000
#define RCC_CR (*(volatile uint32_t*)(RCC_BASE + 0x00))
#define AHB2_EN (*(volatile uint32_t*)(RCC_BASE + 0x4C))
#define APB1ENR1 (*(volatile uint32_t*)(RCC_BASE + 0x5C))
#define RCC_CCIPR (*(volatile uint32_t*)(RCC_BASE + 0x88))
//GPIO registers
#define GPIOA_BASE 0x48000000
#define GPIOA_MODE (*(volatile uint32_t*)(GPIOA_BASE + 0x00))
#define GPIOA_ASCR (*(volatile uint32_t*)(GPIOA_BASE + 0x2C)) //ADC switch
//ADC registers
#define ADC1_BASE 0x50040000
#define ADC_CR (*(volatile uint32_t*)(ADC1_BASE + 0x08))
#define ADC_ISR (*(volatile uint32_t*)(ADC1_BASE + 0x00))
#define ADC_SMPR1 (*(volatile uint32_t*)(ADC1_BASE + 0x14))
#define ADC_SQR1 (*(volatile uint32_t*)(ADC1_BASE + 0x30))
#define ADC_DR (*(volatile uint32_t*)(ADC1_BASE + 0x40))
#define ADC_CFGR (*(volatile uint32_t *)(ADC1_BASE + 0x0C))
#define GPIOG_BASE 0x48001800
#define GPIOG_MODE (*(volatile uint32_t*)(GPIOG_BASE + 0x00))
#define GPIOG_AFRL (*(volatile uint32_t*)(GPIOG_BASE + 0x20))
#define GPIOG_AFRH (*(volatile uint32_t*)(GPIOG_BASE + 0x24))

void delay(uint32_t value){
	for(uint32_t i=0;i<value;i++);
}

int main(){

	AHB2_EN |= (1<<0);// FOR GPIOA
	AHB2_EN |= (1<<6);// FOR GPIOG

	AHB2_EN |= (1<<13); // ADC
	GPIOA_MODE &= ~(3<<0);
	GPIOA_MODE |=  (3<<0); // Making port 0 as Input Mode.
	GPIOA_ASCR |= (1<<0); // ADC Switch open for PORT A pin 0;

	RCC_CCIPR |= (3<<28); // [29:28] ==> ADC clock select, 11 for system clock;
	ADC_CR &= ~(1<<29);// Exit Deep power down mode.
	ADC_CR |= (1<<28); // Enable Voltage regulator.
	delay(1000);

	ADC_CR |= (1<<31);// Calibration bit ==> setting 1, Hardware set 0 after calibration complete.
	while( ADC_CR & (1<<31));//checking if bit becomes 0.

	ADC_ISR &= ~(1<<0);// clears ADCRDY
	ADC_CR |= (1<<0);// Enables ADC.
	while(!(ADC_ISR & (1<<0)));// Waiting for ADC Ready...

	// Sampling Time;
	ADC_SMPR1 |= (7<<15);//[17:15] for Channel 5, 111 for highest value.

	//Conversation;
	ADC_SQR1 &= ~(0xF << 0); //L=0000 for only one conversation;
	ADC_SQR1 |= (0<<0);
	ADC_SQR1 &= ~(0x1F << 6);// Setting sequence for channel 5;
	ADC_SQR1 |= (5<<6);

	//ADC Configuration;
	ADC_CFGR &= ~(3<<3);// 00 For 12 bit resolution.
	ADC_CFGR &= ~(1<<5);// Right alignment.
	ADC_CFGR |= (1<<13); // Continuous Conversation.
	ADC_CFGR &= ~(3<<10); // External Trigger(Software trigger).

	uint32_t adc_value ;
	while(1){
		//Starting conversation;
			ADC_CR |= (1<<2);// ADC Start.
			while(!(ADC_ISR & (1<<2)));//Waiting for EOC;
			adc_value = ADC_DR;
	}

}

