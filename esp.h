#ifndef _ESP_H_
#define _ESP_H_


/***********************************************************************************************************

  Author:     Nima Askari
  Github:     https://www.github.com/NimaLTD
  LinkedIn:   https://www.linkedin.com/in/nimaltd
  Youtube:    https://www.youtube.com/@nimaltd
  Instagram:  https://instagram.com/github.NimaLTD

  Version:    2.0.0

  History:

              2.0.0
              - Rewrite again
              - Support STM32CubeMx Packet installer

***********************************************************************************************************/

#ifdef __cplusplus
extern "C"
{
#endif

/************************************************************************************************************
**************    Include Headers
************************************************************************************************************/

#include "atc.h"

/************************************************************************************************************
**************    Public Definitions
************************************************************************************************************/

#define ESP_SEND_TIME_MIN    100
#define ESP_RESP_TIME_MIN    100

/************************************************************************************************************
**************    Public struct/enum
************************************************************************************************************/

typedef enum
{
  ESP_SLEEPMODE_DISABLE         = 0,
  ESP_SLEEPMODE_MODEM,
  ESP_SLEEPMODE_LIGTH,
  ESP_SLEEPMODE_LISTEN

} ESP_SleepModeTypeDef;

typedef enum
{
  ESP_WIFIMODE_DISABLE          = 0,
  ESP_WIFIMODE_STATION,
  ESP_WIFIMODE_SOFTAP,
  ESP_WIFIMODE_SOFTAP_STATION

} ESP_WifiModeTypeDef;

typedef enum
{
  ESP_WIFIERR_NONE              = 0,
  ESP_WIFIERR_TIMEOUT           = 1,
  ESP_WIFIERR_PASS              = 2,
  ESP_WIFIERR_NOT_FOUND         = 3,
  ESP_WIFIERR_FAILED            = 4

} ESP_WifiErrTypeDef;

typedef struct
{
  ATC_HandleTypeDef             hAtc;

} ESP_HandleTypeDef;

/************************************************************************************************************
**************    Public Functions
************************************************************************************************************/

bool   ESP_Init(ESP_HandleTypeDef* hEsp, UART_HandleTypeDef* hUart, uint16_t BufferSize, const char* pName, bool AutoStore);
void   ESP_DeInit(ESP_HandleTypeDef* hEsp);
void   ESP_Loop(ESP_HandleTypeDef* hEsp);

void   ESP_IdleLineCallback(ESP_HandleTypeDef* hEsp, uint16_t Len);

bool    ESP_Check(ESP_HandleTypeDef* hEsp);
bool    ESP_Restart(ESP_HandleTypeDef* hEsp);
bool    ESP_FactoryReset(ESP_HandleTypeDef* hEsp);
bool    ESP_GetVer(ESP_HandleTypeDef* hEsp, char* pStr);
bool    ESP_EnterDeepSleep(ESP_HandleTypeDef* hEsp, uint32_t DurationMs);
bool    ESP_SetSleepMode(ESP_HandleTypeDef* hEsp, ESP_SleepModeTypeDef eSleepMode);
bool    ESP_GetSleepMode(ESP_HandleTypeDef* hEsp, ESP_SleepModeTypeDef* peSleepMode);
bool    ESP_GetFreeHeap(ESP_HandleTypeDef* hEsp, uint32_t* pHeap, uint32_t* pMinHeap);

bool    ESP_WifiInit(ESP_HandleTypeDef* hEsp);
bool    ESP_WifiDeInit(ESP_HandleTypeDef* hEsp);
bool    ESP_WifiMode(ESP_HandleTypeDef* hEsp, ESP_WifiModeTypeDef WifiMode, bool StationAutoConnect);
bool    ESP_WifiConnectTo(ESP_HandleTypeDef* hEsp, const char* pSSID, const char* pPass, const char* pMac, uint16_t Timeout, ESP_WifiErrTypeDef *peError);


#ifdef __cplusplus
}
#endif
#endif
