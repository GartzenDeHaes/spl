#ifndef _xsqlserver_h
#define _xsqlserver_h

#include <spl/configuration/ConfigurationSettings.h>
#include <spl/Int32.h>
#include <spl/GUID.h>
#include <spl/net/PooledSocketSet.h>
#include <spl/threading/ThreadStartDelegate.h>

#include "XSqlMessageListener.h"

class XSqlServer;
typedef spl::RefCountPtr<XSqlServer> XSqlServerPtr;

class XSqlServer : public spl::IServerConnectionFactory, public IXSqlMessageListener
{
private:
	static const char* XSQLKEY_PORT;
	static const char* XSQLKEY_SCANMASK;
	static const char* XSQLKEY_SCANFREQ;
	static const char* XSQLKEY_ACCEPTMASK;
	static const char* XSQLKEY_PASSKEY;
	static const char* XSQLKEY_USEDISKLOG;
	static const char* XSQLKEY_THREADS;

	inline XSqlServer(XSqlServer& srv) {}

	spl::Guid m_id;

	spl::ConfigurationSettings m_settings;
	spl::PooledSocketServerPtr m_listener;

	virtual spl::IStreamReadListenerPtr Create(spl::TcpSocketPtr sock);
	virtual bool ISqlMessageListener_Authenticate(spl::String& uid, spl::String& pw);
	virtual void ISqlMessageListener_WriteLog(spl::String& msg);

public:
	XSqlServer();
	virtual ~XSqlServer();

	inline spl::Guid& ID()
	{
		return m_id;
	}

	inline int Port()
	{
		return spl::Int32::Parse(*m_settings.Value(XSQLKEY_PORT));
	}

	inline int ThreadsToUse()
	{
		return spl::Int32::Parse(*m_settings.Value(XSQLKEY_THREADS));
	}

	void Start();
	void Stop();
	void Join();

#ifdef DEBUG
	void ValidateMem() const;
	void CheckMem() const;
#endif
};

#endif
