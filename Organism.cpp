/*****************************************************************************
** Program Filename: Organism.cpp
** Author: Joshua McQueen
** Date: 13MAR14
** Description: implementation file for class Organism
** Input: none
** Output: none
*****************************************************************************/

#include "Organism.hpp"

namespace JAM_CS
{
	/*************************************************************************
	** Function: Organism::Organism()
	** Description: default base constructor
	** Parameters: none
	** Pre-Conditions: none
	** Post-Conditions: incompletely initializes object to nonfunctional state
	*************************************************************************/
	Organism::Organism()
	{
		x_loc = 1;
		y_loc = 1;

		org_grid = NULL;
		prev_link = NULL;
		next_link = NULL;
	}

	/*************************************************************************
	** Function: Organism::Organism(int x, int y, 
	**		vector< vector< Organism*> > *grid, Organism *prev, Organism *next)
	** Description: main base constructor
	** Parameters:
	**		int x: x grid coordinate
	**		int y: y grid coordinate
	**		vector<vector<Organism*>> *grid: 2D grid to put Organisms in
	**		Organism *next: link to previous Organism in list
	**		Organism *prev: link to next Organism in list
	** Pre-Conditions: none
	** Post-Conditions: intializes base class Organism to argument values
	*************************************************************************/
	Organism::Organism(int x, int y, vector< vector< Organism*> > *grid,
		Organism *prev, Organism *next)
	{
		x_loc = x;
		y_loc = y;

		org_grid = grid;
		prev_link = prev;
		next_link = next;
		turn_counter = 0;
		initialize_directions();
	}

	/*************************************************************************
	** Function: void Organism::move()
	** Description: searches and moves to an empty adjacent space
	** Parameters: none
	** Pre-Conditions: none
	** Post-Conditions:
	**		attempts to move to random empty adjacent space, if none are
	**		available, it will stay where it is
	*************************************************************************/
	void Organism::move()
	{
		if (condition != "dead")
		{
			int direction = check_surroundings("empty");
			int old_x = x_loc;
			int old_y = y_loc;

			if (condition == "neo")
			{
				condition = "normal";
				graphic = toupper(graphic);
			}

			if (direction != CENTER)
			{
				x_loc = (x_loc + x_directions[direction]);
				y_loc = (y_loc + y_directions[direction]);

				org_grid->at(y_loc).at(x_loc) = this;
				org_grid->at(old_y).at(old_x) = NULL;
			}
			turn_counter++;
		}
	}

	/*************************************************************************
	** Function: int Organism::check_surroundings(string target)
	** Description: searches adjacent spaces for named target
	** Parameters: string target: name of thing to look for
	** Pre-Conditions: none
	** Post-Conditions: returns direction to move in if target is found
	*************************************************************************/
	int Organism::check_surroundings(string target)
	{
		vector<int> free_spaces;

		int direction_to_move;

		// check adjacent spaces; if one is open, add that direction
		// to a temporary vector for possible choices of movement directions
		for (int direction = 0; direction < DIRECTIONS; direction++)
		{
			if (check_freespace((x_loc + x_directions[direction]),
				(y_loc + y_directions[direction]), target))
			{
				free_spaces.push_back(direction);
			}
		}

		// if there was at least 1 adjacent open space, randomly choose 
		// the direction to move to an open space
		if (free_spaces.size() != 0)
		{
			srand(time(NULL) + rand());
			direction_to_move = (rand() % free_spaces.size());
		}
		else // otherwise, don't move 
		{
			free_spaces.push_back(CENTER);
			direction_to_move = 0;
		}
		return free_spaces[direction_to_move];
	}

	/*************************************************************************
	** Function: bool Organism::check_freespace(int x, int y, string target)
	** Description: checks one particular space for target
	** Parameters:
	**		int x: x grid coordinate to check
	**		int y: y grid coordinate to check
	**		string target: name of target to search for
	** Pre-Conditions: none
	** Post-Conditions: 
	**		if target is "empty", searches for an empty space, otherwise
	**		searches for an Organism species; returns a true if it finds
	**		its target, false if not
	*************************************************************************/
	bool Organism::check_freespace(int x, int y, string target)
	{
		int num_rows = static_cast<int>(org_grid->size());
		int num_cols = static_cast<int>(org_grid->at(0).size());

		if ((y < 0) || (y >= num_rows)){ return false; }
		else if ((x < 0) || (x >= num_cols)){ return false; }

		if (target == "empty")
		{
			if (org_grid->at(y).at(x) == NULL){ return true; }
			else{ return false; }
		}
		else
		{
			if (org_grid->at(y).at(x) != NULL)
			{
				if (org_grid->at(y).at(x)->get_species() == target)
				{ return true; }
				else 
				{ return false; }
			}
			else
			{ return false;	}
		}
	}

	/*************************************************************************
	** Function: void Organism::breed(Organism *&organism_list_head)
	** Description: placeholder virtual function for breeding behavior
	** Parameters:
	**		Organism *&organism_list_head: doubly linked list that object will
	**		be in
	** Pre-Conditions: none
	** Post-Conditions: none
	*************************************************************************/
	void Organism::breed(Organism *&organism_list_head)
	{
		// intentionally left blank
	}

	/*************************************************************************
	** Function: void Organism::initialze_directions()
	** Description: intializes the direction array for Organism
	** Parameters: none
	** Pre-Conditions: none
	** Post-Conditions:
	**		direction array is filled with values representing coordinate
	**		offsets in a grid for navigation and movement
	*************************************************************************/
	void Organism::initialize_directions()
	{
		x_directions[0] = x_up;
		x_directions[1] = x_right;
		x_directions[2] = x_down;
		x_directions[3] = x_left;

		y_directions[0] = y_up;
		y_directions[1] = y_right;
		y_directions[2] = y_down;
		y_directions[3] = y_left;	
	}

	/*************************************************************************
	** Function: Organism* Organism::insert_at_head( 
	**		Organism *&organism_list_head, Organism *new_organism)
	** Description: inserts Organism at beginning of doubly linked list
	** Parameters:
	**		Organism *&organism_list_head: head of doubly linked list
	**		Organism *&insert_organism: Organism to add
	** Pre-Conditions: none
	** Post-Conditions: 
	**		Organism is added at the beginning of list and returns pointer
	**		to Object added
	*************************************************************************/
	Organism* Organism::insert_at_head(Organism *&organism_list_head, 
		Organism *&insert_organism)
	{
		// set new organism's next link to what the head points to
		insert_organism->set_next_link(organism_list_head);
		insert_organism->set_prev_link(NULL);
	
		// set the prev_link of what the head previously pointed to
		// to new organism
		if (insert_organism->get_next_link() != NULL)
		{
			insert_organism->get_next_link()->set_prev_link(insert_organism);
		}

		// point head to new organism
		organism_list_head = insert_organism;

		return insert_organism;
	}
}