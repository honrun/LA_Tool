#pragma once

#ifndef _Transfer_forma_H_
#define _Transfer_forma_H_


/* 错误路径 */
#define FILE_PATH_WRONG						-1001
/* 文件名的长度错误 */
#define FILE_NAME_LENGTH_ERROR				-1002
/* 文件名日期错误 */
#define FILE_NAME_DATA_ERROR				-1003
/* 文件名时间错误 */
#define FILE_NAME_TIME_ERROR				-1004
/* 文件名错误 */
#define FILE_NAME_ERROR						-1005
/* 读取文件打开错误 */
#define FILE_READ_OPEN_ERROR				-1006
/* 文件内容格式错误 */
#define FILE_CONTENT_FORMAT_ERROR			-1007
/* 文件内容读取错误 */
#define FILE_CONTENT_READS_ERROR			-1008
/* 文件名批号错误 */
#define FILE_BATCH_TIME_ERROR				-1009
/* 新建文件夹错误 */
#define NEW_FOLDER_ERROR					-1010
/* 新建文件错误 */
#define NEW_FILE_ERROR						-1011
/* 从文件名读取根文件夹名错误 */
#define FILE_FOLDER_READ_ERROR				-1012



/* 新建行结构体错误 */
#define NEW_CONSTRUCTION_STRUCTURE_ERROR	-2000
/* 行长度错误 */
#define LINE_DATA_LENGTH_ERROR				-2001
/* 行UID读取出错 */
#define LINE_UID_READ_ERROR					-2002
/* 行UID长度出错 */
#define LINE_UID_LENGTH_ERROR				-2003
/* 行UID长度出错 */
#define LINE_UID_ALL_F_ERROR				-2004
/* 行Port读取出错 */
#define LINE_PORT_READ_ERROR				-2005
/* 行Port长度出错 */
#define LINE_PORT_LENGTH_ERROR				-2006
/* 行Bin读取出错 */
#define LINE_BIN_READ_ERROR					-2007
/* 行Bin长度出错 */
#define LINE_BIN_LENGTH_ERROR				-2008
/* 行FacN读取出错 */
#define LINE_FACN_READ_ERROR				-2009
/* 行LowN读取出错 */
#define LINE_LOWN_READ_ERROR				-2010
/* 行FacNo读取出错 */
#define LINE_FACNO_READ_ERROR				-2011
/* 行FacNo长度出错 */
#define LINE_FACNO_LENGTH_ERROR				-2012
/* FacNo信息错误 */
#define FACNO_INFORMATION_ERROR				-2013
/* 行LowNo读取出错 */
#define LINE_LOWNO_READ_ERROR				-2014
/* 行LowNo长度出错 */
#define LINE_LOWNO_LENGTH_ERROR				-2015
/* lowNo信息错误 */
#define LOWNO_INFORMATION_ERROR				-2016
/* No分配内存错误 */
#define NO_ALLOCATION_MEMORY_ERROR			-2017
/* FacNo分配内存错误 */
#define FACNO_ALLOCATION_MEMORY_ERROR		-2018
/* LowNo分配内存错误 */
#define LOWNO_ALLOCATION_MEMORY_ERROR		-2019
/* 新插入行结构体成功 */
#define NEW_INSERT_ROW_STRUCTURE_SUCCEEDS	0
/* 释放旧行结构体，新插入行结构体成功 */
#define RELEASE_OLD_ROW_STRUCTURE			-2021
/* 释放新行结构体，新行结构体不进行插入 */
#define RELEASE_NEW_ROW_STRUCTURE			-2022



/* 新建Bin区间结构体错误 */
#define NEW_BIN_INTERVAL_STRUCTURE_ERROR			-3000
/* 初始化Bin结构体错误 */
#define INITIALIZE_BIN_STRUCTURE_ERROR				-3001
/* 设置Bin区间范围错误 */
#define SET_BIN_INTERVAL_RANGE_ERROR				-3002
/* Bin组类型错误 */
#define BIN_GROUP_TYPE_ERROR						-3003
/* 新建Bin组FacNo内存错误 */
#define NEW_BIN_FACNO_MEMORY_ERROR					-3004
/* 新建Bin组LowNo内存错误 */
#define NEW_BIN_LOWNO_MEMORY_ERROR					-3005
/* 行结构体解析到Bin组错误 */
#define ROW_STRUCTURE_RESOLVED_TO_BIN_GROUP_ERROR	-3006



/* 新建哈希望结构体错误 */
#define NEW_HASH_STRUCTURE_ERROR			-4000
/* 哈希链表初始化错误 */
#define HASH_TABLE_INITIALIZATION_ERROR		-4001
/* 哈希链表插入错误 */
#define HASH_TABLE_INSERTION_ERROR			-4002
/* 哈希表申请内存错误 */
#define HASH_TABLE_FOR_MOMORY_ERROR			-4003



/* 区间大小读取错误 */
#define INTERVAL_SIZE_READS_ERROR						-5000
/* 扇区号最大值读取错误 */
#define MAXIMUM_READ_SECTOR_NUMBER_ERROR				-5001
/* 哈希表大小读取错误 */
#define HASH_TABLE_SIZE_READ_ERROR						-5002
/* FacNo字符串最大长度读取错误 */
#define MAXIMUM_LENGTH_READ_FACNO_STRING_ERROR			-5003
/* LowNo字符串最大长度读取错误 */
#define MAXIMUM_LENGTH_READ_LOWNO_STRING_ERROR			-5004
/* 文件行数据字符串最大长度读取错误 */
#define FILE_ROW_DATA_STRING_MAXIMUM_LENGTH_READ_ERROR	-5005
/* 文件行数据字符串最小长度读取错误 */
#define FILE_ROW_DATA_STRING_MINIMUM_LENGTH_READ_ERROR	-5006




#define printfLog(...) do{printf(__VA_ARGS__); fprintf(ex_TransferFormatFileHand, __VA_ARGS__);}while(0)



/* 配置文件里面的参数 */
typedef struct
{
	int iIntervalSize;				//区间大小
	int iSectorNumberMaximum;		//扇区号最大值
	int iHashTableSize;				//哈希表大小
	int iFacNoLengthMax;			//行数据中，FacNo总字符的最大长度
	int iLowNoLengthMax;			//行数据中，FacNo总字符的最大长度
	int iLineLengthMax;				//读取的数据文件中行的最大长度
	int iLineLengthMin;				//读取的数据文件中行的最小长度

	int iFileNameVersionExist;		//文件名中是否存在版本号信息
	int iFileNameBatch;				//文件名中是否存在批次信息
	int iLineFacExist;				//行信息中是否存在Fac信息
	int iLineLowExist;				//行信息中是否存在Low信息
	int iLineTimeExist;				//行信息中是否存在时间信息
}ConfigurationType;



/* 控制窗口显示的信息输出到此文件 */
extern FILE * ex_TransferFormatFileHand;
/* 配置文件 */
extern FILE * ex_ConfigurationFileHand;
/* 从配置文件中获取的数据 */
extern ConfigurationType ex_ConfigurationData;




void fnSelectSort(int * piData, int iDataLength);


#endif
