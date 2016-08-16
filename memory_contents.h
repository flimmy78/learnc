/*
  *******************************************************************************
  * @file    memory_contents.h
  * @author  zjjin
  * @version V0.0.0
  * @date    07-30-2016
  * @brief   
  ******************************************************************************
  * @attention
  *    �ȱ���Ҫ�洢��ROM�в��������ݵĶ��塣
  *
  ******************************************************************************
*/

#ifndef _MEMORY_CONTENTS_H_
#define _MEMORY_CONTENTS_H_

#include "cpu.h"
#include "public.h"


//������������ʼ��ַ����

#define ADDR_Meter_Info		0	//������1�������Ϣ�Σ�SIZE_Bytes-128��
#define ADDR_Current_Info	128	//������2��Ƶ�ǰ������Ϣ�Σ�SIZE_Bytes-128��
#define ADDR_Meter_Para		256	//������2������ò�����Ϣ�Σ�SIZE_Bytes-128��
#define ADDR_Warn_Info		384	//������4��Ʊ�����Ϣ�Σ�SIZE_Bytes-512.
#define ADDR_Calib_Coeffi_Flow	896	//������5������У��ϵ����Ϣ��,SIZE_Bytes-512��
#define ADDR_Calib_Coeffi_Temp	1408	//������6���¶�У��ϵ����Ϣ��,SIZE_Bytes-512��
#define ADDR_HistoryData_Month	1920	//������7������ʷ����,SIZE_Bytes-2048��
#define ADDR_HistoryData_Year		3968	//������8������ʷ����,SIZE_Bytes-2048.

//ʹ�õ�8K�ֽڵ�E2PROM�����ɳ��ޡ�


#define WRITEFLAG	0xAA55	//E2PROMд��־




#pragma pack(1)  //start:�ṹ������
typedef union 
{
	uint32 lu32Time;//Ϊ�����㷽��, ���Զ���һ��uint32��Union, ����Ҫһ�����ĳ�Ա��ֵ, ���鷳
	struct{		
		uint32 min	:6;  //���6λΪ��
		uint32 hour	:5;
		uint32 date	:5;
		uint32 month	:4;
		uint32 year	:12; //���12λΪ��
	}Time;	
	
}Time_YMDHM_U;  //ʱ��,������ʱ�֡�

typedef union 
{
	uint16 lu16Time;
	struct{		
		uint16		:2;  //���2λû��
		uint16 hour	:5;
		uint16 date	:5;
		uint16 month	:4;	//���4λΪ��

	}Time;	
	
}Time_MDH_U;  //����ʱ ʱ�䡣


typedef union 
{
	uint16 lu16Time;
	struct{		
		uint16		:6;  //���6λû��
		uint16 hour	:5;
		uint16 date	:5;	//���5λΪ��

	}Time;	
	
}Time_DH_U;  //��ʱ ʱ�䡣




/*
  ***********************************************************************
	������1, 
	��ǰ��ռ��32�ֽڣ�
	EEPROM�洢��ʼ��ַ����Ϊ ADDR_Meter_Info,Ԥ����128�ֽڿռ�.
  ***********************************************************************
*/
typedef struct  //������1.
{
	uint16 WriteFlag;	//д��־��д������ΪAA55.
	uint8 Addr_primary1;  //����ַ1,1�ֽ�
	uint8 Addr_primary2;  //����ַ2,1�ֽ�
	uint8 Addr_sub[4];	//������ַ��4�ֽڡ�
	uint8 DeviceID[4];	//�豸ID��4�ֽڡ�
	uint8 SN[4];		//������ţ�4�ֽڡ�
	uint8 Calibre;		//�ھ�
	uint8 MeterType;	//�����ͣ���4bits��ʾ������������ȣ���4bits��ʾ���ڱ����ڱ�ȡ�
	uint8 Flowrate_nominal[4];  //��������,3�ֽ�����+1�ֽڵ�λ��	
	uint8 ManufacturerID[2];	//������ID��
	uint8 Ver_software[2];	//����汾
	uint8 Ver_hardware[2];	//Ӳ���汾		
	uint8 Ver_firmware[2];	//�̼��汾	
	uint8 DeliverMark[2];	//������־	
	
	
}Meter_Info_T;  //��ƻ��������Ϣ�ࡣ


/*
  ***********************************************************************
	������2, 
	��ǰ��ռ��28�ֽڣ�
	EEPROM�洢��ʼ��ַ����Ϊ ADDR_Current_Info,Ԥ����128�ֽڿռ�.
  ***********************************************************************
*/
typedef struct  //2
{
	uint16 WriteFlag;	//д��־��д������ΪAA55.
	float ColdTotal;	//��ǰ�ۼ�����,��λkWh
	float HeatTotal;	//��ǰ�ۼ�����,��λkWh
	float FlowTotal;	//�ۼ�����,��λm3(������)
	uint8 WorkTimeTotal[4];	//�ۼƹ���ʱ�䣬Сʱ��
	uint8 WarnTimeTotal[4];	//�ۼƱ���ʱ��,Сʱ
	uint8 SysTime[6];	//ϵͳʱ��
	
	
}Current_Info_T;  //��Ƶ�ǰʵʱ������Ϣ�ࡣ



/*
  ***********************************************************************
	������3, 
	��ǰ��ռ��24�ֽڣ�
	EEPROM�洢��ʼ��ַ����Ϊ ADDR_Meter_Para,Ԥ����128�ֽڿռ�.
  ***********************************************************************
*/
typedef struct  //3
{
	uint16 WriteFlag;	//д��־��д������ΪAA55.
	uint8 Baudrate;	//������
	uint8 SettlementTime_day;	//�ս���ʱ��,Сʱ
	Time_DH_U SettlementTime_month;	//�½���ʱ�䣬��+Сʱ
	Time_MDH_U SettlementTime_year;	//�����ʱ�䣬2�ֽڣ���+��+Сʱ	

	uint16 PulseEquivalent1;	//���嵱��1
	uint16 PulseEquivalent2;	//���嵱��2
	uint32 PowerUpLimit;	//��������

	uint16 Revise_Flowrate_4mA;	//˲ʱ����4mA����ֵ
	uint16 Revise_Flowrate_20mA;	//˲ʱ����20mA����ֵ
	uint16 Revise_Power_4mA;		//����4mA����ֵ
	uint16 Revise_Power_20mA;	//����20mA����ֵ
	
}Meter_Para_T;  //������ࡣ



typedef struct
{
	uint16 WriteFlag;	//д��־��д������ΪAA55.
	uint8 AreaFullFlag;	//�������Ƿ�����ǡ�
	uint8 NowPosition;	//����ָ��洢�����ĸ�λ�á�	
}SaveArea_Head;  //�洢��ͷ��Ϣ



typedef struct
{
	uint8 WarnCode;	//��������
	Time_YMDHM_U StartTime;	//��ʼ����ʱ��,4�ֽ�
	uint8 DurationTime[3];	//����������ʱ��,��λСʱ��
}Warn_Format;  //������Ϣ��ʽ



/*
  ***********************************************************************
	������4, ������Ϣ
	EEPROM�洢��ʼ��ַ����Ϊ ADDR_Warn_Info,
	��ౣ��32�鱨����Ϣ����ǰռ��260�ֽڣ�
	Ԥ����512�ֽڿռ�.
  ***********************************************************************
*/
typedef struct		//4
{
	SaveArea_Head HeadInfo;
	//Warn_Format ; ���ṹ����ֻдͷ��Ϣ������ͷ��Ϣ������������Ϣλ�ã�Ȼ��ת����Ӧλ�ò�����
	
}Warn_Info_T;  //������Ϣ�ࡣ



typedef struct	
{
	uint16 Coeffi[8];	//���8��У׼ϵ��
	Time_YMDHM_U SetTime;	//ϵ������ʱ��,4�ֽ�
	
}Coeffi_Flow_Format;  //����У׼ϵ����ʽ��



/*
  ***********************************************************************
	������5, ����У��ϵ����
	EEPROM�洢��ʼ��ַ����Ϊ ADDR_Calib_Coeffi_Flow,
	��ౣ��6����ʷ��¼�����1�鵱ǰʹ�ò����洢����ǰռ��144�ֽڣ�
	Ԥ����512�ֽڿռ�.
  ***********************************************************************
*/

typedef struct		//5
{
	SaveArea_Head HeadInfo;
	Coeffi_Flow_Format CurrentCoeffi;  //��һ��ŵ�ǰ���ʹ�õ�ϵ����
	//�ڵ�ǰʹ�õ�ϵ�����棬��6�������ʷ��¼�洢��������ͷ��Ϣ�ɵõ���ʷ�洢����,
	//��ʷ�洢���ݲ������ڴ˽ṹ���У�����ͷ��Ϣ�����洢λ�ý��д���
	
}Calib_Coeffi_Flow_T;  //����У׼ϵ��Ϣ�ࡣ


typedef struct	
{
	uint16 Coeffi[8];	//���8��У׼ϵ��
	Time_YMDHM_U SetTime;	//ϵ������ʱ��,4�ֽ�
	
}Coeffi_Temp_Format;  //�¶�У׼ϵ����ʽ��



/*
  ***********************************************************************
	������6, �¶�У��ϵ����
	EEPROM�洢��ʼ��ַ����Ϊ ADDR_Calib_Coeffi_Temp,
	��ౣ��6����ʷ��¼�����1�鵱ǰʹ�ò����洢����ǰռ��144�ֽڣ�
	Ԥ����512�ֽڿռ�.
  ***********************************************************************
*/

typedef struct		//6
{
	SaveArea_Head HeadInfo;
	Coeffi_Temp_Format CurrentCoeffi;	//��һ��ŵ�ǰ���ʹ�õ�ϵ����
	//�ڵ�ǰʹ�õ�ϵ�����棬��6�������ʷ��¼�洢��������ͷ��Ϣ�ɵõ���ʷ�洢����,
	//��ʷ�洢���ݲ������ڴ˽ṹ���У�����ͷ��Ϣ�����洢λ�ý��д���
	
}Calib_Coeffi_Temp_T;  //�¶�У׼ϵ��Ϣ�ࡣ


typedef struct	
{
	uint8 HeatTotal[4];	//�����ۻ���������λ
	uint8 ColdTotal[4];	//�����ۻ���������λ
	uint8 FlowTotal[4];	//�����ۻ���������λ
	uint8 Max_Power[4];	//����ʣ���λ
	Time_YMDHM_U Max_Power_Time;	//����ʳ���ʱ��
	uint8 Max_FlowRate[4];	//���˲ʱ����
	Time_YMDHM_U Max_FlowRate_Time;	//���˲ʱ��������ʱ��
	uint8 WorkTimeTotal[4];	//�ۻ�����ʱ�䣬��������ʱ�䡣
	uint8 WarnTimeTotal[4];	//�ۻ�����ʱ��
	uint8 Max_Temp[4];	//����¶�ֵ����λ���϶�
	Time_YMDHM_U Max_Temp_Time;	//����¶ȳ���ʱ��
	uint8 Max_TempDif[4];	//����²�ֵ����λ���϶�
	Time_YMDHM_U Max_TempDif_Time;	//����¶Ȳ����ʱ��
	
	uint8 Reserved[12];  //Ԥ�������ṹ�����64���ֽ�
	
}History_Month_Format;  //����ʷ���ݸ�ʽ



/*
  ***********************************************************************
	������7, ����ʷ���ݴ洢��
	EEPROM�洢��ʼ��ַ����Ϊ ADDR_HistoryData_Month,
	��ౣ��24������ʷ��¼����ǰռ��4+64*24=1540�ֽڣ�
	Ԥ����2048�ֽڿռ�.
  ***********************************************************************
*/

typedef struct		//7
{
	SaveArea_Head HeadInfo;
	//History_Month_Format;  ���ṹ����ֻдͷ��Ϣ������ͷ��Ϣ������������Ϣλ�ã�Ȼ��ת����Ӧλ�ò�����


	
}HistoryData_Month_T;  //����ʷ����



typedef struct
{
	uint8 HeatTotal[8];	//�����ۻ���������λ
	uint8 ColdTotal[8];	//�����ۻ���������λ
	uint8 FlowTotal[8];	//�����ۻ���������λ
	uint8 Max_Power[4];	//����ʣ���λ
	Time_YMDHM_U Max_Power_Time;	//����ʳ���ʱ��
	uint8 Max_FlowRate[4];	//���˲ʱ����
	Time_YMDHM_U Max_FlowRate_Time;	//���˲ʱ��������ʱ��
	uint8 WorkTimeTotal[4];	//�ۻ�����ʱ�䣬��������ʱ�䡣
	uint8 WarnTimeTotal[4];	//�ۻ�����ʱ��
	uint8 Max_Temp[4];	//����¶�ֵ����λ���϶�
	Time_YMDHM_U Max_Temp_Time;	//����¶ȳ���ʱ��
	uint8 Max_TempDif[4];	//����²�ֵ����λ���϶�
	Time_YMDHM_U Max_TempDif_Time;	//����¶Ȳ����ʱ��
	
						  //���ṹ�����64���ֽ�
	
}History_Year_Format;  //����ʷ����


/*
  ***********************************************************************
	������8, ����ʷ���ݴ洢��
	EEPROM�洢��ʼ��ַ����Ϊ ADDR_HistoryData_Year,
	��ౣ10�������ʷ��¼����ǰռ��4+64*10=644�ֽڣ�
	Ԥ����2048�ֽڿռ�.
  ***********************************************************************
*/

typedef struct		//8
{
	SaveArea_Head HeadInfo;
	//History_Year_Format;���ṹ����ֻдͷ��Ϣ������ͷ��Ϣ������������Ϣλ�ã�Ȼ��ת����Ӧλ�ò�����


	
}HistoryData_Year_T;  //����ʷ����




#pragma pack()   //end:�ṹ������










#endif  //_MEMORY_CONTENTS_H_

