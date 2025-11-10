#ifndef CONSOLE_H
#define CONSOLE_H 1

#include <limine.h>

int initconsole(struct limine_framebuffer* fb);
void consoleputchar(int c);
void consoleputs(const char* str);
void consoleflush();

#endif
