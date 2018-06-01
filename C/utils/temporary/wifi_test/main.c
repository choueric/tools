#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include "wpa_cli.h"

static int init_apinfo(struct apinfo *pinfo, char *ssid, char *mgmt, char *passwd)
{
	memset(pinfo, 0, sizeof(struct apinfo));
	if (ssid != NULL)
		strcpy(pinfo->ssid, ssid);

	if (mgmt != NULL)
		strcpy(pinfo->mgmt, mgmt);

	if (passwd != NULL)
		strcpy(pinfo->passwd, passwd);

	return 0;
}

int main(int argc, char **argv)
{
	struct apinfo netgear, cmcc;
	enum wifi_status status;
	int n = 0;

	init_apinfo(&netgear, "MRYY", MGMT_WPA_PSK, "50215240");
	init_apinfo(&cmcc, "CMCC", MGMT_OPEN, NULL);

	// set ap info
	Wifi_Enable("0", 0);
	Wifi_set_apinfo("0", &cmcc);

	// connect ap
	Wifi_Enable("0", 1);

	// loop to get state
	n = 0;
	while (1) {
		sleep(1);
		printf("--> %d\n", n++);
		status = Wifi_get_status();
	
		if (status == WIFI_CONNECTED) {
			printf("connected\n");
			break;
		}
		printf("status: %d\n", status);
	}
	return 0;
}
