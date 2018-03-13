// DrivesHash.cpp : ���ļ����д���
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
* function: �����ļ��������ݶ�ȡ
*/
int fnFileConfigurationGetData(int * iData, char * pszStringBuff, char * pszCategory)
{
	/* ����Ϣ��ǰ�Ķ�ȡָ�룬����Ϣ���Ʒ���ָ�� */
	char * pszLineStringNow = NULL, *pszLineStringsReturn = NULL;
	/* �е�ǰ�ĳ��� */
	size_t iLineLength = 0;


	pszLineStringNow = pszStringBuff;

	/* �ж��еĳ��ȣ������еĹ̶���ʽ���е���󳤶�Ϊ64 */
	iLineLength = strlen(pszStringBuff);
	if (iLineLength > 64)
	{
		return LINE_DATA_LENGTH_ERROR;
	}
	/* �жϵ�ǰ���ǲ��ǡ�pszCategory������ */
	pszLineStringsReturn = strstr(pszStringBuff, pszCategory);
	if (pszLineStringsReturn == NULL)
	{
		return -1;
	}

	/* �ƶ�ָ�뵽�ַ���=����λ�ã�Ϊ��ȡ������׼�� */
	pszLineStringNow = (char *)memchr(pszLineStringNow, '=', iLineLength);
	if (pszLineStringNow == NULL)
	{
		return -1;
	}
	/* �����ַ���=�����ַ����ո� */
	pszLineStringNow += 2;
	/* ��ȡ�����С */
	*iData = atoi(pszLineStringNow);

	return 0;
}

/*
* return void;
* input:
* function: �����ļ��Ķ�ȡ
*/
int fnFileConfigurationRead(ConfigurationType * ptypeConfiguration, FILE * pFileHand)
{
	/* �����ļ���������ݻ��� */
	char szLineStrings[1024] = { 0 };


	/* ���ļ���ȡһ������ */
	fgets(szLineStrings, sizeof(szLineStrings), pFileHand);
	if (fnFileConfigurationGetData(&ptypeConfiguration->iIntervalSize, szLineStrings, "�����С") != 0)
	{
		printfLog("�����С��ȡ����%s:\n", szLineStrings);
		return INTERVAL_SIZE_READS_ERROR;
	}

	/* ���㻺�� */
	memset(szLineStrings, 0, sizeof(szLineStrings));
	/* ���ļ���ȡһ������ */
	fgets(szLineStrings, sizeof(szLineStrings), pFileHand);
	if (fnFileConfigurationGetData(&ptypeConfiguration->iSectorNumberMaximum, szLineStrings, "���������ֵ") != 0)
	{
		printfLog("���������ֵ��ȡ����%s:\n", szLineStrings);
		return MAXIMUM_READ_SECTOR_NUMBER_ERROR;
	}

	/* ���㻺�� */
	memset(szLineStrings, 0, sizeof(szLineStrings));
	/* ���ļ���ȡһ������ */
	fgets(szLineStrings, sizeof(szLineStrings), pFileHand);
	if (fnFileConfigurationGetData(&ptypeConfiguration->iHashTableSize, szLineStrings, "��ϣ���С") != 0)
	{
		printfLog("��ϣ���С��ȡ����%s:\n", szLineStrings);
		return HASH_TABLE_SIZE_READ_ERROR;
	}

	/* ���㻺�� */
	memset(szLineStrings, 0, sizeof(szLineStrings));
	/* ���ļ���ȡһ������ */
	fgets(szLineStrings, sizeof(szLineStrings), pFileHand);
	if (fnFileConfigurationGetData(&ptypeConfiguration->iFacNoLengthMax, szLineStrings, "FacNo�ַ�����󳤶�") != 0)
	{
		printfLog("FacNo�ַ�����󳤶ȶ�ȡ����%s:\n", szLineStrings);
		return MAXIMUM_LENGTH_READ_FACNO_STRING_ERROR;
	}

	/* ���㻺�� */
	memset(szLineStrings, 0, sizeof(szLineStrings));
	/* ���ļ���ȡһ������ */
	fgets(szLineStrings, sizeof(szLineStrings), pFileHand);
	if (fnFileConfigurationGetData(&ptypeConfiguration->iLowNoLengthMax, szLineStrings, "LowNo�ַ�����󳤶�") != 0)
	{
		printfLog("LowNo�ַ�����󳤶ȶ�ȡ����%s:\n", szLineStrings);
		return MAXIMUM_LENGTH_READ_LOWNO_STRING_ERROR;
	}

	/* ���㻺�� */
	memset(szLineStrings, 0, sizeof(szLineStrings));
	/* ���ļ���ȡһ������ */
	fgets(szLineStrings, sizeof(szLineStrings), pFileHand);
	if (fnFileConfigurationGetData(&ptypeConfiguration->iLineLengthMax, szLineStrings, "�ļ��������ַ�����󳤶�") != 0)
	{
		printfLog("�ļ��������ַ�����󳤶ȶ�ȡ����%s:\n", szLineStrings);
		return FILE_ROW_DATA_STRING_MAXIMUM_LENGTH_READ_ERROR;
	}

	/* ���㻺�� */
	memset(szLineStrings, 0, sizeof(szLineStrings));
	/* ���ļ���ȡһ������ */
	fgets(szLineStrings, sizeof(szLineStrings), pFileHand);
	if (fnFileConfigurationGetData(&ptypeConfiguration->iLineLengthMin, szLineStrings, "�ļ��������ַ�����С����") != 0)
	{
		printfLog("�ļ��������ַ�����С���ȣ�%s:\n", szLineStrings);
		return FILE_ROW_DATA_STRING_MINIMUM_LENGTH_READ_ERROR;
	}

	/* ���㻺�� */
	memset(szLineStrings, 0, sizeof(szLineStrings));
	/* ���ļ���ȡһ������ */
	fgets(szLineStrings, sizeof(szLineStrings), pFileHand);
	if (fnFileConfigurationGetData(&ptypeConfiguration->iFileNameVersionExist, szLineStrings, "�ļ������Ƿ���ڰ汾����Ϣ") != 0)
	{
		printfLog("�ļ������Ƿ���ڰ汾����Ϣ��%s:\n", szLineStrings);
		return FILE_ROW_DATA_STRING_MINIMUM_LENGTH_READ_ERROR;
	}

	/* ���㻺�� */
	memset(szLineStrings, 0, sizeof(szLineStrings));
	/* ���ļ���ȡһ������ */
	fgets(szLineStrings, sizeof(szLineStrings), pFileHand);
	if (fnFileConfigurationGetData(&ptypeConfiguration->iFileNameBatch, szLineStrings, "�ļ������Ƿ����������Ϣ") != 0)
	{
		printfLog("�ļ������Ƿ����������Ϣ��%s:\n", szLineStrings);
		return FILE_ROW_DATA_STRING_MINIMUM_LENGTH_READ_ERROR;
	}

	/* ���㻺�� */
	memset(szLineStrings, 0, sizeof(szLineStrings));
	/* ���ļ���ȡһ������ */
	fgets(szLineStrings, sizeof(szLineStrings), pFileHand);
	if (fnFileConfigurationGetData(&ptypeConfiguration->iLineFacExist, szLineStrings, "����Ϣ���Ƿ����Fac��Ϣ") != 0)
	{
		printfLog("����Ϣ���Ƿ����Fac��Ϣ��%s:\n", szLineStrings);
		return FILE_ROW_DATA_STRING_MINIMUM_LENGTH_READ_ERROR;
	}

	/* ���㻺�� */
	memset(szLineStrings, 0, sizeof(szLineStrings));
	/* ���ļ���ȡһ������ */
	fgets(szLineStrings, sizeof(szLineStrings), pFileHand);
	if (fnFileConfigurationGetData(&ptypeConfiguration->iLineLowExist, szLineStrings, "����Ϣ���Ƿ����Low��Ϣ") != 0)
	{
		printfLog("����Ϣ���Ƿ����Low��Ϣ��%s:\n", szLineStrings);
		return FILE_ROW_DATA_STRING_MINIMUM_LENGTH_READ_ERROR;
	}

	/* ���㻺�� */
	memset(szLineStrings, 0, sizeof(szLineStrings));
	/* ���ļ���ȡһ������ */
	fgets(szLineStrings, sizeof(szLineStrings), pFileHand);
	if (fnFileConfigurationGetData(&ptypeConfiguration->iLineTimeExist, szLineStrings, "����Ϣ���Ƿ����ʱ����Ϣ") != 0)
	{
		printfLog("����Ϣ���Ƿ����ʱ����Ϣ��%s:\n", szLineStrings);
		return FILE_ROW_DATA_STRING_MINIMUM_LENGTH_READ_ERROR;
	}

	return 0;
}

/*
* return void;
* input:
* function: �����ļ���д�루�ָ�Ĭ�����ã�
*/
int fnFileConfigurationWrite(ConfigurationType * ptypeConfiguration, FILE * pFileHand)
{
	fprintf(pFileHand, "�����С = %d\n", ptypeConfiguration->iIntervalSize);
	fprintf(pFileHand, "���������ֵ = %d\n", ptypeConfiguration->iSectorNumberMaximum);
	fprintf(pFileHand, "��ϣ���С = %d\n", ptypeConfiguration->iHashTableSize);
	fprintf(pFileHand, "FacNo�ַ�����󳤶� = %d\n", ptypeConfiguration->iFacNoLengthMax);
	fprintf(pFileHand, "LowNo�ַ�����󳤶� = %d\n", ptypeConfiguration->iLowNoLengthMax);
	fprintf(pFileHand, "�ļ��������ַ�����󳤶� = %d\n", ptypeConfiguration->iLineLengthMax);
	fprintf(pFileHand, "�ļ��������ַ�����С���� = %d\n", ptypeConfiguration->iLineLengthMin);

	fprintf(pFileHand, "�ļ������Ƿ���ڰ汾����Ϣ = %d\n", ptypeConfiguration->iFileNameVersionExist);
	fprintf(pFileHand, "�ļ������Ƿ����������Ϣ = %d\n", ptypeConfiguration->iFileNameBatch);
	fprintf(pFileHand, "����Ϣ���Ƿ����Fac��Ϣ = %d\n", ptypeConfiguration->iLineFacExist);
	fprintf(pFileHand, "����Ϣ���Ƿ����Low��Ϣ = %d\n", ptypeConfiguration->iLineLowExist);
	fprintf(pFileHand, "����Ϣ���Ƿ����ʱ����Ϣ = %d\n", ptypeConfiguration->iLineTimeExist);

	fprintf(pFileHand, "\nע�⣺\n");
	fprintf(pFileHand, "1����ϣ��Ĵ�С�����ó�һ��������\n");
	fprintf(pFileHand, "2������������б��������ֶ�ɾ����ǰ�����ļ����ٴ����г���\n");

	return 0;
}

/*
* return void;
* input:
* function: ���ļ�����ȡ���ļ���������ʱ��
* �ļ���������
123-2017-11-27 183343.txt
*/
int fnFileGetTimeAndDate(char * pszFileTime, char * pszFileDate, char * pszBatch, char * pszVersion, char * pszRootFolderName, char * pszFilePathAndName)
{
	size_t i = 0, iFileTimeLength = 0, iFileDataLength = 0, iFileBatchLength = 0, iFileVersionLength = 0, iFileRootFolderLength = 0;
	/* �ļ���ʣ�೤�� */
	size_t iFileNameRemainingLengthNow = 0;
	char * pszFileNameNow = NULL, *pszFileNameReturn = NULL;
	char szVersionNumber[32] = { 0 };


	pszFileNameNow = pszFilePathAndName;

	/* ��ȡ�ļ����ĳ��� */
	iFileNameRemainingLengthNow = strlen(pszFileNameNow);
	/* ����ļ����ĳ����Ƿ���ȷ���涨���ȴ��ڵ���14��С�ڵ���128 */
	if ((iFileNameRemainingLengthNow < 14) || (iFileNameRemainingLengthNow > MAX_PATH))
	{
		printfLog("FILE_NAME_LENGTH_ERROR is :%d\n", iFileNameRemainingLengthNow);
		return FILE_NAME_LENGTH_ERROR;
	}

	/* ����ļ�����û�а����汾����Ϣ���ͻ�ȡ�ļ���Ŀ¼�ļ�������Ϊ�汾�� */
	if (ex_ConfigurationData.iFileNameVersionExist == 0)
	{
		/* ���ļ�����ȡ���ļ��������Ե�һ���ַ���\����ʼ */
		pszFileNameReturn = (char *)memchr(pszFileNameNow, '\\', iFileNameRemainingLengthNow);
		if (pszFileNameReturn == NULL)
		{
			return FILE_FOLDER_READ_ERROR;
		}
		else
		{
			/* �����ַ���\�� */
			pszFileNameNow = pszFileNameReturn + 1;
			/* ���ļ�����ȡ���ļ����������ַ���\������ */
			pszFileNameReturn = (char *)_memccpy(pszRootFolderName, pszFileNameNow, '\\', iFileNameRemainingLengthNow);
			if (pszFileNameReturn == NULL)
			{
				memset(pszRootFolderName, 0, sizeof(pszRootFolderName));
			}
			/* ��ȡ�ļ����ڵĳ��� */
			iFileRootFolderLength = strlen(pszRootFolderName);
			if (iFileRootFolderLength > 1)
			{
				/* ������һ���ַ���\��ȥ�� */
				--iFileRootFolderLength;
				pszRootFolderName[iFileRootFolderLength] = 0;
			}
		}
	}



	/* ��ȡ�ļ�����ʼ����λ�ã����һ����\���������Ӻ���ǰ�� */
	pszFileNameReturn = strrchr(pszFilePathAndName, '\\');
	if (pszFileNameReturn == NULL)
	{
		printfLog("�ļ�·��������û���ַ���\\���� �ļ�·����Ϊ:%s\n", pszFilePathAndName);
		return -1;
	}
	/* �����ַ���\�� */
	++pszFileNameReturn;
	pszFileNameNow = pszFileNameReturn;

	if (ex_ConfigurationData.iFileNameVersionExist != 0)
	{
		/* ���ļ�����ȡ�汾�ţ����ַ���-����β */
		pszFileNameReturn = (char *)_memccpy(pszVersion, pszFileNameNow, '-', iFileNameRemainingLengthNow);
		if (pszFileNameReturn == NULL)
		{
//			printfLog("FILE_BATCH_TIME_ERROR\n");
			return FILE_BATCH_TIME_ERROR;
		}
		/* �ƶ�ָ�뵽�ַ���-����λ�ã���������Ϊ��ȡ������׼�� */
		pszFileNameNow = (char *)memchr(pszFileNameNow, '-', iFileNameRemainingLengthNow);
		/* �����ַ���-�� */
		pszFileNameNow++;

		/* �ļ�������ȥ���ѻ�ȡ�İ汾�ų��� */
		iFileNameRemainingLengthNow -= strlen(pszVersion);
		/* ���ļ�����ȡ�汾�ţ����ַ���-����β */
		pszFileNameReturn = (char *)_memccpy(szVersionNumber, pszFileNameNow, '-', iFileNameRemainingLengthNow);
		if (pszFileNameReturn == NULL)
		{
//			printfLog("FILE_BATCH_TIME_ERROR\n");
			return FILE_BATCH_TIME_ERROR;
		}
		/* �ƶ�ָ�뵽�ַ���-����λ�ã���������Ϊ��ȡ������׼�� */
		pszFileNameNow = (char *)memchr(pszFileNameNow, '-', iFileNameRemainingLengthNow);
		/* �����ַ���-�� */
		pszFileNameNow++;

		/* �汾����� */
		strcat_s(pszVersion, 32, szVersionNumber);
		/* ��ȡ�ļ����ڵĳ��� */
		iFileVersionLength = strlen(pszVersion);
		if (iFileVersionLength > 1)
		{
			/* ������һ���ַ����ո�ȥ�� */
			--iFileVersionLength;
			pszVersion[iFileVersionLength] = 0;
		}
	}

	if (ex_ConfigurationData.iFileNameBatch != 0)
	{
		/* �ļ�������ȥ���ѻ�ȡ�İ汾�ų��� */
		iFileNameRemainingLengthNow -= strlen(szVersionNumber);
		/* ���ļ�����ȡ���ţ����ַ���-����β */
		pszFileNameReturn = (char *)_memccpy(pszBatch, pszFileNameNow, '-', iFileNameRemainingLengthNow);
		if (pszFileNameReturn == NULL)
		{
//			printfLog("FILE_BATCH_TIME_ERROR\n");
			return FILE_BATCH_TIME_ERROR;
		}
		/* ��ȡ�ļ����ڵĳ��� */
		iFileBatchLength = strlen(pszBatch);
		if (iFileBatchLength > 1)
		{
			/* ������һ���ַ����ո�ȥ�� */
			--iFileBatchLength;
			pszBatch[iFileBatchLength] = 0;
		}
		/* �ƶ�ָ�뵽�ַ���-����λ�ã���������Ϊ��ȡ������׼�� */
		pszFileNameNow = (char *)memchr(pszFileNameNow, '-', iFileNameRemainingLengthNow);
		/* �����ַ���-�� */
		pszFileNameNow++;
	}


	/* �ļ�������ȥ���ѻ�ȡ�����ų��� */
	iFileNameRemainingLengthNow -= strlen(pszBatch);
	/* ���ļ�����ȡ���ڣ����ַ����ո񡯽�β */
	pszFileNameReturn = (char *)_memccpy(pszFileDate, pszFileNameNow, ' ', iFileNameRemainingLengthNow);
	if (pszFileNameReturn == NULL)
	{
//		printfLog("FILE_NAME_DATA_ERROR\n");
		return FILE_NAME_DATA_ERROR;
	}
	/* ��ȡ�ļ����ڵĳ��� */
	iFileDataLength = strlen(pszFileDate);
	if (iFileDataLength > 1)
	{
		/* ������һ���ַ����ո�ȥ�� */
		--iFileDataLength;
		pszFileDate[iFileDataLength] = 0;
		/* �жϻ�ȡ�����ļ������Ƿ���� */
		for (i = 0; i < iFileDataLength; ++i)
		{
			if (((pszFileDate[i] >= '0' && pszFileDate[i] <= '9') || pszFileDate[i] == '-') == 0)
			{
//				printfLog("FILE_NAME_DATA_ERROR\n");
				return FILE_NAME_DATA_ERROR;
			}
		}
	}
	/* �ƶ�ָ�뵽�ַ����ո񡯵�λ�ã���������Ϊ��ȡʱ����׼�� */
	pszFileNameNow = (char *)memchr(pszFileNameNow, ' ', iFileNameRemainingLengthNow);
	/* �����ո��ַ� */
	pszFileNameNow++;

	/* �ļ�������ȥ���ѻ�ȡ�����ڳ��� */
	iFileNameRemainingLengthNow -= strlen(pszFileDate);
	/* ���ļ�����ȡʱ�䣬���ַ���.����β */
	pszFileNameReturn = (char *)_memccpy(pszFileTime, pszFileNameNow, '.', iFileNameRemainingLengthNow);
	if (pszFileNameReturn == NULL)
	{
//		printfLog("FILE_NAME_TIME_ERROR\n");
		return FILE_NAME_TIME_ERROR;
	}
	/* ��ȡ�ļ�ʱ��ĳ��� */
	iFileTimeLength = strlen(pszFileTime);
	if (iFileTimeLength > 1)
	{
		/* ������һ���ַ����ո�ȥ�� */
		--iFileTimeLength;
		pszFileTime[iFileTimeLength] = 0;
		/* ���ļ�����ȡ���ڣ����ַ����ո񡯽�β */
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
* function: �ļ���ȡ
* �ļ����������� 
FlashUID	Port	Bin	time	FacN	lowN	FacNo	LowNo
Start Log Num Total:0,Pass:0 ,Fail:0

start
EC00DEFAFF4132363034303201010711	Port5	A0	[2018-02-13 12:20:51:0148]	4	0	2060 2061 4094 4095 ;	;
EC00F698521930383434303201010711	Port7	A0	[2018-02-13 12:22:23:0258]	4	0	2060 2061 4094 4095 ;	;
End Log Num Total:1,Pass:1 ,Fail:0 
*/
int fnFileRead(FILE * pFileHand, char * pszFileTime, char * szFilePathAndName, VersionType * ptypeVersionHand)
{
	/* �����ݶ�ȡ���� */
	char * pszLineStrings;
	/* �����ݶ�ȡ�����¼���־ */
	int iLineParsingFlag = 0;
	/* �½����нṹ��ָ�� */
	LineDataType * ptypeLineDataNew = NULL;


	/* Ϊ�������½�һ���ڴ滺�� */
	pszLineStrings = (char *)malloc(sizeof(char) * ex_ConfigurationData.iLineLengthMax);

	/* һֱ�����ļ���ĩβ */
	while (feof(pFileHand) == 0)
	{
		/* ���㻺�� */
		memset(pszLineStrings, 0, sizeof(char) * ex_ConfigurationData.iLineLengthMax);
		/* ���ļ���ȡһ������ */
		fgets(pszLineStrings, sizeof(char) * ex_ConfigurationData.iLineLengthMax, pFileHand);

		/* ��ȡ��������С����С���ȣ���ֱ���������ٶ�ȡ��һ�� */
		if (strlen(pszLineStrings) < ex_ConfigurationData.iLineLengthMin)
		{
			/* ��ȡ��һ�� */
			continue;
		}

		/* �½�һ���нṹ�壬�����洢��ǰ�������е���Ϣ */
		if (fnLineDataNew(&ptypeLineDataNew) != 0)
		{
			printfLog("�½��нṹ��ʧ�ܣ�\n");
			return NEW_CONSTRUCTION_STRUCTURE_ERROR;
		}

		/* ����ǰ��ȡ��һ������ */
		iLineParsingFlag = fnLineParsing(ptypeLineDataNew, pszLineStrings);
		if ((iLineParsingFlag == FACNO_ALLOCATION_MEMORY_ERROR) || (iLineParsingFlag == LOWNO_ALLOCATION_MEMORY_ERROR))
		{
			printfLog("����Ϣ��ȡ�����ڴ�ʧ�ܣ�\n");
			/* �ͷŵ�ǰ�½��Ľṹ�� */
			fnLineDataDelete(&ptypeLineDataNew);
			/* �ڴ����ʧ�ܣ����˳���ǰ��ȡ */
			return iLineParsingFlag;
		}
		else if (iLineParsingFlag != 0)
		{
			printfLog("����Ϣ��ȡ����\n");
			/* �ͷŵ�ǰ�½��Ľṹ�� */
			fnLineDataDelete(&ptypeLineDataNew);
			/* ��ȡ��һ�� */
			continue;
		}
		
		/* ��Ӱ汾�š��������ļ�·��������Ϣ */
		strcpy_s(ptypeLineDataNew->version, ptypeVersionHand->version);
		strcpy_s(ptypeLineDataNew->batch, ptypeVersionHand->batch);
		strcpy_s(ptypeLineDataNew->path_and_name, szFilePathAndName);
		/* ����ļ���������Ϣ��û��ʱ����Ϣ���Ͱ��ļ�����ʱ��ӽ��� */
		if (ex_ConfigurationData.iLineTimeExist == 0)
		{
			strcpy_s(ptypeLineDataNew->time, pszFileTime);
		}

		/* ����ȡ�ĵ�ǰ�нṹ���ڵ������Ƿ��д��� */
		if (fnLineDataDetectionError(ptypeLineDataNew) != 0)
		{
			printfLog("��ǰ��ȡ���������д��󣡲����뵽����\n");
			printfLog("��ʱ��Ϊ: %s\n", ptypeLineDataNew->time);
			printfLog("UIDΪ: %s\n", ptypeLineDataNew->flashUID);
			printfLog("BinΪ: %s\n", ptypeLineDataNew->bin);
			printfLog("\n");

			/* �ͷŵ�ǰ�½��Ľṹ�� */
			fnLineDataDelete(&ptypeLineDataNew);
			/* ��ȡ��һ�� */
			continue;
		}

		/* ��ȡBKDR��ϣֵ */
		ptypeLineDataNew->uiHash = fnHashGet32(ptypeLineDataNew->flashUID, HASH_BKDR);

		/* ���нṹ����뵽��ϣ�� */
		if (fnHashLineInsert(ptypeVersionHand, ptypeLineDataNew, ex_ConfigurationData.iHashTableSize) != 0)
		{
			printfLog("�нṹ����뵽��ϣ��ʧ�ܣ�\n");

			/* ��ȡ��һ�� */
			continue;
		}
	} 
	
	/* �ͷŵ�ǰ�л��� */
	free(pszLineStrings);

	return 0;
}

/*
* return void;
* input:
* function: �жϵ�ǰ�ļ��Ƿ���txt�ļ��������д򿪣������ݷ����нṹ��
*/
int fnFileOpenAndRead(char * pszFileName, char * pszFilePath)
{
	FILE * pFile = NULL;
	VersionType * ptypeVersion = NULL;
	/* �ļ�·�������ֻ��桢���ļ����� */
	char szFilePathAndName[MAX_PATH] = { 0 }, szRootFolderName[MAX_PATH] = { 0 };
	/* �ļ����͡��ļ����š��ļ��汾�� */
	char szFileType[MAX_PATH] = { 0 }, szFileBatch[MAX_PATH] = { 0 }, szFileVersion[MAX_PATH] = { 0 };
	/* �ļ����ڻ��棻 �ļ�ʱ�仺�� */
	char szFileDate[MAX_PATH] = { 0 }, szFileTime[MAX_PATH] = { 0 };


	/* ��·�����ļ������ϵ�szFilePathAndName */
	sprintf_s(szFilePathAndName, "%s\\%s", pszFilePath, pszFileName);

	/* ��ȡ�ļ����͵�szFileType */
	fnFileGetType(szFileType, pszFileName);

	/* �жϵ�ǰ�ļ��Ƿ���txt�ļ��������ִ�Сд */
	if (_stricmp(szFileType, "txt") == 0)
	{
		/* �жϵ�ǰ�ļ����Ƿ���ȷ�������ļ�����ȡ�ļ�������ʱ�� */
		if (fnFileGetTimeAndDate(szFileTime, szFileDate, szFileBatch, szFileVersion, szRootFolderName, szFilePathAndName) != 0)
		{
			return FILE_NAME_ERROR;
		}

		/* ʱ�ں����ڽ��кϲ� */
		strcat_s(szFileDate, " ");
		strcat_s(szFileDate, szFileTime);

		/* ���ļ� */
		fopen_s(&pFile, szFilePathAndName, "r");
		if (pFile == NULL)
		{
			printfLog("�ļ���ʧ��!:%s\n", szFilePathAndName);
			return FILE_READ_OPEN_ERROR;
		}

		/* ��Ļ��ӡ��ǰ�򿪵��ļ� */
		printfLog("%s\n", szFilePathAndName);

		/* ����ļ�����û�а����汾����Ϣ���ͰѸ�Ŀ¼�ļ�������Ϊ�汾�� */
		if (ex_ConfigurationData.iFileNameVersionExist == 0)
		{
			strcpy_s(szFileVersion, szRootFolderName);
		}
		/* ����ļ�����û�а���������Ϣ�����ֶ���һ���̶����ַ�����Ϊ���� */
		if (ex_ConfigurationData.iFileNameBatch == 0)
		{
			strcpy_s(szFileBatch, "");
		}

		/* �ɰ汾�ź�����ȥ�½�һ����Ӧ�İ汾�ṹ�� */
		if (fnVersionGetHead(&ptypeVersion, szFileVersion, szFileBatch) != 0)
		{
			printfLog("�汾�Žṹ���½�����:%s-%s\n", szFileVersion, szFileBatch);
			return -1;
		}

		/* ���ļ�ָ�롢�ļ����ڡ��ļ�ʱ�䴫���ļ���ȡ���� */
		if (fnFileRead(pFile, szFileDate, szFilePathAndName, ptypeVersion) != 0)
		{
			printfLog("�ļ�����Ϣ��ȡ����\n");
			fclose(pFile);
			return FILE_CONTENT_READS_ERROR;
		}

		/* �ر��ļ� */
		fclose(pFile);
		return 0;
	}

	/* �����ǰ�ļ�����txt�ļ����ͷ���1 */
	return 1;
}

/*
* return void;
* input:
* function: Ƕ�ײ��ҵ�ǰ�ļ����µ������ļ�
*/
int fnFileScan(char * pszFilePath)
{
	/* �������ļ������ļ�·�� */
	char szPathNow[MAX_PATH] = { 0 }, szPathNext[MAX_PATH] = { 0 };
	/* ��ǰ�򿪵��ļ��л��ļ���Ϣ */
	WIN32_FIND_DATA FindFileData;
	/* ��ǰ�򿪵��ļ��л��ļ���� */
	HANDLE hFind;

	/* �Ȱѵ�ǰ·�����ƣ���ǰ�ļ����µ�һ�β���ʱ���������ļ������Լ��ϡ�*.*�� */
	sprintf_s(szPathNow, "%s\\*.*", pszFilePath);
	/* ��ǰ�ļ����µĵ�һ�δ򿪣��˺������ FindNextFile ����ʹ�� */
	hFind = FindFirstFile(szPathNow, &FindFileData);

	/* ��ǰ·�����Ѿ�û���ļ����ߴ��� */
	if (INVALID_HANDLE_VALUE == hFind)
		return FILE_PATH_WRONG;

	while (TRUE)
	{
		/* �ж��Ƿ�Ϊ�ļ��� */
		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			/* ÿ���ļ����»��У�"."��".."���ļ�����Ϣ����ʾ��ǰ�ļ������ϲ��ļ��С��������ǰ����˵���������ô�ͱ�ʾ���ҵ�������Ч�ļ��� */
			if((strcmp(FindFileData.cFileName, ".") != 0) && (strcmp(FindFileData.cFileName, "..") != 0))
			{
				/* �ѵ�ǰ·�����¸��ļ��е����ָ��Ƶ��¸�·�������� */
				sprintf_s(szPathNext, "%s\\%s", pszFilePath, FindFileData.cFileName);
				/* �ݹ���� */
				fnFileScan(szPathNext);
			}
		}
		/* ��ǰ�򿪵����ļ� */
		else
		{
			/* ���ļ��������� */
			fnFileOpenAndRead((char *)FindFileData.cFileName, pszFilePath);
		}

		/* �ж��Ƿ��Ѿ����ﵱǰ�ļ����µ����һ���ļ�������ǣ����˳���ǰǶ�׵��ļ��� */
		if (!FindNextFile(hFind, &FindFileData))
			break;
	}

	return 0;
}


/*
* return void;
* input:
* function: ���ļ�����ȡ�ļ�������
*/
int fnFileGetType(char * pszFileType, char * pszFileName)
{
	char * pszFileNameReturnPointer = NULL;


	/* ��ȡ�ļ������Ϳ�ʼ����λ�ã����һ����.���������Ӻ���ǰ�� */
	pszFileNameReturnPointer = strrchr(pszFileName, '.');
	if (pszFileNameReturnPointer == NULL)
	{
		printfLog("�ļ�������û���ַ���.���� �ļ���Ϊ:%s\n", pszFileName);
		return -1;
	}
	/* �����ַ���.�� */
	++pszFileNameReturnPointer;

	/* �����ļ������� */
	while (*pszFileNameReturnPointer != 0)
	{
		*pszFileType++ = *pszFileNameReturnPointer++;
	}

	/* �������'\0'����ʾ�ַ������� */
	*pszFileType = 0;

	return 0;
}
