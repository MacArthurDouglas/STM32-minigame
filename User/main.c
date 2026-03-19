#include "stm32f10x.h"
#include "OLED.h"


int main(void)
{
	OLED_Init();
	OLED_ShowChinese(1, 1, CN_WEN);


	// OLED_ShowChar(1,1,'A');
/*   OLED_ShowString(1,3,"HelloWorld");

	OLED_ShowNum(2,1,12345,5);
	
	OLED_ShowBinNum(4,1,0xAA55,16);    */ 

	// OLED_ShowChineseStr(1, 1, "温度");


	while(1)
	{
		
        
	}
}
