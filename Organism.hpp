/*****************************************************************************
** Program Filename: Organism.hpp
** Author: Joshua McQueen
** Date: 13MAR14
** Description: interface/header file for class Organism
** Input: none
** Output: none
*****************************************************************************/

#ifndef ORGANISM2_H_JAM_CS
#define ORGANISM2_H_JAM_CS

enum xcoords { x_up = 0, x_right = 1, x_down = 0, x_left = -1 };
enum ycoords { y_up = -1, y_right = 0, y_down = 1, y_left = 0 };

#define DIRECTIONS 4
#define CENTER -1

#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cctype>

using std::string;
using std::vector;
using std::rand;
using std::srand;
using std::tolower;
using std::toupper;

namespace JAM_CS
{
	class Organism
	{
	public:
		Organism();
		Organism(int x, int y, vector< vector< Organism*> > *grid,
			Organism *prev, Organism *next);

		virtual void move();
		virtual void breed(Organism *&organism_list_head);

		char get_graphic(){ return graphic; }
		string get_species(){ return species; }
		string get_condition(){ return condition; }

		void set_condition(string new_con) { condition = new_con; }
		
		Organism* get_prev_link(){ return prev_link; }
		Organism* get_next_link(){ return next_link; }

		void set_prev_link(Organism *link){ prev_link = link; }
		void set_next_link(Organism *link){ next_link = link; }

		Organism* insert_at_head(Organism *&organism_list_head,
			Organism *&insert_organism);

	protected:
		int x_directions[4];
		int y_directions[4];
		int turn_counter;
		int breed_time;
		int x_loc;
		int y_loc;

		string species;
		string condition;

		char graphic;

		vector< vector< Organism*> > *org_grid;
		void initialize_directions();

		int check_surroundings(string target);
		bool check_freespace(int x, int y, string target);

	private:
		Organism *prev_link;
		Organism *next_link;
	};
}
#endif