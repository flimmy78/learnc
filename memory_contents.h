/*
  *******************************************************************************
  * @file    memory_contents.h
  * @author  zjjin
  * @version V0.0.0
  * @date    07-30-2016
  * @brief   
  ******************************************************************************
  * @attention
  *    热表需要存储到ROM中参数及数据的定义。
  *
  ******************************************************************************
*/

#ifndef _MEMORY_CONTENTS_H_
#define _MEMORY_CONTENTS_H_

#include "cpu.h"
#include "public.h"


//各个参数段起始地址定义

#define ADDR_Meter_Info		0	//参数段1表基本信息段，SIZE_Bytes-128。
#define ADDR_Current_Info	128	//参数段2表计当前数据信息段，SIZE_Bytes-128。
#define ADDR_Meter_Para		256	//参数段2表计设置参数信息段，SIZE_Bytes-128。
#define ADDR_Warn_Info		384	//参数段4表计报警信息段，SIZE_Bytes-512.
#define ADDR_Calib_Coeffi_Flow	896	//参数段5，流速校正系数信息段,SIZE_Bytes-512。
#define ADDR_Calib_Coeffi_Temp	1408	//参数段6，温度校正系数信息段,SIZE_Bytes-512。
#define ADDR_HistoryData_Month	1920	//参数段7，月历史数据,SIZE_Bytes-2048。
#define ADDR_HistoryData_Year		3968	//参数段8，年历史数据,SIZE_Bytes-2048.

//使用的8K字节的E2PROM，不可超限。


#define WRITEFLAG	0xAA55	//E2PROM写标志




#pragma pack(1)  //start:结构体声明
typedef union 
{
	uint32 lu32Time;//为了清零方便, 所以定义一个uint32的Union, 否则要一个个的成员赋值, 很麻烦
	struct{		
		uint32 min	:6;  //最低6位为分
		uint32 hour	:5;
		uint32 date	:5;
		uint32 month	:4;
		uint32 year	:12; //最高12位为年
	}Time;	
	
}Time_YMDHM_U;  //时间,年月日时分。

typedef union 
{
	uint16 lu16Time;
	struct{		
		uint16		:2;  //最低2位没用
		uint16 hour	:5;
		uint16 date	:5;
		uint16 month	:4;	//最高4位为月

	}Time;	
	
}Time_MDH_U;  //月日时 时间。


typedef union 
{
	uint16 lu16Time;
	struct{		
		uint16		:6;  //最低6位没用
		uint16 hour	:5;
		uint16 date	:5;	//最高5位为日

	}Time;	
	
}Time_DH_U;  //日时 时间。




/*
  ***********************************************************************
	参数段1, 
	当前共占用32字节，
	EEPROM存储起始地址分配为 ADDR_Meter_Info,预分配128字节空间.
  ***********************************************************************
*/
typedef struct  //参数段1.
{
	uint16 WriteFlag;	//写标志，写入数据为AA55.
	uint8 Addr_primary1;  //主地址1,1字节
	uint8 Addr_primary2;  //主地址2,1字节
	uint8 Addr_sub[4];	//辅助地址，4字节。
	uint8 DeviceID[4];	//设备ID，4字节。
	uint8 SN[4];		//生产编号，4字节。
	uint8 Calibre;		//口径
	uint8 MeterType;	//表类型，高4bits表示冷量表、热量表等，后4bits表示进口表、出口表等。
	uint8 Flowrate_nominal[4];  //公称流量,3字节流量+1字节单位。	
	uint8 ManufacturerID[2];	//生产商ID。
	uint8 Ver_software[2];	//软件版本
	uint8 Ver_hardware[2];	//硬件版本		
	uint8 Ver_firmware[2];	//固件版本	
	uint8 DeliverMark[2];	//出厂标志	
	
	
}Meter_Info_T;  //表计基本身份信息类。


/*
  ***********************************************************************
	参数段2, 
	当前共占用28字节，
	EEPROM存储起始地址分配为 ADDR_Current_Info,预分配128字节空间.
  ***********************************************************************
*/
typedef struct  //2
{
	uint16 WriteFlag;	//写标志，写入数据为AA55.
	float ColdTotal;	//当前累计冷量,单位kWh
	float HeatTotal;	//当前累计热量,单位kWh
	float FlowTotal;	//累计流量,单位m3(立方米)
	uint8 WorkTimeTotal[4];	//累计工作时间，小时。
	uint8 WarnTimeTotal[4];	//累计报警时间,小时
	uint8 SysTime[6];	//系统时间
	
	
}Current_Info_T;  //表计当前实时数据信息类。



/*
  ***********************************************************************
	参数段3, 
	当前共占用24字节，
	EEPROM存储起始地址分配为 ADDR_Meter_Para,预分配128字节空间.
  ***********************************************************************
*/
typedef struct  //3
{
	uint16 WriteFlag;	//写标志，写入数据为AA55.
	uint8 Baudrate;	//波特率
	uint8 SettlementTime_day;	//日结算时间,小时
	Time_DH_U SettlementTime_month;	//月结算时间，天+小时
	Time_MDH_U SettlementTime_year;	//年结算时间，2字节，月+天+小时	

	uint16 PulseEquivalent1;	//脉冲当量1
	uint16 PulseEquivalent2;	//脉冲当量2
	uint32 PowerUpLimit;	//功率上限

	uint16 Revise_Flowrate_4mA;	//瞬时流量4mA修正值
	uint16 Revise_Flowrate_20mA;	//瞬时流量20mA修正值
	uint16 Revise_Power_4mA;		//功率4mA修正值
	uint16 Revise_Power_20mA;	//功率20mA修正值
	
}Meter_Para_T;  //表参数类。



typedef struct
{
	uint16 WriteFlag;	//写标志，写入数据为AA55.
	uint8 AreaFullFlag;	//数据区是否满标记。
	uint8 NowPosition;	//现在指向存储区的哪个位置。	
}SaveArea_Head;  //存储区头信息



typedef struct
{
	uint8 WarnCode;	//报警代码
	Time_YMDHM_U StartTime;	//开始报警时间,4字节
	uint8 DurationTime[3];	//报警持续的时间,单位小时。
}Warn_Format;  //报警信息格式



/*
  ***********************************************************************
	参数段4, 报警信息
	EEPROM存储起始地址分配为 ADDR_Warn_Info,
	最多保存32组报警信息，当前占用260字节，
	预分配512字节空间.
  ***********************************************************************
*/
typedef struct		//4
{
	SaveArea_Head HeadInfo;
	//Warn_Format ; 本结构体中只写头信息，根据头信息，索引报警信息位置，然后转到对应位置操作。
	
}Warn_Info_T;  //表报警信息类。



typedef struct	
{
	uint16 Coeffi[8];	//最多8个校准系数
	Time_YMDHM_U SetTime;	//系数设置时间,4字节
	
}Coeffi_Flow_Format;  //流量校准系数格式。



/*
  ***********************************************************************
	参数段5, 流量校正系数。
	EEPROM存储起始地址分配为 ADDR_Calib_Coeffi_Flow,
	最多保存6组历史记录，另加1组当前使用参数存储，当前占用144字节，
	预分配512字节空间.
  ***********************************************************************
*/

typedef struct		//5
{
	SaveArea_Head HeadInfo;
	Coeffi_Flow_Format CurrentCoeffi;  //第一组放当前表计使用的系数。
	//在当前使用的系数后面，是6组参数历史记录存储区，根据头信息可得到历史存储内容,
	//历史存储内容不定义在此结构体中，根据头信息索引存储位置进行处理。
	
}Calib_Coeffi_Flow_T;  //流量校准系数息类。


typedef struct	
{
	uint16 Coeffi[8];	//最多8个校准系数
	Time_YMDHM_U SetTime;	//系数设置时间,4字节
	
}Coeffi_Temp_Format;  //温度校准系数格式。



/*
  ***********************************************************************
	参数段6, 温度校正系数。
	EEPROM存储起始地址分配为 ADDR_Calib_Coeffi_Temp,
	最多保存6组历史记录，另加1组当前使用参数存储，当前占用144字节，
	预分配512字节空间.
  ***********************************************************************
*/

typedef struct		//6
{
	SaveArea_Head HeadInfo;
	Coeffi_Temp_Format CurrentCoeffi;	//第一组放当前表计使用的系数。
	//在当前使用的系数后面，是6组参数历史记录存储区，根据头信息可得到历史存储内容,
	//历史存储内容不定义在此结构体中，根据头信息索引存储位置进行处理。
	
}Calib_Coeffi_Temp_T;  //温度校准系数息类。


typedef struct	
{
	uint8 HeatTotal[4];	//当月累积热量，单位
	uint8 ColdTotal[4];	//当月累积冷量，单位
	uint8 FlowTotal[4];	//当月累积流量，单位
	uint8 Max_Power[4];	//最大功率，单位
	Time_YMDHM_U Max_Power_Time;	//最大功率出现时间
	uint8 Max_FlowRate[4];	//最大瞬时流量
	Time_YMDHM_U Max_FlowRate_Time;	//最大瞬时流量出现时间
	uint8 WorkTimeTotal[4];	//累积工作时间，有流量的时间。
	uint8 WarnTimeTotal[4];	//累积报警时间
	uint8 Max_Temp[4];	//最高温度值，单位摄氏度
	Time_YMDHM_U Max_Temp_Time;	//最高温度出现时间
	uint8 Max_TempDif[4];	//最大温差值，单位摄氏度
	Time_YMDHM_U Max_TempDif_Time;	//最高温度差出现时间
	
	uint8 Reserved[12];  //预留，本结构体最大64个字节
	
}History_Month_Format;  //月历史数据格式



/*
  ***********************************************************************
	参数段7, 月历史数据存储。
	EEPROM存储起始地址分配为 ADDR_HistoryData_Month,
	最多保存24组月历史记录，当前占用4+64*24=1540字节，
	预分配2048字节空间.
  ***********************************************************************
*/

typedef struct		//7
{
	SaveArea_Head HeadInfo;
	//History_Month_Format;  本结构体中只写头信息，根据头信息，索引报警信息位置，然后转到对应位置操作。


	
}HistoryData_Month_T;  //月历史数据



typedef struct
{
	uint8 HeatTotal[8];	//当年累积热量，单位
	uint8 ColdTotal[8];	//当年累积冷量，单位
	uint8 FlowTotal[8];	//当年累积流量，单位
	uint8 Max_Power[4];	//最大功率，单位
	Time_YMDHM_U Max_Power_Time;	//最大功率出现时间
	uint8 Max_FlowRate[4];	//最大瞬时流量
	Time_YMDHM_U Max_FlowRate_Time;	//最大瞬时流量出现时间
	uint8 WorkTimeTotal[4];	//累积工作时间，有流量的时间。
	uint8 WarnTimeTotal[4];	//累积报警时间
	uint8 Max_Temp[4];	//最高温度值，单位摄氏度
	Time_YMDHM_U Max_Temp_Time;	//最高温度出现时间
	uint8 Max_TempDif[4];	//最大温差值，单位摄氏度
	Time_YMDHM_U Max_TempDif_Time;	//最高温度差出现时间
	
						  //本结构体最大64个字节
	
}History_Year_Format;  //年历史数据


/*
  ***********************************************************************
	参数段8, 年历史数据存储。
	EEPROM存储起始地址分配为 ADDR_HistoryData_Year,
	最多保10年的年历史记录，当前占用4+64*10=644字节，
	预分配2048字节空间.
  ***********************************************************************
*/

typedef struct		//8
{
	SaveArea_Head HeadInfo;
	//History_Year_Format;本结构体中只写头信息，根据头信息，索引报警信息位置，然后转到对应位置操作。


	
}HistoryData_Year_T;  //年历史数据




#pragma pack()   //end:结构体声明










#endif  //_MEMORY_CONTENTS_H_

