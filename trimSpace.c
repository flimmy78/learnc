#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define IS_SPACE    0x01
#define NOT_SPACE   0x00
#define U8 unsigned char
#define U16 unsigned short
#define NO_ERR  0x00
#define ERROR   0x01
#define ASCII_TO_HEX(c) ((c >='0' && c <='9')?(c-'0'):((c>='A'&&c<='F')?(c-'A'+10):((c>='a'&&c<='f')?(c-'a'+10):0)))
#define GATEWAY_OADD_LEN    6
#define GATEWAY_RETID_OFFSET   4 
#define CONFIG_DELIMITER    ',' 

typedef struct{
    int baud;
    int mode;
    int device;
}comstr;
static comstr gComConfig;

typedef struct
{
	U8 rowId[2];			//计量点, 用作唯一标示一行
	U8 meterAddr[7];        //仪表(热量表,水表等)地址
	U8 vendorId;            //厂商代码
	U8 protoVer;            //仪表协议版本号
	U8 meterType;           //仪表类型
	U8 channel;             //通道号
	U8 valveProtoVer;       //阀控协议版本号
	U8 valveAddr[7];        //阀控地址
	U8 controlPanelAddr[7]; //温控面板地址
	U8 buildId;             //楼栋号
	U8 unitId;              //单元号
	U8 roomId[2];              //房间号
	U8 reserved[8];         //保留字段
}meter_row_str;
typedef meter_row_str* meter_row_ptr;



static const U8 spaces[] = { ' ', '\t', '\n', '\r', 0x00};
U8 isSpace(U8 s)
{
    U16 i = 0;
    U16 arrayLen = sizeof(spaces);
    for (i = 0;i < arrayLen;i++)
        if(s == spaces[i])
            return IS_SPACE;
    return NOT_SPACE;
}

U8 trimSpace(U8* s, U16 len)
{
	U8* p = s;
	U8	buf[256];
	U16 i=0 ,j=0;
	for (i = 0, j = 0;i < len; p++, i++) {
		if(!isSpace(*p)) {
			buf[j] = *p;
			j++;
		}
	}
	memset(s, 0x00, len);
	memcpy(s, buf, j+1);
	return NO_ERR;
}

U8 inverseStrToBCD(U8* s, U16 sLen, U8* t, U16 tLen)
{
	U16 i = 0, j = 0;

	if (sLen % 2)
		return ERROR;

	for (i = 0;i < sLen/2;i++, s += 2) {
		t[i] = (ASCII_TO_HEX(s[0]) << 4 | ASCII_TO_HEX(s[1]));
	}

	for (j = 0;j < i/2; j++) {
		t[j] = t[j] ^ t[i - j - 1];
		t[i - j -1 ] = t[j] ^ t[i - j - 1];
		t[j] = t[j] ^ t[i - j -1];
	}

	return NO_ERR;
}

U8 protoA_radioReadId(U8 *gatewayId, U8 idLen, U8* buf, U16 bufSize)
{
	U16 i = 0;
	if (bufSize<(GATEWAY_RETID_OFFSET+ GATEWAY_OADD_LEN) || idLen < GATEWAY_OADD_LEN) {
		return ERROR;
	}
	buf += GATEWAY_RETID_OFFSET;
	memset(gatewayId, 0, idLen);
	memcpy(gatewayId, buf, GATEWAY_OADD_LEN);
	for (i = 0;i < GATEWAY_OADD_LEN / 2;i++) {//inverse gatewayId
		gatewayId[i] = gatewayId[i] ^ gatewayId[GATEWAY_OADD_LEN - i - 1];
		gatewayId[GATEWAY_OADD_LEN - i - 1] = gatewayId[i] ^ gatewayId[GATEWAY_OADD_LEN - i - 1];
		gatewayId[i] = gatewayId[i] ^ gatewayId[GATEWAY_OADD_LEN - i - 1];
	}
	return NO_ERR;
}

void readComConfig(U8* data)
{
	U8* pData = data;
	U16 configLen = 0;
	U8 tempBuf[32] = { 0 };
	//read baud
	while (pData[configLen] != CONFIG_DELIMITER) configLen++;
	memcpy(tempBuf, pData, configLen);
	gComConfig.baud = atoi(tempBuf);
	pData += configLen + 1;
	memset(tempBuf, 0, 32);
	//read mode
	configLen = 0;
	while (pData[configLen] != CONFIG_DELIMITER) configLen++;
	memcpy(tempBuf, pData, configLen);
	gComConfig.mode = atoi(tempBuf);
	pData += configLen + 1;
	memset(tempBuf, 0, 32);
	//read device
	configLen = 0;
	while (pData[configLen] != '\0') configLen++;
	memcpy(tempBuf, pData, configLen);
	gComConfig.device = atoi(tempBuf);
    printf("baud: %d, mode: %d, device: %d\n", gComConfig.baud, gComConfig.mode, gComConfig.device);
}

int main(void)
{
    printf("meter_row_str's length: %d\n", sizeof(meter_row_str));
    return 0;
}
