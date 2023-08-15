// Game.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#if _DEBUG
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Engine.h"
#include "SceneManager.h"
#include "SoundServiceLocator.h"
#include "SDLSoundSystem.h"

void load()
{
	//create a scene
	//auto scene = dae::SceneManager::GetInstance().CreateScene("scene", true);

	//register the sdl soundSystem
	dae::ServiceLocator::registerSoundSystem<dae::SDLSoundSystem>();
}

int main(int, char* []) {
	
	dae::Engine engine("../Data/");
	engine.Run(load);
	return 0;
}