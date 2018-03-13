// DrivesLine.cpp : ����Ϣ����
//


#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <windows.h>

#include "DrivesBin.h"
#include "DrivesFile.h"
#include "DrivesHash.h"
#include "DrivesLine.h"
#include "TransferFormat.h"






/*
* return void;
* input:
* function: �½�һ���нṹ��
*/
int fnLineDataNew(LineDataType ** ppLineDataType)
{
	*ppLineDataType = (LineDataType *)malloc(sizeof(LineDataType));
	if (*ppLineDataType == NULL)
	{
		printfLog("\n�½��нṹ��ʧ�ܣ�\n");
		return NEW_CONSTRUCTION_STRUCTURE_ERROR;
	}

	/* ��սṹ�� */
	memset(*ppLineDataType, 0, sizeof(LineDataType));

	return 0;
}

/*
* return void;
* input:
* function: �ͷ�һ���нṹ��
*/
int fnLineDataDelete(LineDataType ** ppLineDataType)
{
	if((*ppLineDataType)->fac_no != NULL)
		free((*ppLineDataType)->fac_no);
	if ((*ppLineDataType)->low_no != NULL)
		free((*ppLineDataType)->low_no);

	(*ppLineDataType)->fac_no = NULL;
	(*ppLineDataType)->low_no = NULL;

	if(*ppLineDataType != NULL)
		free(*ppLineDataType);

	*ppLineDataType = NULL;

	return 0;
}

/*
* return void;
* input:
* function: ���һ���нṹ���ڵ������Ƿ��д���
*/
int fnLineDataDetectionError(LineDataType * pLineDataType)
{
	int i = 0;
	static int iUIDIsAllFNumber = 0;

	if (strlen(pLineDataType->port) >= LINE_PORT_LENGTH_MAX)
	{
		printfLog("�нṹ���ڶ�ȡ����Port���ȴ��󣡴���PortΪ:%s\n", pLineDataType->port);
		return LINE_PORT_LENGTH_ERROR;
	}

	if (strlen(pLineDataType->bin) >= LINE_BIN_LENGTH_MAX)
	{
		printfLog("�нṹ���ڶ�ȡ����Bin���ȴ��󣡴���BinΪ:%s\n", pLineDataType->bin);
		return LINE_BIN_LENGTH_ERROR;
	}

	/* �ж�FacN�������Ƿ���ȷ�����Ȳ�������ܹ����ֵ�����������֮�ͣ����ҳ��Ȼ���ں��������е�FacNo���������� */
	if ((pLineDataType->fac_number > ex_ConfigurationData.iSectorNumberMaximum) || (pLineDataType->fac_number != pLineDataType->fac_no_number_length))
	{
		printfLog("�нṹ���ڶ�ȡ����FacN���ȴ��󣡴���FacN����Ϊ:%d\n", pLineDataType->fac_number);
		printfLog("�нṹ���ڶ�ȡ����FacNo���ȴ��󣡴���FacNo����Ϊ:%d\n", pLineDataType->fac_no_number_length);
		printfLog("��ʱ��Ϊ: %s\n", pLineDataType->time);
		printfLog("UIDΪ: %s\n", pLineDataType->flashUID);
		printfLog("BinΪ: %s\n\n", pLineDataType->bin);
//		return LINE_FACNO_LENGTH_ERROR;
	}

	/* �ж�LowN�������Ƿ���ȷ�����Ȳ�������ܹ����ֵ�����������֮�ͣ����ҳ��Ȼ���ں��������е�LowNo���������� */
	if ((pLineDataType->low_number > ex_ConfigurationData.iSectorNumberMaximum) || (pLineDataType->low_number != pLineDataType->low_no_number_length))
	{
		printfLog("�нṹ���ڶ�ȡ����LowN���ȴ��󣡴���LowN����Ϊ:%d\n", pLineDataType->low_number);
		printfLog("�нṹ���ڶ�ȡ����LowNo���ȴ��󣡴���LowNo����Ϊ:%d\n", pLineDataType->low_no_number_length);
		printfLog("��ʱ��Ϊ: %s\n", pLineDataType->time);
		printfLog("UIDΪ: %s\n", pLineDataType->flashUID);
		printfLog("BinΪ: %s\n\n", pLineDataType->bin);
//		return LINE_LOWNO_LENGTH_ERROR;
	}

	for (i = 0; i < pLineDataType->fac_no_number_length; ++i)
	{
		if(pLineDataType->fac_no[i] >= ex_ConfigurationData.iSectorNumberMaximum)
		{
			printfLog("�нṹ���ڶ�ȡ����FacNo�����Ŵ��󣡴���������Ϊ:%d\n", pLineDataType->fac_no[i]);
			return FACNO_INFORMATION_ERROR;
		}
	}

	for (i = 0; i < pLineDataType->low_no_number_length; ++i)
	{
		if (pLineDataType->low_no[i] >= ex_ConfigurationData.iSectorNumberMaximum)
		{
			printfLog("�нṹ���ڶ�ȡ����FacNo�����Ŵ��󣡴���������Ϊ:%d\n", pLineDataType->low_no[i]);
			return LOWNO_INFORMATION_ERROR;
		}
	}

	if (_stricmp(pLineDataType->flashUID, "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF") == 0)
	{
		/* UIDΪȫF����Bin����F0����ӡ��LOG�ļ� */
		if (_stricmp(pLineDataType->bin, "F0") != 0)
		{
			printfLog("��ȡ��UIDΪȫF����Bin��ΪF0��������\n");
			printfLog("��ʱ��Ϊ: %s\n", pLineDataType->time);
			printfLog("BinΪ: %s\n\n", pLineDataType->bin);
//			sprintf_s(pLineDataType->bin, "F1");
		}

		++iUIDIsAllFNumber;
		sprintf_s(pLineDataType->flashUID, "FFFFFFFF%dFFFFFFFF", iUIDIsAllFNumber);
	}

	return 0;
}




/*
* return void;
* input:
* function: ���½����нṹ����뵽�����ͷ
*/
int fnLineDataHeadInsert(LineDataType * ptypeHead, LineDataType * ptypeData)
{
	if (ptypeHead == NULL)
	{
		printfLog("�нṹ����������ͷ����\n");
		return -1;
	}

	if (ptypeData == NULL)
	{
		printfLog("�нṹ����������βΪ�ղ��룡\n");
		return -1;
	}

	ptypeData->next = ptypeHead->next;
	ptypeHead->next = ptypeData;

	return 0;
}

/*
* return void;
* input:
* function: ���½����нṹ����뵽�����β
*/
int fnLineDataEndInsert(LineDataType * ptypeHead, LineDataType * ptypeData)
{
	LineDataType * ptypeTemp;

	if (ptypeHead == NULL)
	{
		printfLog("�нṹ����������β����\n");
		return -1;
	}

	if (ptypeData == NULL)
	{
		printfLog("�нṹ����������βΪ�ղ��룡\n");
		return -1;
	}

	ptypeTemp = ptypeHead;
	while (ptypeTemp->next != NULL)
	{
		ptypeTemp = ptypeTemp->next;
	}

	ptypeTemp->next = ptypeData;

	return 0;
}

int fnLineUIDSameAddRepetition(LineDataType * ptypeData, char * pszBin, char * pszPathAndName)
{
	char ** ppUIDSamePathAndName = NULL;
	char szStringBuff[MAX_PATH] = { 0 };

	++ptypeData->times_of_repetition;

	ppUIDSamePathAndName = (char **)realloc(ptypeData->UID_same_path_and_name, sizeof(char *) * ptypeData->times_of_repetition);
	if (ppUIDSamePathAndName == NULL)
	{
		printfLog("realloc error!\n");
		return -1;
	}
	ptypeData->UID_same_path_and_name = ppUIDSamePathAndName;
	ptypeData->UID_same_path_and_name[ptypeData->times_of_repetition - 1] = (char *)malloc(sizeof(char) * MAX_PATH);
	if (ptypeData->UID_same_path_and_name[ptypeData->times_of_repetition - 1] == NULL)
	{
		printfLog("malloc error!\n");
		return -1;
	}

	sprintf_s(szStringBuff, "%s,%s", pszBin, pszPathAndName);

	_memccpy(ptypeData->UID_same_path_and_name[ptypeData->times_of_repetition - 1], szStringBuff, '\0', MAX_PATH);

	return 0;
}


/*
* return void;
* input:
* function: �ѵ�ǰ�ṹ����뵽������ȥ���ظ�
*/
int fnLineDataOrderInsert(LineDataType * ptypeHead, LineDataType * ptypeData)
{
	LineDataType * ptypeLineDataNow = NULL, *ptypeLineDataPrevious = NULL;
	int iUIDComparingStrcmp = 0, iTimeComparingStrcmp = 0;
	char ** ppUIDSamePathAndName = NULL;


	ptypeLineDataNow = ptypeHead->next;
	ptypeLineDataPrevious = ptypeHead;
	while (ptypeLineDataNow != NULL)
	{
		/* �ȱȽ�Hashֵ�Ƿ���ͬ���������ͬ��ֱ�Ӳ����¸�Ԫ�أ������ͬ���ٱȽ�UID��ʱ������ */
		if (ptypeLineDataNow->uiHash != ptypeData->uiHash)
		{
			ptypeLineDataPrevious = ptypeLineDataNow;
			ptypeLineDataNow = ptypeLineDataNow->next;
		}
		else
		{
			/* ��ȡUID�ַ����Ƚ�ֵ */
			iUIDComparingStrcmp = strcmp(ptypeData->flashUID, ptypeLineDataNow->flashUID);

			/* Ҫ�����UID�ȵ�ǰUID�󣬾��������λ���бȽ� */
			if (iUIDComparingStrcmp > 0)
			{
				ptypeLineDataPrevious = ptypeLineDataNow;
				ptypeLineDataNow = ptypeLineDataNow->next;
			}
			/* Ҫ�����UID�ȵ�ǰUIDС���Ͳ��뵽��ǰ�ṹ���ǰ�棬���˳���ǰ�Ĳ��� */
			else if (iUIDComparingStrcmp < 0)
			{
				ptypeLineDataPrevious->next = ptypeData;
				ptypeData->next = ptypeLineDataNow;

				fnLineUIDSameAddRepetition(ptypeData, ptypeData->bin, ptypeData->path_and_name);

				/* �����µĽṹ�壬����û���ͷžɽṹ�壬�ͷ���NEW_INSERT_ROW_STRUCTURE_SUCCEEDS */
				return NEW_INSERT_ROW_STRUCTURE_SUCCEEDS;
			}
			/* ����ͬ��UID����ô��Ҫ�ͷŵ����е�ĳһ�� */
			else
			{
				fnLineUIDSameAddRepetition(ptypeLineDataNow, ptypeData->bin, ptypeData->path_and_name);

				/* ��ȡʱ��Ƚ�ֵ */
				iTimeComparingStrcmp = strcmp(ptypeData->time, ptypeLineDataNow->time);

				/* ���ʱ����ڻ��ߵ��ڣ���ʾ��ǰ���ݽ��£��Ͱ��ββ��뵽��ǰ�ṹ���ǰ�棬���ͷŵ�ǰ���ҵ��Ľṹ�壬�˳���ǰ�Ĳ��� */
				if (iTimeComparingStrcmp >= 0)
				{
					ptypeData->times_of_repetition = ptypeLineDataNow->times_of_repetition;
					ptypeData->UID_same_path_and_name = ptypeLineDataNow->UID_same_path_and_name;

					/* ���½��Ľṹ��������� */
					ptypeLineDataPrevious->next = ptypeData;
					ptypeData->next = ptypeLineDataNow->next;

					/* �ͷŵ�ǰ���ҵ��Ľṹ�� */
					fnLineDataDelete(&ptypeLineDataNow);
					/* �²���ɹ���������ǰ��ͬUID����Ϣ�ͷţ��ͷ���RELEASE_OLD_ROW_STRUCTURE */
					return RELEASE_OLD_ROW_STRUCTURE;
				}
				else
				{
					/* �ͷ�׼��Ҫ������½��ṹ�� */
					fnLineDataDelete(&ptypeData);
					/* �²���ʧ�ܣ���ǰ��ͬUID����Ϣ���䶯���ͷ���RELEASE_NEW_ROW_STRUCTURE */
					return RELEASE_NEW_ROW_STRUCTURE;
				}
			}
		}
	}

	fnLineUIDSameAddRepetition(ptypeData, ptypeData->bin, ptypeData->path_and_name);

	/* ��ǰ������û����ͬ��UID����ָ���Ѿ����������ĩβ */
	ptypeLineDataPrevious->next = ptypeData;

	return NEW_INSERT_ROW_STRUCTURE_SUCCEEDS;
}

/*
* return void;
* input:
* function: �Ѵ��ļ���ȡ���������ݽ������нṹ��
* ������������
EC00DEFAFF4132363034303201010711	Port5	A1	4	1	2060 2061 4094 4095 ;	1208 ;
*/
int fnLineParsing(LineDataType * ptypeLineData, char * pszLineString)
{
	/* �е�ǰ��ʣ�µĳ��� */
	size_t iLineLengthRemaining = 0;
	/* ����Ϣ��ǰ�Ķ�ȡָ�룬����Ϣ���Ʒ���ָ�� */
	char * pszLineStringNow = NULL, *pszLineStringReturn = NULL;

	pszLineStringNow = pszLineString;

	/* ����֮ǰ�Ѿ��ж����еĳ��� */
#if 0
	/* �ж��еĳ��ȣ��Ƿ�С����С���ȣ������еĹ̶���ʽ */
	iLineLengthRemaining = strlen(pszLineStringNow);
	if (iLineLengthRemaining < ex_ConfigurationData.iLineLengthMin)
	{
		/* ��ӡ������ */
		printfLog("��ȡ�����г��� %d ���� %s\n", (int)iLineLengthRemaining, pszLineString);
		return LINE_DATA_LENGTH_ERROR;
	}
#endif

	/* ��������Ϣ��UID�ĳ��ȣ���ֹ�ļ�����ʱ��UID���� */
	iLineLengthRemaining = strlen(pszLineStringNow);
	pszLineStringReturn = (char *)memchr(pszLineStringNow, '\t', iLineLengthRemaining);
	if (pszLineStringReturn == NULL)
	{
		/* ��ӡ������ */
		printfLog("�ļ�UID��ȡ���� %s\n", pszLineString);
		return LINE_UID_READ_ERROR;
	}
	/* ��������Ϣ��UID�ĳ��ȣ���ֹ�ļ�����ʱ��UID������UID�����ΪLINE_UID_LENGTH_MAX */
	if((pszLineStringReturn - pszLineStringNow) > LINE_UID_LENGTH_MAX)
	{
		/* ��ӡ������ */
		printfLog("��ȡ����UID���ȳ��� %s\n", pszLineString);
		return LINE_UID_LENGTH_ERROR;
	}
	/* ��������Ϣ�е�UID��Ϣ���ṹ�壬������szLineString����Ϣ��\t���ַ����ָ�� */
	pszLineStringReturn = (char *)_memccpy(ptypeLineData->flashUID, pszLineStringNow, '\t', iLineLengthRemaining);
	/* ȥ���������ַ���\t�� */
	ptypeLineData->flashUID[strlen(ptypeLineData->flashUID) - 1] = 0;
	/* �ƶ�ָ�뵽�ַ���\t����λ�ã�����UIDΪ��ȡPort��׼�� */
	pszLineStringNow = (char *)memchr(pszLineStringNow, '\t', iLineLengthRemaining);
	/* ������\t���ַ� */
	pszLineStringNow++;


	/* ��������Ϣ��Port�ĳ��ȣ���ֹ�ļ�����ʱ��Port���� */
	iLineLengthRemaining = strlen(pszLineStringNow);
	pszLineStringReturn = (char *)memchr(pszLineStringNow, '\t', iLineLengthRemaining);
	if (pszLineStringReturn == NULL)
	{
		/* ��ӡ������ */
		printfLog("�ļ�Port��ȡ���� %s\n", pszLineString);
		return LINE_PORT_READ_ERROR;
	}
	/* ��������Ϣ��Port�ĳ��ȣ���ֹ�ļ�����ʱ��Port������Port�����ΪLINE_PORT_LENGTH_MAX */
	if ((pszLineStringReturn - pszLineStringNow) > LINE_PORT_LENGTH_MAX)
	{
		/* ��ӡ������ */
		printfLog("��ȡ����Port���ȳ��� %s\n", pszLineString);
		return LINE_PORT_LENGTH_ERROR;
	}
	/* ��������Ϣ�е�Port��Ϣ���ṹ�壬������szLineString����Ϣ��\t���ַ����ָ�� */
	pszLineStringReturn = (char *)_memccpy(ptypeLineData->port, pszLineStringNow, '\t', iLineLengthRemaining);
	/* ȥ���������ַ���\t�� */
	ptypeLineData->port[strlen(ptypeLineData->port) - 1] = 0;
	/* �ƶ�ָ�뵽�ַ���\t����λ�ã�����PortΪ��ȡBin��׼�� */
	pszLineStringNow = (char *)memchr(pszLineStringNow, '\t', iLineLengthRemaining);
	/* ������\t���ַ� */
	pszLineStringNow++;


	/* ��������Ϣ��Bin�ĳ��ȣ���ֹ�ļ�����ʱ��Bin���� */
	iLineLengthRemaining = strlen(pszLineStringNow);
	pszLineStringReturn = (char *)memchr(pszLineStringNow, '\t', iLineLengthRemaining);
	if (pszLineStringReturn == NULL)
	{
		/* ��ӡ������ */
		printfLog("�ļ�Bin��ȡ���� %s\n", pszLineString);
		return LINE_BIN_READ_ERROR;
	}
	/* Bin�����ΪLINE_BIN_LENGTH_MAX */
	if ((pszLineStringReturn - pszLineStringNow) > LINE_BIN_LENGTH_MAX)
	{
		/* ��ӡ������ */
		printfLog("��ȡ����Bin���ȳ��� %s\n", pszLineString);
		return LINE_BIN_LENGTH_ERROR;
	}
	/* ��������Ϣ�е�Bin��Ϣ���ṹ�壬������szLineString����Ϣ��\t���ַ����ָ�� */
	pszLineStringReturn = (char *)_memccpy(ptypeLineData->bin, pszLineStringNow, '\t', iLineLengthRemaining);
	/* ȥ���������ַ���\t�� */
	ptypeLineData->bin[strlen(ptypeLineData->bin) - 1] = 0;
	/* �ƶ�ָ�뵽�ַ���\t����λ�ã�����BinΪ��ȡʱ����׼�� */
	pszLineStringNow = (char *)memchr(pszLineStringNow, '\t', iLineLengthRemaining);
	/* ������\t���ַ� */
	pszLineStringNow++;


	if (ex_ConfigurationData.iLineTimeExist != 0)
	{
		/* ��������Ϣ��ʱ��ĳ��ȣ���ֹ�ļ�����ʱ��ʱ����� */
		iLineLengthRemaining = strlen(pszLineStringNow);
		pszLineStringReturn = (char *)memchr(pszLineStringNow, '\t', iLineLengthRemaining);
		if (pszLineStringReturn == NULL)
		{
			/* ��ӡ������ */
			printfLog("�ļ�ʱ���ȡ���� %s\n", pszLineString);
			return LINE_BIN_READ_ERROR;
		}
		/* Bin�����ΪLINE_BIN_LENGTH_MAX */
		if ((pszLineStringReturn - pszLineStringNow) > LINE_BIN_LENGTH_MAX)
		{
			/* ��ӡ������ */
			printfLog("��ȡ����ʱ�䳤�ȳ��� %s\n", pszLineString);
			return LINE_BIN_LENGTH_ERROR;
		}
		/* ��������Ϣ�е�ʱ����Ϣ���ṹ�壬������szLineString����Ϣ��\t���ַ����ָ�� */
		pszLineStringReturn = (char *)_memccpy(ptypeLineData->time, pszLineStringNow, '\t', iLineLengthRemaining);
		/* ȥ���������ַ���\t�� */
		ptypeLineData->time[strlen(ptypeLineData->time) - 1] = 0;
		/* �ƶ�ָ�뵽�ַ���\t����λ�ã�����ʱ��Ϊ��ȡFacN��׼�� */
		pszLineStringNow = (char *)memchr(pszLineStringNow, '\t', iLineLengthRemaining);
		/* ������\t���ַ� */
		pszLineStringNow++;
	}

	if (ex_ConfigurationData.iLineFacExist != 0)
	{
		/* ��ȡFacN�ĸ��� */
		ptypeLineData->fac_number = atoi(pszLineStringNow);
		/* �ƶ�ָ�뵽�ַ���\t����λ�ã�����FacNΪ��ȡLowN��׼�� */
		iLineLengthRemaining = strlen(pszLineStringNow);
		pszLineStringNow = (char *)memchr(pszLineStringNow, '\t', iLineLengthRemaining);
		if (pszLineStringNow == NULL)
		{
			/* ��ӡ������ */
			printfLog("��ȡFacN�ĸ������� %s\n", pszLineString);
			return LINE_FACN_READ_ERROR;
		}
		/* ������\t���ַ� */
		pszLineStringNow++;
	}


	if (ex_ConfigurationData.iLineLowExist != 0)
	{
		/* ��ȡLowN�ĸ��� */
		ptypeLineData->low_number = atoi(pszLineStringNow);
		/* �ƶ�ָ�뵽�ַ���\t����λ�ã�����LowNΪ��ȡFacNo��׼�� */
		iLineLengthRemaining = strlen(pszLineStringNow);
		pszLineStringNow = (char *)memchr(pszLineStringNow, '\t', iLineLengthRemaining);
		if (pszLineStringNow == NULL)
		{
			/* ��ӡ������ */
			printfLog("��ȡLowN�ĸ������� %s\n", pszLineString);
			return LINE_LOWN_READ_ERROR;
		}
		/* ������\t���ַ� */
		pszLineStringNow++;
	}


	if (ex_ConfigurationData.iLineFacExist != 0)
	{
		/* ��������Ϣ��FacNo�ĳ��ȣ���ֹ�ļ�����ʱ��FacNo���� */
		iLineLengthRemaining = strlen(pszLineStringNow);
		pszLineStringReturn = (char *)memchr(pszLineStringNow, ';', iLineLengthRemaining);
		if (pszLineStringReturn == NULL)
		{
			/* ��ӡ������ */
			printfLog("�ļ�FacNo��ȡ���� %s\n", pszLineString);
			return LINE_FACNO_READ_ERROR;
		}
		/* ��������Ϣ��FacNo�ĳ��ȣ���ֹ�ļ�����ʱ��FacNo������FacNo�����Ϊ LINE_STRING_LENGTH */
		if ((pszLineStringReturn - pszLineStringNow) > ex_ConfigurationData.iFacNoLengthMax)
		{
			/* ��ӡ������ */
			printfLog("��ȡ����FacNo���ȳ��� %s\n", pszLineString);
			return LINE_FACNO_LENGTH_ERROR;
		}
		/* ���ַ����е�������ת������ֵ�洢���нṹ���е�fac_no */
		if (fnLineNoConversionToNumber(&ptypeLineData->fac_no, &ptypeLineData->fac_no_number_length, pszLineStringNow) != 0)
		{
			return FACNO_ALLOCATION_MEMORY_ERROR;
		}
		/* �ƶ�ָ�뵽�ַ���;����λ�ã�����FacNoΪ��ȡLowNo��׼�� */
		pszLineStringNow = (char *)memchr(pszLineStringNow, ';', iLineLengthRemaining);
		/* ������;���ַ� */
		pszLineStringNow++;
		/* ������\t���ַ� */
		pszLineStringNow++;
	}


	if (ex_ConfigurationData.iLineLowExist != 0)
	{
		/* ��������Ϣ��LowNo�ĳ��ȣ���ֹ�ļ�����ʱ��LowNo���� */
		iLineLengthRemaining = strlen(pszLineStringNow);
		pszLineStringReturn = (char *)memchr(pszLineStringNow, ';', iLineLengthRemaining);
		if (pszLineStringReturn == NULL)
		{
			/* ��ӡ������ */
			printfLog("�ļ�LowNo��ȡ���� %s\n", pszLineString);
			return LINE_LOWNO_READ_ERROR;
		}
		/* ��������Ϣ��LowNo�ĳ��ȣ���ֹ�ļ�����ʱ��LowNo������LowNo�����Ϊ LINE_STRING_LENGTH */
		if ((pszLineStringReturn - pszLineStringNow) > ex_ConfigurationData.iLowNoLengthMax)
		{
			/* ��ӡ������ */
			printfLog("��ȡ����LowNo���ȳ��� %s\n", pszLineString);
			return LINE_LOWNO_LENGTH_ERROR;
		}

		/* ���ַ����е�������ת������ֵ�洢���нṹ���е�low_no */
		if (fnLineNoConversionToNumber(&ptypeLineData->low_no, &ptypeLineData->low_no_number_length, pszLineStringNow) != 0)
		{
			return LOWNO_ALLOCATION_MEMORY_ERROR;
		}
	}

	return 0;
}

/*
* return void;
* input:
* function: ��FacNo����LowNo�е��������ַ�����ʽת����int��ֵ
*/
int fnLineNoConversionToNumber(int ** ppiNumberBuff, int * piNumberLength, char * pszNoString)
{
	/* ��ǰ��ȡ���������� */
	int iCumulativeNumber = 0;
	int * piNumberNow = NULL, * piNumbersReturn = NULL;
	char * pszNoStringReturn = NULL;


	piNumberNow = *ppiNumberBuff;

	pszNoStringReturn = (char *)strchr(pszNoString, ';');
	if (pszNoString == pszNoStringReturn)
		return 0;

	/* ���ҵ��ַ����ո�λ�� */
	pszNoStringReturn = (char *)strchr(pszNoString, ' ');
	while (pszNoStringReturn != NULL)
	{
		/* ���ҵ�����������1 */
		++iCumulativeNumber;

		/* �����ڴ�洢 */
		piNumbersReturn = (int *)realloc(piNumberNow, sizeof(int) * iCumulativeNumber);
		if (piNumbersReturn == NULL)
		{
			/* �ͷ�ԭָ�� */
			free(piNumberNow);
			printfLog("ΪNo�����ڴ����\n");
			return NO_ALLOCATION_MEMORY_ERROR;
		}

		piNumberNow = piNumbersReturn;

		/* ���ַ���ת������ֵ */
		piNumberNow[iCumulativeNumber - 1] = atoi(pszNoString);

		/* ������ǰ���ҵ����ַ����ո� */
		pszNoString = pszNoStringReturn + 1;
		/* ���ָ�뵽���ַ���;����ʾ�Ѿ�����No�ַ�����ĩβ */
		if (*pszNoString == ';')
			break;

		/* ���ҵ��ַ����ո�λ�� */
		pszNoStringReturn = (char *)strchr(pszNoString, ' ');
	}

	*ppiNumberBuff = piNumberNow;
	*piNumberLength = iCumulativeNumber;

	return 0;
}

