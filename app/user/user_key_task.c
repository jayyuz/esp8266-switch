/*
 * user_key_task.c
 *
 *  Created on: 2017Äê5ÔÂ7ÈÕ
 *      Author: Administrator
 */

#include "esp_common.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"
#include "espressif/espconn.h"
#include "espressif/airkiss.h"

#include "gpio.h"
#include "userspiffs.h"

#define KEY_FUNCTION_RESET_PRESS_TIME 11000000

void resetSystemAndRestart();

void keyTask(void *pvParameters) {
	uint32 lastPressTime = 0;
	uint8 state = 0, key_state = 0;
	printf("the key task running\n");

	GPIO_ConfigTypeDef pGPIOConfig;
	pGPIOConfig.GPIO_Mode = GPIO_Mode_Input;
	pGPIOConfig.GPIO_Pin = 0x0001;
	pGPIOConfig.GPIO_Pullup = GPIO_PullUp_DIS;
	gpio_config(&pGPIOConfig);

	for (;;) {
		key_state = gpio_input_get() & 0x00000001;
		if (key_state == 1) {
			if (state) {
				printf("the key pressed,last time is %d\n",
						system_get_time() - lastPressTime);
				if (system_get_time() - lastPressTime > KEY_FUNCTION_RESET_PRESS_TIME) {
					resetSystemAndRestart();
				}
				lastPressTime = system_get_time();
				state = 0;
			}
		} else {
			if (state == 0) {
				lastPressTime = system_get_time();
				state = 1;
			}
		}

		vTaskDelay(15 / portTICK_RATE_MS);
	}
}

void resetSystemAndRestart() {
	eraseSSIDAndPassword();
	gpio16_output_set(1);
	vTaskDelay(1000 / portTICK_RATE_MS);
	gpio16_output_set(0);
	vTaskDelay(1000 / portTICK_RATE_MS);
	gpio16_output_set(1);
	vTaskDelay(1000 / portTICK_RATE_MS);
	gpio16_output_set(0);
	vTaskDelay(1000 / portTICK_RATE_MS);
	gpio16_output_set(1);
	vTaskDelay(1000 / portTICK_RATE_MS);
	gpio16_output_set(0);
	vTaskDelay(1000 / portTICK_RATE_MS);
	system_restart();
}
