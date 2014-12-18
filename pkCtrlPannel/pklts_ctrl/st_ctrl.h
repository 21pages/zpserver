#ifndef ST_CTRL_H
#define ST_CTRL_H
#include <Qt>
namespace ParkinglotsSvr{
#pragma  pack (push,1)
	enum ErrMessage{
		ALL_SUCCEED          = 0,
		//Net Errors, 4Bit
		ERRNET_ConnectionFailed = 4,
		ERRNET_SendDataFailed   = 5,
		ERRNET_RecvDataFailed   = 6,
		//TransLayer Errors, 4Bit
		ERRTRANS_ERROR_MARK				 =0x10,
		ERRTRANS_LESS_DATA				 =0x11,
		ERRTRANS_DST_NOT_REACHABLE		 =0x12,
		ERRTRANS_ERROR_MSG_TYPE			 =0x13,
		ERRTRANS_ERROR_DATA              =0x14
	};

	//0x2000
	struct stMsg_GetHostDetailsReq{

	};

	//0x2800
	struct stMsg_GetHostDetailsRsp{
		quint8 DoneCode;
		quint16 HostType;
		quint16 FirmwareVersion;
		char HostName[64];
		char HostInfo[64];
		quint8 ConnetType;
		//1£ºGPRS£¬2£º3G£¬3£ºWAN£¬4£ºLAN
		quint8 IEEEAdd[8];
		quint8 IEEEAdd_Flag;
		quint8 PANID[2];
		quint8 PANID_Flag;
		quint8 EPANID[8];
		quint8 EPANID_Flag;
		quint16 SensorNum;
		quint16 RelayNum;
		quint16 ANSensorNum;
		quint16 ANRelayNum;
	};
	//0x2001
	struct stMsg_SetHostDetailsReq
	{
		char HostName[64];
		char HostInfo[64];
	};
	//0x2801
	struct stMsg_SetHostDetailsRsp{
		quint8 DoneCode;
	};
	//0x200A
	struct stMsg_RemoveDeviceReq {
		quint8 DeviceID[24];
	};
	//0x280A
	struct stMsg_RemoveDeviceRsp{
		quint8 DoneCode;
	};
	//0x200B
	struct stMsg_GetDeviceListReq{

	};
	//0x280B
	struct stMsg_GetDeviceListRsp
	{
		quint8   DoneCode;
		quint16  nDevCount;
		struct   stCall_DeviceNode
		{
			char DeviceName [32];
			char No[64];
			unsigned char DeviceID[24];
		} devicetable[1];
	};
	//0x200C
	struct stMsg_GetDeviceParamReq
	{
		quint8 DeviceID[24];
		quint8 Opt_DeviceName;
		quint8 Opt_DeviceInfo;
		quint8 Opt_DALStatus;
	};
	//0x280C
	struct stMsg_GetDeviceParamRsp
	{
		quint8 DoneCode;
		unsigned char DeviceID[24];
		quint8 Opt_DeviceName;
		quint8 Opt_DeviceInfo;
		quint8 Opt_DALStatus;
		char DeviceName[32];
		char DeviceInfo[64];
		quint16 DALStatusBytesLen;
		unsigned char DALStatusBytes[1];
	};

	//0x200D
	struct stMsg_setDeviceParamReq
	{
		unsigned char DeviceID[24];
		quint8 Opt_DeviceName;
		quint8 Opt_DeviceInfo;
		char DeviceName[32];
		char DeviceInfo[64];
	};
	//0x280D
	struct stMsg_setDeviceParamRsp
	{
		quint8 DoneCode;
	};
	//0x280E
	struct stMsg_DeviceCtrlReq
	{
		unsigned char DeviceID[24];
		quint16 DALArrayLength;
	};
	//0x280E
	struct stMsg_DeviceCtrlRsp
	{
		quint8 DoneCode;
	};
	#pragma  pack (pop)
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


}

#endif
