/*
 * event.cpp
 *
 *  Created on: Sep 4, 2017
 *      Author: Ronn Quijada
 */

#include "event.h"



vector<eventType> event::eventQueue;

event::event()
{

}

event::~event()
{

}

void event::pushEvent(Uint32 type, Uint8 mButton, SDL_Keycode key)
{
	eventType temp;
	temp.eventVer = type;
	temp.mButton = mButton;
	temp.key = key;

	//add to queue
	eventQueue.push_back(temp);
}

//Only the engine is allowed to update the event broadcaster
//Other objects calling update may cause unknown behavior

void event::update()
{
	eventQueue.clear();
	while (SDL_PollEvent(&m_event) != 0)
	{
		if(m_event.type == SDL_QUIT)
		{
			pushEvent(m_event.type, (Uint8) 0, SDLK_0);
		}
		else if (m_event.type == SDL_KEYDOWN)
		{
			pushEvent(m_event.type, (Uint8) 0, m_event.key.keysym.sym);
		}
		else if (m_event.type == SDL_MOUSEBUTTONDOWN)
		{
			pushEvent(m_event.type, m_event.button.button, SDLK_0);
		}
	}
}

eventType event::getEvent(int index)
{
	return eventQueue[index];
}

int event::getSize()
{
	return eventQueue.size();
}
