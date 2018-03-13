#pragma once

#ifndef _DRIVERS_FILE_H_
#define _DRIVERS_FILE_H_

#ifndef MAX_FILE_DATE
#define MAX_FILE_DATE 32
#endif

#ifndef MAX_FILE_TIME
#define MAX_FILE_TIME 32
#endif



#include "TransferFormat.h"




int fnFileConfigurationGetData(int * iData, char * pszStringBuff, char * pszCategory);

int fnFileConfigurationRead(ConfigurationType * ptypeConfiguration, FILE * pFileHand);

int fnFileConfigurationWrite(ConfigurationType * ptypeConfiguration, FILE * pFileHand);

int fnFileScan(char * pszFileName);

int fnFileOpenAndRead(char * pszFileName, char * pszFilePath);

int fnFileGetType(char * pszFileType, char * pszFileName);

#endif
