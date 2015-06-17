#ifndef ST_CTRLMESSAGE_H
#define ST_CTRLMESSAGE_H

namespace ParkinglotsSvr{
#define MAXPARAMDATALEN 256
#define MAXPARAMNUM 8

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
		//1:GPRS,2:3G,3:WAN,4:LAN
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


	//0x2002
	struct stMsg_PushFirmUpPackReq{
		quint16 SectionNum;		//total sections
		quint16 SectionIndex;	//current sections
		quint16 SectionLen;		//current length（<=4096）
		/*quint8 pSectionData[1];*/	//data
	};

	//0x2802
	struct stMsg_PushFirmUpPackRsp{
		quint8 DoneCode;		//recieve mark,0 succeed,1 failed
		quint16 SectionNum;		//section num
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

	enum dal_datatype{
		DAL_TYPE_NODATA	=0x00,
		DAL_TYPE_BOOL	=0x10,
		DAL_TYPE_UINT8	=0x20,
		DAL_TYPE_UINT16	=0x21,
		DAL_TYPE_UINT24	=0x22,
		DAL_TYPE_UINT32	=0x23,
		DAL_TYPE_UINT40	=0x24,
		DAL_TYPE_UINT48	=0x25,
		DAL_TYPE_UINT56	=0x26,
		DAL_TYPE_UINT64	=0x27,
		DAL_TYPE_INT8	=0x28,
		DAL_TYPE_INT16	=0x29,
		DAL_TYPE_INT24	=0x2A,
		DAL_TYPE_INT32	=0x2B,
		DAL_TYPE_INT40	=0x2C,
		DAL_TYPE_INT48	=0x2D,
		DAL_TYPE_INT56	=0x2E,
		DAL_TYPE_INT64	=0x2F,
		DAL_TYPE_ENUM8	=0x30,
		DAL_TYPE_SINGLEFLOAT	=0x39,
		DAL_TYPE_DOUBLEFLOAT	=0x3A,
		DAL_TYPE_STRING	=0x42
	};


	struct PKLTS_Message{
		struct tag_trans_header{
			quint16 Mark;    //Always be 0xAA55
			quint16 ProtoVersion;
			quint8 Priority;
			quint8 Reserved1;
			quint16 SerialNum;
			qint32 SrcID ; //0xffffffff means from server
			qint32 DstID ; //0xffffffff means to server
			quint16 DataLen; //Data at least have 2 byte, for heart-beating counter.
			quint16 Reserved2;
		} trans_header;
		union uni_trans_payload{
			quint8 data[1];
			struct tag_pklts_app_layer{
				struct tag_app_layer_header{
					quint16 MsgFmtVersion;
					quint8 AskID;
					quint16 MsgType;
				} app_header;
				union  union_MsgUnion
				{
					stMsg_GetHostDetailsReq msg_GetHostDetailsReq;
					stMsg_GetHostDetailsRsp msg_GetHostDetailsRsp;
					stMsg_SetHostDetailsReq msg_SetHostDetailsReq;
					stMsg_SetHostDetailsRsp msg_SetHostDetailsRsp;
					stMsg_RemoveDeviceReq   msg_RemoveDeviceReq;
					stMsg_RemoveDeviceRsp   msg_RemoveDeviceRsp;
					stMsg_GetDeviceParamReq	msg_GetDeviceParamReq;
					stMsg_GetDeviceParamRsp	msg_GetDeviceParamRsp;
					stMsg_setDeviceParamReq msg_setDeviceParamReq;
					stMsg_setDeviceParamRsp msg_setDeviceParamRsp;
					stMsg_DeviceCtrlReq msg_DeviceCtrlReq;
					stMsg_DeviceCtrlRsp msg_DeviceCtrlRsp;
					quint8 msg[1];
				} app_data;
			} app_layer;
		} trans_payload;
	} ;
	//------------------------------------------------------------------
	/**Trans Layer Message Type
	 * Heartbeating message
	**/
	struct PKLTS_Heartbeating
	{
		quint16 Mark;  //Always be 0xBeBe
		//qint32 source_id ; //0xffffffff means from server
		quint16 tmStamp;
	} ;

#pragma pack(pop)
	typedef PKLTS_Message::tag_trans_header PKLTS_Trans_Header;
	typedef PKLTS_Message::uni_trans_payload::tag_pklts_app_layer::tag_app_layer_header PKLTS_App_Header;
	typedef PKLTS_Message::uni_trans_payload::tag_pklts_app_layer PKLTS_App_Layer;
}

#endif // ST_MESSAGE_H


