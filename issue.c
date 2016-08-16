#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define CONFIG_FILED_CNT	2
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

#define	TIME_NODE_DELIM	';'//两个时间点之间的分隔符
#define	TIME_DELIM	':'
#define isDelim(c)	((c==':') || (c=='.'))
#define MAX_TIME_NODE	24//抄表时间点不超过24个点
#define MINUTES_PERDAY	1440//一天有多少分钟
#define WM_USER_EXIT	-1
#define EDIT_MAX_LEN	128
#define I8    signed char
#define U8  unsigned char     	/* unsigned 8  bits. */
#define I16   signed short    	/*   signed 16 bits. */
#define U16 unsigned short    	/* unsigned 16 bits. */
#define I32   signed int   		/*   signed 32 bits. */
#define U32 unsigned int   		/* unsigned 32 bits. */

#define S8    char
#define S16   signed short    	/*   signed 16 bits. */
#define S32   signed int   		/*   signed 32 bits. */

#define GATEWAY_PROTOCOL_VER 0x03	//协议版本号

#ifdef GATEWAY_PROTOCOL_VER
#if GATEWAY_PROTOCOL_VER==0x03
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
#define GATEWAY_BODY_OFFSET	27//消息体偏移量
#define GATEWAY_BODYLEN_OFFSET	17//消息体长度偏移量
#define GATEWAY_MAX_METERINFO_CNT	24//集中器协议中每行最大下发多少行数据
#define GATEWAY_ISSUE_BODY_HEAD	3//下发表地址消息头的长度
#define GATEWAY_ASWCODE_OFFSET		19//集中器返回帧中应答码的偏移量
#define GATEWAY_STATCODE_OFFSET		27//集中器返回帧中成功状态码的偏移量
#define GATEWAY_SEQCODE_OFFSET		28//集中器返回帧中服务器发送的序列码的偏移量
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

#define GAT_MT_SVR_REREAD       0x3A//主站更改补抄表相关参数(0x3A)单位ms
#define GAT_MT_CLT_REREAD       0x3B//集中器响应主站更改补抄表相关参数(0x3B)单位ms

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

#define GAT_MT_SVR_MID			0x98//主站更改集中器及其主站编号
#define GAT_MT_CLT_MID			0x99//集中器响应主站更改集中器及其主站编号

/*集中器协议中MSG_TYPE代表的意义 END*/

#define	GATEWAY_ASW_CODE_SUC				0x01	//操作成功
#define GATEWAY_ASW_CODE_FAIL				0x10	//操作失败
#define GATEWAY_ASW_CODE_NOREC				0x11	//查不到指定计量点号
#define GATEWAY_ASW_CODE_MBUS_BUSY			0x12	//MBUS正忙
#define GATEWAY_ASW_CODE_EXCEED				0x13//参数出允许范围
#define GATEWAY_ASW_CODE_pMsgBody_VER_ERR	0x14	//消息体版本号错误


#endif
#endif

#define PROTO_LEN_MADDR		7//协议中仪表地址长度
#define PROTO_LEN_VADDR		7//协议中阀门地址长度
#define PROTO_LEN_ROWID		2//协议中计量点编号长度
#define PROTO_LEN_ROOMID	2//协议中房间编号长度
#define PROTO_LEN_RSV		8//保留字段

#define LISTVIEW_COL_ITEM	0//修改单行基础信息时, 表头列索引
#define LISTVIEW_COL_VALUE	1//修改单行基础信息时, 值列索引
#define LISTVIEW_TO_DBSTR(db_field, db_field_len, db_field_idx, err_pos)	memset(l8buf, 0, EDIT_MAX_LEN);\
							LISTVIEW_GetItemText(hObjListview, LISTVIEW_COL_VALUE, db_field_idx, l8buf, EDIT_MAX_LEN);\
							lu16strLen = strlen(l8buf);\
							trimSpace(l8buf, lu16strLen);\
							lu16strLen = strlen(l8buf);\
							sprintf(msgBuf, "\n请在%s处输入数字!\n", err_pos);\
							if (isNumber(l8buf, lu16strLen)) {\
								GUI_MessageBox(msgBuf, "失败", GUI_MESSAGEBOX_CF_MODAL);\
								return ERROR;\
							}\
							sprintf(msgBuf, "\n%s过长!\n", err_pos);\
							if (lu16strLen > DB_MINFO_LEN_METERADDR) {\
								GUI_MessageBox(msgBuf, "失败", GUI_MESSAGEBOX_CF_MODAL);\
								return ERROR;\
							}\
							memcpy(pInfo->db_field, l8buf, db_field_len);

#define GATEWAY_TIMENODE_CNT_LEN	1//抄表时间点个数的长度
#define GATEWAY_TIMENODE_LEN		2//每个抄表时间点的长度
#define GATEWAY_TIMENODE_MAX_CNT	24//现在ARM7最多支持多少个抄表时间点

#define HEX_TO_BCD(x) (((x)/0x0A)*0x10+((x)%0x0A))
#define BCD_TO_HEX(x) (((x)/0x10)*0x0A+((x)%0x10))

#define ASCII_TO_HEX(c) ((c >='0' && c <='9')?(c-'0'):((c>='A'&&c<='F')?(c-'A'+10):((c>='a'&&c<='f')?(c-'a'+10):0)))
#define isdigit(c)	((unsigned) ((c)-'0') < 10)

#define IS_SPACE	0x01
#define NOT_SPACE	0x00

#define IP_DELIM	'.'

#pragma pack(push)
#pragma pack(1)
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

typedef struct{//用一个字节表示分钟, 超过60则进位, 对于一般的时间相加足够
	U8 u8hour;		//时, Hex
	U8 u8minute;	//分, Hex
}time_node_str;
typedef time_node_str* time_node_ptr;

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

typedef enum {//时间字符串跳转状态
	tm_state_init = 0,		//初始状态
	tm_state_hour,			//小时状态
	tm_state_delim,			//分隔符状态
	tm_state_min,			//分钟状态
	tm_state_end_legal,		//合法终状态
	tm_state_end_illegal	//不合法终状态
} em_time_state;

typedef struct {
	U8 Method;			// 超声波热计量表20H;电子式热分配表A0H;时间通断面积法B0H;
	U8 DataSource;		//0X0A 上位机，0X0B本地抄表
	U8 Period[2];			// 分摊周期（分钟）
	U8 LogReportTime; //0x00 :打开 ，0x01:不打开
	U8 LogOpenType; 		// 0X00: 打开 ，0X01 ：不打开
	U8 APN;             // GPRS网络APN接入点选择.
	U8 IPAddr[4];			//0是低位IP  3是高位IP 16进制下发
	U8 HostPor[2];  // 主站端口号高8位  (注：端口号高低字节顺序和0x90、0x91读取时顺序相反！)
}gprs_param_str;
typedef gprs_param_str* gprs_param_ptr;

typedef struct {
	U8 Ip[4];//服务器IP, 倒序
	U8 Port[2];//服务器端口
	U8 Address[6];// 集中器地址
	U8 HostAddress[6];// 主站地址
	U8 SoftVer[2];/*例如：0x0232代表2.32*/
	U8 HardwareVer[2];/*例如：0x0232代表2.32*/
	U8 u8APN;  //GPRS模块APN接入点，0-cmnet公网，1-联通M2M物联网，2-威海热电。
	U8 u8Reserved[6];//保留
} gateway_params_str;
typedef gateway_params_str* gateway_params_ptr;

typedef struct {//消息头部结构体
	U8 prefix[GATEWAY_PREFIX_CNT];		//前导符0xFB
	U8 start;							//开始符0x7B
	U8 protoVer;						//协议版本号
	U8 SourceAddr[GATEWAY_SADD_LEN];	//源地址, 发送方地址
	U8 DestAddr[GATEWAY_OADD_LEN];		//目的地址, 接收方地址
	U8 MsgID;							//消息序列
	U16 bodyLen;						//消息体长度
	U8 MsgType;							//消息类型
	sys_time_str sysTime;				//TS时间标签
	U8 HeadCheck;						//消息头校验
} protocol_head_str;
typedef protocol_head_str* protocol_type_ptr;

typedef struct {//集中器协议消息体结构
	protocol_head_str head_str;	//消息头部
	U8 *pMsgBody;				//消息体指针
	U8	bodyChk;				//消息体校验
} gateway_protocol_str;
typedef gateway_protocol_str* gateway_protocol_ptr;

typedef struct {
	U8 succeed;//是否有后继帧, 0-没有, 1-有, 10-异常
	U8 seq;		//本包序号
	U8 meterType;//仪表类型, 固定为0x20
	U8 rowCnt;//本帧包含的表地址数量
} base_info_head_str;
typedef base_info_head_str* base_info_head_ptr;

typedef struct {//补抄相关参数结构体
	U8	mReadCnt;	//热表补抄次数
	U16 mReadIntv;	//热表补抄时间间隔ms
	U8	vReadCnt;	//阀控补抄次数
	U16 vReadIntv;	//阀控补抄时间间隔ms
}reread_param_str;
typedef reread_param_str* reread_param_ptr;

typedef struct {
	U32	DailyHeat;				//结算日热量
	U8	DailyHeatUnit;			//一般为kWh, 0x05
	U32	CurrentHeat;			//当前热量
	U8	CurrentHeatUnit;		//一般为kWh, 0x05
	U32	HeatPower;				//热功率
	U8	HeatPowerUnit;			//一般为kW, 0x17
	U32	Flow;					//流量流速
	U8	FlowUnit;				//一般为m3/h, 0x35
	U32	AccumulateFlow;			//累积流量
	U8	AccumulateFlowUnit;		//一般为m3, 0x2C
	U8	WaterInTemp[3];			//供水温度
	U8 	WaterOutTemp[3];		//回水温度
	U8 	AccumulateWorkTime[3];	//累计工作时间
	U8	RealTime[7];		    //实时时间
	U16	ST;					    //状态ST
}CJ188_Format;
typedef CJ188_Format* CJ188_Format_ptr;

#define CJ188_HEAT_METER
typedef struct {//温控计量一体化, 历史数据格式
        U16 meterId;		//计量点号
        U8	meterType;		//仪表类型
        U8	meterAddr[7];	//仪表地址
        U8	buildId;		//楼号
        U8	unitId;			//单元号
        U16	roomId;			//房间号
        U8	mSecondBCD;		//抄热表秒BCD
        U8	mMinuteBCD;		//抄热表分钟BCD
        U8	mHourBCD;		//抄热表小时BCD
        U8	meterDataLen;	//热表数据长度
#ifdef CJ188_HEAT_METER
        CJ188_Format	MeterData;		//热表数据指针
#endif // CJ188_HEAT_METER
        U8	vSecondBCD;		//抄阀秒BCD
        U8	vMinuteBCD;		//抄阀分钟BCD
        U8	vHourBCD;		//抄阀小时BCD
        U8	RoomTempBCD[3];	//室内温度
        U8	vOpen;			//阀门开度
        U8	vState;			//阀门状态
        U8	vReserve;		//保留
} tempControl_messure_hisdata_str;
typedef tempControl_messure_hisdata_str* tempControl_messure_hisdata_ptr;

typedef struct {//温控计量一体化和通断时间面积法的历史数据头部结构
    U8 succeed;//是否有后继帧, 0-没有, 1-有, 10-异常
    U8 seq;		//本包序号
    U8 u8second;	//存储时的秒
    U8 u8minute;	//存储时的分
    U8 u8hour;		//存储时的时
    U8 u8day;		//存储时的日
    U8 u8month;		//存储时的月
    U8 u8year;		//存储时的年
} hisdata_head_str;
typedef hisdata_head_str*  hisdata_head_ptr;


#pragma pack(pop)

static const U8 spaces[] = { ' ', '\t', '\n', '\r', '\0' };

//读取系统时间
U8 readSysTime(sys_time_ptr pTime)
{
	struct tm NowTime;
	time_t timep;
	time(&timep);
        localtime_r(&timep, &NowTime);

	pTime->u8year	= HEX_TO_BCD(NowTime.tm_year-100);
	pTime->u8month  = HEX_TO_BCD(NowTime.tm_mon+1);
	pTime->u8day    = HEX_TO_BCD(NowTime.tm_mday);
	pTime->u8hour	= HEX_TO_BCD(NowTime.tm_hour);
	pTime->u8minute = HEX_TO_BCD(NowTime.tm_min);
	pTime->u8second = HEX_TO_BCD(NowTime.tm_sec);

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
    memset(pProtoInfo->reserved, 0, PROTO_LEN_RSV);
}


void printBuf(U8* buf, U16 bufSize, const char* file, const char* func, U32 line)
{
	U16 i = 0;
	printf("[%s][%s][%d]buf: ", file, func, line);
	for (i = 0;i < bufSize;i++)
		printf("%02X ", buf[i]);
	printf("\n");
}

/*
**	创建集中器发送帧.
**	@buf:	发送缓冲区
**	@bufSize:	发送缓冲区长度
**	@pProto:	发送帧额外信息
*/
static void createFrame(U8 *buf, U16 *bufSize, gateway_protocol_ptr pProto)
{
	U8 *pTemp = buf;

	memcpy(pTemp, pProto->head_str.prefix, GATEWAY_PREFIX_CNT);//前导符
	pTemp += GATEWAY_PREFIX_CNT;
	*pTemp++ = pProto->head_str.start;//起始符
	*pTemp++ = pProto->head_str.protoVer;//版本号

	memcpy(pTemp, pProto->head_str.SourceAddr, GATEWAY_SADD_LEN);//源地址
	pTemp += GATEWAY_SADD_LEN;
	memcpy(pTemp, pProto->head_str.DestAddr, GATEWAY_OADD_LEN);//目标地址
	pTemp += GATEWAY_OADD_LEN;

	*pTemp++ = pProto->head_str.MsgID;//MID 消息序列
	memcpy(pTemp, (U8*)(&pProto->head_str.bodyLen), sizeof(U16));//消息体长度
	pTemp += sizeof(U16);
	*pTemp++ = pProto->head_str.MsgType;//消息类型

	memcpy(pTemp, (U8*)(&(pProto->head_str.sysTime)), sizeof(sys_time_str));//时间
	pTemp += sizeof(sys_time_str);

	*pTemp++ = pProto->head_str.HeadCheck;//消息头校验

	memcpy(pTemp, pProto->pMsgBody, pProto->head_str.bodyLen);//消息体
	pTemp += pProto->head_str.bodyLen;
	*pTemp++ = pProto->bodyChk;//消息体校验
	memset(pTemp, GATEWAY_SUFIX, GATEWAY_SUFIX_CNT);//结束符
	*bufSize = sizeof(protocol_head_str) + pProto->head_str.bodyLen + GATEWAY_EC_LEN + GATEWAY_SUFIX_CNT;
}

U8 proto_assembleFrame(U8* buf, U16* bufSize, U8* gatewayId, \
	U8 MsgIndex, U8 MsgType, U16 bodyLen, U8* pMsgBody)
{
	gateway_protocol_str protoStr = { 0 };

	memset(protoStr.head_str.prefix, GATEWAY_PREFIX, GATEWAY_PREFIX_CNT);//前导符
	memset(&(protoStr.head_str.start), GATEWAY_START, GATEWAY_START_CNT);//开始符
	memset(&(protoStr.head_str.protoVer), GATEWAY_PROTOCOL_VER, 1);//协议版本号

	protoStr.head_str.SourceAddr[0] = 0x01;
	protoStr.head_str.SourceAddr[1] = 0x00;
	protoStr.head_str.SourceAddr[2] = 0x00;
	protoStr.head_str.SourceAddr[3] = 0x00;
	protoStr.head_str.SourceAddr[4] = 0x00;
	protoStr.head_str.SourceAddr[5] = 0x00;
	memcpy(protoStr.head_str.DestAddr, gatewayId, GATEWAY_OADD_LEN);
	protoStr.head_str.MsgID = MsgIndex;
	protoStr.head_str.bodyLen = bodyLen;
	protoStr.head_str.MsgType = MsgType;
	readSysTime(&(protoStr.head_str.sysTime));
	protoStr.head_str.HeadCheck = countCheck(&(protoStr.head_str.protoVer), \
		(sizeof(protocol_head_str) - GATEWAY_PREFIX_CNT - GATEWAY_START_CNT));
	protoStr.pMsgBody = pMsgBody;
	protoStr.bodyChk = countCheck(protoStr.pMsgBody, bodyLen);
	createFrame(buf, bufSize, &protoStr);
	return NO_ERR;
}

/*
**	分析集中器的返回帧长度及校验的合法性.
**	@buf:		返回帧
**	@bufSize:	返回帧的长度
**	return:		合法返回NO_ERR; 非法返回ERROR, 下同
*/
U8 protoA_retFrameLen(U8* buf, U16 bufSize)
{
	U8	lu8data = 0;
	U16	lu16Length = 0;
	protocol_head_str protoHeadStr = { 0 };

	//检查消息头部长度
	if (bufSize < sizeof(protocol_head_str)) {
		printf("proto head length err\n");
		return ERROR;
	}

	memcpy((U8*)&(protoHeadStr), buf, sizeof(protocol_head_str));

	//检查整个buf的长度
	lu16Length = sizeof(protocol_head_str) + protoHeadStr.bodyLen + GATEWAY_EC_LEN + GATEWAY_SUFIX_CNT;
	if (bufSize != lu16Length) {
		printf("buf length err\n");
		return ERROR;
	}

	//检查前导符, 开始符和协议版本
	if (protoHeadStr.prefix[0] != GATEWAY_PREFIX || \
		protoHeadStr.prefix[1] != GATEWAY_PREFIX || \
		protoHeadStr.start != GATEWAY_START || \
		protoHeadStr.protoVer != GATEWAY_PROTOCOL_VER) {
		printf("prefix: %02X %02X\n", protoHeadStr.prefix[0], protoHeadStr.prefix[1]);
		printf("start: %02X\n", protoHeadStr.start);
		printf("protoVer: %02X\n", protoHeadStr.protoVer);
		return ERROR;
	}

	//检查结束符
	if (buf[bufSize - 1] != GATEWAY_SUFIX || \
		buf[bufSize - 2] != GATEWAY_SUFIX) {
		printf("suffix: %02X %02X\n", buf[bufSize - 1], buf[bufSize - 1]);
		return ERROR;
	}

	//检查头部校验
	lu16Length = (&(protoHeadStr.HeadCheck) - &(protoHeadStr.protoVer));
	lu8data = countCheck(&(protoHeadStr.protoVer), lu16Length);
	if (lu8data != protoHeadStr.HeadCheck) {
		printf("lu8data: %02X, HeadCheck: %02X\n", lu8data, protoHeadStr.HeadCheck);
		return ERROR;
	}

	//检查消息体校验
	lu8data = countCheck(buf + sizeof(protocol_head_str), protoHeadStr.bodyLen);
	if (buf[bufSize - 3] != lu8data) {
		printf("lu8data: %02X, EC: %02X\n", lu8data, buf[bufSize - 3]);
		return ERROR;
	}

	return NO_ERR;
}

/*
**	分析集中器的返回帧.
**	@buf:		返回帧
**	@bufSize:	返回帧的长度
**	@msgType:	集中器返回帧的消息类型,
**				而不是服务器下发帧的类型
**	@seq:		下发帧的序列号
*/
U8 protoA_retFrame(U8* buf, U16 bufSize, U8 msgType, U8 seq)
{
	U8 data;

	//检查消息合法性
	if (protoA_retFrameLen(buf, bufSize) == ERROR)
		return ERROR;

	//成功状态
	data = buf[GATEWAY_STATCODE_OFFSET];
	if (GATEWAY_ASW_CODE_SUC != data)
		return ERROR;

	//消息类型
	data = buf[GATEWAY_ASWCODE_OFFSET];
	if (data != msgType)
		return ERROR;

	return NO_ERR;
}

/*
**	设置集中器时间.
**	@gatewatId: 集中器号
**	@idLen:		集中器号的长度
**	@buf:		发送帧
**	@bufSize:	发送帧的长度
*/
U8 protoW_setTime(U8* buf, U16* bufSize, U8 *gatewayId)
{
	sys_time_str sysTime;
	readSysTime(&sysTime);
	return proto_assembleFrame(buf, bufSize, gatewayId, 0x00, \
		GAT_MT_SVR_TIME_SET, sizeof(sys_time_str), (U8*)&sysTime);
}

/*
**	广播读集中器号.
**	@buf:		发送帧
**	@bufSize:	发送帧的长度
*/
U8 protoR_radioReadId(U8* buf, U16* bufSize)
{
	U8 gatewayId[GATEWAY_OADD_LEN] = { 0 };
	return protoW_setTime(buf, bufSize, gatewayId);
}

/*
**	从集中器返回帧, 读取集中器号.
**	@gatewatId: 集中器号, 从消息中的反序, 变为正序
**	@idLen:		集中器号的长度
**	@buf:		返回帧
**	@bufSize:	返回帧的长度
*/
U8 protoA_radioReadId(U8 *gatewayId, U8 idLen, U8* buf, U16 bufSize)
{
	//检查消息合法性
	if (protoA_retFrameLen(buf, bufSize) == ERROR)
		return ERROR;
	buf += GATEWAY_RETID_OFFSET;
	memcpy(gatewayId, buf, GATEWAY_OADD_LEN);
	inverseArray(gatewayId, GATEWAY_OADD_LEN);

	return NO_ERR;
}

/*
**	下发表地址给集中器.
**	@buf:		发送缓存
**	@bufSize:	发送缓存长度
**	@gatewayId:	集中器号(已翻转的bin格式)
**	@pBodyHead:	消息头的指针
**	@pProtoInfo:消息体的指针
*/
U8 protoW_issueMinfo(U8* buf, U16* bufSize, U8* gatewayId, \
	meterinfo_bodyHead_ptr pBodyHead, meter_row_ptr	pProtoInfo)
{
	U8 bufMsgBody[GATEWAY_FRAME_MAX_LEN] = {0};//用于暂存消息体
	U16 bodyLen = 0;

	bodyLen = (sizeof(meter_row_str)*(pBodyHead->thisRows) + sizeof(meterinfo_bodyHead_str));
	memcpy(bufMsgBody, (U8*)pBodyHead, sizeof(meterinfo_bodyHead_str));//复制消息头
	memcpy(bufMsgBody + sizeof(meterinfo_bodyHead_str), (U8*)pProtoInfo, \
		pBodyHead->thisRows * sizeof(meter_row_str));//复制消息体

	return proto_assembleFrame(buf, bufSize, gatewayId, pBodyHead->seq, \
		GAT_MT_SVR_SEND_MINFO, bodyLen, bufMsgBody);
}

/*
**	单独修改一个计量点的表地址.
**	@buf:		发送缓冲区
**	@bufSize:	发送缓冲区长度
**	@gatewayId:	已反序的集中器ID
**	@pProto:	发送帧额外信息
*/
U8 protoW_modifyOneMinfo(U8* buf, U16* bufSize, U8* gatewayId, meter_row_ptr pProtoInfo)
{
	return proto_assembleFrame(buf, bufSize, gatewayId, 0x00, \
		GAT_MT_SVR_MODIFY_SINFO, sizeof(meter_row_str), (U8*)pProtoInfo);
}

/*
**	下发抄表时间点.
**	@buf:		发送缓冲区
**	@bufSize:	发送缓冲区长度
**	@gatewayId:	已反序的集中器ID
**	@timeCnt:	时间点个数
**	@pTimeNode:	时间点指针
*/
U8 protoW_tmNode(U8* buf, U16* bufSize, U8* gatewayId, U8 timeCnt, time_node_ptr pTimeNode)
{
	U8	bodyBuf[GATEWAY_FRAME_MAX_LEN] = { 0 };
	U16 bodyLen = 0;

	bodyLen = GATEWAY_TIMENODE_CNT_LEN + timeCnt*sizeof(time_node_str);
	memcpy(bodyBuf, &timeCnt, GATEWAY_TIMENODE_CNT_LEN);
	memcpy(bodyBuf + GATEWAY_TIMENODE_CNT_LEN, (U8*)pTimeNode, timeCnt * sizeof(time_node_str));
	return proto_assembleFrame(buf, bufSize, gatewayId, 0x00, \
		GAT_MT_SVR_TIME_POINT, bodyLen, bodyBuf);
}

U8 protoW_modifyGatewayId(U8* buf, U16* bufSize, U8* lu8originalId, U8* lu8targetId)
{
	U8	bodyBuf[GATEWAY_SADD_LEN + GATEWAY_OADD_LEN] = { 0 };
	U16 bodyLen = GATEWAY_SADD_LEN + GATEWAY_OADD_LEN;

	bodyBuf[0] = 0x01;
	bodyBuf[1] = 0x00;
	bodyBuf[2] = 0x00;
	bodyBuf[3] = 0x00;
	bodyBuf[4] = 0x00;
	bodyBuf[5] = 0x00;
	memcpy(bodyBuf + GATEWAY_SADD_LEN, lu8targetId, GATEWAY_OADD_LEN);
	return proto_assembleFrame(buf, bufSize, lu8originalId, 0x00, \
		GAT_MT_SVR_MID, bodyLen, bodyBuf);
}

U8 protoR_GPRSParam(U8* buf, U16* bufSize, U8* gatewayId)
{
	return proto_assembleFrame(buf, bufSize, gatewayId, 0x00, \
		GAT_MT_SVR_RD_CONFIG, 0, NULL);
}

U8 protoA_GPRSParam(U8* buf, U16 bufSize, gateway_params_ptr pParam)
{
	U16 msgLen = 0;

	//检查消息合法性
	if (protoA_retFrameLen(buf, bufSize) == ERROR)
		return ERROR;
	memcpy((U8*)&msgLen, buf + GATEWAY_BODYLEN_OFFSET, GATEWAY_MSGL_LEN);
	memcpy((U8*)pParam, buf + GATEWAY_BODY_OFFSET, msgLen);
	return NO_ERR;
}

U8 protoW_modifyGPRS(U8* buf, U16* bufSize, U8* gatewayId, gprs_param_ptr pGPRSParam)
{
	return proto_assembleFrame(buf, bufSize, gatewayId, 0x00, \
		GAT_MT_SVR_CHIP, sizeof(gprs_param_str), (U8*)pGPRSParam);
}

U8 protoX_reboot(U8* buf, U16* bufSize, U8* gatewayId)
{
	return proto_assembleFrame(buf, bufSize, gatewayId, 0x00, \
		GAT_MT_SVR_REBOOT, 0, NULL);
}

U8 protoW_rereadParam(U8* buf, U16* bufSize, U8* gatewayId, reread_param_ptr pParam)
{
	return proto_assembleFrame(buf, bufSize, gatewayId, 0x00, \
		GAT_MT_SVR_REREAD, sizeof(reread_param_str), (U8*)pParam);
}

U8 protoX_readMeterImmd(U8* buf, U16* bufSize, U8* gatewayId)
{
	return proto_assembleFrame(buf, bufSize, gatewayId, 0x00, \
		GAT_MT_SVR_CPY_IMMDT, 0, NULL);
}

U8 protoR_readBaseInfo(U8* buf, U16* bufSize, U8* gatewayId)
{
	return proto_assembleFrame(buf, bufSize, gatewayId, 0x00, \
		GAT_MT_SVR_RD_ALLINFO, 0, NULL);
}

U8 protoA_readBaseInfo(U8* buf, U16 bufSize, U16* infoCnt, base_info_head_ptr pBodyHead, meter_row_ptr pInfo)
{
	protocol_head_str frameStr = { 0 };
	U8* pMsgBody = NULL;
	U8 i = 0;

	//检查消息合法性
	if (protoA_retFrameLen(buf, bufSize) == ERROR)
		return ERROR;
	memcpy((U8*)&frameStr, buf, sizeof(protocol_head_str));//复制消息头
	pMsgBody = buf + sizeof(protocol_head_str);//指向消息体
	memcpy((U8*)pBodyHead, pMsgBody, sizeof(base_info_head_str));
	if (*infoCnt < pBodyHead->rowCnt)
		return ERROR;
	*infoCnt = pBodyHead->rowCnt;
	pMsgBody += sizeof(base_info_head_str);//指向消息体
	for (i = 0; i < *infoCnt ; i++)
		memcpy((U8*)(pInfo+i), pMsgBody+i* sizeof(meter_row_str), sizeof(meter_row_str));
	return NO_ERR;
}

U8 protoR_readMultiInfo(U8* buf, U16* bufSize, U8* gatewayId, U8* seq)
{
	return proto_assembleFrame(buf, bufSize, gatewayId, 0x00, \
		GAT_MT_SVR_1OF_MFRM, 1, seq);
}

U8 protoR_readHisData(U8* buf, U16* bufSize, U8* gatewayId, U8* timeNode)
{
	sys_time_str sysTime;
	readSysTime(&sysTime);
	return proto_assembleFrame(buf, bufSize, gatewayId, 0x00, \
		GAT_MT_SVR_HISDATA, sizeof(sys_time_str), \
		(timeNode == NULL ? (U8*)&sysTime : timeNode));
}

U8 protoA_hisData(U8* buf, U16 bufSize, U16* hisDataCnt, hisdata_head_ptr pBodyHead, tempControl_messure_hisdata_ptr pHisData)
{
	protocol_head_str protoFrameStr = { 0 };
	U8* pMsgBody = NULL;
	U8 i = 0;
	U16 rowCnt = 0;

	//检查消息合法性
	if (protoA_retFrameLen(buf, bufSize) == ERROR)
		return ERROR;
	memcpy((U8*)&protoFrameStr, buf, sizeof(protocol_head_str));//复制消息头
	pMsgBody = buf + sizeof(protocol_head_str);//指向消息体
	memcpy((U8*)pBodyHead, pMsgBody, sizeof(hisdata_head_str));
	rowCnt = (protoFrameStr.bodyLen - sizeof(hisdata_head_str)) / sizeof(tempControl_messure_hisdata_str);
	if (*hisDataCnt < rowCnt)
		return ERROR;
	*hisDataCnt = rowCnt;
	pMsgBody += sizeof(hisdata_head_str);//指向消息体
	for (i = 0; i < rowCnt; i++) {
		memcpy((pHisData + i), (tempControl_messure_hisdata_ptr)pMsgBody + i, \
			sizeof(tempControl_messure_hisdata_str));
	}
	return NO_ERR;
}

U8 protoA_hisDataSuc(tempControl_messure_hisdata_ptr pHisData)
{
	if (pHisData->vState==0xEE) {
		return ERROR;
	}
	return NO_ERR;
}

U8 protoX_readOneMeter(U8* buf, U16* bufSize, U8* gatewayId, U16* pMeterId)
{
	return proto_assembleFrame(buf, bufSize, gatewayId, 0x00, \
		GAT_MT_SVR_CP_1_METER, PROTO_LEN_ROWID, (U8*)pMeterId);
}

U8 protoA_readOneMeter(U8* buf, U16 bufSize, tempControl_messure_hisdata_ptr pHisData)
{
	//检查消息合法性
	if (protoA_retFrameLen(buf, bufSize) == ERROR)
		return ERROR;

	memcpy((U8*)pHisData, buf + sizeof(protocol_head_str), sizeof(tempControl_messure_hisdata_str));
	return NO_ERR;
}

int issueMinfo(void)
{
	int row = 0, i = 0;
	U16 bufSize = 0;
	U8 buf[1024] = { 0 };
	U8 lu8gatewayId[GATEWAY_OADD_LEN] = { 0x34, 0x12, 0x00, 0x00, 0x00, 0x00 };

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
	
	protoW_issueMinfo(buf, &bufSize, lu8gatewayId, &bodyHeadStr, pProtoInfo);
	printf("buf: ");
	for(i=0;i<bufSize;i++) {
		printf("%02X ", buf[i]);
	}
	printf("\n");

	protoW_modifyOneMinfo(buf, &bufSize, lu8gatewayId, pProtoInfo);
	printf("buf: ");
	for(i=0;i<bufSize;i++) {
		printf("%02X ", buf[i]);
	}
	printf("\n");
    return 0;
}

static const char* stateHash[] = {	
	"init",
	"hour",
	"delim",	
	"minute",
	"legal",
	"illegal"
	};

U8 timeLegal(U8* timeStr, U16 strLen, em_time_state* pState)
{
	U16 i = 0, digitalLen = 0;
	U8 data, value[3] = { 0 };
	em_time_state state = tm_state_init;

	for (i = 0; i < strLen; i++) {
		data = timeStr[i];
		//printf("data: %c\n", data);
		switch (state) {
		case tm_state_init:
			if (isdigit(data)) {
				value[digitalLen] = data;
				digitalLen++;
				state = tm_state_hour;
			} else {
				state = tm_state_end_illegal;
				goto result;
			}
			break;
		case tm_state_hour:
			if (isdigit(data)) {
				if (digitalLen == 2) {
					state = tm_state_end_illegal;
					goto result;
				}
				value[digitalLen] = data;
				if (atoi(value) > 23) {
					state = tm_state_end_illegal;
					goto result;
				}
				digitalLen++;
			} else if (isDelim(data)) {
				digitalLen = 0;
				memset(value, 0, sizeof(value));
				state = tm_state_delim;
			} else {
				state = tm_state_end_illegal;
				goto result;
			}
			break;
		case tm_state_delim:
			if (isdigit(data)) {
				value[digitalLen] = data;
				digitalLen++;
				state = tm_state_min;
			} else {
				state = tm_state_end_illegal;
				goto result;
			}
			break;
		case tm_state_min:
			if (isdigit(data)) {
				if (digitalLen == 2) {
					state = tm_state_end_illegal;
					goto result;
				}
				value[digitalLen] = data;
				if (atoi(value) > 59) {
					state = tm_state_end_illegal;
					goto result;
				}
				digitalLen++;
			} else {
				state = tm_state_end_illegal;
				goto result;
			}
			break;
		default:
			break;
		}
	}


result:
	//printf("reach result\n");
	*pState = state;
	if (state == tm_state_hour || state == tm_state_min)
		return NO_ERR;
	else
		return ERROR;
}

/*
**	时间相加(时:分 + 时:分).
**	@pT1:	第一个时间
**	@pT2:	第二个时间
**	@pRes:	结果
*/
U8 addTime(time_node_ptr pT1, time_node_ptr pT2, time_node_ptr pRes)
{
	U8 carry = 0;
	
	if (pT1->u8hour >24 || pT2->u8hour>24 || pT1->u8minute>60 || pT2->u8minute>60 )
		return ERROR;

	pRes->u8hour = pT1->u8hour + pT2->u8hour;
	pRes->u8minute = pT1->u8minute + pT2->u8minute;
	carry = pRes->u8minute / 60;
	pRes->u8hour += carry;
	pRes->u8hour %= 24;
	pRes->u8minute = pRes->u8minute % 60;
	return NO_ERR;
}

/*
**	把字符形式的时间转化为二进制形式的时间.
**	@timeStr:	时间字符串
**	@timeSize:	字符串长度
**	@pRes:		结果
*/
U8 timeStrToBin(U8* timeStr, U8 timeSize, time_node_ptr pRes)
{
	U8* p = timeStr;
	U8 iRet = NO_ERR;
	em_time_state state = tm_state_init;
	
	//printf("time: %s, timeSize: %d\n", timeStr, timeSize);
	iRet = timeLegal(timeStr, timeSize, &state);
	//printf("state: %d\n", state);
	//printf("end state is: %s\n", stateHash[state]);
	if ( iRet== ERROR) {
		printf("input illegal, return ERROR\n");
		return ERROR;
	}
	
	if (state == tm_state_hour) {//如果状态停在小时状态, 则字符串必全为数字
		pRes->u8hour = atoi(timeStr);
		pRes->u8minute = 0;
	} else if (state==tm_state_min) {
		//先把分隔符修改为字符串的结束符'\0'
		while (*p != TIME_DELIM) p++;
		*p = 0;
		//求小时
		pRes->u8hour = atoi(timeStr);
		p++;//由于p指向的位置为'\0', 所以向后移动一位才能到达分钟
		pRes->u8minute = atoi(p);//p指向了分钟位置, 而timeStr的结束符也是p的结束符
	}
	
	return NO_ERR;
}

U8 calcTimeNode(U8* buf, U16 bufSize, U8* startTime, U8 timeCnt, time_node_ptr pTimeNodes)
{
	U16 i = 0;
	U8	delim = ';';
	em_time_state state = tm_state_init;
	time_node_str timeInterval = { 0,0 };
	time_node_str startTimeStr = { 0,0 };
	U8 hour = 0;
	U8 lu8Time[10] = { 0 };

	if (timeCnt > MAX_TIME_NODE)
		return ERROR;
	//24个小时不能被下列数整除
	if (timeCnt == 7 || timeCnt == 11 || timeCnt == 13 || timeCnt == 14 || timeCnt == 17 || timeCnt == 19 || timeCnt == 21 || timeCnt == 23)
		return ERROR;
	if (timeLegal(startTime, strlen(startTime), &state) == ERROR)
		return ERROR;
	timeStrToBin(startTime, strlen(startTime), &startTimeStr);
	hour = 24 / timeCnt;
	for (i = 0; i < timeCnt ; i++) {
		timeInterval.u8hour = i*hour;
		addTime(&startTimeStr, &timeInterval, &(pTimeNodes[i]));
		printf("pTimeNodes[%d], hour: %d, minute: %d\n", i, pTimeNodes[i].u8hour, pTimeNodes[i].u8minute);
		sprintf(lu8Time, "%02d%c%02d;", pTimeNodes[i].u8hour, TIME_DELIM, pTimeNodes[i].u8minute);
		strcat(buf, lu8Time);
	}
	return NO_ERR;
}

U8 timeStrToBCD(time_node_ptr pTimeStr)
{
	pTimeStr->u8minute = HEX_TO_BCD(pTimeStr->u8minute);
	pTimeStr->u8hour = HEX_TO_BCD(pTimeStr->u8hour);
	return NO_ERR;
}

U8 strToTimeNode(U8* buf, U16 bufSize, U8* pTimeNode, U16* timeCnt)
{
	U8 lu8time[10] = { 0 };
	U8* p = buf;
	U16 i = 0;

	if (*p == TIME_NODE_DELIM)
		p++;

	for (*timeCnt=0, i = 0; *p !='\0' ; p++) {
		if (*p!=TIME_NODE_DELIM) {
			lu8time[i] = *p;
			i++;
		} else {
			if (timeStrToBin(lu8time, strlen(lu8time), &(((time_node_ptr)pTimeNode)[*timeCnt])) == ERROR)
				return ERROR;
			if (timeStrToBCD(&((time_node_ptr)pTimeNode)[*timeCnt]) == ERROR)
				return ERROR;
			*timeCnt += 1;
			i = 0;
			memset(lu8time, 0, 10);
		}
	}
	return NO_ERR;
}

U8 strIpToHex(U8* svrStr, gprs_param_ptr pGPRSParam)
{
	U16 value = 0;
	U8	j = 3, delimCnt = 0;
	U8* p = svrStr;
	U8* q = svrStr;

	while ( *p != '\0') {
		if (*p == IP_DELIM) {
			*p = '\0';
			value = atoi((const char*)q);
			printf("value: %d\n", value);
			if (value > 255) {
				return ERROR;
			} else {
				pGPRSParam->IPAddr[j] = (U8)value;
				j--;
			}
			delimCnt++;
			if (delimCnt>3) {//多于3个分隔符, 返回错误
				return ERROR;
			}
			q = p+1;
		}
		p++;
	}
	if(delimCnt == 3) {
		value = atoi((const char*)q);
		printf("value: %d\n", value);
		if (value > 255) {
			return ERROR;
		} else {
			pGPRSParam->IPAddr[j] = (U8)value;
		}
	}
	return NO_ERR;
}

U8 IpLegal(U8* buf, U16 bufSize)
{
	U8* p = buf;
	U16 i = 0, j = 0;
	U8	delimCnt = 0;
	U8	value[10] = { 0 };

	for (i = 0; i < bufSize ; p++, i++) {
		if (*p == IP_DELIM) {
			delimCnt++;
			if (atoi((const char*)value) > 255 || (delimCnt==1?(atoi((const char*)value)==0):1)) {
				return ERROR;
			}
			j = 0;
			memset(value, 0, 10);
		} else if (isdigit(*p)) {
			value[j] = *p;
			j++;
		} else{
			return ERROR;
		}
	}
	if (delimCnt != 3)
		return ERROR;
	if (atoi((const char*)value) > 255) {
				return ERROR;
	}
	return NO_ERR;
}

int main(int argc, char** argv)
{
	tempControl_messure_hisdata_str hisDataStr[30] = { {0} };
	hisdata_head_str BodyHeadStr;
	U8 lu8data = 0;
	U8 buf[2048] = { 0 };
	U16 bufSize = 0, i = 0;
	U16 hisDataCnt = sizeof(hisDataStr) / sizeof(tempControl_messure_hisdata_str);
	U8 lu8gatewayId[] = { 0x17, 0x90, 0x00, 0x00, 0x00, 0x00 };
	U8 readBuf[] = { 0xFB, 0xFB, 0x7B, 0x03, 0x17, 0x90, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x14, 0x40, 0x00, 0x07, 0x15, 0x29, 0x17, 0x25, 0x07, 0x16, 0x63, 0x04, 0x00, 0x20, 0x62, 0x49, 0x16, 0x21, 0x00, 0x11, 0x11, 0x04, 0x02, 0x50, 0x04, 0x2B, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x16, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x17, 0x00, 0x00, 0x00, 0x00, 0x35, 0x61, 0x00, 0x00, 0x00, 0x2C, 0x73, 0x25, 0x00, 0x27, 0x26, 0x00, 0x61, 0x66, 0x01, 0x17, 0x10, 0x18, 0x23, 0x07, 0x16, 0x20, 0x00, 0x00, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0x7A, 0xFD, 0xFD };
	U16 readBufSize = sizeof(readBuf);
	U16 MeterId = 4;


	protoX_readOneMeter(buf, &bufSize, lu8gatewayId, &MeterId);
	printBuf(buf, bufSize, FILE_LINE);
	lu8data = protoA_readOneMeter(readBuf, readBufSize, &(hisDataStr[0]));
	printf("lu8data: %d\n", lu8data);
	printBuf((U8*)&(hisDataStr[0]), sizeof(tempControl_messure_hisdata_str), FILE_LINE);

	
	
	printf("AA: %02X\n", strtol("AA", NULL, 16));

	return 0;
}
