/*
 * spiffs.c
 *
 *  Created on: 2017Äê2ÔÂ11ÈÕ
 *      Author: Administrator
 */

#include "c_types.h"
#include <fcntl.h>

#include "esp_common.h"
#include "espconn.h"
#include "esp_libc.h"
#include "cJSON.h"
#include "freertos/timers.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "userspiffs.h"
#include "spiffs/spiffs.h"

//#define FS1_FLASH_SIZE      (0x20000)
////#define FS1_FLASH_SIZE      (128*1024)
//#define FS2_FLASH_SIZE      (128*1024)
//
//#define FS1_FLASH_ADDR      (0x3dc000)
//#define FS2_FLASH_ADDR      (1280*1024)
//
//#define SECTOR_SIZE         (4*1024)
//#define LOG_BLOCK           (SECTOR_SIZE)
//#define LOG_PAGE            (128)
//
//#define FD_BUF_SIZE         32*4
//#define CACHE_BUF_SIZE      (LOG_PAGE + 32)*8

#define FS1_FLASH_SIZE (0x20000)
#define FS1_FLASH_ADDR (0x3dc000)
#define SECTOR_SIZE (0x1000)
#define LOG_BLOCK SECTOR_SIZE
#define LOG_PAGE (128)
#define FD_BUF_SIZE (128)
#define CACHE_BUF_SIZE 0x500

spiffs __fs;
#define FS &__fs

void spiffs_fs1_init(void) {
	struct esp_spiffs_config config;

	config.phys_size = FS1_FLASH_SIZE;
	config.phys_addr = FS1_FLASH_ADDR;
	config.phys_erase_block = SECTOR_SIZE;
	config.log_block_size = LOG_BLOCK;
	config.log_page_size = LOG_PAGE;
	config.fd_buf_size = FD_BUF_SIZE * 2;
	config.cache_buf_size = CACHE_BUF_SIZE;

	esp_spiffs_init(&config);
}

void spiffs_fs1_write_read(void) {
	char *buf = "hello world";
	char out[20] = { 0 };

	int pfd = open("myfile", O_RDWR);
	if (read(pfd, out, 20) < 0)
		printf("read errno \n");
	close(pfd);
	printf("--> %s <--\n", out);

	if (out == NULL) {
		pfd = open("myfile", O_TRUNC | O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
		if (pfd <= 3) {
			printf("open file error \n");
		}
		int write_byte = write(pfd, buf, strlen(buf));
		if (write_byte <= 0) {
			printf("write file error \n");
		}

		close(pfd);

		open("myfile", O_RDWR);
		if (read(pfd, out, 20) < 0)
			printf("read errno \n");
		close(pfd);
		printf("--> %s <--\n", out);
	}

}

unsigned char saveSSIDAndPassword(char *ssid, char *password) {
	cJSON *wificonf;
	char *out;
	char *get;
	printf("write file start \n");
	wificonf = cJSON_CreateObject();
	cJSON_AddStringToObject(wificonf, "ssid", ssid);
	cJSON_AddStringToObject(wificonf, "password", password);
	out = cJSON_PrintUnformatted(wificonf);
	printf("wifi config: %s", out);
	int pfd = open("config", O_TRUNC | O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
	if (pfd <= 3) {
		printf("open file error \n");
	}
	int write_byte = write(pfd, out, strlen(out));
	if (write_byte <= 0) {
		printf("write file error \n");
	}
	close(pfd);
//	get = malloc(200);
//	open("config", O_RDWR);
//	if (read(pfd, out, 200) < 0)
//		printf("read errno \n");
//	close(pfd);
//	printf("--> %s <--\n", get);
//	free(get);
	free(out);

	return 0;
}

unsigned char readSSIDAndPassword(WiFi *wifi) {
	int pfd = open("config", O_RDWR);
	char *get;
	get = malloc(200);
	if (pfd == 0) {
		printf("pfd error \n");
		return 1;
	}
	if (read(pfd, get, 100) < 0) {
		printf("read errno \n");
		free(get);
		close(pfd);
		return 1;
	}
	close(pfd);
	printf("--> %s <--\n", get);
	cJSON *wificonf;
	wificonf = cJSON_Parse(get);
	if (wificonf != NULL) {
		if (cJSON_GetObjectItem(wificonf, "ssid") != NULL) {
			wifi->ssid = cJSON_GetObjectItem(wificonf, "ssid")->valuestring;

		}
		if (cJSON_GetObjectItem(wificonf, "password") != NULL) {
			wifi->password =
					cJSON_GetObjectItem(wificonf, "password")->valuestring;
		}
	}
	free(get);
	return 0;
}

unsigned char eraseSSIDAndPassword() {
	char *out;
	out = calloc(10, sizeof(char));
	int pfd = open("config", O_TRUNC | O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
	if (pfd <= 3) {
		printf("open file error \n");
		return 1;
	}
	int write_byte = write(pfd, out, 100);
	if (write_byte <= 0) {
		printf("write file error \n");
		return 1;
	}
	close(pfd);
	return 0;
}
