#include <sys/un.h>
#include "wpa_ctrl.h"
#include "comm.h"


struct wpa_ctrl {
	int s;
	struct sockaddr_un local;
	struct sockaddr_un dest;
};


struct wpa_ctrl * wpa_ctrl_open(const char *ctrl_path)
{
	struct wpa_ctrl *ctrl;
	static int counter = 0;

	ctrl = (struct wpa_ctrl*)malloc(sizeof(*ctrl));
	if (ctrl == NULL)
		return NULL;
	memset(ctrl, 0, sizeof(*ctrl));

	ctrl->s = socket(PF_UNIX, SOCK_DGRAM, 0);
	if (ctrl->s < 0) {
		free(ctrl);
		return NULL;
	}

	ctrl->local.sun_family = AF_UNIX;
	snprintf(ctrl->local.sun_path, sizeof(ctrl->local.sun_path),
		    "/tmp/wpa_ctrl_%d-%d", getpid(), counter++);
	if (bind(ctrl->s, (struct sockaddr *) &ctrl->local,
		    sizeof(ctrl->local)) < 0) {
		close(ctrl->s);
		free(ctrl);
		return NULL;
	}

	ctrl->dest.sun_family = AF_UNIX;
	snprintf(ctrl->dest.sun_path, sizeof(ctrl->dest.sun_path), "%s",
		    ctrl_path);
	if (connect(ctrl->s, (struct sockaddr *) &ctrl->dest,
		    sizeof(ctrl->dest)) < 0) {
		close(ctrl->s);
		unlink(ctrl->local.sun_path);
		free(ctrl);
		return NULL;
	}

	return ctrl;
}

void wpa_ctrl_close(struct wpa_ctrl *ctrl)
{
	close(ctrl->s);
	unlink(ctrl->local.sun_path);
	free(ctrl);
}

int wpa_ctrl_request(struct wpa_ctrl *ctrl, const char *cmd, size_t cmd_len,
		     char *reply, size_t *reply_len,
		     void (*msg_cb)(char *msg, size_t len))
{
	struct timeval tv;
	int res;
	fd_set rfds;
	const char *_cmd;
	size_t _cmd_len;

	_cmd = cmd;
	_cmd_len = cmd_len;

	if (send(ctrl->s, _cmd, _cmd_len, 0) < 0) {
		return -1;
	}

	for (;;) {
		tv.tv_sec = 2;
		tv.tv_usec = 0;
		FD_ZERO(&rfds);
		FD_SET(ctrl->s, &rfds);
		res = select(ctrl->s + 1, &rfds, NULL, NULL, &tv);
		if (FD_ISSET(ctrl->s, &rfds)) {
			res = recv(ctrl->s, reply, *reply_len, 0);
			if (res < 0)
				return res;
			if (res > 0 && reply[0] == '<') {
				if (msg_cb) {
					if ((size_t) res == *reply_len)
						res = (*reply_len) - 1;
					reply[res] = '\0';
					msg_cb(reply, res);
				}
				continue;
			}
			*reply_len = res;
			break;
		} else {
			return -2;
		}
	}
	return 0;
}

static int wpa_ctrl_attach_helper(struct wpa_ctrl *ctrl, int attach)
{
	char buf[10];
	int ret;
	size_t len = 10;

	ret = wpa_ctrl_request(ctrl, attach ? "ATTACH" : "DETACH", 6,
			       buf, &len, NULL);
	if (ret < 0)
		return ret;
	if (len == 3 && memcmp(buf, "OK\n", 3) == 0)
		return 0;
	return -1;
}

int wpa_ctrl_attach(struct wpa_ctrl *ctrl)
{
	return wpa_ctrl_attach_helper(ctrl, 1);
}

int wpa_ctrl_detach(struct wpa_ctrl *ctrl)
{
	return wpa_ctrl_attach_helper(ctrl, 0);
}

int wpa_ctrl_recv(struct wpa_ctrl *ctrl, char *reply, size_t *reply_len)
{
	int res;

	res = recv(ctrl->s, reply, *reply_len, 0);
	if (res < 0)
		return res;
	*reply_len = res;
	return 0;
}

int wpa_ctrl_pending(struct wpa_ctrl *ctrl)
{
	struct timeval tv;
	fd_set rfds;
	tv.tv_sec = 0;
	tv.tv_usec = 0;
	FD_ZERO(&rfds);
	FD_SET(ctrl->s, &rfds);
	select(ctrl->s + 1, &rfds, NULL, NULL, &tv);
	return FD_ISSET(ctrl->s, &rfds);
}

int wpa_ctrl_get_fd(struct wpa_ctrl *ctrl)
{
	return ctrl->s;
}
