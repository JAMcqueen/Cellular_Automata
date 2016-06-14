/*****************************************************************************
** Program Filename: Ant.cpp
** Author: Joshua McQueen
** Date: 13MAR14
** Description: implementation file for class Ant
** Input: none
** Output: none
*****************************************************************************/

#include "Ant.hpp"

namespace JAM_CS
{
	/*************************************************************************
	** Function: Ant::Ant() : Organism()
	** Description: default constructor
	** Parameters: none
	** Pre-Conditions: none
	** Post-Conditions: incompletely initializes object to nonfunctional state
	*************************************************************************/
	Ant::Ant() : Organism()
	{
		breed_time = 3;
		species = "Ant";
		condition = "neo";
		graphic = 'O';
		turn_counter = 0;
	}

	/*************************************************************************
	** Function: Ant::Ant(int x, int y, vector< vector< Organism*> > *grid, 
	**		Organism *prev, Organism *next, int breed_turns, 
	**		string ant_species,	string ant_condition, char ant_graphic) : 
	**		Organism(x, y, grid,  prev, next)
	** Description: main constructor (calls Organism main constructor)
	** Parameters:
	**		int x: x grid coordinate
	**		int y: y grid coordinate
	**		vector<vector<Organism*>> *grid: 2D grid to put Ants in
	**		Organism *next: link to previous Ant in list
	**		Organism *prev: link to next Ant in list
	**		int breed_turns: number of turns to wait before breeding
	**		string ant_species: species name to call this Ant
	**		string ant_condition: condition of Ant
	**		char ant_graphic: character used to represent Ant on screen
	** Pre-Conditions: none
	** Post-Conditions: object is initialized to argument values
	*************************************************************************/
	Ant::Ant(int x, int y, vector< vector< Organism*> > *grid, Organism *prev,
		Organism *next, int breed_turns, string ant_species,
		string ant_condition, char ant_graphic)
		 : Organism(x, y, grid, prev, next)
	{
		breed_time = breed_turns;
		species = ant_species;
		condition = ant_condition;
		graphic = ant_graphic;
	}

	/*************************************************************************
	** Function: void Ant::breed(Organism *&organism_list_head)
	** Description: 
	**		creates a new Ant object in free adjacent space if a certain
	**		number of turns have passed
	** Parameters:
	**		Organism *&organism_list_head: head of Ant doubly linked list
	** Pre-Conditions: none
	** Post-Conditions: attempts to breed ant object
	*************************************************************************/
	void Ant::breed(Organism *&organism_list_head)
	{
		if (condition != "dead")
		{
			if (turn_counter == breed_time)
			{
				int direction = check_surroundings("empty");
				int new_x = 0;
				int new_y = 0;

				if (direction != CENTER)
				{
					new_x = (x_loc + x_directions[direction]);
					new_y = (y_loc + y_directions[direction]);

					Organism *temp_ant = new Ant(new_x,
						new_y, org_grid, NULL, NULL, breed_time, species,
						"neo", tolower(graphic));

					org_grid->at(new_y).at(new_x) = temp_ant;
					insert_at_head(organism_list_head, temp_ant);
				}
				turn_counter = 0;
			}
		}
	}
}