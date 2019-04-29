#pragma once

#include "Field.h"
#include "Resources.h"

#include <memory>
#include <thread>
#include <mutex>
#include <sstream>
#include <iomanip>

class App
	{
	public:
		/*
		╔════════════════════════════════════════╗
		║	Class: App							 ║
		╟────────────────────────────────────────╢
		║	Constructors & Destructor			 ║
		╚════════════════════════════════════════╝
		*/
		App( unsigned int resX, unsigned int resY, unsigned int FPS, unsigned long long randomizerValue, float cellSize, float cellGap, const sf::Color& aliveColor, const sf::Color& deadColor, const sf::Color& hoverColor, const sf::Color& backgroundColor );
		App( App const & )		= delete;
		void operator=( App )	= delete;
		~App();

		/*
		╔════════════════════════════════════════╗
		║	Class: App							 ║
		╟────────────────────────────────────────╢
		║	Public methods						 ║
		╚════════════════════════════════════════╝
		*/
		void				Run						();

	protected:
		/*
		╔════════════════════════════════════════╗
		║	Class: App							 ║
		╟────────────────────────────────────────╢
		║	Protected methods					 ║
		╚════════════════════════════════════════╝
		*/
		void				ProcessInputs			();
		void				Tick					();
		void				Render					();
		void				TogglePause				();
		void				RandomizeField			();
		void				SpeedUp					();
		void				SpeedDown				();
		void				Randomize				();
		void				Derandomize				();
		const std::string	GetRandomizerValuePct	()						const;
		void				SetFPS					(unsigned int FPS);
		void				TickGeneration			();
		void				ClearField				();
		void				ToggleHelp				();

	private:
		/*
		╔════════════════════════════════════════╗
		║	Class: App							 ║
		╟────────────────────────────────────────╢
		║	Private variables and methods		 ║
		╚════════════════════════════════════════╝
		*/
		const sf::String	TITLE			= "Conway's Game of Life";
		const sf::String	TITLE_POSTFIX	= "  --  PRESS F1 to toggle Help, Stats & Settings  --  ";
		const sf::String	ASSETS_PATH		= R"(assets\)";
		const sf::String	FONT_FILE		= "font.ttf";
		const unsigned int	FONT_SIZE		= 15u;
		const float			TEXT_MARGIN		= 10.f;
		const unsigned int	NO_OF_LINES		= 15;

		std::unique_ptr< sf::RenderWindow > appWindow;
		std::unique_ptr< Field >			field;

		bool			paused;
		bool			closing;
		bool			helpOpened;
		unsigned int	executionDelay;
		
		// fonts
		sf::Font 
			font_app;
		// texts
		sf::Text 
			text_HelpTitle,
			text_Separator,
			text_DoubleSeparator,
			text_Escape, 
			text_Enter,
			text_Space,
			text_UpDown,
			text_LeftRight,
			text_Del,
			text_Randomize, 
			text_MouseClick,
			text_AppState, 
			text_Generation, 
			text_ExecutionDelay, 
			text_RandomizerValue;
		// colors
		sf::Color
			color_Background,
			color_Alive,
			color_Dead;
		// shapes
		sf::RectangleShape
			rect_HelpBG;
		// worker threads
		std::thread 
			thread_calculation;
		// mutexes
		std::mutex 
			mu_lock;

		void initText			( sf::Text& textRef, std::string text, sf::Vector2f pos );
		void updateText			( sf::Text& textRef, std::string text, sf::Vector2f pos );
		void calculationTask	();
	};