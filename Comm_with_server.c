#include "sys/select.h"
/************************************************************************************************
 德鲁热表的接受函数
 参数1:设备的描述符，在主线程中准备好，在此使用     
 返回 CONTROL_RET_FAIL 发送失败， CONTROL_RET_SUC 发送成功
 ************************************************************************************************/
void server_com_rev(int u32DeviceFd) {
	unsigned char c;
	fd_set ReadSetFD;
	FD_ZERO(&ReadSetFD);
	FD_SET(u32DeviceFd, &ReadSetFD);
	select(u32DeviceFd + 1, &ReadSetFD, 0, 0, 0);
}

