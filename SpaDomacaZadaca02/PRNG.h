#pragma once

#include <random>

class PRNG
	{
	public:
		template <typename T>
		T Rand( T from, T to )
			{
			std::uniform_int_distribution<T> d( from, to );
			return d( mt );
			}

		template <typename T>
		void Seed( T seed )
			{
			mt.seed( seed );
			}

	private:
		std::mt19937 mt;
	};
