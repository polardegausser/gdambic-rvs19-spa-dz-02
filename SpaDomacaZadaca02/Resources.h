#pragma once

#include "SFML.h"

#include <filesystem>

class Resources
	{
	public:
		static const sf::Font LoadFont( const sf::String& path );
	};