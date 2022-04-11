#include "xsql/XSqlMessage.h"

XSqlMessage::XSqlMessage(IXSqlMessageListener *listener)
: m_listener(listener)
{
	Delegates().Add(this);
}

XSqlMessage::~XSqlMessage()
{
}

void XSqlMessage::IPacket_OnData(int64 i)
{
}

void XSqlMessage::IPacket_OnData(int32 i)
{
}

void XSqlMessage::IPacket_OnData(int16 i)
{
}

void XSqlMessage::IPacket_OnData(byte i)
{
}

void XSqlMessage::IPacket_OnData(bool b)
{
}

void XSqlMessage::IPacket_OnData(double d)
{
}

void XSqlMessage::IPacket_OnData(StringPtr str)
{
}

void XSqlMessage::IPacket_OnData(const Vector<byte>& data, const int len)
{
}

void XSqlMessage::IPacket_OnData(const DateTime& dtm)
{
}

void XSqlMessage::IPacket_OnData(const Date& i)
{
}

void XSqlMessage::IStreamRead_OnError(const String& msg)
{
}

void XSqlMessage::IStreamRead_OnClose()
{
}
