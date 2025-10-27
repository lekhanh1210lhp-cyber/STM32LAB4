#include "func.h"

//
// ĐỊNH NGHĨA CÁC HÀM
// Đã ánh xạ 5 tasks tới 5 chân PA1 - PA5
//

// Task 1 -> Điều khiển PA1 (LED_RED_Pin)
void turnREDLED()
{
	HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
}

// Task 2 -> Điều khiển PA2 (LED_YELLOW_Pin)
void turnGREENLED()
{
	HAL_GPIO_TogglePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin);
}

// Task 3 -> Điều khiển PA3 (LED_GREEN_Pin)
void turnYELLOWLED()
{
	HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
}

// Task 4 -> Điều khiển PA4 (LED_REDA4_Pin)
void turnRED()
{
	HAL_GPIO_TogglePin(LED_REDA4_GPIO_Port, LED_REDA4_Pin);
}

// Task 5 -> Điều khiển PA5 (LED_GREENA5_Pin)
void turnGreen()
{
	HAL_GPIO_TogglePin(LED_GREENA5_GPIO_Port, LED_GREENA5_Pin);
}
