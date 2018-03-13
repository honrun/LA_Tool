// DrivesVersion.cpp : �԰汾�Žṹ����д���
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


/* �汾��ͷ */
VersionType ex_typeVersionHead = { 0 };


/*
* return void;
* input:
* function: �½�һ���汾�����ݽṹ��
*/
int fnVersionNew(VersionType ** ppVersionType)
{
	*ppVersionType = (VersionType *)malloc(sizeof(VersionType));
	if (*ppVersionType == NULL)
	{
		printfLog("\n�½��汾�Žṹ��ʧ�ܣ�\n");
		return NEW_BIN_INTERVAL_STRUCTURE_ERROR;
	}

	/* ��սṹ�� */
	memset(*ppVersionType, 0, sizeof(VersionType));

	return 0;
}


/*
* return void;
* input:
* function: ���½��İ汾�Žṹ����뵽����
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

	/* �����Ƿ��Ѿ������˸ð汾�ŵĽṹ�� */
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

	/* �½��ð汾�ŵĽṹ�� */
	fnVersionNew(&ptypeVersionNow);

	/* ���ư汾������������ */
	strcpy_s(ptypeVersionNow->version, pszVersionName);
	strcpy_s(ptypeVersionNow->batch, pszBatchName);
	/* ��ϣ���ʼ�� */
	if (fnHashLineDataTableInit(&(ptypeVersionNow->ptypeLineDataHashTable), ex_ConfigurationData.iHashTableSize) != 0)
	{
		printfLog("\n�½��汾���ļ�:%sʱ����ϣ���ʼ��ʧ�ܣ�\n", pszVersionName);

		return HASH_TABLE_INITIALIZATION_ERROR;
	}
	/* ��ϣ���ʼ�� */
	if (fnHashLineDataTableInit(&(ptypeVersionNow->ptypeLineDataUIDSameHashTable), ex_ConfigurationData.iHashTableSize) != 0)
	{
		printfLog("\n�½��汾���ļ�:%sʱ����ϣ���ʼ��ʧ�ܣ�\n", pszVersionName);

		return HASH_TABLE_INITIALIZATION_ERROR;
	}
	/* Bin����ʼ�� */
	if (fnBinDataTypeInit(&(ptypeVersionNow->typeBinData), ex_ConfigurationData.iIntervalSize) != 0)
	{
		printfLog("\n�½��汾���ļ�:%sʱ��Bin����ʼ����ʼ��ʧ�ܣ�\n", pszVersionName);

		return INITIALIZE_BIN_STRUCTURE_ERROR;
	}
	/* Bin����ʼ�� */
	if (fnBinDataTypeInit(&(ptypeVersionNow->typeBinDataUIDSame), ex_ConfigurationData.iIntervalSize) != 0)
	{
		printfLog("\n�½��汾���ļ�:%sʱ��Bin����ʼ����ʼ��ʧ�ܣ�\n", pszVersionName);

		return INITIALIZE_BIN_STRUCTURE_ERROR;
	}

	fnVersionInsert(&ex_typeVersionHead, ptypeVersionNow);

	*pptypeHead = ptypeVersionNow;

	return 0;
}

int fnVersionLineToBin(VersionType * ptypeVersionHead)
{
	VersionType * ptypeVersionNow = NULL;

	/* �����汾�ŵĽṹ�� */
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
	/* ������csv�ļ�ָ�� */
	FILE * pFileNewHand = NULL;
	VersionType * ptypeVersionNow = NULL;
	char szFileName[MAX_PATH] = { 0 };
	char szFolderName[MAX_PATH] = { 0 };

	/* �����汾�ŵĽṹ�� */
	ptypeVersionNow = ptypeVersionHead->next;
	while (ptypeVersionNow != NULL)
	{
		sprintf_s(szFolderName, ".\\Log Analysis Results");
		if (_access(szFolderName, 0) == -1)
		{
			/* �½��ļ��� */
			if (_mkdir(szFolderName) != 0)
			{
				printfLog("�½��ļ���:%s����\n", szFolderName);
				return NEW_FOLDER_ERROR;
			}
		}
		sprintf_s(szFolderName, ".\\Log Analysis Results\\%s", ptypeVersionNow->version);
		if (_access(szFolderName, 0) == -1)
		{
			if (_mkdir(szFolderName) != 0)
			{
				printfLog("�½��ļ���:%s����\n", szFolderName);
				return NEW_FOLDER_ERROR;
			}
		}

		/* �½�.csv�ļ� */
		sprintf_s(szFileName, "%s\\%s-%s-UIDSame.csv", szFolderName, ptypeVersionNow->version, ptypeVersionNow->batch);

		/* ����.csv���ļ��������ڽ�������������� */
		fopen_s(&pFileNewHand, szFileName, "w");
		if (pFileNewHand == NULL)
		{
			printfLog("���������ļ�:%s����\n", szFileName);
			return NEW_FILE_ERROR;
		}

		fnBinGetHashLineUIDSameList(pFileNewHand, ptypeVersionNow->ptypeLineDataHashTable, ex_ConfigurationData.iHashTableSize);

		/* �رմ�����.csv���ļ� */
		fclose(pFileNewHand);

		ptypeVersionNow = ptypeVersionNow->next;
	}

	return 0;
}



int fnVersionALLToFile(VersionType * ptypeVersionHead)
{
	/* ������csv�ļ�ָ�� */
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
			/* �½��ļ��� */
			if (_mkdir(szFolderName) != 0)
			{
				printfLog("�½��ļ���:%s����\n", szFolderName);
				return NEW_FOLDER_ERROR;
			}
		}
		sprintf_s(szFolderName, ".\\Log Analysis Results\\%s", ptypeVersionNow->version);
		if (_access(szFolderName, 0) == -1)
		{
			if (_mkdir(szFolderName) != 0)
			{
				printfLog("�½��ļ���:%s����\n", szFolderName);
				return NEW_FOLDER_ERROR;
			}
		}

		/* �½�.csv�ļ� */
		sprintf_s(szFileName, "%s\\%s-%s.csv", szFolderName, ptypeVersionNow->version, ptypeVersionNow->batch);

		/* ����.csv���ļ��������ڽ�������������� */
		fopen_s(&pFileNewHand, szFileName, "w");
		if (pFileNewHand == NULL)
		{
			printfLog("���������ļ�:%s����\n", szFileName);
			return NEW_FILE_ERROR;
		}

		/* ��Bin�ṹ����������������������ļ� */
		fnBinALLPrintfToFile(pFileNewHand, &(ptypeVersionNow->typeBinData), &(ptypeVersionNow->typeBinDataUIDSame));

		/* �رմ�����.csv���ļ� */
		fclose(pFileNewHand);

		ptypeVersionNow = ptypeVersionNow->next;
	}

	return 0;
}
