#ifndef X86_SERIAL_H
#define X86_SERIAL_H 1

// arch/x86/serial.c
int initserial(void);
void serialputchar(int c);
void serialputs(const char* str);

#endif
