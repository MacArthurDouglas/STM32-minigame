#ifndef __KEY_H
#define __KEY_H
#include <stdbool.h>

typedef enum {
    KEY_UP  = 0,    /* 上 */
    KEY_DOWN   = 1,    /* 下 */
    KEY_LEFT  = 2,    /* 左 */
    KEY_RIGHT = 3,    /* 右 */
    KEY_CONFIRM  = 4,    /* 确认 */
} KeyIndex;

void Key_Init(void);
bool Key_GetNum(KeyIndex id);





#endif
