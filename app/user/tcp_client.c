#include "../../app/user/tcp_client.h"

#include "../../app/include/user_config.h"
#include "c_types.h"

#include "esp_common.h"
#include "espconn.h"
#include "cJSON.h"

#define DEMO_AP_SSID      "Apple"
#define DEMO_AP_PASSWORD "ILoveApple"

#define SOFT_AP_SSID "DEMO_AP"
#define SOFT_AP_PASSWORD "12345678"

const unsigned char tcp_server_ip[4] = { 192, 168, 0, 133 };
#define TCP_SERVER_REMOTE_PORT (9999)
#define TCP_CLIENT_GREETING "Hello!This is a tcp client test\n"

#define DEV_ID "201610010000001"
#define DEV_SN "201610010000001"

//unsigned char state = 0;
cJSON *SessionId, *Sequence;
cJSON *Action;
cJSON *smpkg, *content, *action, *devId, *devContent, *devstate;
char *sessionId = "123456789", *out, *in;
int i, sequence = 1; /* declare a few. */
bool devState = true;
bool isLogin = false;

void devLogin() {
	smpkg = cJSON_CreateObject();
	cJSON_AddItemToObject(smpkg, "sequence",
			cJSON_CreateNumber((double) sequence));
	cJSON_AddItemToObject(smpkg, "content", content = cJSON_CreateObject());
	cJSON_AddStringToObject(content, "Action", "DevLogin");
	cJSON_AddStringToObject(content, "DevId", DEV_ID);
	cJSON_AddStringToObject(content, "SN", DEV_SN);
	sequence = 2;
}

void devSendState(void *arg) {
	cJSON *smpkg, *content, *devContent;
	char *out;
	printf("%d\n", 5);
	smpkg = cJSON_CreateObject();
	printf("%d\n", 6);
	cJSON_AddItemToObject(smpkg, "sequence",
			cJSON_CreateNumber((double) sequence));
	printf("%d\n", 7);
	cJSON_AddStringToObject(smpkg, "sessionId", sessionId);
	printf("%d\n", 8);
	cJSON_AddItemToObject(smpkg, "content", content = cJSON_CreateObject());
	cJSON_AddStringToObject(content, "Action", "PushToUser");
	cJSON_AddStringToObject(content, "DevId", DEV_ID);
	//设备需要发送的内容
	printf("%d\n", 9);
	cJSON_AddItemToObject(content, "Content", devContent =
			cJSON_CreateObject());
	cJSON_AddBoolToObject(devContent, "state", devState);

	out = cJSON_Print(smpkg);
	strcat(out, "&$$&");
	printf("%s\n", out);
	espconn_send(arg, out, strlen(out));

	free(out);
	sequence++;
	cJSON_Delete(smpkg);

}

/*--------------------------------------------------------------
 Tcp Client
 -------------------------------------------------------------*/

void TcpClientConnect(void*arg) {
	struct espconn* tcp_server_local = arg;
#if TCP_CLIENT_KEEP_ALIVE_ENABLE
	espconn_set_opt(tcp_server_local, BIT(3)); //enable keep alive ,this api must call in connect callback

	uint32 keep_alvie = 0;
	keep_alvie = TCP_CLIENT_KEEP_ALIVE_IDLE_S;
	espconn_set_keepalive(tcp_server_local, ESPCONN_KEEPIDLE, &keep_alvie);
	keep_alvie = TCP_CLIENT_RETRY_INTVL_S;
	espconn_set_keepalive(tcp_server_local, ESPCONN_KEEPINTVL, &keep_alvie);
	keep_alvie = keep_alvie = TCP_CLIENT_RETRY_INTVL_S;
	espconn_set_keepalive(tcp_server_local, ESPCONN_KEEPCNT, &keep_alvie);
	DBG_PRINT("keep alive enable\n");
#endif
	DBG_LINES("TCP CLIENT CONNECT");
	DBG_PRINT("tcp client ip:%d.%d.%d.%d port:%d\n",
			tcp_server_local->proto.tcp->remote_ip[0],
			tcp_server_local->proto.tcp->remote_ip[1],
			tcp_server_local->proto.tcp->remote_ip[2],
			tcp_server_local->proto.tcp->remote_ip[3],
			tcp_server_local->proto.tcp->remote_port);
	//发送登录请求
	devLogin(); //构建请求包
	out = cJSON_Print(smpkg);
	strcat(out, "&$$&");
	printf("%s\n", out);
	espconn_send(tcp_server_local, out, strlen(out));
	cJSON_Delete(smpkg);
	free(out);
}

void TcpClientDisConnect(void* arg) {
	struct espconn* tcp_server_local = arg;
	DBG_LINES("TCP CLIENT DISCONNECT");
	DBG_PRINT("tcp client ip:%d.%d.%d.%d port:%d\n",
			tcp_server_local->proto.tcp->remote_ip[0],
			tcp_server_local->proto.tcp->remote_ip[1],
			tcp_server_local->proto.tcp->remote_ip[2],
			tcp_server_local->proto.tcp->remote_ip[3],
			tcp_server_local->proto.tcp->remote_port);

	//自动重新连接
	//xTaskCreate(TcpLocalClient,"TcpLocalClient",500,NULL,6,NULL);
}
void TcpClienSendCb(void* arg) {
	struct espconn* tcp_server_local = arg;
	DBG_LINES("TCP CLIENT SendCb");
	DBG_PRINT("tcp client ip:%d.%d.%d.%d port:%d\n",
			tcp_server_local->proto.tcp->remote_ip[0],
			tcp_server_local->proto.tcp->remote_ip[1],
			tcp_server_local->proto.tcp->remote_ip[2],
			tcp_server_local->proto.tcp->remote_ip[3],
			tcp_server_local->proto.tcp->remote_port);
}

void TcpRecvCb(void *arg, char *pdata, unsigned short len) {
	struct espconn* tcp_server_local = arg;
	char *temp, *tempin, *temp1, *temp2;

	DBG_PRINT("Recv tcp client ip:%d.%d.%d.%d port:%d len:%d\n",
			tcp_server_local->proto.tcp->remote_ip[0],
			tcp_server_local->proto.tcp->remote_ip[1],
			tcp_server_local->proto.tcp->remote_ip[2],
			tcp_server_local->proto.tcp->remote_ip[3],
			tcp_server_local->proto.tcp->remote_port, len);
	//espconn_send(tcp_server_local,pdata,len);
	DBG_PRINT("msg:%s\n", pdata);
	if (strstr(pdata, "&$$&")) {
		DBG_PRINT("msg:%d\n", 0);
		smpkg = cJSON_Parse(pdata);
		if (smpkg != NULL) {
			DBG_PRINT("msg:%d\n", 1);
			Sequence = cJSON_GetObjectItem(smpkg, "sequence");
			if (Sequence->valueint == 1) {
				DBG_PRINT("msg:%d\n", 2);
				//login
				SessionId = cJSON_GetObjectItem(smpkg, "sessionId");
				if (SessionId != NULL) {
					sessionId = SessionId->valuestring;
					isLogin = true;
					DBG_LINES("Login Succed");
				} else {
					DBG_LINES("Login Failed");
				}
				devSendState(tcp_server_local);
			} else if (Sequence->valueint == 0) {
				DBG_PRINT("msg:%d\n", 3);
				content = cJSON_GetObjectItem(smpkg, "content");
				if (content != NULL) {
					printf("%d\n", 2);
					action = cJSON_GetObjectItem(content, "Action");
					if (action != NULL) {
						printf("%d\n", 3);
						printf("%s\n", action->valuestring);
						if (strcmp("ServerPushToDev", action->valuestring)
								== 0) {
							devContent = cJSON_GetObjectItem(content,
									"Content");
							if (devContent != NULL) {
								printf("%d\n", 4);
								devstate = cJSON_GetObjectItem(devContent,
										"state");
								if (devstate != NULL) {
									devState = devstate->type;
									printf("Dev state is %d\n", devState);
									if (devState) {
										gpio16_output_set(0);
									} else {
										gpio16_output_set(1);
									}

								}
							}
						}

					}
				}

				cJSON_Delete(smpkg);

				devSendState(tcp_server_local);
			} else {

			}
		}

	}
}
void TcpReconnectCb(void *arg, sint8 err) {
	struct espconn* tcp_server_local = arg;
	DBG_LINES("TCP CLIENT RECONNECT");
	DBG_PRINT("status:%d\n", err);
	DBG_PRINT("tcp client ip:%d.%d.%d.%d port:%d\n",
			tcp_server_local->proto.tcp->remote_ip[0],
			tcp_server_local->proto.tcp->remote_ip[1],
			tcp_server_local->proto.tcp->remote_ip[2],
			tcp_server_local->proto.tcp->remote_ip[3],
			tcp_server_local->proto.tcp->remote_port\
);
}

void TcpLocalClient(void* arg) {

	uint8 con_status = wifi_station_get_connect_status();
	while (con_status != STATION_GOT_IP) {
		con_status = wifi_station_get_connect_status();
		DBG_PRINT("Connect ap %s\n",
				con_status == STATION_IDLE ? "IDLE" :
				con_status == STATION_CONNECTING ? "Connecting..." :
				con_status == STATION_WRONG_PASSWORD ? "Password" :
				con_status == STATION_NO_AP_FOUND ? "ap_not_find" :
				con_status == STATION_CONNECT_FAIL ? "Connect fail" : "Get ip");
		vTaskDelay(100);
	}
	static struct espconn tcp_client;
	static esp_tcp tcp;
	tcp_client.type = ESPCONN_TCP;
	tcp_client.proto.tcp = &tcp;
	tcp.remote_port = TCP_SERVER_REMOTE_PORT;
	memcpy(tcp.remote_ip, tcp_server_ip, sizeof(tcp_server_ip));
	espconn_regist_connectcb(&tcp_client, TcpClientConnect);
	espconn_regist_recvcb(&tcp_client, TcpRecvCb);
	espconn_regist_reconcb(&tcp_client, TcpReconnectCb);
	espconn_regist_disconcb(&tcp_client, TcpClientDisConnect);
	espconn_regist_sentcb(&tcp_client, TcpClienSendCb);
	DBG_PRINT("tcp client connect server,server ip:%d.%d.%d.%d port:%d\n",
			tcp_client.proto.tcp->remote_ip[0],
			tcp_client.proto.tcp->remote_ip[1],
			tcp_client.proto.tcp->remote_ip[2],
			tcp_client.proto.tcp->remote_ip[3],
			tcp_client.proto.tcp->remote_port\
);
	espconn_connect(&tcp_client);
	vTaskDelete(NULL);
}

void StaConectApConfig(char*ssid, char*password) {
	if (wifi_get_opmode() != 0x01 && wifi_get_opmode() != 0x03) {
		ERR_PRINT("Mode not include sta\n");
		wifi_set_opmode(0x03);
		//return;
	}
	struct station_config* sta = (struct station_config*) malloc(
			sizeof(struct station_config));
	if (sta == NULL) {
		ERR_PRINT("memory not enough,heap_size=%ukByte\n",
				system_get_free_heap_size() / 1023);
		return;
	}
	bzero(sta, sizeof(struct station_config));
	wifi_station_get_config(sta);
	sprintf(sta->ssid, "%s", ssid);
	sprintf(sta->password, "%s", password);
	wifi_station_set_config(sta);
	wifi_station_connect();

	bzero(sta, sizeof(struct station_config));
	wifi_station_get_config(sta);
	DBG_LINES("STA_CONNECT_AP");
	DBG_PRINT("ssid:%s\n", sta->ssid);
	DBG_PRINT("password:%d\n", sta->password);
	free(sta);
	sta = NULL;

}

void WifiConfig(void* arg) {
	StaConectApConfig(DEMO_AP_SSID, DEMO_AP_PASSWORD);
	vTaskDelete(NULL);
}
