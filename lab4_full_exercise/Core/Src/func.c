#include "func.h"

// Dùng các tên Pin và Port đúng từ main.h
void turnREDLED()
{
	HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
}

void turnYELLOWLED()
{
	HAL_GPIO_TogglePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin);
}

void turnGREENLED()
{
	HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
}

void turnRED()
{
    // Sửa thành GPIOB (hoặc dùng macro Port)
	HAL_GPIO_TogglePin(red2_GPIO_Port, red2_Pin);
}

void turnGreen()
{
    // Sửa thành GPIOB (hoặc dùng macro Port)
	HAL_GPIO_TogglePin(green2_GPIO_Port, green2_Pin);
}
