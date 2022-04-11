#include <spl/web/HttpCookie.h>
#include <spl/web/HttpUtility.h>
#include <spl/text/StringBuffer.h>

using namespace spl;

HttpCookie::HttpCookie()
: m_key(), m_value(), m_attribs()
{
}

HttpCookie::HttpCookie(const String& key, const String& value, const String& path)
: m_key(key), m_value(value), m_attribs()
{
	m_attribs.Set("path", path);
}

HttpCookie::HttpCookie(const HttpCookie& cookie)
: m_key(cookie.m_key), m_value(cookie.m_value), m_attribs(cookie.m_attribs)
{
}

HttpCookie::~HttpCookie()
{
}

HttpCookie& HttpCookie::operator =(const HttpCookie& cookie)
{
	m_key = cookie.m_key;
	m_value = cookie.m_value;
	m_attribs = cookie.m_attribs;
	
	return *this;
}

StringPtr HttpCookie::ToString()
{
	StringBuffer buf;
	
	buf.Append(HttpUtility::HtmlEncode(m_key));
	buf.Append('=');
	buf.Append(HttpUtility::HtmlEncode(m_value));
	
	if (m_attribs.Count() > 0)
	{
		buf.Append("; ");
		
		for(Hashtable<String, String>::Iterator iter(m_attribs.Begin()); iter.Next(); )
		{
			buf.Append(HttpUtility::HtmlEncode(iter.CurrentKeyRef()));
			buf.Append('=');
			buf.Append(HttpUtility::HtmlEncode(iter.CurrentRef()));
			buf.Append(';');
		}
	}
	
	return buf.ToString();
}

///@BUG Doesn't handle multiple cookies.
HttpCookiePtr HttpCookie::Parse(const String& hdr)
{
	ASSERT(! hdr.HasNonPrintableCharacters());
	
	RefCountPtr<Vector<StringPtr> > parts(hdr.Split(";"));
	
	if (parts->Count() == 0)
	{
		return HttpCookiePtr(); 
	}
	
	int pos;
	StringPtr key;
	StringPtr value;
	if (0 > (pos = parts->ElementAtRef(0)->IndexOf('=')))
	{
		key = parts->ElementAtRef(0)->Trim();
		value = new String();
	}
	else
	{
		key = parts->ElementAtRef(0)->Substring(0, pos);
		value = parts->ElementAtRef(0)->Substring(pos + 1);
	}
	
	HttpCookiePtr cookie(new HttpCookie(HttpUtility::HtmlDecode(key), HttpUtility::HtmlDecode(value)));
	
	for ( int x = 1; x < parts->Count(); x++ )
	{
		if (0 > (pos = parts->ElementAtRef(x)->IndexOf('=')))
		{
			key = parts->ElementAtRef(x)->Trim();
			value = new String();
		}
		else
		{
			key = parts->ElementAtRef(x)->Substring(0, pos);
			value = parts->ElementAtRef(x)->Substring(pos + 1);
		}
		cookie->Attributes().Set(HttpUtility::HtmlDecode(key), HttpUtility::HtmlDecode(value));
	}
	
	return cookie;
}

#ifdef DEBUG
void HttpCookie::ValidateMem() const
{
	m_key.ValidateMem();
	m_value.ValidateMem();
	m_attribs.ValidateMem();
}

void HttpCookie::CheckMem() const
{
	m_key.CheckMem();
	m_value.CheckMem();
	m_attribs.CheckMem();
}
#endif
