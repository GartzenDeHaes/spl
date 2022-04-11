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
#include <spl/Debug.h>
#include <spl/io/BlockingStream.h>
#include <spl/io/log/Log.h>
#include <spl/net/Packet.h>
#include <spl/io/MemoryStream.h>

using namespace spl;

#ifdef DEBUG

static void _TestPacket1()
{
	Packet *pkt = new Packet();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_NOTE_MEM( pkt );
	pkt->CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("Packet test 1.0");

	delete pkt;
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("Packet test 1.1");

	Log::SWriteOkFail( "Packet test 1" );
}

static void _TestPacket2()
{
	Packet *pkt = new Packet();
	ASSERT_MEM( pkt, sizeof(Packet) );
	pkt->ValidateMem();

	BlockingStream con(MemoryStreamPtr(new MemoryStream()));

	pkt->Append("hi");
	pkt->Append(71010);
	pkt->Append(66.6);
	pkt->Append((int16)3);
	pkt->SendPacket(con);

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_NOTE_MEM( pkt );
	pkt->CheckMem();
	con.CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("Packet test 2.0");

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_NOTE_MEM( pkt );
	pkt->CheckMem();
	con.CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("Packet test 2.1");

	pkt->ReadPacket(con);
	StringPtr str = pkt->ReadString();
	UNIT_ASSERT("pkt->ReadString == hi", str->Equals("hi"));
	UNIT_ASSERT("pkt->ReadInt == 71010", pkt->ReadInt32() == 71010);
	UNIT_ASSERT("pkt->ReadDouble == 66.6", pkt->ReadDouble() == 66.6);
	UNIT_ASSERT("pkt->ReadInt16 = 3.14", pkt->ReadInt16() == 3);

	delete pkt;
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	con.CheckMem();
	str.CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("Packet test 2.2");

	Log::SWriteOkFail( "Packet test 2" );
}

void TestPacket()
{
	_TestPacket1();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();

	_TestPacket2();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
}

#endif
