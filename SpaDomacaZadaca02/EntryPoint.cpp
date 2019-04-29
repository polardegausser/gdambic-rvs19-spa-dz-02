#include "App.h"
#include "Settings.h"

int main() {
	try
		{
		std::unique_ptr< App > app = std::make_unique< App >(USE_DESKTOP_RES ? sf::VideoMode::getDesktopMode().width : RES_X, USE_DESKTOP_RES ? sf::VideoMode::getDesktopMode().height : RES_Y, FPS, RANDOMIZER_VALUE, CELL_SIZE, CELL_GAP, ALIVE_COLOR, DEAD_COLOR, HOVER_COLOR, BG_COLOR);
		app->Run();
		return 0;
		}
	catch( std::runtime_error e )
		{
		MessageBox( nullptr, ( LPCWSTR ) e.what(), ( LPCWSTR ) "Error", 0 );
		return -1;
		}
	}
