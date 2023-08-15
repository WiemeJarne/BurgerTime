#pragma once
#include <string>
#include <memory>
#include "Singleton.h"

struct SDL_Texture;

namespace dae
{
	class TextureComponent;
	class Font;
	class ResourceManager final : public Singleton<ResourceManager>
	{
	public:
		void Init(const std::string& data);
		SDL_Texture* LoadTexture(const std::string& file) const;
		std::shared_ptr<Font> LoadFont(const std::string& file, unsigned int size) const;
	private:
		friend class Singleton<ResourceManager>;
		ResourceManager() = default;
		std::string m_dataPath;
	};
}