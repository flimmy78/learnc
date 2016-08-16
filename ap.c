#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define U8  unsigned char
#define U32 unsigned int
#define U16 unsigned short
#define S16 signed short
#define S8  char
#define NO_ERR  0x00
#define ERROR   0x01
#define isdigit(c) ((unsigned) ((c)-'0') < 10)
#define ishex(c)	((unsigned) ((c)-'0') < 10 || (unsigned) ((c)-'a') < 6 || (unsigned) ((c)-'A') < 6 )



void std_vararg_func(const char *fmt, ...) {
        va_list ap;
        va_start(ap, fmt);

        printf("%d\n", va_arg(ap, int));
        printf("%f\n", va_arg(ap, double));
        printf("%s\n", va_arg(ap, char*));

        va_end(ap);
}

typedef struct{		
	U32 min		:6;  //最低6位为分
	U32 hour	:5;
	U32 date	:5;
	U32 month	:4;
	U32 year	:12; //最高12位为年
}Time;	

typedef enum {
    float_state_init = 0,
    float_state_sign,
    float_state_digit,
    float_state_dec_point,
    float_state_err
} em_float_state;

U8 isFloat(U8* buf, U16 bufSize)
{
	em_float_state state = float_state_init;
	S8 c;
	S8 flag = 0;
	U16 i = 0;

	for (i = 0; i < bufSize;i++) {
		c = buf[i];
		switch (state) {
		case float_state_init:
			if (c == '+' || c=='-') {
				state = float_state_sign;
			} else if (isdigit(c)) {
				state = float_state_digit;
			} else {
				state = float_state_err;
				goto result;
			}
			break;
		case float_state_sign:
			if (isdigit(c)) {
				state = float_state_digit;
			} else {
				state = float_state_err;
				goto result;
			}
			break;
		case float_state_digit:
			if (c == '.') {
				if(flag == 0) {
					flag = 1;
					state = float_state_dec_point;
				} else {
					state = float_state_err;
					goto result;
				}
			} else if ((c != '.') && !isdigit(c)) {
				state = float_state_err;
			}
			break;
		case float_state_dec_point:
			if (!isdigit(c)) {
				state = float_state_err;
				goto result;
			} else {
				state = float_state_digit;
			}
			break;
		default:
			state = float_state_err;
			goto result;
			break;
		}
	}
result:
	//float_state_digit 是唯一的接受状态
	return ((state == float_state_digit) ? NO_ERR : ERROR);
}

U16 getCoeByPercentErr(float percentErr, U16 oldCoe)
{
	S16 i = 0;
	U16 newCoeFix = 0;
	float newCoeFoat = 0.0;
	newCoeFoat = 3.272;

	newCoeFix |= (((int)newCoeFoat) << 12);
	for (i = 2; i >= 0;i--) {
		newCoeFoat -= (int)newCoeFoat;
		newCoeFoat *= 16;
		newCoeFix |= (((int)newCoeFoat) << (4 * i));
	}
	return newCoeFix;
}

void printyes()
{
	printf("Yes\n");
}

void printno()
{
	printf("No\n");
}

void testTripleExp(void)
{
	(1==2)?  printyes(): printno();
}

int main(int argc, char* argv[])
{
	Time mTime = {0};
	U8* p = (U8*)&mTime;
	
	printf("%02X %02X %02X %02X\n", p[0], p[1], p[2], p[3]);
	
	mTime.min	= 56;
	mTime.hour	= 23;
	mTime.date	= 12;
	mTime.month = 7;
	mTime.year	= 2016;

	printf("%02X %02X %02X %02X\n", p[0], p[1], p[2], p[3]);
	mTime = 0;
	return 0;
}
