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
#include <spl/io/File.h>
#include <spl/math/StateMachine.h>
//#include <spl/xml/XmlElement.h>
//#include <spl/xml/XmlDocument.h>

using namespace spl;

IStateEventListener::~IStateEventListener()
{
}

TransitionActionResolver::TransitionActionResolver()
: m_map()
{
}

TransitionActionResolver::TransitionActionResolver(const TransitionActionResolver& resolver)
: m_map(resolver.m_map)
{
}

TransitionActionResolver::~TransitionActionResolver()
{
}

#ifdef DEBUG
void TransitionActionResolver::ValidateMem() const
{
	m_map.ValidateMem();
}

void TransitionActionResolver::CheckMem() const
{
	m_map.CheckMem();
}
#endif

State::State
(
	const char *name, 
	const char *enterActionName, 
	const char *leaveActionName, 
	IStateEventListener *enterAction, 
	IStateEventListener *leaveAction
)
:	m_name(name), 
	m_enterActionName(enterActionName), 
	m_leaveActionName(leaveActionName), 
	m_enterAction(enterAction),
	m_leaveAction(leaveAction),
	m_fromThisState(),
	m_transitionList()
{
}

State::~State()
{
	for ( int x = 0; x < m_transitionList.Count(); x++ )
	{
		delete m_transitionList.ElementAt(x);
	}
	m_transitionList.Clear();	
}

Transition *State::GetTransition( const String& inputText ) 
{ 
	Transition *tran = m_fromThisState.Get( inputText );
	if ( NULL != tran )
	{
		return tran;
	}
	// check for lamda default transition
	return m_fromThisState.Get(String("*"));
}

void State::FireOnLeave(const String& input, State *to)
{
	if ( NULL != m_leaveAction )
	{
		m_leaveAction->OnStateLeave(input, this, to);
	}
}

void State::FireOnEnter(const String& input, State *from)
{
	if ( NULL != m_enterAction )
	{
		m_enterAction->OnStateEnter(input, from, this);
	}
}

#ifdef DEBUG
void State::ValidateMem() const
{
	m_name.ValidateMem();
	m_enterActionName.ValidateMem();
	m_leaveActionName.ValidateMem();
	m_fromThisState.ValidateMem();
	m_transitionList.ValidateMem();
}

void State::CheckMem() const
{
	m_name.CheckMem();
	m_enterActionName.CheckMem();
	m_leaveActionName.CheckMem();
	m_fromThisState.CheckMem();
	m_transitionList.CheckMem();
}
#endif

Transition::Transition
(
 	const char *onInput, 
 	State *from, 
 	State *to, 
 	const char *actionName, 
	IStateEventListener *transitionAction
)
: 	m_onInput(onInput), 
	m_from(from), 
	m_to(to), 
	m_actionName(actionName), 
	m_transitionAction(transitionAction)
{
}

Transition::~Transition()
{
}

void Transition::FireOnTransition(const String& input)
{
	if ( NULL != m_transitionAction )
	{
		m_transitionAction->OnStateTransition(input, this);
	}
}

#ifdef DEBUG
void Transition::ValidateMem() const
{
	m_onInput.ValidateMem();
	m_actionName.ValidateMem();
}

void Transition::CheckMem() const
{
	m_onInput.CheckMem();
	m_actionName.CheckMem();
}
#endif

StateMachine::StateMachine()
: m_resolver(), m_currentState(NULL), m_stateIdx(), m_states()
{
}

StateMachine::~StateMachine()
{
	for ( int x = 0; x < m_states.Count(); x++ )
	{
		delete m_states.ElementAt(x);
	}
}

void StateMachine::SetState( const char *name )
{
	State *state = m_stateIdx.Get( String(name) );
	if ( NULL == state )
	{
		throw new InvalidArgumentException("State not found");
	}
	ASSERT_MEM( state, sizeof(State) );
	state->ValidateMem();
	m_currentState = state;
	m_currentState->FireOnEnter(String(""), NULL);
}

void StateMachine::DefineState( const char *name, const char *enterActionName, const char *leaveActionName )
{
	IStateEventListener *ea = m_resolver.Get(enterActionName);
	if ( NULL == ea && enterActionName[0] != '\0' )
	{
		throw new InvalidArgumentException("Enter action not found");
	}
	IStateEventListener *la = m_resolver.Get(leaveActionName);
	if ( NULL == la && leaveActionName[0] != '\0' )
	{
		throw new InvalidArgumentException("Leave action not found");
	}
	String sname(name);
	if ( m_stateIdx.ContainsKey( sname ) )
	{
		throw new InvalidArgumentException("State already exists -- state names must be unique");
	}

	State *state = new State(name, enterActionName, leaveActionName, ea, la);
	if ( NULL == state )
	{
		throw OutOfMemoryException();
	}
	m_stateIdx.Set( sname, state );
	m_states.Add( state );
}

void StateMachine::DefineTransition
( 
	const char *onInput, 
 	const char *from, 
 	const char *to, 
 	const char *actionName 
)
{
	IStateEventListener *la = m_resolver.Get(actionName);
	if ( NULL == la )
	{
		throw new InvalidArgumentException("Action not found");
	}
	State *tstate = m_stateIdx.Get(to);
	if ( NULL == tstate )
	{
		throw new InvalidArgumentException("To state not found");
	}
	State *fstate = m_stateIdx.Get( from );
	if ( NULL == fstate )
	{
		throw new InvalidArgumentException("From state not found");
	}
	if ( fstate->ContainsTransition( onInput ) )
	{
		throw new InvalidArgumentException("State already has a transition defined for this input");
	}
	fstate->AddTransitionFromThisState( new Transition(onInput, fstate, tstate, actionName, la) );
}

void StateMachine::ChangeState(const char *input)
{
	String sinput(input);
	if ( NULL == m_currentState )
	{
		throw new StateException("Current state is NULL, use SetState to set the start state");
	}
	ASSERT_MEM( m_currentState, sizeof(State) );
	m_currentState->ValidateMem();
	
	Transition *trans = m_currentState->GetTransition(sinput);
	if ( NULL == trans )
	{
		throw new StateException("There is no transition for that input");
	}
	ASSERT_MEM( trans, sizeof(Transition) );
	trans->ValidateMem();
	ASSERT( trans->GetFromState() == m_currentState );
	ASSERT( trans->InputText().Equals(input) || trans->InputText().Equals("*") );
	
	m_currentState->FireOnLeave(sinput, trans->GetToState());
	trans->FireOnTransition(sinput);
	m_currentState = trans->GetToState();
	m_currentState->FireOnEnter(sinput, trans->GetFromState());
}

void StateMachine::Load( const char *filename )
{
	if ( ! File::Exists(filename) )
	{
		throw new IOException("File not found");
	}
/*	XmlDocumentPtr doc = XmlDocument::Parse(filename);
	XmlElementPtr statemachine = doc->FirstChildElement("statemachine");
	if ( statemachine.IsNull() )
	{
		throw new StateException("Invalid XML file -- top-level '<statemachine>' tag not found.");
	}
	XmlElementPtr state = statemachine->FirstChildElement("state");
	while ( state.IsNotNull() )
	{
		XmlAttributePtr name = state->Attribute("name");
		if ( name.IsNull() )
		{
			throw new StateException("<state> must have a name attribute");
		}
		XmlAttributePtr enteraction = state->Attribute("enteraction");
		XmlAttributePtr exitaction = state->Attribute("exitaction");
		DefineState( name->Value()->GetChars(), (enteraction.IsNull()) ? NULL : enteraction->Value()->GetChars(), (exitaction.IsNull()) ? NULL : exitaction->Value()->GetChars() );

		state = state->NextSiblingElement("state");
	}

	state = statemachine->FirstChildElement("state");
	while ( state.IsNotNull() )
	{
		XmlAttributePtr name = state->Attribute("name");

		XmlElementPtr transition = state->FirstChildElement("transition");
		while ( transition.IsNotNull() )
		{
			XmlAttributePtr input = transition->Attribute("input");
			XmlAttributePtr to = transition->Attribute("to");
			if ( input.IsNull() || to.IsNull() )
			{
				throw new StateException("<transition> must have 'input' and 'to' attributes");
			}
			XmlAttributePtr enteraction = transition->Attribute("action");
			DefineTransition(input->Value()->GetChars(), name->Value()->GetChars(), to->Value()->GetChars(), (enteraction.IsNull()) ? NULL : enteraction->Value()->GetChars());

			transition = transition->NextSiblingElement("transition");
		}

		state = state->NextSiblingElement("state");
	}*/
}

#ifdef DEBUG
void StateMachine::ValidateMem() const
{
	m_resolver.ValidateMem();
	m_states.ValidateMem();
	m_stateIdx.ValidateMem();
}

void StateMachine::CheckMem() const
{
	m_resolver.CheckMem();
	m_states.CheckMem();
	m_stateIdx.CheckMem();
}
#endif
