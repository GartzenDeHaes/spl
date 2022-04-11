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
#ifndef _neuralnetwork_h
#define _neuralnetwork_h

#include <spl/collection/Array.h>
#include <spl/data/Connection.h>
#include <spl/Exception.h>
#include <spl/Memory.h>

namespace spl
{
	/** 
	 * @defgroup math Math
	 * @{
	 */

	class NeuralNetworkException : public Exception
	{
	public:
		NeuralNetworkException(const char *msg) : Exception(msg)
		{
		}
	};

	enum NetworkLayer
	{
		NET_INPUT = 0,
		NET_L1 = 1,
		NET_L2 = 2,
		NET_OUTPUT = 3
	};

	/** @brief Backpropagation neural network with optional momentum and adaptive learn rate.
	 *
	 */
	class Network : public IMemoryValidate
	{
	private:
		Network();
		void BuildNetwork( int inlen, int len1, int len2, int outlen );
		void DeleteArrays();
		void RandomizeWeights();

		int wt1ToInLen() { return m_inputLen * m_h1len; }
		int wt2To1Len() { return m_h1len * m_h2len; }
		int wtOutTo2Len() { return m_outputLen * m_h2len; }

		int m_inputLen;
		int m_h1len;
		int m_h2len;
		int m_outputLen;

		Array<double> m_layer1;
		Array<double> m_layer2;
		
		Array<double> m_errors1; // error for each node
		Array<double> m_errors2;
		Array<double> m_errorsOut;
		
		Array<double> m_wt1ToIn; // weights from layer 1 to inputs
		Array<double> m_delta1ToIn; // deltas from layer 1 to inputs
		
		Array<double> m_wt2To1; // weights from layer 2 to layer 1
		Array<double> m_delta2To1; // deltas from layer 2 to layer 1
		
		Array<double> m_wtOutTo2; // weights from output to layer 3
		Array<double> m_deltaOutTo2; // deltas from output to layer 3

	protected:
		void Init();
		void ActivateLayer( Array<double>& layer1, Array<double>& layer2, Array<double>& wt );

		inline void Activate()
		{
			ActivateLayer(m_input,  m_layer1, m_wt1ToIn);
			ActivateLayer(m_layer1, m_layer2, m_wt2To1);
			ActivateLayer(m_layer2, m_output, m_wtOutTo2);
		}

		int m_networkId;
		double m_learnRate;
		double m_momentum;
		double m_wtRange;
		bool m_useAdaptiveLR;
		bool m_useAdaptiveMom;
		double m_previousError;

		Array<double> m_input; // nodes
		Array<double> m_output;

	public:
		Network( int inlen, int len1, int len2, int outlen );
		Network( int id, Connection& conn );
		Network( const Network& net );
		virtual ~Network();

		void Write( Connection& conn );
		double CalcError( const Array<double>& input, const Array<double>& output );
		double Train(const Array<double>& input, const Array<double>& output, double dErrTarget, int maxCycles);

		inline void Activate( const Array<double>& input, Array<double>& output )
		{
			input.CopyToBinary(m_input);
			Activate();
			m_output.CopyToBinary(output);
		}

		inline int NetworkId() const
		{
			return m_networkId;
		}

		inline int InputCount() const
		{
			return m_inputLen;
		}

		inline int Layer1Count() const
		{
			return m_h1len;
		}

		inline int Layer2Count() const
		{
			return m_h2len;
		}

		inline int OutputCount() const
		{
			return m_outputLen;
		}

		inline void ResetID()
		{
			m_networkId = -1;
		}

		Network& operator =(const Network& net);

	#ifdef DEBUG
		void ValidateMem() const;
		void CheckMem() const;
	#endif
	};

	/** @} */
}
#endif
