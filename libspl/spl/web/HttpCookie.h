#ifndef _httpcookie_h
#define _httpcookie_h

#include <spl/collection/Hashtable.h>
#include <spl/RefCountPtr.h>
#include <spl/String.h>

namespace spl
{
class HttpCookie;
typedef RefCountPtr<HttpCookie> HttpCookiePtr;

class HttpCookie : public IMemoryValidate
{
private:
	String m_key;
	String m_value;
	Hashtable<String, String> m_attribs;

public:
	HttpCookie();
	HttpCookie(const String& key, const String& value, const String& path = "/");
	HttpCookie(const HttpCookie& cookie);
	virtual ~HttpCookie();
	
	HttpCookie& operator =(const HttpCookie& cookie);
	
	inline String& Key() { return m_key; }
	inline String& Value() { return m_value; }
	inline Hashtable<String, String>& Attributes() { return m_attribs; }
	
	StringPtr ToString();
	
	static HttpCookiePtr Parse(const String& hdr);
	
#ifdef DEBUG
	virtual void ValidateMem() const;
	virtual void CheckMem() const;
#endif
};
}
#endif
