#pragma once

#ifndef _DRIVERS_LINE_H_
#define _DRIVERS_LINE_H_


#ifndef LINE_STRING_LENGTH
#define LINE_STRING_LENGTH (4096 * 5 * 2 + 1024)
#endif

/* ����С���� */
#ifndef LINE_STRING_LENGTH_MIN
#define LINE_STRING_LENGTH_MIN 48
#endif

/* FacNo��󳤶� */
#ifndef LINE_FACNO_LENGTH_MAX
#define LINE_FACNO_LENGTH_MAX LINE_STRING_LENGTH
#endif

/* LowNo��󳤶� */
#ifndef LINE_LOWNO_LENGTH_MAX
#define LINE_LOWNO_LENGTH_MAX LINE_STRING_LENGTH
#endif

/* UID��󳤶� */
#ifndef LINE_UID_LENGTH_MAX
#define LINE_UID_LENGTH_MAX 64
#endif

/* Port��󳤶� */
#ifndef LINE_PORT_LENGTH_MAX
#define LINE_PORT_LENGTH_MAX 32
#endif

/* Bin��󳤶� */
#ifndef LINE_BIN_LENGTH_MAX
#define LINE_BIN_LENGTH_MAX 32
#endif

/* ʱ����󳤶� */
#ifndef LINE_TIME_LENGTH_MAX
#define LINE_TIME_LENGTH_MAX 32
#endif

/* ��ʱ����Ϣ�Ƿ���� */
#ifndef LINE_TIME_EXIST
#define LINE_TIME_EXIST 1
#endif

/* ��Fac��Ϣ�Ƿ���� */
#ifndef LINE_FAC_EXIST
#define LINE_FAC_EXIST 1
#endif

/* ��Low��Ϣ�Ƿ���� */
#ifndef LINE_LOW_EXIST
#define LINE_LOW_EXIST 1
#endif

/* �ļ����汾����Ϣ�Ƿ���� */
#ifndef FILE_NAME_VERSION_EXIST
#define FILE_NAME_VERSION_EXIST 1
#endif

/* �ļ���������Ϣ�Ƿ���� */
#ifndef FILE_NAME_BATCH_EXIST
#define FILE_NAME_BATCH_EXIST 1
#endif



/* �ļ�����ÿ�����ݵĽṹ�� */
typedef struct LineDataStruct {
	char ** UID_same_path_and_name;			//UID��ͬ�ļ�·��������
	char path_and_name[MAX_PATH];			//��ǰ�ļ�·��������
	char time[LINE_TIME_LENGTH_MAX];		//ʱ��
	char version[LINE_TIME_LENGTH_MAX];		//�汾��
	char batch[LINE_TIME_LENGTH_MAX];		//����

	char flashUID[LINE_UID_LENGTH_MAX];		//UID
	char port[LINE_PORT_LENGTH_MAX];		//Port
	char bin[LINE_BIN_LENGTH_MAX];			//���
	int fac_number;							//������������
	int low_number;							//�޸��󻵿�����
	int * fac_no;							//����������Ϣ
	int * low_no;							//�޸��󻵿���Ϣ

	int fac_no_number_length;				//��������õ��ĳ�����������
	int low_no_number_length;				//��������õ����޸��󻵿�����

	unsigned int times_of_repetition;		//UID�ظ�����
	unsigned int uiHash;					//UID��HASHֵ

	struct LineDataStruct * next;
	struct LineDataStruct * previous;
}LineDataType;





int fnLineDataNew(LineDataType ** ppLineDataType);

int fnLineDataDelete(LineDataType ** ppLineDataType);

int fnLineDataDetectionError(LineDataType * pLineDataType);

int fnLineDataHeadInsert(LineDataType * ptypeHead, LineDataType * ptypeData);

int fnLineDataEndInsert(LineDataType * ptypeHead, LineDataType * ptypeData);

int fnLineUIDSameAddRepetition(LineDataType * ptypeData, char * pszBin, char * pszPathAndName);

int fnLineDataOrderInsert(LineDataType * ptypeHead, LineDataType * ptypeData);

int fnLineDataShow(LineDataType * ptypeLineDataHead);

int fnLineParsing(LineDataType * ptypeLineData, char * pszLineString);

int fnLineNoConversionToNumber(int ** ppiNumberBuff, int * piNumberLength, char * pszNoString);

#endif
