#ifndef POSIXCALLS_H
#define POSIXCALLS_H 1

int fork(void);
int exit(int);
int write(int, const void*, int);
int read(int, void*, int);
int close(int);
int kill(int);
int exec(const char*, char**);
int open(const char*, int);

#endif
