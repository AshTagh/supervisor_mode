/***************************************************************************
 * Project           			: shakti devt board
 * Name of the file	     		: maze.c
 * Brief Description of file             : A configurable rat in a maze.

 Copyright (C) 2019  IIT Madras. All rights reserved.

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <https://www.gnu.org/licenses/>.

 ***************************************************************************/
/**
  @file maze.c
  @brief A configurable rat in a maze
  @detail A simple implemenation of rat in a maze program. 
 */

//#include <stdio.h>//Includes the defintions of standard input/output functions//
#include <wchar.h>//
#include <locale.h>
#include "uart.h"
#define SIZE 8

/** @fn void clear()
 * @brief  A function ot clear screen
 */
void clear()
{
	printf("\e[2J\e[H");
}
char maze[SIZE][SIZE] = {
	{'*','*','*','*','*','*','*','*'},
	{'*',' ',' ',' ',' ',' ',' ','*'},
	{'*',' ','*','*','*','*',' ','*'},
	{'*',' ','*','*','*','*',' ','*'},
	{'*',' ','*','*','*','*',' ','*'},
	{'*',' ','*','f',' ',' ',' ','*'},
	{'*',' ',' ','*','*','*','*','*'},
	{'*','*','*','*','*','*','*','*'}
};

char maze1[SIZE][SIZE] = {
	{'*','*','*','*','*','*','*','*'},
	{'*',' ',' ',' ',' ',' ',' ','*'},
	{'*',' ','*','*','*','*',' ','*'},
	{'*',' ','*','*','*','*',' ','*'},
	{'*',' ','*','*','*','*',' ','*'},
	{'*',' ','*','f',' ',' ',' ','*'},
	{'*',' ',' ','*','*','*','*','*'},
	{'*','*','*','*','*','*','*','*'}
};
char  solution[SIZE][SIZE] = {
	{'*','*','*','*','*','*','*','*'},
	{'*','*','*','*','*','*','*','*'},
	{'*','*','*','*','*','*','*','*'},
	{'*','*','*','*','*','*','*','*'},
	{'*','*','*','*','*','*','*','*'},
	{'*','*','*','*','*','*','*','*'},
	{'*','*','*','*','*','*','*','*'},
	{'*','*','*','*','*','*','*','*'}
};

char solution[SIZE][SIZE];

/** @fn void printsolution()
 * @brief A function to print the matrix with very recent move
 */
void printsolution()
{
	int i,j;
	for (i = 0; i < SIZE; i++)
	{
		for (j = 0; j < SIZE; j++)
		{
			printf("\t");
			if (maze1[i][j] == ' ')
			{
				printf(" ");
			}
			if (maze1[i][j] == '*')
			{
				printf("★ ");
			}
			if (maze1[i][j] == '.')
			{
				printf("🐀 ");
			}
			if (maze1[i][j] == 'f')
			{
				printf("☕ ");
			}
		}
		printf("\n");
	}
}

/** @fn intsolvemaze(int r, int c)
 * @brief Solves the maze problem 
 * @return int
 */
int solvemaze(int r, int c)
{
	if (maze[r][c] == 'f')
	{
		solution[r][c] = 'f';
		return 1;
	}
	if (r >= 0 && c >= 0 && r < SIZE && c < SIZE && solution[r][c] == '*' && maze[r][c] ==' ')
	{
		solution[r][c] = '.';
		maze1[r][c]='.';
		printsolution();
		printf("\e[2J\e[H");
		for (int i = 0; i < 1000; i++)
			maze1[r][c]=' ';
		if (solvemaze(r+1, c))
			return 1;
		if (solvemaze(r, c+1))
			return 1;
		if(solvemaze(r-1, c))
			return 1;
		if (solvemaze(r, c-1))
			return 1;
		solution[r][c] = '*';
		return 0;
	}
	return 0;

}

void main()
{
	printf("\n Starting MAZE\n");
	printf("\e[2J\e[H");
	if (solvemaze(6,1))
		printsolution();
	else
		printf("No solution\n");
}
