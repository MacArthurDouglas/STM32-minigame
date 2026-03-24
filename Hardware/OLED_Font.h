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
    CHESS_RED_CHE = 13,
    CHESS_RED_MA = 14,
    CHESS_RED_XIANG = 15,
    CHESS_RED_SHI = 16,
    CHESS_RED_SHUAI = 17,
    CHESS_RED_BING = 18,
    CHESS_RED_PAO = 19,
    CHESS_BLACK_CHE = 20,
    CHESS_BLACK_MA = 21,
    CHESS_BLACK_XIANG = 22,
    CHESS_BLACK_SHI = 23,
    CHESS_BLACK_SHUAI = 24,
    CHESS_BLACK_BING = 25,
    CHESS_BLACK_PAO = 26,
    CHESS_SHANG_CHU_HE= 27,
    CHESS_XIA_CHU_HE= 28,
    CHESS_DIAN_WEI= 29,

    HONG=30,
    HEI=31,
    FANG=32,
    SHENG=33,
    LI=34,
    GANTANHAO=35,
    AN=36,
    REN=37,
    YI=38,
    JIAN=39,
    CHONG=40,
    XIN=41,
    KAI=42,
    SHI=43,
    YOU=44,
    XI=45,
    JUHAO=46,
    XIA=47,
    FU=48,
    WEI=49,


} ChineseIndex;


#endif
