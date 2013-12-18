#ifndef ZP_NETTRANSTHREAD_H
#define ZP_NETTRANSTHREAD_H

#include <QObject>
#include <QMap>
#include <QList>
#include <QAbstractSocket>
#include <QMutex>
class zp_netTransThread : public QObject
{
    Q_OBJECT
public:
    explicit zp_netTransThread(int nPayLoad = 4096,QObject *parent = 0);

    QList <QObject *> clientsList();
    int CurrentClients();
    void SetPayload(int nPayload);
    bool isActive(){return m_bActivated;}

    bool CanExit();
    bool SSLConnection(){return m_bSSLConnection ;}
    void SetSSLConnection(bool bssl){ m_bSSLConnection = bssl;}

private:
    bool m_bActivated;
    bool m_bSSLConnection;
    QMap<QObject *,QList<QByteArray> > m_buffer_sending;
    QMap<QObject *,QList<qint64> > m_buffer_sending_offset;
    QMap<QObject*,int> m_clientList;
    int m_nPayLoad;
    QMutex m_mutex_protect;
public slots:
    //�µĿͻ����ӵ���
    void incomingConnection(QObject * threadid,qintptr socketDescriptor);
    //��ͻ��˷�������
    void SendDataToClient(QObject * objClient,const QByteArray &  dtarray);
    //��ͻ��˹㲥���ݣ������� objFromClient
    void BroadcastData(QObject * objFromClient,const QByteArray &  dtarray);
    //�ñ��̲߳��ٽ����µ�����
    void Deactivate(){m_bActivated = true;}
    //������ֹ���̵߳���������
    void DeactivateImmediately(zp_netTransThread *);
    //�߳����пͻ�
    void KickAllClients(zp_netTransThread *);
protected slots:
    //�ͻ����ӱ��ر�
    void client_closed();
    //�µ����ݵ���
    void new_data_recieved();
    //һ�����ݷ������
    void some_data_sended(qint64);
    //�ͻ��˴���
    void displayError(QAbstractSocket::SocketError socketError);
    //SSL���ܿ�ʼ
    void on_encrypted();
signals:
    //������Ϣ
    void evt_SocketError(QObject * senderSock ,QAbstractSocket::SocketError socketError);
    //�µĿͻ�������
    void evt_NewClientConnected(QObject * client);
    //�ͻ����˳�
    void evt_ClientDisconnected(QObject * client);
    //�յ�һ������
    void evt_Data_recieved(QObject * ,const QByteArray &  );
    //һ�����ݱ�����
    void evt_Data_transferred(QObject * client,qint64);
};

#endif // ZP_NETTRANSTHREAD_H
