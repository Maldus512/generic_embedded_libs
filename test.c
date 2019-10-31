#include "debounce.h"
#include "keyboard.h"
#include "circular_buffer.h"
#include <stdio.h>
//#include "parameter.h"


int main() {
    char buffer[16];
    char test1[16];
    circular_buf_t cbuf;
    circular_buf_init(&cbuf,buffer, 16);

    circular_buf_put(&cbuf,"ciao1", 6);
    circular_buf_put(&cbuf,"ciao2", 6);
    circular_buf_get(&cbuf, test1, 6);
    printf("%s\n", test1);
    circular_buf_put(&cbuf,"ciao3", 5);
    circular_buf_get(&cbuf, test1, 6);
    printf("%s\n", test1);
    circular_buf_get(&cbuf, test1, 6);
    printf("%s\n", test1);
    return 0;
}