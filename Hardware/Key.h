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

#ifdef __cplusplus
extern "C" {
#endif
void Key_Init(void);
bool keyDown(KeyIndex id);

#ifdef __cplusplus
}
#endif



#endif
