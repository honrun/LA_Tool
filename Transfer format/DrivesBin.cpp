// DrivesHash.cpp : 对Bin结构体进行处理
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



/* 所有的Bin组 */
BinType ex_typeBinData;


/*
* return void;
* input:
* function: 新建一个Bin区间数据结构体
*/
int fnBinGroupInformationNew(GroupInformationType ** ppGroupInformationType)
{
	*ppGroupInformationType = (GroupInformationType *)malloc(sizeof(GroupInformationType));
	if (*ppGroupInformationType == NULL)
	{
		printfLog("\n新建Bin区间结构体失败！\n");
		return NEW_BIN_INTERVAL_STRUCTURE_ERROR;
	}

	/* 清空结构体 */
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
* function: 把新建的Bin区间结构体插入到链表
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
* function: 行结构体解析到Bin组
*/
int fnBinGetLineData(GroupInformationType * ptypeGroupInformationHead, LineDataType * ptypeLineData, int iScope)
{
	/* 轮询的Bin组下的各区间指针 */
	GroupInformationType * ptypeGroupInformationNow = NULL;
	/* 轮询的区间大最值 */
	int iScopeNow = 0;
	/* 行信息内的FanNo与LowNo的单个扇区号 */
	int iPopFacNo = 0, iPopLowNo = 0;
	int * piFacNoReturn = NULL, *piLowNoReturn = NULL;
	/* 行信息内FanNo与LowNo的标志，如果当前行有相应的扇区，则标志置位 */
	int iFacNoOrLowNoFlag = 0;
	int i = 0;


	/* 依次遍历每个扇区号 */
	for (i = 0; i < ptypeLineData->fac_no_number_length; ++i)
	{
		/* 得到当前扇区号 */
		iPopFacNo = ptypeLineData->fac_no[i];

		/* 从小开始查找扇区区间。（链头不存储数据） */
		iScopeNow = iScope;
		ptypeGroupInformationNow = ptypeGroupInformationHead->next;
		while (ptypeGroupInformationNow != NULL)
		{
			/* 查询当前扇区号属于哪个范围区间 */
			if (iPopFacNo < iScopeNow)
			{
				/* 如果当前区间扇区号FacNo的哈希表的相应位等于零，表示当前扇区号还没有插入数据，就进行插入 */
				if (ptypeGroupInformationNow->fac_hash_flag[iPopFacNo] == 0)
				{
					/* 如果当前长度对BIN_INCREASES_AMOUNT_MEMORY取模等于0，那么表示当前内存已经满了，需要再增加BIN_INCREASES_AMOUNT_MEMORY内存 */
					if ((ptypeGroupInformationNow->fac_no_number_length % BIN_INCREASES_AMOUNT_MEMORY) == 0)
					{
						/* 增加内存存储 */
						piFacNoReturn = (int *)realloc(ptypeGroupInformationNow->fac_no, sizeof(int) * (ptypeGroupInformationNow->fac_no_number_length + BIN_INCREASES_AMOUNT_MEMORY));
						if (piFacNoReturn == NULL)
						{
							printfLog("行数据转移到Bin结构体时，增加内存错误！\n");
							return NEW_BIN_FACNO_MEMORY_ERROR;
						}
						ptypeGroupInformationNow->fac_no = piFacNoReturn;
					}
					ptypeGroupInformationNow->fac_no[ptypeGroupInformationNow->fac_no_number_length] = iPopFacNo;
					++ptypeGroupInformationNow->fac_no_number_length;

					/* 当前区间扇区号FacNo的哈希表的相应位置1 */
					ptypeGroupInformationNow->fac_hash_flag[iPopFacNo] = 1;
				}
				/* 当前行有当前区间的扇区，所以标志位置位 */
				ptypeGroupInformationNow->fac_flag = 1;
				break;
			}
			/* 区间向大增加一个区间 */
			iScopeNow += iScope;
			ptypeGroupInformationNow = ptypeGroupInformationNow->next;
		}
	}

	/* 依次遍历每个扇区号 */
	for (i = 0; i < ptypeLineData->low_no_number_length; ++i)
	{
		/* 得到当前扇区号 */
		iPopLowNo = ptypeLineData->low_no[i];

		/* 从小开始查找扇区区间。（链头不存储数据） */
		iScopeNow = iScope;
		ptypeGroupInformationNow = ptypeGroupInformationHead->next;
		while (ptypeGroupInformationNow != NULL)
		{
			/* 查询当前扇区号属于哪个范围区间 */
			if (iPopLowNo < iScopeNow)
			{
				/* 如果当前区间扇区号FacNo的哈希表的相应位等于零，表示当前扇区号还没有插入数据，就进行插入 */
				if (ptypeGroupInformationNow->low_hash_flag[iPopLowNo] == 0)
				{
					/* 如果当前长度对BIN_INCREASES_AMOUNT_MEMORY取模等于0，那么表示当前内存已经满了，需要再增加BIN_INCREASES_AMOUNT_MEMORY内存 */
					if ((ptypeGroupInformationNow->low_no_number_length % BIN_INCREASES_AMOUNT_MEMORY) == 0)
					{
						/* 增加内存存储 */
						piLowNoReturn = (int *)realloc(ptypeGroupInformationNow->low_no, sizeof(int) * (ptypeGroupInformationNow->low_no_number_length + BIN_INCREASES_AMOUNT_MEMORY));
						if (piLowNoReturn == NULL)
						{
							printfLog("行数据转移到Bin结构体时，增加内存错误！\n");
							return NEW_BIN_FACNO_MEMORY_ERROR;
						}
						ptypeGroupInformationNow->low_no = piLowNoReturn;
					}
					ptypeGroupInformationNow->low_no[ptypeGroupInformationNow->low_no_number_length] = iPopLowNo;
					++ptypeGroupInformationNow->low_no_number_length;

					/* 当前区间扇区号FacNo的哈希表的相应位置1 */
					ptypeGroupInformationNow->low_hash_flag[iPopLowNo] = 1;
				}
				/* 当前行有当前区间的扇区，所以标志位置位 */
				ptypeGroupInformationNow->low_flag = 1;
				break;
			}
			/* 区间向大增加一个区间 */
			iScopeNow += iScope;
			ptypeGroupInformationNow = ptypeGroupInformationNow->next;
		}
	}


	/* 一行数据处理完成后查询是属于哪个组，链头不存储数据 */
	ptypeGroupInformationNow = ptypeGroupInformationHead->next;
	while (ptypeGroupInformationNow != NULL)
	{
		/* 如果当前区间标志置位，则把区间的总数加一 */
		if (ptypeGroupInformationNow->fac_flag != 0)
		{
			++ptypeGroupInformationNow->fac_number;
			iFacNoOrLowNoFlag |= 0x01;
		}
		/* 如果当前区间标志置位，则把区间的总数加一 */
		if (ptypeGroupInformationNow->low_flag != 0)
		{
			++ptypeGroupInformationNow->low_number;
			iFacNoOrLowNoFlag |= 0x02;
		}
		/* 区间标志清零 */
		ptypeGroupInformationNow->fac_flag = 0;
		ptypeGroupInformationNow->low_flag = 0;

		/* 前往下一个区间 */
		ptypeGroupInformationNow = ptypeGroupInformationNow->next;
	}


	/* 判断当前行结构体是否有FacNo和LowNo信息，如果有，就把相应的总数量加1 */
	if (iFacNoOrLowNoFlag & 0x01)
		++ptypeGroupInformationHead->fac_number;
	if (iFacNoOrLowNoFlag & 0x02)
		++ptypeGroupInformationHead->low_number;

	/* 不同类型的Bin组统计有多少个 */
	++ptypeGroupInformationHead->bin_number;

	return 0;
}


/*
* return void;
* input:
* function: 把哈希表中的一个链表行数据解析到不同的组Bin
*/
int fnBinGetOneLineList(BinType * ptypeBin, LineDataType * ptypeLineDataHead)
{
	/* 哈希表元素下的当前行结构体 */
	LineDataType * ptypeLineDataNow = NULL;

	/* 循环读取当前行结构体链表内的数据。（链头不存储数据） */
	ptypeLineDataNow = ptypeLineDataHead->next;
	while (ptypeLineDataNow != NULL)
	{
		if (strcmp(ptypeLineDataNow->bin, "A0") == 0)
		{
			/* 把当前行结构体解析到BinA0组 */
			if (fnBinGetLineData(ptypeBin->A0, ptypeLineDataNow, ptypeBin->scope) != 0)
			{
				return ROW_STRUCTURE_RESOLVED_TO_BIN_GROUP_ERROR;
			}
		}
		else if (strcmp(ptypeLineDataNow->bin, "A1") == 0)
		{
			/* 把当前行结构体解析到BinA1组 */
			if (fnBinGetLineData(ptypeBin->A1, ptypeLineDataNow, ptypeBin->scope) != 0)
			{
				return ROW_STRUCTURE_RESOLVED_TO_BIN_GROUP_ERROR;
			}
		}
		else if (strcmp(ptypeLineDataNow->bin, "B0") == 0)
		{
			/* 把当前行结构体解析到BinB0组 */
			if (fnBinGetLineData(ptypeBin->B0, ptypeLineDataNow, ptypeBin->scope) != 0)
			{
				return ROW_STRUCTURE_RESOLVED_TO_BIN_GROUP_ERROR;
			}
		}
		else if (strcmp(ptypeLineDataNow->bin, "B1") == 0)
		{
			/* 把当前行结构体解析到BinB1组 */
			if (fnBinGetLineData(ptypeBin->B1, ptypeLineDataNow, ptypeBin->scope) != 0)
			{
				return ROW_STRUCTURE_RESOLVED_TO_BIN_GROUP_ERROR;
			}
		}
		else if (strcmp(ptypeLineDataNow->bin, "C0") == 0)
		{
			/* 把当前行结构体解析到BinC0组 */
			if (fnBinGetLineData(ptypeBin->C0, ptypeLineDataNow, ptypeBin->scope) != 0)
			{
				return ROW_STRUCTURE_RESOLVED_TO_BIN_GROUP_ERROR;
			}
		}
		else if (strcmp(ptypeLineDataNow->bin, "C1") == 0)
		{
			/* 把当前行结构体解析到BinC1组 */
			if (fnBinGetLineData(ptypeBin->C1, ptypeLineDataNow, ptypeBin->scope) != 0)
			{
				return ROW_STRUCTURE_RESOLVED_TO_BIN_GROUP_ERROR;
			}
		}
		else if (strcmp(ptypeLineDataNow->bin, "E0") == 0)
		{
			/* 把当前行结构体解析到BinF0组 */
			if (fnBinGetLineData(ptypeBin->E0, ptypeLineDataNow, ptypeBin->scope) != 0)
			{
				return ROW_STRUCTURE_RESOLVED_TO_BIN_GROUP_ERROR;
			}
		}
		else if (strcmp(ptypeLineDataNow->bin, "F0") == 0)
		{
			/* 把当前行结构体解析到BinF0组 */
			if (fnBinGetLineData(ptypeBin->F0, ptypeLineDataNow, ptypeBin->scope) != 0)
			{
				return ROW_STRUCTURE_RESOLVED_TO_BIN_GROUP_ERROR;
			}
		}
		else if (strcmp(ptypeLineDataNow->bin, "F1") == 0)
		{
			/* 把当前行结构体解析到BinF1组 */
			if (fnBinGetLineData(ptypeBin->F1, ptypeLineDataNow, ptypeBin->scope) != 0)
			{
				return ROW_STRUCTURE_RESOLVED_TO_BIN_GROUP_ERROR;
			}
		}
		else if (strcmp(ptypeLineDataNow->bin, "F2") == 0)
		{
			/* 把当前行结构体解析到BinF1组 */
			if (fnBinGetLineData(ptypeBin->F2, ptypeLineDataNow, ptypeBin->scope) != 0)
			{
				return ROW_STRUCTURE_RESOLVED_TO_BIN_GROUP_ERROR;
			}
		}
		else if (strcmp(ptypeLineDataNow->bin, "F3") == 0)
		{
			/* 把当前行结构体解析到BinF1组 */
			if (fnBinGetLineData(ptypeBin->F3, ptypeLineDataNow, ptypeBin->scope) != 0)
			{
				return ROW_STRUCTURE_RESOLVED_TO_BIN_GROUP_ERROR;
			}
		}
		else
		{
			/* Bin组类型错误 */
			printfLog("错误的Bin组类型！类型为:%s\n", ptypeLineDataNow->bin);
		}

		/* 前往下一个结构体 */
		ptypeLineDataNow = ptypeLineDataNow->next;
	}

	return 0;
}
/*
* return void;
* input:
* function: 把哈希表中的一个链表行数据打印到文件
*/
int fnBinGetOneLineUIDSameList(FILE * pFileNewHand, LineDataType * ptypeLineDataHead)
{
	/* 哈希表元素下的当前行结构体 */
	LineDataType * ptypeLineDataNow = NULL;
	int i = 0, iTimesRepetitionSum = 0;

	/* 循环读取当前行结构体链表内的数据。（链头不存储数据） */
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

		/* 前往下一个结构体 */
		ptypeLineDataNow = ptypeLineDataNow->next;
	}

	return iTimesRepetitionSum;
}




/*
* return void;
* input:
* function: 把哈希表中的所有链表行数据发送到解析函数
*/
int fnBinGetHashLineList(BinType * ptypeBin, LineDataType * ptypeLineDataHashTable, int iHashTableLength)
{
	/* 各Bin组哈希表表头 */
	LineDataType * ptypeLineDataHead;
	int i = 0;


	/* 把哈希表内的每一个元素（头指针）传入函数 */
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
* function: 把哈希表中的所有链表行数据发送到打印函数
*/
int fnBinGetHashLineUIDSameList(FILE * pFileNewHand, LineDataType * ptypeLineDataHashTable, int iHashTableLength)
{
	/* 各Bin组哈希表表头 */
	LineDataType * ptypeLineDataHead;
	int i = 0, iTimesRepetitionSum = 0;


	/* 把哈希表内的每一个元素（头指针）传入函数 */
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
* function: 求解当前Bin组所有区间的百分比
*/
int fnBinPercentage(GroupInformationType * ptypeGroupInformationHead)
{
	GroupInformationType * ptypeGroupInformationNow = NULL;
	int iFacNoNunberSum = 0, iLowNoNumberSum = 0;

	/* 获取当前Bin组的坏扇区总数 */
	iFacNoNunberSum = ptypeGroupInformationHead->fac_number;
	iLowNoNumberSum = ptypeGroupInformationHead->low_number;

	/* 链表头除了坏扇区总数不存储其它数据 */
	ptypeGroupInformationNow = ptypeGroupInformationHead->next;
	while (ptypeGroupInformationNow != NULL)
	{
		/* 除数不为零判断 */
		if (iFacNoNunberSum != 0)
			ptypeGroupInformationNow->fac_percentage = ptypeGroupInformationNow->fac_number * 100.0f / (float)iFacNoNunberSum;
		else
			ptypeGroupInformationNow->fac_percentage = 0.0f;

		/* 除数不为零判断 */
		if (iLowNoNumberSum != 0)
			ptypeGroupInformationNow->low_percentage = ptypeGroupInformationNow->low_number * 100.0f / (float)iLowNoNumberSum;
		else
			ptypeGroupInformationNow->low_percentage = 0.0f;

		/* 前往下个区间 */
		ptypeGroupInformationNow = ptypeGroupInformationNow->next;
	}

	return 0;
}

/*
* return void;
* input:
* function: 求解当前所有Bin区间的百分比
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
* function: 对当前Bin组所有区间的FacNo与LowNo进行排序
*/
int fnBinSorting(GroupInformationType * ptypeGroupInformationHead)
{
	GroupInformationType * ptypeGroupInformationNow = NULL;
	int iFacNoNunberSum = 0, iLowNoNumberSum = 0;


	/* 链表头除了坏扇区总数不存储其它数据 */
	ptypeGroupInformationNow = ptypeGroupInformationHead->next;
	while (ptypeGroupInformationNow != NULL)
	{
		fnSelectSort(ptypeGroupInformationNow->fac_no, ptypeGroupInformationNow->fac_no_number_length);
		fnSelectSort(ptypeGroupInformationNow->low_no, ptypeGroupInformationNow->low_no_number_length);

		/* 前往下个区间 */
		ptypeGroupInformationNow = ptypeGroupInformationNow->next;
	}

	return 0;
}

/*
* return void;
* input:
* function: 所有Bin区间的FacNo与LowNo进行排序
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
* function: 打印当前Bin结构体内所有的区间数据到文件
*/
int fnBinGroupInformationPrintfToFile(FILE * pFileHand, GroupInformationType * ptypeGroupInformation, int iScope)
{
	GroupInformationType * ptypeGroupInformationNow = NULL;
	int iScopeNow = 0, iScopeOld = 0;
	int i;

	/* 打印fac_no信息 */
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

	/* 打印low_no信息 */
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
* function: 打印当前结构体内所有的Bin组结构体数据到文件
*/
int fnBinALLPrintfToFile(FILE * pFileHand, BinType * ptypeBinData, BinType * ptypeBinDataUIDSame)
{
	/* 写入表格标题 */
	fputs("Bin,Number,Same number\n", pFileHand);

	/* 打印Bin组号 */
	fputs("A0:,", pFileHand);
	/* 打印BinA0总样品数 */
	fprintf(pFileHand, "%d,", ptypeBinData->A0->bin_number);
	fprintf(pFileHand, "%d\n", ptypeBinDataUIDSame->A0->bin_number);
	/* 打印BinA0详细信息 */
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

/* 打印Bin组号 */
	fputs("Total:,", pFileHand);
	/* 打印BinA0总样品数 */
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
* function: 全局变量Bin结构体初始化
*/
int fnBinDataTypeInit(BinType * pbinType, int iScope)
{
	int i = 0, iSectionNumber = 0;
	GroupInformationType * ptypeGroupInformation = NULL;

	/* Bin区间最大值除以当前设置区间范围，得到要生成的区间个数 */
	iSectionNumber = ex_ConfigurationData.iSectorNumberMaximum / iScope;
	if (iSectionNumber < 1)
		return SET_BIN_INTERVAL_RANGE_ERROR;

	/* 如果没有除尽，那么就再增加一个区间，用以存储最后面的那些尾数 */
	if ((ex_ConfigurationData.iSectorNumberMaximum % iScope) != 0)
		++iSectionNumber;

	/* 初始化每个Bin组的头结构体，头结构体不放区间结构数据，只存放了相应Bin总数 */
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

	/* 记录区间范围 */
	pbinType->scope = iScope;

	/* iSectionNumber为：根据配置文件内的区间范围得到的区间个数， 并新建相应个区间数结构体插入到相应的链表*/
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
