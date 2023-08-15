#include <stdexcept>
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Engine.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "Timer.h"
#include "EventQueue.h"
#include "CollisionManager.h"
#include "EventQueueManager.h"

SDL_Window* g_window{};

void PrintSDLVersion()
{
	SDL_version version{};
	SDL_VERSION(&version);
	printf("We compiled against SDL version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	SDL_GetVersion(&version);
	printf("We are linking against SDL version %u.%u.%u.\n",
		version.major, version.minor, version.patch);

	SDL_IMAGE_VERSION(&version);
	printf("We compiled against SDL_image version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	version = *IMG_Linked_Version();
	printf("We are linking against SDL_image version %u.%u.%u.\n",
		version.major, version.minor, version.patch);

	SDL_TTF_VERSION(&version)
	printf("We compiled against SDL_ttf version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	version = *TTF_Linked_Version();
	printf("We are linking against SDL_ttf version %u.%u.%u.\n",
		version.major, version.minor, version.patch);
}

dae::Engine::Engine(const std::string &dataPath)
{
	PrintSDLVersion();
	
	if (SDL_Init(SDL_INIT_VIDEO) != 0) 
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	constexpr int windowWidth{ 416 };
	constexpr int windowHeight{ 480 };

	SceneManager::GetInstance().SetScenesSize(windowWidth, windowHeight);

	g_window = SDL_CreateWindow(
		"Programming 4 assignment",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		windowWidth,
		windowHeight,
		SDL_WINDOW_OPENGL
	);
	if (g_window == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	Renderer::GetInstance().Init(g_window);

	ResourceManager::GetInstance().Init(dataPath);
}

dae::Engine::~Engine()
{
	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(g_window);
	g_window = nullptr;
	SDL_Quit();
}

void dae::Engine::Run(const std::function<void()>& load)
{
	load();

	const float desiredFPS{ 144.f };
	std::chrono::duration<long long, std::milli> msPerFrame{ static_cast<long long>(1000.f / desiredFPS) };

	auto& input = InputManager::GetInstance();
	auto& sceneManager = SceneManager::GetInstance();
	auto& eventQueueManager = EventQueueManager::GetInstance();
	auto& renderer = Renderer::GetInstance();
	auto& collisionManager = CollisionManager::GetInstance();

	Timer::GetInstance().Start();
	bool doContinue = true;
	while (doContinue)
	{
		const auto start = std::chrono::high_resolution_clock::now();

		doContinue = input.ProcessInput();
		sceneManager.Update();
		eventQueueManager.HandleEventQueues();
		renderer.Render();
		collisionManager.Update();

		Timer::GetInstance().Update();

		const auto sleepTime{ start + std::chrono::milliseconds(msPerFrame) - std::chrono::high_resolution_clock::now()};
		std::this_thread::sleep_for(sleepTime);
	}
}