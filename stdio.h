#ifndef _STDIO__H
#define _STDIO__H

void stdio_init(void);
void stdio_shutdown(void);
int stdio_getchar(void);

int printf(const char* format, ...);


#endif // _STDIO__H
