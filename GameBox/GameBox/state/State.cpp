#include "State.h"
#include "../Game.h"

State::State(States::ID id, Game* pGame) {
	m_id = id;
	m_game = pGame;
}

State::~State() {
}
