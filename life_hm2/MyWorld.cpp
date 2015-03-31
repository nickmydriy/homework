#include <iostream>
#include "stdafx.h"

#ifdef WIN32
#include <windows.h>
#include "conio.h"
#define CLEAR_SCREEN "cls"
#define gtchar _getch()
#else
#define CLEAR_SCREEN "clear"
#include <termios.h>
#include <unistd.h>
#define gtchar mygetch()
int mygetch( ) {
  struct termios oldt,
                 newt;
  int            ch;
  tcgetattr( STDIN_FILENO, &oldt );
  newt = oldt;
  newt.c_lflag &= ~( ICANON | ECHO );
  tcsetattr( STDIN_FILENO, TCSANOW, &newt );
  ch = getchar();
  tcsetattr( STDIN_FILENO, TCSANOW, &oldt );
  return ch;
}
#endif


using namespace std;

bool **world;                                               // our world
int height, width;                                          
int population, born, die, step_no_changes, pop_size;



void world_out_line()                                       // output world
{
	int value;
	cin >> value;
	int left = max(width / 2 - value / 2,0), right = min(width / 2 + value / 2, width - 1);
	int bot = max(height / 2 - value / 2,0), top = min(height / 2 + value / 2, height - 1);
	while(1)
	{
		system (CLEAR_SCREEN);
		cout << "Movement on the field with WASD('w'(up), 's'(down), 'a'(left), 'd'(right));" << endl;
		for( int i = bot; i < top; i++)
		{
			for( int j = left; j < right; j++)
				if( world[i][j] == 1)
					cout << "# ";
				else 
					cout << "* ";
			cout << "\n";
		}
		cout << "\n" << "left = " << left << "; right = " << right << "; bot = " << bot << "; top = " << top << "\n";
		cout << "size = " << right - left;
		char c = gtchar;
		switch ( c )
		{
		case 'w':
			{
				if (bot > 0)
				{
					top--;
					bot--;
				}
			}
			break;
		case 's':
			{
				if (top < height)
				{
					top++;
					bot++;
				}
			}
			break;
		case 'd':
			{
				if (right < width)
				{
					right++;
					left++;
				}
			}
			break;
		case 'a':
			{
				if (left > 0)
				{
					right--;
					left--;
				}
			}
			break;
		}

	}
}

void world_new(int side,int del) //memory allocation, memory removing; side : 0-bot,1-top,2-left,3-right; del : the number of rows to delete
{
	bool **new_world;
	if(side < 2)                                   //add (delete) string;                   
	{
		new_world = new bool*[height + 1 - del];     //create a new array
		for(int i = 0; i < height + 1 - del; i++)
			new_world[i] = new bool[width];
		for(int i = -(side - 1); i < height - (side - 1) - del; i++)            //copy old array in new
			for (int j = 0; j < width; j++)
					new_world[i][j] = world[i + (side - 1) - (side - 1) * del][j];
		for(int i = 0; i < width; i++)
			new_world[side * (height - del)][i] = 0;
		for( int i = 0; i < height; i++)        // delete old array
			delete[] world[i];
		delete[] world;
		height += (1 - del);
		world = new_world;		 // replace new array
	}
	else
	{
		if(side == 2)                  // like up, but add (delete) row
			side = 0;
		else
			side = 1;
		new_world = new bool*[height];
		for(int i = 0; i < height; i++)
			new_world[i] = new bool[width + 1 - del];
		for(int i = 0; i < height; i++)
			for(int j = -(side - 1); j < width - (side - 1) - del; j++)
				new_world[i][j] = world[i][j + (side - 1) - (side -1) * del];
		for(int i = 0; i < height; i++)
			new_world[i][side * (width - del)] = 0;

		for( int i = 0; i < height; i++)
			delete[] world[i];
		delete[] world;
		width += (1 - del);
		world = new_world;
	}
}

void world_ini( int size, int count)            // custom initialization
{
	world = new bool*[size + 4];
	for( int i = 0; i < size + 4; i++)
		world[i] = new bool[size + 4];
	for( int i = 0; i < size + 4; i++)
		for( int j = 0; j < size + 4; j++)
			world[i][j] = 0;
	while(count != 0)
	{
		int x = rand() % size + 2, y = rand() % size + 2;
		if(world[x][y] == 0)
		{
			world[x][y] = 1;
			count--;
		}
	}
	width = size + 4;
	height = size + 4;
}

void world_boarder_check()          // verification of the need for additional lines
{
	for (int i = 1; i < width - 1; i++)
		if (world[1][i] == 1)
		{
			world_new(0,0);                   //if this need, allocate memory
			break;
		}
		for (int i = 1; i < width - 1; i++)
			if (world[height - 2][i] == 1)
			{
				world_new(1,0);             //like up
				break;
			}
			for (int i = 1; i < height - 1; i++)
				if (world[i][1] == 1)
				{
					world_new(2,0);
					break;
				}
				for (int i = 1; i < height - 1; i++)
					if (world[i][width - 2] == 1)
					{
						world_new(3,0);
						break;
					}
}

int pop()                        // calculation of population size
{
	int pop_size = 0;
	for( int i = 0; i < height; i++)
		for( int j = 0; j < width; j++)
			if (world[i][j] == 1)
				pop_size++;
	return pop_size;
}



void world_step( int i)          // step of simulation
{
	if(i % 10 == 0)               // every 10 steps delete ussles data
	{
		cout << i << " step: population size: " << pop() << "\n" << "born/die: " << (float)born/die << "\n";  //statistic
		born = 0; 
		die = 0;
		int x = 0;
		while (1)                                // check for ussles data
		{
			bool f = 0;
			for ( int i = 0; i < width; i++)
				if (world[x][i] == 1)
				{
					f = 1;
					break;
				}
				if (f == 1) break;
				x++;
				if(x >= height) break;
		}
		if(x > 2) 
			world_new(0, x - 1);
		x = height - 1;
		while (1)                       // like up
		{
			bool f = 0;
			for ( int i = 0; i < width; i++)
				if (world[x][i] == 1)
				{
					f = 1;
					break;
				}
				if (f == 1) break;
				x--;			
				if (x < 0) break;
		}
		if( height - 1 - x > 2)
			world_new(1, height - 2 - x);

		x = 0;
		while (1)
		{
			bool f = 0;
			for ( int i = 0; i < height; i++)
				if (world[i][x] == 1)
				{
					f = 1;
					break;
				}
				if (f == 1) break;
				x++;
				if (x >= width) break;
		}
		if(x > 2) 
			world_new(2, x - 1);
		x = width - 1;
		while (1)
		{
			bool f = 0;
			for ( int i = 0; i < height; i++)
				if (world[i][x] == 1)
				{
					f = 1;
					break;
				}
				if (f == 1) break;
				x--;
				if (x < 0) break;
		}
		if( width - 1 - x > 2)
			world_new(3, width - 2 - x);
	}
	bool **c_world;                                // step of simulation
	int pop_change = pop();
	c_world = new bool*[height];                   // array of changes
	for( int i = 0; i < height; i++)
		c_world[i] = new bool[width];
	for( int i = 0; i < height; i++)
		for( int j = 0; j < width; j++)
			c_world[i][j] = 0;
	for( int i = 1; i < height - 1; i++)
		for( int j = 1; j < width - 1; j++)
		{
			if (world[i][j] == 1)
			{
				int count = 0;
				for( int z = i - 1; z < i + 2; z++)
					for( int v = j - 1; v < j + 2; v++)
						if (world[z][v] == 1)
							count++;
				if(count != 3 && count != 4)
					c_world[i][j] = 1;
				else
					die++;

			}
			else
			{
				int count = 0;
				for( int z = i - 1; z < i + 2; z++)
					for( int v = j - 1; v < j + 2; v++)
						if (world[z][v] == 1)
							count++;
				if(count == 3)
				{
					c_world[i][j] = 1;
					born++;
				}
			}
		}		

		for( int i = 1; i < height - 1; i++)
			for( int j = 1; j < width - 1; j++)
			{
				if (c_world[i][j] == 1)
				{
					if( world[i][j] == 1)
						world[i][j] = 0;
					else world[i][j] = 1;
				}
			}
			if(pop_change == pop() && step_no_changes == 0)
				step_no_changes = i;
			else
				step_no_changes == 0;

			for( int i = 0; i < height; i++)
				delete[] c_world[i];
			delete[] c_world;
			world_boarder_check();                    //checker


}

void world_ini_user(int size)                         //user initilization
{	
	world = new bool*[size + 4];
	for( int i = 0; i < size + 4; i++)
		world[i] = new bool[size + 4];
	for( int i = 0; i < size + 4; i++)
		for( int j = 0; j < size + 4; j++)
			world[i][j] = 0;
	for( int i = 2; i < size + 2; i++)
		for( int j = 2; j < size + 2; j++)
			cin >> world[i][j];
	for( int i = 2; i < size + 2; i++)
		for( int j = 2; j < size + 2; j++)
			if(world[i][j] == 1)
				pop_size++;		
	width = size + 4;
	height = size + 4;
}

int main()
{
	int steps, size, count;
	//cout << "Please, check this on Windows. Good luck." << "\n" <<"\n";
	cout << "Please, Enter the size of the start of the field and the number of simulation steps." << "\n"<< "\n";
	cin >> size >> steps;
	//world_ini(size, count);
	cout << "Please, Enter start of the field. (1 - populated, 0 - non populated)" << "\n" << "\n";
	world_ini_user(size);
	for( int i = 1; i < steps; i++)
		world_step(i);
	cout << "population growth: " << pop() - pop_size << "\n" << "\n";
	if(step_no_changes != 0)
		cout << "after " << step_no_changes << " step population didnt changes" << "\n"<< "\n";
	cout << "Please, Enter desired size of field."<<"\n";
	world_out_line();	
}
