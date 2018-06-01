/*
 * mach_conf_common.h
 *
 *  Created on: 2014-8-21
 *      Author: 50215243
 */

#ifndef MACH_CONF_COMMON_H_
#define MACH_CONF_COMMON_H_

#define MACH_CONF_NAME_LEN 15

enum mach_conf_type
{
	MACH_CONF_TYPE_AM1808,
	MACH_CONF_TYPE_AM335X,
	MACH_CONF_TYPE_INVALID,
};

struct machconf
{
	struct machconf_ops *ops;
	char name[MACH_CONF_NAME_LEN + 1];
	enum mach_conf_type type;
};

struct machconf_ops
{
	INT32 (* init)(struct machconf* board);
	INT32 (* release)(struct machconf* board);
	INT32 (* read_config)(struct machconf* board,CHAR *buffer, INT32 size);
	INT32 (* set_elecserno)(struct machconf* board, const CHAR *pcElecSerialNo, INT32 iLength);
	INT32 (* set_machtype)(struct machconf* board, const CHAR *pcMachType, INT32 iLength);
	BOOL (* get_parseresult)(struct machconf* board, UINT64 *result);
};

void InitMachConfAdapters();
void Init_MachConf_Am1808_ops();
void Init_MachConf_Am335x_ops();
struct machconf_ops* Get_MachConf_Am1808_ops();
struct machconf_ops* Get_MachConf_Am335x_ops();
struct machconf* MachConfSetup(const char* name, enum mach_conf_type type);

#endif /* MACH_CONF_COMMON_H_ */
