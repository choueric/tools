#ifndef _BRIDGE_H
#define _BRIDGE_H

short read_rx_dep(int fd);
short read_tx_dep(int fd);
int recv_data(int fd, unsigned char *buf, int len);
int send_data(int fd, unsigned char *buf, int len);
unsigned char *find_vir_dev_reply(unsigned char *buf, int data_size);

#endif
