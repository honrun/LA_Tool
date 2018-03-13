// DrivesLine.cpp : 行信息处理
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
* function: 新建一个行结构体
*/
int fnLineDataNew(LineDataType ** ppLineDataType)
{
	*ppLineDataType = (LineDataType *)malloc(sizeof(LineDataType));
	if (*ppLineDataType == NULL)
	{
		printfLog("\n新建行结构体失败！\n");
		return NEW_CONSTRUCTION_STRUCTURE_ERROR;
	}

	/* 清空结构体 */
	memset(*ppLineDataType, 0, sizeof(LineDataType));

	return 0;
}

/*
* return void;
* input:
* function: 释放一个行结构体
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
* function: 检查一个行结构体内的数据是否有错误
*/
int fnLineDataDetectionError(LineDataType * pLineDataType)
{
	int i = 0;
	static int iUIDIsAllFNumber = 0;

	if (strlen(pLineDataType->port) >= LINE_PORT_LENGTH_MAX)
	{
		printfLog("行结构体内读取到的Port长度错误！错误Port为:%s\n", pLineDataType->port);
		return LINE_PORT_LENGTH_ERROR;
	}

	if (strlen(pLineDataType->bin) >= LINE_BIN_LENGTH_MAX)
	{
		printfLog("行结构体内读取到的Bin长度错误！错误Bin为:%s\n", pLineDataType->bin);
		return LINE_BIN_LENGTH_ERROR;
	}

	/* 判断FacN的数据是否正确，长度不会大于能够出现的所有扇区数之和，并且长度会等于后面所带有的FacNo的扇区号数 */
	if ((pLineDataType->fac_number > ex_ConfigurationData.iSectorNumberMaximum) || (pLineDataType->fac_number != pLineDataType->fac_no_number_length))
	{
		printfLog("行结构体内读取到的FacN长度错误！错误FacN长度为:%d\n", pLineDataType->fac_number);
		printfLog("行结构体内读取到的FacNo长度错误！错误FacNo长度为:%d\n", pLineDataType->fac_no_number_length);
		printfLog("行时间为: %s\n", pLineDataType->time);
		printfLog("UID为: %s\n", pLineDataType->flashUID);
		printfLog("Bin为: %s\n\n", pLineDataType->bin);
//		return LINE_FACNO_LENGTH_ERROR;
	}

	/* 判断LowN的数据是否正确，长度不会大于能够出现的所有扇区数之和，并且长度会等于后面所带有的LowNo的扇区号数 */
	if ((pLineDataType->low_number > ex_ConfigurationData.iSectorNumberMaximum) || (pLineDataType->low_number != pLineDataType->low_no_number_length))
	{
		printfLog("行结构体内读取到的LowN长度错误！错误LowN长度为:%d\n", pLineDataType->low_number);
		printfLog("行结构体内读取到的LowNo长度错误！错误LowNo长度为:%d\n", pLineDataType->low_no_number_length);
		printfLog("行时间为: %s\n", pLineDataType->time);
		printfLog("UID为: %s\n", pLineDataType->flashUID);
		printfLog("Bin为: %s\n\n", pLineDataType->bin);
//		return LINE_LOWNO_LENGTH_ERROR;
	}

	for (i = 0; i < pLineDataType->fac_no_number_length; ++i)
	{
		if(pLineDataType->fac_no[i] >= ex_ConfigurationData.iSectorNumberMaximum)
		{
			printfLog("行结构体内读取到的FacNo扇区号错误！错误扇区号为:%d\n", pLineDataType->fac_no[i]);
			return FACNO_INFORMATION_ERROR;
		}
	}

	for (i = 0; i < pLineDataType->low_no_number_length; ++i)
	{
		if (pLineDataType->low_no[i] >= ex_ConfigurationData.iSectorNumberMaximum)
		{
			printfLog("行结构体内读取到的FacNo扇区号错误！错误扇区号为:%d\n", pLineDataType->low_no[i]);
			return LOWNO_INFORMATION_ERROR;
		}
	}

	if (_stricmp(pLineDataType->flashUID, "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF") == 0)
	{
		/* UID为全F，但Bin不是F0，打印到LOG文件 */
		if (_stricmp(pLineDataType->bin, "F0") != 0)
		{
			printfLog("读取到UID为全F，但Bin不为F0的行数据\n");
			printfLog("行时间为: %s\n", pLineDataType->time);
			printfLog("Bin为: %s\n\n", pLineDataType->bin);
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
* function: 把新建的行结构体插入到链表表头
*/
int fnLineDataHeadInsert(LineDataType * ptypeHead, LineDataType * ptypeData)
{
	if (ptypeHead == NULL)
	{
		printfLog("行结构体插入链表表头错误！\n");
		return -1;
	}

	if (ptypeData == NULL)
	{
		printfLog("行结构体插入链表表尾为空插入！\n");
		return -1;
	}

	ptypeData->next = ptypeHead->next;
	ptypeHead->next = ptypeData;

	return 0;
}

/*
* return void;
* input:
* function: 把新建的行结构体插入到链表表尾
*/
int fnLineDataEndInsert(LineDataType * ptypeHead, LineDataType * ptypeData)
{
	LineDataType * ptypeTemp;

	if (ptypeHead == NULL)
	{
		printfLog("行结构体插入链表表尾错误！\n");
		return -1;
	}

	if (ptypeData == NULL)
	{
		printfLog("行结构体插入链表表尾为空插入！\n");
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
* function: 把当前结构体插入到链表，并去除重复
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
		/* 先比较Hash值是否相同。如果不相同，直接查找下个元素；如果相同，再比较UID、时间等情况 */
		if (ptypeLineDataNow->uiHash != ptypeData->uiHash)
		{
			ptypeLineDataPrevious = ptypeLineDataNow;
			ptypeLineDataNow = ptypeLineDataNow->next;
		}
		else
		{
			/* 获取UID字符串比较值 */
			iUIDComparingStrcmp = strcmp(ptypeData->flashUID, ptypeLineDataNow->flashUID);

			/* 要插入的UID比当前UID大，就再向后移位进行比较 */
			if (iUIDComparingStrcmp > 0)
			{
				ptypeLineDataPrevious = ptypeLineDataNow;
				ptypeLineDataNow = ptypeLineDataNow->next;
			}
			/* 要插入的UID比当前UID小，就插入到当前结构体的前面，并退出当前的查找 */
			else if (iUIDComparingStrcmp < 0)
			{
				ptypeLineDataPrevious->next = ptypeData;
				ptypeData->next = ptypeLineDataNow;

				fnLineUIDSameAddRepetition(ptypeData, ptypeData->bin, ptypeData->path_and_name);

				/* 插入新的结构体，并且没有释放旧结构体，就返回NEW_INSERT_ROW_STRUCTURE_SUCCEEDS */
				return NEW_INSERT_ROW_STRUCTURE_SUCCEEDS;
			}
			/* 有相同的UID，那么就要释放掉其中的某一个 */
			else
			{
				fnLineUIDSameAddRepetition(ptypeLineDataNow, ptypeData->bin, ptypeData->path_and_name);

				/* 获取时间比较值 */
				iTimeComparingStrcmp = strcmp(ptypeData->time, ptypeLineDataNow->time);

				/* 如果时间大于或者等于，表示当前数据较新，就把形参插入到当前结构体的前面，并释放当前查找到的结构体，退出当前的查找 */
				if (iTimeComparingStrcmp >= 0)
				{
					ptypeData->times_of_repetition = ptypeLineDataNow->times_of_repetition;
					ptypeData->UID_same_path_and_name = ptypeLineDataNow->UID_same_path_and_name;

					/* 把新建的结构体插入链表 */
					ptypeLineDataPrevious->next = ptypeData;
					ptypeData->next = ptypeLineDataNow->next;

					/* 释放当前查找到的结构体 */
					fnLineDataDelete(&ptypeLineDataNow);
					/* 新插入成功，并把以前相同UID的信息释放，就返回RELEASE_OLD_ROW_STRUCTURE */
					return RELEASE_OLD_ROW_STRUCTURE;
				}
				else
				{
					/* 释放准备要插入的新建结构体 */
					fnLineDataDelete(&ptypeData);
					/* 新插入失败，以前相同UID的信息不变动，就返回RELEASE_NEW_ROW_STRUCTURE */
					return RELEASE_NEW_ROW_STRUCTURE;
				}
			}
		}
	}

	fnLineUIDSameAddRepetition(ptypeData, ptypeData->bin, ptypeData->path_and_name);

	/* 当前链表中没有相同的UID并且指针已经到了链表的末尾 */
	ptypeLineDataPrevious->next = ptypeData;

	return NEW_INSERT_ROW_STRUCTURE_SUCCEEDS;
}

/*
* return void;
* input:
* function: 把从文件读取到的行数据解析到行结构体
* 行内容样例：
EC00DEFAFF4132363034303201010711	Port5	A1	4	1	2060 2061 4094 4095 ;	1208 ;
*/
int fnLineParsing(LineDataType * ptypeLineData, char * pszLineString)
{
	/* 行当前所剩下的长度 */
	size_t iLineLengthRemaining = 0;
	/* 行信息当前的读取指针，行信息复制返回指针 */
	char * pszLineStringNow = NULL, *pszLineStringReturn = NULL;

	pszLineStringNow = pszLineString;

	/* 程序之前已经判断了行的长度 */
#if 0
	/* 判断行的长度，是否小于最小长度，根据行的固定格式 */
	iLineLengthRemaining = strlen(pszLineStringNow);
	if (iLineLengthRemaining < ex_ConfigurationData.iLineLengthMin)
	{
		/* 打印出错行 */
		printfLog("读取到的行长度 %d 出错！ %s\n", (int)iLineLengthRemaining, pszLineString);
		return LINE_DATA_LENGTH_ERROR;
	}
#endif

	/* 计算行信息中UID的长度，防止文件出错时，UID过长 */
	iLineLengthRemaining = strlen(pszLineStringNow);
	pszLineStringReturn = (char *)memchr(pszLineStringNow, '\t', iLineLengthRemaining);
	if (pszLineStringReturn == NULL)
	{
		/* 打印出错行 */
		printfLog("文件UID读取出错！ %s\n", pszLineString);
		return LINE_UID_READ_ERROR;
	}
	/* 计算行信息中UID的长度，防止文件出错时，UID过长，UID最长长度为LINE_UID_LENGTH_MAX */
	if((pszLineStringReturn - pszLineStringNow) > LINE_UID_LENGTH_MAX)
	{
		/* 打印出错行 */
		printfLog("读取到的UID长度出错！ %s\n", pszLineString);
		return LINE_UID_LENGTH_ERROR;
	}
	/* 复制行信息中的UID信息到结构体，并返回szLineString行信息‘\t’字符后的指针 */
	pszLineStringReturn = (char *)_memccpy(ptypeLineData->flashUID, pszLineStringNow, '\t', iLineLengthRemaining);
	/* 去掉最后面的字符‘\t’ */
	ptypeLineData->flashUID[strlen(ptypeLineData->flashUID) - 1] = 0;
	/* 移动指针到字符‘\t’的位置，跳过UID为获取Port做准备 */
	pszLineStringNow = (char *)memchr(pszLineStringNow, '\t', iLineLengthRemaining);
	/* 跳过‘\t’字符 */
	pszLineStringNow++;


	/* 计算行信息中Port的长度，防止文件出错时，Port过长 */
	iLineLengthRemaining = strlen(pszLineStringNow);
	pszLineStringReturn = (char *)memchr(pszLineStringNow, '\t', iLineLengthRemaining);
	if (pszLineStringReturn == NULL)
	{
		/* 打印出错行 */
		printfLog("文件Port读取出错！ %s\n", pszLineString);
		return LINE_PORT_READ_ERROR;
	}
	/* 计算行信息中Port的长度，防止文件出错时，Port过长，Port最长长度为LINE_PORT_LENGTH_MAX */
	if ((pszLineStringReturn - pszLineStringNow) > LINE_PORT_LENGTH_MAX)
	{
		/* 打印出错行 */
		printfLog("读取到的Port长度出错！ %s\n", pszLineString);
		return LINE_PORT_LENGTH_ERROR;
	}
	/* 复制行信息中的Port信息到结构体，并返回szLineString行信息‘\t’字符后的指针 */
	pszLineStringReturn = (char *)_memccpy(ptypeLineData->port, pszLineStringNow, '\t', iLineLengthRemaining);
	/* 去掉最后面的字符‘\t’ */
	ptypeLineData->port[strlen(ptypeLineData->port) - 1] = 0;
	/* 移动指针到字符‘\t’的位置，跳过Port为获取Bin做准备 */
	pszLineStringNow = (char *)memchr(pszLineStringNow, '\t', iLineLengthRemaining);
	/* 跳过‘\t’字符 */
	pszLineStringNow++;


	/* 计算行信息中Bin的长度，防止文件出错时，Bin过长 */
	iLineLengthRemaining = strlen(pszLineStringNow);
	pszLineStringReturn = (char *)memchr(pszLineStringNow, '\t', iLineLengthRemaining);
	if (pszLineStringReturn == NULL)
	{
		/* 打印出错行 */
		printfLog("文件Bin读取出错！ %s\n", pszLineString);
		return LINE_BIN_READ_ERROR;
	}
	/* Bin最长长度为LINE_BIN_LENGTH_MAX */
	if ((pszLineStringReturn - pszLineStringNow) > LINE_BIN_LENGTH_MAX)
	{
		/* 打印出错行 */
		printfLog("读取到的Bin长度出错！ %s\n", pszLineString);
		return LINE_BIN_LENGTH_ERROR;
	}
	/* 复制行信息中的Bin信息到结构体，并返回szLineString行信息‘\t’字符后的指针 */
	pszLineStringReturn = (char *)_memccpy(ptypeLineData->bin, pszLineStringNow, '\t', iLineLengthRemaining);
	/* 去掉最后面的字符‘\t’ */
	ptypeLineData->bin[strlen(ptypeLineData->bin) - 1] = 0;
	/* 移动指针到字符‘\t’的位置，跳过Bin为获取时间做准备 */
	pszLineStringNow = (char *)memchr(pszLineStringNow, '\t', iLineLengthRemaining);
	/* 跳过‘\t’字符 */
	pszLineStringNow++;


	if (ex_ConfigurationData.iLineTimeExist != 0)
	{
		/* 计算行信息中时间的长度，防止文件出错时，时间过长 */
		iLineLengthRemaining = strlen(pszLineStringNow);
		pszLineStringReturn = (char *)memchr(pszLineStringNow, '\t', iLineLengthRemaining);
		if (pszLineStringReturn == NULL)
		{
			/* 打印出错行 */
			printfLog("文件时间读取出错！ %s\n", pszLineString);
			return LINE_BIN_READ_ERROR;
		}
		/* Bin最长长度为LINE_BIN_LENGTH_MAX */
		if ((pszLineStringReturn - pszLineStringNow) > LINE_BIN_LENGTH_MAX)
		{
			/* 打印出错行 */
			printfLog("读取到的时间长度出错！ %s\n", pszLineString);
			return LINE_BIN_LENGTH_ERROR;
		}
		/* 复制行信息中的时间信息到结构体，并返回szLineString行信息‘\t’字符后的指针 */
		pszLineStringReturn = (char *)_memccpy(ptypeLineData->time, pszLineStringNow, '\t', iLineLengthRemaining);
		/* 去掉最后面的字符‘\t’ */
		ptypeLineData->time[strlen(ptypeLineData->time) - 1] = 0;
		/* 移动指针到字符‘\t’的位置，跳过时间为获取FacN做准备 */
		pszLineStringNow = (char *)memchr(pszLineStringNow, '\t', iLineLengthRemaining);
		/* 跳过‘\t’字符 */
		pszLineStringNow++;
	}

	if (ex_ConfigurationData.iLineFacExist != 0)
	{
		/* 获取FacN的个数 */
		ptypeLineData->fac_number = atoi(pszLineStringNow);
		/* 移动指针到字符‘\t’的位置，跳过FacN为获取LowN做准备 */
		iLineLengthRemaining = strlen(pszLineStringNow);
		pszLineStringNow = (char *)memchr(pszLineStringNow, '\t', iLineLengthRemaining);
		if (pszLineStringNow == NULL)
		{
			/* 打印出错行 */
			printfLog("获取FacN的个数出错！ %s\n", pszLineString);
			return LINE_FACN_READ_ERROR;
		}
		/* 跳过‘\t’字符 */
		pszLineStringNow++;
	}


	if (ex_ConfigurationData.iLineLowExist != 0)
	{
		/* 获取LowN的个数 */
		ptypeLineData->low_number = atoi(pszLineStringNow);
		/* 移动指针到字符‘\t’的位置，跳过LowN为获取FacNo做准备 */
		iLineLengthRemaining = strlen(pszLineStringNow);
		pszLineStringNow = (char *)memchr(pszLineStringNow, '\t', iLineLengthRemaining);
		if (pszLineStringNow == NULL)
		{
			/* 打印出错行 */
			printfLog("获取LowN的个数出错！ %s\n", pszLineString);
			return LINE_LOWN_READ_ERROR;
		}
		/* 跳过‘\t’字符 */
		pszLineStringNow++;
	}


	if (ex_ConfigurationData.iLineFacExist != 0)
	{
		/* 计算行信息中FacNo的长度，防止文件出错时，FacNo过长 */
		iLineLengthRemaining = strlen(pszLineStringNow);
		pszLineStringReturn = (char *)memchr(pszLineStringNow, ';', iLineLengthRemaining);
		if (pszLineStringReturn == NULL)
		{
			/* 打印出错行 */
			printfLog("文件FacNo读取出错！ %s\n", pszLineString);
			return LINE_FACNO_READ_ERROR;
		}
		/* 计算行信息中FacNo的长度，防止文件出错时，FacNo过长，FacNo最长长度为 LINE_STRING_LENGTH */
		if ((pszLineStringReturn - pszLineStringNow) > ex_ConfigurationData.iFacNoLengthMax)
		{
			/* 打印出错行 */
			printfLog("读取到的FacNo长度出错！ %s\n", pszLineString);
			return LINE_FACNO_LENGTH_ERROR;
		}
		/* 把字符串中的扇区号转换成数值存储到行结构体中的fac_no */
		if (fnLineNoConversionToNumber(&ptypeLineData->fac_no, &ptypeLineData->fac_no_number_length, pszLineStringNow) != 0)
		{
			return FACNO_ALLOCATION_MEMORY_ERROR;
		}
		/* 移动指针到字符‘;’的位置，跳过FacNo为获取LowNo做准备 */
		pszLineStringNow = (char *)memchr(pszLineStringNow, ';', iLineLengthRemaining);
		/* 跳过‘;’字符 */
		pszLineStringNow++;
		/* 跳过‘\t’字符 */
		pszLineStringNow++;
	}


	if (ex_ConfigurationData.iLineLowExist != 0)
	{
		/* 计算行信息中LowNo的长度，防止文件出错时，LowNo过长 */
		iLineLengthRemaining = strlen(pszLineStringNow);
		pszLineStringReturn = (char *)memchr(pszLineStringNow, ';', iLineLengthRemaining);
		if (pszLineStringReturn == NULL)
		{
			/* 打印出错行 */
			printfLog("文件LowNo读取出错！ %s\n", pszLineString);
			return LINE_LOWNO_READ_ERROR;
		}
		/* 计算行信息中LowNo的长度，防止文件出错时，LowNo过长，LowNo最长长度为 LINE_STRING_LENGTH */
		if ((pszLineStringReturn - pszLineStringNow) > ex_ConfigurationData.iLowNoLengthMax)
		{
			/* 打印出错行 */
			printfLog("读取到的LowNo长度出错！ %s\n", pszLineString);
			return LINE_LOWNO_LENGTH_ERROR;
		}

		/* 把字符串中的扇区号转换成数值存储到行结构体中的low_no */
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
* function: 把FacNo或者LowNo中的扇区号字符串形式转换成int数值
*/
int fnLineNoConversionToNumber(int ** ppiNumberBuff, int * piNumberLength, char * pszNoString)
{
	/* 当前读取到的扇区数 */
	int iCumulativeNumber = 0;
	int * piNumberNow = NULL, * piNumbersReturn = NULL;
	char * pszNoStringReturn = NULL;


	piNumberNow = *ppiNumberBuff;

	pszNoStringReturn = (char *)strchr(pszNoString, ';');
	if (pszNoString == pszNoStringReturn)
		return 0;

	/* 查找到字符‘空格’位置 */
	pszNoStringReturn = (char *)strchr(pszNoString, ' ');
	while (pszNoStringReturn != NULL)
	{
		/* 查找到的扇区数加1 */
		++iCumulativeNumber;

		/* 增加内存存储 */
		piNumbersReturn = (int *)realloc(piNumberNow, sizeof(int) * iCumulativeNumber);
		if (piNumbersReturn == NULL)
		{
			/* 释放原指针 */
			free(piNumberNow);
			printfLog("为No分配内存错误！\n");
			return NO_ALLOCATION_MEMORY_ERROR;
		}

		piNumberNow = piNumbersReturn;

		/* 把字符串转换成数值 */
		piNumberNow[iCumulativeNumber - 1] = atoi(pszNoString);

		/* 跳过当前查找到的字符‘空格’ */
		pszNoString = pszNoStringReturn + 1;
		/* 如果指针到了字符‘;’表示已经到了No字符串的末尾 */
		if (*pszNoString == ';')
			break;

		/* 查找到字符‘空格’位置 */
		pszNoStringReturn = (char *)strchr(pszNoString, ' ');
	}

	*ppiNumberBuff = piNumberNow;
	*piNumberLength = iCumulativeNumber;

	return 0;
}

