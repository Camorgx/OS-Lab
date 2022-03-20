#include "io.h"
#include "vga.h"

#define VGA_BASE (short*)0xB8000 // vga 显存起始地址
#define VGA_SCREEN_WIDTH 80 // vga 屏幕宽度（可容纳字符数）
#define VGA_SCREEN_HEIGHT 25 // vga 屏幕高度

#define CURSOR_LINE_REG 0xE // 行号寄存器
#define CURSOR_COL_REG 0xF // 列号寄存器
#define CURSOR_INDEX_PORT 0x3D4 // 光标行列索引端口号
#define CURSOR_DATA_PORT 0x3D5 // 光标数据端口号

/* ========= 以下函数仅供参考，可以根据自己的需求进行修改，甚至删除 ========= */

/* 将光标设定到特定位置
 * 提示：使用 outb */
void set_cursor_pos(unsigned pos) {
    // 设置行号
    outb(CURSOR_INDEX_PORT, CURSOR_LINE_REG);
    outb(CURSOR_DATA_PORT, (unsigned char)((pos >> 8) & 0xff));
    // 设置列号
    outb(CURSOR_INDEX_PORT, CURSOR_COL_REG);
    outb(CURSOR_DATA_PORT, (unsigned char)(pos & 0xff));
}

/* 读取光标当前所在位置
 * 提示：使用 inb */ 
unsigned get_cursor_pos(void) {
    int x, y;
    // 获取行号
    outb(CURSOR_INDEX_PORT, CURSOR_LINE_REG);
    x = inb(CURSOR_DATA_PORT);
    // 获取列号
    outb(CURSOR_INDEX_PORT, CURSOR_COL_REG);
    y = inb(CURSOR_DATA_PORT);
    return (x << 8) | y;
}

/* 滚屏，vga 屏幕满时使用。丢弃第一行内容，将剩余行整体向上滚动一行
 * 提示：使用指针修改显存 */
void scroll_screen(void) {
    short* pos = VGA_BASE;
    for (int i = 0; i < VGA_SCREEN_HEIGHT - 1; ++i)
        for (int j = 0; j < VGA_SCREEN_WIDTH; ++j) {
            *pos = *(pos + VGA_SCREEN_WIDTH);
            ++pos;
        }
    for (int i = 0; i < VGA_SCREEN_WIDTH; ++i)
        *(pos++) = 0x0020;
    set_cursor_pos((VGA_SCREEN_HEIGHT - 1) * VGA_SCREEN_WIDTH + 1);
}

/* 向 vga 的特定光标位置 pos 输出一个字符
 * 提示：使用指针修改显存 */
void put_char2pos(unsigned char c, int color, unsigned pos) {
    short* dest = VGA_BASE + pos;
    *dest = ((color & 0xff) << 8) | c;
}

/* ========= 以下函数接口禁止修改 ========= */

/* 清除屏幕上所有字符，并将光标位置重置到顶格
 * 提示：使用指针修改显存 */
void clear_screen(void) {
    short* pos = VGA_BASE;
    for (int i = 0; i < VGA_SCREEN_HEIGHT; ++i)
        for (int j = 0; j < VGA_SCREEN_WIDTH; ++j)
            *(pos++) = 0x0020;
    set_cursor_pos(0);
}

/* 向 vga 的当前光标位置输出一个字符串，并移动光标位置到串末尾字符的下一位
 * 如果超出了屏幕范围，则需要滚屏
 * 需要能够处理转义字符 \n */
void append2screen(char *str, int color) { 
    unsigned cur_pos = get_cursor_pos();
    for (char* i = str; *i != '\0'; ++i) {
        if (*i == '\n') { 
            cur_pos = ((cur_pos + 1) / VGA_SCREEN_WIDTH + 1) * VGA_SCREEN_WIDTH + 1;
            if (cur_pos + 1 > VGA_SCREEN_HEIGHT * VGA_SCREEN_WIDTH) {
                scroll_screen();
                cur_pos = (VGA_SCREEN_HEIGHT - 1) * VGA_SCREEN_WIDTH + 1;
            }
            continue;
        }
        put_char2pos(*i, color, cur_pos++);
        if (cur_pos + 1 > VGA_SCREEN_HEIGHT * VGA_SCREEN_WIDTH) {
            scroll_screen();
            cur_pos = (VGA_SCREEN_HEIGHT - 1) * VGA_SCREEN_WIDTH + 1;
        }
    }
    set_cursor_pos(cur_pos);
}