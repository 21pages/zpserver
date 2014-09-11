#ifndef ST_CTRL_H
#define ST_CTRL_H
#include <windows.h>
#include <tchar.h>
#pragma  pack (push,1)
namespace ParkinglotsSvr{
	//0x2000
	struct stMsg_GetHostDetailsReq{
		//ĿǰΪ��
	};

	//0x2800
	struct stMsg_GetHostDetailsRsp{
		unsigned __int8 DoneCode;//ִ�н������
		unsigned __int16 HostType;//��������
		unsigned __int16 FirmwareVersion;//���ع̼��汾��
		char HostName[64];//�������ƣ����Ȳ�����64�ֽ�
		char HostInfo[64];//�������������Ȳ�����64�ֽ�
		unsigned __int8 ConnetType;// �������������ӷ�ʽ��
		//1��GPRS��2��3G��3��WAN��4��LAN
		unsigned __int8 IEEEAdd[8];// Э����IEEE��ַ��8�ֽ�ʮ��������
		unsigned __int8 IEEEAdd_Flag;// Э����IEEE��ַʹ�ܣ�0δʹ�ܣ�1ʹ��
		unsigned __int8 PANID[2];// PANID��2�ֽ�ʮ��������
		unsigned __int8 PANID_Flag;// PANIDʹ�ܣ�0δʹ�ܣ�1ʹ��
		unsigned __int8 EPANID[8];// ��չPANID��8�ֽ�ʮ��������
		unsigned __int8 EPANID_Flag;// ��չPANIDʹ�ܣ�0δʹ�ܣ�1ʹ��
		unsigned __int16 SensorNum;// ����������
		unsigned __int16 RelayNum;// �м�����
		unsigned __int16 ANSensorNum;// �쳣����������
		unsigned __int16 ANRelayNum;// �쳣�м�����
	};

}
#pragma  pack (pop)

typedef unsigned __int32 (__stdcall * fp_st_getMACInfo)(
	const char * address, 
	unsigned __int16 port,
	unsigned __int32 macID, 
	ParkinglotsSvr::stMsg_GetHostDetailsRsp * pOutputBuf);

//This class help client app to get dll method easily
class pklts_ctrl{
private:
	HMODULE m_dllMod;
	fp_st_getMACInfo m_fn_st_getMACInfo;
public:
	inline pklts_ctrl(const _TCHAR * dllFilePath)
	{
		m_dllMod = ::LoadLibrary(dllFilePath);
		if (m_dllMod !=NULL)
		{
			m_fn_st_getMACInfo = (fp_st_getMACInfo )::GetProcAddress(m_dllMod,"st_getMACInfo");
		}
		else
		{
			m_fn_st_getMACInfo = NULL;
		}
	}

	inline ~pklts_ctrl()
	{
		if (m_dllMod)
		{
			::FreeLibrary(m_dllMod);
			m_dllMod = 0;
		}
	}

	inline bool valid()
	{
		if (m_dllMod==0) return false;
		if (m_fn_st_getMACInfo == 0) return false;
		return true;
	}

public:
	inline unsigned __int32  st_getMACInfo(const char * address, unsigned __int16 port,unsigned __int32 macID, ParkinglotsSvr::stMsg_GetHostDetailsRsp * pOutputBuf)
	{
		return m_fn_st_getMACInfo(address,port,macID,pOutputBuf);
	}

};


#endif