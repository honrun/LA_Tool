// Transfer format.cpp : �������̨Ӧ�ó������ڵ㡣
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


/* ���ƴ�����ʾ����Ϣ��������ļ�ָ�� */
FILE * ex_TransferFormatFileHand = NULL;
/* �����ļ�ָ�� */
FILE * ex_ConfigurationFileHand = NULL;
/* �������ļ��л�ȡ�����ݣ���������ΪĬ������ */
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
	/* ����ʼʱ��ʱ�䣬�������ʱ��ʱ�� */
	time_t typeStartTime = 0, typeStopTime = 0;
	/* ����ʼʱ��ʱ���ַ������棬�������ʱ��ʱ���ַ������� */
	char szStartTimeStringBuff[64] = { 0 }, szStopTimeStringBuff[64] = { 0 };

	/* ����record.log���ı��ļ����ѿ��ƴ�����ʾ����Ϣ��������ļ� */
	fopen_s(&ex_TransferFormatFileHand, "record.log", "w");
	if (ex_TransferFormatFileHand == NULL)
	{
		printfLog("����record.log����\n");
		/* ��Ļ��ʾ��ʾ�˳��ź� */
		system("pause");
		return -1;
	}

	{
		/* ��¼����ʼʱ��ʱ�� */
		time(&typeStartTime);
		ctime_s(szStartTimeStringBuff, sizeof(szStartTimeStringBuff), &typeStartTime);
		printfLog("Time Start is: %s", szStartTimeStringBuff);
	}


	/* �������ļ� */
	fopen_s(&ex_ConfigurationFileHand, "configuration.ini", "r");
	if(ex_ConfigurationFileHand == NULL)
	{
		printfLog("��ǰĿ¼û�������ļ����½�configuration.ini\n");
		/* �½������ļ� */
		fopen_s(&ex_ConfigurationFileHand, "configuration.ini", "w");
		if (ex_ConfigurationFileHand == NULL)
		{
			printfLog("�½������ļ�ʧ�ܣ�\n");
			return -1;
		}
		/* ��Ĭ������д���ļ� */
		fnFileConfigurationWrite(&ex_ConfigurationData, ex_ConfigurationFileHand);
		/* �ر������ļ� */
		fclose(ex_ConfigurationFileHand);
	}
	else if (fnFileConfigurationRead(&ex_ConfigurationData, ex_ConfigurationFileHand) != 0)
	{
		/* �رն�ȡ�����ļ� */
		fclose(ex_ConfigurationFileHand);
		printfLog("��ǰĿ¼�����ļ��д����½� configuration.ini\n");
		/* �½������ļ� */
		fopen_s(&ex_ConfigurationFileHand, "configuration.ini", "w");
		if (ex_ConfigurationFileHand == NULL)
		{
			printfLog("�½������ļ�ʧ�ܣ�\n");
			return -1;
		}
		/* ������д���ļ� */
		fnFileConfigurationWrite(&ex_ConfigurationData, ex_ConfigurationFileHand);
		/* �ر������ļ� */
		fclose(ex_ConfigurationFileHand);
	}

	{
		/* ��ȡ�������ļ����ʱ�� */
		time(&typeStopTime);
		ctime_s(szStopTimeStringBuff, sizeof(szStopTimeStringBuff), &typeStopTime);
		printfLog("Time Read configuration.ini is: %s", szStopTimeStringBuff);
	}


	/* Ƕ��ɨ�������ļ� */
	fnFileScan("."); 

	{
		/* ��ȡǶ��ɨ�������ĺ��ʱ�� */
		time(&typeStopTime);
		ctime_s(szStopTimeStringBuff, sizeof(szStopTimeStringBuff), &typeStopTime);
		printfLog("Time fnFileScan is: %s", szStopTimeStringBuff);
	}


	/* ���ظ�UID�����������ļ� */
	fnVersionUIDSameLineToFile(&ex_typeVersionHead);

	/* �ѹ�ϣ���ڵ���������Bin�ṹ�� */
	fnVersionLineToBin(&ex_typeVersionHead);


	{
		/* ��ȡ�ѹ�ϣ���ڵ���������Bin�ṹ����ʱ�� */
		time(&typeStopTime);
		ctime_s(szStopTimeStringBuff, sizeof(szStopTimeStringBuff), &typeStopTime);
		printfLog("Time fnBinGetHashLineList is: %s", szStopTimeStringBuff);
	}


	/* ��Bin�ṹ����������������������ļ� */
	fnVersionALLToFile(&ex_typeVersionHead);


	{
		/* ��¼�ر��½�����������ļ����ʱ�� */
		time(&typeStopTime);
		ctime_s(szStopTimeStringBuff, sizeof(szStopTimeStringBuff), &typeStopTime);
		printfLog("Time Stop is: %s", szStopTimeStringBuff);
	}

	{
		/* ��Ļ��ӡ�����ܹ���ʱʱ�� */
		printfLog("�����ܹ���ʱ: %.0lf �롣\n", difftime(typeStopTime, typeStartTime));
	}

	printfLog("\n\n�ļ�������ϣ����ڵ�ǰĿ¼�µ� Log Analysis Results �ļ����²鿴��Ӧ����ͳ�ƽ����\n\n");

	/* �ر��½��Ĵ�����Ϣ����ļ� */
	fclose(ex_TransferFormatFileHand);

	/* ��Ļ��ʾ��ʾ�˳��ź� */
	system("pause");

	return 0;
}


/*
* return void;
* input: piData:Ҫ������������ݻ���; iDataLength: �������ݳ���;
* function: ѡ������
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
