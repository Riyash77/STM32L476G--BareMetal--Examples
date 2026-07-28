

#include <stdint.h>

#define RCC_BASE 0x40021000
#define RCC_AHB2ENR (*(volatile uint32_t*)(RCC_BASE + 0x4C))
#define RCC_APB2ENR (*(volatile uint32_t*)(RCC_BASE + 0x60))
#define RCC_CCIPR (*(volatile uint32_t*)(RCC_BASE + 0x88))

//ADC Registers
#define ADC_BASE 0x50040000
#define ADC_ISR (*(volatile uint32_t*)(ADC_BASE + 0x00))
#define ADC_CR (*(volatile uint32_t*)(ADC_BASE + 0x08))
#define ADC_CFGR (*(volatile uint32_t*)(ADC_BASE + 0x0C))
#define ADC_SMPR1 (*(volatile uint32_t*)(ADC_BASE + 0x14))
#define ADC_SQR1 (*(volatile uint32_t*)(ADC_BASE + 0x30))
#define ADC_SQR2 (*(volatile uint32_t*)(ADC_BASE + 0x34))
#define ADC_DR (*(volatile uint32_t*)(ADC_BASE + 0x40))

//GPIOA Registers
#define GPIOA_BASE 0x48000000
#define GPIOA_MODER (*(volatile uint32_t*)(GPIOA_BASE + 0x00))
#define GPIOA_ASCR (*(volatile uint32_t*)(GPIOA_BASE + 0x2C))
#define GPIOA_PUPDR (*(volatile uint32_t*)(GPIOA_BASE + 0x0C))

//GPIOB Registers
#define GPIOB_BASE 0x48000400
#define GPIOB_MODER (*(volatile uint32_t*)(GPIOB_BASE + 0x00))
#define GPIOB_OTYPER (*(volatile uint32_t*)(GPIOB_BASE + 0x04))
#define GPIOB_OSPEEDR (*(volatile uint32_t*)(GPIOB_BASE + 0x08))
#define GPIOB_PUPDR (*(volatile uint32_t*)(GPIOB_BASE + 0x0C))
#define GPIOB_AFRL (*(volatile uint32_t*)(GPIOB_BASE + 0x20))

//USART1 Registers;
#define USART1_BASE 0x40013800
#define USART1_CR1 (*(volatile uint32_t*)(USART1_BASE + 0x00))
#define USART1_CR2 (*(volatile uint32_t*)(USART1_BASE + 0x04))
#define USART1_CR3 (*(volatile uint32_t*)(USART1_BASE + 0x08))
#define USART1_BRR (*(volatile uint32_t*)(USART1_BASE + 0x0C))
#define USART1_ISR (*(volatile uint32_t*)(USART1_BASE + 0x1C))
#define USART1_TDR (*(volatile uint32_t*)(USART1_BASE + 0x28))

void Delay(uint32_t count){
	for(uint32_t i=0; i<count;i++);
}

void ADC_INIT(){

	//Clock;
	RCC_AHB2ENR |= (1<<0);//Port A clock
	RCC_AHB2ENR |= (1<<13);//ADC clock
	RCC_CCIPR |= (3<<28);//11 for system clock as source for ADC
	RCC_CCIPR &= ~(3<<0);
	RCC_CCIPR |= (1<<0);//01 for system clock as source for USART1
	//GPIO;
	GPIOA_MODER |= (3<<0);// Alternate function for ADC in PA0;
	GPIOA_PUPDR &= ~(3<<0);//No PULL UP / PULL Down;
	GPIOA_ASCR |= (1<<0);//Analog switch
	//ADC
	ADC_CR &= ~(1<<29); // For exiting Deep Power Down mode
	ADC_CR |= (1<<28); //Enabling ADC Voltage Regulator
	ADC_CR &= ~(1<<30);// Single ended mode for Calibration;
	ADC_CR |= (1<<31);// Starting ADC Calibration
	while(ADC_CR & (1<<31));// 0 When CAL completes.

	//Sampling process;
	ADC_SMPR1 &= ~(7<<15);
	ADC_SMPR1 |= (4<<15);//For channel 5, 17:15 are responsible for Cycles.
	ADC_SQR1 &= ~(0xF<<0);//L==> Number of conversation is 1 (0000)

	ADC_SQR1 &= ~(0x1F << 6);
	ADC_SQR1 |=  (5 << 6);

	ADC_CR |= (1<<0); //Enabling ADC.
	while(!((ADC_ISR)&(1<<0)));//Checking RDY flag.

}

uint16_t ADC_Read(void){

	ADC_CR |= (1<<2);//Starting the Conversation;
	while(!(ADC_ISR & (1<<2)));//Wait until End of Conversation;
	ADC_ISR |= (1<<2);//Clearing EOC flag
	return (uint16_t)ADC_DR;//Data Register.

}

void USART1_Init(){

	//PB6 for TX function in AF 7;
	RCC_AHB2ENR |=(1<<1);// GPIOB Clock;
	RCC_APB2ENR |= (1<<14);//USART1 Clock;
	RCC_CCIPR &= ~(3<<0);
	RCC_CCIPR |= (1<<0);//Clock select for USART1;
	GPIOB_MODER &=~(3<<12);
	GPIOB_MODER |=(2<<12);//AF Chosen for PB6
	GPIOB_OTYPER &= ~(1<<6);//Push-Pull
	GPIOB_OSPEEDR &= ~(3<<12);
	GPIOB_OSPEEDR |= (3<<12);// For HIGH speed.

	GPIOB_AFRL &= ~(0xF <<24);
	GPIOB_AFRL |= (7<<24);//0111 FOR AF 7;

	USART1_BRR = 417;
	USART1_CR1 |= (1<<3);//TE enable
	USART1_CR1 |= (1<<0);//UART Enable;

}

void SendChar(char ch){

	while(!(USART1_ISR & (1<<7)));//Waiting for TX ready;
	USART1_TDR = ch;
}

void SendString(char *str){
	while(*str !='\0'){
		SendChar(*str);
		str++;
	}
}

void SendNumber(uint16_t value){

	char buffer[5];//For printing the adc value;
	int i=0;
	if(value == 0){
		SendChar('0');
		return;
	}
	while(value > 0){
		buffer[i]=(value%10)+'0';
		value = value/10;
		i++;
	}
	while(i>0){
		SendChar(buffer[--i]);
	}
}

int main(){

	ADC_INIT();
	uint16_t adc_value;
	USART1_Init();
    while(1){
    	adc_value = ADC_Read();
    	SendNumber(adc_value);
    	SendString("\r\n");
    	SendString("\n");
    	Delay(1000000);
    }

}
