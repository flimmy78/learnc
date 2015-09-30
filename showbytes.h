#ifndef SHOWBYTES_H
#define SHOWBYTES_H

typedef unsigned char *byte_pointer;
void show_bytes(byte_pointer start, int len);
void show_int(int x);
void show_float(float x);
void show_pointer(void *x);
void test_show_bytes(int val);

#endif // SHOWBYTES
