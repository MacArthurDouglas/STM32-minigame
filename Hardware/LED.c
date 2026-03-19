#include "stm32f10x.h"                  // Device header

void LED_Init(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1|GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_SetBits(GPIOA,GPIO_Pin_1|GPIO_Pin_2);
}
void LED_ON(int id){
	if(id==1){
		GPIO_ResetBits(GPIOA,GPIO_Pin_1);
	}
	else if(id==2){
		GPIO_ResetBits(GPIOA,GPIO_Pin_2);
	}
}

void LED_OFF(int id){
	if(id==1){
		GPIO_SetBits(GPIOA,GPIO_Pin_1);
	}
	else if(id==2){
		GPIO_SetBits(GPIOA,GPIO_Pin_2);
	}

}
void LED_Turn(int id){
	uint8_t led;
	if(id==1){
		led=GPIO_Pin_1;
	}
	else if(id==2){
		led=GPIO_Pin_2;
		
	}
	if(GPIO_ReadOutputDataBit(GPIOA,led)==0){
		GPIO_SetBits(GPIOA,led);
	}
	else{
		GPIO_ResetBits(GPIOA,led);
	}
}

