#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

void DYTAN_tag(void *addr, size_t size, char *name) {}
void DYTAN_display(void * addr, size_t size, char *fmt, ...) {}
int main (int argc, char **argv){
	int a = 1;
DYTAN_tag(&a, sizeof(a), "a");
	int b = 3;
	int c = a + b;
	printf("%d\n",c);
DYTAN_display(&c, sizeof(c), "c=%c", c);
}
