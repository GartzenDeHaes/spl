/*
 *   This file is part of the Standard Portable Library (SPL).
 *
 *   SPL is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   SPL is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with SPL.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <spl/Int32.h>
#include <spl/web/HttpUtility.h>
#include <spl/text/StringBuffer.h>
#include <spl/web/Uri.h>

using namespace spl;

Uri::Uri(  )
:	m_protocol("http"), 
	m_server(""), 
	m_port(80), 
	m_path(""), 
	m_filename(""), 
	m_fileext(""), 
	m_args(),
	m_argIdx()
{
}

Uri::Uri( const String &cpuri )
:	m_protocol("http"), 
	m_server(""), 
	m_port(80), 
	m_path(""), 
	m_filename(""), 
	m_fileext(""), 
	m_args(),
	m_argIdx()
{
	Parse( cpuri );
}

Uri::Uri( const Uri& uri )
:	m_protocol(uri.m_protocol), 
	m_server(uri.m_server), 
	m_port(uri.m_port), 
	m_path(uri.m_path), 
	m_filename(uri.m_filename), 
	m_fileext(uri.m_fileext), 
	m_args(uri.m_args),
	m_argIdx(uri.m_argIdx)
{
}

Uri::~Uri()
{
}

Uri& Uri::operator =(const Uri& uri)
{
	m_protocol = uri.m_protocol;
	m_server = uri.m_server;
	m_port = uri.m_port;
	m_path = uri.m_path;
	m_filename = uri.m_filename;
	m_fileext = uri.m_fileext;
	m_args = uri.m_args;
	m_argIdx = uri.m_argIdx;

	return *this;
}

void Uri::Parse( const String& cstr )
{
	/// @TODO -- This parse needs work

	StringPtr str = String(cstr).Replace('\\', '/');

	int pos = 0;
	int idx = str->IndexOf(':');
	if ( idx > -1 )
	{
		// protocol
		StringPtr proto = str->Substring(pos, idx);
		proto->ToLower();
		m_protocol.Set(*proto);
		pos = idx + 1;

		if ( pos >= str->Length() )
		{
			return;
		}
		if ( str->CharAt(pos++) != '/' )
		{
			throw new InvalidArgumentException("Expected ://");
		}
		if ( pos >= str->Length() )
		{
			return;
		}
		if ( str->CharAt(pos++) != '/' )
		{
			throw new InvalidArgumentException("Expected ://");
		}
		if ( pos >= str->Length() )
		{
			return;
		}

		// server dns name
		StringPtr temp;

		if ( (idx = str->IndexOf(':', pos + 1)) < 0 )
		{
			idx = str->IndexOf('/', pos + 1);
		}
		if ( 0 > idx )
		{
			temp = str->Substring(pos);
			pos += temp->Length();
		}
		else
		{
			temp = str->Mid(pos, idx);
			pos = idx;
		}
		m_server.Set( *temp );

		if ( pos >= str->Length() )
		{
			return;
		}

		if ( str->CharAt(pos) == ':' )
		{
			pos++;
			idx = str->IndexOf('/', pos + 1);
			temp = str->Mid(pos, idx);
			pos += temp->Length();
			ASSERT( Int32::IsInt(*temp) );
			m_port = Int32::Parse(*temp);
		}
	}
	if ( pos >= str->Length() )
	{
		return;
	}
	StringPtr fullpath;
	if ( 0 > (idx = str->IndexOf('?', pos)) )
	{
		// no params
		fullpath = str->Substring(pos);
		pos = str->Length();
	}
	else
	{
		// params
		fullpath = str->Mid(pos, idx);
		pos = idx;
	}
	if ( 0 > (idx = fullpath->IndexOf('/')) )
	{
		m_filename.Set( *fullpath );
	}
	else if ( fullpath->IndexOf('.') < 0 )
	{
		// there is no file name
		m_path.Set( *fullpath );
	}
	else
	{
		idx = fullpath->LastIndexOf("/");
		ASSERT(idx > -1);
		m_filename.Set( *fullpath->Substring(idx+1) );
		m_path.Set( *fullpath->Substring(0, idx+1) );
	}

	if ( (idx = m_filename.IndexOf('.')) > -1 )
	{
		m_fileext.Set( &m_filename.GetChars()[idx+1] );
	}
	if ( pos >= str->Length() )
	{
		return;
	}
	ASSERT( str->CharAt(pos) == '?' );
	StringPtr params = str->Substring(pos+1);
	RefCountPtr<Vector<StringPtr> > parts = params->Split("&");

	for ( int x = 0; x < parts->Count(); x++ )
	{
		StringPtr part = parts->ElementAt(x);
		if ( 0 > (idx = part->IndexOf('=')) )
		{
			Association<String, String> pair(*HttpUtility::UrlDecode(*part), String(""));
			m_args.Add( pair );
		}
		else
		{
			StringPtr key = part->Substring(0, idx);
			StringPtr val = part->Substring(idx+1);
			
			Association<String, String> pair(*HttpUtility::UrlDecode(*key), *HttpUtility::UrlDecode(*val));
			m_args.Add( pair );
			m_argIdx.Set( pair.KeyRef(), pair.ValueRef() );
		}
	}
}

StringPtr Uri::AbsolutePath() const
{
	StringBuffer buf;

	if ( ! m_path.StartsWith("/") )
	{
		buf.Append('/');
	}
	buf.Append( m_path );
	if ( ! m_path.EndsWith("/") )
	{
		buf.Append('/');
	}
	buf.Append( m_filename );

	int argcount = m_args.Count();
	if ( argcount > 0 )
	{
		buf.Append('?');
	}

	for ( int x = 0; x < argcount; x++ )
	{
		Association<String, String>& pair = m_args.ElementAtRef(x);
		if ( HttpUtility::UrlEncodeRequired(pair.KeyRef()) )
		{
			buf.Append( HttpUtility::UrlEncode(pair.KeyRef()) );
		}
		else
		{
			buf.Append( pair.KeyRef() );
		}
		buf.Append( "=" );
		if ( HttpUtility::UrlEncodeRequired(pair.ValueRef()) )
		{
			buf.Append( HttpUtility::UrlEncode(pair.ValueRef()) );
		}
		else
		{
			buf.Append( pair.ValueRef() );
		}
		if ( x + 1 < argcount )
		{
			buf.Append( "&" );
		}
	}

	return buf.ToString();
}

StringPtr Uri::ToString() const
{
	StringBuffer buf;
	
	if ( m_server.Length() > 0 )
	{
		buf.Append( m_protocol );
		buf.Append( "://" );
		buf.Append( m_server );
	}

	if ( m_port != 80 )
	{
		buf.Append(':');
		buf.Append(Int32::ToString(m_port));
	}

	buf.Append( AbsolutePath() );

	return buf.ToString();
}

uint32 Uri::HashCode() const
{
	return ToString()->HashCode();
}

#ifdef DEBUG
void Uri::ValidateMem() const
{
	m_protocol.ValidateMem();
	m_server.ValidateMem();
	m_path.ValidateMem();
	m_filename.ValidateMem();
	m_fileext.ValidateMem();
	m_args.ValidateMem();
	m_argIdx.ValidateMem();
}

void Uri::CheckMem() const
{
	m_protocol.CheckMem();
	m_server.CheckMem();
	m_path.CheckMem();
	m_filename.CheckMem();
	m_fileext.CheckMem();
	m_args.CheckMem();
	m_argIdx.CheckMem();
}
#endif
