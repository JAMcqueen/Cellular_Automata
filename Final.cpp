/*****************************************************************************
** Program Filename: Final.cpp
** Author: Joshua McQueen
** Date: 13MAR14
** Description: Program file for final project
** Input: none
** Output: simulation of ants' and doodlebugs' lifecycles, v2
*****************************************************************************/

#define COL_MAX 20
#define ROW_MAX 20

#define ANT_NUM 100//100
#define BUG_NUM 5

#include "OrganismGrid.hpp"

using JAM_CS::OrganismGrid;

int main()
{
	OrganismGrid grid(COL_MAX, ROW_MAX, ANT_NUM, BUG_NUM);

	grid.draw_grid();
	getch();
	while (true)
	{
		grid.run_simulation();
	}

	return 0;
}