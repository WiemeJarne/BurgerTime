#include "SDLSoundSystem.h"
#include <SDL.h>
#include <SDL_mixer.h>
#include <iostream>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <algorithm>

using namespace dae;

class SDLSoundSystem::SDLSoundSystemImpl final
{
public:
	SDLSoundSystemImpl()
	{
		//initalize SDL audio system
		if (SDL_Init(SDL_INIT_AUDIO) == -1)
			std::cout << "failed to initialize SLD audio system: " << SDL_GetError() << '\n';

		//initialize SDL_mixer
		if (Mix_OpenAudio(48000, MIX_DEFAULT_FORMAT, 2, 2048) == -1)
			std::cout << "failed to open audio device: " << Mix_GetError() << '\n';

		//create and start the sound thread
		m_SoundThread = std::jthread{ [&] { SoundThread(); } };
	}

	~SDLSoundSystemImpl()
	{
		for (auto& sound : m_Sounds)
		{
			Mix_FreeChunk(std::get<1>(sound));
		}

		m_Continue = false;
		m_SoundQueueCondition.notify_all();

		Mix_CloseAudio();
	}

	void SoundThread()
	{
		while (m_Continue)
		{
			std::unique_lock lock(m_SoundMutex);
			m_SoundQueueCondition.wait(lock);

			if (!m_SoundQueue.empty() && m_Continue)
			{
				auto& sound{ m_SoundQueue.front() };
				m_SoundQueue.pop();

				lock.unlock();

				if(std::get<0>(sound) >= m_Sounds.size())
				{
					auto toLoadSound = std::find_if(m_SoundsToLoad.begin(), m_SoundsToLoad.end(), [&sound](const std::pair<std::string, int>& toLoadSound) { return std::get<0>(sound) == toLoadSound.second; });

					if(toLoadSound != m_SoundsToLoad.end())
					{
						Mix_Chunk* pChunk{ Mix_LoadWAV(("..\\Data\\" + toLoadSound->first).c_str()) };

						if (pChunk)
							m_Sounds.push_back({ toLoadSound->first, pChunk, std::get<2>(sound)});
					}
				}

				Play(std::get<0>(sound), std::get<1>(sound));
			}
		}
	}

	void AddSoundToQueue(const std::string& path, const int volume, bool loop)
	{
		//check if the sound is already loaded
		int soundId{ -1 };
		for(int index{}; index < static_cast<int>(m_Sounds.size()); ++index)
		{
			if (std::get<0>(m_Sounds[index]) == path)
			{
				soundId = index;
				break;
			}
		}

		//if the soundId is -1 then the sound has to be loaded so add it to the m_SoundsToLoad vector
		if(soundId == -1)
		{
			soundId = static_cast<int>(m_Sounds.size());
			m_SoundsToLoad.push_back({ path, soundId });
		}

		const std::lock_guard lock{ m_SoundMutex };
		m_SoundQueue.push(std::make_tuple(  static_cast<dae::soundId>(soundId), volume, loop ));
		m_SoundQueueCondition.notify_all();
	}

	void Play(const soundId soundId, const int volume)
	{
		if (static_cast<int>(soundId) > m_Sounds.size())
		{
			std::cout << "invalid soundId\n";
			return;
		}

		int toUseVolume{};
		if (volume > MIX_MAX_VOLUME)
			toUseVolume = MIX_MAX_VOLUME;
		else if (volume < 0)
			toUseVolume = 0;
		else toUseVolume = volume;

		Mix_Volume(-1, toUseVolume);

		int loops{};
		if (std::get<2>(m_Sounds[soundId]))
			loops = -1; //meaning infinite

		Mix_PlayChannel(-1, std::get<1>(m_Sounds[soundId]), loops);
	}

private:
	std::vector<std::tuple<const std::string, Mix_Chunk*, bool>> m_Sounds{}; //string is for path name, bool is for looping
	std::vector<std::pair<const std::string, int>> m_SoundsToLoad{};
	std::queue<std::tuple<const soundId, const int, bool>> m_SoundQueue; //bool is for looping
	std::jthread m_SoundThread{};
	std::mutex m_SoundMutex{};
	std::condition_variable m_SoundQueueCondition{};
	std::atomic<bool> m_Continue{ true };
};

SDLSoundSystem::SDLSoundSystem()
	: m_pImpl{ new SDLSoundSystemImpl() }
{}

SDLSoundSystem::~SDLSoundSystem()
{
	delete m_pImpl;
}

void SDLSoundSystem::Play(const std::string& path, const int volume, bool loop)
{
	m_pImpl->AddSoundToQueue(path, volume, loop);
}