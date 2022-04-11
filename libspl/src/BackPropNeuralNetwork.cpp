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
#include <spl/math/Math.h>
#include <spl/math/NeuralNetwork.h>

using namespace spl;

void Network::DeleteArrays()
{
	//if ( m_input != NULL )
	//{
	//	delete m_input;
	//}
	//if ( m_output != NULL )
	//{
	//	delete m_output;
	//}
	//if ( m_layer1 != NULL )
	//{
	//	delete m_layer1;
	//}
	//if ( m_layer2 != NULL )
	//{
	//	delete m_layer2;
	//}
	//
	//if ( m_errors1 != NULL )
	//{
	//	delete m_errors1;
	//}
	//if ( m_errors2 != NULL )
	//{
	//	delete m_errors2;
	//}
	//if ( m_errorsOut != NULL )
	//{
	//	delete m_errorsOut;
	//}
	//
	//if ( m_wt1ToIn != NULL)
	//{
	//	delete m_wt1ToIn;
	//}
	//if ( m_delta1ToIn != NULL )
	//{
	//	delete m_delta1ToIn;
	//}
	//if ( m_wt2To1 != NULL )
	//{
	//	delete m_wt2To1;
	//}
	//if ( m_delta2To1 != NULL )
	//{
	//	delete m_delta2To1;
	//}
	//
	//if ( m_wtOutTo2 != NULL )
	//{
	//	delete m_wtOutTo2;
	//}
	//if ( m_deltaOutTo2 != NULL )
	//{
	//	delete m_deltaOutTo2;
	//}
}

void Network::RandomizeWeights()
{
	int x;
	// initialize the weights
	int len = m_inputLen * m_h1len;
	ASSERT_MEM( m_wt1ToIn, len );
	for (x = 0; x < len; x++)
	{
		m_wt1ToIn[x] = (Math::Random() * m_wtRange * 2.0) - m_wtRange;
	}
	len = m_h1len * m_h2len;
	ASSERT_MEM( m_wt2To1, len );
	for (x = 0; x < len; x++)
	{
		m_wt2To1[x] = (Math::Random() * m_wtRange * 2.0) - m_wtRange;
	}
	len = m_outputLen * m_h2len;
	ASSERT_MEM( m_wtOutTo2, len );
	for (x = 0; x < len; x++)
	{
		m_wtOutTo2[x] = (Math::Random() * m_wtRange * 2.0) - m_wtRange;
	}
}

void Network::BuildNetwork( int inlen, int len1, int len2, int outlen )
{
	m_inputLen = inlen;
	m_h1len = len1;
	m_h2len = len2;
	m_outputLen = outlen;

	m_input = Array<double>(inlen);
	m_layer1 = Array<double>(len1);
	m_layer2 = Array<double>(len2);
	m_output = Array<double>(outlen);
	m_errors1 = Array<double>(len1);
	m_errors2 = Array<double>(len2);
	m_errorsOut = Array<double>(outlen);
	m_wt1ToIn = Array<double>(inlen * len1);
	m_delta1ToIn = Array<double>(inlen * len1);
	m_wt2To1 = Array<double>(len1 * len2);
	m_delta2To1 = Array<double>(len1 * len2);
	m_wtOutTo2 = Array<double>(outlen * len2);
	m_deltaOutTo2 = Array<double>(outlen * len2);
	
	RandomizeWeights();
}

void Network::Init() 
{
 	m_networkId = -1;
	m_learnRate = .2;
	m_momentum = .05;
	m_wtRange = .2;
	m_useAdaptiveLR = true;
	m_useAdaptiveMom = false;
	m_previousError = 0.0;
}

/**
 *	Normal init
 */
Network::Network( int inlen, int len1, int len2, int outlen )
:	m_input(),
	m_output(),
	m_layer1(),
	m_layer2(),
	m_errors1(),
	m_errors2(),
	m_errorsOut(),
	m_wt1ToIn(),
	m_delta1ToIn(),
	m_wt2To1(),
	m_delta2To1(),
	m_wtOutTo2(),
	m_deltaOutTo2()
{
	Init();
	BuildNetwork( inlen, len1, len2, outlen );
}

Network::Network( const Network& net )
:	m_input(),
	m_output(),
	m_layer1(),
	m_layer2(),
	m_errors1(),
	m_errors2(),
	m_errorsOut(),
	m_wt1ToIn(),
	m_delta1ToIn(),
	m_wt2To1(),
	m_delta2To1(),
	m_wtOutTo2(),
	m_deltaOutTo2()
{
	Init();
	BuildNetwork( 1, 1, 1, 1 );

	*this = net;
}

/**
 *  Datebase load
 */
Network::Network( int id, Connection& conn )
:	m_input(),
	m_output(),
	m_layer1(),
	m_layer2(),
	m_errors1(),
	m_errors2(),
	m_errorsOut(),
	m_wt1ToIn(),
	m_delta1ToIn(),
	m_wt2To1(),
	m_delta2To1(),
	m_wtOutTo2(),
	m_deltaOutTo2()
{
	Init();

	CommandPtr cmd = conn.CreateCommand( "selNetwork" );
	cmd->CreateParameter("_NETWORK_ID", id);
	RecordSetPtr rs = cmd->ExecuteQuery();
	if ( ! rs->Next() )
	{
		throw NeuralNetworkException("Network not found");
	}

	m_networkId = id;
	m_learnRate = rs->GetColumn("LEARN_RATE_NUM")->GetFloat64();
	m_momentum = rs->GetColumn("MOMENTUM_NUM")->GetFloat64();
	m_wtRange = rs->GetColumn("WEIGHT_RANGE_NUM")->GetFloat64();
	m_useAdaptiveLR = rs->GetColumn("ADAPT_LEARN_RATE_IND")->GetBit();
	m_useAdaptiveMom = rs->GetColumn("ADAPT_MOM_IND")->GetBit();
	m_previousError = rs->GetColumn("PREV_ERROR_NUM")->GetFloat64();
	int input = rs->GetColumn("IN_NODE_NUM")->GetInt32();
	int h1 = rs->GetColumn("L1_NODE_NUM")->GetInt32();
	int h2 = rs->GetColumn("L2_NODE_NUM")->GetInt32();
	int output = rs->GetColumn("OUT_NODE_NUM")->GetInt32();

	BuildNetwork( input, h1, h2, output );

	cmd = conn.CreateCommand("selNodes" );
	cmd->CreateParameter("_NETWORK_ID", id );

	rs = cmd->ExecuteQuery();
	if ( 0 == rs->RowCount() )
	{
		// just use the newly created network;
		return;
	}
	while ( rs->Next() )
	{
		int idx = rs->GetColumn("INDEX_NUM")->GetInt32();
		double val = rs->GetColumn("VAL_NUM")->GetFloat64();

		switch ( rs->GetColumn("LAYER_NUM")->GetInt32() )
		{
			case NET_INPUT:
				m_input[idx] = val;
				break;
			case NET_L1:
				m_layer1[idx] = val;
				break;
			case NET_L2:
				m_layer2[idx] = val;
				break;
			case NET_OUTPUT:
				m_output[idx] = val;
				break;
			default:
				throw NeuralNetworkException( "Unexpected layer num in nodes " );
		}
	}

	cmd = conn.CreateCommand("selConns");
	cmd->CreateParameter("_NETWORK_ID", id );

	rs = cmd->ExecuteQuery();
	if ( 0 == rs->RowCount() )
	{
		throw NeuralNetworkException("Connections for network not found");
	}
	while ( rs->Next() )
	{
		int idx = rs->GetColumn("INDEX_NUM")->GetInt32();
		double weight = rs->GetColumn("WEIGHT_NUM")->GetFloat64();
		double delta = rs->GetColumn("DELTA_NUM")->GetFloat64();

		switch ( rs->GetColumn("LAYER_NUM")->GetByte() )
		{
			case NET_L1:
				m_wt1ToIn[idx] = weight;
				m_delta1ToIn[idx] = delta;
				break;
			case NET_L2:
				m_wt2To1[idx] = weight;
				m_delta2To1[idx] = delta;
				break;
			case NET_OUTPUT:
				m_wtOutTo2[idx] = weight;
				m_deltaOutTo2[idx] = delta;
				break;
			default:
				throw NeuralNetworkException("Unexpected layer num in nodes" );
		}
	}
}

Network::~Network()
{
	DeleteArrays();
}

Network& Network::operator =(const Network& net)
{
	DeleteArrays();

	Init();
	BuildNetwork( net.m_inputLen, net.m_h1len, net.m_h2len, net.m_outputLen );

	int x;
	for (x = 0; x < m_inputLen; x++)
	{
		m_input[x] = net.m_input[x];
	}
	for (x = 0; x < m_h1len; x++)
	{
		m_layer1[x] = net.m_layer1[x];
	}
	for (x = 0; x < m_h2len; x++)
	{
		m_layer2[x] = net.m_layer2[x];
	}
	for (x = 0; x < m_outputLen; x++)
	{
		m_output[x] = net.m_output[x];
	}

	for (x = 0; x < wt1ToInLen(); x++)
	{
		m_wt1ToIn[x] = net.m_wt1ToIn[x];
		m_delta1ToIn[x] = net.m_delta1ToIn[x];
	}
	for (x = 0; x < wt2To1Len(); x++)
	{
		m_wt2To1[x] = net.m_wt2To1[x];
		m_delta2To1[x] = net.m_delta2To1[x];
	}
	for (x = 0; x < wtOutTo2Len(); x++)
	{
		m_wtOutTo2[x] = net.m_wtOutTo2[x];
		m_deltaOutTo2[x] = net.m_deltaOutTo2[x];
	}

	return *this;
}

/// <summary>  
/// layer1 is "behind" layer 2.  If layer1 is Input, then layer2
/// is hidden layer 1.
/// </summary>
void Network::ActivateLayer(Array<double>& layer1, Array<double>& layer2, Array<double>& wt)
{
	double dSum;
	int iWtPos = 0;
	//double *iWtPos = &wt[0];
	//double *layer1p;
	//double *layer1pend = &layer1[l1len -1];
	
	for (int x = 0; x < layer2.Length(); x++)
	{
		dSum = 0.0;

		// calculate the ouput for this node	
		// for all the nodes connected to this one
		
		for (int y = 0; y < layer1.Length(); y++)
		{
			// sum the weights * outputs.  note that the weights are
			dSum += layer1[y] * wt[iWtPos++];
		}
		//dSum += *layer1p++ * *wt++;

		// set the sum for the current node
		//layer2[x] = (1 - Math.exp(-(dSum)))/(1+Math.exp(-(dSum)));
		// layer2[x] = (1.0-exp(-2.0*dSum))/(1.0+exp(-2.0*dSum));
		
		// the -1.0 in the exponent changes the logistic slope
		layer2[x] = 1.0 / (1 + exp(- 1.0 * dSum));
	}
}

double Network::CalcError( const Array<double>& input, const Array<double>& output )
{
	double dErrTotal = 0;
	int x;
	for (x = 0; x < m_inputLen; x++)
	{
		m_input[x] = input[x];
	}
	Activate();
				
	// calculate the final error
	for (x = 0; x < m_outputLen; x++)
	{
		// calculate the absolute error for the output layer
		double err = output[x] - m_output[x];
		
		// save the squared error
		dErrTotal += err * err;
	}
	return dErrTotal;
}

double Network::Train(const Array<double>& input, const Array<double>& output, double dErrTarget, int maxCycles)
{
	int delay = 0;
	int iWtPos;
	double dErrTotal;
	int x;

	ASSERT(input.Length() == m_input.Length());
	ASSERT(output.Length() == m_output.Length());

	do 
	{
		// set the errors to 0
		m_errors1.ClearBinary();
		m_errors2.ClearBinary();
		input.CopyToBinary(m_input);
		
		// activate
		Activate();
		
		iWtPos = 0;
		dErrTotal = 0.0;
		
		// calculate the final error
		for (x = 0; x < m_outputLen; x++)
		{
			// calculate the absolute error for the output layer
			m_errorsOut[x] = output[x] - m_output[x];

			// save the squared error
			dErrTotal += m_errorsOut[x] * m_errorsOut[x];
			
			// back prop the error, update the weights
			for (int y = 0; y < m_h2len; y++)
			{
				// back prop the error
				m_errors2[y] += m_errorsOut[x] * m_wtOutTo2[iWtPos];
				
				// update the delta and weight
				m_wtOutTo2[iWtPos] += (m_deltaOutTo2[iWtPos] = m_learnRate * m_errorsOut[x] * m_layer2[y] + m_momentum * m_deltaOutTo2[iWtPos]);
				//
				// need to check for exploding weights here
				//
				if (m_wtOutTo2[iWtPos] > 100.0 || m_wtOutTo2[iWtPos] < - 100.0)
				{
					throw NeuralNetworkException("Exploding weights");
				}
				if (m_wtOutTo2[iWtPos] == 0.0)
				{
					throw NeuralNetworkException("Weight underflow");
				}
				iWtPos++;
			}
		}
		iWtPos = 0;
		// hidden layer 2
		for (x = 0; x < m_h2len; x++)
		{
			if (m_layer2[x] > 0)
			{
				m_errors2[x] *= m_layer2[x] * (1.0 - m_layer2[x]);
			}
			else
			{
				m_errors2[x] *= - m_layer2[x] * (1.0 + m_layer2[x]);
			}
			// back prop the error, update the weights
			for (int y = 0; y < m_h1len; y++)
			{
				m_errors1[y] += m_errors2[x] * m_wt2To1[iWtPos];
				// update the delta and weight
				m_wt2To1[iWtPos] += (m_delta2To1[iWtPos] = m_learnRate * m_errors2[x] * m_layer1[y] + m_momentum * m_delta2To1[iWtPos]);
				//
				// need to check for exploding weights here
				//
				if (m_wt2To1[iWtPos] > 100.0 || m_wt2To1[iWtPos] < - 100.0)
				{
					throw NeuralNetworkException("Exploding weights");
				}
				if (m_wt2To1[iWtPos] == 0.0)
				{
					throw NeuralNetworkException("Weight underflow");
				}
				iWtPos++;
			}
		}
		iWtPos = 0;
		// hidden layer 1
		for (x = 0; x < m_h1len; x++)
		{
			if (m_layer1[x] > 0)
			{
				m_errors1[x] *= m_layer1[x] * (1 - m_layer1[x]);
			}
			else
			{
				m_errors1[x] *= - m_layer1[x] * (1 + m_layer1[x]);
			}
			// back prop the error, update the weights
			for (int y = 0; y < m_inputLen; y++)
			{
				//dpErrorsIn[y] += dpErrors1[x] * dpWt1ToIn[iWtPos];
				// update the delta and weight
				m_wt1ToIn[iWtPos] += (m_delta1ToIn[iWtPos] = m_learnRate * m_errors1[x] * m_input[y] + m_momentum * m_delta1ToIn[iWtPos]);
				//
				// need to check for exploding weights here
				//
				if (m_wt1ToIn[iWtPos] > 100.0 || m_wt1ToIn[iWtPos] < - 100.0)
				{
					throw NeuralNetworkException("Exploding weights");
				}
				if (m_wt1ToIn[iWtPos] == 0.0)
				{
					throw NeuralNetworkException("Weight underflow");
				}
				iWtPos++;
			}
		}
		if (m_useAdaptiveLR)
		{
			delay++;
			if (dErrTotal < m_previousError && delay > 10)
			{
				m_learnRate += .00001;
				delay = 0;
			}
			else if (m_learnRate > .02)
			{
				m_learnRate -= .005 * m_learnRate;
			}
			m_previousError = dErrTotal;
		}
	}
	while (dErrTotal > dErrTarget && maxCycles-- > 0);

	return dErrTotal;
}

void Network::Write( Connection& conn )
{
	int x;
	byte layerNum;
	bool update = true;
	CommandPtr cmd;

	if ( m_networkId < 0 )
	{
		update = false;
		cmd = conn.CreateCommand( "insNetwork" );
		cmd->CreateParameter("_NETWORK_ID", DbSqlType::SQL_TYPE_INT32, ParameterDirection::PARAM_DIR_OUT);
		cmd->CreateParameter("_LEARN_RATE_NUM", m_learnRate);
		cmd->CreateParameter("_MOMENTUM_NUM", m_momentum);
		cmd->CreateParameter("_WEIGHT_RANGE_NUM", m_wtRange);
		cmd->CreateParameter("_ADAPT_LEARN_RATE_IND", m_useAdaptiveLR);
		cmd->CreateParameter("_ADAPT_MOM_IND", m_useAdaptiveMom);
		cmd->CreateParameter("_PREV_ERROR_NUM", m_previousError);
		cmd->CreateParameter("_IN_NODE_NUM", m_inputLen);
		cmd->CreateParameter("_L1_NODE_NUM", m_h1len );
		cmd->CreateParameter("_L2_NODE_NUM", m_h2len );
		cmd->CreateParameter("_OUT_NODE_NUM", m_outputLen);
		cmd->ExecuteNonQuery();

		m_networkId = cmd->GetParameter("_NETWORK_ID")->GetInt32();
		ASSERT( m_networkId >= 0 );
	}


	if ( update )
	{
		cmd = conn.CreateCommand("updNode");
	}
	else
	{
		cmd = conn.CreateCommand("insNode");
	}
	cmd->CreateParameter("_NETWORK_ID", m_networkId);
	cmd->CreateParameter("_LAYER_NUM", DbSqlType::SQL_TYPE_INT8, ParameterDirection::PARAM_DIR_IN);
	cmd->CreateParameter("_INDEX_NUM", DbSqlType::SQL_TYPE_INT32, ParameterDirection::PARAM_DIR_IN);
	cmd->CreateParameter("_VAL_NUM", DbSqlType::SQL_TYPE_FLOAT64, ParameterDirection::PARAM_DIR_IN);

	layerNum = NET_INPUT;
	cmd->GetParameter("_LAYER_NUM")->Set(layerNum);
	for (x = 0; x < m_inputLen; x++)
	{
		cmd->GetParameter("_INDEX_NUM")->Set(x);
		cmd->GetParameter("@VAL_NUM")->Set(m_input[x]);
		cmd->ExecuteNonQuery();
	}	
	layerNum = NET_L1;
	cmd->GetParameter("_LAYER_NUM")->Set(layerNum);
	for (x = 0; x < m_h1len; x++)
	{
		cmd->GetParameter("_INDEX_NUM")->Set(x);
		cmd->GetParameter("_VAL_NUM")->Set(m_layer1[x]);
		cmd->ExecuteNonQuery();
	}
	layerNum = NET_L2;
	cmd->GetParameter("@LAYER_NUM")->Set(layerNum);
	for (x = 0; x < m_h2len; x++)
	{
		cmd->GetParameter("_INDEX_NUM")->Set(x);
		cmd->GetParameter("_VAL_NUM")->Set(m_layer2[x]);
		cmd->ExecuteNonQuery();
	}
	layerNum = NET_OUTPUT;
	cmd->GetParameter("@LAYER_NUM")->Set(layerNum);
	for (x = 0; x < m_outputLen; x++)
	{
		cmd->GetParameter("_INDEX_NUM")->Set(x);
		cmd->GetParameter("_VAL_NUM")->Set(m_output[x]);
		cmd->ExecuteNonQuery();
	}

	if ( update )
	{
		cmd = conn.CreateCommand("updConn");
	}
	else
	{
		cmd = conn.CreateCommand("insConn");
	}
	cmd->CreateParameter("_NETWORK_ID", m_networkId);
	cmd->CreateParameter("_LAYER_NUM", DbSqlType::SQL_TYPE_INT8, ParameterDirection::PARAM_DIR_IN);
	cmd->CreateParameter("_INDEX_NUM", DbSqlType::SQL_TYPE_INT32, ParameterDirection::PARAM_DIR_IN);
	cmd->CreateParameter("_WEIGHT_NUM", DbSqlType::SQL_TYPE_FLOAT64, ParameterDirection::PARAM_DIR_IN);
	cmd->CreateParameter("_DELTA_NUM", DbSqlType::SQL_TYPE_FLOAT64, ParameterDirection::PARAM_DIR_IN);

	layerNum = NET_L1;
	cmd->GetParameter("@LAYER_NUM")->Set(layerNum);
	for (x = 0; x < wt1ToInLen(); x++)
	{
		cmd->GetParameter("@INDEX_NUM")->Set(x);
		cmd->GetParameter("@WEIGHT_NUM")->Set(m_wt1ToIn[x]);
		cmd->GetParameter("@DELTA_NUM")->Set(m_delta1ToIn[x]);
		cmd->ExecuteNonQuery();
	}
	layerNum = NET_L2;
	cmd->GetParameter("@LAYER_NUM")->Set(layerNum);
	for (x = 0; x < wt2To1Len(); x++)
	{
		cmd->GetParameter("@INDEX_NUM")->Set(x);
		cmd->GetParameter("@WEIGHT_NUM")->Set(m_wt2To1[x]);
		cmd->GetParameter("@DELTA_NUM")->Set(m_delta2To1[x]);
		cmd->ExecuteNonQuery();
	}
	layerNum = NET_OUTPUT;
	cmd->GetParameter("@LAYER_NUM")->Set(layerNum);
	for (x = 0; x < wtOutTo2Len(); x++)
	{
		cmd->GetParameter("@INDEX_NUM")->Set(x);
		cmd->GetParameter("@WEIGHT_NUM")->Set(m_wtOutTo2[x]);
		cmd->GetParameter("@DELTA_NUM")->Set(m_deltaOutTo2[x]);
		cmd->ExecuteNonQuery();
	}
}

#ifdef DEBUG
void Network::ValidateMem() const
{
	ASSERT_MEM( m_input, m_inputLen * sizeof(double) );
	ASSERT_MEM( m_output, m_outputLen * sizeof(double) );
	ASSERT_MEM( m_layer1, m_h1len * sizeof(double) );
	ASSERT_MEM( m_layer2, m_h2len * sizeof(double) );
	ASSERT_MEM( m_errors1, m_h1len * sizeof(double) );
	ASSERT_MEM( m_errors2, m_h2len * sizeof(double) );
	ASSERT_MEM( m_errorsOut, m_outputLen * sizeof(double) );
	ASSERT_MEM( m_wt1ToIn, m_inputLen * m_h1len * sizeof(double) );
	ASSERT_MEM( m_delta1ToIn, m_inputLen * m_h1len * sizeof(double) );
	ASSERT_MEM( m_wt2To1, m_h1len * m_h2len * sizeof(double) );
	ASSERT_MEM( m_delta2To1, m_h1len * m_h2len * sizeof(double) );
	ASSERT_MEM( m_wtOutTo2, m_outputLen * m_h2len * sizeof(double) );
	ASSERT_MEM( m_deltaOutTo2, m_outputLen * m_h2len * sizeof(double) );
}

void Network::CheckMem() const
{
	DEBUG_NOTE_MEM_ALLOCATION( m_input );
	DEBUG_NOTE_MEM_ALLOCATION( m_output );
	DEBUG_NOTE_MEM_ALLOCATION( m_layer1 );
	DEBUG_NOTE_MEM_ALLOCATION( m_layer2 );
	DEBUG_NOTE_MEM_ALLOCATION( m_errors1 );
	DEBUG_NOTE_MEM_ALLOCATION( m_errors2 );
	DEBUG_NOTE_MEM_ALLOCATION( m_errorsOut );
	DEBUG_NOTE_MEM_ALLOCATION( m_wt1ToIn );
	DEBUG_NOTE_MEM_ALLOCATION( m_delta1ToIn );
	DEBUG_NOTE_MEM_ALLOCATION( m_wt2To1 );
	DEBUG_NOTE_MEM_ALLOCATION( m_delta2To1 );
	DEBUG_NOTE_MEM_ALLOCATION( m_wtOutTo2 );
	DEBUG_NOTE_MEM_ALLOCATION( m_deltaOutTo2 );
}
#endif
