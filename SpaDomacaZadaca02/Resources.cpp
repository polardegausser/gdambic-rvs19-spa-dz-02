#include "Resources.h"

const sf::Font Resources::LoadFont( const sf::String& path )
	{
	sf::Font loaded;
	if ( std::experimental::filesystem::exists( path.toAnsiString() ) )
		{
		if ( !loaded.loadFromFile( path ) )
			{
			throw std::runtime_error( "Failed loading font " + path );
			}
		}
	else
		{
		throw std::runtime_error( "No font file " + path );
		}
	return loaded;
	}
