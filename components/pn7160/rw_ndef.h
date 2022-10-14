/*
*         Copyright (c), NXP Semiconductors Caen / France
*
*                     (C)NXP Semiconductors
*       All rights are reserved. Reproduction in whole or in part is
*      prohibited without the written consent of the copyright owner.
*  NXP reserves the right to make changes without notice at any time.
* NXP makes no warranty, expressed, implied or statutory, including but
* not limited to any implied warranty of merchantability or fitness for any
*particular purpose, or that the use will not infringe any third party patent,
* copyright or trademark. NXP must not be liable for any loss or damage
*                          arising from its use.
*/

#ifndef _RW_NDEF_H_
#define _RW_NDEF_H_

#define RW_MAX_NDEF_FILE_SIZE 500

typedef void RW_NDEF_Callback_t (unsigned char*, unsigned short);

extern unsigned char NdefBuffer[RW_MAX_NDEF_FILE_SIZE];

extern unsigned char *pRW_NdefMessage;
extern unsigned short RW_NdefMessage_size;

extern RW_NDEF_Callback_t *pRW_NDEF_PullCb;
extern RW_NDEF_Callback_t *pRW_NDEF_PushCb;

#endif
