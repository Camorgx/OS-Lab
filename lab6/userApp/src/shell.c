#include "userInterface.h"
#include "string.h"

typedef struct myCommand {
    char *name;
    char *description;
    int (*func)(int argc, char (*argv)[32]);
    void (*help_func)(void);
} myCommand;

typedef struct command_list {
    myCommand* command;
    struct command_list* next;
} command_list;

command_list* cmd_list_head;

/*
    功能：增加命令
    1.使用malloc创建一个cm的结构体，新增命令。
    2.同时还需要维护一个表头为head的链表。
*/
void addNewCmd(const char *cmd, int (*func)(int argc, char (*argv)[32]),
               void (*help_func)(void), const char* description) {
    myCommand* command = (myCommand*) malloc(sizeof(myCommand));
    command->name = (char*) malloc((strlen(cmd) + 1) * sizeof(char));
    memcpy(command->name, cmd, (strlen(cmd) + 1) * sizeof(char));
    command->func = func;
    command->help_func = help_func;
    command->description = (char*) malloc((strlen(description) + 1) * sizeof(char));
    memcpy(command->description, description, (strlen(description) + 1) * sizeof(char));
    command_list* next = (command_list*) malloc(sizeof(command_list));
    next->next = cmd_list_head->next;
    next->command = command;
    cmd_list_head->next = next;
}

int func_cmd(UNUSED int argc, UNUSED char (*argv)[32]) {
    for (command_list* p = cmd_list_head->next; p; p = p->next)
        printf(0x7, "%s ", p->command->name);
    printf(0x7, "\n");
    return 0;
}

void help_help(void) {
    printf(0x7, "USAGE: help [cmd]\n");
}

int func_help(int argc, char (*argv)[32]) {
    if (argc == 1) {
        help_help();
        return 0;
    }
    for (command_list* p = cmd_list_head->next; p; p = p->next)
        if (strcmp(argv[1], p->command->name) == 0) {
            printf(0x7, "%s\n", p->command->description);
            if (p->command->help_func) p->command->help_func();
            return 0;
        }
    printf(0x7, "Command %s not found!\n", argv[1]);
    return 1;
}

int func_exit(UNUSED int argc, UNUSED char (*argv)[32]) { return 0; }

int func_clear(UNUSED int argc, UNUSED char (*argv)[32]) {
    clear_screen();
    return 0;
}

void split(char ans[8][32], const char* line) {
    int quote_cnt = 0;
    for (int i = 0; line[i] != '\0'; ++i)
        if (line[i] == '\"') ++quote_cnt;
    if (quote_cnt % 2) {
        ans[0][0] = '\0';
        return;
    }
    int cnt = 0, cnt_p = 0;
    int in_quote = 0;
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
    int BUF_len;	//输入缓存区的长度

    do {
        BUF_len = 0; 
        printf(0x07, "Student>>");
        while((BUF[BUF_len] = uart_get_char()) != '\r')
            printf(0x7, "%c", BUF[BUF_len++]);
        printf(0x7, "\n");
        BUF[BUF_len] = '\0';

        char argv[8][32];
        split(argv, BUF);
        int argc = 0;
        for (int i = 0; argv[i][0] != '\0'; ++i) ++argc;
        if (argc == 0) continue;
        int flag = 0;
        for (command_list* p = cmd_list_head->next; p; p = p->next) {
            if (strcmp(argv[0], p->command->name) == 0) {
                p->command->func(argc, argv);
                flag = 1;
            }
            if (strcmp(argv[0], "exit") == 0) return;
        }
        if (!flag) printf(0x7, "Command %s not found!\n", argv[0]);
    } while(1);
}

void initShell(void) {
    cmd_list_head = (command_list*) malloc(sizeof(command_list));
    addNewCmd("cmd", func_cmd, NULL, "Display all commands.");
    addNewCmd("help", func_help, help_help, "Get help of a certain command.");
    addNewCmd("exit", func_exit, NULL, "Exit the shell.");
    addNewCmd("clear", func_clear, NULL, "Clear screen.");
}
