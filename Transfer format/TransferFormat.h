#pragma once

#ifndef _Transfer_forma_H_
#define _Transfer_forma_H_


/* ����·�� */
#define FILE_PATH_WRONG						-1001
/* �ļ����ĳ��ȴ��� */
#define FILE_NAME_LENGTH_ERROR				-1002
/* �ļ������ڴ��� */
#define FILE_NAME_DATA_ERROR				-1003
/* �ļ���ʱ����� */
#define FILE_NAME_TIME_ERROR				-1004
/* �ļ������� */
#define FILE_NAME_ERROR						-1005
/* ��ȡ�ļ��򿪴��� */
#define FILE_READ_OPEN_ERROR				-1006
/* �ļ����ݸ�ʽ���� */
#define FILE_CONTENT_FORMAT_ERROR			-1007
/* �ļ����ݶ�ȡ���� */
#define FILE_CONTENT_READS_ERROR			-1008
/* �ļ������Ŵ��� */
#define FILE_BATCH_TIME_ERROR				-1009
/* �½��ļ��д��� */
#define NEW_FOLDER_ERROR					-1010
/* �½��ļ����� */
#define NEW_FILE_ERROR						-1011
/* ���ļ�����ȡ���ļ��������� */
#define FILE_FOLDER_READ_ERROR				-1012



/* �½��нṹ����� */
#define NEW_CONSTRUCTION_STRUCTURE_ERROR	-2000
/* �г��ȴ��� */
#define LINE_DATA_LENGTH_ERROR				-2001
/* ��UID��ȡ���� */
#define LINE_UID_READ_ERROR					-2002
/* ��UID���ȳ��� */
#define LINE_UID_LENGTH_ERROR				-2003
/* ��UID���ȳ��� */
#define LINE_UID_ALL_F_ERROR				-2004
/* ��Port��ȡ���� */
#define LINE_PORT_READ_ERROR				-2005
/* ��Port���ȳ��� */
#define LINE_PORT_LENGTH_ERROR				-2006
/* ��Bin��ȡ���� */
#define LINE_BIN_READ_ERROR					-2007
/* ��Bin���ȳ��� */
#define LINE_BIN_LENGTH_ERROR				-2008
/* ��FacN��ȡ���� */
#define LINE_FACN_READ_ERROR				-2009
/* ��LowN��ȡ���� */
#define LINE_LOWN_READ_ERROR				-2010
/* ��FacNo��ȡ���� */
#define LINE_FACNO_READ_ERROR				-2011
/* ��FacNo���ȳ��� */
#define LINE_FACNO_LENGTH_ERROR				-2012
/* FacNo��Ϣ���� */
#define FACNO_INFORMATION_ERROR				-2013
/* ��LowNo��ȡ���� */
#define LINE_LOWNO_READ_ERROR				-2014
/* ��LowNo���ȳ��� */
#define LINE_LOWNO_LENGTH_ERROR				-2015
/* lowNo��Ϣ���� */
#define LOWNO_INFORMATION_ERROR				-2016
/* No�����ڴ���� */
#define NO_ALLOCATION_MEMORY_ERROR			-2017
/* FacNo�����ڴ���� */
#define FACNO_ALLOCATION_MEMORY_ERROR		-2018
/* LowNo�����ڴ���� */
#define LOWNO_ALLOCATION_MEMORY_ERROR		-2019
/* �²����нṹ��ɹ� */
#define NEW_INSERT_ROW_STRUCTURE_SUCCEEDS	0
/* �ͷž��нṹ�壬�²����нṹ��ɹ� */
#define RELEASE_OLD_ROW_STRUCTURE			-2021
/* �ͷ����нṹ�壬���нṹ�岻���в��� */
#define RELEASE_NEW_ROW_STRUCTURE			-2022



/* �½�Bin����ṹ����� */
#define NEW_BIN_INTERVAL_STRUCTURE_ERROR			-3000
/* ��ʼ��Bin�ṹ����� */
#define INITIALIZE_BIN_STRUCTURE_ERROR				-3001
/* ����Bin���䷶Χ���� */
#define SET_BIN_INTERVAL_RANGE_ERROR				-3002
/* Bin�����ʹ��� */
#define BIN_GROUP_TYPE_ERROR						-3003
/* �½�Bin��FacNo�ڴ���� */
#define NEW_BIN_FACNO_MEMORY_ERROR					-3004
/* �½�Bin��LowNo�ڴ���� */
#define NEW_BIN_LOWNO_MEMORY_ERROR					-3005
/* �нṹ�������Bin����� */
#define ROW_STRUCTURE_RESOLVED_TO_BIN_GROUP_ERROR	-3006



/* �½���ϣ���ṹ����� */
#define NEW_HASH_STRUCTURE_ERROR			-4000
/* ��ϣ�����ʼ������ */
#define HASH_TABLE_INITIALIZATION_ERROR		-4001
/* ��ϣ���������� */
#define HASH_TABLE_INSERTION_ERROR			-4002
/* ��ϣ�������ڴ���� */
#define HASH_TABLE_FOR_MOMORY_ERROR			-4003



/* �����С��ȡ���� */
#define INTERVAL_SIZE_READS_ERROR						-5000
/* ���������ֵ��ȡ���� */
#define MAXIMUM_READ_SECTOR_NUMBER_ERROR				-5001
/* ��ϣ���С��ȡ���� */
#define HASH_TABLE_SIZE_READ_ERROR						-5002
/* FacNo�ַ�����󳤶ȶ�ȡ���� */
#define MAXIMUM_LENGTH_READ_FACNO_STRING_ERROR			-5003
/* LowNo�ַ�����󳤶ȶ�ȡ���� */
#define MAXIMUM_LENGTH_READ_LOWNO_STRING_ERROR			-5004
/* �ļ��������ַ�����󳤶ȶ�ȡ���� */
#define FILE_ROW_DATA_STRING_MAXIMUM_LENGTH_READ_ERROR	-5005
/* �ļ��������ַ�����С���ȶ�ȡ���� */
#define FILE_ROW_DATA_STRING_MINIMUM_LENGTH_READ_ERROR	-5006




#define printfLog(...) do{printf(__VA_ARGS__); fprintf(ex_TransferFormatFileHand, __VA_ARGS__);}while(0)



/* �����ļ�����Ĳ��� */
typedef struct
{
	int iIntervalSize;				//�����С
	int iSectorNumberMaximum;		//���������ֵ
	int iHashTableSize;				//��ϣ���С
	int iFacNoLengthMax;			//�������У�FacNo���ַ�����󳤶�
	int iLowNoLengthMax;			//�������У�FacNo���ַ�����󳤶�
	int iLineLengthMax;				//��ȡ�������ļ����е���󳤶�
	int iLineLengthMin;				//��ȡ�������ļ����е���С����

	int iFileNameVersionExist;		//�ļ������Ƿ���ڰ汾����Ϣ
	int iFileNameBatch;				//�ļ������Ƿ����������Ϣ
	int iLineFacExist;				//����Ϣ���Ƿ����Fac��Ϣ
	int iLineLowExist;				//����Ϣ���Ƿ����Low��Ϣ
	int iLineTimeExist;				//����Ϣ���Ƿ����ʱ����Ϣ
}ConfigurationType;



/* ���ƴ�����ʾ����Ϣ��������ļ� */
extern FILE * ex_TransferFormatFileHand;
/* �����ļ� */
extern FILE * ex_ConfigurationFileHand;
/* �������ļ��л�ȡ������ */
extern ConfigurationType ex_ConfigurationData;




void fnSelectSort(int * piData, int iDataLength);


#endif
