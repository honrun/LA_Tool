// Transfer format.cpp : 定义控制台应用程序的入口点。
//


#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <windows.h>
#include <time.h>  

#include "DrivesBin.h"
#include "DrivesFile.h"
#include "DrivesHash.h"
#include "DrivesLine.h"
#include "DrivesVersion.h"
#include "TransferFormat.h"


/* 控制窗口显示的信息输出到此文件指针 */
FILE * ex_TransferFormatFileHand = NULL;
/* 配置文件指针 */
FILE * ex_ConfigurationFileHand = NULL;
/* 从配置文件中获取的数据，以下数据为默认数据 */
ConfigurationType ex_ConfigurationData = {
1024, 
4096, 
HASH_TABLE_LINE_LENGTH, 
LINE_FACNO_LENGTH_MAX, 
LINE_LOWNO_LENGTH_MAX, 
LINE_STRING_LENGTH, 
LINE_STRING_LENGTH_MIN, 

FILE_NAME_VERSION_EXIST, 
FILE_NAME_BATCH_EXIST, 
LINE_FAC_EXIST, 
LINE_LOW_EXIST, 
LINE_TIME_EXIST,
};




int main(int argc, char *argv[])
{
	/* 程序开始时的时间，程序结束时的时间 */
	time_t typeStartTime = 0, typeStopTime = 0;
	/* 程序开始时的时间字符串缓存，程序结束时的时间字符串缓存 */
	char szStartTimeStringBuff[64] = { 0 }, szStopTimeStringBuff[64] = { 0 };

	/* 创建record.log新文本文件，把控制窗口显示的信息输出到此文件 */
	fopen_s(&ex_TransferFormatFileHand, "record.log", "w");
	if (ex_TransferFormatFileHand == NULL)
	{
		printfLog("创建record.log错误！\n");
		/* 屏幕显示提示退出信号 */
		system("pause");
		return -1;
	}

	{
		/* 记录程序开始时的时间 */
		time(&typeStartTime);
		ctime_s(szStartTimeStringBuff, sizeof(szStartTimeStringBuff), &typeStartTime);
		printfLog("Time Start is: %s", szStartTimeStringBuff);
	}


	/* 打开配置文件 */
	fopen_s(&ex_ConfigurationFileHand, "configuration.ini", "r");
	if(ex_ConfigurationFileHand == NULL)
	{
		printfLog("当前目录没有配置文件，新建configuration.ini\n");
		/* 新建配置文件 */
		fopen_s(&ex_ConfigurationFileHand, "configuration.ini", "w");
		if (ex_ConfigurationFileHand == NULL)
		{
			printfLog("新建配置文件失败！\n");
			return -1;
		}
		/* 把默认配置写入文件 */
		fnFileConfigurationWrite(&ex_ConfigurationData, ex_ConfigurationFileHand);
		/* 关闭配置文件 */
		fclose(ex_ConfigurationFileHand);
	}
	else if (fnFileConfigurationRead(&ex_ConfigurationData, ex_ConfigurationFileHand) != 0)
	{
		/* 关闭读取配置文件 */
		fclose(ex_ConfigurationFileHand);
		printfLog("当前目录配置文件有错误，新建 configuration.ini\n");
		/* 新建配置文件 */
		fopen_s(&ex_ConfigurationFileHand, "configuration.ini", "w");
		if (ex_ConfigurationFileHand == NULL)
		{
			printfLog("新建配置文件失败！\n");
			return -1;
		}
		/* 把配置写入文件 */
		fnFileConfigurationWrite(&ex_ConfigurationData, ex_ConfigurationFileHand);
		/* 关闭配置文件 */
		fclose(ex_ConfigurationFileHand);
	}

	{
		/* 获取打开配置文件后的时间 */
		time(&typeStopTime);
		ctime_s(szStopTimeStringBuff, sizeof(szStopTimeStringBuff), &typeStopTime);
		printfLog("Time Read configuration.ini is: %s", szStopTimeStringBuff);
	}


	/* 嵌套扫描所有文件 */
	fnFileScan("."); 

	{
		/* 获取嵌套扫描所有文后的时间 */
		time(&typeStopTime);
		ctime_s(szStopTimeStringBuff, sizeof(szStopTimeStringBuff), &typeStopTime);
		printfLog("Time fnFileScan is: %s", szStopTimeStringBuff);
	}


	/* 把重复UID的情况输出到文件 */
	fnVersionUIDSameLineToFile(&ex_typeVersionHead);

	/* 把哈希表内的内容整理到Bin结构体 */
	fnVersionLineToBin(&ex_typeVersionHead);


	{
		/* 获取把哈希表内的内容整理到Bin结构体后的时间 */
		time(&typeStopTime);
		ctime_s(szStopTimeStringBuff, sizeof(szStopTimeStringBuff), &typeStopTime);
		printfLog("Time fnBinGetHashLineList is: %s", szStopTimeStringBuff);
	}


	/* 把Bin结构体整理后的所有数据输出到文件 */
	fnVersionALLToFile(&ex_typeVersionHead);


	{
		/* 记录关闭新建的整理输出文件后的时间 */
		time(&typeStopTime);
		ctime_s(szStopTimeStringBuff, sizeof(szStopTimeStringBuff), &typeStopTime);
		printfLog("Time Stop is: %s", szStopTimeStringBuff);
	}

	{
		/* 屏幕打印程序总共用时时间 */
		printfLog("程序总共用时: %.0lf 秒。\n", difftime(typeStopTime, typeStartTime));
	}

	printfLog("\n\n文件分析完毕，请在当前目录下的 Log Analysis Results 文件夹下查看对应分析统计结果！\n\n");

	/* 关闭新建的窗口信息输出文件 */
	fclose(ex_TransferFormatFileHand);

	/* 屏幕显示提示退出信号 */
	system("pause");

	return 0;
}


/*
* return void;
* input: piData:要进行排序的数据缓存; iDataLength: 缓存数据长度;
* function: 选择排序
*/
void fnSelectSort(int * piData, int iDataLength)
{
	int i = 0, j = 0, k = 0, iDataTemp = 0;

	for (i = 0; i < iDataLength - 1; ++i)
	{
		k = i;

		for (j = i + 1; j < iDataLength; ++j)
		{
			if (piData[k] > piData[j])
				k = j;
		}

		if (i != k)
		{
			iDataTemp = piData[i];
			piData[i] = piData[k];
			piData[k] = iDataTemp;
		}
	}
}
