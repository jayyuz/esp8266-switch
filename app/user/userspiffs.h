/*
 * spiffs.h
 *
 *  Created on: 2017年2月11日
 *      Author: Administrator
 */

#ifndef APP_USER_USERSPIFFS_H_
#define APP_USER_USERSPIFFS_H_
#include "user_config.h"

void spiffs_fs1_init(void);
void spiffs_fs1_write_read(void);
unsigned char saveSSIDAndPassword(char *ssid, char *password);
unsigned char readSSIDAndPassword(WiFi *wifi);
unsigned char eraseSSIDAndPassword(void);

#endif /* APP_USER_USERSPIFFS_H_ */
