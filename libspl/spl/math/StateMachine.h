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
#ifndef _statemachine_h
#define _statemachine_h

#include <spl/Debug.h>
#include <spl/Exception.h>
#include <spl/collection/Hashtable.h>
#include <spl/collection/List.h>
#include <spl/Memory.h>
#include <spl/String.h>
#include <spl/collection/Vector.h>

namespace spl
{
	class State;
	class Transition;

	/** 
	 * @defgroup math Math
	 * @{
	 */

	/** @brief State event callback inteface. */
	class IStateEventListener : public IMemoryValidate
	{
	public:
		virtual ~IStateEventListener();
		virtual void OnStateEnter( const String &input, State *from, State *to ) = 0;
		virtual void OnStateLeave( const String &input, State *from, State *to ) = 0;
		virtual void OnStateTransition( const String &input, Transition *trans ) = 0;
		
	#ifdef DEBUG
		virtual void ValidateMem() const = 0;
		virtual void CheckMem() const = 0;
	#endif
	};

	/** @brief Allows action function names to be put in a file and resolved at runtime.
	  *
	  */
	class TransitionActionResolver : public IMemoryValidate
	{
	protected:
		Hashtable<String, IStateEventListener *> m_map;

	public:
		TransitionActionResolver();
		TransitionActionResolver(const TransitionActionResolver& resolver);
		virtual ~TransitionActionResolver();

		inline void Add(String& name, IStateEventListener *handler) { m_map.Set(name, handler); }
		inline IStateEventListener *Get(String& name) { return m_map.Get(name); }
		inline bool Has(String& name) { return m_map.ContainsKey(name); }
		inline void Add(const char *name, IStateEventListener *handler) { m_map.Set(String(name), handler); }
		inline IStateEventListener *Get(const char *name) { return m_map.Get(String(name)); }
		inline bool Has(const char *name) { return m_map.ContainsKey(String(name)); }

	#ifdef DEBUG
		virtual void ValidateMem() const;
		virtual void CheckMem() const;
	#endif
	};

	/**@brief A transition between two states on an input.
	  *
	  */
	class Transition : public IMemoryValidate
	{
	protected:
		String m_onInput;
		State *m_from;
		State *m_to;
		String m_actionName;
		IStateEventListener *m_transitionAction;
		
	public:
		Transition(const char *onInput, State *from, State *to, const char *actionName, IStateEventListener *transitionAction);
		virtual ~Transition();
				
		inline String &InputText() { return m_onInput; }
		inline State *GetFromState() { return m_from; }
		inline State *GetToState() { return m_to; }
		
		void FireOnTransition(const String& input);

	#ifdef DEBUG
		virtual void ValidateMem() const;
		virtual void CheckMem() const;
	#endif
	};

	/** @brief State is state
	  *
	  */
	class State : public IMemoryValidate
	{
	protected:
		String m_name;
		String m_enterActionName;
		String m_leaveActionName;

		IStateEventListener *m_enterAction;
		IStateEventListener *m_leaveAction;

		Hashtable<String, Transition *> m_fromThisState;
		Vector<Transition *> m_transitionList;

	public:
		State(const char *name, const char *enterActionName, const char *leaveActionName, IStateEventListener *enterAction, IStateEventListener *leaveAction);
		virtual ~State();

		void FireOnLeave(const String& input, State *to);
		void FireOnEnter(const String& input, State *to);
				
		inline void AddTransitionFromThisState( Transition *trans ) 
		{ 
			ASSERT_MEM(trans, sizeof(Transition)); 
			m_transitionList.Add(trans); 
			m_fromThisState.Set( trans->InputText(), trans ); 
		}
		
		inline bool ContainsTransition( const String& inputText ) 
		{ 
			return m_fromThisState.ContainsKey( inputText ); 
		}

		inline bool ContainsTransition( const char *inputText ) 
		{ 
			return m_fromThisState.ContainsKey( String(inputText) ); 
		}

		Transition *GetTransition( const String& inputText );
		String &Name() { return m_name; }

		inline Transition *GetTransition( const char *inputText ) 
		{ 
			return GetTransition( String(inputText) ); 
		}

	#ifdef DEBUG
		virtual void ValidateMem() const;
		virtual void CheckMem() const;
	#endif
	};

	/** @brief You should consider using SMC instead (http://smc.sourceforge.net/). */
	class StateMachine : public IMemoryValidate
	{
	private:
		// I suppose we should allow the copy constructor ...
		inline StateMachine(const StateMachine& s) {}
		inline void operator =(const StateMachine& s) {}

	protected:
		TransitionActionResolver m_resolver;
		State *m_currentState;
		Hashtable<String, State *> m_stateIdx;
		List<State *> m_states;
			
	public:
		StateMachine();
		virtual ~StateMachine();

		inline TransitionActionResolver& GetResolver() { return m_resolver; }
		
		void SetState( const char *statename );
		void DefineState( const char *name, const char *enterActionName, const char *leaveActionName );
		void DefineTransition( const char *onInput, const char *from, const char *to, const char *actionName );
		
		void ChangeState(const char *input);
		inline String CurrentStateName() { return (NULL == m_currentState) ? String() : m_currentState->Name();  }

		void Load( const char *filename );
		
	#ifdef DEBUG
		virtual void ValidateMem() const;
		virtual void CheckMem() const;
	#endif
	};

	/** @} */
}
#endif
