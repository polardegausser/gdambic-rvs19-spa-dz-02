#include "App.h"
#include "Settings.h"

App::App( unsigned int resX, unsigned int resY, unsigned int FPS, unsigned long long randomizerValue, float cellSize, float cellGap, const sf::Color& aliveColor, const sf::Color& deadColor, const sf::Color& hoverColor, const sf::Color& backgroundColor )
	: color_Background(backgroundColor), color_Alive(aliveColor), color_Dead(deadColor)
	{
	appWindow = std::make_unique<sf::RenderWindow>(sf::VideoMode(resX, resY), TITLE + TITLE_POSTFIX, sf::Style::Fullscreen);
	SetFPS( FPS );

	font_app = Resources::LoadFont( ASSETS_PATH + FONT_FILE );

	executionDelay = EXEC_DELAY_INIT;
	closing = false;
	paused = true;
	helpOpened = true;

	field = std::make_unique<Field>( (unsigned int)(appWindow->getSize().x / (cellSize + cellGap)), (unsigned int)((appWindow->getSize().y) / (cellSize + cellGap)), sf::Vector2f(0, 0), randomizerValue, cellSize, cellGap, aliveColor, deadColor, hoverColor );

	// help rectangle
	rect_HelpBG = sf::RectangleShape( sf::Vector2f( 355, ( 3 * TEXT_MARGIN ) + ( ( float ) NO_OF_LINES * ( float ) FONT_SIZE ) ) );
	rect_HelpBG.setOutlineThickness( cellGap );
	rect_HelpBG.setOutlineColor( aliveColor );
	rect_HelpBG.setFillColor( deadColor );
	rect_HelpBG.setPosition( appWindow->getSize().x / 2.f - rect_HelpBG.getSize().x / 2.f, appWindow->getSize().y / 2.f - rect_HelpBG.getSize().y / 2.f );

	// texts
	initText( text_HelpTitle			, " Help, Stats & Settings (F1 to toggle) "																		, sf::Vector2f( rect_HelpBG.getPosition().x + ( TEXT_MARGIN * 2 ), rect_HelpBG.getPosition().y + TEXT_MARGIN + ( float ) ( FONT_SIZE *  0 ) ) );
	initText( text_DoubleSeparator		, "======================================="																		, sf::Vector2f( rect_HelpBG.getPosition().x + ( TEXT_MARGIN * 2 ), rect_HelpBG.getPosition().y + TEXT_MARGIN + ( float ) ( FONT_SIZE *  1 ) ) );
	initText( text_Escape				, "ESC         =>                     quit"																		, sf::Vector2f( rect_HelpBG.getPosition().x + ( TEXT_MARGIN * 2 ), rect_HelpBG.getPosition().y + TEXT_MARGIN + ( float ) ( FONT_SIZE *  2 ) ) );
	initText( text_Enter				, "Enter       => evaluate next generation"																		, sf::Vector2f( rect_HelpBG.getPosition().x + ( TEXT_MARGIN * 2 ), rect_HelpBG.getPosition().y + TEXT_MARGIN + ( float ) ( FONT_SIZE *  3 ) ) );
	initText( text_Space				, "Space       =>             toggle pause"																		, sf::Vector2f( rect_HelpBG.getPosition().x + ( TEXT_MARGIN * 2 ), rect_HelpBG.getPosition().y + TEXT_MARGIN + ( float ) ( FONT_SIZE *  4 ) ) );
	initText( text_UpDown				, "Up/Down     =>   change execution delay"																		, sf::Vector2f( rect_HelpBG.getPosition().x + ( TEXT_MARGIN * 2 ), rect_HelpBG.getPosition().y + TEXT_MARGIN + ( float ) ( FONT_SIZE *  5 ) ) );
	initText( text_LeftRight			, "Left/Right  =>  change randomizer value"																		, sf::Vector2f( rect_HelpBG.getPosition().x + ( TEXT_MARGIN * 2 ), rect_HelpBG.getPosition().y + TEXT_MARGIN + ( float ) ( FONT_SIZE *  6 ) ) );
	initText( text_Del					, "Del         =>          clear the field"																		, sf::Vector2f( rect_HelpBG.getPosition().x + ( TEXT_MARGIN * 2 ), rect_HelpBG.getPosition().y + TEXT_MARGIN + ( float ) ( FONT_SIZE *  7 ) ) );
	initText( text_Randomize			, "R           =>      randomize the field"																		, sf::Vector2f( rect_HelpBG.getPosition().x + ( TEXT_MARGIN * 2 ), rect_HelpBG.getPosition().y + TEXT_MARGIN + ( float ) ( FONT_SIZE *  8 ) ) );
	initText( text_MouseClick			, "Mouse click =>        toggle cell state"																		, sf::Vector2f( rect_HelpBG.getPosition().x + ( TEXT_MARGIN * 2 ), rect_HelpBG.getPosition().y + TEXT_MARGIN + ( float ) ( FONT_SIZE *  9 ) ) );
	initText( text_Separator			, "---------------------------------------"																		, sf::Vector2f( rect_HelpBG.getPosition().x + ( TEXT_MARGIN * 2 ), rect_HelpBG.getPosition().y + TEXT_MARGIN + ( float ) ( FONT_SIZE * 10 ) ) );
	initText( text_AppState				, "Current state:    paused"																					, sf::Vector2f( rect_HelpBG.getPosition().x + ( TEXT_MARGIN * 2 ), rect_HelpBG.getPosition().y + TEXT_MARGIN + ( float ) ( FONT_SIZE * 11 ) ) );
	initText( text_Generation			, "Generation:       " + std::to_string( field->GetGeneration() )												, sf::Vector2f( rect_HelpBG.getPosition().x + ( TEXT_MARGIN * 2 ), rect_HelpBG.getPosition().y + TEXT_MARGIN + ( float ) ( FONT_SIZE * 12 ) ) );
	initText( text_ExecutionDelay		, "Execution delay:  " + std::to_string( executionDelay ) + "ms"												, sf::Vector2f( rect_HelpBG.getPosition().x + ( TEXT_MARGIN * 2 ), rect_HelpBG.getPosition().y + TEXT_MARGIN + ( float ) ( FONT_SIZE * 13 ) ) );
	initText( text_RandomizerValue		, "Randomizer value: 1/" + std::to_string( field->GetRandomizerValue() ) + " (" + GetRandomizerValuePct() + "%)", sf::Vector2f( rect_HelpBG.getPosition().x + ( TEXT_MARGIN * 2 ), rect_HelpBG.getPosition().y + TEXT_MARGIN + ( float ) ( FONT_SIZE * 14 ) ) );
	}

App::~App()
	{
	paused = true;
	sf::sleep( sf::milliseconds( 1 ) );

	closing = true;
	thread_calculation.join();

	if ( appWindow != nullptr )
		{
		if ( appWindow->isOpen() )
			{
			appWindow->close();
			}
		}
	}

void App::Run()
	{
	RandomizeField();
	thread_calculation = std::thread( &App::calculationTask, this );

	while ( appWindow->isOpen() )
		{
		ProcessInputs();
		Tick();
		Render();
		sf::sleep( sf::milliseconds( 1 ) );
		}
	}

void App::ProcessInputs()
	{
	sf::Event event;

	while ( appWindow->pollEvent( event ) )
		{
		if ( event.type == sf::Event::MouseMoved )
			{
			field->SetLocalMousePos( sf::Vector2u( event.mouseMove.x, event.mouseMove.y ) );
			}
		else if ( event.type == sf::Event::MouseButtonPressed )
			{
			field->MouseClicked();
			}
		else if ( event.type == sf::Event::Closed )
			{
			closing = true;
			appWindow->close();
			}
		else if ( event.type == sf::Event::KeyPressed )
			{
			switch ( event.key.code )
				{
				case sf::Keyboard::F1:
					ToggleHelp();
					break;
				case sf::Keyboard::Escape:
					appWindow->close();
					break;
				case sf::Keyboard::Space:
					TogglePause();
					break;
				case sf::Keyboard::R:
					RandomizeField();
					break;
				case sf::Keyboard::Enter:
					if ( paused )
						TickGeneration();
					break;
				case sf::Keyboard::Up:
					SpeedDown();
					break;
				case sf::Keyboard::Down:
					SpeedUp();
					break;
				case sf::Keyboard::Left:
					Randomize();
					break;
				case sf::Keyboard::Right:
					Derandomize();
					break;
				case sf::Keyboard::Delete:
					ClearField();
					break;
				}
			}
		}
	}

void App::Tick()
	{

	}

void App::Render()
	{
	appWindow->clear( color_Background );
	appWindow->draw( *field );
	if ( helpOpened )
		{
		appWindow->draw( rect_HelpBG );
		
		appWindow->draw( text_HelpTitle );
		appWindow->draw( text_DoubleSeparator );
		appWindow->draw( text_Escape );
		appWindow->draw( text_Enter );
		appWindow->draw( text_Space );
		appWindow->draw( text_UpDown );
		appWindow->draw( text_LeftRight );
		appWindow->draw( text_Del );
		appWindow->draw( text_Randomize );
		appWindow->draw( text_MouseClick );
		appWindow->draw( text_Separator );
		appWindow->draw( text_AppState );
		appWindow->draw( text_Generation );
		appWindow->draw( text_ExecutionDelay );
		appWindow->draw( text_RandomizerValue );
		}
	appWindow->display();
	}

void App::TogglePause()
	{
	paused = !paused;
	sf::String stateStr = paused ? "paused" : "running";
	updateText( text_AppState, "Current state:    " + stateStr, sf::Vector2f( rect_HelpBG.getPosition().x + ( TEXT_MARGIN * 2 ), rect_HelpBG.getPosition().y + TEXT_MARGIN + ( float ) ( FONT_SIZE * 11 ) ) );
	}

void App::RandomizeField()
	{
	field->Randomize();
	updateText( text_Generation, "Generation:       0", sf::Vector2f( rect_HelpBG.getPosition().x + ( TEXT_MARGIN * 2 ), rect_HelpBG.getPosition().y + TEXT_MARGIN + ( float ) ( FONT_SIZE * 12 ) ) );

	}

void App::SpeedUp()
	{
	if ( executionDelay > 0 )
		executionDelay -= 10;
	updateText( text_ExecutionDelay, "Execution delay:  " + std::to_string( executionDelay ) + "ms", sf::Vector2f( rect_HelpBG.getPosition().x + ( TEXT_MARGIN * 2 ), rect_HelpBG.getPosition().y + TEXT_MARGIN + ( float ) ( FONT_SIZE * 13 ) ) );
	}

void App::SpeedDown()
	{
	executionDelay += 10;
	updateText( text_ExecutionDelay, "Execution delay:  " + std::to_string( executionDelay ) + "ms", sf::Vector2f( rect_HelpBG.getPosition().x + ( TEXT_MARGIN * 2 ), rect_HelpBG.getPosition().y + TEXT_MARGIN + ( float ) ( FONT_SIZE * 13 ) ) );
	}

void App::Randomize()
	{
	field->SetRandomizerValue( field->GetRandomizerValue() + 1 );
	initText( text_RandomizerValue, "Randomizer value: 1/" + std::to_string( field->GetRandomizerValue() ) + " (" + GetRandomizerValuePct() + "%)", sf::Vector2f( rect_HelpBG.getPosition().x + ( TEXT_MARGIN * 2 ), rect_HelpBG.getPosition().y + TEXT_MARGIN + ( float ) ( FONT_SIZE * 14 ) ) );
	}

void App::Derandomize()
	{
	if( field->GetRandomizerValue() > 1 )
		field->SetRandomizerValue( field->GetRandomizerValue() - 1 );
	initText( text_RandomizerValue, "Randomizer value: 1/" + std::to_string( field->GetRandomizerValue() ) + " (" + GetRandomizerValuePct() + "%)", sf::Vector2f( rect_HelpBG.getPosition().x + ( TEXT_MARGIN * 2 ), rect_HelpBG.getPosition().y + TEXT_MARGIN + ( float ) ( FONT_SIZE * 14 ) ) );
	}

const std::string App::GetRandomizerValuePct() const
	{
	float pct = 100.f / field->GetRandomizerValue();
	std::stringstream ss;
	ss << std::fixed << std::setprecision( 2 ) << pct;
	return ss.str();
	}

void App::SetFPS( unsigned int FPS )
	{
	appWindow->setFramerateLimit( FPS );
	}

void App::TickGeneration()
	{
	if ( !field->IsStable() )
		{
		field->TickGeneration();
		initText( text_Generation, "Generation:       " + std::to_string( field->GetGeneration() ), sf::Vector2f( rect_HelpBG.getPosition().x + ( TEXT_MARGIN * 2 ), rect_HelpBG.getPosition().y + TEXT_MARGIN + ( float ) ( FONT_SIZE * 12 ) ) );
		}
	else
		{
		initText( text_Generation, "Generation:       " + std::to_string( field->GetGeneration() ) + " (done)", sf::Vector2f( rect_HelpBG.getPosition().x + ( TEXT_MARGIN * 2 ), rect_HelpBG.getPosition().y + TEXT_MARGIN + ( float ) ( FONT_SIZE * 12 ) ) );
		}
	}

void App::ClearField()
	{
	field->Clear();
	}

void App::ToggleHelp()
	{
	helpOpened = !helpOpened;
	}

void App::initText( sf::Text& textRef, std::string text, sf::Vector2f pos )
	{
	textRef.setFont( font_app );
	textRef.setString( text );
	textRef.setCharacterSize( FONT_SIZE );
	textRef.setFillColor( color_Alive );
	textRef.setPosition( pos.x, pos.y );
	}

void App::updateText( sf::Text& textRef, std::string text, sf::Vector2f pos )
	{
	textRef.setString( text );
	textRef.setPosition( pos.x, pos.y );
	}

void App::calculationTask()
	{
	while ( !closing )
		{
		while ( !paused )
			{
			mu_lock.lock();
			sf::sleep( sf::milliseconds( executionDelay ) );
			TickGeneration();
			mu_lock.unlock();
			}
		sf::sleep( sf::milliseconds( 10 ) );
		}
	return;
	}