/*****************************************************************************
** Program Filename: OrganismGrid.cpp
** Author: Joshua McQueen
** Date: 13MAR14
** Description: implementation file for class OrganismGrid
** Input: none
** Output: none
*****************************************************************************/

#include "OrganismGrid.hpp"

namespace JAM_CS
{
	/*************************************************************************
	** Function: OrganismGrid::OrganismGrid()
	** Description: default constructor
	** Parameters: none
	** Pre-Conditions: none
	** Post-Conditions: initializes "empty" OrganismGrid object
	*************************************************************************/
	OrganismGrid::OrganismGrid()
	{
		max_columns = 20;
		max_rows = 20;

		ant_count = 0;
		new_ant_count = 0;
		bug_count = 0;
		new_bug_count = 0;
		fed_bug_count = 0;
		starving_bug_count = 0;
		turn_count = 0;

		LifeListHead = NULL;
		grid_pntr = NULL; 

		initialize_curse_stuff();
	}

	/*************************************************************************
	** Function: OrganismGrid::OrganismGrid(int max_cols, int row_max, 
	**		int ants, int bugs)
	** Description: main constructor
	** Parameters:
	**		int max_cols: maximum number of columns in grid
	**		int row_max: maximum number of rows in grid
	**		int ants: maximum number of Ants in grid
	**		int bugs: maximum number of DoodleBugs in grid
	** Pre-Conditions: none
	** Post-Conditions:
	*************************************************************************/
	OrganismGrid::OrganismGrid(int max_cols, int row_max, int ants, int bugs)
	{
		max_columns = max_cols;
		max_rows = row_max;

		ant_count = 0;
		new_ant_count = 0;
		bug_count = 0;
		new_bug_count = 0;
		fed_bug_count = 0;
		starving_bug_count = 0;
		turn_count = 0;

		LifeListHead = NULL;

		initialize_curse_stuff();

		make_grid();
		grid_pntr = &organism_grid; 
		add_creatures(ants, bugs);
	}

	/*************************************************************************
	** Function: OrganismGrid::~OrganismGrid()
	** Description: destructor
	** Parameters: none
	** Pre-Conditions: none
	** Post-Conditions: frees dynamically allocated memory
	*************************************************************************/
	OrganismGrid::~OrganismGrid()
	{
		clean_up();
	}

	/*************************************************************************
	** Function: void OrganismGrid::run_simulation()
	** Description: main loop for program
	** Parameters: none
	** Pre-Conditions: none
	** Post-Conditions: 
	**		Moves, breeds, and disposes organisms in sequence. Also draws
	**		the grid and gets user input
	*************************************************************************/
	void OrganismGrid::run_simulation()
	{
		randomize_order(LifeListHead);
		move_organisms(LifeListHead);
		breed_organisms(LifeListHead);
		remove_dead(LifeListHead);

		turn_count++;
		draw_grid();
		get_input();
	}

	/*************************************************************************
	** Function: void OrganismGrid::make_grid()
	** Description: 
	**		Initializes the 2D vector of Organism pointers (class member) 
	**		"organism_grid" according to values passed in from the constructor. 
	**		The grid is of size (class members) max_rows * max_columns. Sets
	**		each pointer in the grid to NULL
	** Parameters: none
	** Pre-Conditions: none
	** Post-Conditions: the grid is resized and all elements set to NULL
	*************************************************************************/
	void OrganismGrid::make_grid()
	{
		organism_grid.resize(max_rows);

		for (int row = 0; row < max_rows; row++)
		{
			organism_grid[row].resize(max_columns);

			for (int element = 0; element < max_columns; element++)
			{
				organism_grid[row][element] = NULL;
			}
		}
	}

	/*************************************************************************
	** Function: void OrganismGrid::add_creatures(int num_ants, int num_bugs)
	** Description: adds Ant and DoodleBug objects to the organism grid
	** Parameters:
	**		int ants: maximum number of Ants in grid
	**		int bugs: maximum number of DoodleBugs in grid
	** Pre-Conditions: none
	** Post-Conditions: grid has been initialized already
	*************************************************************************/
	void OrganismGrid::add_creatures(int num_ants, int num_bugs)
	{
		int x = 0;
		int y = 0;
		Organism *last_creature = NULL;

		// add doodlebugs to grid
		for (int bug = 0; bug < num_bugs; bug++)
		{
			srand(time(NULL) + rand());

			x = (rand() % max_columns);
			y = (rand() % max_rows);

			// if the randomly chosen spot is free, add bug
			if (organism_grid[y][x] == NULL)
			{
				if (LifeListHead != NULL)
				{
					last_creature->set_next_link(new DoodleBug(x, y, grid_pntr,
						last_creature, NULL, BUG_BREED_TURN, BUG_SPECIES_NAME, 
						"normal", BUG_GRAPHIC, BUG_STARVE_TURN, 
						BUG_PREY_NAME));

					last_creature = last_creature->get_next_link();
					organism_grid[y][x] = last_creature;
				}
				else
				{
					LifeListHead = new DoodleBug(x, y, grid_pntr, NULL,
						NULL, BUG_BREED_TURN, BUG_SPECIES_NAME, "normal",
						BUG_GRAPHIC, BUG_STARVE_TURN, BUG_PREY_NAME);
					organism_grid[y][x] = LifeListHead;

					last_creature = LifeListHead;
				}
				bug_count++;
			}
			else // repeat iteration again if spot was taken
			{
				bug--;
			}
		}

		//add ants to grid
		for (int ant = 0; ant < num_ants; ant++)
		{
			srand(time(NULL) + rand());

			x = (rand() % max_columns);
			y = (rand() % max_rows);

			// if the randomly chosen spot is free, add ant
			if (organism_grid[y][x] == NULL)
			{
				last_creature->set_next_link(new Ant(x, y, grid_pntr, 
					last_creature, NULL, ANT_BREED_TURN, ANT_SPECIES_NAME,
					"normal", ANT_GRAPHIC));
				last_creature = last_creature->get_next_link();
				organism_grid[y][x] = last_creature;

				ant_count++;
			}
			else // repeat iteration again if spot was taken
			{
				ant--;
			}
		}
		total_count = (ant_count + bug_count);
	}

	/*************************************************************************
	** Function: void OrganismGrid::draw_grid()
	** Description: draws the grid and related information to console screen
	** Parameters: none
	** Pre-Conditions: none
	** Post-Conditions: grid and side menu are drawn to screen
	*************************************************************************/
	void OrganismGrid::draw_grid()
	{
		char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
		char letter = 'a';
		char org_char = '-';

		string org_condition = "";

		int alphabet_length = strlen(alphabet);
		int current_letter = 0;
		int current_row = 0;
		int color = 0;

		ant_count = 0;
		new_ant_count = 0;
		bug_count = 0;
		new_bug_count = 0;
		fed_bug_count = 0;
		starving_bug_count = 0;
		total_count = 0;

		clear();
		addch(' ');
	
		// draw x-axis
		draw_x_axis(alphabet, alphabet_length);
		current_row++;
		move(current_row, 0);
	
		for (int row_pos = 0; row_pos < max_rows; row_pos++)
		{
			// draws y-axis of current row
			letter = alphabet[current_letter];
			addch(letter);
	
			// draws current organism
			for (int col_pos = 0; col_pos < max_columns; col_pos++)
			{
				if (organism_grid[row_pos][col_pos] != NULL)
				{
					org_char = organism_grid[row_pos][col_pos]->get_graphic();
					org_condition =
						organism_grid[row_pos][col_pos]->get_condition();

					color = get_color(org_condition, org_char);
				}
				else
				{
					org_char = BG_CHAR;
					color = white_black;
				}
				draw_color_char(color, org_char);
			}
			current_letter++;
			current_row++;
			move(current_row, 0);
		}
		draw_side_menu();
	}

	/*************************************************************************
	** Function: void OrganismGrid::draw_x_axis(char alphabet_chars[], 
	**		int alpha_len)
	** Description: draws x-axis labels for grid
	** Parameters: 
	**		char alphabet_chars[]: array of chars that contains alphabet
	**		int alpha_len: length of alphabet array
	** Pre-Conditions: none
	** Post-Conditions: x-axis labels of organism grid are drawn to screen
	*************************************************************************/
	void OrganismGrid::draw_x_axis(char alphabet_chars[], int alpha_len)
	{
		char letter = 'a';

		for (int x = 0; x < max_columns; x++)
		{
			if (x > alpha_len)
			{
				letter = alphabet_chars[(alpha_len / 26)];
				addch(letter);

				letter = alphabet_chars[(alpha_len%x)];
				addch(letter);
			}
			else
			{
				letter = alphabet_chars[x];
				addch(letter);
			}
		}
	}

	/*************************************************************************
	** Function: int OrganismGrid::get_color(string org_condition,
	**		char org_graphic)
	** Description: 
	**		returns a value used to determine what color to draw an organism
	**		based on the organism's condition and current graphic
	** Parameters:
	**		string org_condition: condition of organism to check
	**		char org_graphic: graphic of organism to check
	** Pre-Conditions: none
	** Post-Conditions: returns a number for coloring organism graphic
	*************************************************************************/
	int OrganismGrid::get_color(string org_condition, char org_graphic)
	{
		int color_pair = 0;

		if ((org_graphic == 'O')) // adult ants are green
		{
			color_pair = green_black;
			ant_count++;
		}
		else if (org_graphic == 'o') // neo ants are yellow
		{
			color_pair = yellow_black;
			new_ant_count++;
			ant_count++;
		}
		else if (org_graphic == 'X') // adult doodlebugs are cyan
		{
			// a doodlebug that has just eaten is red
			if (org_condition == "fed")
			{
				color_pair = red_black;
				fed_bug_count++;
				bug_count++;
			}
			// a doodlebug that is about to starve to death is magenta
			else if (org_condition == "starving")
			{
				color_pair = magenta_black;
				starving_bug_count++;
				bug_count++;
			}
			else // normal, adult doodlebug
			{
				color_pair = cyan_black;
				bug_count++;
			}
		}
		else if (org_graphic == 'x') // neo doodlebugs are blue
		{
			color_pair = blue_black;
			new_bug_count++;
			bug_count++;
		}
		total_count = (ant_count + bug_count);
		return color_pair;
	}

	/*************************************************************************
	** Function: void OrganismGrid::draw_side_menu()
	** Description: 
	**		draws supplementary data beside the organism grid. contains
	**		information about the # of types of organisms
	** Parameters: none
	** Pre-Conditions: none
	** Post-Conditions: side menu is drawn to screen
	*************************************************************************/
	void OrganismGrid::draw_side_menu()
	{
		int row_num = 1;

		mvprintw(row_num, SIDE_MENU_X_POS, "  TURN: %i", turn_count);
		row_num++;

		move(row_num, SIDE_MENU_X_POS);
		if ((turn_count != 0) && (turn_count % ANT_BREED_TURN == 0))
		{ print_color_str(green_black, "  ANTS BREED"); }
		row_num++;

		move(row_num, SIDE_MENU_X_POS);
		if ((turn_count != 0) && (turn_count % BUG_BREED_TURN == 0))
		{ print_color_str(cyan_black, "  DOODLEBUGS BREED"); }
		row_num++;

		mvprintw(row_num, SIDE_MENU_X_POS, "  # ANTS: %i", ant_count);
		row_num++;

		mvprintw(row_num, SIDE_MENU_X_POS, "  # JUVENILE ANTS: %i", 
			new_ant_count);
		row_num++;

		mvprintw(row_num, SIDE_MENU_X_POS, "  # DOODLEBUGS: %i",
			bug_count);
		row_num++;

		mvprintw(row_num, SIDE_MENU_X_POS, "  # JUVENILE DOODLEBUGS: %i", 
			new_bug_count);
		row_num++;

		mvprintw(row_num, SIDE_MENU_X_POS, "  # FED DOODLEBUGS: %i", 
			fed_bug_count);
		row_num++;

		mvprintw(row_num, SIDE_MENU_X_POS, "  # STARVING DOODLEBUGS: %i", 
			starving_bug_count);
		row_num++;

		move(row_num, SIDE_MENU_X_POS);
		print_color_str(green_black, "  O");
		printw(" is an adult ant");
		row_num++;

		move(row_num, SIDE_MENU_X_POS);
		print_color_str(yellow_black, "  o");
		printw(" is a juvenile ant");
		row_num++;

		move(row_num, SIDE_MENU_X_POS);
		print_color_str(cyan_black, "  X");
		printw(" is an adult doodlebug");
		row_num++;

		move(row_num, SIDE_MENU_X_POS);
		print_color_str(blue_black, "  x");
		printw(" is a juvenile doodlebug");
		row_num++;

		move(row_num, SIDE_MENU_X_POS);
		print_color_str(red_black, "  X");
		printw(" is an adult doodlebug that has just fed");
		row_num++;

		move(row_num, SIDE_MENU_X_POS);
		print_color_str(magenta_black, "  X");
		printw(" is an adult doodlebug that is starving");
		row_num++;

		move(row_num, SIDE_MENU_X_POS);
		printw("  Hit any key to continue to next turn");
		row_num++;

		move(row_num, SIDE_MENU_X_POS);
		printw("  Hit 'q' to quit simulation");
		row_num++;

		if (turn_count == 0)
		{
			move(row_num, SIDE_MENU_X_POS);
			print_color_str(red_black, "  PRESS ANY KEY TO START");
		}
		move(0, 0);
	}

	/*************************************************************************
	** Function: void OrganismGrid::draw_color_char(int char_color, 
	**		char print_char)
	** Description: draws a colored char to screen
	** Parameters:
	**		int char_color: color to draw char
	**		char print_char: what char to draw
	** Pre-Conditions: none
	** Post-Conditions: the char is drawn to the screen in the specified color
	*************************************************************************/
	void OrganismGrid::draw_color_char(int char_color, char print_char)
	{
		attron(COLOR_PAIR(char_color));
		addch(print_char);
		attroff(COLOR_PAIR(char_color));
	}

	/*************************************************************************
	** Function: void OrganismGrid::print_color_str(int str_color, 
	**		string print_str)
	** Description: draws a colored string to screen
	** Parameters:
	**		int str_color: color to draw string
	**		char print_str: what string to draw
	** Pre-Conditions: none
	** Post-Conditions: a colored string is output to the console screen
	*************************************************************************/
	void OrganismGrid::print_color_str(int str_color, string print_str)
	{
		attron(COLOR_PAIR(str_color));
		printw(print_str.c_str());
		attroff(COLOR_PAIR(str_color));
	}

	/*************************************************************************
	** Function: void OrganismGrid::get_input()
	** Description: gets user single char input
	** Parameters: none
	** Pre-Conditions: none
	** Post-Conditions: if q is pressed, program exits; any other key continues
	*************************************************************************/
	void OrganismGrid::get_input()
	{
		char response = 'a';

		response = getch();

		if (response == 'q')
		{
			clean_up();
		}
	}

	/*************************************************************************
	** Function: void OrganismGrid::move_organisms(
	**		Organism *organism_list_head)
	** Description: 
	**		executes organism moving behavior for all the organisms from
	**		a linked list
	** Parameters: 
	**		Organism *organism_list_head: head of a doubly linked list
	**		of organisms
	** Pre-Conditions: none
	** Post-Conditions: 
	**		all the organisms in the list call their "move" behavior
	*************************************************************************/
	void OrganismGrid::move_organisms(Organism *organism_list_head)
	{ 
		if (organism_list_head != NULL)
		{
			Organism *org_pntr = organism_list_head;
			while (org_pntr != NULL)
			{
				org_pntr->move();
				org_pntr = org_pntr->get_next_link();
			}
		}
		else
		{
			end_simulation();
		}
	}

	/*************************************************************************
	** Function: void OrganismGrid::breed_organisms(
	**		Organism *&organism_list_head)
	** Description:
	**		executes breeding behavior for all organisms in a doubly
	**		linked list
	** Parameters:
	**		Organism *&organism_list_head: head of doubly linked list
	** Pre-Conditions: none
	** Post-Conditions: calls breeding behavior for all the organisms in list
	*************************************************************************/
	void OrganismGrid::breed_organisms(Organism *&organism_list_head)
	{
		if (organism_list_head != NULL)
		{
			Organism *org_pntr = organism_list_head;
			while (org_pntr != NULL)
			{
				org_pntr->breed(organism_list_head);
				org_pntr = org_pntr->get_next_link();
			}
		}
		else
		{
			end_simulation();
		}
	}

	/*************************************************************************
	** Function: void OrganismGrid::remove_dead(Organism *&organism_list_head)
	** Description: removes "dead" organisms from a linked list of organisms
	** Parameters:
	**		Organism *organism_list_head: head of a doubly linked list
	**		of organisms
	** Pre-Conditions: none
	** Post-Conditions: 
	**		all the dead organisms from the list are removed appropiately
	*************************************************************************/
	void OrganismGrid::remove_dead(Organism *&organism_list_head)
	{
		if (organism_list_head != NULL)
		{
			Organism *org_pntr = organism_list_head;
			while (org_pntr != NULL)
			{
				if (org_pntr->get_condition() == "dead")
				{
					org_pntr = remove_organism(org_pntr, organism_list_head);
				}
				else
				{
					org_pntr = org_pntr->get_next_link();
				}
			}
		}
		else
		{
			end_simulation();
		}
	}

	/*************************************************************************
	** Function: Organism* OrganismGrid::remove_organism(Organism *target,
	**		Organism *&organism_list_head)
	** Description: removes one organism from its doubly linked list
	** Parameters:
	**		Organism *target: pointer to organism to remove
	**		Organism *&organism_list_head: head of doubly linked list
	** Pre-Conditions: none
	** Post-Conditions: 
	**		deletes Organism target object and links previous and next nodes,
	**		unless target is the last object in the list, in which case the
	**		program calls its exit functions
	*************************************************************************/
	Organism* OrganismGrid::remove_organism(Organism *target,
		Organism *&organism_list_head)
	{
		// set next_link of previous organism to next_link of removed organism
		if (target != organism_list_head)
		{
			target->get_prev_link()->set_next_link(target->get_next_link());
		}
		else
		{
			if (target->get_next_link() != NULL)
			{
				organism_list_head = target->get_next_link();
			}
			else
			{
				delete target;
				organism_list_head = NULL;
				end_simulation();
				// shouldn't get here
				return NULL;
			}
		}

		// set prev_link of next organism to prev_link of removed organism
		if (target->get_next_link() != NULL)
		{
			target->get_next_link()->set_prev_link(target->get_prev_link());
			Organism *temp = target->get_next_link();
			delete target;
			return temp;
		}
		// shouldn't get here
		return NULL;
	}

	/*************************************************************************
	** Function: void OrganismGrid::randomize_order(Organism *&LifeListHead)
	** Description: randomly rearranges order Organisms in doubly linked list
	** Parameters:
	**		Organism *&LifeListHead: list to rearrange
	** Pre-Conditions: none
	** Post-Conditions: the order of which the Organisms are linked is 
	**		randomly mixed up
	*************************************************************************/
	void OrganismGrid::randomize_order(Organism *&LifeListHead)
	{
		int orgs_left_to_move = total_count;

		Organism *temp_container_head = *&LifeListHead;
		Organism *temp_container_iterator = temp_container_head;

		Organism *temp_insert_helper = new Organism();

		LifeListHead = NULL;

		LifeListHead = 
			move_random_organism(temp_container_iterator, 
			temp_container_head, orgs_left_to_move);

		while (temp_container_head != NULL)
		{					
			temp_insert_helper->insert_at_head(LifeListHead,
				move_random_organism(temp_container_iterator,
				temp_container_head, orgs_left_to_move));

			if (orgs_left_to_move == 0)
			{
				break;
			}
		}

		delete temp_insert_helper;
	}

	/*************************************************************************
	** Function: Organism*& OrganismGrid::move_random_organism( 
	**		Organism *&list_iterator, Organism *&list_head, int &list_size)
	** Description:
	**		removes and returns a random organism from a doubly linked list 
	**			to be moved to another
	** Parameters:
	**		Organism *&list_iterator: object to return to be removed 
	**		Organism *&list_head: head of list you are moving from
	**		int &list_size: reference to the size of the list you are
	**			moving from
	** Pre-Conditions: none
	** Post-Conditions: returns organism to be moved
	*************************************************************************/
	Organism*& OrganismGrid::move_random_organism(Organism *&list_iterator, 
		Organism *&list_head, int &list_size)
	{
		int index = 0;
		list_iterator = list_head;

		// generate random index to move
		srand(time(NULL) + rand());
		index = (rand() % list_size);

		if (index >= list_size)
		{
			clear();
			move(0, 0);
			printw("ERROR");
			refresh();
			getch();
			clean_up();
		}

		// iterate through list until we get to the random "index"
		for (int position = 0; position < index; position++)
		{
			if (list_iterator->get_next_link() != NULL)
			{
				list_iterator = list_iterator->get_next_link();
			}
		}

		// check if node is at the beginning
		if (list_iterator->get_prev_link() == NULL)
		{
			// check if node is at the end, which would mean it's the
			// only node left in the list
			if (list_iterator->get_next_link() != NULL)
			{
				// if it's at the beginning of the list, and not the
				// last item, connect the head to the node after the
				// node to be removed, and set that node's
				// prev_link to NULL. set the removed node's
				// next link to NULL
				list_head = list_iterator->get_next_link();
				list_iterator->get_next_link()->set_prev_link(NULL);
				list_iterator->set_next_link(NULL);
			}
			else // if it's the last node in the list, set the head to NULL
			{
				list_head = NULL;
			}
		}
		else // if node isn't at the beginning
		{
			// connect the previous node's next_link to the node
			// after the node to be removed's prev_link
			list_iterator->get_prev_link()->
				set_next_link(list_iterator->get_next_link());

			// we were at last element in list?
			if (list_iterator->get_next_link() == NULL)
			{
				//clear();
				//move(0, 0);
				//printw("iterator->get_next_link is NULL");
				//refresh();
				//getch();
			}
			else
			{
				list_iterator->get_next_link()->
					set_prev_link(list_iterator->get_prev_link());
			}
			// set node to be removed's previous link to NULL
			list_iterator->set_prev_link(NULL);
		}
		// set the node's next link to NULL
		list_iterator->set_next_link(NULL);

		// decrement list size counter
		list_size--;

		// return node to be removed
		return list_iterator;
	}

	/*************************************************************************
	** Function: void OrganismGrid::end_simulation()
	** Description: calls clean-up functions and exits program
	** Parameters: none
	** Pre-Conditions: none
	** Post-Conditions:
	**		ncurses settings are deactivated and dynamically allocated
	**		memory is freed
	*************************************************************************/
	void OrganismGrid::end_simulation()
	{
		clear();
		move(0, 0);
		printw("GAME OVER!!!");
		refresh();
		getch();
		clean_up();
	}

	/*********************************************************************
	** Function: void initialize_curse_stuff()
	** Description: calls initialization functions for ncurses
	** Parameters: none
	** Pre-Conditions: none
	** Post-Conditions: ncurses functionality is enabled
	*********************************************************************/
	void OrganismGrid::initialize_curse_stuff()
	{
		initscr();
		noecho();
		keypad(stdscr, TRUE);
		start_color();
		init_pair(1, COLOR_CYAN, COLOR_BLACK);
		init_pair(2, COLOR_GREEN, COLOR_BLACK);
		init_pair(3, COLOR_BLUE, COLOR_BLACK);
		init_pair(4, COLOR_YELLOW, COLOR_BLACK);
		init_pair(5, COLOR_WHITE, COLOR_BLACK);
		init_pair(6, COLOR_RED, COLOR_BLACK);
		init_pair(7, COLOR_MAGENTA, COLOR_BLACK);

		clear();
		refresh();
	}

	/*************************************************************************
	** Function: void OrganismGrid::clean_up()
	** Description: frees dynamically allocated memory and exits program
	** Parameters: none
	** Pre-Conditions: none
	** Post-Conditions: memory is freed and program exits
	*************************************************************************/
	void OrganismGrid::clean_up()
	{
		Organism *temp;
		int ants_deleted = 0;
		int bugs_deleted = 0;

		while (LifeListHead != NULL)
		{
				if (LifeListHead->get_species()	== ANT_SPECIES_NAME)
				{
					ants_deleted++;
				}
				else if (LifeListHead->get_species() == BUG_SPECIES_NAME)
				{
					bugs_deleted++;
				}

				temp = LifeListHead;
				LifeListHead = LifeListHead->get_next_link();

				delete temp;
		}
	 
		clear();
		printw("%i ants deleted, %i bugs deleted", ants_deleted, bugs_deleted);
		refresh();

		getch();
		nocbreak();
		echo();
		endwin();
		exit(0);
	}
}