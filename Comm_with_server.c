#include "sys/select.h"
/************************************************************************************************
 ��³�ȱ�Ľ��ܺ���
 ����1:�豸���������������߳���׼���ã��ڴ�ʹ��     
 ���� CONTROL_RET_FAIL ����ʧ�ܣ� CONTROL_RET_SUC ���ͳɹ�
 ************************************************************************************************/
void server_com_rev(int u32DeviceFd) {
	unsigned char c;
	fd_set ReadSetFD;
	FD_ZERO(&ReadSetFD);
	FD_SET(u32DeviceFd, &ReadSetFD);
	select(u32DeviceFd + 1, &ReadSetFD, 0, 0, 0);
}

