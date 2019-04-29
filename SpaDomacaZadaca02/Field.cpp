#include "Field.h"

Field::Field(unsigned int fieldWidth, unsigned int fieldHeight, const sf::Vector2f& fieldPos, unsigned long long randomizerValue, float cellSize, float cellGap, const sf::Color& aliveColor, const sf::Color& deadColor, const sf::Color& hoverColor )
	: pos(fieldPos), randomizerValue(randomizerValue), cellSize(cellSize), cellGap(cellGap), aliveColor(aliveColor), deadColor(deadColor)
	{
	field = std::vector<std::vector<bool>>(fieldWidth, std::vector<bool>(fieldHeight));

	if ( cellSize > 1.f )
		{
		vertices = sf::VertexArray( sf::PrimitiveType::Quads, fieldWidth * fieldHeight * 4 );
		}
	else
		{
		vertices = sf::VertexArray( sf::PrimitiveType::Points, fieldWidth * fieldHeight );
		}

	cellTotalSize = cellSize + cellGap;
	hoveredCellRect = sf::RectangleShape( sf::Vector2f( cellSize, cellSize ) );
	hoveredCellRect.setFillColor( hoverColor );
	generation = 0;
	stable = false;

	randomizer.Seed( ( unsigned long ) time( nullptr ) );

	UpdateVertices();
	}

void Field::draw( sf::RenderTarget& target, sf::RenderStates states ) const
	{
	target.draw(vertices, states);

	if ( hoveredOnCell )
		{
		target.draw( hoveredCellRect, states );
		}
	}

void Field::SetPos( const sf::Vector2f& pos )
	{
	this->pos = pos;
	}

const sf::Vector2f& Field::GetPos() const
	{
	return pos;
	}

const sf::Vector2u Field::GetSize() const
	{
	if ( field.size() > 0 )
		{
		return sf::Vector2u( ( unsigned int ) field.size(), ( unsigned int ) field[ 0 ].size() );
		}
	return sf::Vector2u( 0, 0 );
	}

void Field::Randomize()
	{
	sf::Vector2u fieldSize = GetSize();

	for ( unsigned int x = 0; x < fieldSize.x; x++ )
		{
		for ( unsigned int y = 0; y < fieldSize.y; y++ )
			{
			bool alive = randomizer.Rand<unsigned long long>( 1, randomizerValue ) == 1;
			field[ x ][ y ] = alive;
			}
		}

	generation = 0;
	stable = false;
	UpdateVertices();
	}

void Field::Clear()
	{
	for ( auto &column : field )
		{
		std::fill( column.begin(), column.end(), false );
		}

	generation = 0;
	stable = false;
	UpdateVertices();
	}

void Field::TickGeneration()
	{
	if ( !stable )
		{
		bool changed = false;
		std::vector<std::vector<bool>> tmpField( field );

		for ( unsigned int x = 0; x < tmpField.size(); x++ )
			{
			for ( unsigned int y = 0; y < tmpField[0].size(); y++ )
				{
				int aliveNeighbours = GetAliveNeighbours( tmpField, x, y );
				if ( field[ x ][ y ] && ( aliveNeighbours < 2 || aliveNeighbours > 3 ) )
					{
					field[ x ][ y ] = false;
					changed = true;
					}
				else if ( !field[ x ][ y ] && aliveNeighbours == 3 )
					{
					field[ x ][ y ] = true;
					changed = true;
					}
				}
			}
		stable = !changed;
		if ( !stable )
			{
			generation++;
			UpdateVertices();
			}
		}
	}

void Field::SetCellSize( float cellSize )
	{
	this->cellSize = cellSize;
	cellTotalSize = cellSize + cellGap;
	hoveredCellRect.setSize( sf::Vector2f( cellSize, cellSize ) );
	UpdateVertices();
	}

float Field::GetCellSize() const
	{
	return cellSize;
	}

void Field::SetCellGap( float cellGap )
	{
	this->cellGap = cellGap;
	cellTotalSize = cellGap + cellSize;
	UpdateVertices();
	}

float Field::GetCellGap() const
	{
	return cellGap;
	}

void Field::SetLocalMousePos( const sf::Vector2u& localMousePos )
	{
	unsigned int relX = ( unsigned int ) ( localMousePos.x - pos.x );
	unsigned int relY = ( unsigned int ) ( localMousePos.y - pos.y );

	unsigned int fieldSizeX = ( unsigned int ) ( field.size() * cellTotalSize );
	unsigned int fieldSizeY = ( unsigned int ) ( field[ 0 ].size() * cellTotalSize );

	hoveredOnCell = false;
	if ( relX < fieldSizeX && relX >= 0 && relY < fieldSizeY && relY >= 0 )
		{
		if ( relX % ( unsigned int ) cellTotalSize < cellSize && relY % ( unsigned int ) cellTotalSize < cellSize )
			{
			hoveredCellCoordinates = sf::Vector2u( ( unsigned int ) ( relX / cellTotalSize ), ( unsigned int ) ( relY / cellTotalSize ) );
			hoveredCellRect.setPosition( hoveredCellCoordinates.x * cellTotalSize + pos.x, hoveredCellCoordinates.y * cellTotalSize + pos.y );
			hoveredOnCell = true;
			}
		}
	}

void Field::MouseClicked()
	{
	if ( hoveredOnCell )
		{
		field[ hoveredCellCoordinates.x ][ hoveredCellCoordinates.y ] = !field[ hoveredCellCoordinates.x ][ hoveredCellCoordinates.y ];
		stable = false;
		UpdateVertices();
		}
	}

unsigned long long Field::GetGeneration() const
	{
	return generation;
	}

void Field::SetAliveColor( const sf::Color& aliveColor )
	{
	this->aliveColor = aliveColor;
	UpdateVertices();
	}

const sf::Color& Field::GetAliveColor() const
	{
	return aliveColor;
	}

void Field::SetDeadColor( const sf::Color& deadColor )
	{
	this->deadColor = deadColor;
	UpdateVertices();
	}

const sf::Color& Field::GetDeadColor() const
	{
	return deadColor;
	}

void Field::SetHoverColor( const sf::Color& hoverColor )
	{
	hoveredCellRect.setFillColor( hoverColor );
	}

const sf::Color& Field::GetHoverColor() const
	{
	return hoveredCellRect.getFillColor();
	}

bool Field::IsHoveredCell() const
	{
	return hoveredOnCell;
	}

const sf::Vector2u& Field::GetHoveredCellCoordinates() const
	{
	return hoveredCellCoordinates;
	}

void Field::SetRandomizerValue( unsigned long long randomizerValue )
	{
	this->randomizerValue = randomizerValue;
	}

unsigned long long Field::GetRandomizerValue() const
	{
	return randomizerValue;
	}

bool Field::IsStable() const
	{
	return stable;
	}

unsigned int Field::GetAliveNeighbours( const std::vector<std::vector<bool>>& field, unsigned int x, unsigned int y ) const
	{
	int aliveNeighbours = 0;
	int offsets[ 8 ][ 2 ] = { { -1, -1 }, { 0, -1 }, { 1, -1 }, { -1, 0 }, { 1, 0 }, { -1, 1 }, { 0, 1 }, { 1, 1 } };

	for ( const auto& offset : offsets )
		{
		int xOfNeighbour = x + offset[ 0 ];
		int yOfNeighbour = y + offset[ 1 ];

		if ( xOfNeighbour >= 0 && yOfNeighbour >= 0 && xOfNeighbour < field.size() && yOfNeighbour < field[ 0 ].size() )
			{
			if ( field[ xOfNeighbour ][ yOfNeighbour ] )
				aliveNeighbours++;
			}
		}
	return aliveNeighbours;
	}

void Field::UpdateVertices()
	{
	size_t i = 0;

	for ( unsigned int x = 0; x < field.size(); x++ )
		{
		for ( unsigned int y = 0; y < field[ 0 ].size(); y++ )
			{
			float currX = pos.x + x * cellTotalSize;
			float currY = pos.y + y * cellTotalSize;

			if ( cellSize > 1.f )
				{
				sf::Vertex* quadOffset = &vertices[ i * 4 ];

				for ( unsigned int j = 0; j < 4; j++ )
					{
					if ( field[ x ][ y ] )
						{
						quadOffset[ j ].color = aliveColor;
						}
					else
						{
						quadOffset[ j ].color = deadColor;
						}
					}

				quadOffset[ 0 ].position = sf::Vector2f( currX, currY );
				quadOffset[ 1 ].position = sf::Vector2f( currX + cellSize, currY );
				quadOffset[ 2 ].position = sf::Vector2f( currX + cellSize, currY + cellSize );
				quadOffset[ 3 ].position = sf::Vector2f( currX, currY + cellSize );
				}
			else
				{
				sf::Vertex* vertex = &vertices[ i ];

				if ( field[ x ][ y ] )
					{
					vertex->color = aliveColor;
					}
				else
					{
					vertex->color = deadColor;
					}
				}
			i++;
			}
		}
	}