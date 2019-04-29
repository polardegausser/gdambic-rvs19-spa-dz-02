#pragma once

#include "SFML.h"
#include "Randomizer.h"

class Field : public sf::Drawable
	{
	public:
		Field( unsigned int fieldWidth, unsigned int fieldHeight, const sf::Vector2f& fieldPos, unsigned long long randomizerValue, float cellSize, float cellGap, const sf::Color& aliveColor, const sf::Color& deadColor, const sf::Color& hoverColor );
		
		void				draw						( sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default )	const;
		void				SetPos						( const sf::Vector2f& pos );
		const sf::Vector2f& GetPos						()																					const;
		const sf::Vector2u	GetSize						()																					const;
		void				Randomize					();
		void				Clear						();
		void				TickGeneration				();
		void				SetCellSize					( float cellSize );
		float				GetCellSize					()																					const;
		void				SetCellGap					( float cellGap );
		float				GetCellGap					()																					const;
		void				SetLocalMousePos			( const sf::Vector2u& localMousePos );
		void				MouseClicked				();
		unsigned long long	GetGeneration				()																					const;
		void				SetAliveColor				( const sf::Color& aliveColor );
		const sf::Color&	GetAliveColor				()																					const;
		void				SetDeadColor				( const sf::Color& deadColor );
		const sf::Color&	GetDeadColor				()																					const;
		void				SetHoverColor				( const sf::Color& hoverColor );
		const sf::Color&	GetHoverColor				()																					const;
		bool				IsHoveredCell				()																					const;
		const sf::Vector2u& GetHoveredCellCoordinates	()																					const;
		void				SetRandomizerValue			( unsigned long long randomizerValue );
		unsigned long long	GetRandomizerValue			()																					const;
		bool				IsStable() const;

	protected:
		unsigned int		GetAliveNeighbours			( const std::vector<std::vector<bool>>& field, unsigned int x, unsigned int y )		const;
		void				UpdateVertices				();

	private:
		std::vector<std::vector<bool>> 
			field;
		sf::RectangleShape 
			hoveredCellRect;
		sf::VertexArray 
			vertices;
		sf::Vector2f 
			pos;
		bool 
			hoveredOnCell, 
			stable;
		PRNG 
			randomizer;
		unsigned long long 
			generation, 
			randomizerValue;
		float 
			cellSize, 
			cellGap, 
			cellTotalSize;
		sf::Color 
			aliveColor, 
			deadColor;
		sf::Vector2u 
			hoveredCellCoordinates;
	};