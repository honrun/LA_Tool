#ifndef _DRIVERS_BIN_H_
#define _DRIVERS_BIN_H_

#include "DrivesLine.h"



#ifndef BIN_SECTION_RANGE_MAX
#define BIN_SECTION_RANGE_MAX 65536
#else
#error "BIN_SECTION_RANGE_MAX 已经被定义！"
#endif




#define BIN_INCREASES_AMOUNT_MEMORY 128






/* 整理后Bin的结构体 */
typedef struct GroupInformationStruct {
	int * fac_no;										//原厂坏块号
	int * low_no;										//修复后坏块号
	int fac_no_number_length;							//记录当前结构体内有多少个不同的原厂坏块号，供给上面int * fac_no;申请内存时当作大小使用
	int low_no_number_length;							//记录当前结构体内有多少个不同的修复后坏块号，供给上面int * low_no;申请内存时当作大小使用

	int fac_number;										//原厂当前区间的坏块数量
	int low_number;										//修复后当前区间的坏块数量

	float fac_percentage;								//原厂坏块当前区间百分比
	float low_percentage;								//修复后坏块当前区间百分比

	char * fac_hash_flag;								//当前区间扇区号FacNo的哈希表
	char * low_hash_flag;								//当前区间扇区号LowNo的哈希表
	int fac_flag;										//原厂坏块标志量
	int low_flag;										//修复后坏块标志量

	int bin_number;										//头结构体内这个变量有用，用来记录各种Bin的数量

	struct GroupInformationStruct * next;
	struct GroupInformationStruct * previous;
}GroupInformationType;


/* 所有Bin分组的结构体 */
typedef struct binStruct {
	GroupInformationType * A0;          //A0组
	GroupInformationType * A1;          //A1组
	GroupInformationType * B0;          //B0组
	GroupInformationType * B1;          //B1组
	GroupInformationType * C0;          //C0组
	GroupInformationType * C1;          //C1组
	GroupInformationType * E0;          //E0组
	GroupInformationType * F0;          //F0组
	GroupInformationType * F1;          //F1组
	GroupInformationType * F2;          //F2组
	GroupInformationType * F3;          //F3组

	int scope;                          //区间内范围数

	struct binStruct * next;
	struct binStruct * previous;
}BinType;




extern BinType ex_typeBinData;


int fnBinGroupInformationNew(GroupInformationType ** ppGroupInformationType);

int fnBinInsertGroupInformation(GroupInformationType * ptypeHead, GroupInformationType * ptypeData);

int fnBinGetLineData(GroupInformationType * ptypeGroupInformation, LineDataType * ptypeLineData, int iScope);

int fnBinGetHashLineList(BinType * ptypeBin, LineDataType * ptypeLineDataHashTable, int iHashTableLength);

int fnBinGetHashLineUIDSameList(FILE * pFileNewHand, LineDataType * ptypeLineDataHashTable, int iHashTableLength);

int fnBinALLPercentage(BinType * ptypeBin);

int fnBinALLSorting(BinType * ptypeBin);

int fnBinGroupInformationShow(GroupInformationType * ptypeGroupInformation);

int fnBinALLPrintfToFile(FILE * pFileHand, BinType * ptypeBinData, BinType * ptypeBinDataUIDSame);

int fnBinALLShow(BinType * ptypeBin);

int fnBinDataTypeInit(BinType * pbinType, int iScope);


#endif
