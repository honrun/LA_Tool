// DrivesVersion.cpp : 对版本号结构体进行处理
//


#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <direct.h>
#include <io.h>
#include <malloc.h>
#include <windows.h>
#include <time.h>

#include "TransferFormat.h"
#include "DrivesBin.h"
#include "DrivesFile.h"
#include "DrivesHash.h"
#include "DrivesVersion.h"


/* 版本号头 */
VersionType ex_typeVersionHead = { 0 };


/*
* return void;
* input:
* function: 新建一个版本号数据结构体
*/
int fnVersionNew(VersionType ** ppVersionType)
{
	*ppVersionType = (VersionType *)malloc(sizeof(VersionType));
	if (*ppVersionType == NULL)
	{
		printfLog("\n新建版本号结构体失败！\n");
		return NEW_BIN_INTERVAL_STRUCTURE_ERROR;
	}

	/* 清空结构体 */
	memset(*ppVersionType, 0, sizeof(VersionType));

	return 0;
}


/*
* return void;
* input:
* function: 把新建的版本号结构体插入到链表
*/
int fnVersionInsert(VersionType * ptypeHead, VersionType * ptypeData)
{
	VersionType * ptypeVersionNow = NULL;

	ptypeVersionNow = ptypeHead;
	while (ptypeVersionNow->next != NULL)
	{
		ptypeVersionNow = ptypeVersionNow->next;
	}

	ptypeVersionNow->next = ptypeData;

	return 0;
}

int fnVersionGetHead(VersionType ** pptypeHead, char * pszVersionName, char * pszBatchName)
{
	VersionType * ptypeVersionNow = NULL;

	/* 查找是否已经建立了该版本号的结构体 */
	ptypeVersionNow = ex_typeVersionHead.next;
	while (ptypeVersionNow != NULL)
	{
		if ((strcmp(ptypeVersionNow->version, pszVersionName) == 0) && (strcmp(ptypeVersionNow->batch, pszBatchName) == 0))
		{
			*pptypeHead = ptypeVersionNow;
			return 0;
		}

		ptypeVersionNow = ptypeVersionNow->next;
	}

	/* 新建该版本号的结构体 */
	fnVersionNew(&ptypeVersionNow);

	/* 复制版本号与批号名字 */
	strcpy_s(ptypeVersionNow->version, pszVersionName);
	strcpy_s(ptypeVersionNow->batch, pszBatchName);
	/* 哈希表初始化 */
	if (fnHashLineDataTableInit(&(ptypeVersionNow->ptypeLineDataHashTable), ex_ConfigurationData.iHashTableSize) != 0)
	{
		printfLog("\n新建版本号文件:%s时，哈希表初始化失败！\n", pszVersionName);

		return HASH_TABLE_INITIALIZATION_ERROR;
	}
	/* 哈希表初始化 */
	if (fnHashLineDataTableInit(&(ptypeVersionNow->ptypeLineDataUIDSameHashTable), ex_ConfigurationData.iHashTableSize) != 0)
	{
		printfLog("\n新建版本号文件:%s时，哈希表初始化失败！\n", pszVersionName);

		return HASH_TABLE_INITIALIZATION_ERROR;
	}
	/* Bin表格初始化 */
	if (fnBinDataTypeInit(&(ptypeVersionNow->typeBinData), ex_ConfigurationData.iIntervalSize) != 0)
	{
		printfLog("\n新建版本号文件:%s时，Bin表格初始化初始化失败！\n", pszVersionName);

		return INITIALIZE_BIN_STRUCTURE_ERROR;
	}
	/* Bin表格初始化 */
	if (fnBinDataTypeInit(&(ptypeVersionNow->typeBinDataUIDSame), ex_ConfigurationData.iIntervalSize) != 0)
	{
		printfLog("\n新建版本号文件:%s时，Bin表格初始化初始化失败！\n", pszVersionName);

		return INITIALIZE_BIN_STRUCTURE_ERROR;
	}

	fnVersionInsert(&ex_typeVersionHead, ptypeVersionNow);

	*pptypeHead = ptypeVersionNow;

	return 0;
}

int fnVersionLineToBin(VersionType * ptypeVersionHead)
{
	VersionType * ptypeVersionNow = NULL;

	/* 遍历版本号的结构体 */
	ptypeVersionNow = ptypeVersionHead->next;
	while (ptypeVersionNow != NULL)
	{
		fnBinGetHashLineList(&(ptypeVersionNow->typeBinData), ptypeVersionNow->ptypeLineDataHashTable, ex_ConfigurationData.iHashTableSize);
		
		fnBinGetHashLineList(&(ptypeVersionNow->typeBinDataUIDSame), ptypeVersionNow->ptypeLineDataUIDSameHashTable, ex_ConfigurationData.iHashTableSize);

		ptypeVersionNow = ptypeVersionNow->next;
	}

	return 0;
}

int fnVersionUIDSameLineToFile(VersionType * ptypeVersionHead)
{
	/* 整理后的csv文件指针 */
	FILE * pFileNewHand = NULL;
	VersionType * ptypeVersionNow = NULL;
	char szFileName[MAX_PATH] = { 0 };
	char szFolderName[MAX_PATH] = { 0 };

	/* 遍历版本号的结构体 */
	ptypeVersionNow = ptypeVersionHead->next;
	while (ptypeVersionNow != NULL)
	{
		sprintf_s(szFolderName, ".\\Log Analysis Results");
		if (_access(szFolderName, 0) == -1)
		{
			/* 新建文件夹 */
			if (_mkdir(szFolderName) != 0)
			{
				printfLog("新建文件夹:%s错误！\n", szFolderName);
				return NEW_FOLDER_ERROR;
			}
		}
		sprintf_s(szFolderName, ".\\Log Analysis Results\\%s", ptypeVersionNow->version);
		if (_access(szFolderName, 0) == -1)
		{
			if (_mkdir(szFolderName) != 0)
			{
				printfLog("新建文件夹:%s错误！\n", szFolderName);
				return NEW_FOLDER_ERROR;
			}
		}

		/* 新建.csv文件 */
		sprintf_s(szFileName, "%s\\%s-%s-UIDSame.csv", szFolderName, ptypeVersionNow->version, ptypeVersionNow->batch);

		/* 创建.csv新文件，以用于接下来的整理输出 */
		fopen_s(&pFileNewHand, szFileName, "w");
		if (pFileNewHand == NULL)
		{
			printfLog("创建整理文件:%s错误！\n", szFileName);
			return NEW_FILE_ERROR;
		}

		fnBinGetHashLineUIDSameList(pFileNewHand, ptypeVersionNow->ptypeLineDataHashTable, ex_ConfigurationData.iHashTableSize);

		/* 关闭创建的.csv新文件 */
		fclose(pFileNewHand);

		ptypeVersionNow = ptypeVersionNow->next;
	}

	return 0;
}



int fnVersionALLToFile(VersionType * ptypeVersionHead)
{
	/* 整理后的csv文件指针 */
	FILE * pFileNewHand = NULL;
	VersionType * ptypeVersionNow = NULL;
	char szFileName[128] = { 0 };
	char szFolderName[128] = { 0 };


	ptypeVersionNow = ptypeVersionHead->next;
	while (ptypeVersionNow != NULL)
	{
		sprintf_s(szFolderName, ".\\Log Analysis Results");
		if (_access(szFolderName, 0) == -1)
		{
			/* 新建文件夹 */
			if (_mkdir(szFolderName) != 0)
			{
				printfLog("新建文件夹:%s错误！\n", szFolderName);
				return NEW_FOLDER_ERROR;
			}
		}
		sprintf_s(szFolderName, ".\\Log Analysis Results\\%s", ptypeVersionNow->version);
		if (_access(szFolderName, 0) == -1)
		{
			if (_mkdir(szFolderName) != 0)
			{
				printfLog("新建文件夹:%s错误！\n", szFolderName);
				return NEW_FOLDER_ERROR;
			}
		}

		/* 新建.csv文件 */
		sprintf_s(szFileName, "%s\\%s-%s.csv", szFolderName, ptypeVersionNow->version, ptypeVersionNow->batch);

		/* 创建.csv新文件，以用于接下来的整理输出 */
		fopen_s(&pFileNewHand, szFileName, "w");
		if (pFileNewHand == NULL)
		{
			printfLog("创建整理文件:%s错误！\n", szFileName);
			return NEW_FILE_ERROR;
		}

		/* 把Bin结构体整理后的所有数据输出到文件 */
		fnBinALLPrintfToFile(pFileNewHand, &(ptypeVersionNow->typeBinData), &(ptypeVersionNow->typeBinDataUIDSame));

		/* 关闭创建的.csv新文件 */
		fclose(pFileNewHand);

		ptypeVersionNow = ptypeVersionNow->next;
	}

	return 0;
}
