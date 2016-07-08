#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define CONFIG_FILED_CNT		2
#define CONFIG_FIELD_NAME_ID	"f_id"
#define CONFIG_FIELD_NAME_VALUE	"f_value"

#define CONFIG_DELIMITER	','

#define DB_MINFO_LEN_ROWID			3	//数据库中计量点长度
#define DB_MINFO_LEN_METERADDR		14	//数据库中仪表地址长度
#define DB_MINFO_LEN_VENDORID		3	//数据库中厂商代码长度
#define DB_MINFO_LEN_PROTOVER		3	//数据库中仪表协议版本号长度
#define DB_MINFO_LEN_METERTYPE		2	//数据库中仪表类型长度
#define DB_MINFO_LEN_CHANNEL		2	//数据库中通道号长度
#define DB_MINFO_LEN_VALVEPROTOVER	2	//数据库中阀控协议版本号长度
#define DB_MINFO_LEN_VALVEADDR		14	//数据库中阀控地址长度
#define DB_MINFO_LEN_CTLPANELADDR	14	//数据库中温控面板地址长度
#define DB_MINFO_LEN_BUILDID		3	//数据库中楼栋号长度
#define DB_MINFO_LEN_UNITID			2	//数据库中单元号长度
#define DB_MINFO_LEN_ROOMID			4	//数据库中房间号长度
#define DB_MINFO_LEN_GATEWAYID		12	//集中器编号长度


#define  FILE_LINE   __FILE__,__FUNCTION__,__LINE__

#define NO_ERR	0x00//无错
#define ERROR	0x01//有错

#define WM_USER_EXIT	-1
#define EDIT_MAX_LEN	128
#define I8    signed char
#define U8  unsigned char     	/* unsigned 8  bits. */
#define I16   signed short    	/*   signed 16 bits. */
#define U16 unsigned short    	/* unsigned 16 bits. */
#define I32   signed long   	/*   signed 32 bits. */
#define U32 unsigned long   	/* unsigned 32 bits. */
#define I16P I16              	/*   signed 16 bits OR MORE ! */
#define U16P U16              	/* unsigned 16 bits OR MORE ! */

#define S8    char
#define S16   signed short    	/*   signed 16 bits. */
#define S32   signed long   	/*   signed 32 bits. */

#define PROTOCOL_VER 0x03	//协议版本号

#if PROTOCOL_VER==0x03
#define GATEWAY_PREFIX      0xFB    //集中器通讯协议的前导符
#define GATEWAY_SUFIX       0xFD    //集中器通讯协议的结束符
#define GATEWAY_START       0x7B    //集中器通讯协议的起始符
#define GATEWAY_PREFIX_CNT  2       //集中器通讯协议的前导符个数
#define GATEWAY_SUFIX_CNT   2       //集中器通讯协议的结束符个数
#define GATEWAY_START_CNT   1       //集中器通讯协议的起始符个数
#define GATEWAY_VER_LEN     1       //集中器通讯协议的版本号长度
#define GATEWAY_SADD_LEN    6       //集中器通讯协议的源地址长度
#define GATEWAY_OADD_LEN    6       //集中器通讯协议的目标地址长度
#define GATEWAY_MID_LEN     1       //集中器通讯协议的消息序列号长度
#define GATEWAY_MSGL_LEN    2       //集中器通讯协议的消息字节数长度
#define GATEWAY_MT_LEN      1       //集中器通讯协议的消息类型长度
#define GATEWAY_TS_LEN      6       //集中器通讯协议的年月日长度
#define GATEWAY_HCK_LEN     1       //集中器通讯协议的头部校验长度
#define GATEWAY_EC_LEN      1       //集中器通讯协议的消息体校验长度
#define GATEWAY_HEAD_LEN    (GATEWAY_VER_LEN+GATEWAY_SADD_LEN+GATEWAY_OADD_LEN+GATEWAY_MID_LEN\
                            +GATEWAY_MSGL_LEN+GATEWAY_MT_LEN+GATEWAY_TS_LEN)
#define GATEWAY_RETID_OFFSET	(GATEWAY_PREFIX_CNT+GATEWAY_START_CNT+GATEWAY_VER_LEN)//offset that gatewayId in return frame
#define GATEWAY_MAIN_FRAME_LEN	30	//集中器协议中 排除消息体部分的帧长度(包括消息体校验)
#define GATEWAY_FRAME_MAX_LEN	1024//每帧协议最大长度
#define GATEWAY_METERINFO_LEN	40//仪表信息的长度
#define GATEWAY_MAX_METERINFO_CNT	24//集中器协议中每行最大下发多少行数据
#define GATEWAY_ISSUE_BODY_HEAD	3//下发表地址消息头的长度
//异常回应代码--表示意义
#define GAT_EXCEP_FAIL          0x10//失败、异常
#define GAT_EXCEP_MEASURE_POINT 0x11//异常，查不到指定计量点号。
#define GAT_EXCEP_CHANNEL_BUSY  0x12//异常，MBUS 通道正忙，现在不能抄表。
#define GAT_EXCEP_PARAM_EXCEED  0x13//参数超出允许范围
#define GAT_EXCEP_PROTO_VER     0x14//消息体版本号错误

/*集中器协议中MSG_TYPE代表的意义 START*/
#define GAT_MT_SVR_CP_1_METER   0x06//主站下发抄单表指令(0x06)
#define GAT_MT_CLT_CP_1_METER   0x07//集中器向主站发送单抄表结果(0x07)

#define GAT_MT_SVR_1OF_MFRM     0x11//主站请求多包传输特定包数据(0x11)
#define GAT_MT_SVR_MODIFY_SINFO 0x13//主站更改单个计量点基础信息(0x13)
#define GAT_MT_CLT_MODIFY_SINFO 0x14//集中器响应主站更改单个热表地址信息(0x14)

#define GAT_MT_SVR_SEND_MINFO   0x0C//主站下发所有仪表地址(0x0C)
#define GAT_MT_CLT__SEND_MINFO	0x0D//集中器响应主站下发仪表地址(0x0D)

#define GAT_MT_SVR_TIME_POINT   0x20//主站设置定时抄表时间(0x20)
#define GAT_MT_CLT_TIME_POINT   0x21//设置定时抄表时间反馈(0x21)

#define GAT_MT_SVR_HISDATA      0x22//主站向集中器请求历史时间点的所有表数据(0x22)
#define GAT_MT_CLT_HISDATA      0x23//集中器向主站发送固定时间点数据内容(0x23)

#define GAT_MT_SVR_TIME_SET     0x24//设置集中器时钟时间(0x24)
#define GAT_MT_CLT_TIME_SET     0x25//集中器响应设置时间(0x25)

#define GAT_MT_SVR_TIME_RD      0x26//读取集中器时钟时间(0x26)
#define GAT_MT_CLT_TIME_RD      0x27//集中器响应设置时间(0x27)

#define GAT_MT_CLT_LOGIN        0x2C//GPRS登陆帧(0x2C)
#define GAT_MT_SVR_LOGIN        0x2D//主站回应GPRS登录(0x2D)

#define GAT_MT_CLT_HB           0x2E//GPRS心跳(0x2E)
#define GAT_MT_SVR_HB           0x2F//GPRS心跳回应(0x2F)

#define GAT_MT_SVR_REBOOT       0x34//集中器重启指令(0x34)
#define GAT_MT_CLT_REBOOT       0x35//对重启指令的回应(0x35)集中器收到重启指令后, 会先回应上位机, 5秒后重启

#define GAT_MT_SVR_SHISDATA     0x3C//主站向集中器请求历史时间点某单表的数据(0x3C)
#define GAT_MT_CLT_SHISDATA     0x3D//集中器向主站发送固定时间点某单表的数据(0x3D)

#define GAT_MT_SVR_CPY_IMMDT    0x3E//设置集中器立即抄表(0x3E)
#define GAT_MT_CLT_CPY_IMMDT    0x3F//集中器响应立即抄表设置(0x3F)

#define GAT_MT_SVR_RD_CONFIG    0x90//主站读取集中器参数(0x90)
#define GAT_MT_CLT_RD_CONFIG    0x91//主站读取集中器参数响应(0x91)

#define GAT_MT_SVR_RD_ALLINFO   0x92//主站读取集中器中全部热表地址信息 (0x92 )
#define GAT_MT_CLT_RD_ALLINFO   0x93//集中器向主站发送热表地址信息(0x93)

#define GAT_MT_SVR_CHIP         0x94//主站更改集中器IP及端口号(0x94)
#define GAT_MT_CLT_CHIP         0x95//主站更改集中器IP及端口号响应(0x95)
/*集中器协议中MSG_TYPE代表的意义 END*/

#define	GATEWAY_ASW_CODE_SUC				0x01	//操作成功
#define GATEWAY_ASW_CODE_FAIL				0x10	//操作失败
#define GATEWAY_ASW_CODE_NOREC				0x11	//查不到指定计量点号
#define GATEWAY_ASW_CODE_MBUS_BUSY			0x12	//MBUS正忙
#define GATEWAY_ASW_CODE_EXCEED				0x13//参数出允许范围
#define GATEWAY_ASW_CODE_MSGBODY_VER_ERR	0x14	//消息体版本号错误


#endif

#define PROTO_LEN_MADDR		7//协议中仪表地址长度
#define PROTO_LEN_VADDR		7//协议中阀门地址长度
#define PROTO_LEN_ROWID		2//协议中计量点编号长度
#define PROTO_LEN_ROOMID	2//协议中房间编号长度
#define PROTO_LEN_RSV		8//保留字段

//为避免编译器字节对齐, 全部使用单字节结构
typedef struct {//集中器协议体结构
	U8 SourceAddr[GATEWAY_SADD_LEN];//源地址, 低字节在前
	U8 DestAddr[GATEWAY_OADD_LEN];  //目标地址, 低字节在前
	U8 MsgIndex;                    //消息序列号
	U8 MsgLen[GATEWAY_MSGL_LEN];    //消息体长度, 低字节在前
	U8 MsgType;                     //消息类型
	U8 ssmmhhDDMMYY[GATEWAY_TS_LEN];//秒分时日月年, 低字节在前
	U8 *MsgBody;                    //消息体, 低字节在前
} gateway_protocol_str;//类型名用下划线分隔, 下同
typedef gateway_protocol_str* gateway_protocol_ptr;

typedef struct{//集中器仪表基础信息结构
	U8 rowId[PROTO_LEN_ROWID];		//计量点, 用作唯一标示一行(Little Ending, Hex)
	U8 meterAddr[PROTO_LEN_MADDR];  //仪表(热量表,水表等)地址(Little Ending, BCD)
	U8 vendorId;					//厂商代码(Hex)
	U8 protoVer;					//仪表协议版本号(Hex)
	U8 meterType;					//仪表类型(Hex)
	U8 channel;						//通道号(Hex)
	U8 valveProtoVer;				//阀控协议版本号(Hex)
	U8 valveAddr[PROTO_LEN_VADDR];  //阀控地址(Little Ending, BCD)
	U8 controlPanelAddr[PROTO_LEN_VADDR]; //温控面板地址(Little Ending, BCD)
	U8 buildId;						//楼栋号(Hex)
	U8 unitId;						//单元号(Hex)
	U8 roomId[PROTO_LEN_ROOMID];    //房间号(Little Ending, Hex)
	U8 reserved[PROTO_LEN_RSV];     //保留字段
}meter_row_str;
typedef meter_row_str* meter_row_ptr;

typedef struct
{
	U8 totalRows;			//一共有多少行表地址
	U8 seq;					//本帧的索引号
	U8 thisRows;			//本帧包含多少行表地址
}meterinfo_bodyHead_str;
typedef meterinfo_bodyHead_str* meterinfo_bodyHead_ptr;

#define HEX_TO_BCD(x) (((x)/0x0A)*0x10+((x)%0x0A))
#define BCD_TO_HEX(x) (((x)/0x10)*0x0A+((x)%0x10))

#define ASCII_TO_HEX(c) ((c >='0' && c <='9')?(c-'0'):((c>='A'&&c<='F')?(c-'A'+10):((c>='a'&&c<='f')?(c-'a'+10):0)))
#define isdigit(c)	((unsigned) ((c)-'0') < 10)

#define IS_SPACE	0x01
#define NOT_SPACE	0x00


typedef struct{
	U8 rowId[DB_MINFO_LEN_ROWID+1];					//计量点
	U8 meterAddr[DB_MINFO_LEN_METERADDR];			//仪表(热量表,水表等)地址
	U8 vendorId[DB_MINFO_LEN_VENDORID+1];				//厂商代码
	U8 protoVer[DB_MINFO_LEN_PROTOVER+1];				//仪表协议版本号
	U8 meterType[DB_MINFO_LEN_METERTYPE];			//仪表类型
	U8 channel[DB_MINFO_LEN_CHANNEL+1];				//通道号
	U8 valveProtoVer[DB_MINFO_LEN_VALVEPROTOVER+1];	//阀控协议版本号
	U8 valveAddr[DB_MINFO_LEN_VALVEADDR];			//阀控地址
	U8 controlPanelAddr[DB_MINFO_LEN_CTLPANELADDR];	//温控面板地址
	U8 buildId[DB_MINFO_LEN_BUILDID+1];				//楼栋号
	U8 unitId[DB_MINFO_LEN_UNITID+1];					//单元号
	U8 roomId[DB_MINFO_LEN_ROOMID+1];					//房间号
	U8 gatewayId[DB_MINFO_LEN_GATEWAYID];			//集中器编号
} db_meterinfo_str;

typedef db_meterinfo_str* db_meterinfo_ptr;


typedef struct{//系统时间结构, 倒序存储以方便使用
	U8 u8second;	//秒
	U8 u8minute;	//分
	U8 u8hour;		//时
	U8 u8day;		//日
	U8 u8month;		//月
	U8 u8year;		//年
}sys_time_str;
typedef sys_time_str* sys_time_ptr;

//手持机设置的索引号, 在数据库中的行号也与此对应
typedef enum {
	config_com_para = 0,//端口相关参数
	config_gateway_id,	//集中器号
	config_server_id	//主站号
}config_rowidx;


static const U8 spaces[] = { ' ', '\t', '\n', '\r', '\0' };

//读取系统时间
U8 readSysTime(sys_time_ptr pTime)
{
	pTime->u8year	= HEX_TO_BCD(16);
	pTime->u8month  = HEX_TO_BCD(7);
	pTime->u8day    = HEX_TO_BCD(7);
	pTime->u8hour	   = HEX_TO_BCD(15);
	pTime->u8minute = HEX_TO_BCD(46);
	pTime->u8second = HEX_TO_BCD(23);

	return 0;
}

//校验, 累加和取反加一
U8 countCheck(U8 *data, U16 len)
{
	U8 cs = 0;
	U16 i;
	for (i = 0;i < len;i++, data++)
		cs += *data;

	return ((~cs) + 1);
}

/*
**	判断字符是否是空格.
**	@s:	字符
*/
U8 isSpace(U8 s)
{
	U16 i = 0;
	U16 arrayLen = sizeof(spaces);
	for (i = 0;i < arrayLen;i++)
		if(s == spaces[i])
			return IS_SPACE;
	return NOT_SPACE;
}

/*
**	去掉字符串中的空格.
**	@s:		字符串
**	@len:	字符串长度
*/
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

/*
**	检查字符串是否都是数字.
**	@s:		字符串
**	@len:	字符串长度
*/
U8 isNumber(U8* s, U16 len)
{
	U16 i = 0;
	for (i = 0;i < len; s++, i++) {
		if (!isdigit(*s)) {
			return ERROR;
		}
	}

	return NO_ERR;
}


void inverseArray(U8* buf, U16 bufSize)
{
        U16 i = 0;
        for (i = 0;i < bufSize / 2; i++) {//swap two symmetric elements
                buf[i] = buf[i] ^ buf[bufSize - i - 1];
                buf[bufSize - i - 1] = buf[i] ^ buf[bufSize - i - 1];
                buf[i] = buf[i] ^ buf[bufSize - i - 1];
        }
}

/*
**	反序输入的字符串至BCD码.
**	@s:		源字符串
**	@sLen:	源字符串长度
**	@t:		目标字符串
**	@tLen:	目标字符串长度
*/
U8 inverseStrToBCD(U8* s, U16 sLen, U8* t, U16 tLen)
{
	U16 i = 0;

	if (sLen % 2)
		return ERROR;

	for (i = 0;i < sLen/2;i++, s += 2) {//convert ascii to hex each two chars
		t[i] = (ASCII_TO_HEX(s[0]) << 4 | ASCII_TO_HEX(s[1]));
	}

	inverseArray(t, i);
	return NO_ERR;
}

/*
**	将数字字符串的最前面的'0'去掉.
**	@buf:		字符串
**	@bufSize:	字符串长度
*/
void trimZero(U8* buf, U8 bufSize)
{
	U16 i = 0;
	U8* lu8pBuf = buf;
	U8 tmpBuf[512] = { 0 };
	while (i < bufSize && (*lu8pBuf == '0')) {
		i++;
		lu8pBuf++;
	}
	memcpy(tmpBuf, lu8pBuf, bufSize - i);
	memset(buf, 0, bufSize);
	memcpy(buf, tmpBuf, bufSize - i);
}

/*
**	将不足12位的集中器号前面补零.
**	@data:	用户输入的集中器号
*/
void suppplementTo12(U8* data)
{
	U8 lu8tmpStr[EDIT_MAX_LEN] = { 0 };
	U8 lu8InputLen = 0;

	//supplement '0' before data, if lu8InputLen < 2 * GATEWAY_OADD_LEN
	lu8InputLen = strlen((const char*)data);
	//we use 2 chars to represent a byte, so the mod is 2.
	if (lu8InputLen % 2) {//if lu8InputLen is Odd, lu8InputLen must <= (2 * GATEWAY_OADD_LEN - 1)
		if (lu8InputLen > 2 * GATEWAY_OADD_LEN - 1)
			return;
	}
	else {//if lu8InputLen is Even, lu8InputLen must <= 2 * GATEWAY_OADD_LEN
		if (lu8InputLen > 2 * GATEWAY_OADD_LEN)
			return;
	}
	memset(lu8tmpStr, '0', 2 * GATEWAY_OADD_LEN - lu8InputLen);
	memcpy(lu8tmpStr + (2 * GATEWAY_OADD_LEN - lu8InputLen), data, lu8InputLen);
	memcpy(data, lu8tmpStr, 2 * GATEWAY_OADD_LEN);
}

/*
**	将数据库读取上来的表地址数据结构转化为协议结构.
**	@pDbInfo:	数据库中的信息指针
**	@pProtoInfo:	协议帧指针
*/
void asciiToProtoBin(db_meterinfo_ptr pDbInfo, meter_row_ptr pProtoInfo)
{
	U16 i = 0;

	i = atoi((const char*)pDbInfo->rowId);
	pProtoInfo->rowId[0] = (U8)i;//L
	pProtoInfo->rowId[1] = (U8)(i >> 8);//H
	inverseStrToBCD(pDbInfo->meterAddr, DB_MINFO_LEN_METERADDR, pProtoInfo->meterAddr, PROTO_LEN_MADDR);
	pProtoInfo->vendorId = atoi((const char*)pDbInfo->vendorId);
	pProtoInfo->protoVer = atoi((const char*)pDbInfo->protoVer);
	pProtoInfo->meterType = (ASCII_TO_HEX(pDbInfo->meterType[0]) << 4 | ASCII_TO_HEX(pDbInfo->meterType[1]));
	pProtoInfo->channel = atoi((const char*)pDbInfo->channel);
	pProtoInfo->valveProtoVer = atoi((const char*)pDbInfo->valveProtoVer);
	inverseStrToBCD(pDbInfo->valveAddr, DB_MINFO_LEN_METERADDR, pProtoInfo->valveAddr, PROTO_LEN_VADDR);
	inverseStrToBCD(pDbInfo->controlPanelAddr, DB_MINFO_LEN_METERADDR, pProtoInfo->controlPanelAddr, PROTO_LEN_VADDR);
	pProtoInfo->buildId = atoi((const char*)pDbInfo->buildId);
	pProtoInfo->unitId = atoi((const char*)pDbInfo->unitId);
    printf("original roomId: %s\n", pDbInfo->roomId);
	i = atoi((const char*)pDbInfo->roomId);
    printf("roomId: %d\n", i);
	pProtoInfo->roomId[0] = (U8)i;//L
	pProtoInfo->roomId[1] = (U8)(i >> 8);//H
    printf("transformed roomId: %02X %02X\n", pProtoInfo->roomId[0], pProtoInfo->roomId[1]);
	memset(pProtoInfo->reserved, 0, PROTO_LEN_RSV);
}


//创建集中器通讯桢, 私有函数
static void createFrame(U8 *sendBuf, U16 *sendLen, gateway_protocol_ptr pProto)
{
	U8 *pTemp = sendBuf;
	U16 lenFrame = 0;
	U8 *pHeadStart = NULL;//消息头起始位置, 用于计算消息头的校验值
	U8 *pBodyStart = NULL;//消息体起始位置, 用于计算消息体的校验值
	U16 u16dataLen = 0;
	
	printf("Line %d, Func createFrame\n", __LINE__);
	memset(pTemp, GATEWAY_PREFIX, GATEWAY_PREFIX_CNT);//前导符
	pTemp += GATEWAY_PREFIX_CNT;
	*pTemp++ = GATEWAY_START;//起始符
	pHeadStart = pTemp;
	lenFrame += GATEWAY_PREFIX_CNT + GATEWAY_START_CNT;

	printf("Line %d, Func createFrame\n", __LINE__);
	*pTemp++ = PROTOCOL_VER;//版本号

	printf("Line %d, Func createFrame\n", __LINE__);
	memcpy(pTemp, pProto->SourceAddr, GATEWAY_SADD_LEN);//源地址
	pTemp += GATEWAY_SADD_LEN;
	memcpy(pTemp, pProto->DestAddr, GATEWAY_OADD_LEN);//目标地址
	pTemp += GATEWAY_OADD_LEN;

	printf("Line %d, Func createFrame\n", __LINE__);
	*pTemp++ = pProto->MsgIndex;//MID 消息序列
	memcpy(pTemp, pProto->MsgLen, GATEWAY_MSGL_LEN);//消息体长度
	pTemp += GATEWAY_MSGL_LEN;
	*pTemp++ = pProto->MsgType;//消息类型

	printf("Line %d, Func createFrame\n", __LINE__);
	memcpy(pTemp, pProto->ssmmhhDDMMYY, GATEWAY_TS_LEN);//时间
	pTemp += GATEWAY_TS_LEN;

	printf("Line %d, Func createFrame\n", __LINE__);
	*pTemp++ = countCheck(pHeadStart, GATEWAY_HEAD_LEN);//消息头校验
	pBodyStart = pTemp;//消息体校验字节预留

	printf("Line %d, Func createFrame\n", __LINE__);
	u16dataLen = (pProto->MsgLen[1] << 8 | pProto->MsgLen[0]);//消息体长度
	printf("dataBody's Length: %d\n", u16dataLen);
	memcpy(pTemp, pProto->MsgBody, u16dataLen);//消息体
	pTemp += u16dataLen;
	*pTemp++ = countCheck(pBodyStart, u16dataLen);//消息体校验
	memset(pTemp, GATEWAY_SUFIX, GATEWAY_SUFIX_CNT);

	printf("Line %d, Func createFrame\n", __LINE__);
	lenFrame += (GATEWAY_HEAD_LEN + GATEWAY_HCK_LEN);//协议头长度+协议头校验长度
	lenFrame += u16dataLen;//消息体长度
	lenFrame += (GATEWAY_EC_LEN + GATEWAY_SUFIX_CNT);//消息体校验长度+结束符长度
	*sendLen = lenFrame;
}

U8 protoW_issueMinfo(U8* buf, U16* bufSize, U8* gatewayId, \
	meterinfo_bodyHead_ptr pBodyHead, meter_row_ptr	pProtoInfo)
{
	gateway_protocol_str protoStr;
	U8 bufMsgBody[GATEWAY_FRAME_MAX_LEN] = {0};
	U16 len = 0;

    memset(protoStr.DestAddr, 0xff, 6);
	protoStr.DestAddr[0] = 0x34;
	protoStr.DestAddr[1] = 0x12;
	protoStr.DestAddr[2] = 0x00;
	protoStr.DestAddr[3] = 0x00;
	protoStr.DestAddr[4] = 0x00;
	protoStr.DestAddr[5] = 0x00;
	
	protoStr.SourceAddr[0] = 0x01;
	protoStr.SourceAddr[1] = 0x00;
	protoStr.SourceAddr[2] = 0x00;
	protoStr.SourceAddr[3] = 0x00;
	protoStr.SourceAddr[4] = 0x00;
	protoStr.SourceAddr[5] = 0x00;
	
	protoStr.MsgIndex = pBodyHead->seq;

	len = GATEWAY_METERINFO_LEN*pBodyHead->thisRows + sizeof(meterinfo_bodyHead_str);
	
	memcpy(protoStr.MsgLen, (U8*)&len, GATEWAY_MSGL_LEN);
	printf("protoStr.MsgLen[0]: %02X, protoStr.MsgLen[1]: %02X\n", protoStr.MsgLen[0], protoStr.MsgLen[1]);
	//inverseArray(protoStr.MsgLen, GATEWAY_MSGL_LEN);
	//printf("protoStr.MsgLen[0]: %02X, protoStr.MsgLen[1]: %02X\n", protoStr.MsgLen[0], protoStr.MsgLen[1]);
	printf("BodyLen: %d, 0x%04X; protoStr.MsgLen[0]: %02X, protoStr.MsgLen[1]: %02X\n", len, len, protoStr.MsgLen[0], protoStr.MsgLen[1]);
	protoStr.MsgType = GAT_MT_SVR_SEND_MINFO;
	readSysTime((sys_time_ptr)protoStr.ssmmhhDDMMYY);
	printf("Line %d, Func protoW_issueMinfo\n", __LINE__);
	memcpy(bufMsgBody, (U8*)pBodyHead, sizeof(meterinfo_bodyHead_str));//copy body head
	printf("Line %d, Func protoW_issueMinfo\n", __LINE__);
	memcpy(bufMsgBody + sizeof(meterinfo_bodyHead_str), (U8*)pProtoInfo, \
	pBodyHead->thisRows*sizeof(meter_row_str));//copy message body
	printf("Line %d, Func protoW_issueMinfo\n", __LINE__);
	protoStr.MsgBody = bufMsgBody;
	printf("Line %d, Func protoW_issueMinfo\n", __LINE__);
	createFrame(buf, bufSize, &protoStr);
	printf("Line %d, Func protoW_issueMinfo\n", __LINE__);
	return NO_ERR;
}

int main(void)
{
	int row = 0, i = 0;
	U16 bufSize = 0;
	U8 buf[1024] = { 0 };

	meterinfo_bodyHead_str bodyHeadStr;
    db_meterinfo_ptr pDbInfo = malloc(3*sizeof(db_meterinfo_str));
    meter_row_ptr pProtoInfo = malloc(3*sizeof(meter_row_str));
	
	memset(pDbInfo, 0, 3*sizeof(db_meterinfo_str));
	memset(pProtoInfo, 0, 3*sizeof(meter_row_str));

	strcpy(pDbInfo->rowId , "1");
	strcpy(pDbInfo->meterAddr, "11110020160708");
	strcpy(pDbInfo->vendorId, "1");
	strcpy(pDbInfo->protoVer, "0");
	strcpy(pDbInfo->meterType, "20");
	strcpy(pDbInfo->channel, "1");
	strcpy(pDbInfo->valveProtoVer, "0");
	strcpy(pDbInfo->valveAddr, "11110020160709");
	strcpy(pDbInfo->controlPanelAddr, "11110020160709");
	strcpy(pDbInfo->buildId, "1");
	strcpy(pDbInfo->unitId, "4");
	strcpy(pDbInfo->roomId, "1102");
	strcpy(pDbInfo->gatewayId , "000000009017");
	
	strcpy((pDbInfo+1)->rowId , "2");
	strcpy((pDbInfo+1)->meterAddr, "11110020170808");
	strcpy((pDbInfo+1)->vendorId, "1");
	strcpy((pDbInfo+1)->protoVer, "0");
	strcpy((pDbInfo+1)->meterType, "20");
	strcpy((pDbInfo+1)->channel, "1");
	strcpy((pDbInfo+1)->valveProtoVer, "0");
	strcpy((pDbInfo+1)->valveAddr, "11110020170809");
	strcpy((pDbInfo+1)->controlPanelAddr, "11110020170809");
	strcpy((pDbInfo+1)->buildId, "2");
	strcpy((pDbInfo+1)->unitId, "3");
	strcpy((pDbInfo+1)->roomId, "1517");
	strcpy((pDbInfo+1)->gatewayId , "000000009017");
	
	strcpy((pDbInfo+2)->rowId , "3");
	strcpy((pDbInfo+2)->meterAddr, "11110020180908");
	strcpy((pDbInfo+2)->vendorId, "1");
	strcpy((pDbInfo+2)->protoVer, "0");
	strcpy((pDbInfo+2)->meterType, "20");
	strcpy((pDbInfo+2)->channel, "1");
	strcpy((pDbInfo+2)->valveProtoVer, "0");
	strcpy((pDbInfo+2)->valveAddr, "11110020180909");
	strcpy((pDbInfo+2)->controlPanelAddr, "11110020180909");
	strcpy((pDbInfo+2)->buildId, "4");
	strcpy((pDbInfo+2)->unitId, "7");
	strcpy((pDbInfo+2)->roomId, "0102");
	strcpy((pDbInfo+2)->gatewayId , "000000009017");
	
	asciiToProtoBin(pDbInfo, pProtoInfo);
	asciiToProtoBin(pDbInfo+1, pProtoInfo+1);
	asciiToProtoBin(pDbInfo+2, pProtoInfo+2);
	
	for(row=0;row<3;row++){
		printf("pProtoInfo[%d]: ", row);
		for(i=0;i<sizeof(meter_row_str);i++) {
			printf("%02X ", ((U8*)(pProtoInfo+row))[i]);
		}
		printf("\n");
	}
	
	bodyHeadStr.totalRows = 3;
	bodyHeadStr.seq = 0;
	bodyHeadStr.thisRows = 3;
	
	protoW_issueMinfo(buf, &bufSize, NULL, &bodyHeadStr, pProtoInfo);
	printf("buf: ", row);
	for(i=0;i<bufSize;i++) {
		printf("%02X ", buf[i]);
	}
	printf("\n");
    return 0;
}
