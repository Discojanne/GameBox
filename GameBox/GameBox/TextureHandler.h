#pragma once


#include <SFML\Graphics.hpp>
#include <unordered_map>


class TextureHandler
{
public:
	
	static TextureHandler& getInstance()
	{
		static TextureHandler    instance; // Guaranteed to be destroyed.
										   // Instantiated on first use.
		return instance;
	}

	~TextureHandler();
	TextureHandler(TextureHandler const&) = delete;
	void operator=(TextureHandler const&) = delete;

	// Returns texture if it's already loaded
	sf::Texture& getTexture(const std::string& path);
private:
	TextureHandler();


	bool addTexture(const std::string& path);

	std::unordered_map<std::string, sf::Texture*> m_texList;

};
