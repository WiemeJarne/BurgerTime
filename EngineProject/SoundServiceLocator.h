#pragma once
#include <string>
#include <memory>

namespace dae
{
	using soundId = unsigned short;

	class SoundSystem
	{
	public:
		virtual ~SoundSystem() = default;
		virtual void Play(const std::string& path, const int volume, bool loop = false) = 0;
	};

	class DefaultSoundSystem final : public SoundSystem
	{
	public:
		void Play(const std::string&, const int, bool) override {}
	};

	
	class ServiceLocator final
	{
	public:
		~ServiceLocator() = default;
		static SoundSystem& GetSoundSystem() { return *m_spSoundSystem; }

		template<typename T>
		static void registerSoundSystem()
		{	
			static_assert(std::is_base_of<SoundSystem, T>::value);
			m_spSoundSystem = std::make_unique<T>();
		}

	private:
		static std::unique_ptr<SoundSystem> m_spSoundSystem;
	};
}