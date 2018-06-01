/* ****************************************************************************
 * file:	uart_common.h
 * date:	2014-09-03
 * func:	header file for uart function; only used internal
 * author:	50214332
 * ****************************************************************************/
#ifndef	__UART_COMMON_H__
#define	__UART_COMMON_H__

#include "UartAccess.h"

#define	UART_NAME_LEN		20

enum uart_type {
	UART_TYPE_SERIAL,
	UART_TYPE_SPIBRIDGE,
};

struct uart_ops {
	INT32 (* Uart_Open)(struct uart_dev *dev, const CHAR *pDeviceName);
	INT32 (* Uart_Close)(struct uart_dev *dev, INT32 hDeviceHandle);
	BOOL  (* Uart_SetConfig)(struct uart_dev *dev,INT32 hDeviceHandle,enum UART_DATABITS enDataBits, enum UART_PARITY_MODE enParity, enum UART_STOPBITS enStopBits, INT32 nBaudRate);
	VOID  (* Uart_DisableTx)(struct uart_dev *dev,INT32 hDeviceHandle);
    VOID  (* Uart_EnableTx)(struct uart_dev *dev,INT32 hDeviceHandle);
    VOID  (* Uart_DisableRx)(struct uart_dev *dev,INT32 hDeviceHandle);
    VOID  (* Uart_EnableRx)(struct uart_dev *dev,INT32 hDeviceHandle);
    INT32 (* Uart_Read)(struct uart_dev *dev,INT32 hDeviceHandle, CHAR *pBuff, INT32 nLength);
    INT32 (* Uart_Write)(struct uart_dev *dev,INT32 hDeviceHandle, CHAR *pBuff, INT32 nLength);
};

struct uart_dev {
	struct uart_ops *ops;
	CHAR name[UART_NAME_LEN + 1];
	const CHAR *path;  					// path of dev node or sys node
	enum uart_type type;
};

INT InitUartAdapters(VOID);
VOID init_uart_serial(VOID);
VOID init_uart_spibridge(VOID);

struct uart_dev *UartSetup(const CHAR *name, const CHAR *path, enum uart_type);
struct uart_ops *Get_Uart_Serial_Ops(VOID);
struct uart_ops *Get_Uart_SpiBridge_Ops(VOID);

#endif
