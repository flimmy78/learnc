#include <stdio.h>
typedef unsigned char uint8;
typedef unsigned int uint32;
#define QUEUE_DATA_TYPE uint8

typedef struct {
	QUEUE_DATA_TYPE *Out; /* 指向数据输出位置         */
	QUEUE_DATA_TYPE *In; /* 指向数据输入位置         */
	QUEUE_DATA_TYPE *End; /* 指向Buf的结束位置        */
	uint32 NData; /* 队列中数据个数           */
	uint32 MaxData; /* 队列中允许存储的数据个数 */

	uint8 (*ReadEmpty)(); /* 读空处理函数             */
	uint8 (*WriteFull)(); /* 写满处理函数             */
	QUEUE_DATA_TYPE Buf[1]; /* 存储数据的空间           */
} DataQueue;

typedef struct {
	//QUEUE_DATA_TYPE *Out; /* 指向数据输出位置         */
	QUEUE_DATA_TYPE *In; /* 指向数据输入位置         */
	QUEUE_DATA_TYPE *End; /* 指向Buf的结束位置        */
	uint32 NData; /* 队列中数据个数           */
	uint32 MaxData; /* 队列中允许存储的数据个数 */

	uint8 (*ReadEmpty)(); /* 读空处理函数             */
	uint8 (*WriteFull)(); /* 写满处理函数             */
	QUEUE_DATA_TYPE Buf[1]; /* 存储数据的空间           */
} DataQueue1;

int main()
{
    printf("force convert length DataQueue1: %u\n", (uint32) (((DataQueue1 *) 0)->Buf));
    printf("DataQueue length: %u\n", sizeof(DataQueue));
    return 0;
}
