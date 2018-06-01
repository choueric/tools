#ifndef WPA_CLI_H
#define WPA_CLI_H

#define MGMT_OPEN "open"
#define MGMT_WPA_PSK "wpa-psk"
#define MGMT_WEP "wep"
#define MGMT_WPA_EAP "wpa-eap"

struct apinfo {
	char ssid[20];
	char mgmt[20];
	char passwd[20];
	void *data;
};

enum wifi_status{
	WIFI_CONNECTED=0,
	WIFI_GROUP_HANDSHAKE,
	WIFI_4WAY_HANDSHAKE,
	WIFI_ASSOCIATED,
	WIFI_ASSOCIATING,
	WIFI_SCANNING,
	WIFI_INACTIVE,
	WIFI_DISCONNECTED,
	WIFI_UNKNOWN_STATUS
};

enum wifi_error{
	WIFI_NONE_ERROR=0,
	WIFI_KEY_ERROR,
	WIFI_IFACE_ERROR,
	WIFI_NO_AP_ERROR,
	WIFI_UNKNOWN_ERROR
};

int Wifi_set_apinfo(char *id, struct apinfo *pinfo);
//int Wifi_enable_network(int enable);
enum wifi_status Wifi_get_status();

int Wifi_GetLastError();
int Wifi_SetScanSsid(char *id, int scan_ssid);
void Wifi_Authenticate(const char* ssid, const char* key);
int Wifi_Enable(char *id, int enable);
int Wifi_SwitchAntenna(int id);
int Wifi_CurtAntennaId();
const char *Wifi_GetCurBssid();
const char *Wifi_GetCurKeyMgmt();
int Wifi_GetCurRate();
int Wifi_GetCurFrequency();
int Wifi_GetCurChannel();
char Wifi_GetMode();
int Wifi_GetCurSignalLevel();
int Wifi_GetAPSignal(const char *ssid);
int Wifi_Init();
int Wifi_Detect();
const char *Wifi_GetVersion();




#endif
