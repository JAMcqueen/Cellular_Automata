/*****************************************************************************
** Program Filename: DoodleBug.cpp
** Author: Joshua McQueen
** Date: 13MAR14
** Description: implementation file for class DoodleBug
** Input: none
** Output: none
*****************************************************************************/

#include "DoodleBug.hpp"

namespace JAM_CS
{
	/*************************************************************************
	** Function: DoodleBug::DoodleBug() : Organism()
	** Description: default constructor
	** Parameters: none
	** Pre-Conditions: none
	** Post-Conditions: incompletely initializes object to nonfunctional state
	*************************************************************************/
	DoodleBug::DoodleBug() : Organism()
	{
		breed_time = 8;
		species = "DoodleBug";
		condition = "neo";
		graphic = 'X';
		starve_time = 3;
		food_counter = 0;
		turn_counter = 0;
	}

	/*************************************************************************
	** Function: DoodleBug::DoodleBug(int x, int y, 
	**		vector< vector< Organism*> > *grid, Organism *prev, Organism *next, 
	**		int breed_turns, string dbug_species, string dbug_condition, 
	**		char dbug_graphic, int starve_turns, string prey) : 
	**		Organism(x, y, grid, prev, next)
	** Description: main constructor (calls Organism main constructor)
	** Parameters:
	**		int x: x grid coordinate
	**		int y: y grid coordinate
	**		vector<vector<Organism*>> *grid: 2D grid to put DoodleBugs in
	**		Organism *next: link to previous DoodleBug in list
	**		Organism *prev: link to next DoodleBug in list
	**		int breed_turns: number of turns to wait before breeding
	**		string dbug_species: species name to call this DoodleBug
	**		string dbug_condition: condition of DoodleBug
	**		char dbug_graphic: character used to represent DoodleBug on screen
	**		int starve_turns: number of turns it takes for a DoodleBug to
	**			die if it hasn't eaten in that time
	**		string bug_prey: name of species DoodleBug will hunt
	** Pre-Conditions: none
	** Post-Conditions: object is initialized to argument values
	*************************************************************************/
	DoodleBug::DoodleBug(int x, int y, vector< vector< Organism*> > *grid,
		Organism *prev, Organism *next, int breed_turns,
		string dbug_species, string dbug_condition, char dbug_graphic,
		int starve_turns, string bug_prey) 
		: Organism(x, y, grid, prev, next)
	{
		breed_time = breed_turns;
		species = dbug_species;
		condition = dbug_condition;
		graphic = dbug_graphic;
		starve_time = starve_turns;
		food_counter = starve_turns;
		prey = bug_prey;
	}

	/*************************************************************************
	** Function: void DoodleBug::move()
	** Description: executes hunting behavior/movement for DoodleBug in grid
	** Parameters: none
	** Pre-Conditions: none
	** Post-Conditions:
	**		attemps to hunt, if nothing is found, it moves to an adjacent
	**		free space. if it hasn't eaten in so many turns it flags itself
	**		to be removed from the grid
	*************************************************************************/
	void DoodleBug::move()
	{
		if (condition != "dead")
		{
			bool found_prey = false;

			if (condition == "neo")
			{
				condition = "normal";
				graphic = toupper(graphic);
			}
			else if (condition == "fed")
			{
				condition = "normal";
			}

			found_prey = hunt();

			if (!found_prey)
			{
				Organism::move();
				food_counter--;
			}
			else
			{
				food_counter = starve_time;
				turn_counter++;
			}

			if (food_counter == 1)
			{
				condition = "starving";
			}
			else if (food_counter == 0)
			{
				condition = "dying";
			}
		}
	}

	/*************************************************************************
	** Function: bool DoodleBug::hunt()
	** Description:
	**		searchs for prey in adjacent spaces, if found it will move to that
	**		space and flag the prey object that was there to be removed
	** Parameters: none
	** Pre-Conditions: none
	** Post-Conditions:
	**		returns true and moves if prey is found, if not, just returns
	**		false
	*************************************************************************/
	bool DoodleBug::hunt()
	{
		int direction = check_surroundings(prey);
		int old_x = x_loc;
		int old_y = y_loc;
		bool found_prey = false;

		if (direction != CENTER)
		{
			x_loc = (x_loc + x_directions[direction]);
			y_loc = (y_loc + y_directions[direction]);
			found_prey = true;
			condition = "fed";

			org_grid->at(y_loc).at(x_loc)->set_condition("dead");
			org_grid->at(y_loc).at(x_loc) = this;
			org_grid->at(old_y).at(old_x) = NULL;
		}
		return found_prey;
	}

	/*************************************************************************
	** Function: DoodleBug::breed(Organism *&organism_list_head)
	** Description:
	**		creates a new DoodleBug object in free adjacent space if a certain
	**		number of turns have passed
	** Parameters:
	**		Organism *&organism_list_head: head of DoodleBug doubly linked list
	** Pre-Conditions: none
	** Post-Conditions: attempts to breed DoodleBug object
	*************************************************************************/
	void DoodleBug::breed(Organism *&organism_list_head)
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

					Organism *temp_bug = new DoodleBug(new_x,
						new_y, org_grid, NULL, NULL, breed_time, species,
						"neo", tolower(graphic), starve_time, prey);

					org_grid->at(new_y).at(new_x) = temp_bug;
					insert_at_head(organism_list_head, temp_bug);
				}
				turn_counter = 0;
			}

			if (condition == "dying")
			{
				condition = "dead";
				org_grid->at(y_loc).at(x_loc) = NULL;
			}
		}
	}
}