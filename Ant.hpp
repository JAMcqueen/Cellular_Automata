/*****************************************************************************
** Program Filename: Ant.hpp
** Author: Joshua McQueen
** Date: 13MAR14
** Description: interface/header file for class Ant
** Input: none
** Output: none
*****************************************************************************/

#ifndef ANT2_H_JAM_CS
#define ANT2_H_JAM_CS

#include "Organism.hpp"

namespace JAM_CS
{
	class Ant : public Organism
	{
	public:
		Ant();
		Ant(int x, int y, vector< vector< Organism*> > *grid, Organism *prev, 
			Organism *next, int breed_turns, string ant_species, 
			string ant_condition, char ant_graphic);
		virtual void breed(Organism *&organism_list_head);
	};
}
#endif