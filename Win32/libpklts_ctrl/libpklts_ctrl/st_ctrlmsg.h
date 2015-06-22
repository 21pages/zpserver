#ifndef ST_CTRLMESSAGE_H
#define ST_CTRLMESSAGE_H

namespace ParkinglotsCtrl{
#pragma  pack (push,1)
	enum ErrMessage{
		ALL_SUCCEED          = 0,
		//Net Errors, 4Bit
		ERRNET_WSAStartupFailed = 1,
		ERRNET_GetAddrInfoError = 2,
		ERRNET_SocketFailedErr  = 3,
		ERRNET_ConnectionFailed = 4,
		ERRNET_SendDataFailed   = 5,
		ERRNET_RecvDataFailed   = 6,
		ERRNET_ShutDownFailed   = 7,
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
		unsigned __int8 DoneCode;
		unsigned __int16 HostType;
		unsigned __int16 FirmwareVersion;
		char HostName[64];
		char HostInfo[64];
		unsigned __int8 ConnetType;
		//1:GPRS,2:3G,3:WAN,4:LAN
		unsigned __int8 IEEEAdd[8];
		unsigned __int8 IEEEAdd_Flag;
		unsigned __int8 PANID[2];
		unsigned __int8 PANID_Flag;
		unsigned __int8 EPANID[8];
		unsigned __int8 EPANID_Flag;
		unsigned __int16 SensorNum;
		unsigned __int16 RelayNum;
		unsigned __int16 ANSensorNum;
		unsigned __int16 ANRelayNum;
	};
	//0x2001
	struct stMsg_SetHostDetailsReq
	{
		char HostName[64];
		char HostInfo[64];
	};
	//0x2801
	struct stMsg_SetHostDetailsRsp{
		unsigned __int8 DoneCode;
	};


	//0x2002
	struct stMsg_PushFirmUpPackReq{
		unsigned __int16 SectionNum;		//total sections
		unsigned __int16 SectionIndex;	//current sections
		unsigned __int16 SectionLen;		//current length（<=4096）
		/*unsigned __int8 pSectionData[1];*/	//data
	};

	//0x2802
	struct stMsg_PushFirmUpPackRsp{
		unsigned __int8 DoneCode;		//recieve mark,0 succeed,1 failed
		unsigned __int16 SectionNum;		//section num
	};




	//0x200A
	struct stMsg_RemoveDeviceReq {
		unsigned __int8 DeviceID[24];
	};
	//0x280A
	struct stMsg_RemoveDeviceRsp{
		unsigned __int8 DoneCode;
	};
	//0x200B
	struct stMsg_GetDeviceListReq{

	};
	//0x280B
	struct stMsg_GetDeviceListRsp
	{
		unsigned __int8   DoneCode;
		unsigned __int16  nDevCount;
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
		unsigned __int8 DeviceID[24];
		unsigned __int8 Opt_DeviceName;
		unsigned __int8 Opt_DeviceInfo;
		unsigned __int8 Opt_DALStatus;
	};
	//0x280C
	struct stMsg_GetDeviceParamRsp
	{
		unsigned __int8 DoneCode;
		unsigned char DeviceID[24];
		unsigned __int8 Opt_DeviceName;
		unsigned __int8 Opt_DeviceInfo;
		unsigned __int8 Opt_DALStatus;
		char DeviceName[32];
		char DeviceInfo[64];
		unsigned __int16 DALStatusBytesLen;
		unsigned char DALStatusBytes[1];
	};

	//0x200D
	struct stMsg_setDeviceParamReq
	{
		unsigned char DeviceID[24];
		unsigned __int8 Opt_DeviceName;
		unsigned __int8 Opt_DeviceInfo;
		char DeviceName[32];
		char DeviceInfo[64];
	};
	//0x280D
	struct stMsg_setDeviceParamRsp
	{
		unsigned __int8 DoneCode;
	};
	//0x280E
	struct stMsg_DeviceCtrlReq
	{
		unsigned char DeviceID[24];
		unsigned __int16 DALArrayLength;
	};
	//0x280E
	struct stMsg_DeviceCtrlRsp
	{
		unsigned __int8 DoneCode;
	};

	struct PKLTS_Message{
		struct tag_trans_header{
			unsigned __int16 Mark;    //Always be 0xAA55
			unsigned __int16 ProtoVersion;
			unsigned __int8 Priority;
			unsigned __int8 Reserved1;
			unsigned __int16 SerialNum;
			__int32 SrcID ; //0xffffffff means from server
			__int32 DstID ; //0xffffffff means to server
			unsigned __int16 DataLen; //Data at least have 2 byte, for heart-beating counter.
			unsigned __int16 Reserved2;
		} trans_header;
		union uni_trans_payload{
			unsigned __int8 data[1];
			struct tag_pklts_app_layer{
				struct tag_app_layer_header{
					unsigned __int16 MsgFmtVersion;
					unsigned __int8 AskID;
					unsigned __int16 MsgType;
				} app_header;
				union  union_MsgUnion
				{
					stMsg_GetHostDetailsReq msg_GetHostDetailsReq;
					stMsg_GetHostDetailsRsp msg_GetHostDetailsRsp;
					stMsg_SetHostDetailsReq msg_SetHostDetailsReq;
					stMsg_SetHostDetailsRsp msg_SetHostDetailsRsp;
					stMsg_PushFirmUpPackReq msg_PushFirmUpPackReq;
					stMsg_PushFirmUpPackRsp msg_PushFirmUpPackRsp;
					stMsg_RemoveDeviceReq   msg_RemoveDeviceReq;
					stMsg_RemoveDeviceRsp   msg_RemoveDeviceRsp;
					stMsg_GetDeviceListReq msg_GetDeviceListReq;
					stMsg_GetDeviceListRsp msg_GetDeviceListRsp;
					stMsg_GetDeviceParamReq	msg_GetDeviceParamReq;
					stMsg_GetDeviceParamRsp	msg_GetDeviceParamRsp;
					stMsg_setDeviceParamReq msg_setDeviceParamReq;
					stMsg_setDeviceParamRsp msg_setDeviceParamRsp;
					stMsg_DeviceCtrlReq msg_DeviceCtrlReq;
					stMsg_DeviceCtrlRsp msg_DeviceCtrlRsp;
					unsigned __int8 msg[1];
				} app_data;
			} app_layer;
		} trans_payload;
	} ;

#pragma pack(pop)
	typedef PKLTS_Message::tag_trans_header PKLTS_Trans_Header;
	typedef PKLTS_Message::uni_trans_payload::tag_pklts_app_layer::tag_app_layer_header PKLTS_App_Header;
	typedef PKLTS_Message::uni_trans_payload::tag_pklts_app_layer PKLTS_App_Layer;
}

#endif // ST_MESSAGE_H


