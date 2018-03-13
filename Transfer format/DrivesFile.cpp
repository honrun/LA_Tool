// DrivesHash.cpp : 对文件进行处理
//


#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <windows.h>

#include "DrivesLine.h"
#include "DrivesHash.h"
#include "TransferFormat.h"
#include "DrivesVersion.h"
#include "DrivesFile.h"




/*
* return void;
* input:
* function: 配置文件的行数据读取
*/
int fnFileConfigurationGetData(int * iData, char * pszStringBuff, char * pszCategory)
{
	/* 行信息当前的读取指针，行信息复制返回指针 */
	char * pszLineStringNow = NULL, *pszLineStringsReturn = NULL;
	/* 行当前的长度 */
	size_t iLineLength = 0;


	pszLineStringNow = pszStringBuff;

	/* 判断行的长度，根据行的固定格式，行的最大长度为64 */
	iLineLength = strlen(pszStringBuff);
	if (iLineLength > 64)
	{
		return LINE_DATA_LENGTH_ERROR;
	}
	/* 判断当前行是不是“pszCategory”数据 */
	pszLineStringsReturn = strstr(pszStringBuff, pszCategory);
	if (pszLineStringsReturn == NULL)
	{
		return -1;
	}

	/* 移动指针到字符‘=’的位置，为获取数据做准备 */
	pszLineStringNow = (char *)memchr(pszLineStringNow, '=', iLineLength);
	if (pszLineStringNow == NULL)
	{
		return -1;
	}
	/* 跳过字符‘=’与字符‘空格’ */
	pszLineStringNow += 2;
	/* 获取区间大小 */
	*iData = atoi(pszLineStringNow);

	return 0;
}

/*
* return void;
* input:
* function: 配置文件的读取
*/
int fnFileConfigurationRead(ConfigurationType * ptypeConfiguration, FILE * pFileHand)
{
	/* 配置文件里的行数据缓存 */
	char szLineStrings[1024] = { 0 };


	/* 从文件读取一行数据 */
	fgets(szLineStrings, sizeof(szLineStrings), pFileHand);
	if (fnFileConfigurationGetData(&ptypeConfiguration->iIntervalSize, szLineStrings, "区间大小") != 0)
	{
		printfLog("区间大小读取错误！%s:\n", szLineStrings);
		return INTERVAL_SIZE_READS_ERROR;
	}

	/* 清零缓存 */
	memset(szLineStrings, 0, sizeof(szLineStrings));
	/* 从文件读取一行数据 */
	fgets(szLineStrings, sizeof(szLineStrings), pFileHand);
	if (fnFileConfigurationGetData(&ptypeConfiguration->iSectorNumberMaximum, szLineStrings, "扇区号最大值") != 0)
	{
		printfLog("扇区号最大值读取错误！%s:\n", szLineStrings);
		return MAXIMUM_READ_SECTOR_NUMBER_ERROR;
	}

	/* 清零缓存 */
	memset(szLineStrings, 0, sizeof(szLineStrings));
	/* 从文件读取一行数据 */
	fgets(szLineStrings, sizeof(szLineStrings), pFileHand);
	if (fnFileConfigurationGetData(&ptypeConfiguration->iHashTableSize, szLineStrings, "哈希表大小") != 0)
	{
		printfLog("哈希表大小读取错误！%s:\n", szLineStrings);
		return HASH_TABLE_SIZE_READ_ERROR;
	}

	/* 清零缓存 */
	memset(szLineStrings, 0, sizeof(szLineStrings));
	/* 从文件读取一行数据 */
	fgets(szLineStrings, sizeof(szLineStrings), pFileHand);
	if (fnFileConfigurationGetData(&ptypeConfiguration->iFacNoLengthMax, szLineStrings, "FacNo字符串最大长度") != 0)
	{
		printfLog("FacNo字符串最大长度读取错误！%s:\n", szLineStrings);
		return MAXIMUM_LENGTH_READ_FACNO_STRING_ERROR;
	}

	/* 清零缓存 */
	memset(szLineStrings, 0, sizeof(szLineStrings));
	/* 从文件读取一行数据 */
	fgets(szLineStrings, sizeof(szLineStrings), pFileHand);
	if (fnFileConfigurationGetData(&ptypeConfiguration->iLowNoLengthMax, szLineStrings, "LowNo字符串最大长度") != 0)
	{
		printfLog("LowNo字符串最大长度读取错误！%s:\n", szLineStrings);
		return MAXIMUM_LENGTH_READ_LOWNO_STRING_ERROR;
	}

	/* 清零缓存 */
	memset(szLineStrings, 0, sizeof(szLineStrings));
	/* 从文件读取一行数据 */
	fgets(szLineStrings, sizeof(szLineStrings), pFileHand);
	if (fnFileConfigurationGetData(&ptypeConfiguration->iLineLengthMax, szLineStrings, "文件行数据字符串最大长度") != 0)
	{
		printfLog("文件行数据字符串最大长度读取错误！%s:\n", szLineStrings);
		return FILE_ROW_DATA_STRING_MAXIMUM_LENGTH_READ_ERROR;
	}

	/* 清零缓存 */
	memset(szLineStrings, 0, sizeof(szLineStrings));
	/* 从文件读取一行数据 */
	fgets(szLineStrings, sizeof(szLineStrings), pFileHand);
	if (fnFileConfigurationGetData(&ptypeConfiguration->iLineLengthMin, szLineStrings, "文件行数据字符串最小长度") != 0)
	{
		printfLog("文件行数据字符串最小长度！%s:\n", szLineStrings);
		return FILE_ROW_DATA_STRING_MINIMUM_LENGTH_READ_ERROR;
	}

	/* 清零缓存 */
	memset(szLineStrings, 0, sizeof(szLineStrings));
	/* 从文件读取一行数据 */
	fgets(szLineStrings, sizeof(szLineStrings), pFileHand);
	if (fnFileConfigurationGetData(&ptypeConfiguration->iFileNameVersionExist, szLineStrings, "文件名中是否存在版本号信息") != 0)
	{
		printfLog("文件名中是否存在版本号信息！%s:\n", szLineStrings);
		return FILE_ROW_DATA_STRING_MINIMUM_LENGTH_READ_ERROR;
	}

	/* 清零缓存 */
	memset(szLineStrings, 0, sizeof(szLineStrings));
	/* 从文件读取一行数据 */
	fgets(szLineStrings, sizeof(szLineStrings), pFileHand);
	if (fnFileConfigurationGetData(&ptypeConfiguration->iFileNameBatch, szLineStrings, "文件名中是否存在批次信息") != 0)
	{
		printfLog("文件名中是否存在批次信息！%s:\n", szLineStrings);
		return FILE_ROW_DATA_STRING_MINIMUM_LENGTH_READ_ERROR;
	}

	/* 清零缓存 */
	memset(szLineStrings, 0, sizeof(szLineStrings));
	/* 从文件读取一行数据 */
	fgets(szLineStrings, sizeof(szLineStrings), pFileHand);
	if (fnFileConfigurationGetData(&ptypeConfiguration->iLineFacExist, szLineStrings, "行信息中是否存在Fac信息") != 0)
	{
		printfLog("行信息中是否存在Fac信息！%s:\n", szLineStrings);
		return FILE_ROW_DATA_STRING_MINIMUM_LENGTH_READ_ERROR;
	}

	/* 清零缓存 */
	memset(szLineStrings, 0, sizeof(szLineStrings));
	/* 从文件读取一行数据 */
	fgets(szLineStrings, sizeof(szLineStrings), pFileHand);
	if (fnFileConfigurationGetData(&ptypeConfiguration->iLineLowExist, szLineStrings, "行信息中是否存在Low信息") != 0)
	{
		printfLog("行信息中是否存在Low信息！%s:\n", szLineStrings);
		return FILE_ROW_DATA_STRING_MINIMUM_LENGTH_READ_ERROR;
	}

	/* 清零缓存 */
	memset(szLineStrings, 0, sizeof(szLineStrings));
	/* 从文件读取一行数据 */
	fgets(szLineStrings, sizeof(szLineStrings), pFileHand);
	if (fnFileConfigurationGetData(&ptypeConfiguration->iLineTimeExist, szLineStrings, "行信息中是否存在时间信息") != 0)
	{
		printfLog("行信息中是否存在时间信息！%s:\n", szLineStrings);
		return FILE_ROW_DATA_STRING_MINIMUM_LENGTH_READ_ERROR;
	}

	return 0;
}

/*
* return void;
* input:
* function: 配置文件的写入（恢复默认配置）
*/
int fnFileConfigurationWrite(ConfigurationType * ptypeConfiguration, FILE * pFileHand)
{
	fprintf(pFileHand, "区间大小 = %d\n", ptypeConfiguration->iIntervalSize);
	fprintf(pFileHand, "扇区号最大值 = %d\n", ptypeConfiguration->iSectorNumberMaximum);
	fprintf(pFileHand, "哈希表大小 = %d\n", ptypeConfiguration->iHashTableSize);
	fprintf(pFileHand, "FacNo字符串最大长度 = %d\n", ptypeConfiguration->iFacNoLengthMax);
	fprintf(pFileHand, "LowNo字符串最大长度 = %d\n", ptypeConfiguration->iLowNoLengthMax);
	fprintf(pFileHand, "文件行数据字符串最大长度 = %d\n", ptypeConfiguration->iLineLengthMax);
	fprintf(pFileHand, "文件行数据字符串最小长度 = %d\n", ptypeConfiguration->iLineLengthMin);

	fprintf(pFileHand, "文件名中是否存在版本号信息 = %d\n", ptypeConfiguration->iFileNameVersionExist);
	fprintf(pFileHand, "文件名中是否存在批次信息 = %d\n", ptypeConfiguration->iFileNameBatch);
	fprintf(pFileHand, "行信息中是否存在Fac信息 = %d\n", ptypeConfiguration->iLineFacExist);
	fprintf(pFileHand, "行信息中是否存在Low信息 = %d\n", ptypeConfiguration->iLineLowExist);
	fprintf(pFileHand, "行信息中是否存在时间信息 = %d\n", ptypeConfiguration->iLineTimeExist);

	fprintf(pFileHand, "\n注意：\n");
	fprintf(pFileHand, "1、哈希表的大小请设置成一个质数。\n");
	fprintf(pFileHand, "2、如果程序运行奔溃，请手动删除当前配置文件后，再次运行程序。\n");

	return 0;
}

/*
* return void;
* input:
* function: 由文件名获取到文件的日期与时间
* 文件名样例：
123-2017-11-27 183343.txt
*/
int fnFileGetTimeAndDate(char * pszFileTime, char * pszFileDate, char * pszBatch, char * pszVersion, char * pszRootFolderName, char * pszFilePathAndName)
{
	size_t i = 0, iFileTimeLength = 0, iFileDataLength = 0, iFileBatchLength = 0, iFileVersionLength = 0, iFileRootFolderLength = 0;
	/* 文件名剩余长度 */
	size_t iFileNameRemainingLengthNow = 0;
	char * pszFileNameNow = NULL, *pszFileNameReturn = NULL;
	char szVersionNumber[32] = { 0 };


	pszFileNameNow = pszFilePathAndName;

	/* 获取文件名的长度 */
	iFileNameRemainingLengthNow = strlen(pszFileNameNow);
	/* 检查文件名的长度是否正确，规定长度大于等于14，小于等于128 */
	if ((iFileNameRemainingLengthNow < 14) || (iFileNameRemainingLengthNow > MAX_PATH))
	{
		printfLog("FILE_NAME_LENGTH_ERROR is :%d\n", iFileNameRemainingLengthNow);
		return FILE_NAME_LENGTH_ERROR;
	}

	/* 如果文件名中没有包含版本号信息，就获取文件根目录文件夹名作为版本号 */
	if (ex_ConfigurationData.iFileNameVersionExist == 0)
	{
		/* 从文件名读取根文件夹名，以第一个字符‘\’起始 */
		pszFileNameReturn = (char *)memchr(pszFileNameNow, '\\', iFileNameRemainingLengthNow);
		if (pszFileNameReturn == NULL)
		{
			return FILE_FOLDER_READ_ERROR;
		}
		else
		{
			/* 跳过字符‘\’ */
			pszFileNameNow = pszFileNameReturn + 1;
			/* 从文件名读取根文件夹名，以字符‘\’结束 */
			pszFileNameReturn = (char *)_memccpy(pszRootFolderName, pszFileNameNow, '\\', iFileNameRemainingLengthNow);
			if (pszFileNameReturn == NULL)
			{
				memset(pszRootFolderName, 0, sizeof(pszRootFolderName));
			}
			/* 获取文件日期的长度 */
			iFileRootFolderLength = strlen(pszRootFolderName);
			if (iFileRootFolderLength > 1)
			{
				/* 把最后的一个字符‘\’去掉 */
				--iFileRootFolderLength;
				pszRootFolderName[iFileRootFolderLength] = 0;
			}
		}
	}



	/* 获取文件名开始处的位置（最后一个‘\’处），从后往前找 */
	pszFileNameReturn = strrchr(pszFilePathAndName, '\\');
	if (pszFileNameReturn == NULL)
	{
		printfLog("文件路径名出错，没有字符‘\\’！ 文件路径名为:%s\n", pszFilePathAndName);
		return -1;
	}
	/* 跳过字符‘\’ */
	++pszFileNameReturn;
	pszFileNameNow = pszFileNameReturn;

	if (ex_ConfigurationData.iFileNameVersionExist != 0)
	{
		/* 从文件名读取版本号，以字符‘-’结尾 */
		pszFileNameReturn = (char *)_memccpy(pszVersion, pszFileNameNow, '-', iFileNameRemainingLengthNow);
		if (pszFileNameReturn == NULL)
		{
//			printfLog("FILE_BATCH_TIME_ERROR\n");
			return FILE_BATCH_TIME_ERROR;
		}
		/* 移动指针到字符‘-’的位置，跳过批号为获取日期做准备 */
		pszFileNameNow = (char *)memchr(pszFileNameNow, '-', iFileNameRemainingLengthNow);
		/* 跳过字符‘-’ */
		pszFileNameNow++;

		/* 文件名长度去掉已获取的版本号长度 */
		iFileNameRemainingLengthNow -= strlen(pszVersion);
		/* 从文件名读取版本号，以字符‘-’结尾 */
		pszFileNameReturn = (char *)_memccpy(szVersionNumber, pszFileNameNow, '-', iFileNameRemainingLengthNow);
		if (pszFileNameReturn == NULL)
		{
//			printfLog("FILE_BATCH_TIME_ERROR\n");
			return FILE_BATCH_TIME_ERROR;
		}
		/* 移动指针到字符‘-’的位置，跳过批号为获取日期做准备 */
		pszFileNameNow = (char *)memchr(pszFileNameNow, '-', iFileNameRemainingLengthNow);
		/* 跳过字符‘-’ */
		pszFileNameNow++;

		/* 版本号组合 */
		strcat_s(pszVersion, 32, szVersionNumber);
		/* 获取文件日期的长度 */
		iFileVersionLength = strlen(pszVersion);
		if (iFileVersionLength > 1)
		{
			/* 把最后的一个字符‘空格’去掉 */
			--iFileVersionLength;
			pszVersion[iFileVersionLength] = 0;
		}
	}

	if (ex_ConfigurationData.iFileNameBatch != 0)
	{
		/* 文件名长度去掉已获取的版本号长度 */
		iFileNameRemainingLengthNow -= strlen(szVersionNumber);
		/* 从文件名读取批号，以字符‘-’结尾 */
		pszFileNameReturn = (char *)_memccpy(pszBatch, pszFileNameNow, '-', iFileNameRemainingLengthNow);
		if (pszFileNameReturn == NULL)
		{
//			printfLog("FILE_BATCH_TIME_ERROR\n");
			return FILE_BATCH_TIME_ERROR;
		}
		/* 获取文件日期的长度 */
		iFileBatchLength = strlen(pszBatch);
		if (iFileBatchLength > 1)
		{
			/* 把最后的一个字符‘空格’去掉 */
			--iFileBatchLength;
			pszBatch[iFileBatchLength] = 0;
		}
		/* 移动指针到字符‘-’的位置，跳过批号为获取日期做准备 */
		pszFileNameNow = (char *)memchr(pszFileNameNow, '-', iFileNameRemainingLengthNow);
		/* 跳过字符‘-’ */
		pszFileNameNow++;
	}


	/* 文件名长度去掉已获取的批号长度 */
	iFileNameRemainingLengthNow -= strlen(pszBatch);
	/* 从文件名读取日期，以字符‘空格’结尾 */
	pszFileNameReturn = (char *)_memccpy(pszFileDate, pszFileNameNow, ' ', iFileNameRemainingLengthNow);
	if (pszFileNameReturn == NULL)
	{
//		printfLog("FILE_NAME_DATA_ERROR\n");
		return FILE_NAME_DATA_ERROR;
	}
	/* 获取文件日期的长度 */
	iFileDataLength = strlen(pszFileDate);
	if (iFileDataLength > 1)
	{
		/* 把最后的一个字符‘空格’去掉 */
		--iFileDataLength;
		pszFileDate[iFileDataLength] = 0;
		/* 判断获取到的文件日期是否出错 */
		for (i = 0; i < iFileDataLength; ++i)
		{
			if (((pszFileDate[i] >= '0' && pszFileDate[i] <= '9') || pszFileDate[i] == '-') == 0)
			{
//				printfLog("FILE_NAME_DATA_ERROR\n");
				return FILE_NAME_DATA_ERROR;
			}
		}
	}
	/* 移动指针到字符‘空格’的位置，跳过日期为获取时间做准备 */
	pszFileNameNow = (char *)memchr(pszFileNameNow, ' ', iFileNameRemainingLengthNow);
	/* 跳过空格字符 */
	pszFileNameNow++;

	/* 文件名长度去掉已获取的日期长度 */
	iFileNameRemainingLengthNow -= strlen(pszFileDate);
	/* 从文件名读取时间，以字符‘.’结尾 */
	pszFileNameReturn = (char *)_memccpy(pszFileTime, pszFileNameNow, '.', iFileNameRemainingLengthNow);
	if (pszFileNameReturn == NULL)
	{
//		printfLog("FILE_NAME_TIME_ERROR\n");
		return FILE_NAME_TIME_ERROR;
	}
	/* 获取文件时间的长度 */
	iFileTimeLength = strlen(pszFileTime);
	if (iFileTimeLength > 1)
	{
		/* 把最后的一个字符‘空格’去掉 */
		--iFileTimeLength;
		pszFileTime[iFileTimeLength] = 0;
		/* 从文件名读取日期，以字符‘空格’结尾 */
		for (i = 0; i < iFileTimeLength; ++i)
		{
			if ((pszFileTime[i] >= '0' && pszFileTime[i] <= '9') == 0)
			{
//				printfLog("FILE_NAME_TIME_ERROR\n");
				return FILE_NAME_TIME_ERROR;
			}
		}
	}

	return 0;
}

/*
* return void;
* input:
* function: 文件读取
* 文件内容样例： 
FlashUID	Port	Bin	time	FacN	lowN	FacNo	LowNo
Start Log Num Total:0,Pass:0 ,Fail:0

start
EC00DEFAFF4132363034303201010711	Port5	A0	[2018-02-13 12:20:51:0148]	4	0	2060 2061 4094 4095 ;	;
EC00F698521930383434303201010711	Port7	A0	[2018-02-13 12:22:23:0258]	4	0	2060 2061 4094 4095 ;	;
End Log Num Total:1,Pass:1 ,Fail:0 
*/
int fnFileRead(FILE * pFileHand, char * pszFileTime, char * szFilePathAndName, VersionType * ptypeVersionHand)
{
	/* 行数据读取缓存 */
	char * pszLineStrings;
	/* 行数据读取解析事件标志 */
	int iLineParsingFlag = 0;
	/* 新建的行结构体指针 */
	LineDataType * ptypeLineDataNew = NULL;


	/* 为行数据新建一个内存缓存 */
	pszLineStrings = (char *)malloc(sizeof(char) * ex_ConfigurationData.iLineLengthMax);

	/* 一直读到文件的末尾 */
	while (feof(pFileHand) == 0)
	{
		/* 清零缓存 */
		memset(pszLineStrings, 0, sizeof(char) * ex_ConfigurationData.iLineLengthMax);
		/* 从文件读取一行数据 */
		fgets(pszLineStrings, sizeof(char) * ex_ConfigurationData.iLineLengthMax, pFileHand);

		/* 读取的数据行小于最小长度，就直接舍弃，再读取下一行 */
		if (strlen(pszLineStrings) < ex_ConfigurationData.iLineLengthMin)
		{
			/* 读取下一行 */
			continue;
		}

		/* 新建一个行结构体，用来存储当前读到的行的信息 */
		if (fnLineDataNew(&ptypeLineDataNew) != 0)
		{
			printfLog("新建行结构体失败！\n");
			return NEW_CONSTRUCTION_STRUCTURE_ERROR;
		}

		/* 处理当前读取的一行数据 */
		iLineParsingFlag = fnLineParsing(ptypeLineDataNew, pszLineStrings);
		if ((iLineParsingFlag == FACNO_ALLOCATION_MEMORY_ERROR) || (iLineParsingFlag == LOWNO_ALLOCATION_MEMORY_ERROR))
		{
			printfLog("行信息读取分配内存失败！\n");
			/* 释放当前新建的结构体 */
			fnLineDataDelete(&ptypeLineDataNew);
			/* 内存分配失败，则退出当前读取 */
			return iLineParsingFlag;
		}
		else if (iLineParsingFlag != 0)
		{
			printfLog("行信息读取错误！\n");
			/* 释放当前新建的结构体 */
			fnLineDataDelete(&ptypeLineDataNew);
			/* 读取下一行 */
			continue;
		}
		
		/* 添加版本号、批号与文件路径名字信息 */
		strcpy_s(ptypeLineDataNew->version, ptypeVersionHand->version);
		strcpy_s(ptypeLineDataNew->batch, ptypeVersionHand->batch);
		strcpy_s(ptypeLineDataNew->path_and_name, szFilePathAndName);
		/* 如果文件内容行信息内没有时间信息，就把文件名的时间加进来 */
		if (ex_ConfigurationData.iLineTimeExist == 0)
		{
			strcpy_s(ptypeLineDataNew->time, pszFileTime);
		}

		/* 检查获取的当前行结构体内的数据是否有错误 */
		if (fnLineDataDetectionError(ptypeLineDataNew) != 0)
		{
			printfLog("当前获取的行数据有错误！不插入到链表。\n");
			printfLog("行时间为: %s\n", ptypeLineDataNew->time);
			printfLog("UID为: %s\n", ptypeLineDataNew->flashUID);
			printfLog("Bin为: %s\n", ptypeLineDataNew->bin);
			printfLog("\n");

			/* 释放当前新建的结构体 */
			fnLineDataDelete(&ptypeLineDataNew);
			/* 读取下一行 */
			continue;
		}

		/* 获取BKDR哈希值 */
		ptypeLineDataNew->uiHash = fnHashGet32(ptypeLineDataNew->flashUID, HASH_BKDR);

		/* 把行结构体插入到哈希表 */
		if (fnHashLineInsert(ptypeVersionHand, ptypeLineDataNew, ex_ConfigurationData.iHashTableSize) != 0)
		{
			printfLog("行结构体插入到哈希表失败！\n");

			/* 读取下一行 */
			continue;
		}
	} 
	
	/* 释放当前行缓存 */
	free(pszLineStrings);

	return 0;
}

/*
* return void;
* input:
* function: 判断当前文件是否是txt文件，并进行打开，把内容放入行结构体
*/
int fnFileOpenAndRead(char * pszFileName, char * pszFilePath)
{
	FILE * pFile = NULL;
	VersionType * ptypeVersion = NULL;
	/* 文件路径加名字缓存、根文件夹名 */
	char szFilePathAndName[MAX_PATH] = { 0 }, szRootFolderName[MAX_PATH] = { 0 };
	/* 文件类型、文件批号、文件版本号 */
	char szFileType[MAX_PATH] = { 0 }, szFileBatch[MAX_PATH] = { 0 }, szFileVersion[MAX_PATH] = { 0 };
	/* 文件日期缓存； 文件时间缓存 */
	char szFileDate[MAX_PATH] = { 0 }, szFileTime[MAX_PATH] = { 0 };


	/* 把路径和文件名整合到szFilePathAndName */
	sprintf_s(szFilePathAndName, "%s\\%s", pszFilePath, pszFileName);

	/* 获取文件类型到szFileType */
	fnFileGetType(szFileType, pszFileName);

	/* 判断当前文件是否是txt文件，不区分大小写 */
	if (_stricmp(szFileType, "txt") == 0)
	{
		/* 判断当前文件名是否正确，并由文件名获取文件日期与时间 */
		if (fnFileGetTimeAndDate(szFileTime, szFileDate, szFileBatch, szFileVersion, szRootFolderName, szFilePathAndName) != 0)
		{
			return FILE_NAME_ERROR;
		}

		/* 时期和日期进行合并 */
		strcat_s(szFileDate, " ");
		strcat_s(szFileDate, szFileTime);

		/* 打开文件 */
		fopen_s(&pFile, szFilePathAndName, "r");
		if (pFile == NULL)
		{
			printfLog("文件打开失败!:%s\n", szFilePathAndName);
			return FILE_READ_OPEN_ERROR;
		}

		/* 屏幕打印当前打开的文件 */
		printfLog("%s\n", szFilePathAndName);

		/* 如果文件名中没有包含版本号信息，就把根目录文件夹名作为版本号 */
		if (ex_ConfigurationData.iFileNameVersionExist == 0)
		{
			strcpy_s(szFileVersion, szRootFolderName);
		}
		/* 如果文件名中没有包含批次信息，就手动添一个固定空字符串作为批次 */
		if (ex_ConfigurationData.iFileNameBatch == 0)
		{
			strcpy_s(szFileBatch, "");
		}

		/* 由版本号和批次去新建一个对应的版本结构体 */
		if (fnVersionGetHead(&ptypeVersion, szFileVersion, szFileBatch) != 0)
		{
			printfLog("版本号结构体新建错误！:%s-%s\n", szFileVersion, szFileBatch);
			return -1;
		}

		/* 把文件指针、文件日期、文件时间传给文件读取函数 */
		if (fnFileRead(pFile, szFileDate, szFilePathAndName, ptypeVersion) != 0)
		{
			printfLog("文件内信息读取错误！\n");
			fclose(pFile);
			return FILE_CONTENT_READS_ERROR;
		}

		/* 关闭文件 */
		fclose(pFile);
		return 0;
	}

	/* 如果当前文件不是txt文件，就返回1 */
	return 1;
}

/*
* return void;
* input:
* function: 嵌套查找当前文件夹下的所有文件
*/
int fnFileScan(char * pszFilePath)
{
	/* 搜索的文件名与文件路径 */
	char szPathNow[MAX_PATH] = { 0 }, szPathNext[MAX_PATH] = { 0 };
	/* 当前打开的文件夹或文件信息 */
	WIN32_FIND_DATA FindFileData;
	/* 当前打开的文件夹或文件句柄 */
	HANDLE hFind;

	/* 先把当前路径复制，当前文件夹下第一次查找时查找所有文件，所以加上“*.*” */
	sprintf_s(szPathNow, "%s\\*.*", pszFilePath);
	/* 当前文件夹下的第一次打开，此函数配合 FindNextFile 函数使用 */
	hFind = FindFirstFile(szPathNow, &FindFileData);

	/* 当前路径下已经没有文件或者错误 */
	if (INVALID_HANDLE_VALUE == hFind)
		return FILE_PATH_WRONG;

	while (TRUE)
	{
		/* 判断是否为文件夹 */
		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			/* 每个文件夹下会有："."和".."的文件夹信息，表示当前文件夹与上层文件夹。如果不是前面所说的情况，那么就表示查找到的是有效文件夹 */
			if((strcmp(FindFileData.cFileName, ".") != 0) && (strcmp(FindFileData.cFileName, "..") != 0))
			{
				/* 把当前路径与下个文件夹的名字复制到下个路径缓存中 */
				sprintf_s(szPathNext, "%s\\%s", pszFilePath, FindFileData.cFileName);
				/* 递归查找 */
				fnFileScan(szPathNext);
			}
		}
		/* 当前打开的是文件 */
		else
		{
			/* 打开文件，并操作 */
			fnFileOpenAndRead((char *)FindFileData.cFileName, pszFilePath);
		}

		/* 判断是否已经到达当前文件夹下的最后一个文件，如果是，则退出当前嵌套的文件夹 */
		if (!FindNextFile(hFind, &FindFileData))
			break;
	}

	return 0;
}


/*
* return void;
* input:
* function: 由文件名获取文件的类型
*/
int fnFileGetType(char * pszFileType, char * pszFileName)
{
	char * pszFileNameReturnPointer = NULL;


	/* 获取文件名类型开始处的位置（最后一个‘.’处），从后往前找 */
	pszFileNameReturnPointer = strrchr(pszFileName, '.');
	if (pszFileNameReturnPointer == NULL)
	{
		printfLog("文件名出错，没有字符‘.’！ 文件名为:%s\n", pszFileName);
		return -1;
	}
	/* 跳过字符‘.’ */
	++pszFileNameReturnPointer;

	/* 复制文件名类型 */
	while (*pszFileNameReturnPointer != 0)
	{
		*pszFileType++ = *pszFileNameReturnPointer++;
	}

	/* 最后设置'\0'，表示字符串结束 */
	*pszFileType = 0;

	return 0;
}
