#ifndef __OLED_FONT_H
#define __OLED_FONT_H

/*OLED字模库，宽8像素，高16像素*/
extern uint8_t OLED_F8x16[][16];

/*OLED中文字模库，宽16像素，高16像素*/
/*取模方式：列行式，从左到右，从上到下*/
extern uint8_t OLED_F16x16[][32];


/* ✅ 正确写法 - 改用枚举索引，完全避免中文 */
typedef enum {
    CN_WEN  = 0,    /* 温 */
    CN_DU   = 1,    /* 度 */
    CN_SHI  = 2,    /* 湿 */
    CN_NIAN = 3,    /* 年 */
    CN_YUE  = 4,    /* 月 */
    CN_RI   = 5,    /* 日 */
    CN_SHI2 = 6,    /* 时 */
    CN_FEN  = 7,    /* 分 */
    CN_MIAO = 8,    /* 秒 */
    SELECTION_BOX = 9,   //选择框
    CHE_SELECTED_2 = 10,
    CHE_SELECTED_3 = 11,
    CHE = 12,
} ChineseIndex;


#endif
