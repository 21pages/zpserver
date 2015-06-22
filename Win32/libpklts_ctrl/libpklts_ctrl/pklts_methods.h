#ifndef ST_CTRL_H
#define ST_CTRL_H
#include <windows.h>
#include <tchar.h>
#include "st_ctrlmsg.h"


namespace ParkinglotsCtrl{
	unsigned __int32  st_getMACInfo(
			const char * address,
			unsigned __int16 port,
			unsigned __int32 macID,
			stMsg_GetHostDetailsRsp * pOutputBuf);

	unsigned __int32  st_setHostDetails(
			const char * address,
			unsigned __int16 port,
			unsigned __int32 macID,
			const stMsg_SetHostDetailsReq * pInData,
			stMsg_SetHostDetailsRsp * pOutputBuf);

	unsigned __int32  st_removeDevice(
			const char * address,
			unsigned __int16 port,
			unsigned __int32 macID,
			const stMsg_RemoveDeviceReq * pInData,
			stMsg_RemoveDeviceRsp * pOutputBuf);

	unsigned __int32  st_getDeviceList(
			const char * address,
			unsigned __int16 port,
			unsigned __int32 macID,
			stMsg_GetDeviceListRsp ** ppOutputBuf);

	void  st_freeDeviceList(
			stMsg_GetDeviceListRsp * pOutputBuf);

	unsigned __int32  st_getDeviceParam(
			const char * address,
			unsigned __int16 port,
			unsigned __int32 macID,
			const stMsg_GetDeviceParamReq * pInBuf,
			stMsg_GetDeviceParamRsp ** ppOutputBuf);

	void  st_freeDeviceParam(
			stMsg_GetDeviceParamRsp * pOutputBuf);

	unsigned __int32  st_setDeviceParam(
			const char * address,
			unsigned __int16 port,
			unsigned __int32 macID,
			const stMsg_setDeviceParamReq * pInData,
			stMsg_setDeviceParamRsp * pOutputBuf);

	unsigned __int32  st_deviceCtrl(
			const char * address,
			unsigned __int16 port,
			unsigned __int32 macID,
			const stMsg_DeviceCtrlReq * pInData,
			const unsigned __int8 * pDAL,
			stMsg_DeviceCtrlRsp * pOutputBuf);

	unsigned __int32 st_updateFirmware(
			const char * address,
			unsigned __int16 port,
			unsigned __int32 macID,
			const stMsg_PushFirmUpPackReq * pInData,
			const unsigned __int8 * pblock,
			stMsg_PushFirmUpPackRsp *pOutputBuf
			);
}

#endif