
/************************************************************************************************************
**************    Include Headers
************************************************************************************************************/

#include "esp.h"
#include <string.h>
#include <stdio.h>

/************************************************************************************************************
**************    Private Definitions
************************************************************************************************************/

// none

/************************************************************************************************************
**************    Private Variables
************************************************************************************************************/

const char* esp_ok = "\r\nOK\r\n";
const char* esp_error = "\r\nERROR\r\n";
const char* esp_send_ok = "\r\nSEND OK\r\n";
const char* esp_send_fail = "\r\nSEND FAIL\r\n";
const char* esp_busy = "\r\nbusy p...\r\n";

/************************************************************************************************************
**************    Private Functions
************************************************************************************************************/

/************************************************************************************************************
**************    Public Functions
************************************************************************************************************/

/**
  * @brief  Initializes the ESP handle structure.
  * @param  hEsp: Pointer to the ESP handle.
  * @param  hUart: Pointer to the UART handle.
  * @param  BufferSize: Size of the RX buffer. It needs 2X memory.
  * @param  pName: Name identifier for the ATC.
  * @param  AutoStore: Select auto store parameter mode.
  * @retval true if initialization is successful, false otherwise.
  */
bool ESP_Init(ESP_HandleTypeDef* hEsp, UART_HandleTypeDef* hUart, uint16_t BufferSize, const char* pName, bool AutoStore)
{
  bool answer = false;
  do
  {
    if (hEsp == NULL)
    {
      break;
    }
    memset(hEsp, 0, sizeof(ESP_HandleTypeDef));
    if (ATC_Init(&hEsp->hAtc, hUart, BufferSize, pName) == false)
    {
      break;
    }
    ATC_Receive(&hEsp->hAtc, NULL, 2000, 1, "ready\r\n");
    if (ATC_SendReceive(&hEsp->hAtc, "ATE0\r\n", ESP_SEND_TIME_MIN, NULL, ESP_RESP_TIME_MIN, 1, esp_ok) != 1)
    {
      break;
    }
    if (!ATC_Send(&hEsp->hAtc, "AT+SYSSTORE=%d\r\n", ESP_SEND_TIME_MIN, AutoStore))
    {
      break;
  }
    if (ATC_Receive(&hEsp->hAtc, NULL, ESP_RESP_TIME_MIN, 1, esp_ok) != 1)
    {
      break;
    }
    answer = true;

  } while (0);

  return answer;
}

/***********************************************************************************************************/

/**
  * @brief  DeInitializes the ESP handle structure.
  * @param  hAtc: Pointer to the ESP handle.
  * @retval None.
  */
void ESP_DeInit(ESP_HandleTypeDef* hEsp)
{
  do
  {
    if (hEsp == NULL)
    {
      break;
    }
    ATC_DeInit(&hEsp->hAtc);
    memset(hEsp, 0, sizeof(ESP_HandleTypeDef));

  } while (0);
}

/***********************************************************************************************************/

/**
  * @brief
  * @param  hEsp: Pointer to the ESP handle.
  * @param  .
  * @retval .
  */
void ESP_Loop(ESP_HandleTypeDef* hEsp)
{
  ATC_Loop(&hEsp->hAtc);
}

/***********************************************************************************************************/

/**
  * @brief  Callback for handling UART idle line detection.
  * @param  hEsp: Pointer to the ESP handle.
  * @param  Len: Length of received data.
  * @retval None.
  */
inline void ESP_IdleLineCallback(ESP_HandleTypeDef* hEsp, uint16_t Len)
{
  ATC_IdleLineCallback(&hEsp->hAtc, Len);
}

/***********************************************************************************************************/

/**
  * @brief
  * @param  hEsp: Pointer to the ESP handle.
  * @param  .
  * @retval .
  */
bool ESP_Check(ESP_HandleTypeDef* hEsp)
{
  bool answer = false;
  do
  {
    if (ATC_SendReceive(&hEsp->hAtc, "AT\r\n", ESP_SEND_TIME_MIN, NULL, ESP_RESP_TIME_MIN, 1, esp_ok) != 1)
    {
      break;
    }
    answer = true;

  } while (0);
  
  return answer;
}

/***********************************************************************************************************/

/**
  * @brief
  * @param  hEsp: Pointer to the ESP handle.
  * @param  .
  * @retval .
  */
bool ESP_Restart(ESP_HandleTypeDef* hEsp)
{
  bool answer = false;
  do
  {
    if (ATC_SendReceive(&hEsp->hAtc, "AT+RST\r\n", ESP_SEND_TIME_MIN, NULL, ESP_RESP_TIME_MIN, 1, esp_ok) != 1)
    {
      break;
    }
    answer = true;

  } while (0);

  return answer;
}

/***********************************************************************************************************/

/**
  * @brief
  * @param  hEsp: Pointer to the ESP handle.
  * @param  .
  * @retval .
  */
bool ESP_FactoryReset(ESP_HandleTypeDef* hEsp)
{
  bool answer = false;
  do
  {
    if (ATC_SendReceive(&hEsp->hAtc, "AT+RESTORE\r\n", ESP_SEND_TIME_MIN, NULL, ESP_RESP_TIME_MIN, 1, esp_ok) != 1)
    {
      break;
    }
    if (ATC_SendReceive(&hEsp->hAtc, "AT+RST\r\n", ESP_SEND_TIME_MIN, NULL, ESP_RESP_TIME_MIN, 1, esp_ok) != 1)
    {
      break;
    }
    ATC_Delay(200);
    answer = true;

  } while (0);

  return answer;
}

/***********************************************************************************************************/

/**
  * @brief
  * @param  hEsp: Pointer to the ESP handle.
  * @param  .
  * @retval .
  */
bool ESP_GetVer(ESP_HandleTypeDef* hEsp, char* pStr)
{
  bool answer = false;
  do
  {
    char* str;
    if (ATC_SendReceive(&hEsp->hAtc, "AT+GMR\r\n", ESP_SEND_TIME_MIN, &str, ESP_RESP_TIME_MIN, 1, "AT version:") != 1)
    {
      break;
    }
    if (pStr != NULL)
    {
      sscanf(pStr, "AT version:%11[^\r\n(]", str);
      strcpy(pStr, str);
    }
    ATC_Delay(10);
    answer = true;

  } while (0);

  return answer;
}

/***********************************************************************************************************/

/**
  * @brief
  * @param  hEsp: Pointer to the ESP handle.
  * @param  .
  * @retval .
  */
bool ESP_EnterDeepSleep(ESP_HandleTypeDef* hEsp, uint32_t DurationMs)
{
  bool answer = false;
  do
  {
    if (!ATC_Send(&hEsp->hAtc, "AT+GSLP=%d\r\n", ESP_SEND_TIME_MIN, DurationMs))
  {
    break;
  }
  if (ATC_Receive(&hEsp->hAtc, NULL, ESP_RESP_TIME_MIN, 1, esp_ok) != 1)
  {
    break;
  }
  answer = true;

  } while (0);

  return answer;
}

/***********************************************************************************************************/

/**
  * @brief
  * @param  hEsp: Pointer to the ESP handle.
  * @param  .
  * @retval .
  */
bool ESP_SetSleepMode(ESP_HandleTypeDef* hEsp, ESP_SleepModeTypeDef eSleepMode)
{
  bool answer = false;
  do
  {
    if (!ATC_Send(&hEsp->hAtc, "AT+SLEEP=%d\r\n", ESP_SEND_TIME_MIN, eSleepMode))
    {
      break;
    }
    if (ATC_Receive(&hEsp->hAtc, NULL, ESP_RESP_TIME_MIN, 1, esp_ok) != 1)
    {
      break;
    }
    answer = true;

  } while (0);

  return answer;
}

/***********************************************************************************************************/

/**
  * @brief
  * @param  hEsp: Pointer to the ESP handle.
  * @param  .
  * @retval .
  */
bool ESP_GetSleepMode(ESP_HandleTypeDef* hEsp, ESP_SleepModeTypeDef* peSleepMode)
{
  bool answer = false;
  char* str;
  uint32_t val1;
  do
  {
    if (peSleepMode == NULL)
    {
      break;
  }
  if (ATC_SendReceive(&hEsp->hAtc, "AT+SLEEP?\r\n", ESP_SEND_TIME_MIN, &str, ESP_RESP_TIME_MIN, 1, "+SLEEP:") != 1)
  {
    break;
  }

  if (sscanf(str, "\r\n+SLEEP:%ld", &val1) != 1)
  {
    break;
  }
  if (peSleepMode != NULL)
  {
    *peSleepMode = val1;
  }
  answer = true;

} while (0);

return answer;
}

/***********************************************************************************************************/

/**
  * @brief
  * @param  hEsp: Pointer to the ESP handle.
  * @param  .
  * @retval .
  */
bool ESP_GetFreeHeap(ESP_HandleTypeDef* hEsp, uint32_t* pHeap, uint32_t* pMinHeap)
{
  bool answer = false;
  uint32_t val1, val2;
  char* str;
  do
  {
    if (ATC_SendReceive(&hEsp->hAtc, "AT+SYSRAM?\r\n", ESP_SEND_TIME_MIN, &str, ESP_RESP_TIME_MIN, 1, "+SYSRAM:") != 1)
    {
      break;
    }
    if (sscanf(str, "+SYSRAM:%ld,%ld\r\n", &val1, &val2) != 2)
    {
      break;
    }
    if (pHeap != NULL)
    {
      *pHeap = val1;
    }
    if (pMinHeap != NULL)
    {
      *pMinHeap = val2;
    }
    answer = true;

  } while (0);

  return answer;
}

/***********************************************************************************************************/

/**
  * @brief
  * @param  hEsp: Pointer to the ESP handle.
  * @param  .
  * @retval .
  */
bool ESP_WifiInit(ESP_HandleTypeDef *hEsp)
{
  bool answer = false;
  do
  {
    if (ATC_SendReceive(&hEsp->hAtc, "AT+CWINIT=1\r\n", ESP_SEND_TIME_MIN, NULL, ESP_RESP_TIME_MIN, 1, esp_ok) != 1)
    {
      break;
    }
    answer = true;

  } while (0);

  return answer;
}

/***********************************************************************************************************/

/**
  * @brief
  * @param  hEsp: Pointer to the ESP handle.
  * @param  .
  * @retval .
  */
bool ESP_WifiDeInit(ESP_HandleTypeDef *hEsp)
{
  bool answer = false;
  do
  {
    if (ATC_SendReceive(&hEsp->hAtc, "AT+CWINIT=0\r\n", ESP_SEND_TIME_MIN, NULL, ESP_RESP_TIME_MIN, 1, esp_ok) != 1)
    {
      break;
    }
    answer = true;

  } while (0);

  return answer;
}

/***********************************************************************************************************/

/**
  * @brief
  * @param  hEsp: Pointer to the ESP handle.
  * @param  .
  * @retval .
  */
bool ESP_WifiMode(ESP_HandleTypeDef* hEsp, ESP_WifiModeTypeDef WifiMode, bool StationAutoConnect)
{
  bool answer = false;
  do
  {
    if (!ATC_Send(&hEsp->hAtc, "AT+CWMODE=%d,%d\r\n", ESP_SEND_TIME_MIN, WifiMode, StationAutoConnect))
    {
      break;
    }
    if (ATC_Receive(&hEsp->hAtc, NULL, ESP_RESP_TIME_MIN, 1, esp_ok) != 1)
    {
      break;
    }
    answer = true;

  } while (0);

  return answer;
}

/***********************************************************************************************************/

/**
  * @brief
  * @param  hEsp: Pointer to the ESP handle.
  * @param  .
  * @retval .
  */
bool ESP_WifiStationConnect(ESP_HandleTypeDef* hEsp, const char* pSSID,
                         const char* pPass, const char *pMac,
                         uint16_t Timeout, ESP_WifiErrTypeDef *peError)
{
  bool answer = false;
  char str_at[256];
  char str_timeout[6] = {0};
  memset(str_at, 0, sizeof(str_at));
  do
  {
    sprintf(str_at, "AT+CWJAP=\"%s\",\"%s\"", pSSID, pPass);
    if (pMac != NULL)
    {
      strcat(str_at, ",\"");
      strcat(str_at, pMac);
      strcat(str_at, "\",");
    }
    else
    {
      strcat(str_at, ",,");
    }
    strcat(str_at, ",,,,");
    sprintf(str_timeout, "%d", Timeout);
    strcat(str_at, str_timeout);
    strcat(str_at, "\r\n");
    int ret = ATC_SendReceive(&hEsp->hAtc, str_at, ESP_SEND_TIME_MIN, NULL, (Timeout * 1000) + 1000, 2, esp_ok, "+CWJAP:1", "+CWJAP:2", "+CWJAP:3", "+CWJAP:4");
    if (ret == 1)
    {
      if (peError != NULL)
      {
        *peError = ESP_WIFIERR_NONE;
      }
      answer = true;
    }
    else
    {
      if (peError != NULL)
      {
        *peError = ret;
      }
    }

  } while (0);

  return answer;
}

/***********************************************************************************************************/

/**
  * @brief
  * @param  hEsp: Pointer to the ESP handle.
  * @param  .
  * @retval .
  */
bool ESP_WifiStationDisconnect(ESP_HandleTypeDef* hEsp)
{
  bool answer = false;
  do
  {
    if (ATC_SendReceive(&hEsp->hAtc, "AT+CWQAP\r\n", ESP_SEND_TIME_MIN, NULL, ESP_RESP_TIME_MIN, 1, esp_ok) != 1)
    {
      break;
    }
    answer = true;

  } while (0);

  return answer;
}

/***********************************************************************************************************/

/**
  * @brief
  * @param  hEsp: Pointer to the ESP handle.
  * @param  .
  * @retval .
  */
bool ESP_WifiSoftApStart(ESP_HandleTypeDef* hEsp,
                         const char* pSsid, const char* pPass,
                         uint8_t channel, ESP_WifiEncTypeDef Enc,
                         int8_t MaxConnection, bool Hide)
{
  bool answer = false;
  do
  {
    if (!ATC_Send(&hEsp->hAtc, "AT+CWSA=\"%s\",\"%s\",%d,%d,%d,%d\r\n", ESP_SEND_TIME_MIN, pSsid, pPass, channel, Enc, MaxConnection, Hide))
    {
      break;
    }
    if (ATC_Receive(&hEsp->hAtc, NULL, ESP_RESP_TIME_MIN, 1, esp_ok) != 1)
    {
      break;
    }
    answer = true;

  } while (0);

  return answer;
}

/***********************************************************************************************************/

/**
  * @brief
  * @param  hEsp: Pointer to the ESP handle.
  * @param  .
  * @retval .
  */
bool ESP_WifiSoftApDisconnectAll(ESP_HandleTypeDef* hEsp)
{
  bool answer = false;
  do
  {
    if (ATC_SendReceive(&hEsp->hAtc, "AT+CWQIF\r\n", ESP_SEND_TIME_MIN, NULL, ESP_RESP_TIME_MIN, 1, esp_ok) != 1)
    {
      break;
    }
    answer = true;

  } while (0);

  return answer;
}

/***********************************************************************************************************/

/**
  * @brief
  * @param  hEsp: Pointer to the ESP handle.
  * @param  .
  * @retval .
  */
bool ESP_WifiSoftApDisconnectMac(ESP_HandleTypeDef* hEsp, const char* pMac)
{
  bool answer = false;
  do
  {
    if (!ATC_Send(&hEsp->hAtc, "AT+CWQIF=\"%s\"\r\n", ESP_SEND_TIME_MIN, pMac))
    {
      break;
    }
    if (ATC_Receive(&hEsp->hAtc, NULL, ESP_RESP_TIME_MIN, 1, esp_ok) != 1)
    {
      break;
    }
    answer = true;

  } while (0);

  return answer;
}

