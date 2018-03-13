// DrivesHash.cpp : ��ϣ�����ļ�
//


#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <windows.h>

#include "DrivesBin.h"
#include "DrivesFile.h"
#include "DrivesLine.h"
#include "TransferFormat.h"
#include "DrivesVersion.h"
#include "DrivesHash.h"


/* ��ϣ�� */
LineDataType * ex_typeLineDataHashTable = NULL;





unsigned int SDBMHash(char *str)
{
	unsigned int hash = 0;

	while (*str)
	{
		hash = (*str++) + (hash << 6) + (hash << 16) - hash;
	}

	return (hash & 0x7FFFFFFF);
}

unsigned int RSHash(char *str)
{
	unsigned int b = 378551;
	unsigned int a = 63689;
	unsigned int hash = 0;

	while (*str)
	{
		hash = hash * a + (*str++);
		a *= b;
	}

	return (hash & 0x7FFFFFFF);
}

unsigned int JSHash(char *str)
{
	unsigned int hash = 1315423911;

	while (*str)
	{
		hash ^= ((hash << 5) + (*str++) + (hash >> 2));
	}

	return (hash & 0x7FFFFFFF);
}

unsigned int PJWHash(char *str)
{
	unsigned int BitsInUnignedInt = (unsigned int)(sizeof(unsigned int) * 8);
	unsigned int ThreeQuarters = (unsigned int)((BitsInUnignedInt * 3) / 4);
	unsigned int OneEighth = (unsigned int)(BitsInUnignedInt / 8);
	unsigned int HighBits = (unsigned int)(0xFFFFFFFF) << (BitsInUnignedInt - OneEighth);
	unsigned int hash = 0;
	unsigned int test = 0;

	while (*str)
	{
		hash = (hash << OneEighth) + (*str++);
		if ((test = hash & HighBits) != 0)
		{
			hash = ((hash ^ (test >> ThreeQuarters)) & (~HighBits));
		}
	}

	return (hash & 0x7FFFFFFF);
}

unsigned int ELFHash(char *str)
{
	unsigned int hash = 0;
	unsigned int x = 0;

	while (*str)
	{
		hash = (hash << 4) + (*str++);
		if ((x = hash & 0xF0000000L) != 0)
		{
			hash ^= (x >> 24);
			hash &= ~x;
		}
	}

	return (hash & 0x7FFFFFFF);
}

unsigned int BKDRHash(char *str)
{
	unsigned int seed = 131; // 31 131 1313 13131 131313 etc..
	unsigned int hash = 0;

	while (*str)
	{
		hash = hash * seed + (*str++);
	}

	return (hash & 0x7FFFFFFF);
}

unsigned int DJBHash(char *str)
{
	unsigned int hash = 5381;

	while (*str)
	{
		hash += (hash << 5) + (*str++);
	}

	return (hash & 0x7FFFFFFF);
}

unsigned int APHash(char *str)
{
	unsigned int hash = 0;
	int i;

	for (i = 0; *str; i++)
	{
		if ((i & 1) == 0)
		{
			hash ^= ((hash << 7) ^ (*str++) ^ (hash >> 3));
		}
		else
		{
			hash ^= (~((hash << 11) ^ (*str++) ^ (hash >> 5)));
		}
	}

	return (hash & 0x7FFFFFFF);
}

/*
* return void;
* input:
* function: ��ϣֵ��ȡ
*/
unsigned int fnHashGet32(char * pszBuff, HashPolynomialEnum iHashCategory)
{
	unsigned int uiHash = 0;

	switch (iHashCategory)
	{
		case HASH_SDBM: uiHash = SDBMHash(pszBuff); break;
		case HASH_RS: uiHash = RSHash(pszBuff); break;
		case HASH_JS: uiHash = JSHash(pszBuff); break;
		case HASH_PJW: uiHash = PJWHash(pszBuff); break;
		case HASH_ELF: uiHash = ELFHash(pszBuff); break;
		case HASH_BKDR: uiHash = BKDRHash(pszBuff); break;
		case HASH_DJB: uiHash = DJBHash(pszBuff); break;
		case HASH_AP: uiHash = APHash(pszBuff); break;
		default: return 0;
	}

	return (uiHash & 0x7FFFFFFF);
}


/*
* return void;
* input:
* function: �нṹ���ϣ�����
*/
int fnHashLineInsert(VersionType * ptypeVersionHand, LineDataType * ptypeLineDataNew, int iHashTableLength)
{
	int iInsertFlag = 0;
	unsigned int uiHashTableSubscript = 0;
	LineDataType * ptypeLineDataTemp = NULL;



	/* �½�һ���нṹ�壬�����洢��ǰ�������е���Ϣ */
	if (fnLineDataNew(&ptypeLineDataTemp) != 0)
	{
		printfLog("�½��нṹ��ʧ�ܣ�\n");
		return NEW_CONSTRUCTION_STRUCTURE_ERROR;
	}
	memcpy(ptypeLineDataTemp, ptypeLineDataNew, sizeof(LineDataType));

	ptypeLineDataTemp->fac_no = (int *)malloc(sizeof(int) * ptypeLineDataNew->fac_no_number_length);
	if (ptypeLineDataTemp->fac_no == NULL)
	{
		printfLog("ptypeLineDataTemp->fac_no malloc error!\n");
	}
	ptypeLineDataTemp->low_no = (int *)malloc(sizeof(int) * ptypeLineDataNew->low_no_number_length);
	if (ptypeLineDataTemp->low_no == NULL)
	{
		printfLog("ptypeLineDataTemp->low_no malloc error!\n");
	}
	memcpy(ptypeLineDataTemp->fac_no, ptypeLineDataNew->fac_no, sizeof(int) * ptypeLineDataNew->fac_no_number_length);
	memcpy(ptypeLineDataTemp->low_no, ptypeLineDataNew->low_no, sizeof(int) * ptypeLineDataNew->low_no_number_length);


	/* �ɹ�ϣֵȡģ�õ���ϣ�±� */
	uiHashTableSubscript = ptypeLineDataNew->uiHash % iHashTableLength;

	/* ���нṹ����뵽��Ӧ�Ĺ�ϣ���Ԫ���� */
	iInsertFlag = fnLineDataOrderInsert(&(ptypeVersionHand->ptypeLineDataHashTable[uiHashTableSubscript]), ptypeLineDataNew);
	if (iInsertFlag == -1)
	{
		printfLog("��ϣ�������ʧ�ܣ�\n");
		return HASH_TABLE_INSERTION_ERROR;
	}

	/* ���нṹ����뵽��Ӧ�Ĺ�ϣ���Ԫ���� */
	iInsertFlag = fnLineDataHeadInsert(&(ptypeVersionHand->ptypeLineDataUIDSameHashTable[uiHashTableSubscript]), ptypeLineDataTemp);
	if (iInsertFlag == -1)
	{
		printfLog("��ϣ�������ʧ�ܣ�\n");
		return HASH_TABLE_INSERTION_ERROR;
	}

	return 0;
}

/*
* return void;
* input:
* function: �нṹ���ϣ���ʼ��
*/
int fnHashLineDataTableInit(LineDataType ** pptypeLineDataHashTable, int iHashTableLength)
{
	int i = 0, iHashLineDataTableInitFlag = 0;

	/* �����ڴ� */
	*pptypeLineDataHashTable = (LineDataType *)malloc(sizeof(LineDataType) * iHashTableLength);
	if (*pptypeLineDataHashTable == NULL)
	{
		printf("�нṹ���ϣ���ʼ������\n");
		return NEW_HASH_STRUCTURE_ERROR;
	}
	/* ���� */
	memset(*pptypeLineDataHashTable, 0, sizeof(LineDataType) * iHashTableLength);

	return 0;
}
