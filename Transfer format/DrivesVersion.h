#ifndef _DrivesVersion_H_
#define _DrivesVersion_H_

#include "DrivesLine.h"
#include "DrivesBin.h"



/* 版本号最大长度 */
#ifndef VERSION_NAME_LENGTH_MAX
#define VERSION_NAME_LENGTH_MAX 64
#endif




/* 文件版本的结构体 */
typedef struct VersionStruct {
	char version[VERSION_NAME_LENGTH_MAX];					//版本号
	char batch[VERSION_NAME_LENGTH_MAX];					//批号
	LineDataType * ptypeLineDataHashTable;					//哈西表
	LineDataType * ptypeLineDataUIDSameHashTable;			//具有重复UID的哈西表
	BinType typeBinData;									//Bin系列组数据
	BinType typeBinDataUIDSame;								//具有重复UID的Bin系列组数据

	struct VersionStruct * next;
	struct VersionStruct * previous;
}VersionType;



/* 版本号头 */
extern VersionType ex_typeVersionHead;



int fnVersionNew(VersionType ** ppVersionType);

int fnVersionInsert(VersionType * ptypeHead, VersionType * ptypeData);

int fnVersionGetHead(VersionType ** pptypeHead, char * pszVersionName, char * pszBatchName);

int fnVersionLineToBin(VersionType * ptypeVersionHead);

int fnVersionUIDSameLineToFile(VersionType * ptypeVersionHead);

int fnVersionALLToFile(VersionType * ptypeVersionHead);


#endif
