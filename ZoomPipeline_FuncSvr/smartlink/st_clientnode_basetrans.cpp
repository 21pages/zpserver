﻿#include "st_clientnode_basetrans.h"
#include "st_client_table.h"
#include <assert.h>
#include <QDebug>
namespace ParkinglotsSvr{
	st_clientNode_baseTrans::st_clientNode_baseTrans(st_client_table * pClientTable, QObject * pClientSock ,QObject *parent) :
		zp_plTaskBase(parent)
	{
		m_bUUIDRecieved = false;
		m_currentReadOffset = 0;
		m_currentMessageSize = 0;
		m_pClientSock = pClientSock;
		m_uuid = 0xffffffff;//Not Valid
		m_pClientTable = pClientTable;
		bTermSet = false;
		m_last_Report = m_last_Watching = QDateTime::currentDateTime();
		m_remotePort = 0;
		m_bWatchingDog = false;
	}
	void st_clientNode_baseTrans::setRemoteInfo(QString addr, quint16 port)
	{
		m_remotePort = port;
		m_remoteAddress = addr;
		m_peerInfo = QString("{%1:%2}").arg( m_remoteAddress).arg(m_remotePort);
	}
	QString st_clientNode_baseTrans::peerInfo()
	{
		return m_peerInfo;
	}

	quint32 st_clientNode_baseTrans::uuid()
	{
		return m_uuid;
	}
	QObject * st_clientNode_baseTrans::sock()
	{
		return m_pClientSock;
	}
	bool st_clientNode_baseTrans::uuidValid()
	{
		return m_bUUIDRecieved;
	}
	QDateTime st_clientNode_baseTrans::lastActiveTime()
	{
		return m_last_Report;
	}
	qint32 st_clientNode_baseTrans::bytesLeft()
	{
		return m_currentHeader.DataLen + sizeof(PKLTS_Trans_Header)
				-m_currentMessageSize ;
	}
	//judge whether id is valid.
	bool st_clientNode_baseTrans::bIsValidUserId(quint32 id)
	{
		return id >=(unsigned int)0x00000002 && id <=(unsigned int)0xFFFFFFFE;
	}

	//The main functional method, will run in thread pool
	int st_clientNode_baseTrans::run()
	{
		if (bTermSet==true)
		{
			qDebug()<<peerInfo()<< QString("%1(%2) Node Martked Deleted, return.").arg((quint64)this).arg(ref());
			return 0;
		}
		int nCurrSz = -1;
		int nMessage = m_nMessageBlockSize;
		while (--nMessage>=0 && nCurrSz!=0  )
		{
			QByteArray block;
			m_mutex_rawData.lock();
			if (m_list_RawData.size())
				block =  *m_list_RawData.begin();
			m_mutex_rawData.unlock();
			if (block.isEmpty()==false && block.isNull()==false)
			{
				m_currentReadOffset = filter_message(block,m_currentReadOffset);
				if (m_currentReadOffset >= block.size())
				{
					m_mutex_rawData.lock();
					if (m_list_RawData.empty()==false)
						m_list_RawData.pop_front();
					else
					{
						QString msg = peerInfo()+tr(" m_list_RawData should not be empty!");
						qFatal(msg.toStdString().c_str());
					}
					m_currentReadOffset = 0;
					m_mutex_rawData.unlock();
				}
			}
			else
			{
				m_mutex_rawData.lock();
				//pop empty cabs
				if (m_list_RawData.empty()==false)
					m_list_RawData.pop_front();
				m_mutex_rawData.unlock();
			}
			m_mutex_rawData.lock();
			nCurrSz = m_list_RawData.size();
			m_mutex_rawData.unlock();
		}
		m_mutex_rawData.lock();
		nCurrSz = m_list_RawData.size();
		m_mutex_rawData.unlock();
		if (nCurrSz==0)
			return 0;
		return -1;
	}

	//push new binary data into queue
	int st_clientNode_baseTrans::push_new_data(const  QByteArray &  dtarray)
	{
		int res = 0;
		m_mutex_rawData.lock();
		res = m_list_RawData.size();
		if (res>=16)
		{
			if (res % 10 == 0)
				qWarning()<< peerInfo() << tr("Task Blocked. %1 tasks in queue...").arg(res);
		}
		if (res <=256)
			m_list_RawData.push_back(dtarray);
		else
			qCritical()<< peerInfo() << tr("Task Blocked Too Badly. %1 tasks in queue, lost 1 package!!").arg(res);
		res = m_list_RawData.size();
		m_mutex_rawData.unlock();
		m_last_Report = QDateTime::currentDateTime();

		return res;
	}
	//!deal one message, affect m_currentRedOffset,m_currentMessageSize,m_currentHeader
	//!return bytes Used.
	int st_clientNode_baseTrans::filter_message(QByteArray  block, int offset)
	{
		const int blocklen = block.length();
		while (blocklen>offset)
		{
			const char * dataptr = block.constData();

			//Recieve First 2 byte
			while (m_currentMessageSize<2 && blocklen>offset )
			{
				m_currentBlock.push_back(dataptr[offset++]);
				m_currentMessageSize++;
			}
			if (m_currentMessageSize < 2) //First 2 byte not complete
				continue;

			if (m_currentMessageSize==2)
			{
				const char * headerptr = m_currentBlock.constData();
				memcpy((void *)&m_currentHeader,headerptr,2);
			}


			//Heart Beating
			if (m_currentHeader.Mark == 0xBEBE && m_bWatchingDog==false)
			{
				if (m_currentMessageSize< sizeof(PKLTS_Heartbeating) && blocklen>offset )
				{
					int nCpy = offset - blocklen;
					if (nCpy > sizeof(PKLTS_Heartbeating) - m_currentMessageSize)
						nCpy =  sizeof(PKLTS_Heartbeating) - m_currentMessageSize;
					m_currentBlock.push_back(QByteArray(dataptr+offset,nCpy));
					offset += nCpy;
					m_currentMessageSize+=nCpy;
				}
				if (m_currentMessageSize < sizeof(PKLTS_Heartbeating)) //Header not completed.
					continue;

				//Send back
				emit evt_SendDataToClient(this->sock(),m_currentBlock);

				//This Message is Over. Start a new one.
				m_currentMessageSize = 0;
				m_currentBlock = QByteArray();
				continue;
			}
			else if (m_currentHeader.Mark == 0x55AA && m_bWatchingDog==false)
				//Trans Message
			{
				//while (m_currentMessageSize< sizeof(PKLTS_TRANS_HEADER) && blocklen>offset)
				//{
				//	m_currentBlock.push_back(dataptr[offset++]);
				//	m_currentMessageSize++;
				//}
				if (m_currentMessageSize< sizeof(PKLTS_Trans_Header) && blocklen>offset)
				{
					int nCpy =  blocklen - offset;
					if (nCpy > sizeof(PKLTS_Trans_Header) - m_currentMessageSize)
						nCpy =  sizeof(PKLTS_Trans_Header) - m_currentMessageSize;
					m_currentBlock.push_back(QByteArray(dataptr+offset,nCpy));
					offset += nCpy;
					m_currentMessageSize+=nCpy;
				}
				if (m_currentMessageSize < sizeof(PKLTS_Trans_Header)) //Header not completed.
					continue;
				else if (m_currentMessageSize == sizeof(PKLTS_Trans_Header))//Header just  completed.
				{
					const char * headerptr = m_currentBlock.constData();
					memcpy((void *)&m_currentHeader,headerptr,sizeof(PKLTS_Trans_Header));

					//continue reading if there is data left behind
					if (block.length()>offset)
					{
						qint32 bitLeft = m_currentHeader.DataLen + sizeof(PKLTS_Trans_Header)
								-m_currentMessageSize ;
						//while (bitLeft>0 && blocklen>offset)
						//{
						//	m_currentBlock.push_back(dataptr[offset++]);
						//	m_currentMessageSize++;
						//	bitLeft--;
						//}
						if (bitLeft>0 && blocklen>offset)
						{
							int nCpy =  blocklen - offset;
							if (nCpy > bitLeft)
								nCpy =  bitLeft;
							m_currentBlock.push_back(QByteArray(dataptr+offset,nCpy));
							offset += nCpy;
							m_currentMessageSize+=nCpy;
							bitLeft -= nCpy;
						}
						//deal block, may be send data as soon as possible;
						deal_current_message_block();
						if (bitLeft>0)
							continue;
						//This Message is Over. Start a new one.
						m_currentMessageSize = 0;
						m_currentBlock = QByteArray();
						continue;
					}
				}
				else
				{
					if (block.length()>offset)
					{
						qint32 bitLeft = m_currentHeader.DataLen + sizeof(PKLTS_Trans_Header)
								-m_currentMessageSize ;
						//while (bitLeft>0 && blocklen>offset)
						//{
						//	m_currentBlock.push_back(dataptr[offset++]);
						//	m_currentMessageSize++;
						//	bitLeft--;
						//}
						if (bitLeft>0 && blocklen>offset)
						{
							int nCpy =  blocklen - offset;
							if (nCpy > bitLeft)
								nCpy =  bitLeft;
							m_currentBlock.push_back(QByteArray(dataptr+offset,nCpy));
							offset += nCpy;
							m_currentMessageSize+=nCpy;
							bitLeft -= nCpy;
						}
						//deal block, may be processed as soon as possible;
						deal_current_message_block();
						if (bitLeft>0)
							continue;
						//This Message is Over. Start a new one.
						m_currentMessageSize = 0;
						m_currentBlock = QByteArray();
						continue;
					}
				} // end if there is more bytes to append
			} //end deal trans message
			//The client want server close its connection from remote.
			else if (m_currentHeader.Mark == 0xDFDF)
			{
				m_bWatchingDog = true;
				if (m_currentMessageSize< sizeof(PKLTS_Watchdog) && blocklen>offset )
				{
					int nCpy = offset - blocklen;
					if (nCpy > sizeof(PKLTS_Watchdog) - m_currentMessageSize)
						nCpy =  sizeof(PKLTS_Watchdog) - m_currentMessageSize;
					m_currentBlock.push_back(QByteArray(dataptr+offset,nCpy));
					offset += nCpy;
					m_currentMessageSize+=nCpy;
				}
				if (m_currentMessageSize < sizeof(PKLTS_Watchdog)) //Header not completed.
					continue;

				//Send back
				emit evt_SendDataToClient(this->sock(),m_currentBlock);
				//Check macid
				const PKLTS_Watchdog * dog = (PKLTS_Watchdog *)m_currentBlock.constData();
				quint32 macid = dog->macid;
				qDebug()<<tr("Client Send Watching Dog 0xDFDF->")<<macid;

				//This Message is Over. Start a new one.
				m_currentMessageSize = 0;
				m_currentBlock = QByteArray();
				dog = NULL;
				//Check whether macid is still working, ignore 0 (debug server)
				if (macid!=0)
				{
					if (m_pClientTable->clientNodeFromUUID(macid))
					{
						m_last_Watching = QDateTime::currentDateTime();
						qDebug()<<tr("Watching Dog Checker macid ok:")<<macid;
						continue;
					}
					if (m_pClientTable->cross_svr_find_uuid(macid).length())
					{
						m_last_Watching = QDateTime::currentDateTime();
						qDebug()<<tr("Watching Dog Checker cross svr  macid ok:")<<macid;
						continue;
					}
					if (m_last_Watching.secsTo(QDateTime::currentDateTime())>=3600 * 2)
					{
						QByteArray arrayClean;
						arrayClean.append(0xca);
						arrayClean.append(0xca);
						qWarning()<<tr("Watching Dog Checker cross svr  macid failed, force reboot remote mac:")<<macid;
						emit evt_SendDataToClient(this->sock(),arrayClean);
						m_last_Watching = QDateTime::currentDateTime();
					}
					else
						qWarning()<<tr("Watching Dog Checker last goot time is:")<<m_last_Watching;
				}
				else
					qWarning()<<tr("Watching Dog Checker gives a checkid = 0");
				continue;
			}
			else if (m_currentHeader.Mark == 0x0000)
			{
				qDebug()<<tr("Client Send Immediatlly Disconnection Cmd Header 0x0000.");
				m_currentMessageSize = 0;
				m_currentBlock = QByteArray();
				offset = blocklen;
				emit evt_close_client(this->sock());
			}
			else if (m_currentHeader.Mark == 0x5959)
			{
				qDebug()<<tr("Client Send Responce Cmd Header 0x5959.");
				m_currentMessageSize = 0;
				m_currentBlock = QByteArray();
			}
			else
			{
				const char * ptrCurrData =  m_currentBlock.constData();
				emit evt_Message(this,tr("Client Send a unknown start Header %1 %2. Close client immediately.")
								 .arg((int)(ptrCurrData[0])).arg((int)(ptrCurrData[1])));
				qWarning()<<"Client "<<peerInfo()<<"Send BAD Data :"<<m_currentBlock.left(256).toHex().data();
				qWarning()<<"Close client Immediatly."<<peerInfo();
				m_currentMessageSize = 0;
				m_currentBlock = QByteArray();
				offset = blocklen;
				emit evt_close_client(this->sock());
			}
		} // end while block len > offset

		return offset;
	}
	//in Trans-Level, do nothing.
	int st_clientNode_baseTrans::deal_current_message_block()
	{
		int nRes = 0;
		//First, get uuid as soon as possible
		if (m_bUUIDRecieved==false)
		{
			if (bIsValidUserId( m_currentHeader.SrcID) )
			{
				m_bUUIDRecieved = true;
				m_uuid =  m_currentHeader.SrcID;
				//regisit client node to hash-table;
				m_pClientTable->regisitClientUUID(this);
			}
			else if (m_currentHeader.SrcID==0xffffffff)
			{
				//New clients
			}
			else //Invalid
			{
				qWarning()<<peerInfo()<<tr("Client ID %1 is invalid! Close client immediatly.").arg(m_currentHeader.SrcID);
				emit evt_Message(this,tr("Client ID is invalid! Close client immediatly."));
				m_currentBlock = QByteArray();
				emit evt_close_client(this->sock());
				nRes = 1;
			}
		}
		else
		{
			if (!( bIsValidUserId(m_currentHeader.SrcID)
				  ||
				  (m_currentHeader.SrcID==0xffffffff)
				  )
					)
			{
				qWarning()<<peerInfo()<<tr("Client ID %1 is invalid! Close client immediatly.").arg(m_currentHeader.SrcID);
				emit evt_Message(this,tr("Client ID is invalid! Close client immediatly."));
				m_currentBlock = QByteArray();
				emit evt_close_client(this->sock());
				nRes = 1;
			}
			if (bIsValidUserId(m_currentHeader.SrcID)==true &&
					m_uuid != m_currentHeader.SrcID)
			{
				qWarning()<<peerInfo()<<tr("Client ID Changed in Runtime! Close client immediatly, %1->%2.").arg(m_uuid).arg(m_currentHeader.SrcID);
				emit evt_Message(this,tr("Client ID Changed in Runtime! Close client immediatly, %1->%2.").arg(m_uuid).arg(m_currentHeader.SrcID));
				m_currentBlock = QByteArray();
				emit evt_close_client(this->sock());
				nRes = 1;
			}


		}

		return nRes;
	}
	void st_clientNode_baseTrans::CheckHeartBeating()
	{
		QDateTime dtm = QDateTime::currentDateTime();
		qint64 usc = this->m_last_Report.secsTo(dtm);
		if (usc >=m_pClientTable->heartBeatingThrd())
		{
			qWarning()<<peerInfo()<<tr("Client ") + QString("%1").arg((unsigned int)((quint64)this)) + tr(" is dead, kick out.");
			emit evt_Message(this,tr("Client ") + QString("%1").arg((unsigned int)((quint64)this)) + tr(" is dead, kick out."));
			emit evt_close_client(this->sock());
		}
	}
}
