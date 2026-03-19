#include "stm32f10x.h"
#include "OLED.h"
#include "Key.h"
#include "ChinessChess.h"
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))


int main(void)
{
	OLED_Init();
	Key_Init();
	// OLED_ShowChinese(1, 1, CN_WEN);
	ChineseIndex tempstr[]={CHE,SELECTION_BOX,CHE_SELECTED_2,CHE_SELECTED_3};
	OLED_ShowChineseStr(1,1,tempstr,ARRAY_SIZE(tempstr));


	// OLED_ShowChar(1,1,'A');
/*   OLED_ShowString(1,3,"HelloWorld");

	OLED_ShowNum(2,1,12345,5);
	
	OLED_ShowBinNum(4,1,0xAA55,16);    */ 

	// OLED_ShowChineseStr(1, 1, "温度");

	while(1)
	{
		OLED_Clear();
/* 		uint16_t data=GPIO_ReadInputData(GPIOA);
		OLED_ShowBinNum(2,1,data,16);
		uint16_t dataB=GPIO_ReadInputData(GPIOB);
		OLED_ShowBinNum(3,1,dataB,16); */
		if(keyDown(KEY_UP)){
			OLED_ShowChar(2,1,'U');
		}
		if(keyDown(KEY_DOWN)){
			OLED_ShowChar(2,3,'D');
		}
		if(keyDown(KEY_RIGHT)){
			OLED_ShowChar(2,5,'R');
		}
		if(keyDown(KEY_LEFT)){
			OLED_ShowChar(2,7,'L');
		}
		if(keyDown(KEY_CONFIRM)){
			OLED_ShowChar(2,9,'C');
		}
	}
}
