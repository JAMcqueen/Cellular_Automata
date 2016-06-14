/*****************************************************************************
** Program Filename: DoodleBug.hpp
** Author: Joshua McQueen
** Date: 13MAR14
** Description: interface/header file for class DoodleBug
** Input: none
** Output: none
*****************************************************************************/

#ifndef DBUG2_H_JAM_CS
#define DBUG2_H_JAM_CS

#include "Organism.hpp"

namespace JAM_CS
{
	class DoodleBug : public Organism
	{
	public:
		DoodleBug();
		DoodleBug(int x, int y, vector< vector< Organism*> > *grid,
			Organism *prev, Organism *next, int breed_turns, 
			string dbug_species, string dbug_condition, char dbug_graphic, 
			int starve_turns, string prey);
		virtual void move();
		virtual void breed(Organism *&organism_list_head);
		bool hunt();
	protected:
		string prey;
		int food_counter;
		int starve_time;
	private:
		// left blank
	};
}

#endif