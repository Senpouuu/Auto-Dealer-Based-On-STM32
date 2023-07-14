#ifndef __UPLOAD_H
#define __UPLOAD_H
#include "stm32f10x.h"                  // Device header

void TIM3IT_Init(void);
void DataUpload(void);
extern u8 upload;

#endif
