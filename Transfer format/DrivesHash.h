#pragma once

#ifndef _DRIVERS_HASH_H_
#define _DRIVERS_HASH_H_



#include "DrivesFile.h"
#include "DrivesLine.h"
#include "DrivesVersion.h"


#define HASH_TABLE_LINE_LENGTH 1597


enum HashPolynomialEnum {
	HASH_SDBM = 0,
	HASH_RS,
	HASH_JS,
	HASH_PJW,
	HASH_ELF,
	HASH_BKDR,
	HASH_DJB,
	HASH_AP,
};



/* ¹þÏ£±í±íÍ· */
extern LineDataType * ex_typeLineDataHashTable;



unsigned int fnHashGet32(char * pszBuff, HashPolynomialEnum iHashCategory);

int fnHashLineInsert(VersionType * ptypeVersionHand, LineDataType * ptypeLineDataNew, int iHashTableLength);

int fnHashLineDataTableShow(LineDataType * ptypeLineDataHashTable, int iHashTableLength);

int fnHashLineDataTableInit(LineDataType ** pptypeLineDataHashTable, int iHashTableLength);


#endif
