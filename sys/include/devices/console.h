#ifndef X86_CONSOLE_H
#define X86_CONSOLE_H 1

int initconsole(void);
void consoleputchar(int c);
void consoleputs(const char* str);
void consoleflush(void);

#endif
