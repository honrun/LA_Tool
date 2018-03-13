// DrivesHash.cpp : ��Bin�ṹ����д���
//


#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <windows.h>
#include <time.h>

#include "TransferFormat.h"
#include "DrivesFile.h"
#include "DrivesHash.h"
#include "DrivesVersion.h"
#include "DrivesBin.h"



/* ���е�Bin�� */
BinType ex_typeBinData;


/*
* return void;
* input:
* function: �½�һ��Bin�������ݽṹ��
*/
int fnBinGroupInformationNew(GroupInformationType ** ppGroupInformationType)
{
	*ppGroupInformationType = (GroupInformationType *)malloc(sizeof(GroupInformationType));
	if (*ppGroupInformationType == NULL)
	{
		printfLog("\n�½�Bin����ṹ��ʧ�ܣ�\n");
		return NEW_BIN_INTERVAL_STRUCTURE_ERROR;
	}

	/* ��սṹ�� */
	memset(*ppGroupInformationType, 0, sizeof(GroupInformationType));

	(*ppGroupInformationType)->fac_hash_flag = (char *)malloc(sizeof(char) * ex_ConfigurationData.iSectorNumberMaximum);
	(*ppGroupInformationType)->low_hash_flag = (char *)malloc(sizeof(char) * ex_ConfigurationData.iSectorNumberMaximum);
	memset((*ppGroupInformationType)->fac_hash_flag, 0, sizeof(char) * ex_ConfigurationData.iSectorNumberMaximum);
	memset((*ppGroupInformationType)->low_hash_flag, 0, sizeof(char) * ex_ConfigurationData.iSectorNumberMaximum);

	return 0;
}


/*
* return void;
* input:
* function: ���½���Bin����ṹ����뵽����
*/
int fnBinInsertGroupInformation(GroupInformationType * ptypeHead, GroupInformationType * ptypeData)
{
	GroupInformationType * ptypeGroupInformationNow = NULL;

	ptypeGroupInformationNow = ptypeHead;
	while (ptypeGroupInformationNow->next != NULL)
	{
		ptypeGroupInformationNow = ptypeGroupInformationNow->next;
	}

	ptypeGroupInformationNow->next = ptypeData;

	return 0;
}

/*
* return void;
* input:
* function: �нṹ�������Bin��
*/
int fnBinGetLineData(GroupInformationType * ptypeGroupInformationHead, LineDataType * ptypeLineData, int iScope)
{
	/* ��ѯ��Bin���µĸ�����ָ�� */
	GroupInformationType * ptypeGroupInformationNow = NULL;
	/* ��ѯ���������ֵ */
	int iScopeNow = 0;
	/* ����Ϣ�ڵ�FanNo��LowNo�ĵ��������� */
	int iPopFacNo = 0, iPopLowNo = 0;
	int * piFacNoReturn = NULL, *piLowNoReturn = NULL;
	/* ����Ϣ��FanNo��LowNo�ı�־�������ǰ������Ӧ�����������־��λ */
	int iFacNoOrLowNoFlag = 0;
	int i = 0;


	/* ���α���ÿ�������� */
	for (i = 0; i < ptypeLineData->fac_no_number_length; ++i)
	{
		/* �õ���ǰ������ */
		iPopFacNo = ptypeLineData->fac_no[i];

		/* ��С��ʼ�����������䡣����ͷ���洢���ݣ� */
		iScopeNow = iScope;
		ptypeGroupInformationNow = ptypeGroupInformationHead->next;
		while (ptypeGroupInformationNow != NULL)
		{
			/* ��ѯ��ǰ�����������ĸ���Χ���� */
			if (iPopFacNo < iScopeNow)
			{
				/* �����ǰ����������FacNo�Ĺ�ϣ�����Ӧλ�����㣬��ʾ��ǰ�����Ż�û�в������ݣ��ͽ��в��� */
				if (ptypeGroupInformationNow->fac_hash_flag[iPopFacNo] == 0)
				{
					/* �����ǰ���ȶ�BIN_INCREASES_AMOUNT_MEMORYȡģ����0����ô��ʾ��ǰ�ڴ��Ѿ����ˣ���Ҫ������BIN_INCREASES_AMOUNT_MEMORY�ڴ� */
					if ((ptypeGroupInformationNow->fac_no_number_length % BIN_INCREASES_AMOUNT_MEMORY) == 0)
					{
						/* �����ڴ�洢 */
						piFacNoReturn = (int *)realloc(ptypeGroupInformationNow->fac_no, sizeof(int) * (ptypeGroupInformationNow->fac_no_number_length + BIN_INCREASES_AMOUNT_MEMORY));
						if (piFacNoReturn == NULL)
						{
							printfLog("������ת�Ƶ�Bin�ṹ��ʱ�������ڴ����\n");
							return NEW_BIN_FACNO_MEMORY_ERROR;
						}
						ptypeGroupInformationNow->fac_no = piFacNoReturn;
					}
					ptypeGroupInformationNow->fac_no[ptypeGroupInformationNow->fac_no_number_length] = iPopFacNo;
					++ptypeGroupInformationNow->fac_no_number_length;

					/* ��ǰ����������FacNo�Ĺ�ϣ�����Ӧλ��1 */
					ptypeGroupInformationNow->fac_hash_flag[iPopFacNo] = 1;
				}
				/* ��ǰ���е�ǰ��������������Ա�־λ��λ */
				ptypeGroupInformationNow->fac_flag = 1;
				break;
			}
			/* �����������һ������ */
			iScopeNow += iScope;
			ptypeGroupInformationNow = ptypeGroupInformationNow->next;
		}
	}

	/* ���α���ÿ�������� */
	for (i = 0; i < ptypeLineData->low_no_number_length; ++i)
	{
		/* �õ���ǰ������ */
		iPopLowNo = ptypeLineData->low_no[i];

		/* ��С��ʼ�����������䡣����ͷ���洢���ݣ� */
		iScopeNow = iScope;
		ptypeGroupInformationNow = ptypeGroupInformationHead->next;
		while (ptypeGroupInformationNow != NULL)
		{
			/* ��ѯ��ǰ�����������ĸ���Χ���� */
			if (iPopLowNo < iScopeNow)
			{
				/* �����ǰ����������FacNo�Ĺ�ϣ�����Ӧλ�����㣬��ʾ��ǰ�����Ż�û�в������ݣ��ͽ��в��� */
				if (ptypeGroupInformationNow->low_hash_flag[iPopLowNo] == 0)
				{
					/* �����ǰ���ȶ�BIN_INCREASES_AMOUNT_MEMORYȡģ����0����ô��ʾ��ǰ�ڴ��Ѿ����ˣ���Ҫ������BIN_INCREASES_AMOUNT_MEMORY�ڴ� */
					if ((ptypeGroupInformationNow->low_no_number_length % BIN_INCREASES_AMOUNT_MEMORY) == 0)
					{
						/* �����ڴ�洢 */
						piLowNoReturn = (int *)realloc(ptypeGroupInformationNow->low_no, sizeof(int) * (ptypeGroupInformationNow->low_no_number_length + BIN_INCREASES_AMOUNT_MEMORY));
						if (piLowNoReturn == NULL)
						{
							printfLog("������ת�Ƶ�Bin�ṹ��ʱ�������ڴ����\n");
							return NEW_BIN_FACNO_MEMORY_ERROR;
						}
						ptypeGroupInformationNow->low_no = piLowNoReturn;
					}
					ptypeGroupInformationNow->low_no[ptypeGroupInformationNow->low_no_number_length] = iPopLowNo;
					++ptypeGroupInformationNow->low_no_number_length;

					/* ��ǰ����������FacNo�Ĺ�ϣ�����Ӧλ��1 */
					ptypeGroupInformationNow->low_hash_flag[iPopLowNo] = 1;
				}
				/* ��ǰ���е�ǰ��������������Ա�־λ��λ */
				ptypeGroupInformationNow->low_flag = 1;
				break;
			}
			/* �����������һ������ */
			iScopeNow += iScope;
			ptypeGroupInformationNow = ptypeGroupInformationNow->next;
		}
	}


	/* һ�����ݴ�����ɺ��ѯ�������ĸ��飬��ͷ���洢���� */
	ptypeGroupInformationNow = ptypeGroupInformationHead->next;
	while (ptypeGroupInformationNow != NULL)
	{
		/* �����ǰ�����־��λ����������������һ */
		if (ptypeGroupInformationNow->fac_flag != 0)
		{
			++ptypeGroupInformationNow->fac_number;
			iFacNoOrLowNoFlag |= 0x01;
		}
		/* �����ǰ�����־��λ����������������һ */
		if (ptypeGroupInformationNow->low_flag != 0)
		{
			++ptypeGroupInformationNow->low_number;
			iFacNoOrLowNoFlag |= 0x02;
		}
		/* �����־���� */
		ptypeGroupInformationNow->fac_flag = 0;
		ptypeGroupInformationNow->low_flag = 0;

		/* ǰ����һ������ */
		ptypeGroupInformationNow = ptypeGroupInformationNow->next;
	}


	/* �жϵ�ǰ�нṹ���Ƿ���FacNo��LowNo��Ϣ������У��Ͱ���Ӧ����������1 */
	if (iFacNoOrLowNoFlag & 0x01)
		++ptypeGroupInformationHead->fac_number;
	if (iFacNoOrLowNoFlag & 0x02)
		++ptypeGroupInformationHead->low_number;

	/* ��ͬ���͵�Bin��ͳ���ж��ٸ� */
	++ptypeGroupInformationHead->bin_number;

	return 0;
}


/*
* return void;
* input:
* function: �ѹ�ϣ���е�һ�����������ݽ�������ͬ����Bin
*/
int fnBinGetOneLineList(BinType * ptypeBin, LineDataType * ptypeLineDataHead)
{
	/* ��ϣ��Ԫ���µĵ�ǰ�нṹ�� */
	LineDataType * ptypeLineDataNow = NULL;

	/* ѭ����ȡ��ǰ�нṹ�������ڵ����ݡ�����ͷ���洢���ݣ� */
	ptypeLineDataNow = ptypeLineDataHead->next;
	while (ptypeLineDataNow != NULL)
	{
		if (strcmp(ptypeLineDataNow->bin, "A0") == 0)
		{
			/* �ѵ�ǰ�нṹ�������BinA0�� */
			if (fnBinGetLineData(ptypeBin->A0, ptypeLineDataNow, ptypeBin->scope) != 0)
			{
				return ROW_STRUCTURE_RESOLVED_TO_BIN_GROUP_ERROR;
			}
		}
		else if (strcmp(ptypeLineDataNow->bin, "A1") == 0)
		{
			/* �ѵ�ǰ�нṹ�������BinA1�� */
			if (fnBinGetLineData(ptypeBin->A1, ptypeLineDataNow, ptypeBin->scope) != 0)
			{
				return ROW_STRUCTURE_RESOLVED_TO_BIN_GROUP_ERROR;
			}
		}
		else if (strcmp(ptypeLineDataNow->bin, "B0") == 0)
		{
			/* �ѵ�ǰ�нṹ�������BinB0�� */
			if (fnBinGetLineData(ptypeBin->B0, ptypeLineDataNow, ptypeBin->scope) != 0)
			{
				return ROW_STRUCTURE_RESOLVED_TO_BIN_GROUP_ERROR;
			}
		}
		else if (strcmp(ptypeLineDataNow->bin, "B1") == 0)
		{
			/* �ѵ�ǰ�нṹ�������BinB1�� */
			if (fnBinGetLineData(ptypeBin->B1, ptypeLineDataNow, ptypeBin->scope) != 0)
			{
				return ROW_STRUCTURE_RESOLVED_TO_BIN_GROUP_ERROR;
			}
		}
		else if (strcmp(ptypeLineDataNow->bin, "C0") == 0)
		{
			/* �ѵ�ǰ�нṹ�������BinC0�� */
			if (fnBinGetLineData(ptypeBin->C0, ptypeLineDataNow, ptypeBin->scope) != 0)
			{
				return ROW_STRUCTURE_RESOLVED_TO_BIN_GROUP_ERROR;
			}
		}
		else if (strcmp(ptypeLineDataNow->bin, "C1") == 0)
		{
			/* �ѵ�ǰ�нṹ�������BinC1�� */
			if (fnBinGetLineData(ptypeBin->C1, ptypeLineDataNow, ptypeBin->scope) != 0)
			{
				return ROW_STRUCTURE_RESOLVED_TO_BIN_GROUP_ERROR;
			}
		}
		else if (strcmp(ptypeLineDataNow->bin, "E0") == 0)
		{
			/* �ѵ�ǰ�нṹ�������BinF0�� */
			if (fnBinGetLineData(ptypeBin->E0, ptypeLineDataNow, ptypeBin->scope) != 0)
			{
				return ROW_STRUCTURE_RESOLVED_TO_BIN_GROUP_ERROR;
			}
		}
		else if (strcmp(ptypeLineDataNow->bin, "F0") == 0)
		{
			/* �ѵ�ǰ�нṹ�������BinF0�� */
			if (fnBinGetLineData(ptypeBin->F0, ptypeLineDataNow, ptypeBin->scope) != 0)
			{
				return ROW_STRUCTURE_RESOLVED_TO_BIN_GROUP_ERROR;
			}
		}
		else if (strcmp(ptypeLineDataNow->bin, "F1") == 0)
		{
			/* �ѵ�ǰ�нṹ�������BinF1�� */
			if (fnBinGetLineData(ptypeBin->F1, ptypeLineDataNow, ptypeBin->scope) != 0)
			{
				return ROW_STRUCTURE_RESOLVED_TO_BIN_GROUP_ERROR;
			}
		}
		else if (strcmp(ptypeLineDataNow->bin, "F2") == 0)
		{
			/* �ѵ�ǰ�нṹ�������BinF1�� */
			if (fnBinGetLineData(ptypeBin->F2, ptypeLineDataNow, ptypeBin->scope) != 0)
			{
				return ROW_STRUCTURE_RESOLVED_TO_BIN_GROUP_ERROR;
			}
		}
		else if (strcmp(ptypeLineDataNow->bin, "F3") == 0)
		{
			/* �ѵ�ǰ�нṹ�������BinF1�� */
			if (fnBinGetLineData(ptypeBin->F3, ptypeLineDataNow, ptypeBin->scope) != 0)
			{
				return ROW_STRUCTURE_RESOLVED_TO_BIN_GROUP_ERROR;
			}
		}
		else
		{
			/* Bin�����ʹ��� */
			printfLog("�����Bin�����ͣ�����Ϊ:%s\n", ptypeLineDataNow->bin);
		}

		/* ǰ����һ���ṹ�� */
		ptypeLineDataNow = ptypeLineDataNow->next;
	}

	return 0;
}
/*
* return void;
* input:
* function: �ѹ�ϣ���е�һ�����������ݴ�ӡ���ļ�
*/
int fnBinGetOneLineUIDSameList(FILE * pFileNewHand, LineDataType * ptypeLineDataHead)
{
	/* ��ϣ��Ԫ���µĵ�ǰ�нṹ�� */
	LineDataType * ptypeLineDataNow = NULL;
	int i = 0, iTimesRepetitionSum = 0;

	/* ѭ����ȡ��ǰ�нṹ�������ڵ����ݡ�����ͷ���洢���ݣ� */
	ptypeLineDataNow = ptypeLineDataHead->next;
	while (ptypeLineDataNow != NULL)
	{
		if (ptypeLineDataNow->times_of_repetition > 1)
		{
			fprintf(pFileNewHand, "UID is: %s\n", ptypeLineDataNow->flashUID);
			fprintf(pFileNewHand, "Repetition is: %d\n", ptypeLineDataNow->times_of_repetition - 1);
			fprintf(pFileNewHand, "Bin and Path is:\n");
			for (i = 0; i < ptypeLineDataNow->times_of_repetition; ++i)
			{
				fprintf(pFileNewHand, "%s\n", ptypeLineDataNow->UID_same_path_and_name[i]);
			}
			fprintf(pFileNewHand, "\n");

			iTimesRepetitionSum += ptypeLineDataNow->times_of_repetition - 1;
		}

		/* ǰ����һ���ṹ�� */
		ptypeLineDataNow = ptypeLineDataNow->next;
	}

	return iTimesRepetitionSum;
}




/*
* return void;
* input:
* function: �ѹ�ϣ���е��������������ݷ��͵���������
*/
int fnBinGetHashLineList(BinType * ptypeBin, LineDataType * ptypeLineDataHashTable, int iHashTableLength)
{
	/* ��Bin���ϣ���ͷ */
	LineDataType * ptypeLineDataHead;
	int i = 0;


	/* �ѹ�ϣ���ڵ�ÿһ��Ԫ�أ�ͷָ�룩���뺯�� */
	for (i = 0; i < iHashTableLength; ++i)
	{
		ptypeLineDataHead = &ptypeLineDataHashTable[i];
		if (fnBinGetOneLineList(ptypeBin, ptypeLineDataHead) != 0)
		{
			return ROW_STRUCTURE_RESOLVED_TO_BIN_GROUP_ERROR;
		}
	}

	return 0;
}

/*
* return void;
* input:
* function: �ѹ�ϣ���е��������������ݷ��͵���ӡ����
*/
int fnBinGetHashLineUIDSameList(FILE * pFileNewHand, LineDataType * ptypeLineDataHashTable, int iHashTableLength)
{
	/* ��Bin���ϣ���ͷ */
	LineDataType * ptypeLineDataHead;
	int i = 0, iTimesRepetitionSum = 0;


	/* �ѹ�ϣ���ڵ�ÿһ��Ԫ�أ�ͷָ�룩���뺯�� */
	for (i = 0; i < iHashTableLength; ++i)
	{
		ptypeLineDataHead = &ptypeLineDataHashTable[i];
		iTimesRepetitionSum += fnBinGetOneLineUIDSameList(pFileNewHand, ptypeLineDataHead);
	}

	fprintf(pFileNewHand, "\niTimesRepetitionSum is: %d\n", iTimesRepetitionSum);

	return 0;
}


/*
* return void;
* input:
* function: ��⵱ǰBin����������İٷֱ�
*/
int fnBinPercentage(GroupInformationType * ptypeGroupInformationHead)
{
	GroupInformationType * ptypeGroupInformationNow = NULL;
	int iFacNoNunberSum = 0, iLowNoNumberSum = 0;

	/* ��ȡ��ǰBin��Ļ��������� */
	iFacNoNunberSum = ptypeGroupInformationHead->fac_number;
	iLowNoNumberSum = ptypeGroupInformationHead->low_number;

	/* ����ͷ���˻������������洢�������� */
	ptypeGroupInformationNow = ptypeGroupInformationHead->next;
	while (ptypeGroupInformationNow != NULL)
	{
		/* ������Ϊ���ж� */
		if (iFacNoNunberSum != 0)
			ptypeGroupInformationNow->fac_percentage = ptypeGroupInformationNow->fac_number * 100.0f / (float)iFacNoNunberSum;
		else
			ptypeGroupInformationNow->fac_percentage = 0.0f;

		/* ������Ϊ���ж� */
		if (iLowNoNumberSum != 0)
			ptypeGroupInformationNow->low_percentage = ptypeGroupInformationNow->low_number * 100.0f / (float)iLowNoNumberSum;
		else
			ptypeGroupInformationNow->low_percentage = 0.0f;

		/* ǰ���¸����� */
		ptypeGroupInformationNow = ptypeGroupInformationNow->next;
	}

	return 0;
}

/*
* return void;
* input:
* function: ��⵱ǰ����Bin����İٷֱ�
*/
int fnBinALLPercentage(BinType * ptypeBin)
{
	fnBinPercentage(ptypeBin->A0);
	fnBinPercentage(ptypeBin->A1);

	fnBinPercentage(ptypeBin->B0);
	fnBinPercentage(ptypeBin->B1);

	fnBinPercentage(ptypeBin->C0);
	fnBinPercentage(ptypeBin->C1);

	fnBinPercentage(ptypeBin->F0);
	fnBinPercentage(ptypeBin->F1);

	return 0;
}


/*
* return void;
* input:
* function: �Ե�ǰBin�����������FacNo��LowNo��������
*/
int fnBinSorting(GroupInformationType * ptypeGroupInformationHead)
{
	GroupInformationType * ptypeGroupInformationNow = NULL;
	int iFacNoNunberSum = 0, iLowNoNumberSum = 0;


	/* ����ͷ���˻������������洢�������� */
	ptypeGroupInformationNow = ptypeGroupInformationHead->next;
	while (ptypeGroupInformationNow != NULL)
	{
		fnSelectSort(ptypeGroupInformationNow->fac_no, ptypeGroupInformationNow->fac_no_number_length);
		fnSelectSort(ptypeGroupInformationNow->low_no, ptypeGroupInformationNow->low_no_number_length);

		/* ǰ���¸����� */
		ptypeGroupInformationNow = ptypeGroupInformationNow->next;
	}

	return 0;
}

/*
* return void;
* input:
* function: ����Bin�����FacNo��LowNo��������
*/
int fnBinALLSorting(BinType * ptypeBin)
{
	fnBinSorting(ptypeBin->A0);
	fnBinSorting(ptypeBin->A1);

	fnBinSorting(ptypeBin->B0);
	fnBinSorting(ptypeBin->B1);

	fnBinSorting(ptypeBin->C0);
	fnBinSorting(ptypeBin->C1);

	fnBinSorting(ptypeBin->F0);
	fnBinSorting(ptypeBin->F1);

	return 0;
}

/*
* return void;
* input:
* function: ��ӡ��ǰBin�ṹ�������е��������ݵ��ļ�
*/
int fnBinGroupInformationPrintfToFile(FILE * pFileHand, GroupInformationType * ptypeGroupInformation, int iScope)
{
	GroupInformationType * ptypeGroupInformationNow = NULL;
	int iScopeNow = 0, iScopeOld = 0;
	int i;

	/* ��ӡfac_no��Ϣ */
	fputs("FacNo:,", pFileHand);
	fprintf(pFileHand, "%d\n", ptypeGroupInformation->fac_number);
	iScopeOld = 0;
	iScopeNow = iScope;
	ptypeGroupInformationNow = ptypeGroupInformation->next;
	while (ptypeGroupInformationNow != NULL)
	{
		fprintf(pFileHand, "%04d~%04d,", iScopeOld, iScopeNow - 1);
		fprintf(pFileHand, "%d,", ptypeGroupInformationNow->fac_number);
		fprintf(pFileHand, "%5.2f%%,", ptypeGroupInformationNow->fac_percentage);
		for (i = 0; i < ptypeGroupInformationNow->fac_no_number_length; ++i)
		{
			fprintf(pFileHand, "%04d ", ptypeGroupInformationNow->fac_no[i]);
		}
		fprintf(pFileHand, "\n");

		iScopeOld = iScopeNow;
		iScopeNow += iScope;
		ptypeGroupInformationNow = ptypeGroupInformationNow->next;
	}

	/* ��ӡlow_no��Ϣ */
	fputs("LowNo:,", pFileHand);
	fprintf(pFileHand, "%d\n", ptypeGroupInformation->low_number);
	iScopeOld = 0;
	iScopeNow = iScope;
	ptypeGroupInformationNow = ptypeGroupInformation->next;
	while (ptypeGroupInformationNow != NULL)
	{
		fprintf(pFileHand, "%04d~%04d,", iScopeOld, iScopeNow - 1);
		fprintf(pFileHand, "%d,", ptypeGroupInformationNow->low_number);
		fprintf(pFileHand, "%5.2f%%,", ptypeGroupInformationNow->low_percentage);
		for (i = 0; i < ptypeGroupInformationNow->low_no_number_length; ++i)
		{
			fprintf(pFileHand, "%04d ", ptypeGroupInformationNow->low_no[i]);
		}
		fprintf(pFileHand, "\n");

		iScopeOld = iScopeNow;
		iScopeNow += iScope;
		ptypeGroupInformationNow = ptypeGroupInformationNow->next;
	}

	fputs("\n", pFileHand);

	return 0;
}

/*
* return void;
* input:
* function: ��ӡ��ǰ�ṹ�������е�Bin��ṹ�����ݵ��ļ�
*/
int fnBinALLPrintfToFile(FILE * pFileHand, BinType * ptypeBinData, BinType * ptypeBinDataUIDSame)
{
	/* д������� */
	fputs("Bin,Number,Same number\n", pFileHand);

	/* ��ӡBin��� */
	fputs("A0:,", pFileHand);
	/* ��ӡBinA0����Ʒ�� */
	fprintf(pFileHand, "%d,", ptypeBinData->A0->bin_number);
	fprintf(pFileHand, "%d\n", ptypeBinDataUIDSame->A0->bin_number);
	/* ��ӡBinA0��ϸ��Ϣ */
//	fnBinGroupInformationPrintfToFile(pFileHand, ptypeBin->A0, ptypeBin->scope);

	fputs("A1:,", pFileHand);
	fprintf(pFileHand, "%d,", ptypeBinData->A1->bin_number);
	fprintf(pFileHand, "%d\n", ptypeBinDataUIDSame->A1->bin_number);
//	fnBinGroupInformationPrintfToFile(pFileHand, ptypeBin->A1, ptypeBin->scope);

	fputs("B0:,", pFileHand);
	fprintf(pFileHand, "%d,", ptypeBinData->B0->bin_number);
	fprintf(pFileHand, "%d\n", ptypeBinDataUIDSame->B0->bin_number);
//	fnBinGroupInformationPrintfToFile(pFileHand, ptypeBin->B0, ptypeBin->scope);

	fputs("B1:,", pFileHand);
	fprintf(pFileHand, "%d,", ptypeBinData->B1->bin_number);
	fprintf(pFileHand, "%d\n", ptypeBinDataUIDSame->B1->bin_number);
//	fnBinGroupInformationPrintfToFile(pFileHand, ptypeBin->B1, ptypeBin->scope);

	fputs("C0:,", pFileHand);
	fprintf(pFileHand, "%d,", ptypeBinData->C0->bin_number);
	fprintf(pFileHand, "%d\n", ptypeBinDataUIDSame->C0->bin_number);
//	fnBinGroupInformationPrintfToFile(pFileHand, ptypeBin->C0, ptypeBin->scope);

	fputs("C1:,", pFileHand);
	fprintf(pFileHand, "%d,", ptypeBinData->C1->bin_number);
	fprintf(pFileHand, "%d\n", ptypeBinDataUIDSame->C1->bin_number);
//	fnBinGroupInformationPrintfToFile(pFileHand, ptypeBin->C1, ptypeBin->scope);

//	while (1);
	fputs("E0:,", pFileHand);
	fprintf(pFileHand, "%d,", ptypeBinData->E0->bin_number);
	fprintf(pFileHand, "%d\n", ptypeBinDataUIDSame->E0->bin_number);
	//	fnBinGroupInformationPrintfToFile(pFileHand, ptypeBin->E0, ptypeBin->scope);

	fputs("F0:,", pFileHand);
	fprintf(pFileHand, "%d,", ptypeBinData->F0->bin_number);
	fprintf(pFileHand, "%d\n", ptypeBinDataUIDSame->F0->bin_number);
	//	fnBinGroupInformationPrintfToFile(pFileHand, ptypeBin->F0, ptypeBin->scope);

	fputs("F1:,", pFileHand);
	fprintf(pFileHand, "%d,", ptypeBinData->F1->bin_number);
	fprintf(pFileHand, "%d\n", ptypeBinDataUIDSame->F1->bin_number);
	//	fnBinGroupInformationPrintfToFile(pFileHand, ptypeBin->F1, ptypeBin->scope);

	fputs("F2:,", pFileHand);
	fprintf(pFileHand, "%d,", ptypeBinData->F2->bin_number);
	fprintf(pFileHand, "%d\n", ptypeBinDataUIDSame->F2->bin_number);
	//	fnBinGroupInformationPrintfToFile(pFileHand, ptypeBin->F2, ptypeBin->scope);

	fputs("F3:,", pFileHand);
	fprintf(pFileHand, "%d,", ptypeBinData->F3->bin_number);
	fprintf(pFileHand, "%d\n", ptypeBinDataUIDSame->F3->bin_number);
	//	fnBinGroupInformationPrintfToFile(pFileHand, ptypeBin->F3, ptypeBin->scope);

/* ��ӡBin��� */
	fputs("Total:,", pFileHand);
	/* ��ӡBinA0����Ʒ�� */
	fprintf(pFileHand, "%d,", ptypeBinData->A0->bin_number
							+ ptypeBinData->A1->bin_number
							+ ptypeBinData->B0->bin_number
							+ ptypeBinData->B1->bin_number
							+ ptypeBinData->C0->bin_number
							+ ptypeBinData->C1->bin_number
							+ ptypeBinData->E0->bin_number
							+ ptypeBinData->F0->bin_number
							+ ptypeBinData->F1->bin_number
							+ ptypeBinData->F2->bin_number
							+ ptypeBinData->F3->bin_number);

	fprintf(pFileHand, "%d\n", ptypeBinDataUIDSame->A0->bin_number
							+ ptypeBinDataUIDSame->A1->bin_number
							+ ptypeBinDataUIDSame->B0->bin_number
							+ ptypeBinDataUIDSame->B1->bin_number
							+ ptypeBinDataUIDSame->C0->bin_number
							+ ptypeBinDataUIDSame->C1->bin_number
							+ ptypeBinDataUIDSame->E0->bin_number
							+ ptypeBinDataUIDSame->F0->bin_number
							+ ptypeBinDataUIDSame->F1->bin_number
							+ ptypeBinDataUIDSame->F2->bin_number
							+ ptypeBinDataUIDSame->F3->bin_number);

	return 0;
}

/*
* return void;
* input:
* function: ȫ�ֱ���Bin�ṹ���ʼ��
*/
int fnBinDataTypeInit(BinType * pbinType, int iScope)
{
	int i = 0, iSectionNumber = 0;
	GroupInformationType * ptypeGroupInformation = NULL;

	/* Bin�������ֵ���Ե�ǰ�������䷶Χ���õ�Ҫ���ɵ�������� */
	iSectionNumber = ex_ConfigurationData.iSectorNumberMaximum / iScope;
	if (iSectionNumber < 1)
		return SET_BIN_INTERVAL_RANGE_ERROR;

	/* ���û�г�������ô��������һ�����䣬���Դ洢��������Щβ�� */
	if ((ex_ConfigurationData.iSectorNumberMaximum % iScope) != 0)
		++iSectionNumber;

	/* ��ʼ��ÿ��Bin���ͷ�ṹ�壬ͷ�ṹ�岻������ṹ���ݣ�ֻ�������ӦBin���� */
	if(fnBinGroupInformationNew(&pbinType->A0) != 0)
		return NEW_BIN_INTERVAL_STRUCTURE_ERROR;
	if (fnBinGroupInformationNew(&pbinType->A1) != 0)
		return NEW_BIN_INTERVAL_STRUCTURE_ERROR;
	if (fnBinGroupInformationNew(&pbinType->B0) != 0)
		return NEW_BIN_INTERVAL_STRUCTURE_ERROR;
	if (fnBinGroupInformationNew(&pbinType->B1) != 0)
		return NEW_BIN_INTERVAL_STRUCTURE_ERROR;
	if (fnBinGroupInformationNew(&pbinType->C0) != 0)
		return NEW_BIN_INTERVAL_STRUCTURE_ERROR;
	if (fnBinGroupInformationNew(&pbinType->C1) != 0)
		return NEW_BIN_INTERVAL_STRUCTURE_ERROR;
	if (fnBinGroupInformationNew(&pbinType->E0) != 0)
		return NEW_BIN_INTERVAL_STRUCTURE_ERROR;
	if (fnBinGroupInformationNew(&pbinType->F0) != 0)
		return NEW_BIN_INTERVAL_STRUCTURE_ERROR;
	if (fnBinGroupInformationNew(&pbinType->F1) != 0)
		return NEW_BIN_INTERVAL_STRUCTURE_ERROR;
	if (fnBinGroupInformationNew(&pbinType->F2) != 0)
		return NEW_BIN_INTERVAL_STRUCTURE_ERROR;
	if (fnBinGroupInformationNew(&pbinType->F3) != 0)
		return NEW_BIN_INTERVAL_STRUCTURE_ERROR;

	/* ��¼���䷶Χ */
	pbinType->scope = iScope;

	/* iSectionNumberΪ�����������ļ��ڵ����䷶Χ�õ������������ ���½���Ӧ���������ṹ����뵽��Ӧ������*/
	for (i = 0; i<iSectionNumber; ++i)
	{
		if (fnBinGroupInformationNew(&ptypeGroupInformation) != 0)
			return NEW_BIN_INTERVAL_STRUCTURE_ERROR;
		if (fnBinInsertGroupInformation(pbinType->A0, ptypeGroupInformation) != 0)
			return INITIALIZE_BIN_STRUCTURE_ERROR;

		if (fnBinGroupInformationNew(&ptypeGroupInformation) != 0)
			return NEW_BIN_INTERVAL_STRUCTURE_ERROR;
		if (fnBinInsertGroupInformation(pbinType->A1, ptypeGroupInformation) != 0)
			return INITIALIZE_BIN_STRUCTURE_ERROR;

		if (fnBinGroupInformationNew(&ptypeGroupInformation) != 0)
			return NEW_BIN_INTERVAL_STRUCTURE_ERROR;
		if (fnBinInsertGroupInformation(pbinType->B0, ptypeGroupInformation) != 0)
			return INITIALIZE_BIN_STRUCTURE_ERROR;

		if (fnBinGroupInformationNew(&ptypeGroupInformation) != 0)
			return NEW_BIN_INTERVAL_STRUCTURE_ERROR;
		if (fnBinInsertGroupInformation(pbinType->B1, ptypeGroupInformation) != 0)
			return INITIALIZE_BIN_STRUCTURE_ERROR;

		if (fnBinGroupInformationNew(&ptypeGroupInformation) != 0)
			return NEW_BIN_INTERVAL_STRUCTURE_ERROR;
		if (fnBinInsertGroupInformation(pbinType->C0, ptypeGroupInformation) != 0)
			return INITIALIZE_BIN_STRUCTURE_ERROR;

		if (fnBinGroupInformationNew(&ptypeGroupInformation) != 0)
			return NEW_BIN_INTERVAL_STRUCTURE_ERROR;
		if (fnBinInsertGroupInformation(pbinType->C1, ptypeGroupInformation) != 0)
			return INITIALIZE_BIN_STRUCTURE_ERROR;

		if (fnBinGroupInformationNew(&ptypeGroupInformation) != 0)
			return NEW_BIN_INTERVAL_STRUCTURE_ERROR;
		if (fnBinInsertGroupInformation(pbinType->E0, ptypeGroupInformation) != 0)
			return INITIALIZE_BIN_STRUCTURE_ERROR;

		if (fnBinGroupInformationNew(&ptypeGroupInformation) != 0)
			return NEW_BIN_INTERVAL_STRUCTURE_ERROR;
		if (fnBinInsertGroupInformation(pbinType->F0, ptypeGroupInformation) != 0)
			return INITIALIZE_BIN_STRUCTURE_ERROR;

		if (fnBinGroupInformationNew(&ptypeGroupInformation) != 0)
			return NEW_BIN_INTERVAL_STRUCTURE_ERROR;
		if (fnBinInsertGroupInformation(pbinType->F1, ptypeGroupInformation) != 0)
			return INITIALIZE_BIN_STRUCTURE_ERROR;

		if (fnBinGroupInformationNew(&ptypeGroupInformation) != 0)
			return NEW_BIN_INTERVAL_STRUCTURE_ERROR;
		if (fnBinInsertGroupInformation(pbinType->F2, ptypeGroupInformation) != 0)
			return INITIALIZE_BIN_STRUCTURE_ERROR;

		if (fnBinGroupInformationNew(&ptypeGroupInformation) != 0)
			return NEW_BIN_INTERVAL_STRUCTURE_ERROR;
		if (fnBinInsertGroupInformation(pbinType->F3, ptypeGroupInformation) != 0)
			return INITIALIZE_BIN_STRUCTURE_ERROR;
	}

	return 0;
}
