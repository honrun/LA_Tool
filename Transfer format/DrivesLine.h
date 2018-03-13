#pragma once

#ifndef _DRIVERS_LINE_H_
#define _DRIVERS_LINE_H_


#ifndef LINE_STRING_LENGTH
#define LINE_STRING_LENGTH (4096 * 5 * 2 + 1024)
#endif

/* 行最小长度 */
#ifndef LINE_STRING_LENGTH_MIN
#define LINE_STRING_LENGTH_MIN 48
#endif

/* FacNo最大长度 */
#ifndef LINE_FACNO_LENGTH_MAX
#define LINE_FACNO_LENGTH_MAX LINE_STRING_LENGTH
#endif

/* LowNo最大长度 */
#ifndef LINE_LOWNO_LENGTH_MAX
#define LINE_LOWNO_LENGTH_MAX LINE_STRING_LENGTH
#endif

/* UID最大长度 */
#ifndef LINE_UID_LENGTH_MAX
#define LINE_UID_LENGTH_MAX 64
#endif

/* Port最大长度 */
#ifndef LINE_PORT_LENGTH_MAX
#define LINE_PORT_LENGTH_MAX 32
#endif

/* Bin最大长度 */
#ifndef LINE_BIN_LENGTH_MAX
#define LINE_BIN_LENGTH_MAX 32
#endif

/* 时间最大长度 */
#ifndef LINE_TIME_LENGTH_MAX
#define LINE_TIME_LENGTH_MAX 32
#endif

/* 行时间信息是否存在 */
#ifndef LINE_TIME_EXIST
#define LINE_TIME_EXIST 1
#endif

/* 行Fac信息是否存在 */
#ifndef LINE_FAC_EXIST
#define LINE_FAC_EXIST 1
#endif

/* 行Low信息是否存在 */
#ifndef LINE_LOW_EXIST
#define LINE_LOW_EXIST 1
#endif

/* 文件名版本号信息是否存在 */
#ifndef FILE_NAME_VERSION_EXIST
#define FILE_NAME_VERSION_EXIST 1
#endif

/* 文件名批次信息是否存在 */
#ifndef FILE_NAME_BATCH_EXIST
#define FILE_NAME_BATCH_EXIST 1
#endif



/* 文件里面每行数据的结构体 */
typedef struct LineDataStruct {
	char ** UID_same_path_and_name;			//UID相同文件路径和名字
	char path_and_name[MAX_PATH];			//当前文件路径和名字
	char time[LINE_TIME_LENGTH_MAX];		//时间
	char version[LINE_TIME_LENGTH_MAX];		//版本号
	char batch[LINE_TIME_LENGTH_MAX];		//批号

	char flashUID[LINE_UID_LENGTH_MAX];		//UID
	char port[LINE_PORT_LENGTH_MAX];		//Port
	char bin[LINE_BIN_LENGTH_MAX];			//组号
	int fac_number;							//出厂坏块数量
	int low_number;							//修复后坏块数量
	int * fac_no;							//出厂坏块信息
	int * low_no;							//修复后坏块信息

	int fac_no_number_length;				//程序计数得到的出厂坏块数量
	int low_no_number_length;				//程序计数得到的修复后坏块数量

	unsigned int times_of_repetition;		//UID重复次数
	unsigned int uiHash;					//UID的HASH值

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
