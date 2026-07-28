

#include <stdint.h>

//CLOCK Registers;
#define RCC_BASE 0x40021000
#define RCC_AHB2ENR (*(volatile uint32_t*)(RCC_BASE + 0x4C))
#define RCC_APB2ENR (*(volatile uint32_t*)(RCC_BASE + 0x60))
#define RCC_CCIPR (*(volatile uint32_t*)(RCC_BASE + 0x88))

//GPIO Registers;
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

uint32_t data;
//Function for character;
void USART1_SendChar(char c){

	while(!(USART1_ISR &(1<<7)));//Checking the 7th bit for TXE;
	while(!(USART1_ISR & (1<<21)));//Checking for Transmitter acknowledge bit;
	USART1_TDR = c;//Fixing the data to Transmit Data Register;
	data= USART1_TDR;
	while(!(USART1_ISR & (1<<6)));//Checking for Transmission complete;
}

void USART1_String(const char *str){

	// H E L L O \0;
	while(*str){
		USART1_SendChar(*str++);
	}
}

//Function for String;

//Delay Function;
void delay(uint32_t count){
	for(uint32_t i=0;i<count;i++);
}


int main(){

	//Clock Enabling Registers;
	RCC_AHB2ENR |=(1<<1);//GPIOB Clock Enable;
	RCC_APB2ENR |=(1<<14);//USART1 Clock Enable;
	RCC_CCIPR &= ~(3<<0);
	RCC_CCIPR |= (1<<0);//Selecting the SYSCLK as Clock source for UART1;

	//GPIOB Registers;
	GPIOB_MODER &= ~(3<<12);
	GPIOB_MODER |= (2<<12);//[13:12] PINS for MODE selection, 10 for AF.
	GPIOB_OTYPER &= ~(1<<6);// For PushPull Output;
	GPIOB_OSPEEDR &= ~(3<<12);
	GPIOB_OSPEEDR |= (3<<12);//[13:12] Pins 11 for HIGH SPEED OUTPUT;
	GPIOB_PUPDR &= ~(3<<12); //00 FOR NO PUSH/PULL;
	GPIOB_AFRL &= ~(0xF << 24);// Clearing [27:24] for AF Selection.
	GPIOB_AFRL |= (7<<24);// Setting 0111 for AF7 ; AF7 for PIN6 is USART TX;

	//USART1 Registers;
	USART1_CR1 &= ~(1<<0);//Initially disabling the UART (UE = 0);
	USART1_BRR = 417;//For 9600 BaudRate;f_clk / BR;
	USART1_CR2 &= ~(3<<12);// 00 For only 1 STOP BIT;
	USART1_CR1 |= (1<<3);// Transmitter Enable;
	USART1_CR1 |= (1<<2);//Receiver Enable (Optional);
	USART1_CR1 |= (1<<0);//Enabling USART;

	while(1){
		//USART1_SendChar('A');
		USART1_String("Hello USART1");
		// \r for move the cursor to beginning;
		USART1_SendChar('\n');
		delay(5000000);
	}


}

