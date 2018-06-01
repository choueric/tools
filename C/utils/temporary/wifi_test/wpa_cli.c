#include "wpa_cli.h"
#include "wpa_ctrl.h"
#include "comm.h"

#define CMD_RESULTS_LEN 2048

static struct wpa_ctrl *ctrl_conn;
static int wpa_cli_attached = 0;
static const char *ctrl_iface_dir = "/var/run/wpa_supplicant";
static char *ctrl_ifname = NULL;
static char cmd_results[CMD_RESULTS_LEN]={0};

/******************* common communication interface *******************/
static struct wpa_ctrl * wpa_cli_open_connection(const char *ifname)
{
	char *cfile;
	int flen;

	if (ifname == NULL)
		return NULL;

	flen = strlen(ctrl_iface_dir) + strlen(ifname) + 2;
	cfile = (char*)malloc(flen);
	if (cfile == NULL)
		return NULL;
	snprintf(cfile, flen, "%s/%s", ctrl_iface_dir, ifname);

	ctrl_conn = wpa_ctrl_open(cfile);
	free(cfile);
	return ctrl_conn;
}

static void wpa_cli_close_connection(void)
{
	if (ctrl_conn == NULL)
		return;

	if (wpa_cli_attached) {
		wpa_ctrl_detach(ctrl_conn);
		wpa_cli_attached = 0;
	}
	wpa_ctrl_close(ctrl_conn);
	ctrl_conn = NULL;
}

static void wpa_cli_msg_cb(char *msg, size_t len)
{
}

static int _wpa_ctrl_command(struct wpa_ctrl *ctrl, const char *cmd)
{

	size_t len;
	int ret;

	if (ctrl_conn == NULL) {
		printf("Not connected to wpa_supplicant - command dropped.\n");
		return -1;
	}
	len = sizeof(cmd_results) - 1;
	ret = wpa_ctrl_request(ctrl, cmd, strlen(cmd), cmd_results, &len,
			       wpa_cli_msg_cb);
	if (ret == -2) {
		printf("'%s' command timed out.\n", cmd);
		return -2;
	} else if (ret < 0) {
		printf("'%s' command failed.\n", cmd);
		return -1;
	}

	return 0;
}

static int wpa_ctrl_command(struct wpa_ctrl *ctrl, const char *cmd)
{
	return _wpa_ctrl_command(ctrl, cmd);
}

/************************** cli various cmd interface ******************/

static int wpa_cli_cmd_status(struct wpa_ctrl *ctrl, int argc, const char *argv[])
{
	return wpa_ctrl_command(ctrl, "STATUS");
}

static int wpa_cli_cmd_ping(struct wpa_ctrl *ctrl, int argc, const char *argv[])
{
	return wpa_ctrl_command(ctrl, "PING");
}

static int wpa_cli_cmd_mib(struct wpa_ctrl *ctrl, int argc, const char *argv[])
{
	return wpa_ctrl_command(ctrl, "MIB");
}

static int wpa_cli_cmd_pmksa(struct wpa_ctrl *ctrl, int argc, const char *argv[])
{
	return wpa_ctrl_command(ctrl, "PMKSA");
}

static int wpa_cli_cmd_set(struct wpa_ctrl *ctrl, int argc, const char *argv[])
{
	char cmd[256];
	int res;

	if (argc == 0) {
		return 0;
	}

	if (argc != 2) {
		printf("Invalid SET command: needs two arguments (variable "
		       "name and value)\n");
		return -1;
	}

	res = snprintf(cmd, sizeof(cmd), "SET %s %s", argv[0], argv[1]);
	if (res < 0 || (size_t) res >= sizeof(cmd) - 1) {
		printf("Too long SET command.\n");
		return -1;
	}
	return wpa_ctrl_command(ctrl, cmd);
}

static int wpa_cli_cmd_logoff(struct wpa_ctrl *ctrl, int argc, const char *argv[])
{
	return wpa_ctrl_command(ctrl, "LOGOFF");
}

static int wpa_cli_cmd_logon(struct wpa_ctrl *ctrl, int argc, const char *argv[])
{
	return wpa_ctrl_command(ctrl, "LOGON");
}

static int wpa_cli_cmd_reassociate(struct wpa_ctrl *ctrl, int argc,
		const char *argv[])
{
	return wpa_ctrl_command(ctrl, "REASSOCIATE");
}

static int wpa_cli_cmd_preauthenticate(struct wpa_ctrl *ctrl, int argc,
		const char *argv[])
{
	char cmd[256];
	int res;

	if (argc != 1) {
		printf("Invalid PREAUTH command: needs one argument "
		       "(BSSID)\n");
		return -1;
	}

	res = snprintf(cmd, sizeof(cmd), "PREAUTH %s", argv[0]);
	if (res < 0 || (size_t) res >= sizeof(cmd) - 1) {
		printf("Too long PREAUTH command.\n");
		return -1;
	}
	return wpa_ctrl_command(ctrl, cmd);
}

static int wpa_cli_cmd_ap_scan(struct wpa_ctrl *ctrl, int argc, const char *argv[])
{
	char cmd[256];
	int res;

	if (argc != 1) {
		printf("Invalid AP_SCAN command: needs one argument (ap_scan "
		       "value)\n");
		return -1;
	}
	res = snprintf(cmd, sizeof(cmd), "AP_SCAN %s", argv[0]);
	if (res < 0 || (size_t) res >= sizeof(cmd) - 1) {
		printf("Too long AP_SCAN command.\n");
		return -1;
	}
	return wpa_ctrl_command(ctrl, cmd);
}

static int wpa_cli_cmd_stkstart(struct wpa_ctrl *ctrl, int argc,
				const char *argv[])
{
	char cmd[256];
	int res;

	if (argc != 1) {
		printf("Invalid STKSTART command: needs one argument "
		       "(Peer STA MAC address)\n");
		return -1;
	}

	res = snprintf(cmd, sizeof(cmd), "STKSTART %s", argv[0]);
	if (res < 0 || (size_t) res >= sizeof(cmd) - 1) {
		printf("Too long STKSTART command.\n");
		return -1;
	}
	return wpa_ctrl_command(ctrl, cmd);
}

static int wpa_cli_cmd_level(struct wpa_ctrl *ctrl, int argc, const char *argv[])
{
	char cmd[256];
	int res;

	if (argc != 1) {
		printf("Invalid LEVEL command: needs one argument (debug "
		       "level)\n");
		return -1;
	}
	res = snprintf(cmd, sizeof(cmd), "LEVEL %s", argv[0]);
	if (res < 0 || (size_t) res >= sizeof(cmd) - 1) {
		printf("Too long LEVEL command.\n");
		return -1;
	}
	return wpa_ctrl_command(ctrl, cmd);
}

static int wpa_cli_cmd_identity(struct wpa_ctrl *ctrl, int argc, const char *argv[])
{
	char cmd[256], *pos, *end;
	int i, ret;

	if (argc < 2) {
		printf("Invalid IDENTITY command: needs two arguments "
		       "(network id and identity)\n");
		return -1;
	}

	end = cmd + sizeof(cmd);
	pos = cmd;
	ret = snprintf(pos, end - pos, WPA_CTRL_RSP "IDENTITY-%s:%s",
			  argv[0], argv[1]);
	if (ret < 0 || ret >= end - pos) {
		printf("Too long IDENTITY command.\n");
		return -1;
	}
	pos += ret;
	for (i = 2; i < argc; i++) {
		ret = snprintf(pos, end - pos, " %s", argv[i]);
		if (ret < 0 || ret >= end - pos) {
			printf("Too long IDENTITY command.\n");
			return -1;
		}
		pos += ret;
	}

	return wpa_ctrl_command(ctrl, cmd);
}

static int wpa_cli_cmd_password(struct wpa_ctrl *ctrl, int argc, const char *argv[])
{
	char cmd[256], *pos, *end;
	int i, ret;

	if (argc < 2) {
		printf("Invalid PASSWORD command: needs two arguments "
		       "(network id and password)\n");
		return -1;
	}

	end = cmd + sizeof(cmd);
	pos = cmd;
	ret = snprintf(pos, end - pos, WPA_CTRL_RSP "PASSWORD-%s:%s",
			  argv[0], argv[1]);
	if (ret < 0 || ret >= end - pos) {
		printf("Too long PASSWORD command.\n");
		return -1;
	}
	pos += ret;
	for (i = 2; i < argc; i++) {
		ret = snprintf(pos, end - pos, " %s", argv[i]);
		if (ret < 0 || ret >= end - pos) {
			printf("Too long PASSWORD command.\n");
			return -1;
		}
		pos += ret;
	}

	return wpa_ctrl_command(ctrl, cmd);
}

static int wpa_cli_cmd_new_password(struct wpa_ctrl *ctrl, int argc,
		const char *argv[])
{
	char cmd[256], *pos, *end;
	int i, ret;

	if (argc < 2) {
		printf("Invalid NEW_PASSWORD command: needs two arguments "
		       "(network id and password)\n");
		return -1;
	}

	end = cmd + sizeof(cmd);
	pos = cmd;
	ret = snprintf(pos, end - pos, WPA_CTRL_RSP "NEW_PASSWORD-%s:%s",
			  argv[0], argv[1]);
	if (ret < 0 || ret >= end - pos) {
		printf("Too long NEW_PASSWORD command.\n");
		return -1;
	}
	pos += ret;
	for (i = 2; i < argc; i++) {
		ret = snprintf(pos, end - pos, " %s", argv[i]);
		if (ret < 0 || ret >= end - pos) {
			printf("Too long NEW_PASSWORD command.\n");
			return -1;
		}
		pos += ret;
	}

	return wpa_ctrl_command(ctrl, cmd);
}

static int wpa_cli_cmd_pin(struct wpa_ctrl *ctrl, int argc, const char *argv[])
{
	char cmd[256], *pos, *end;
	int i, ret;

	if (argc < 2) {
		printf("Invalid PIN command: needs two arguments "
		       "(network id and pin)\n");
		return -1;
	}

	end = cmd + sizeof(cmd);
	pos = cmd;
	ret = snprintf(pos, end - pos, WPA_CTRL_RSP "PIN-%s:%s",
			  argv[0], argv[1]);
	if (ret < 0 || ret >= end - pos) {
		printf("Too long PIN command.\n");
		return -1;
	}
	pos += ret;
	for (i = 2; i < argc; i++) {
		ret = snprintf(pos, end - pos, " %s", argv[i]);
		if (ret < 0 || ret >= end - pos) {
			printf("Too long PIN command.\n");
			return -1;
		}
		pos += ret;
	}
	return wpa_ctrl_command(ctrl, cmd);
}

static int wpa_cli_cmd_otp(struct wpa_ctrl *ctrl, int argc, const char *argv[])
{
	char cmd[256], *pos, *end;
	int i, ret;

	if (argc < 2) {
		printf("Invalid OTP command: needs two arguments (network "
		       "id and password)\n");
		return -1;
	}

	end = cmd + sizeof(cmd);
	pos = cmd;
	ret = snprintf(pos, end - pos, WPA_CTRL_RSP "OTP-%s:%s",
			  argv[0], argv[1]);
	if (ret < 0 || ret >= end - pos) {
		printf("Too long OTP command.\n");
		return -1;
	}
	pos += ret;
	for (i = 2; i < argc; i++) {
		ret = snprintf(pos, end - pos, " %s", argv[i]);
		if (ret < 0 || ret >= end - pos) {
			printf("Too long OTP command.\n");
			return -1;
		}
		pos += ret;
	}

	return wpa_ctrl_command(ctrl, cmd);
}

static int wpa_cli_cmd_passphrase(struct wpa_ctrl *ctrl, int argc,
		const char *argv[])
{
	char cmd[256], *pos, *end;
	int i, ret;

	if (argc < 2) {
		printf("Invalid PASSPHRASE command: needs two arguments "
		       "(network id and passphrase)\n");
		return -1;
	}

	end = cmd + sizeof(cmd);
	pos = cmd;
	ret = snprintf(pos, end - pos, WPA_CTRL_RSP "PASSPHRASE-%s:%s",
			  argv[0], argv[1]);
	if (ret < 0 || ret >= end - pos) {
		printf("Too long PASSPHRASE command.\n");
		return -1;
	}
	pos += ret;
	for (i = 2; i < argc; i++) {
		ret = snprintf(pos, end - pos, " %s", argv[i]);
		if (ret < 0 || ret >= end - pos) {
			printf("Too long PASSPHRASE command.\n");
			return -1;
		}
		pos += ret;
	}

	return wpa_ctrl_command(ctrl, cmd);
}

static int wpa_cli_cmd_bssid(struct wpa_ctrl *ctrl, int argc, const char *argv[])
{
	char cmd[256], *pos, *end;
	int i, ret;

	if (argc < 2) {
		printf("Invalid BSSID command: needs two arguments (network "
		       "id and BSSID)\n");
		return -1;
	}

	end = cmd + sizeof(cmd);
	pos = cmd;
	ret = snprintf(pos, end - pos, "BSSID");
	if (ret < 0 || ret >= end - pos) {
		printf("Too long BSSID command.\n");
		return -1;
	}
	pos += ret;
	for (i = 0; i < argc; i++) {
		ret = snprintf(pos, end - pos, " %s", argv[i]);
		if (ret < 0 || ret >= end - pos) {
			printf("Too long BSSID command.\n");
			return -1;
		}
		pos += ret;
	}

	return wpa_ctrl_command(ctrl, cmd);
}

static int wpa_cli_cmd_list_networks(struct wpa_ctrl *ctrl, int argc,
		const char *argv[])
{
	return wpa_ctrl_command(ctrl, "LIST_NETWORKS");
}

static int wpa_cli_cmd_select_network(struct wpa_ctrl *ctrl, int argc,
		const char *argv[])
{
	char cmd[32];

	if (argc < 1) {
		printf("Invalid SELECT_NETWORK command: needs one argument "
		       "(network id)\n");
		return -1;
	}

	snprintf(cmd, sizeof(cmd), "SELECT_NETWORK %s", argv[0]);
	cmd[sizeof(cmd) - 1] = '\0';

	return wpa_ctrl_command(ctrl, cmd);
}


static int wpa_cli_cmd_enable_network(struct wpa_ctrl *ctrl, int argc,
		const char *argv[])
{
	char cmd[32];

	if (argc < 1) {
		printf("Invalid ENABLE_NETWORK command: needs one argument "
		       "(network id)\n");
		return -1;
	}

	snprintf(cmd, sizeof(cmd), "ENABLE_NETWORK %s", argv[0]);
	cmd[sizeof(cmd) - 1] = '\0';

	return wpa_ctrl_command(ctrl, cmd);
}


static int wpa_cli_cmd_disable_network(struct wpa_ctrl *ctrl, int argc,
		const char *argv[])
{
	char cmd[32];

	if (argc < 1) {
		printf("Invalid DISABLE_NETWORK command: needs one argument "
		       "(network id)\n");
		return -1;
	}

	snprintf(cmd, sizeof(cmd), "DISABLE_NETWORK %s", argv[0]);
	cmd[sizeof(cmd) - 1] = '\0';

	return wpa_ctrl_command(ctrl, cmd);
}

static int wpa_cli_cmd_add_network(struct wpa_ctrl *ctrl, int argc,
		const char *argv[])
{
	return wpa_ctrl_command(ctrl, "ADD_NETWORK");
}

static int wpa_cli_cmd_remove_network(struct wpa_ctrl *ctrl, int argc,
		const char *argv[])
{
	char cmd[32];

	if (argc < 1) {
		printf("Invalid REMOVE_NETWORK command: needs one argument "
		       "(network id)\n");
		return -1;
	}

	snprintf(cmd, sizeof(cmd), "REMOVE_NETWORK %s", argv[0]);
	cmd[sizeof(cmd) - 1] = '\0';

	return wpa_ctrl_command(ctrl, cmd);
}

static void wpa_cli_show_network_variables(void)
{
}

static int wpa_cli_cmd_set_network(struct wpa_ctrl *ctrl, int argc,
		const char *argv[])
{
	char cmd[256];
	int res;

	if (argc == 0) {
		wpa_cli_show_network_variables();
		return 0;
	}

	if (argc != 3) {
		printf("Invalid SET_NETWORK command: needs three arguments\n"
		       "(network id, variable name, and value)\n");
		return -1;
	}

	res = snprintf(cmd, sizeof(cmd), "SET_NETWORK %s %s %s",
			  argv[0], argv[1], argv[2]);
	if (res < 0 || (size_t) res >= sizeof(cmd) - 1) {
		printf("Too long SET_NETWORK command.\n");
		return -1;
	}
	return wpa_ctrl_command(ctrl, cmd);
}

static int wpa_cli_cmd_get_network(struct wpa_ctrl *ctrl, int argc,
		const char *argv[])
{
	char cmd[256];
	int res;

	if (argc == 0) {
		wpa_cli_show_network_variables();
		return 0;
	}

	if (argc != 2) {
		printf("Invalid GET_NETWORK command: needs two arguments\n"
		       "(network id and variable name)\n");
		return -1;
	}

	res = snprintf(cmd, sizeof(cmd), "GET_NETWORK %s %s",
			  argv[0], argv[1]);
	if (res < 0 || (size_t) res >= sizeof(cmd) - 1) {
		printf("Too long GET_NETWORK command.\n");
		return -1;
	}
	return wpa_ctrl_command(ctrl, cmd);
}

static int wpa_cli_cmd_disconnect(struct wpa_ctrl *ctrl, int argc,
		const char *argv[])
{
	return wpa_ctrl_command(ctrl, "DISCONNECT");
}

static int wpa_cli_cmd_reconnect(struct wpa_ctrl *ctrl, int argc,
		const char *argv[])
{
	return wpa_ctrl_command(ctrl, "RECONNECT");
}

static int wpa_cli_cmd_save_config(struct wpa_ctrl *ctrl, int argc,
		const char *argv[])
{
	return wpa_ctrl_command(ctrl, "SAVE_CONFIG");
}

static int wpa_cli_cmd_scan(struct wpa_ctrl *ctrl, int argc, const char *argv[])
{
	return wpa_ctrl_command(ctrl, "SCAN");
}

static int wpa_cli_cmd_scan_results(struct wpa_ctrl *ctrl, int argc,
		const char *argv[])
{
	return wpa_ctrl_command(ctrl, "SCAN_RESULTS");
}

static int wpa_cli_cmd_get_capability(struct wpa_ctrl *ctrl, int argc,
		const char *argv[])
{
	char cmd[64];

	if (argc < 1 || argc > 2) {
		printf("Invalid GET_CAPABILITY command: need either one or "
		       "two arguments\n");
		return -1;
	}

	if ((argc == 2) && strcmp(argv[1], "strict") != 0) {
		printf("Invalid GET_CAPABILITY command: second argument, "
		       "if any, must be 'strict'\n");
		return -1;
	}

	snprintf(cmd, sizeof(cmd), "GET_CAPABILITY %s%s", argv[0],
		    (argc == 2) ? " strict" : "");
	cmd[sizeof(cmd) - 1] = '\0';

	return wpa_ctrl_command(ctrl, cmd);
}

static int wpa_cli_list_interfaces(struct wpa_ctrl *ctrl)
{
	printf("Available interfaces:\n");
	return wpa_ctrl_command(ctrl, "INTERFACES");
}

static int wpa_cli_cmd_interface(struct wpa_ctrl *ctrl, int argc, const char *argv[])
{
	if (argc < 1) {
		wpa_cli_list_interfaces(ctrl);
		return 0;
	}

	wpa_cli_close_connection();
	free(ctrl_ifname);
	ctrl_ifname = strdup(argv[0]);

	if (wpa_cli_open_connection(ctrl_ifname)) {
		printf("Connected to interface '%s.\n", ctrl_ifname);
		if (wpa_ctrl_attach(ctrl_conn) == 0) {
			wpa_cli_attached = 1;
		} else {
			printf("Warning: Failed to attach to "
			       "wpa_supplicant.\n");
		}
	} else {
		printf("Could not connect to interface '%s' - re-trying\n",
			ctrl_ifname);
	}
	return 0;
}

static int wpa_cli_cmd_reconfigure(struct wpa_ctrl *ctrl, int argc,
		const  char *argv[])
{
	return wpa_ctrl_command(ctrl, "RECONFIGURE");
}

static int wpa_cli_cmd_terminate(struct wpa_ctrl *ctrl, int argc,
		const char *argv[])
{
	return wpa_ctrl_command(ctrl, "TERMINATE");
}

static int wpa_cli_cmd_interface_add(struct wpa_ctrl *ctrl, int argc,
		const char *argv[])
{
	char cmd[256];

	if (argc < 1) {
		printf("Invalid INTERFACE_ADD command: needs at least one "
		       "argument (interface name)\n"
		       "All arguments: ifname confname driver ctrl_interface "
		       "driver_param bridge_name\n");
		return -1;
	}

	/*
	 * INTERFACE_ADD <ifname>TAB<confname>TAB<driver>TAB<ctrl_interface>TAB
	 * <driver_param>TAB<bridge_name>
	 */
	snprintf(cmd, sizeof(cmd), "INTERFACE_ADD %s\t%s\t%s\t%s\t%s\t%s",
		    argv[0],
		    argc > 1 ? argv[1] : "", argc > 2 ? argv[2] : "",
		    argc > 3 ? argv[3] : "", argc > 4 ? argv[4] : "",
		    argc > 5 ? argv[5] : "");
	cmd[sizeof(cmd) - 1] = '\0';
	return wpa_ctrl_command(ctrl, cmd);
}

static int wpa_cli_cmd_interface_remove(struct wpa_ctrl *ctrl, int argc,
		const char *argv[])
{
	char cmd[128];

	if (argc != 1) {
		printf("Invalid INTERFACE_REMOVE command: needs one argument "
		       "(interface name)\n");
		return -1;
	}

	snprintf(cmd, sizeof(cmd), "INTERFACE_REMOVE %s", argv[0]);
	cmd[sizeof(cmd) - 1] = '\0';
	return wpa_ctrl_command(ctrl, cmd);
}

struct wpa_cli_cmd {
	const char *cmd;
	int (*handler)(struct wpa_ctrl *ctrl, int argc, const char *argv[]);
};

static struct wpa_cli_cmd wpa_cli_commands[] = {
	{ "status", wpa_cli_cmd_status },
	{ "ping", wpa_cli_cmd_ping },
	{ "mib", wpa_cli_cmd_mib },
	{ "interface", wpa_cli_cmd_interface },
	{ "level", wpa_cli_cmd_level },
	{ "set", wpa_cli_cmd_set },
	{ "logon", wpa_cli_cmd_logon },
	{ "logoff", wpa_cli_cmd_logoff },
	{ "pmksa", wpa_cli_cmd_pmksa },
	{ "reassociate", wpa_cli_cmd_reassociate },
	{ "preauthenticate", wpa_cli_cmd_preauthenticate },
	{ "identity", wpa_cli_cmd_identity },
	{ "password", wpa_cli_cmd_password },
	{ "new_password", wpa_cli_cmd_new_password },
	{ "pin", wpa_cli_cmd_pin },
	{ "otp", wpa_cli_cmd_otp },
	{ "passphrase", wpa_cli_cmd_passphrase },
	{ "bssid", wpa_cli_cmd_bssid },
	{ "list_networks", wpa_cli_cmd_list_networks },
	{ "select_network", wpa_cli_cmd_select_network },
	{ "enable_network", wpa_cli_cmd_enable_network },
	{ "disable_network", wpa_cli_cmd_disable_network },
	{ "add_network", wpa_cli_cmd_add_network },
	{ "remove_network", wpa_cli_cmd_remove_network },
	{ "set_network", wpa_cli_cmd_set_network },
	{ "get_network", wpa_cli_cmd_get_network },
	{ "save_config", wpa_cli_cmd_save_config },
	{ "disconnect", wpa_cli_cmd_disconnect },
	{ "reconnect", wpa_cli_cmd_reconnect },
	{ "scan", wpa_cli_cmd_scan },
	{ "scan_results", wpa_cli_cmd_scan_results },
	{ "get_capability", wpa_cli_cmd_get_capability },
	{ "reconfigure", wpa_cli_cmd_reconfigure },
	{ "terminate", wpa_cli_cmd_terminate },
	{ "interface_add", wpa_cli_cmd_interface_add },
	{ "interface_remove", wpa_cli_cmd_interface_remove },
	{ "ap_scan", wpa_cli_cmd_ap_scan },
	{ "stkstart", wpa_cli_cmd_stkstart },
	{ NULL, NULL }
};

/******************** wrapper from cmd to request *********************/
static int wpa_request(struct wpa_ctrl *ctrl, int argc, const char *argv[])
{
	struct wpa_cli_cmd *cmd, *match = NULL;
	int count;
	int ret = 0;

	count = 0;
	cmd = wpa_cli_commands;
	while (cmd->cmd) {
		if (strncasecmp(cmd->cmd, argv[0], strlen(argv[0])) == 0)
		{
			match = cmd;
			if (strcasecmp(cmd->cmd, argv[0]) == 0) {
				/* we have an exact match */
				count = 1;
				break;
			}
			count++;
		}
		cmd++;
	}

	if (count > 1) {
		printf("Ambiguous command '%s'; possible commands:", argv[0]);
		cmd = wpa_cli_commands;
		while (cmd->cmd) {
			if (strncasecmp(cmd->cmd, argv[0],
					   strlen(argv[0])) == 0) {
				printf(" %s", cmd->cmd);
			}
			cmd++;
		}
		printf("\n");
		ret = 1;
	} else if (count == 0) {
		printf("Unknown command '%s'\n", argv[0]);
		ret = 1;
	} else {
		ret = match->handler(ctrl, argc - 1, &argv[1]);
	}

	return ret;
}

static void wpa_cli_cleanup(void)
{
	wpa_cli_close_connection();
}

static char * wpa_cli_get_default_ifname(void)
{
	char *ifname = NULL;

	struct dirent *dent;
	DIR *dir = opendir(ctrl_iface_dir);
	if (!dir)
		return NULL;
	while ((dent = readdir(dir))) {
		/*
		 * Skip the file if it is not a socket. Also accept
		 * DT_UNKNOWN (0) in case the C library or underlying
		 * file system does not support d_type.
		 */
		if (dent->d_type != DT_SOCK && dent->d_type != DT_UNKNOWN)
			continue;

		if (strcmp(dent->d_name, ".") == 0 ||
		    strcmp(dent->d_name, "..") == 0)
			continue;
		ifname = strdup(dent->d_name);
		break;
	}
	closedir(dir);

	return ifname;
}


static int wpa_request_open(const char* ifname, const char* iface_dir)
{
	if (ifname) {
		if (ctrl_ifname) free(ctrl_ifname);
		ctrl_ifname = strdup(ifname);
	}

	if (ctrl_ifname == NULL)
		ctrl_ifname = wpa_cli_get_default_ifname();

	if (iface_dir)
		ctrl_iface_dir = iface_dir;

	ctrl_conn = wpa_cli_open_connection(ctrl_ifname);
	if (!ctrl_conn) {
			return -1;
	}

	return 0;
}

static void wpa_request_close()
{
	free(ctrl_ifname);
	ctrl_ifname = NULL;
	wpa_cli_cleanup();
}

static const char* wpa_cmd_results()
{
	return cmd_results;
}

static void clear_cmd_results()
{
	memset(cmd_results, 0, sizeof(cmd_results));
}

static int wpa_cmd_ok()
{
	const char *result = wpa_cmd_results();
	return !strncasecmp(result, "OK", 2);
}

#define WPA_REQUEST_CMD() \
do{	\
	ret = wpa_request_open(NULL, NULL); \
	if (!ret) { \
		ret = wpa_request(ctrl_conn, \
			sizeof(request_cmd)/(sizeof(char*)), \
			request_cmd); \
		wpa_request_close(); \
	} \
}while(0)

#define ID_LEN 5
#define INT2STR(str, id) \
	do {\
		snprintf((str), ID_LEN (id));\
	} while (0)

/************* using wrapper for various cmd from user space ************/
// these functions are exported to users 

static int wpa_get_status()
{
	const char *request_cmd[] = {"status"};
	int ret = 0;
	WPA_REQUEST_CMD();
	return (ret==0);
}

static int wpa_request_scan()
{
	const char *request_cmd[] = {"scan"};
	int ret = 0;
	WPA_REQUEST_CMD();
	return (ret==0);
}

static int wpa_get_scan_results()
{
	const char *request_cmd[] = {"scan_results"};
	int ret = 0;
	WPA_REQUEST_CMD();
	return (ret==0);
}


static int wpa_disable(const char* network)
{
	const char *request_cmd[] = {"disable_network",network};
	int ret = 0;
	WPA_REQUEST_CMD();
	return (ret==0);
}

static int wpa_set_ssid(const  char* network, const char *ssid )
{
	const char *request_cmd[4] = {"set_network",network,"ssid", ssid};
	 int ret = 0;
	 WPA_REQUEST_CMD();
	 return (ret==0);
}

static int wpa_set_scan_ssid(const char* network, int scan)
{
	const char *request_cmd[4] = {"set_network",network,"scan_ssid", scan?"1":"0"};
	int ret = 0;
	WPA_REQUEST_CMD();
	return (ret==0);
}

static int wpa_set_key_mgmt(const char* network, const char *key_mgmt)
{
	const char *request_cmd[4] = {"set_network",network,"key_mgmt", key_mgmt};
	int ret = 0;
	WPA_REQUEST_CMD();
	return (ret==0);
}

static int wpa_set_auth_alg(const char* network, const char *auth_alg)
{
	const char *request_cmd[4] = {"set_network",network,"auth_alg", auth_alg};
	int ret = 0;
	WPA_REQUEST_CMD();
	return (ret==0);
}

static int wpa_set_proto(const char* network, const char *proto)
{
	const char *request_cmd[4] = {"set_network",network,"proto", proto};
	int ret = 0;
	WPA_REQUEST_CMD();
	return (ret==0);
}

static int wpa_set_pairwise(const char* network, const char *pairwise)
{
	const char *request_cmd[4] = {"set_network",network,"pairwise", pairwise};
	int ret = 0;
	WPA_REQUEST_CMD();
	return (ret==0);
}

static int wpa_set_group(const char* network, const char *group)
{
	const char *request_cmd[4] = {"set_network",network,"group", group};
	int ret = 0;
	WPA_REQUEST_CMD();
	return (ret==0);
}

static int wpa_set_psk(const char* network, const char *psk)
{
	const char *request_cmd[4] = {"set_network",network,"psk", psk};
	int ret = 0;
	WPA_REQUEST_CMD();
	return (ret==0);
}

static int wpa_set_wep_key(const char* network, const char *key)
{
	const char *request_cmd[4] = {"set_network",network,"wep_key0", key};
	int ret = 0;
	WPA_REQUEST_CMD();
	return (ret==0);
}

static int wpa_enable(const char* network)
{
	const char *request_cmd[] = {"enable_network",network};
	int ret = 0;
	WPA_REQUEST_CMD();
	return (ret==0);
}


/********************** helper functions *****************************/

static const char* get_status_desc(void)
{
	if( !wpa_get_status() )
		clear_cmd_results();
	return wpa_cmd_results();
}

static inline char* create_double_quote(const char *str)
{
	char *tmp;
	tmp = (char *)malloc(strlen(str)+3);
	if (tmp) {
		strcpy(tmp, "\"");
		strcat(tmp, str);
		strcat(tmp, "\"");
	}
	return tmp;
}

static char* create_wifi_ssid(const char *ssid )
{
	return create_double_quote(ssid);
}

static inline void destroy_wifi_ssid( char* ssid )
{
	free(ssid);
}


static char* create_wep_key(const char *key)
{
	char *tmp = NULL;
	int len = strlen(key);

	switch( len )
	{
	case 5:
	case 13:
		tmp = (char *)malloc(len+3);
		if (tmp) {
			strcpy(tmp, "\"");
			strcat(tmp, key);
			strcat(tmp, "\"");
		}
		break;
	case 10:
	case 26:
	default:
		tmp = (char *)malloc(len+3);
		if (tmp) {
			strcpy(tmp, key);
		}
		break;
	}

	return tmp;
}

static inline void destroy_wep_key(char *key)
{
	free(key);
}


static char* create_wpa_psk(const char *psk)
{
	return create_double_quote(psk);
}

static inline void destroy_wpa_psk(char *psk)
{
	free(psk);
}

static void inline wifi_lock()
{
}

static void inline wifi_unlock()
{
}

/************************* interface functions ************************/

int Wifi_Enable(char *id, int enable)
{
	int ret;

	wifi_lock();
	ret = enable ? wpa_enable(id): wpa_disable(id);
	wifi_unlock();

	return ret;
}

int Wifi_SetScanSsid(char *id, int scan_ssid)
{
	return wpa_set_scan_ssid(id, scan_ssid);
}

static int wifi_open_auth(const char* id, const char *essid)
{
	char *ssid = NULL;

	int ret = 0;

	if(!(ssid = create_wifi_ssid(essid)))
		return 0;

	wpa_set_ssid(id, ssid);
	ret = wpa_cmd_ok();

	if ( ret && (ret = wpa_set_key_mgmt(id, "NONE")) )
		ret = wpa_cmd_ok();

	if ( ret && (ret  =wpa_set_auth_alg(id, "OPEN")) )
		ret = wpa_cmd_ok();

	destroy_wifi_ssid(ssid);

	return ret;
}


static int wifi_wep_auth(const char* id, const char* essid, const char* key)
{
	char *ssid = NULL;
	char *wep_key = NULL;
	int ret = 0;

	if( !(ssid = create_wifi_ssid(essid)) )
		return 0;

	if( !(wep_key = create_wep_key(key)) )
		goto end0;

	wpa_set_ssid(id, ssid);
	ret = wpa_cmd_ok();

	if ( ret && (ret = wpa_set_key_mgmt(id, "NONE")) )
		ret = wpa_cmd_ok();

	if ( ret && (ret = wpa_set_auth_alg(id, "OPEN SHARED")) )
		ret = wpa_cmd_ok();

	if ( ret && (ret = wpa_set_wep_key(id, wep_key)) )
		ret = wpa_cmd_ok();

	destroy_wep_key(wep_key);

end0:
	destroy_wifi_ssid(ssid);

	return ret;
}


static int wifi_wpa_auth(const char* network, const char* essid, const char* psk)
{
	char *ssid = NULL;
	char *key = NULL;
	int ret = 0;

	if( !(ssid = create_wifi_ssid(essid)) )
		return 0;

	if( !(key=create_wpa_psk(psk)) )
		goto end0;

	wpa_set_ssid(network, ssid);
	ret = wpa_cmd_ok();

	if ( ret && (ret=wpa_set_key_mgmt(network, "WPA-PSK")) )
		ret = wpa_cmd_ok();

	if ( ret && (ret=wpa_set_proto(network, "WPA RSN")) )
		ret = wpa_cmd_ok();

	if ( ret && (ret=wpa_set_pairwise(network, "TKIP CCMP")) )
		ret = wpa_cmd_ok();

	if ( ret && (ret=wpa_set_group(network, "TKIP CCMP")) )
		ret = wpa_cmd_ok();

	if ( ret && (ret=wpa_set_psk(network, key)) )
		ret = wpa_cmd_ok();

	destroy_wpa_psk(key);

end0:
	destroy_wifi_ssid(ssid);

	return ret;
}

#if 0
void Wifi_Authenticate(const char* ssid, const char* key)
{
	wifi_lock();

	wifi_open_auth("0", ssid);

	if (key&& strlen(key))
	{
		wifi_wep_auth("1", ssid, key);
		wifi_wpa_auth("2", ssid, key);
	} else {
		wifi_open_auth("1", ssid);
		wifi_open_auth("2", ssid);
	}

	wifi_unlock();
}
#endif

const char* Wifi_GetApList(void)
{
	int sync = 0;

	static char ap_list[CMD_RESULTS_LEN];

	wifi_lock();

	if( wpa_request_scan() && wpa_cmd_ok() ) {
		clear_cmd_results();
		if (sync) {
			while(0==strlen(wpa_cmd_results()))
				wpa_get_scan_results();
		} else {
			sleep(1);
			wpa_get_scan_results();
		}
	} else {
		clear_cmd_results();
	}

	memcpy(ap_list, wpa_cmd_results(), CMD_RESULTS_LEN);

	wifi_unlock();

	return ap_list;
}

enum wifi_status Wifi_get_status(void)
{
	enum wifi_status ret = WIFI_UNKNOWN_STATUS;

	wifi_lock();

	char *p = strstr(get_status_desc(), "wpa_state");
	if (p) {
		if(!strncasecmp(p, "wpa_state=COMPLETED", 19))
			ret = WIFI_CONNECTED;
		else if (!strncasecmp(p, "wpa_state=GROUP_HANDSHAKE", 25))
			ret = WIFI_GROUP_HANDSHAKE;
		else if (!strncasecmp(p, "wpa_state=4WAY_HANDSHAKE", 24))
			ret = WIFI_4WAY_HANDSHAKE;
		else if (!strncasecmp(p, "wpa_state=ASSOCIATED", 20))
			ret = WIFI_ASSOCIATED;
		else if (!strncasecmp(p, "wpa_state=ASSOCIATING", 21))
			ret = WIFI_ASSOCIATING;
		else if(!strncasecmp(p, "wpa_state=SCANNING", 18))
			ret = WIFI_SCANNING;
		else if(!strncasecmp(p, "wpa_state=INACTIVE", 18))
			ret = WIFI_INACTIVE;
		else if (!strncasecmp(p, "wpa_state=DISCONNECTED", 22))
			ret = WIFI_DISCONNECTED;
	}

	wifi_unlock();

	return (ret);
}

const char *Wifi_GetCurBssid(void)
{
	static char bssid[20]={0};

	wifi_lock();

	char *p = strstr(get_status_desc(), "bssid");
	if (p) {
		memcpy(bssid, p+6, 17);
		bssid[18]='\0';
	} else {
		memset(bssid, 0, sizeof(bssid));
	}

	wifi_unlock();

	return bssid;
}

const char *Wifi_GetCurKeyMgmt(void)
{
	static char key_mgmt[20]={0};
	const char *status, *pos;

	wifi_lock();
	status = get_status_desc();
	pos = strstr(status, "key_mgmt=");
	if (pos) {
		pos += 9;
		sscanf(pos, "%20s\n", key_mgmt);
		if(!strcmp(key_mgmt, "NONE"))
			if(strstr(status,"WEP"))
				strcpy(key_mgmt, "WEP");
	}
	wifi_unlock();

	return key_mgmt;
}

int Wifi_GetLastError(void)
{
	int ret = WIFI_UNKNOWN_ERROR;

	wifi_lock();

	char *p = strstr(get_status_desc(), "error");
	if (p) {
		if(!strncasecmp(p, "error=NONE", 10))
			ret = WIFI_NONE_ERROR;
		else if(!strncasecmp(p, "error=WPA_KEY", 13))
			ret = WIFI_KEY_ERROR;
		else if(!strncasecmp(p, "error=IFACE", 11))
			ret = WIFI_IFACE_ERROR;
		else if(!strncasecmp(p, "error=NO_AP", 11))
			ret = WIFI_NO_AP_ERROR;
	}

	wifi_unlock();

	return (ret);
}

struct ap_info {
	char bssid[18];
	int frequency;
	int signal_level;
	char crype[64];
	char ssid[32];
};

static const char* get_line(const char *content, char *line, int line_len)
{
	const char *pos;
	int n;

	pos = strstr(content, "\n");
	if (pos) {
		n = pos-content;
		pos += 1;
	} else {
		n = strlen(content);
		pos = content+n;
	}

	if (n>=line_len)
		n=line_len-1;
	strncpy(line, content, n);
	line[n]=0;

	return pos;
}

static int match_ap_with(const char *ssid, const struct ap_info *info)
{
	if(strlen(info->ssid)==0)
		return strcmp(ssid, info->crype);
	return strcmp(ssid, info->ssid);
}

static int get_signal_from_aplist(const char *ssid, const char *ap_list)
{
	struct ap_info info;
	const char *str = ap_list;

#define LINE_LEN 128
	char line[LINE_LEN];

	while (str&&(*str)) {
		str = get_line(str, line, LINE_LEN);
		memset(&info, 0, sizeof(info));
		sscanf(line, "%18s %d %d %64s %32s\n",
				info.bssid, &info.frequency, &info.signal_level, info.crype, info.ssid);

		if(!match_ap_with(ssid, &info))
			return info.signal_level;
	}

	return 0;
}

int Wifi_GetAPSignal(const char *ssid)
{
	const char *ap_list, *pos;

	ap_list = Wifi_GetApList();

	if (ap_list) {
		pos = strstr(ap_list, "\n");
		if (pos)
			ap_list = pos+1;
		else
			ap_list = NULL;
	}

	return get_signal_from_aplist(ssid, ap_list);
}


static int wifi_inited = 0;
int Wifi_Init(VOID)
{
	if (wifi_inited) {
		return 1;
	} else {
		system("/usr/wifi/wifi_init.sh");
		wifi_inited = 1;
		return 1;
	}
}

/*************************************************************/
int Wifi_set_apinfo(char *id, struct apinfo *pinfo)
{
	int ret = 0;

	if (!strcmp(pinfo->mgmt, MGMT_WPA_PSK)) {
		ret = wifi_wpa_auth(id, pinfo->ssid, pinfo->passwd);
	} else if (!strcmp(pinfo->mgmt, MGMT_WEP)) {
		ret = wifi_wep_auth(id, pinfo->ssid, pinfo->passwd);
	} else if (!strcmp(pinfo->mgmt, MGMT_OPEN)) {
		ret = wifi_open_auth(id, pinfo->ssid);
	}

	return ret;
}
