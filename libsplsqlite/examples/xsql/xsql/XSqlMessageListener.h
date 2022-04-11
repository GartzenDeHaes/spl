#ifndef _xsqlmessagelistener_h
#define _xsqlmessagelistener_h

#include <spl/String.h>

class IXSqlMessageListener
{
public:
	IXSqlMessageListener();
	virtual ~IXSqlMessageListener();
	
	virtual bool ISqlMessageListener_Authenticate(spl::String& uid, spl::String& pw) = 0;
	virtual void ISqlMessageListener_WriteLog(spl::String& msg) = 0;
};

#endif
