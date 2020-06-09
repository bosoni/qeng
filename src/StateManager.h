#pragma once
#include <vector>
#include "Core.h"

class BaseGameState
{

public:
	virtual void Init()
	{
	}

	virtual void Dispose()
	{
	}

	virtual void Update(float deltaTime)
	{
	}
};

class StateManager
{
	class State
	{
	public:
		BaseGameState* game;
		bool active;
		State(BaseGameState* gameState, bool actived)
		{
			game = gameState;
			active = actived;
		}
	};

	std::vector<State*> states;

	// When adding/removing state, then Render() and Update() aren't called.
	bool stateChanged = false;

public:
	void Clear()
	{
		stateChanged = true;
		for (int c = states.size() - 1; c >= 0; c--)
			if (states[c]->active)
			{
				states[c]->game->Dispose();
			}

		states.clear();
	}

	void Add(BaseGameState* game, bool active)
	{
		stateChanged = true;
		State* thisState = new State(game, active);
		states.push_back(thisState);
		if (active == true)
		{
			Log("+Add scene...");
			thisState->game->Init();
		}
	}

	void Remove(BaseGameState* game)
	{
		for (int c = states.size() - 1; c >= 0; c--)
			if (states[c]->game == game)
			{
				Log("-Remove scene...");
				stateChanged = true;
				states[c]->game->Dispose();
				states.erase(states.begin() + c);
			}
	}

	void SetActive(BaseGameState* game, bool active)
	{
		for (State* s : states)
		{
			if (s->game == game)
			{
				if (s->active == false && active == true) s->game->Init();
				else if (s->active == true && active == false) s->game->Dispose();
				s->active = active;
			}
		}
	}

	bool IsAnyStateActive()
	{
		for (State* s : states)
			if (s->active == true)
				return true;

		return false;
	}

	void Update(float gameTime)
	{
		if (stateChanged)
		{
			stateChanged = false;
			return;
		}

		for (int c = 0; c < states.size(); c++)
			if (states[c]->active)
			{
				states[c]->game->Update(gameTime);
			}
	}

};
