#include "io.h"
#include "myPrintk.h"
#include "uart.h"
#include "vga.h"
#include "i8253.h"
#include "i8259A.h"
#include "tick.h"
#include "wallClock.h"
#include "vsprintf.h"

typedef struct myCommand {
    char name[80];
    char help_content[200];
    int (*func)(int argc, char (*argv)[8]);
} myCommand; 

int func_cmd(int argc, char (*argv)[8]);
int func_help(int argc, char (*argv)[8]);

myCommand cmd = {"cmd", "List all command\n", func_cmd};
myCommand help = {"help", "Usage: help [command]\nDisplay info about [command]\n", func_help};
myCommand* commands[8] = {&cmd, &help};
int command_num = 2;

int func_cmd(int argc, char (*argv)[8]) {
    for (int i = 0; i < command_num; ++i)
        myPrintf(0x7, "%s ", commands[i]->name);
    myPrintf(0x7, "\n");
    return 0;
} 

int func_help(int argc, char (*argv)[8]) {
    for (int i = 0; i < command_num; ++i) {
        if (strcmp(argv[1], commands[i]->name) == 0) {
            myPrintf(0x7, "%s\n", commands[i]->help_content);
            return 0;
        }
    }
    myPrintf(0x7, "Command %s not found!\n", argv[1]);
    return 1;
}

void split(char ans[8][8], const char* line) {
    int quote_cnt = 0;
    for (int i = 0; line[i] != '\0'; ++i)
        if (line[i] == '\"') ++quote_cnt;
    if (quote_cnt % 2) {
        ans[0][0] = '\0';
        return;
    }
    int cnt = 0, cnt_p = 0;
    int in_quote = 0;
    int num = 8, num_p = 8;
    for (int i = 0; line[i] != '\0'; ++i) {
        if (line[i] == '\"') in_quote = !in_quote;
        else if (!in_quote && line[i] == ' ' && cnt) {
            ans[cnt_p++][cnt] = '\0'; cnt = 0;
        }
        else ans[cnt_p][cnt++] = line[i];  
    }
    ans[cnt_p][cnt] = '\0';
    ans[cnt_p + 1][0] = '\0';
}

void startShell(void) {
    //我们通过串口来实现数据的输入
    char BUF[64] = {0}; //输入缓存区
    int BUF_len = 0;	//输入缓存区的长度

    do {
        BUF_len = 0; 
        myPrintf(0x07, "Student>>");
        while((BUF[BUF_len] = uart_get_char()) != '\r')
            myPrintf(0x7, "%c", BUF[BUF_len++]);
        myPrintf(0x7, "\n");
        BUF[BUF_len] = '\0';

        char argv[8][8];
        split(argv, BUF);
        int argc = 0;
        for (int i = 0; argv[i][0] != '\0'; ++i) ++argc;
        int flag = 0;
        for (int i = 0; i < command_num; ++i) {
            if (strcmp(argv[0], commands[i]->name) == 0) {
                commands[i]->func(argc, argv);
                flag = 1;
            }
            else if (strcmp(argv[0], "exit") == 0) return;
        }
        if (!flag) myPrintf(0x7, "Command %s not found!\n", argv[0]);
    } while(1);
}
