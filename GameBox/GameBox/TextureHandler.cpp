#include "TextureHandler.h"

TextureHandler::TextureHandler()
{
	if (!addTexture("default.png"))
		printf("Failed to load default texture");
}

TextureHandler::~TextureHandler()
{
	for (auto& i : m_texList)
	{
		delete i.second;
	}
}

bool TextureHandler::addTexture(const std::string& path)
{
	if (!m_texList.count(path))
	{
		// Load and store the texture in the texture map
		sf::Texture* texture = new sf::Texture;
		
		if (texture->loadFromFile("../Resources/" + path))
		{
			m_texList[path] = texture;
			return true;
		}
		else
		{
			delete texture;
			printf("Failed to load texture: texture not found");
			return false;
		}
	}
	else
	{
		return true;
	}
}

sf::Texture& TextureHandler::getTexture(const std::string& path)
{
	if (addTexture(path))
	{
		return *m_texList[path];
	}
	else
	{
		printf("Failed to set texture: could not load");
		return *m_texList["default.png"];
	}
}
