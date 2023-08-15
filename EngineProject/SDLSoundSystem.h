#pragma once
#include "SoundServiceLocator.h"
#include <memory>

namespace dae
{
	class SDLSoundSystem final : public SoundSystem
	{
	public:
		SDLSoundSystem();
		~SDLSoundSystem();
		
		void Play(const std::string& path, const int volume, bool loop = false) override;

	private:
		class SDLSoundSystemImpl;

		SDLSoundSystemImpl* m_pImpl;
	};
}