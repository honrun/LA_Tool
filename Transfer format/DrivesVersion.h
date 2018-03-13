#ifndef _DrivesVersion_H_
#define _DrivesVersion_H_

#include "DrivesLine.h"
#include "DrivesBin.h"



/* �汾����󳤶� */
#ifndef VERSION_NAME_LENGTH_MAX
#define VERSION_NAME_LENGTH_MAX 64
#endif




/* �ļ��汾�Ľṹ�� */
typedef struct VersionStruct {
	char version[VERSION_NAME_LENGTH_MAX];					//�汾��
	char batch[VERSION_NAME_LENGTH_MAX];					//����
	LineDataType * ptypeLineDataHashTable;					//������
	LineDataType * ptypeLineDataUIDSameHashTable;			//�����ظ�UID�Ĺ�����
	BinType typeBinData;									//Binϵ��������
	BinType typeBinDataUIDSame;								//�����ظ�UID��Binϵ��������

	struct VersionStruct * next;
	struct VersionStruct * previous;
}VersionType;



/* �汾��ͷ */
extern VersionType ex_typeVersionHead;



int fnVersionNew(VersionType ** ppVersionType);

int fnVersionInsert(VersionType * ptypeHead, VersionType * ptypeData);

int fnVersionGetHead(VersionType ** pptypeHead, char * pszVersionName, char * pszBatchName);

int fnVersionLineToBin(VersionType * ptypeVersionHead);

int fnVersionUIDSameLineToFile(VersionType * ptypeVersionHead);

int fnVersionALLToFile(VersionType * ptypeVersionHead);


#endif
