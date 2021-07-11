#pragma once

// Forward declare the StateStack
class StateStack;

namespace States {

	enum ID : char {
		None,
		MainMenu,
		Game
	};
}

// Base class for all states
class State {

public:
	State();
	virtual ~State();

	// Process input for the state.
	virtual void processInput(float dt) = 0;
	// Updates the state - runs every frame
	virtual void update(float dt) = 0;


private:

};