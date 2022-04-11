#ifndef _xsqlmessage_h
#define _xsqlmessage_h

#include <spl/net/PacketBuilder.h>

#include "XSqlMessageListener.h"

using namespace spl;

class XSqlMessage;
typedef spl::RefCountPtr<XSqlMessage> XSqlMessagePtr;

class XSqlMessage : public spl::PacketBuilder, public spl::IPacketListener
{
private:
	virtual void IPacket_OnData(int64 i);
	virtual void IPacket_OnData(int32 i);
	virtual void IPacket_OnData(int16 i);
	virtual void IPacket_OnData(byte i);
	virtual void IPacket_OnData(bool b);
	virtual void IPacket_OnData(double d);
	virtual void IPacket_OnData(StringPtr str);
	virtual void IPacket_OnData(const Vector<byte>& data, const int len);
	virtual void IPacket_OnData(const DateTime& dtm);
	virtual void IPacket_OnData(const Date& i);

	virtual void IStreamRead_OnError(const String& msg);
	virtual void IStreamRead_OnClose();
	
	inline XSqlMessage(XSqlMessage& msg) {}
	
	IXSqlMessageListener *m_listener;
	
public:
	XSqlMessage(IXSqlMessageListener *listener);
	
	virtual ~XSqlMessage();
};

#endif
