#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "Key.h"
void Key_Init(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_15|GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);


	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12|GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);


	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);


}
typedef enum {
  KEY_GPIO_A  = 1,    /* GPIOA */
  KEY_GPIO_B  = 2,    /* GPIOB */
} KeyLocationType;

bool keyDown(KeyIndex key){
	uint16_t location;
	KeyLocationType type;
	uint8_t keyDownSignal;
	if(key==KEY_UP){
		location=GPIO_Pin_6;
		type=KEY_GPIO_A;
		keyDownSignal=0;
	}
	else if(key==KEY_DOWN){
		location=GPIO_Pin_12;
		type = KEY_GPIO_A;
		keyDownSignal=0;
	}
	else if(key==KEY_LEFT){
		location=GPIO_Pin_15;
		type = KEY_GPIO_B;
		keyDownSignal=0;
	}
	else if (key==KEY_RIGHT)
	{
		location=GPIO_Pin_6;
		type = KEY_GPIO_B;
		keyDownSignal=0;
	}
	else if (key==KEY_CONFIRM)
	{
		location=GPIO_Pin_9;
		type = KEY_GPIO_B;
		keyDownSignal=1;
	}
	else{
		return false;
	}
	
	
	bool KeyNum=false;
	if(GPIO_ReadInputDataBit((type==KEY_GPIO_A)?GPIOA:GPIOB,location)==keyDownSignal){
		Delay_ms(20);
		while(GPIO_ReadInputDataBit((type==KEY_GPIO_A)?GPIOA:GPIOB,location)==keyDownSignal);
		Delay_ms(20);
		KeyNum=true;
	}
	return KeyNum;
}
