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
/*using System;
using System.Collections;
using System.Diagnostics;
using System.Text;

	/// <summary>
	/// Definition of a fixed width field
	/// </summary>
	public class FixedFieldDef
	{
		/// <summary>
		/// Define a field with a repeating fill character
		/// </summary>
		/// <param name="name">Field Name (Unique in record)</param>
		/// <param name="len">Length in characters</param>
		/// <param name="recposition">Character position in record</param>
		/// <param name="ordnal">Field number (starting at zero)</param>
		/// <param name="fill">Default fill character</param>
		public FixedFieldDef( string name, int len, int recposition, int ordnal, char fill )
		{
			m_name = name;
			m_len = len;
			m_recpos = recposition;
			m_fieldpos = ordnal;
			m_templateData = new string( fill, m_len );
		}

		/// <summary>
		/// Define a field with default data
		/// </summary>
		/// <param name="name">Field Name (Unique in record)</param>
		/// <param name="len">Length in characters</param>
		/// <param name="recposition">Character position in record</param>
		/// <param name="ordnal">Field number (starting at zero)</param>
		/// <param name="defaultData">Default field data (lenght must equal field length)</param>
		public FixedFieldDef( string name, int len, int recposition, int ordnal, string defaultData )
		{
			Debug.Assert( defaultData.Length == len );
			if ( defaultData.Length != len )
			{
				throw new Exception(name + " incorrect length" );
			}
			m_name = name;
			m_len = len;
			m_recpos = recposition;
			m_fieldpos = ordnal;
			m_templateData = defaultData;
		}

		/// <summary>
		/// Size in chars
		/// </summary>
		public int Length
		{
			get
			{
				return m_len;
			}
		}

		/// <summary>
		/// Record unique field name
		/// </summary>
		public string Name
		{
			get
			{
				return m_name;
			}
		}

		/// <summary>
		/// Default field data
		/// </summary>
		public string DefaultData
		{
			get
			{
				return m_templateData;
			}
		}

		/// <summary>
		/// Zero based field position
		/// </summary>
		public int OrdinalPosition
		{
			get
			{
				return m_fieldpos;
			}
		}

		/// <summary>
		/// Extract the data for this field.
		/// </summary>
		/// <param name="record">A single flat record line</param>
		/// <returns></returns>
		public string Parse( string record )
		{
			Debug.Assert( m_recpos + m_len <= record.Length, m_name + " record too short" );
			return record.Substring(m_recpos, m_len);
		}

		protected string m_name;
		protected int m_len;
		protected int m_recpos;
		/// <summary>Field ordinal position</summary>
		protected int m_fieldpos;
		protected string m_templateData;
	}

	/// <summary>
	/// Definition for a fixed width file record.  (See FlatRecordFactory)
	/// </summary>
	public class FixedRecordDef
	{
		/// <summary>
		/// Define a record
		/// </summary>
		/// <param name="linelen">Length of the record in chars</param>
		public FixedRecordDef( int linelen )
		{
			m_linelen = linelen;
		}

		/// <summary>
		/// Add a field to the record
		/// </summary>
		/// <param name="name">Unique field name</param>
		/// <param name="len">Length in chars</param>
		/// <param name="defData">Default data (length must equal field length</param>
		public void AddField( string name, int len, string defData )
		{
			FixedFieldDef fld = new FixedFieldDef( name, len, m_recordEnd, m_fields.Count, defData );
			AddField( fld );
		}

		/// <summary>
		/// Add a field to the record
		/// </summary>
		/// <param name="name">Unique field name</param>
		/// <param name="len">Length in chars</param>
		/// <param name="fill">Fill default field data</param>
		public void AddField( string name, int len, char fill )
		{
			FixedFieldDef fld = new FixedFieldDef( name, len, m_recordEnd, m_fields.Count, fill );
			AddField( fld );
		}

		/// <summary>
		/// Add the created field
		/// </summary>
		/// <param name="fld"></param>
		private void AddField( FixedFieldDef fld )
		{
			m_recordEnd += fld.Length;
			Debug.Assert( m_recordEnd <= m_linelen, "Field exceeds max record size" );

			m_fields.Add( fld );
			m_fieldIndex.Add( fld.Name, fld );
		}

		/// <summary>
		/// Parse a record from a text file
		/// </summary>
		/// <param name="record">A line from the fixed width file</param>
		/// <returns>FixedRecord</returns>
		public FixedRecord Parse( string record )
		{
			FixedFieldData[] data = new FixedFieldData[ m_fields.Count ];

			for ( int x = 0; x < m_fields.Count; x++ )
			{
				data[x] = new FixedFieldData( (FixedFieldDef)m_fields[x], record );
			}
			return new FixedRecord( this, data );
		}

		/// <summary>
		/// Creates an empty data record
		/// </summary>
		/// <returns>FixedRecord</returns>
		public FixedRecord Create( )
		{
			FixedFieldData[] data = new FixedFieldData[ m_fields.Count ];

			for ( int x = 0; x < m_fields.Count; x++ )
			{
				data[x] = new FixedFieldData( (FixedFieldDef)m_fields[x] );
			}
			return new FixedRecord( this, data );
		}

		/// <summary>
		/// Get a record from the record catch
		/// </summary>
		/// <returns>FixedRecord</returns>
		public FixedRecord CreateCached()
		{
			lock ( m_cache )
			{
				if ( m_cache.Count == 0 )
				{
					return Create();
				}
				FixedRecord rcd = (FixedRecord)m_cache[ m_cache.Count-1 ];
				m_cache.RemoveAt( m_cache.Count - 1 );
				return rcd;
			}
		}

		/// <summary>
		/// Put a record back into the cache
		/// </summary>
		/// <param name="rcd"></param>
		public void ReleaseCached( FixedRecord rcd )
		{
			lock( m_cache )
			{
				rcd.Reset();
				m_cache.Add( rcd );
			}
		}

		/// <summary>
		/// Get the field position in the file
		/// </summary>
		/// <param name="name">Field name</param>
		/// <returns>field ordinal position</returns>
		public int GetFieldOrdinal( string name )
		{
			return ((FixedFieldDef)m_fieldIndex[ name ]).OrdinalPosition;
		}

		protected Hashtable m_fieldIndex = new Hashtable();
		protected ArrayList m_fields = new ArrayList();
		protected int m_linelen;
		protected int m_recordEnd;

		protected ArrayList m_cache = new ArrayList();
	}

	/// <summary>
	/// Field data for a record
	/// </summary>
	public class FixedFieldData
	{
		/// <summary>
		/// Initialize the field data from the field definition
		/// </summary>
		/// <param name="field"></param>
		public FixedFieldData( FixedFieldDef field )
		{
			m_field = field;
			m_data = field.DefaultData;
		}

		/// <summary>
		/// Initialize the field data from a record
		/// </summary>
		/// <param name="field"></param>
		/// <param name="record"></param>
		public FixedFieldData( FixedFieldDef field, string record )
		{
			m_field = field;
			m_data = field.Parse( record );
		}

		/// <summary>
		/// The field's data
		/// </summary>
		public string Value
		{
			get
			{
				return m_data;
			}
		}

		/// <summary>
		/// Parse the field data as a Date (MMDDYY)
		/// </summary>
		/// <returns>Date</returns>
		public Date ParseDate()
		{
			Debug.Assert( m_field.Length == 6 );
			string mm = m_data.Substring(0, 2);
			string dd = m_data.Substring(2, 2);
			string yy = m_data.Substring(4, 2);

			int iyy = Int32.Parse( yy );
			if ( iyy > 50 )
			{
				iyy += 1900;
			}
			else
			{
				iyy += 2000;
			}
			return new Date( iyy, Int32.Parse(mm), Int32.Parse(dd) );
		}

		/// <summary>
		/// Parse the field's data as an integer
		/// </summary>
		/// <returns>int</returns>
		public int ParseInt()
		{
			string i = StripZeros(m_data).Trim();
			if ( i.Length == 0 )
			{
				i = "0";
			}
			return Int32.Parse( i );
		}

		/// <summary>
		/// Parse the field's data as money (+00000000000000;-00000000000000)
		/// </summary>
		/// <returns>money</returns>
		public decimal ParseMoney() 
		{
			char sign = m_data[0];
			Debug.Assert( sign == '+' || sign == '-', "Invalid money format " + m_data );
			decimal amt = Decimal.Parse( m_data.Substring( 1 ) ) / 100;
			if ( sign == '-' )
			{
				return -amt;
			}
			return amt;
		}

		/// <summary>
		/// Parse a largish integer
		/// </summary>
		/// <returns>decimal</returns>
		public decimal ParseDecimal()
		{
			return Decimal.Parse( StripZeros(m_data) );
		}

		/// <summary>
		/// Parse a string with the format of "YYYY-MM-DD:HH:NN:SS.000000"
		/// </summary>
		/// <returns>DateTime</returns>
		public DateTime ParseDateTime()
		{
			int yyyy = Int32.Parse( m_data.Substring(0, 4) );
			int mm = Int32.Parse( m_data.Substring(5, 2) );
			int dd = Int32.Parse( m_data.Substring(8, 2) );

			string timepart = m_data.Substring(11);
			string[] tps = timepart.Split(new char[] {':'});
			return new DateTime(yyyy, mm, dd, Int32.Parse(tps[0]), Int32.Parse(tps[1]), (int)Double.Parse(tps[2]));
		}

		/// <summary>
		/// Return a four digit year
		/// </summary>
		/// <returns>int</returns>
		public int ParseYear()
		{
			Debug.Assert( m_field.Length == 2 || m_field.Length == 4 );
			if ( m_field.Length == 4 )
			{
				return ParseInt();
			}
			int yy = Int32.Parse( m_data );
			if ( yy > 50 )
			{
				return yy + 1900;
			}
			return yy + 2000;
		}

		/// <summary>
		/// Set a short date format (MMDDYY)
		/// </summary>
		/// <param name="dt">date</param>
		public void SetDate( Date dt )
		{
			Debug.Assert( m_field.Length == 6 );
			m_data = dt.Format("MMDDYY");
		}

		/// <summary>
		/// Set a left justified integer, pad with the default fill
		/// </summary>
		/// <param name="i">int</param>
		public void SetIntLeft( int i )
		{
			m_data = Right((m_field.DefaultData + i.ToString()), m_field.Length);
			
			Debug.Assert( m_data.Length == m_field.Length );
			if ( m_data.Length != m_field.Length )
			{
				throw new Exception( m_field.Name + " incorrect length" );
			}
		}
		
		public void SetSignedIntLeft( int i )
		{
			char sign = '+';
			if ( i < 0 )
			{
				sign = '-';
				i = -i;
			}
			m_data = sign + Right(m_field.DefaultData + i.ToString(), m_field.Length-1);
			Debug.Assert(m_data.Length == m_field.Length, "Assumption failure");

			Debug.Assert( m_data.Length == m_field.Length );
			if ( m_data.Length != m_field.Length )
			{
				throw new Exception( m_field.Name + " incorrect length" );
			}
		}

		/// <summary>
		/// Set the exact string.  The length must equal the field size.
		/// </summary>
		/// <param name="val">the data</param>
		public void Set( string val )
		{
			Debug.Assert( val.Length == m_field.Length );
			m_data = val;

			Debug.Assert( m_data.Length == m_field.Length );
			if ( m_data.Length != m_field.Length )
			{
				throw new Exception( m_field.Name + " incorrect length" );
			}
		}

		/// <summary>
		/// Set the field as tandem format money (+00000000000000;-00000000000000)
		/// </summary>
		/// <param name="val">money</param>
		public void SetMoney( decimal val )
		{
			char sign = '+';
			if ( val < 0 )
			{
				sign = '-';
				val = -val;
			}
			m_data = sign + Right((m_field.DefaultData + val.ToString("#0.00").Replace(".", "")), m_field.Length-1);
			Debug.Assert(m_data.Length == m_field.Length, "Assumption failure");

			Debug.Assert( m_data.Length == m_field.Length );
			if ( m_data.Length != m_field.Length )
			{
				throw new Exception( m_field.Name + " incorrect length" );
			}
		}

		/// <summary>
		/// Set left justified, padded with the default fill
		/// </summary>
		/// <param name="val">data</param>
		public void SetLeft( string val )
		{
			m_data = Right((m_field.DefaultData + val), m_field.Length);

			Debug.Assert( m_data.Length == m_field.Length );
			if ( m_data.Length != m_field.Length )
			{
				throw new Exception( m_field.Name + " incorrect length" );
			}
		}

		/// <summary>
		/// format as YYYY-MM-DD:HH:NN:SS.000000
		/// </summary>
		/// <param name="dtm">date</param>
		public void SetDateTime( DateTime dtm )
		{
			Debug.Assert( m_field.Length == 26, "Invalid field size" );
			m_data = dtm.ToString("yyyy-MM-dd:HH:mm:ss.000000");
			Debug.Assert( m_data.Length == 26 );

			Debug.Assert( m_data.Length == m_field.Length );
			if ( m_data.Length != m_field.Length )
			{
				throw new Exception( m_field.Name + " incorrect length" );
			}
		}

		/// <summary>
		/// format as YYYY-MM-DD:HH:NN:SS.000000
		/// </summary>
		/// <param name="dt"></param>
		public void SetDateTime( Date dt )
		{
			Debug.Assert( m_field.Length == 26, "Invalid field size" );
			m_data = dt.Year + "-" + dt.Month.ToString("00") + "-" + dt.Day.ToString("00") + ":00:00:00.000000";
			Debug.Assert( m_data.Length == 26 );

			Debug.Assert( m_data.Length == m_field.Length );
			if ( m_data.Length != m_field.Length )
			{
				throw new Exception( m_field.Name + " incorrect length" );
			}
		}

		/// <summary>
		/// Left justify largish integer
		/// </summary>
		/// <param name="val"></param>
		public void SetDecimal ( decimal val )
		{
			m_data = Right((m_field.DefaultData + val.ToString()), m_field.Length);

			Debug.Assert( m_data.Length == m_field.Length );
			if ( m_data.Length != m_field.Length )
			{
				throw new Exception( m_field.Name + " incorrect length" );
			}
		}

		/// <summary>
		/// Special handling for the cert id.  Certs are left justified if numeric,
		/// right justified otherwise.
		/// </summary>
		/// <param name="val">cert id</param>
		public void SetSpecial( string val )
		{
			val = val.Trim();
			if ( val.Length == 0 )
			{
				m_data  = m_field.DefaultData;
				return;
			}
			int ival = 0;
			if ( IsNumeric( val, ref ival ) )
			{
				// left justify
				SetIntLeft( ival );
			}
			else
			{
				// right justify
				m_data = (val + m_field.DefaultData).Substring(0, m_field.Length);
			}

			Debug.Assert( m_data.Length == m_field.Length );
			if ( m_data.Length != m_field.Length )
			{
				throw new Exception( m_field.Name + " incorrect length" );
			}
		}

		/// <summary>
		/// Is the string a numeric value?
		/// </summary>
		/// <param name="str">certid</param>
		/// <param name="i">the value if numeric</param>
		/// <returns></returns>
		internal static bool IsNumeric( string str, ref int i )
		{
			try
			{
				i = Int32.Parse( str );
				return true;
			}
			catch ( Exception *ex )
			{
				delete ex;
				return false;
			}
		}

		/// <summary>
		/// Just like VB Right$
		/// </summary>
		/// <param name="str">the string</param>
		/// <param name="len">lenght to return</param>
		/// <returns></returns>
		internal static string Right( string str, int len )
		{
			if ( str.Length <= len )
			{
				return str;
			}
			return str.Substring(str.Length - len);
		}

		/// <summary>
		/// Return the substring right padded with spaces
		/// </summary>
		/// <param name="str">data</param>
		/// <param name="start">start position</param>
		/// <param name="len">length</param>
		/// <returns></returns>
		internal static string PadSubstring( string str, int start, int len )
		{
			if ( len + start > str.Length )
			{
				string part = str.Substring( start, str.Length - start );
				return part + (new string( ' ', len - part.Length ));
			}
			return str.Substring( start, len );
		}

		/// <summary>
		/// Strip leading zeros from a string
		/// </summary>
		/// <param name="str">the string</param>
		/// <returns>the string without leading zeros</returns>
		internal static string StripZeros( string str )
		{
			string ret = "";
			bool copying = false;

			for ( int x = 0; x < str.Length; x++ )
			{
				if ( !copying && str[x] == '0' )
				{
					ret += ' ';
				}
				else
				{
					copying = true;
					ret += str[x];
				}
			}
			return ret;
		}

		/// <summary>
		/// Reset field to default value
		/// </summary>
		public void Reset()
		{
			m_data = m_field.DefaultData;
			Debug.Assert( m_data.Length == m_field.Length );

			if ( m_data.Length != m_field.Length )
			{
				throw new Exception( m_field.Name + " incorrect length" );
			}
		}

		protected FixedFieldDef m_field;
		protected string m_data;
	}

	/// <summary>
	/// The data contained in a fixed width record
	/// </summary>
	public class FixedRecord
	{
		/// <summary>
		/// Initialize the record from the definition
		/// </summary>
		/// <param name="def"></param>
		/// <param name="fields"></param>
		public FixedRecord ( FixedRecordDef def, FixedFieldData[] fields )
		{
			m_recdef = def;
			m_data = fields;
		}

		/// <summary>
		/// Number of fields in the record
		/// </summary>
		public int Count
		{
			get
			{
				return m_data.Length;
			}
		}

		/// <summary>
		/// Data field by ordinal position
		/// </summary>
		public FixedFieldData this[ int index ]
		{
			get
			{
				return m_data[index];
			}
		}

		/// <summary>
		/// Data field by name
		/// </summary>
		public FixedFieldData this[ string name ]
		{
			get
			{
				return m_data[ m_recdef.GetFieldOrdinal( name ) ];
			}
		}

		/// <summary>
		/// Reset all fields to default values
		/// </summary>
		public void Reset()
		{
			for( int x = 0; x < m_data.Length; x++ )
			{
				m_data[x].Reset();
			}
		}

		/// <summary>
		/// Create the fixed text record
		/// </summary>
		/// <returns>Fixed text string</returns>
		public override string ToString()
		{
			StringBuilder buf = new StringBuilder();
			for( int x = 0; x < m_data.Length; x++ )
			{
				buf.Append( m_data[x].Value );
			}
			return buf.ToString() + "\r\n";
		}

		protected FixedRecordDef m_recdef;
		protected FixedFieldData[] m_data;
	}

*/
				
