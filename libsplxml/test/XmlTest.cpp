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
/*
   Test program for TinyXML.
*/

#include <stdio.h>
#include <spl/Debug.h>
#include <spl/Double.h>
#include <spl/Int32.h>
#include <spl/io/log/Log.h>
#include <spl/xml/XmlAttribute.h>
#include <spl/xml/XmlComment.h>
#include <spl/xml/XmlDeclaration.h>
#include <spl/xml/XmlDocument.h>
#include <spl/xml/XmlElement.h>
#include <spl/xml/XmlNode.h>
#include <spl/xml/XmlText.h>
#include <spl/xml/XmlUnknown.h>

#ifdef DEBUG

using namespace spl;

bool XmlTest (const char* testString, const char* expected, const String& found, bool noEcho = false)
{
	bool pass = !strcmp( expected, found.GetChars() );
	UNIT_ASSERT(testString, pass);
	return pass;
}

bool XmlTest (const char* testString, const char* expected, StringPtr found, bool noEcho = false)
{
	bool pass = !strcmp( expected, found->GetChars() );
	UNIT_ASSERT(testString, pass);
	return pass;
}

bool XmlTest (const char* testString, const char* expected, const char *found, bool noEcho = false)
{
	bool pass = !strcmp( expected, found );
	UNIT_ASSERT(testString, pass);
	return pass;
}

bool XmlTest( const char* testString, int expected, int found, bool noEcho = false )
{
	bool pass = ( expected == found );
	UNIT_ASSERT(testString, pass);
	return pass;
}

//
// We start with the 'demoStart' todo list. Process it. And
// should hopefully end up with the todo list as illustrated.
//
static const char* _demoStart =
	"<?xml version=\"1.0\"  standalone='no' >\n"
	"<!-- Our to do list data -->"
	"<ToDo>\n"
	"<!-- Do I need a secure PDA? -->\n"
	"<Item priority=\"1\" distance='close'> Go to the <bold>Toy store!</bold></Item>"
	"<Item priority=\"2\" distance='none'> Do bills   </Item>"
	"<Item priority=\"2\" distance='far &amp; back'> Look for Evil Dinosaurs! </Item>"
	"</ToDo>";

/*	What the todo list should look like after processing.
	In stream (no formatting) representation. */
static const char* _demoEnd =
	"<?xml version=\"1.0\" standalone=\"no\" ?>"
	"<!-- Our to do list data -->"
	"<ToDo>"
	"<!-- Do I need a secure PDA? -->"
	"<Item priority=\"2\" distance=\"close\">Go to the"
	"<bold>Toy store!"
	"</bold>"
	"</Item>"
	"<Item priority=\"1\" distance=\"far\">Talk to:"
	"<Meeting where=\"School\">"
	"<Attendee name=\"Marple\" position=\"teacher\" />"
	"<Attendee name=\"Voel\" position=\"counselor\" />"
	"</Meeting>"
	"<Meeting where=\"Lunch\" />"
	"</Item>"
	"<Item priority=\"2\" distance=\"here\">Do bills"
	"</Item>"
	"</ToDo>";
//
// This file demonstrates some basic functionality of TinyXml.
// Note that the example is very contrived. It presumes you know
// what is in the XML file. But it does test the basic operations,
// and show how to add and remove nodes.
//

void _TestTinyXml()
{
	int count = 0;
	XmlElementPtr element;
	XmlNodePtr node;

	{
		// The example parses from the character string (above):
		{
			// Write to a file and read it back, to check file I/O.

			XmlDocumentPtr doc = XmlDocument::ParseXml(_demoStart);
			doc.ValidateMem();

			DEBUG_CLEAR_MEM_CHECK_POINTS();
			doc.CheckMem();
			DEBUG_DUMP_MEM_LEAKS();
			UNIT_ASSERT_MEM_NOTED("TinyXML 0");

			doc->Write("demotest.xml");
			UNIT_ASSERT( "demotest.xml should exist", File::Exists("demotest.xml"));

			DEBUG_CLEAR_MEM_CHECK_POINTS();
			doc.CheckMem();
			DEBUG_DUMP_MEM_LEAKS();
			UNIT_ASSERT_MEM_NOTED("TinyXML 1");
		}
		DEBUG_CLEAR_MEM_CHECK_POINTS();
		DEBUG_DUMP_MEM_LEAKS();
		UNIT_ASSERT_MEM_NOTED("TinyXML 2");

		XmlDocumentPtr doc = XmlDocument::Parse( "demotest.xml" );

		// Walk all the top level nodes of the document.
		count = 0;
		for( node = doc->FirstChild(); node.IsNotNull(); node = node->NextSibling() )
		{
			count++;
		}
		XmlTest( "Top level nodes, using First / Next.", 3, count );
		
		//printf( "** Demo doc read from disk: ** \n\n" );
		//printf( "** Printing via doc.Print **\n" );
		//doc.Print( stdout );

		{
			//printf( "** Printing via TiXmlPrinter **\n" );
			//TiXmlPrinter printer;
			//doc.Accept( &printer );
			//fprintf( stdout, "%s", printer.CStr() );
		}
		XmlElementPtr todoElement;
		XmlElementPtr itemElement;

		// --------------------------------------------------------
		// An example of changing existing attributes, and removing
		// an element from the document.
		// --------------------------------------------------------

		// Get the "ToDo" element.
		// It is a child of the document, and can be selected by name.
		node = doc->FirstChild( "ToDo" );
		ASSERT( node.IsNotNull() );
		todoElement = node->ToElement();
		ASSERT( todoElement.IsNotNull()  );

		// Going to the toy store is now our second priority...
		// So set the "priority" attribute of the first item in the list.
		node = todoElement->FirstChildElement();	// This skips the "PDA" comment.
		ASSERT( node.IsNotNull() );
		itemElement = node->ToElement();
		ASSERT( itemElement.IsNotNull()  );
		itemElement->SetAttribute( "priority", 2 );

		// Change the distance to "doing bills" from
		// "none" to "here". It's the next sibling element.
		itemElement = itemElement->NextSiblingElement();
		ASSERT( itemElement.IsNotNull() );
		itemElement->SetAttribute( "distance", "here" );

		// Remove the "Look for Evil Dinosaurs!" item.
		// It is 1 more sibling away. We ask the parent to remove
		// a particular child.
		itemElement = itemElement->NextSiblingElement();
		todoElement->RemoveChild( itemElement );

		itemElement.Release();

		// --------------------------------------------------------
		// What follows is an example of created elements and text
		// nodes and adding them to the document.
		// --------------------------------------------------------

		// Add some meetings.
		XmlElementPtr item = XmlElement::CreateElement( "Item" );
		item->SetAttribute( "priority", "1" );
		item->SetAttribute( "distance", "far" );

		XmlTextPtr text = XmlText::CreateText( "Talk to:" );

		XmlElementPtr meeting1 = XmlElement::CreateElement( "Meeting" );
		meeting1->SetAttribute( "where", "School" );

		XmlElementPtr meeting2 = XmlElement::CreateElement( "Meeting" );
		meeting2->SetAttribute( "where", "Lunch" );

		XmlElementPtr attendee1 = XmlElement::CreateElement( "Attendee" );
		attendee1->SetAttribute( "name", "Marple" );
		attendee1->SetAttribute( "position", "teacher" );

		XmlElementPtr attendee2 = XmlElement::CreateElement( "Attendee" );
		attendee2->SetAttribute( "name", "Voel" );
		attendee2->SetAttribute( "position", "counselor" );

		// Assemble the nodes we've created:
		meeting1->AppendChild( attendee1 );
		meeting1->AppendChild( attendee2 );

		item->AppendChild( text );
		item->AppendChild( meeting1 );
		item->AppendChild( meeting2 );

		// And add the node to the existing list after the first child.
		node = todoElement->FirstChild( "Item" );
		ASSERT( node.IsNotNull() );
		itemElement = node->ToElement();
		ASSERT( itemElement.IsNotNull() );

		todoElement->InsertAfter( itemElement, item );

		//printf( "\n** Demo doc processed: ** \n\n" );
		//doc.Print( stdout );

		// --------------------------------------------------------
		// Different tests...do we have what we expect?
		// --------------------------------------------------------

		//////////////////////////////////////////////////////

		node = doc->RootElement();
		node.ValidateMem();
		ASSERT( node.IsNotNull() );
		XmlTest( "Root element exists.", true, ( node.IsNotNull() && node->IsElement() ) );
		XmlTest ( "Root element value is 'ToDo'.", "ToDo",  node->Name().GetChars());

		node = node->FirstChild();
		XmlTest( "First child exists & is a comment.", true, ( node.IsNotNull() && node->IsComment() ) );
		node = node->NextSibling();
		XmlTest( "Sibling element exists & is an element.", true, ( node.IsNotNull() && node->IsElement() ) );
		XmlTest ( "Value is 'Item'.", "Item", node->Name().GetChars() );

		node = node->FirstChild();
		XmlTest ( "First child exists.", true, ( node.IsNotNull() && node->IsText() ) );
		XmlTest ( "Value is 'Go to the'.", "Go to the", node->Value()->GetChars() );

		doc.ValidateMem();
		
		// Walk all the top level nodes of the document.
		count = 0;
		for( node = doc->FirstChild(); node.IsNotNull(); node = node->NextSibling() )
		{
			count++;
		}
		XmlTest( "Top level nodes, using First / Next.", 3, count );

		count = 0;
		for( node = doc->LastChild();
			 node.IsNotNull();
			 node = node->PreviousSibling() )
		{
			count++;
		}
		XmlTest( "Top level nodes, using Last / Previous.", 3, count );

		// Walk all the top level nodes of the document,
		// using a different syntax.
		//count = 0;
		//for( node = doc.IterateChildren( 0 );
		//	 node;
		//	 node = doc.IterateChildren( node ) )
		//{
		//	count++;
		//}
		//XmlTest( "Top level nodes, using IterateChildren.", 3, count );

		// Walk all the elements in a node.
		count = 0;
		for( element = todoElement->FirstChildElement();
			 element.IsNotNull();
			 element = element->NextSiblingElement() )
		{
			count++;
		}
		XmlTest( "Children of the 'ToDo' element, using First / Next.", 3, count );

		// Walk all the elements in a node by value.
		count = 0;
		for( node = todoElement->FirstChild( "Item" ); node.IsNotNull(); node = node->NextSibling( "Item" ) )
		{
			count++;
		}
		XmlTest( "'Item' children of the 'ToDo' element, using First/Next.", 3, count );

		count = 0;
		for( node = todoElement->LastChild( "Item" );
			 node.IsNotNull();
			 node = node->PreviousSibling( "Item" ) )
		{
			count++;
		}
		XmlTest( "'Item' children of the 'ToDo' element, using Last/Previous.", 3, count );

		{
			XmlDocumentPtr doc = XmlDocument::ParseXml( "<Element0 attribute0='foo0' attribute1= noquotes attribute2 = '&gt;' />" );
			XmlElementPtr element0 = (XmlElementPtr)doc->FirstChild();
			UNIT_ASSERT("Should be an element", element0->NodeType() == XmlNode::ELEMENT);
			
			XmlTest ( "Element parsed, value is 'Element0'.", "Element0", element0->Name() );
			XmlTest ( "Reads attribute 'attribute0=\"foo0\"'.", "foo0", element0->Attribute( "attribute0" )->Value()->GetChars());
			XmlTest ( "Reads incorrectly formatted 'attribute1=noquotes'.", "noquotes", element0->Attribute( "attribute1" )->Value()->GetChars() );
			XmlTest ( "Read attribute with entity value '>'.", ">", element0->Attribute( "attribute2" )->Value()->GetChars() );
		}

		{
			const char* error =	"<?xml version=\"1.0\" standalone=\"no\" ?>\n"
								"<passages count=\"006\" formatversion=\"20020620\">\n"
								"    <wrong error>\n"
								"</passages>";

			XmlDocumentPtr docTest;
			try
			{
				docTest = XmlDocument::ParseXml( error );
				UNIT_ASSERT("should throw an error", false);
			}
			catch ( XmlException *xex )
			{
				XmlTest( "Error row", xex->Row(), 3 );
				XmlTest( "Error column", xex->Col(), 12 );
				delete xex;
			}
		}
	}
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("TinyXML 3");
	Log::SWriteOkFail( "TinyXML 1" );
	
	{
		const char* str = "<doc attr0='1' attr1='2.0' attr2='foo' />";

		XmlDocumentPtr doc = XmlDocument::ParseXml( str );

		XmlElementPtr ele = doc->FirstChildElement();

		int iVal;
		XmlAttributePtr result;
		double dVal;

		dVal = Double::Parse(*ele->Attribute( "attr0" )->Value());
		XmlTest( "Query attribute: int as double", (int)dVal, 1 );
		dVal = Double::Parse(*ele->Attribute( "attr1" )->Value());
		XmlTest( "Query attribute: double as double", (int)dVal, 2 );
		iVal = Int32::Parse(*ele->Attribute( "attr1" )->Value());
		XmlTest( "Query attribute: double as int", iVal, 2 );
		result = ele->Attribute( "attr2" );
		XmlTest( "Query attribute: not a number", Int32::IsInt(*result->Value()), false );
		result = ele->Attribute( "bar" );
		XmlTest( "Query attribute: does not exist", result.IsNotNull(), false );
	}
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("TinyXML 4");
	Log::SWriteOkFail( "TinyXML 2 attribute types" );
	
	{
		const char* str =	"\t<?xml version=\"1.0\" standalone=\"no\" ?>\t<room doors='2'>\n"
							"</room>";


		XmlParsingData dt(str, 8, 0, 0, true);
		XmlDocumentPtr doc = XmlDocument::ParseXml(str, &dt);
		//XmlDocument doc;
		//doc.SetTabSize( 8 );
		//doc.LoadXml( str );

		XmlElementPtr roomHandle = doc->FirstChildElement( "room" );

		XmlElementPtr room = roomHandle->ToElement();
		ASSERT( room.IsNotNull() );
		XmlAttributePtr doors = room->Attribute(0);
		ASSERT( doors.IsNotNull() );

		XmlTest( "Location tracking: Tab 8: room row", room->Row(), 1 );
		XmlTest( "Location tracking: Tab 8: room col", room->Column(), 49 );
		XmlTest( "Location tracking: Tab 8: doors row", doors->Row(), 1 );
		XmlTest( "Location tracking: Tab 8: doors col", doors->Column(), 55 );
	}
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("TinyXML 5");
	Log::SWriteOkFail( "TinyXML 3 row column" );
	
	{
		const char* str =	"\t<?xml version=\"1.0\" standalone=\"no\" ?>\t<room doors='2'>\n"
							"  <!-- Silly example -->\n"
							"    <door wall='north'>A great door!</door>\n"
							"\t<door wall='east'/>"
							"</room>";

		XmlDocumentPtr doc = XmlDocument::ParseXml( str );

		XmlElementPtr roomHandle = doc->FirstChildElement( "room" );
		XmlElementPtr commentHandle = (XmlElementPtr)doc->FirstChildElement( "room" )->FirstChild();
		XmlElementPtr textHandle = (XmlElementPtr)doc->FirstChildElement( "room" )->ChildElement( "door", 0 )->FirstChild();
		XmlElementPtr door0Handle = doc->FirstChildElement( "room" )->ChildElement( 0 );
		XmlElementPtr door1Handle = doc->FirstChildElement( "room" )->ChildElement( 1 );
		ASSERT(door0Handle.IsNotNull() && door1Handle.IsNotNull());

		UNIT_ASSERT("element", roomHandle->IsElement() );
		UNIT_ASSERT("text", textHandle->IsText() );
		UNIT_ASSERT("elemetn", door0Handle->IsElement() );
		UNIT_ASSERT("element", door1Handle->IsElement() );

		XmlDeclarationPtr declaration = doc->FirstChild()->ToDeclaration();
		assert( declaration.IsNotNull() );
		XmlElementPtr room = roomHandle->ToElement();
		ASSERT( room.IsNotNull() );
		XmlAttributePtr doors = room->Attribute(0);
		ASSERT( doors.IsNotNull() );
		XmlTextPtr text = textHandle->ToText();
		XmlCommentPtr comment = commentHandle->ToComment();
		ASSERT( comment.IsNotNull() );
		XmlElementPtr door0 = door0Handle->ToElement();
		XmlElementPtr door1 = door1Handle->ToElement();

		XmlTest( "Location tracking: Declaration row", declaration->Row(), 1 );
		XmlTest( "Location tracking: Declaration col", declaration->Column(), 5 );
		XmlTest( "Location tracking: room row", room->Row(), 1 );
		XmlTest( "Location tracking: room col", room->Column(), 45 );
		XmlTest( "Location tracking: doors row", doors->Row(), 1 );
		XmlTest( "Location tracking: doors col", doors->Column(), 51 );
		XmlTest( "Location tracking: Comment row", comment->Row(), 2 );
		XmlTest( "Location tracking: Comment col", comment->Column(), 3 );
		XmlTest( "Location tracking: text row", text->Row(), 3 ); 
		XmlTest( "Location tracking: text col", text->Column(), 24 );
		XmlTest( "Location tracking: door0 row", door0->Row(), 3 );
		XmlTest( "Location tracking: door0 col", door0->Column(), 5 );
		XmlTest( "Location tracking: door1 row", door1->Row(), 4 );
		XmlTest( "Location tracking: door1 col", door1->Column(), 5 );
	}
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("TinyXML 6");
	Log::SWriteOkFail( "TinyXML 4"  );
	
	if ( File::Exists("test/utf8test.xml"))
	{
		// --------------------------------------------------------
		// UTF-8 testing. It is important to test:
		//	1. Making sure name, value, and text read correctly
		//	2. Row, Col functionality
		//	3. Correct output
		// --------------------------------------------------------
		//printf ("\n** UTF-8 **\n");
		{
			XmlDocumentPtr doc = XmlDocument::Parse( "test/utf8test.xml" );

			// Get the attribute "value" from the "Russian" element and check it.
			XmlElementPtr element = doc->FirstChildElement( "document" )->FirstChildElement( "Russian" )->ToElement();
			const unsigned char correctValue[] = {	0xd1U, 0x86U, 0xd0U, 0xb5U, 0xd0U, 0xbdU, 0xd0U, 0xbdU, 
													0xd0U, 0xbeU, 0xd1U, 0x81U, 0xd1U, 0x82U, 0xd1U, 0x8cU, 0 };

			XmlTest( "UTF-8: Russian value.", (const char*)correctValue, element->Attribute( "value" )->Value(), true );
			XmlTest( "UTF-8: Russian value row.", 4, element->Row() );
			XmlTest( "UTF-8: Russian value column.", 5, element->Column() );

			const unsigned char russianElementName[] = {	0xd0U, 0xa0U, 0xd1U, 0x83U,
															0xd1U, 0x81U, 0xd1U, 0x81U,
															0xd0U, 0xbaU, 0xd0U, 0xb8U,
															0xd0U, 0xb9U, 0 };
			const char russianText[] = "<\xD0\xB8\xD0\xBC\xD0\xB5\xD0\xB5\xD1\x82>";

			XmlTextPtr text = doc->FirstChildElement( "document" )->FirstChildElement( (const char*) russianElementName )->Child( 0 )->ToText();
			XmlTest( "UTF-8: Browsing russian element name.",
					 russianText,
					 text->Value(),
					 true );
			XmlTest( "UTF-8: Russian element name row.", 7, text->Row() );
			XmlTest( "UTF-8: Russian element name column.", 47, text->Column() );

			XmlDeclarationPtr dec = doc->Child( 0 )->ToDeclaration();
			XmlTest( "UTF-8: Declaration column.", 1, dec->Column() );
			XmlTest( "UTF-8: Document column.", 1, doc->Column() );

			// Now try for a round trip.
			doc->Write( "utf8testout.xml" );

			// This test doesnt' work -- utf8testverify.xml is never created.

			// Check the round trip.
			//char savedBuf[256];
			//char verifyBuf[256];
			//int okay = 1;

			//FILE* saved  = fopen( "utf8testout.xml", "r" );
			//FILE* verify = fopen( "utf8testverify.xml", "r" );
			//if ( saved && verify )
			//{
			//	while ( fgets( verifyBuf, 256, verify ) )
			//	{
			//		fgets( savedBuf, 256, saved );
			//		if ( strcmp( verifyBuf, savedBuf ) )
			//		{
			//			okay = 0;
			//			break;
			//		}
			//	}
			//	fclose( saved );
			//	fclose( verify );
			//}
			//XmlTest( "UTF-8: Verified multi-language round trip.", 1, okay );

			// On most Western machines, this is an element that contains
			// the word "resume" with the correct accents, in a latin encoding.
			// It will be something else completely on non-wester machines,
			// which is why TinyXml is switching to UTF-8.
			const char latin[] = "<element>r\x82sum\x82</element>";

			XmlDocumentPtr latinDoc = XmlDocument::ParseXml( latin, NULL, TIXML_ENCODING_LEGACY );
			text = latinDoc->FirstChildElement()->FirstChild()->ToText();
			XmlTest( "Legacy encoding: Verify text element.", "r\x82sum\x82", text->Value() );
		}		
		DEBUG_CLEAR_MEM_CHECK_POINTS();
		DEBUG_DUMP_MEM_LEAKS();
		UNIT_ASSERT_MEM_NOTED("TinyXML 7");
		Log::SWriteOkFail( "TinyXML UTF-8" );
	}

	//////////////////////
	// Copy and assignment
	//////////////////////
	//printf ("\n** Copy and Assignment **\n");
	{
		XmlElementPtr element = XmlElement::CreateElement( "foo" );
		element.ValidateMem();
		element->_Parse( "<element name='value' />", NULL, TIXML_ENCODING_UNKNOWN );

		XmlElementPtr elementCopy = XmlElement::CreateElement( "" );
		*elementCopy = *element;
		XmlElementPtr elementAssign = XmlElement::CreateElement( "foo" );
		elementAssign->_Parse( "<incorrect foo='bar'/>", 0, TIXML_ENCODING_UNKNOWN );
		*elementAssign = *element;

		XmlTest( "Copy/Assign: element copy #1.", "element", elementCopy->Name() );
		XmlTest( "Copy/Assign: element copy #2.", "value", elementCopy->Attribute( "name" )->Value() );
		XmlTest( "Copy/Assign: element assign #1.", "element", elementAssign->Name() );
		XmlTest( "Copy/Assign: element assign #2.", "value", elementAssign->Attribute( "name" )->Value() );
		XmlTest( "Copy/Assign: element assign #3.", true, ( elementAssign->Attribute( "foo" ).IsNotNull()) );

		XmlComment comment;
		comment._Parse( "<!--comment-->", 0, TIXML_ENCODING_UNKNOWN );
		XmlComment commentCopy( comment );
		XmlComment commentAssign;
		commentAssign = commentCopy;
		XmlTest( "Copy/Assign: comment copy.", "comment", commentCopy.Value() );
		XmlTest( "Copy/Assign: comment assign.", "comment", commentAssign.Value() );

		XmlUnknownPtr unknown = XmlUnknown::CreateUnknown();
		unknown->_Parse( "<[unknown]>", 0, TIXML_ENCODING_UNKNOWN );
		XmlUnknownPtr unknownCopy = XmlUnknown::CreateUnknown();
		*unknownCopy = *unknown;
		XmlUnknownPtr unknownAssign = XmlUnknown::CreateUnknown();
		unknownAssign->_Parse( "<hi/>", 0, TIXML_ENCODING_UNKNOWN );
		*unknownAssign = *unknownCopy;
		XmlTest( "Copy/Assign: unknown copy.", "[unknown]", unknownCopy->Value() );
		XmlTest( "Copy/Assign: unknown assign.", "[unknown]", unknownAssign->Value() );
		
		XmlText text( "TextNode" );
		XmlText textCopy( text );
		XmlText textAssign( "incorrect" );
		textAssign = text;
		XmlTest( "Copy/Assign: text copy.", "TextNode", textCopy.Value() );
		XmlTest( "Copy/Assign: text assign.", "TextNode", textAssign.Value() );

		XmlDeclaration dec;
		dec._Parse( "<?xml version='1.0' encoding='UTF-8'?>", 0, TIXML_ENCODING_UNKNOWN );
		XmlDeclaration decCopy( dec );
		XmlDeclaration decAssign;
		decAssign = dec;

		XmlTest( "Copy/Assign: declaration copy.", "UTF-8", decCopy.Encoding() );
		XmlTest( "Copy/Assign: text assign.", "UTF-8", decAssign.Encoding() );

		//XmlDocument doc;
		//elementCopy.AppendChild( textCopy );
		//doc.AppendChild( decAssign );
		//doc.AppendChild( elementCopy );
		//doc.AppendChild( unknownAssign );

		//XmlDocument docCopy( doc );
		//XmlDocument docAssign;
		//docAssign = docCopy;
	}	
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("TinyXML 8");
	Log::SWriteOkFail( "TinyXML copy and assignment" );
	
	//////////////////////////////////////////////////////
	{
		const char *xmlcp = "<text><start>This  is    \ntext</start></text>";
		XmlParsingData conf(xmlcp, 3, 0, 0, false);
        
		XmlDocumentPtr doc = XmlDocument::ParseXml( "<text><start>This  is    \ntext</start></text>", &conf );
		UNIT_ASSERT("should be element", doc->FirstChild()->NodeType() == XmlNode::ELEMENT);
		XmlElementPtr text = (XmlElementPtr)doc->FirstChild();
		UNIT_ASSERT("should be element", text->FirstChild()->NodeType() == XmlNode::ELEMENT);
		XmlElementPtr start = (XmlElementPtr)text->FirstChild();
		UNIT_ASSERT("should be element", start->FirstChild()->NodeType() == XmlNode::TEXT);
		XmlElementPtr content = (XmlElementPtr)start->FirstChild();
		
		XmlTest ( "Condense white space OFF.", "This  is    \ntext",
					content->Value(),
					true );
	}
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("TinyXML 9");
	Log::SWriteOkFail( "TinyXML no condense white space" );
	
	//////////////////////////////////////////////////////
	// GetText();
	{
		const char* str = "<foo>This is text</foo>";
		XmlDocumentPtr doc = XmlDocument::ParseXml(str);
		XmlElementPtr element = doc->RootElement();

		XmlTest( "InnerText() normal use.", "This is text", element->InnerText() );

		str = "<foo><b>This is text</b></foo>";
		doc = XmlDocument::ParseXml( str );
		element = doc->RootElement();

		XmlTest( "InnerText() contained element.", "This is text", element->InnerText() );

		str = "<foo>This is <b>text</b></foo>";
		XmlDocument::SetCondenseWhiteSpace( false );
		doc = XmlDocument::ParseXml( str );
		XmlDocument::SetCondenseWhiteSpace( true );
		element = doc->RootElement();

		XmlTest( "InnerText() partial.", "This is text", element->InnerText() );
	}
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("TinyXML 10");
	Log::SWriteOkFail( "TinyXML InnerText()" );
	
	//////////////////////////////////////////////////////
	// CDATA
	{
		const char* str =	"<xmlElement>"
								"<![CDATA["
									"I am > the rules!\n"
									"...since I make symbolic puns"
								"]]>"
							"</xmlElement>";
		XmlDocumentPtr doc = XmlDocument::ParseXml( str );
		//doc.Print();
		XmlTest( "CDATA copy.", "I am > the rules!\n...since I make symbolic puns", doc->FirstChildElement()->FirstChild()->Value(), true );

		//XmlTest( "CDATA parse.", doc.FirstChildElement()->FirstChild()->Value(), 
		//						 "I am > the rules!\n...since I make symbolic puns",
		//						 true );

		XmlDocumentPtr doc1 = doc->Clone();
		//doc.Print();

		XmlTest( "CDATA copy.", "I am > the rules!\n...since I make symbolic puns", doc1->FirstChildElement()->FirstChild()->Value(), true );
	}
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("TinyXML 11");
	Log::SWriteOkFail( "TinyXML CDATA" );
	
	{
		// [ 1482728 ] Wrong wide char parsing
		char buf[256];
		buf[255] = 0;
		for( int i=0; i<255; ++i ) 
		{
			buf[i] = (char)((i>=32) ? i : 32);
		}
		String str( "<xmlElement><![CDATA[" );
		str = str + buf;
		str = str + "]]></xmlElement>";

		XmlDocumentPtr doc = XmlDocument::ParseXml( str.GetChars() );

		//XmlPrinter printer;
		//printer.SetStreamPrinting();
		//doc.Accept( &printer );

		//XmlTest( "CDATA with all bytes #1.", str.c_str(), printer.CStr(), true );
	}
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("TinyXML 12");
	Log::SWriteOkFail( "TinyXML 1482728" );
	
	{
		// [ 1480107 ] Bug-fix for STL-streaming of CDATA that contains tags
		// CDATA streaming had a couple of bugs, that this tests for.
		const char* str =	"<xmlElement>"
								"<![CDATA["
									"<b>I am > the rules!</b>\n"
									"...since I make symbolic puns"
								"]]>"
							"</xmlElement>";
		XmlDocumentPtr doc = XmlDocument::ParseXml(str);

		XmlTest( "CDATA parse. [ 1480107 ]", 
				"<b>I am > the rules!</b>\n...since I make symbolic puns",
				doc->FirstChildElement()->FirstChild()->Value(),
				true );

		XmlDocumentPtr doc1 = doc->Clone();
		//doc.Print();

		XmlTest( "CDATA copy. [ 1480107 ]", 
								 "<b>I am > the rules!</b>\n...since I make symbolic puns",
				doc1->FirstChildElement()->FirstChild()->Value(),
								 true );
	}
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("TinyXML 12");
	Log::SWriteOkFail( "TinyXML 1480107" );
	
	//////////////////////////////////////////////////////

	const int FUZZ_ITERATION = 200;

	// The only goal is not to crash on bad input.
	int len = (int) strlen( _demoStart );
	for( int i = 0; i < FUZZ_ITERATION; ++i ) 
	{
		char* demoCopy = new char[ len+1 ];
		ASSERT_MEM( demoCopy, len+1 );

		strcpy( demoCopy, _demoStart );

		demoCopy[ i%len ] = (char)((i+1)*3);
		demoCopy[ (i*7)%len ] = '>';
		demoCopy[ (i*11)%len ] = '<';
		ASSERT_MEM( demoCopy, len+1 );

		XmlDocumentPtr xml;
		try
		{
			xml = XmlDocument::ParseXml( demoCopy );
		}
		catch ( XmlException *xex )
		{
			delete xex;
		}
		ASSERT_MEM( demoCopy, len+1 );
		DEBUG_CLEAR_MEM_CHECK_POINTS();
		xml.CheckMem();
		DEBUG_NOTE_MEM(demoCopy);
		DEBUG_DUMP_MEM_LEAKS();
		UNIT_ASSERT_MEM_NOTED("TinyXML 12a");

		delete [] demoCopy;

		DEBUG_VALIDATE();
	}
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("TinyXML 13");
	Log::SWriteOkFail( "TinyXML Fuzzing" );
	
	//////////////////////////////////////////////////////

	// InsertBeforeChild and InsertAfterChild causes crash.
	{
		XmlElementPtr parent = XmlElement::CreateElement( "Parent" );
		XmlElementPtr childText0 = XmlElement::CreateElement( "childText0" );
		XmlElementPtr childText1 = XmlElement::CreateElement( "childText1" );
		parent->AppendChild( childText0 );
		parent->InsertBefore( childText0, childText1 );

		XmlTest( "Test InsertBeforeChild on empty node.", ( childText1.Get() == parent->FirstChild().Get() ), true );
	}
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("TinyXML 14");
	Log::SWriteOkFail( "TinyXML InsertBeforeChild and InsertAfterChild" );
	
	{
		// InsertBeforeChild and InsertAfterChild causes crash.
		XmlElementPtr parent = XmlElement::CreateElement( "Parent" );
		XmlElementPtr childText0 = XmlElement::CreateElement( "childText0" );
		XmlElementPtr childText1 = XmlElement::CreateElement( "childText1" );
		parent->AppendChild( childText0 );
		parent->InsertAfter( childText0, childText1 );

		XmlTest( "Test InsertAfterChild on empty node. ", ( childText1.Get() == parent->LastChild().Get() ), true );
	}
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("TinyXML 15");
	Log::SWriteOkFail( "TinyXML InsertBeforeChild and InsertAfterChild 2" );
	
	// Reports of missing constructors, irregular string problems.
	{
			// verifying some basic string functions:
			String a;
			String b( "Hello" );
			String c( "ooga" );

			c = " World!";
			a = b;
			a = a + c;
			a = a;

			XmlTest( "Basic TiXmlString test. ", "Hello World!", a );
	}
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("TinyXML 16");
	Log::SWriteOkFail( "TinyXML missing constructors, irregular string problems" );

#if defined(_WINDOWS) || defined(__CYGWIN__)
	///@TODO Takes a very long time to run on linux (cygwin is OK).  Possibliy due to
	/// debug heap needing to do scans to find pointers.
	if ( File::Exists("test/midsummerNightsDreamWithAVeryLongFilenameToConfuseTheStringHandlingRoutines.xml"))
	{
		{
			XmlDocumentPtr doc = XmlDocument::Parse( "test/midsummerNightsDreamWithAVeryLongFilenameToConfuseTheStringHandlingRoutines.xml" );
			doc.ValidateMem();
			DEBUG_CLEAR_MEM_CHECK_POINTS();
			doc.CheckMem();
			DEBUG_DUMP_MEM_LEAKS();
			UNIT_ASSERT_MEM_NOTED("TinyXML 17-1");

		}
		DEBUG_CLEAR_MEM_CHECK_POINTS();
		DEBUG_DUMP_MEM_LEAKS();
		UNIT_ASSERT_MEM_NOTED("TinyXML 17");
		Log::SWriteOkFail( "TinyXML Long filenames" );
	}
	else
	{
	  Log::SWriteError("long xml file not found");
	}
#endif

	{
		// Entities not being written correctly.
		// From Lynn Allen

		const char* passages =
			"<?xml version=\"1.0\" standalone=\"no\" ?>"
			"<passages count=\"006\" formatversion=\"20020620\">"
				"<psg context=\"Line 5 has &quot;quotation marks&quot; and &apos;apostrophe marks&apos;."
				" It also has &lt;, &gt;, and &amp;, as well as a fake copyright &#xA9;.\"> </psg>"
			"</passages>";

		XmlDocumentPtr doc = XmlDocument::ParseXml( passages );
		XmlElementPtr psg = doc->RootElement()->FirstChildElement();
		ASSERT( psg->Attribute( "context" ).IsNotNull() );
		StringPtr context = psg->Attribute( "context" )->Value();
		const char* expected = "Line 5 has \"quotation marks\" and 'apostrophe marks'. It also has <, >, and &, as well as a fake copyright \xC2\xA9.";

		XmlTest( "Entity transformation: read. ", expected, context->GetChars(), true );

		FILE* textfile = fopen( "textfile.txt", "w" );
		if ( textfile )
		{
			fputs( psg->ToString()->GetChars(), textfile );
			fclose( textfile );
		}
		textfile = fopen( "textfile.txt", "r" );
		ASSERT( textfile );
		if ( textfile )
		{
			char buf[ 1024 ];
			fgets( buf, 1024, textfile );
			XmlTest( "Entity transformation: write. ",
					 "<psg context=\'Line 5 has &quot;quotation marks&quot; and &apos;apostrophe marks&apos;."
					 " It also has &lt;, &gt;, and &amp;, as well as a fake copyright \xC2\xA9.' />",
					 buf,
					 true );
		}
		fclose( textfile );
		File::Delete("textfile.txt");
	}
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("TinyXML 18");
	Log::SWriteOkFail( "TinyXML Entities written correctly" );
	
	{
		FILE* textfile = fopen( "test5.xml", "w" );
		if ( textfile )
		{
			fputs("<?xml version='1.0'?><a.elem xmi.version='2.0'/>", textfile);
			fclose(textfile);

			XmlDocumentPtr doc = XmlDocument::Parse( "test5.xml" );
		}
	}
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("TinyXML 19");
	Log::SWriteOkFail( "TinyXML 5" );
	
	{
		FILE* textfile = fopen( "test6.xml", "w" );
		if ( textfile )
		{
			fputs("<element><Name>1.1 Start easy ignore fin thickness&#xA;</Name></element>", textfile );
			fclose(textfile);

			XmlDocumentPtr doc = XmlDocument::Parse( "test6.xml" );

			XmlTextPtr text = doc->FirstChildElement()->FirstChildElement()->FirstChild()->ToText();
			XmlTest( "Entity with one digit.",
						text->Value()->GetChars(), "1.1 Start easy ignore fin thickness\n" );
		}
	}
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("TinyXML 20");
	Log::SWriteOkFail( "TinyXML 6" );
	
	{
		// DOCTYPE not preserved (950171)
		// 
		const char* doctype =
			"<?xml version=\"1.0\" ?>"
			"<!DOCTYPE PLAY SYSTEM 'play.dtd'>"
			"<!ELEMENT title (#PCDATA)>"
			"<!ELEMENT books (title,authors)>"
			"<element />";

		XmlDocumentPtr doc = XmlDocument::ParseXml( doctype );
		doc->Write( "test7.xml" );
		doc = XmlDocument::Parse( "test7.xml" );
		
		XmlUnknownPtr unknown = doc->Child( 1 )->ToUnknown();
		XmlTest( "Correct value of unknown.", "!DOCTYPE PLAY SYSTEM 'play.dtd'", unknown->Value() );
	}
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("TinyXML 20");
	Log::SWriteOkFail( "TinyXML 950171" );
	
	{
		// [ 791411 ] Formatting bug
		// Comments do not stream out correctly.
		const char* doctype = 
			"<!-- Somewhat<evil> -->";
		XmlDocumentPtr doc = XmlDocument::ParseXml( doctype );

		XmlCommentPtr comment = doc->Child( 0 )->ToComment();

		XmlTest( "Comment formatting.", " Somewhat<evil> ", comment->Value() );
		#ifdef TIXML_USE_STL
		std::string str;
		str << (*comment);
		XmlTest( "Comment streaming.", "<!-- Somewhat<evil> -->", str.c_str() );
		#endif
	}
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("TinyXML 21");
	Log::SWriteOkFail( "TinyXML 791411" );
	
	{
		// [ 870502 ] White space issues
		XmlDocumentPtr doc;
		XmlTextPtr text;
	
		const char* doctype0 = "<element> This has leading and trailing space </element>";
		const char* doctype1 = "<element>This has  internal space</element>";
		const char* doctype2 = "<element> This has leading, trailing, and  internal space </element>";

		XmlDocument::SetCondenseWhiteSpace( false );
		doc = XmlDocument::ParseXml( doctype0 );
		text = doc->FirstChildElement( "element" )->Child( 0 )->ToText();
		XmlTest( "White space kept.", " This has leading and trailing space ", text->Value()->GetChars() );

		doc = XmlDocument::ParseXml( doctype1 );
		text = doc->FirstChildElement( "element" )->Child( 0 )->ToText();
		XmlTest( "White space kept.", "This has  internal space", text->Value() );

		doc = XmlDocument::ParseXml( doctype2 );
		text = doc->FirstChildElement( "element" )->Child( 0 )->ToText();
		XmlTest( "White space kept.", " This has leading, trailing, and  internal space ", text->Value()->GetChars() );

		XmlDocument::SetCondenseWhiteSpace( true );
		doc = XmlDocument::ParseXml( doctype0 );
		text = doc->FirstChildElement( "element" )->Child( 0 )->ToText();
		XmlTest( "White space condensed.", "This has leading and trailing space", text->Value() );

		doc = XmlDocument::ParseXml( doctype1 );
		text = doc->FirstChildElement( "element" )->Child( 0 )->ToText();
		XmlTest( "White space condensed.", "This has internal space", text->Value() );

		doc = XmlDocument::ParseXml( doctype2 );
		text = doc->FirstChildElement( "element" )->Child( 0 )->ToText();
		XmlTest( "White space condensed.", "This has leading, trailing, and internal space", text->Value() );
	}
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("TinyXML 21");
	Log::SWriteOkFail( "TinyXML 870502" );
	
	{
		// Double attributes
		const char* doctype = "<element attr='red' attr='blue' />";

		XmlDocumentPtr doc = XmlDocument::ParseXml( doctype );
		
		XmlTest( "Parsing repeated attributes.", "blue", doc->FirstChildElement( "element" )->Attribute( "attr" )->Value() );
	}
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("TinyXML 22");
	Log::SWriteOkFail( "TinyXML Double attributes" );
	
	{
		// Embedded null in stream.
		const char* doctype = "<element att\0r='red' attr='blue' />";

		XmlDocumentPtr doc;
		try
		{
			doc = XmlDocument::ParseXml( doctype );
			UNIT_ASSERT("Should have thrown an error", false);
		}
		catch ( XmlException *ex )
		{
			delete ex;
		}
	}
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("TinyXML 23");
	Log::SWriteOkFail( "TinyXML Embedded null in stream" );
	
	{
			// Legacy mode test. (This test may only pass on a western system)
			const char* str =
						"<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>"
						"<ä>"
						"CöntäntßäöüÄÖÜ"
						"</ä>";

			XmlDocumentPtr doc = XmlDocument::ParseXml( str );

			XmlElementPtr aHandle = doc->FirstChildElement( "ä" );
			XmlNodePtr tHandle = aHandle->Child( 0 );
			ASSERT( aHandle->IsElement() );
			ASSERT( tHandle->IsText() );
			XmlTest( "ISO-8859-1 Parsing.", "CöntäntßäöüÄÖÜ", tHandle->ToText()->Value() );
	}
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("TinyXML 24");
	Log::SWriteOkFail( "TinyXML Legacy mode" );
	
	{
		// Empty documents should return TIXML_ERROR_PARSING_EMPTY, bug 1070717
		const char* str = "    ";
		XmlDocumentPtr doc;
		try
		{
			doc = XmlDocument::ParseXml( str );
			UNIT_ASSERT("Should have thrown an error", false);
		}
		catch ( XmlException *xex )
		{
			delete xex;
		}
	}
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("TinyXML 25");
	Log::SWriteOkFail( "TinyXML 1070717" );
	
	{
		// String equality. [ 1006409 ] string operator==/!= no worky in all cases
		String temp;
		XmlTest( "Empty tinyxml string compare equal", ( temp == "" ), true );

		String    foo;
		String    bar( "" );
		XmlTest( "Empty tinyxml string compare equal", ( foo == bar ), true );
	}
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("TinyXML 26");
	Log::SWriteOkFail( "TinyXML 1006409" );
	
	{
		// Bug [ 1195696 ] from marlonism
		XmlDocument::SetCondenseWhiteSpace(false); 
		XmlDocumentPtr xml = XmlDocument::ParseXml("<text><break/>This hangs</text>"); 
	}
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("TinyXML 26");
	Log::SWriteOkFail( "TinyXML 1195696" );
	
	{
		// Bug [ 1243992 ] - another infinite loop
		XmlDocument::SetCondenseWhiteSpace(false);
		XmlDocumentPtr doc = XmlDocument::ParseXml("<p><pb></pb>test</p>");
	} 
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("TinyXML 27");
	Log::SWriteOkFail( "TinyXML 1243992" );
	
	{
		// Low entities
		XmlDocumentPtr xml = XmlDocument::ParseXml( "<test>&#x0e;</test>" );
		const char result[] = { 0x0e, 0 };
		XmlTest( "Low entities.", xml->FirstChildElement()->InnerText()->GetChars(), result );
		//xml.Print();
	}
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("TinyXML 28");
	Log::SWriteOkFail( "TinyXML Low entities" );
	
	{
		// Bug [ 1451649 ] Attribute values with trailing quotes not handled correctly
		XmlDocumentPtr xml;
		try
		{
			xml = XmlDocument::ParseXml( "<foo attribute=bar\" />" );
			UNIT_ASSERT("Should have thrown an error", false);
		}
		catch ( XmlException *xex )
		{
			delete xex;
		}
	}
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("TinyXML 29");
	Log::SWriteOkFail( "TinyXML 1451649" );
	
	#ifdef TIXML_USE_STL
	{
		// Bug [ 1449463 ] Consider generic query
		XmlDocumentPtr xml = XmlDocument::ParseXml( "<foo bar='3' barStr='a string'/>" );

		XmlElement* ele = xml->FirstChildElement();
		double d;
		int i;
		float f;
		bool b;
		//std::string str;

		XmlTest( "QueryValueAttribute", ele->QueryValueAttribute( "bar", &d ), TIXML_SUCCESS );
		XmlTest( "QueryValueAttribute", ele->QueryValueAttribute( "bar", &i ), TIXML_SUCCESS );
		XmlTest( "QueryValueAttribute", ele->QueryValueAttribute( "bar", &f ), TIXML_SUCCESS );
		XmlTest( "QueryValueAttribute", ele->QueryValueAttribute( "bar", &b ), TIXML_WRONG_TYPE );
		XmlTest( "QueryValueAttribute", ele->QueryValueAttribute( "nobar", &b ), TIXML_NO_ATTRIBUTE );
		//XmlTest( "QueryValueAttribute", ele->QueryValueAttribute( "barStr", &str ), TIXML_SUCCESS );

		XmlTest( "QueryValueAttribute", (d==3.0), true );
		XmlTest( "QueryValueAttribute", (i==3), true );
		XmlTest( "QueryValueAttribute", (f==3.0f), true );
		//XmlTest( "QueryValueAttribute", (str==std::string( "a string" )), true );
	}
	#endif
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("TinyXML 30");
	Log::SWriteOkFail( "TinyXML 1449463" );
	
	#ifdef TIXML_USE_STL
	{
		// [ 1505267 ] redundant malloc in TiXmlElement::Attribute
		XmlDocumentPtr xml = XmlDocument::ParseXml( "<foo bar='3' />" );
		XmlElement* ele = xml->FirstChildElement();
		double d;
		int i;

		std::string bar = "bar";

		const std::string* atrrib = ele->Attribute( bar );
		ele->Attribute( bar, &d );
		ele->Attribute( bar, &i );

		XmlTest( "Attribute", atrrib->empty(), false );
		XmlTest( "Attribute", (d==3.0), true );
		XmlTest( "Attribute", (i==3), true );
	}
	#endif
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("TinyXML 31");
	Log::SWriteOkFail( "TinyXML 1505267" );
	
	{
		// [ 1356059 ] Allow TiXMLDocument to only be at the top level
		XmlDocumentPtr xml = XmlDocument::ParseXml("<hi/>");
		XmlDocumentPtr xml2 = XmlDocument::ParseXml("<hi/>");
		try
		{
			xml->AppendChild( xml2 );
			XmlTest( "Document only at top level.", false, true );
		}
		catch (XmlException *xec)
		{
			delete xec;
		}
	}
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("TinyXML 32");
	Log::SWriteOkFail( "TinyXML 1356059" );
	
	{
		// [ 1663758 ] Failure to report error on bad XML
		XmlDocumentPtr xml;
		try
		{
			xml = XmlDocument::ParseXml("<x>");
			UNIT_ASSERT("Should have thrown an error", false);
		}
		catch ( XmlException *xex )
		{
			delete xex;
		}
		try
		{
			xml = XmlDocument::ParseXml("<x> ");
			UNIT_ASSERT("Should have thrown an error", false);
		}
		catch ( XmlException *xex )
		{
			delete xex;
		}
	} 
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("TinyXML 33");
	Log::SWriteOkFail( "TinyXML 1663758" );
	
	//Log::WriteCheck( "TinyXML 1635701" );
	//{
	//	// [ 1635701 ] fail to parse files with a tag separated into two lines
	//	// I'm not sure this is a bug. Marked 'pending' for feedback.
	//	XmlDocument xml;
	//	xml.LoadXml( "<title><p>text</p\n><title>" );
	//	//xml.Print();
	//	//XmlTest( "Tag split by newline", xml.Error(), false );
	//}
	//DEBUG_CLEAR_MEM_CHECK_POINTS();
	//DEBUG_DUMP_MEM_LEAKS();
	//UNIT_ASSERT_MEM_NOTED("TinyXML 34");
	//Log::WriteOkFail(  );
	
	//Log::WriteCheck( "TinyXML 1475201" );
	//{
	//	// [ 1475201 ] TinyXML parses entities in comments
	//	XmlDocument xml;
	//	istringstream parse1( "<!-- declarations for <head> & <body> -->"
	//					      "<!-- far &amp; away -->" );
	//	parse1 >> xml;
	//
	//	XmlNode* e0 = xml.FirstChild();
	//	XmlNode* e1 = e0->NextSibling();
	//	XmlComment* c0 = e0->ToComment();
	//	XmlComment* c1 = e1->ToComment();
	//
	//	XmlTest( "Comments ignore entities.", " declarations for <head> & <body> ", c0->Value(), true );
	//	XmlTest( "Comments ignore entities.", " far &amp; away ", c1->Value(), true );
	//}
	//DEBUG_CLEAR_MEM_CHECK_POINTS();
	//DEBUG_DUMP_MEM_LEAKS();
	//UNIT_ASSERT_MEM_NOTED("TinyXML 35");
	//Log::WriteOkFail(  );
	
	{
		// [ 1475201 ] TinyXML parses entities in comments
		XmlDocumentPtr xml = XmlDocument::ParseXml("<!-- declarations for <head> & <body> -->"
				  "<!-- far &amp; away -->" );

		XmlNodePtr e0 = xml->FirstChild();
		XmlNodePtr e1 = e0->NextSibling();
		XmlCommentPtr c0 = e0->ToComment();
		XmlCommentPtr c1 = e1->ToComment();

		XmlTest( "Comments ignore entities.", " declarations for <head> & <body> ", c0->Value(), true );
		XmlTest( "Comments ignore entities.", " far &amp; away ", c1->Value(), true );
	}
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("TinyXML 35");
	Log::SWriteOkFail( "TinyXML 1475201" );

	File::Delete("test5.xml");
	File::Delete("test6.xml");
	File::Delete("test7.xml");
	File::Delete("utf8testout.xml");
	File::Delete("demotest.xml");
}

void _TestXml()
{
	_TestTinyXml();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
}

#endif
