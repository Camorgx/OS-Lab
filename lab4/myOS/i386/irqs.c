#include "irqs.h" 

const int output_pos = VGA_SCREEN_HEIGHT * VGA_SCREEN_WIDTH;

void ignoreIntBody(void) {
    const char str[] = "unknown interrupt";
    for (int i = 0; str[i] != '\0'; ++i)
        put_char2pos(str[i], 0x2, output_pos + i);
}
