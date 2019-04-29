#pragma once
// ==========================================================================

// Screen settings ----------------------------------------------------------
const bool					USE_DESKTOP_RES		= true;
const unsigned int			RES_X				= 1280;
const unsigned int			RES_Y				= 720;
const unsigned int			FPS					= 60;

// Size of the celll and gap between cells (in px) --------------------------
const float					CELL_SIZE			= 16.f;
const float					CELL_GAP			= 4.f;

// Time to sleep before next calc -------------------------------------------
const unsigned int			EXEC_DELAY_INIT		= 100;

// Probability of getting more living cells per field -----------------------
const unsigned long long	RANDOMIZER_VALUE	= 4ull;

// UI colors ----------------------------------------------------------------
const sf::Color				ALIVE_COLOR			= sf::Color(148, 0, 211);
const sf::Color				DEAD_COLOR			= sf::Color(225, 225, 225);
const sf::Color				HOVER_COLOR			= sf::Color(63, 211, 0, 200);
const sf::Color				BG_COLOR			= sf::Color(255, 255, 255);
const sf::Color				TEXT_COLOR			= sf::Color(0, 0, 0);

// ==========================================================================
