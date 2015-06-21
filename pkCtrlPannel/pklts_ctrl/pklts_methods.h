#ifndef ST_CTRL_H
#define ST_CTRL_H
#include <Qt>
#include <string>
#include <QByteArray>
#include "st_ctrlmsg.h"
namespace ParkinglotsCtrl{

	quint32  st_getMACInfo(
			const char * address,
			quint16 port,
			quint32 macID,
			stMsg_GetHostDetailsRsp * pOutputBuf);

	quint32  st_setHostDetails(
			const char * address,
			quint16 port,
			quint32 macID,
			const stMsg_SetHostDetailsReq * pInData,
			stMsg_SetHostDetailsRsp * pOutputBuf);

	quint32  st_removeDevice(
			const char * address,
			quint16 port,
			quint32 macID,
			const stMsg_RemoveDeviceReq * pInData,
			stMsg_RemoveDeviceRsp * pOutputBuf);

	quint32  st_getDeviceList(
			const char * address,
			quint16 port,
			quint32 macID,
			stMsg_GetDeviceListRsp ** ppOutputBuf);

	void  st_freeDeviceList(
			stMsg_GetDeviceListRsp * pOutputBuf);

	quint32  st_getDeviceParam(
			const char * address,
			quint16 port,
			quint32 macID,
			const stMsg_GetDeviceParamReq * pInBuf,
			stMsg_GetDeviceParamRsp ** ppOutputBuf);

	void  st_freeDeviceParam(
			stMsg_GetDeviceParamRsp * pOutputBuf);

	quint32  st_setDeviceParam(
			const char * address,
			quint16 port,
			quint32 macID,
			const stMsg_setDeviceParamReq * pInData,
			stMsg_setDeviceParamRsp * pOutputBuf);

	quint32  st_deviceCtrl(
			const char * address,
			quint16 port,
			quint32 macID,
			const stMsg_DeviceCtrlReq * pInData,
			const quint8 * pDAL,
			stMsg_DeviceCtrlRsp * pOutputBuf);

	quint32 st_updateFirmware(
			const char * address,
			quint16 port,
			quint32 macID,
			const stMsg_PushFirmUpPackReq * pInData,
			const quint8 * pblock,
			stMsg_PushFirmUpPackRsp *pOutputBuf
			);

	//Toolkit function
	bool devidStr2Array(const std::string & strDevid, unsigned char array[/*24*/], int arrayMaxSize);
	bool HexStr2Array(const std::string & hexstring,QByteArray * array);
}

#endif
