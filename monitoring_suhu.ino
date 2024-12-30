#include "main.h"
#include "dht11.h"
#include "gps.h"
#include <stdio.h>

UART_HandleTypeDef huart1;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_USART1_UART_Init();

  DHT11_Init();
  GPS_Init(&huart1);

  char buffer[100];
  float temperature, humidity;
  GPS_Data gps_data;

  while (1)
  {
    // Baca data dari sensor DHT11
    if (DHT11_Read(&temperature, &humidity) == DHT11_OK)
    {
      sprintf(buffer, "Temperature: %.2f C, Humidity: %.2f %%\r\n", temperature, humidity);
      HAL_UART_Transmit(&huart1, (uint8_t*)buffer, strlen(buffer), HAL_MAX_DELAY);
    }
    else
    {
      HAL_UART_Transmit(&huart1, (uint8_t*)"DHT11 Error\r\n", 13, HAL_MAX_DELAY);
    }

    // Baca data dari modul GPS
    if (GPS_Read(&gps_data) == GPS_OK)
    {
      sprintf(buffer, "Latitude: %.6f, Longitude: %.6f\r\n", gps_data.latitude, gps_data.longitude);
      HAL_UART_Transmit(&huart1, (uint8_t*)buffer, strlen(buffer), HAL_MAX_DELAY);
    }
    else
    {
      HAL_UART_Transmit(&huart1, (uint8_t*)"GPS Error\r\n", 11, HAL_MAX_DELAY);
    }

    HAL_Delay(2000);
  }
}

static void MX_USART1_UART_Init(void)
{
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
}

static void MX_GPIO_Init(void)
{
  __HAL_RCC_GPIOA_CLK_ENABLE();
}

void SystemClock_Config(void)
{
  // Konfigurasi clock
}

void Error_Handler(void)
{
  while(1)
  {
  }
}