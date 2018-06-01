#ifndef WPA_CTRL_H
#define WPA_CTRL_H

#include <sys/types.h>

#define WPA_CTRL_REQ "CTRL-REQ-"
#define WPA_CTRL_RSP "CTRL-RSP-"
#define WPA_EVENT_CONNECTED "CTRL-EVENT-CONNECTED "
#define WPA_EVENT_DISCONNECTED "CTRL-EVENT-DISCONNECTED "
#define WPA_EVENT_TERMINATING "CTRL-EVENT-TERMINATING "
#define WPA_EVENT_PASSWORD_CHANGED "CTRL-EVENT-PASSWORD-CHANGED "
#define WPA_EVENT_EAP_NOTIFICATION "CTRL-EVENT-EAP-NOTIFICATION "
#define WPA_EVENT_EAP_STARTED "CTRL-EVENT-EAP-STARTED "
#define WPA_EVENT_EAP_METHOD "CTRL-EVENT-EAP-METHOD "
#define WPA_EVENT_EAP_SUCCESS "CTRL-EVENT-EAP-SUCCESS "
#define WPA_EVENT_EAP_FAILURE "CTRL-EVENT-EAP-FAILURE "


struct wpa_ctrl * wpa_ctrl_open(const char *ctrl_path);
void wpa_ctrl_close(struct wpa_ctrl *ctrl);
int wpa_ctrl_request(struct wpa_ctrl *ctrl, const char *cmd, size_t cmd_len,
		     char *reply, size_t *reply_len,
		     void (*msg_cb)(char *msg, size_t len));
int wpa_ctrl_attach(struct wpa_ctrl *ctrl);
int wpa_ctrl_detach(struct wpa_ctrl *ctrl);
int wpa_ctrl_recv(struct wpa_ctrl *ctrl, char *reply, size_t *reply_len);
int wpa_ctrl_pending(struct wpa_ctrl *ctrl);
int wpa_ctrl_get_fd(struct wpa_ctrl *ctrl);

#endif /* WPA_CTRL_H */
