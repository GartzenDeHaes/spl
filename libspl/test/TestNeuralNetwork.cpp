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
#include <spl/io/log/Log.h>
#include <spl/math/NeuralNetwork.h>

#ifdef DEBUG

using namespace spl;

void _TestNetwork(  )
{
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	DEBUG_ASSERT_MEM_NOTED();

	Network *net = new Network(2, 2, 1, 1);
	DEBUG_NOTE_MEM_ALLOCATION( net );
	net->CheckMem();
	DEBUG_ASSERT_MEM_NOTED();

	delete net;
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	DEBUG_ASSERT_MEM_NOTED();

	net = new Network(2, 2, 2, 1);
	double in1[] = {1.0,0.0};
	double out1[] = {1.0};
	double in2[] = {0.0, 1.0};
	double out2[] = {1.0};
	double in3[] = {1.0,1.0};
	double out3[] = {0.0};
	double in4[] = {0.0,0.0};
	double out4[] = {0.0};

	{
		Array<double> in1a(in1, 2);
		Array<double> out1a(out1, 1);

		Array<double> in2a(in2, 2);
		Array<double> out2a(out2, 1);
		Array<double> in3a(in3, 2);
		Array<double> out3a(out3, 1);
		Array<double> in4a(in4, 2);
		Array<double> out4a(out4, 1);

		double error = 1.0;
		while ( error > .0001 )
		{
			error = net->Train(in1a, out1a, .0000001, 10000);
			error += net->Train(in2a, out2a, .0000001, 10000);
			error += net->Train(in3a, out3a, .0000001, 10000);
			error += net->Train(in4a, out4a, .0000001, 10000);
		}
	}

	delete net;

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	DEBUG_ASSERT_MEM_NOTED();
	
	Log::SWriteOkFail( "Neural network test 1" );
}

#endif
