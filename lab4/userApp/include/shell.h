#ifndef __SHELL_H__
#define __SHELL_H__

extern void initShell(void);
extern void startShell(void);

extern void addNewCmd(const char *cmd, int (*func)(int argc, char (*argv)[8]),
    void (*help_func)(void), const char* description);
            
#endif