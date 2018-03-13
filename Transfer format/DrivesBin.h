#ifndef _DRIVERS_BIN_H_
#define _DRIVERS_BIN_H_

#include "DrivesLine.h"



#ifndef BIN_SECTION_RANGE_MAX
#define BIN_SECTION_RANGE_MAX 65536
#else
#error "BIN_SECTION_RANGE_MAX �Ѿ������壡"
#endif




#define BIN_INCREASES_AMOUNT_MEMORY 128






/* �����Bin�Ľṹ�� */
typedef struct GroupInformationStruct {
	int * fac_no;										//ԭ�������
	int * low_no;										//�޸��󻵿��
	int fac_no_number_length;							//��¼��ǰ�ṹ�����ж��ٸ���ͬ��ԭ������ţ���������int * fac_no;�����ڴ�ʱ������Сʹ��
	int low_no_number_length;							//��¼��ǰ�ṹ�����ж��ٸ���ͬ���޸��󻵿�ţ���������int * low_no;�����ڴ�ʱ������Сʹ��

	int fac_number;										//ԭ����ǰ����Ļ�������
	int low_number;										//�޸���ǰ����Ļ�������

	float fac_percentage;								//ԭ�����鵱ǰ����ٷֱ�
	float low_percentage;								//�޸��󻵿鵱ǰ����ٷֱ�

	char * fac_hash_flag;								//��ǰ����������FacNo�Ĺ�ϣ��
	char * low_hash_flag;								//��ǰ����������LowNo�Ĺ�ϣ��
	int fac_flag;										//ԭ�������־��
	int low_flag;										//�޸��󻵿��־��

	int bin_number;										//ͷ�ṹ��������������ã�������¼����Bin������

	struct GroupInformationStruct * next;
	struct GroupInformationStruct * previous;
}GroupInformationType;


/* ����Bin����Ľṹ�� */
typedef struct binStruct {
	GroupInformationType * A0;          //A0��
	GroupInformationType * A1;          //A1��
	GroupInformationType * B0;          //B0��
	GroupInformationType * B1;          //B1��
	GroupInformationType * C0;          //C0��
	GroupInformationType * C1;          //C1��
	GroupInformationType * E0;          //E0��
	GroupInformationType * F0;          //F0��
	GroupInformationType * F1;          //F1��
	GroupInformationType * F2;          //F2��
	GroupInformationType * F3;          //F3��

	int scope;                          //�����ڷ�Χ��

	struct binStruct * next;
	struct binStruct * previous;
}BinType;




extern BinType ex_typeBinData;


int fnBinGroupInformationNew(GroupInformationType ** ppGroupInformationType);

int fnBinInsertGroupInformation(GroupInformationType * ptypeHead, GroupInformationType * ptypeData);

int fnBinGetLineData(GroupInformationType * ptypeGroupInformation, LineDataType * ptypeLineData, int iScope);

int fnBinGetHashLineList(BinType * ptypeBin, LineDataType * ptypeLineDataHashTable, int iHashTableLength);

int fnBinGetHashLineUIDSameList(FILE * pFileNewHand, LineDataType * ptypeLineDataHashTable, int iHashTableLength);

int fnBinALLPercentage(BinType * ptypeBin);

int fnBinALLSorting(BinType * ptypeBin);

int fnBinGroupInformationShow(GroupInformationType * ptypeGroupInformation);

int fnBinALLPrintfToFile(FILE * pFileHand, BinType * ptypeBinData, BinType * ptypeBinDataUIDSame);

int fnBinALLShow(BinType * ptypeBin);

int fnBinDataTypeInit(BinType * pbinType, int iScope);


#endif
