/* 
 * 请确保以下的代码可以原封不动的成功编译和执行
 * 助教测试时会替换相应的参数
 * 不需要修改
 * （全部划掉）
 */

#include "io.h"
#include "myPrintk.h"
#include "uart.h"
#include "vga.h"

extern void startShell(void);

void myMain(void){    
    startShell();
}
