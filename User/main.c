#include "stm32f10x.h"
#include "OLED.h"
#include "Key.h"
#include "ChinessChess.h"
#include "Delay.h"
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))


int main(void)
{
	OLED_Init();
	Key_Init();
	// OLED_ShowChinese(1, 1, CN_WEN);
	// ChineseIndex tempstr[]={CHE,SELECTION_BOX,CHE_SELECTED_2,CHE_SELECTED_3};
	// OLED_ShowChineseStr(1,1,tempstr,ARRAY_SIZE(tempstr));
/* 	ChineseIndex tempstr[]={13,14,15,16,17,18,19,20};
	OLED_ShowChineseStr(1,1,tempstr,ARRAY_SIZE(tempstr));
	ChineseIndex tempstr2[]={21,22,23,24,25,26,27,28};
	OLED_ShowChineseStr(2,1,tempstr2,ARRAY_SIZE(tempstr2));
	ChineseIndex tempstr3[]={29};
	OLED_ShowChineseStr(3,1,tempstr3,ARRAY_SIZE(tempstr3)); */


	// OLED_ShowChar(1,1,'A');
/*   OLED_ShowString(1,3,"HelloWorld");

	OLED_ShowNum(2,1,12345,5);
	
	OLED_ShowBinNum(4,1,0xAA55,16);    */ 

	// OLED_ShowChineseStr(1, 1, "温度");

	
	game();
	while(1)
	{
		
	}
}
