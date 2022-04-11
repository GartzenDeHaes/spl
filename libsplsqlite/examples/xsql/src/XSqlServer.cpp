#include "xsql/XSqlServer.h"

#include <spl/Int32.h>
#include <spl/math/Random.h>
#include <spl/net/PacketBuilder.h>

#include "xsql/XSqlMessage.h"

using namespace spl;

const char* XSqlServer::XSQLKEY_PORT = "PORT";
const char* XSqlServer::XSQLKEY_SCANMASK = "SCANMASK";
const char* XSqlServer::XSQLKEY_SCANFREQ = "SCANFREQ";
const char* XSqlServer::XSQLKEY_ACCEPTMASK = "ACCEPTMASK";
const char* XSqlServer::XSQLKEY_PASSKEY = "PASSKEY";
const char* XSqlServer::XSQLKEY_USEDISKLOG = "USEDISKLOG";
const char* XSqlServer::XSQLKEY_THREADS = "THREADS";

XSqlServer::XSqlServer()
: m_settings(), m_listener(), m_id()
{
	m_settings.Add(XSQLKEY_PORT, "8089");
	m_settings.Add(XSQLKEY_SCANMASK, "127.0.0.*");
	m_settings.Add(XSQLKEY_SCANFREQ, "60");
	m_settings.Add(XSQLKEY_ACCEPTMASK, "*.*.*.*");
	m_settings.Add(XSQLKEY_PASSKEY, Int32::ToString(Random::SNextInt()));
	m_settings.Add(XSQLKEY_USEDISKLOG, "true");
	m_settings.Add(XSQLKEY_THREADS, "64");

	m_settings.Load("xsql.ini");
}

XSqlServer::~XSqlServer()
{
	Stop();
}

void XSqlServer::Start()
{
	if (m_listener.IsNotNull())
	{
		return;
	}

	m_listener = new PooledSocketServer(this, Port(), ThreadsToUse());
}

void XSqlServer::Stop()
{
	m_listener->CloseAndDelete();
}

void XSqlServer::Join()
{
	m_listener->Join();
}

IStreamReadListenerPtr XSqlServer::Create(TcpSocketPtr sock)
{
	PacketBuilderPtr pktb(new XSqlMessage(this));
	return pktb;
}

bool XSqlServer::ISqlMessageListener_Authenticate(String& uid, String& pw)
{
	return true;
}

void XSqlServer::ISqlMessageListener_WriteLog(spl::String& msg)
{
	printf("%s\n", msg.GetChars());
}

#ifdef DEBUG
void XSqlServer::ValidateMem() const
{
	m_settings.ValidateMem();
	m_listener.ValidateMem();
}

void XSqlServer::CheckMem() const
{
	m_settings.CheckMem();
	m_listener.CheckMem();
}
#endif
