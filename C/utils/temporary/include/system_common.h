/*
 * system_commmon.h
 *
 *  Created on: 2014-7-10
 *      Author: 50215243
 */

#ifndef SYSTEM_COMMON_H_
#define SYSTEM_COMMON_H_

#include "systemAccess.h"

#define SYSTEM_NAME_LEN 15

enum board_type
{
	BOARD_TYPE_AM335X,
	BOARD_TYPE_AM1808,
	BOARD_TYPE_DIPPER,
	BOARD_TYPE_INVALID,
};

struct system_board
{
	struct system_ops *ops;
	char name[SYSTEM_NAME_LEN + 1];
	enum board_type type;
};

struct system_ops
{
	INT32 (* GetCpuStatus)(struct system_board* board, int* status, int num);
	INT32 (* GetCpuRate)(struct system_board* board);
	BOOL (* GetAllTaskRunTime)(struct system_board* board, ULONG* us, UINT32 size);
	BOOL (* GetMemInfo)(struct system_board* board, int& total, int& free);
	INT32 (* GetKernelVersion)(struct system_board* board, char* buffer, int size);
	INT32 (* GetUbootVersion)(struct system_board* board, char* buffer, int size);
	INT32 (* GetBiosVersion)(struct system_board* board, char* buffer, int size);
	INT32 (* GetSelftestResult)(struct system_board* board, unsigned int* presult);
	BOOL (* SetKernelTime)(struct system_board* board, const MM_SYSTIME rtcTime);
	INT32 (* GetMachType)(struct system_board* board, char* buffer, int size);
	BOOL (*GetFirmwareVersion)(struct system_board *board, enum fw_id id, char *buf, int size);
};

BOOL System_SetKernelTime_Common(struct system_board* board, const MM_SYSTIME rtcTime);
INT32 System_GetSelftestResult_Common(struct system_board* board, UINT32 *presult);
INT32 System_GetBiosVersion_Common(struct system_board* board, CHAR *buffer, INT32 size);
INT32 System_GetKernelVersion_Common(struct system_board* board, CHAR *buffer, INT32 size);
BOOL System_GetMemInfo_Common(struct system_board* board, INT32& total, INT32& free);
INT32 System_GetUbootVersion_Common(struct system_board* board, CHAR *buffer, INT32 size);

void InitSystemAdapters();
void Init_System_Am335x_ops();
void Init_System_Am1808_ops();
void Init_System_dipper_ops();
struct system_ops* Get_System_Am335x_ops();
struct system_ops* Get_System_Am1808_ops();
struct system_ops* Get_System_dipper_ops();
struct system_board* System_Setup(const char* name, enum board_type type);

#endif /* SYSTEM_COMMON_H_ */
