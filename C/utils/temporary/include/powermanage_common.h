/*
 * powermanage_common.h
 *
 *  Created on: 2014-7-9
 *      Author: 50215243
 */

#ifndef POWERMANAGE_COMMON_H_
#define POWERMANAGE_COMMON_H_

#include "PowerManageAccess.h"

#define PMIC_BUCK1_ADDRESS (1)
#define PMIC_BUCK2_ADDRESS (2)
#define PMIC_BUCK3_ADDRESS (3)
#define PMIC_BUCK4_ADDRESS (4)
#define PMIC_BOOST_ADDRESS (5)
#define PMIC_BUCK_BOOST_ADDRESS (6)
#define PMIC_LED_CONFIG_ADDRESS (7)
#define PMIC_LED_DAC_ADDRESS (8)

BOOL Set_PMIC_Reg(const BYTE reg, const BYTE value);

#define POWERMANAGE_NAME_LEN 15

enum pm_board_type
{
	PM_BOARD_TYPE_AM1808,
	PM_BOARD_TYPE_AM335X,
	PM_BOARD_TYPE_IMC,
	PM_BOARD_TYPE_INVALID,
};

struct powermanage
{
	struct powermanage_ops *ops;
	char name[POWERMANAGE_NAME_LEN + 1];
	enum pm_board_type type;
};

struct powermanage_ops
{
	BOOL (* init)(struct powermanage* board);
	BOOL (* suspend_resume_manage)(struct powermanage* board, SUSPEND_RESUME_FLAG flag, MODULE_POWERMANAGE module);
	BOOL (* reboot)(struct powermanage* board, BOOL syncData);
	BOOL (* poweroff)(struct powermanage* board, BOOL syncData);
	BOOL (* change_cpu_freq)(struct powermanage* board, SYSTEM_FREQ freq);
	BOOL (* get_realtime_power_info)(struct powermanage* board, struct realtime_power_info *realtimePowerInfo); 
	BOOL (* get_acquire_power_info)(struct powermanage* board, struct acquire_power_info *acquirePowerInfo);
	BOOL (* get_power_off_flag)(struct powermanage* board);
};

void InitPowerManageAdapters();
void Init_PowerManage_Am1808_ops();
void Init_PowerManage_Am335x_ops();
void Init_PowerManage_Imc_ops();

struct powermanage_ops* Get_PowerManage_Am1808_ops();
struct powermanage_ops* Get_PowerManage_Am335x_ops();
struct powermanage_ops* Get_PowerManage_Imc_ops();
struct powermanage* PowerManageSetup(const char* name, enum pm_board_type type);

struct powermanage *GetPowerBoardDev();

BOOL SetPowerBoardDev(struct powermanage *dev);

#endif /* POWERMANAGE_COMMON_H_ */
