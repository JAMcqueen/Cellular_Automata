/*****************************************************************************
** Program Filename: OrganismGrid.hpp
** Author: Joshua McQueen
** Date: 13MAR14
** Description: interface/header file for class OrganismGrid
** Input: none
** Output: none
*****************************************************************************/

#ifndef ORGANISM_GRID_H_JAM_CS
#define ORGANISM_GRID_H_JAM_CS

#define ANT_BREED_TURN 3 // 3
#define ANT_SPECIES_NAME "ant"
#define ANT_GRAPHIC 'O'

#define BUG_BREED_TURN 8 // 8
#define BUG_SPECIES_NAME "doodlebug"
#define BUG_GRAPHIC 'X'
#define BUG_STARVE_TURN 3 // 3
#define BUG_PREY_NAME "ant"

#define BG_CHAR '-'

#define SIDE_MENU_X_POS 21

enum colors { cyan_black = 1, green_black = 2, blue_black = 3,
	yellow_black = 4, white_black = 5, red_black = 6, magenta_black = 7};

#include "Ant.hpp"
#include "DoodleBug.hpp"

#include <curses.h>
#include <cstring>

using JAM_CS::Ant;
using JAM_CS::DoodleBug;

using std::strlen;

namespace JAM_CS
{
	class OrganismGrid
	{
	public:
		Organism* LifeListHead;

		vector< vector<Organism*> > *grid_pntr;
		vector< vector<Organism*> > organism_grid;

		OrganismGrid();
		OrganismGrid(int max_cols, int max_rows, int ants, int bugs);

		~OrganismGrid();

		void run_simulation();
		void draw_grid();

		int get_max_cols(){ return max_columns; }
		int get_max_rows(){ return max_rows; }
	private:
		int max_columns;
		int max_rows;

		int ant_count;
		int new_ant_count;
		int bug_count;
		int new_bug_count;
		int fed_bug_count;
		int starving_bug_count;
		int total_count;

		int turn_count;
		bool auto_movement; // FALSE if you want automove
		bool turn_off_auto;

		void draw_x_axis(char alphabet_chars[], int alpha_len);
		int get_color(string org_condition, char org_graphic);
		void draw_color_char(int char_color, char print_char);
		void print_color_str(int str_color, string print_str);
		void draw_side_menu();
		
		void randomize_order(Organism *&LifeListHead);
		Organism*& move_random_organism(Organism *&list_iterator,
			Organism *&list_head, int &list_size);
		void move_organisms(Organism *organism_list_head);
		Organism* remove_organism(Organism *target, Organism *&organism_list_head);
		void breed_organisms(Organism *&org_pntr);
		void get_input();
		void remove_dead(Organism *&organism_list_head);

		void make_grid();
		void add_creatures(int num_ants, int num_bugs);
		void initialize_curse_stuff();
		void end_simulation();
		void clean_up();
	};
}
#endif