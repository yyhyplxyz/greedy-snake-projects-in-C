#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <termios.h>
#include <unistd.h>
#define SNAKE_MAX_LENGTH 20
#define  SNAKE_HEAD 'H'
#define SNAKE_BODY 'X'
#define SNAKE_FOOD '$'
#define SNAKE_CELL ' '
#define WALL_CELL '*'
void snakeMOVE(int, int);
void put_money(void);
void output(void);
void gameover(void);
char map[12][12] =
    {"***********",
    "*XXXXH    *",
    "*         *",
    "*         *",
    "*         *",
    "*         *",
    "*         *",
    "*         *",
    "*         *",
    "*         *",
    "*         *",
    "***********"};
int snakeX[SNAKE_MAX_LENGTH] = {1,2,3,4,5};
int snakeY[SNAKE_MAX_LENGTH] = {1,1,1,1,1};
int snakelength = 5;// the global variables which are defined by the TA or the teacher
int fx, fy;// a variable to store the food's position
char movable[4]={'a','d','s','w'};// an array to store the direction choice
int distance[4]={999,999,999,999};// initialise the distance array , it helps to find the minumn distance between food and snake's head
int gamestate = 1;
static struct termios ori_attr, cur_attr;
int judge(int dx, int dy)// to judge whether the snake can move into the direction
{
	if (map[snakeY[snakelength - 1] + dy][snakeX[snakelength - 1] + dx] == ' ' || map[snakeY[snakelength - 1] + dy][snakeX[snakelength - 1] + dx] == '$' )
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
int dis(int dx, int dy)// to caluculate the distance between the food and the snake's head
{
	return abs(fx - (snakeX[snakelength - 1] + dx)) + abs(fy - (snakeY[snakelength - 1] + dy));
}
void clearscreen()
{
	printf("\033[2J\033[1;1H");
}
#define get_char getchar
char kbhit()// it's a function like kbhit in windows 
{
    struct termios stored_settings;
    struct termios new_settings;
    tcgetattr (0, &stored_settings);
    new_settings = stored_settings;
    new_settings.c_lflag &= (~ICANON);
    new_settings.c_cc[VTIME] = 0;
    new_settings.c_cc[VMIN] = 1;
    tcsetattr (0, TCSANOW, &new_settings);
    
    char c = get_char ();

    tcsetattr (0, TCSANOW, &stored_settings);
    
    return c;
}
int main()
{
	put_money();// set the food randomly in the map
	output();
	// print the map
	char ch;
	while (ch = kbhit() && gamestate)
	{	
		if(ch == ' ')
		gameover();
		if(judge(-1, 0))//judge whether snake can move into this direction
	{
		distance[0] = dis(-1, 0);
	}
	if(judge(1, 0))
	{
		distance[1] = dis(1, 0);
	}
	if(judge(0, 1))
	{
		distance[2] = dis(0, 1);
	}
	if(judge(0, -1))
	{
		distance[3] = dis(0, -1);
	}
	int min = 999;
	int k = 0;
	int i, j;
	for( i = 0; i < 4; i++)
	{
		if(min >= distance[i])
		{
			min = distance[i];
			k = i;
		}
	}
			switch(movable[k])
			{// it means input A or a, the snake will move left
			case 'a':
				clearscreen();
				snakeMOVE(-1,0);
				break;//it means input D or d, the snake will move right
			case 'd':
				clearscreen();
				snakeMOVE(1,0);
				break;//it means input W or w the snake will move upwards
			case 'w':
				clearscreen();
				snakeMOVE(0,-1);
				break;// it means input S or s the snake will move downwards
			case 's':
				clearscreen();
				snakeMOVE(0,1);
				break;
			}
		if(gamestate == 0)
		gameover();// if you input blank space ,the game is over
	}
		return 0;
}
void snakeMOVE(int a,int b)
{
	
	if (map[snakeY[snakelength - 1] + b][snakeX[snakelength - 1] + a]=='$' )// when the sanke touch the food the snake body size will grow one
	{
		 snakelength++;
		 snakeX[snakelength - 1] = snakeX[snakelength - 2] + a;
		 snakeY[snakelength - 1] = snakeY[snakelength - 2] + b;
	}
	if( a == -1 || b == -1)// when the snake move left or move upwards, the tail of the snake will change first so I separate my sntences to control different movement
	{
		int i, j;
	for( i = 1; i <= 10 ; i++)// clear the original traces, including the food
		{
			for( j = 1; j < 10; j++)
			{
				map[i][j] = ' ';
			}
		}
		int tempx[20];// a temporary array to store the original snake body's position
		int tempy[20];
		for( i = 0; i < snakelength - 1; i++)
		{
			tempx[i] = snakeX[i];
			tempy[i] = snakeY[i];
		}
		for( i = 0; i < snakelength - 1; i++)// make the original snake body's position move to it's previous position 
		{
			snakeX[i + 1] = tempx[i];
			snakeY[i + 1] = tempy[i];
		}
			snakeX[0] += a;// make the snake's tail move accoring to our command
			snakeY[0] += b;
		if(map[snakeX[0]][snakeY[0]] == '*' || map[snakeX[0]][snakeY[0]] == 'X') // when the snake touch itself or touch the wall, the game is over
		{
			gameover();
		}
		map[snakeY[snakelength - 1]][snakeX[snakelength - 1]] = 'H';// make the snake head become 'H'
		for ( i = 0; i < snakelength - 1; i++)
		{
			map[snakeY[i]][snakeX[i]] = 'X';// show the snake body
		}
		put_money();// randomly set the food
		output();// print the map and the snake 
	}
	else{
		int i, j;
		for( i = 1; i <= 10 ; i++)// clear the original traces, including the food
		{
			for( j = 1; j < 10; j++)
			{
				map[i][j] = ' ';
			}
		}
		if (snakeX[snakelength - 1] + a == snakeX[snakelength - 2] && snakeY[snakelength - 1] + b == snakeY[snakelength - 2])// avoid the snake touch itself
		{
			return ;
		}
		if (map[snakeY[snakelength - 1] + b][snakeX[snakelength - 1] + a]=='*' || map[snakeY[snakelength - 1] + b][snakeX[snakelength - 1] + a]== 'X')//if the snake touchs its body or the wall, the game is over
		{
			gameover();
		}
		for (i = 0; i < snakelength - 1; i++)// make the original snake body's position move to it's previous position
		{
			snakeX[i] = snakeX[i + 1];
			snakeY[i] = snakeY[i + 1];
		}
		snakeX[snakelength - 1] += a;// change the snake head's position
		snakeY[snakelength - 1] += b;
		for(i = 0; i < snakelength - 1; i++)
		{
			map[snakeY[i]][snakeX[i]] = 'X';
		}
		map[snakeY[snakelength - 1]][snakeX[snakelength - 1]] = 'H';// make the snake head become 'H'
		put_money();// randomly set the food
		output();// print the map and the snake 
	}	
}
void output() // just print function to show the map and the snake
{
	int i, j;
	for ( i = 0; i < 12; i++)
	{
		for ( j = 0; j < 12; j++){
			printf("%c",map[i][j]);
		}
		printf("\n");
	}
}
void gameover()// inform u that the game is over and exit the programe
{
	clearscreen();
	gamestate = 0;
	printf("The game is over\n");
	exit(1);
}
void put_money(void) // randomly set a number and make food appear in the position of this number 
{  
    int flag = 1;
    while (flag) {
        srand(time(NULL));
         fx = rand() % 12;
         fy = rand() % 12;
        if (map[fy][fx] == ' ') 
	{
            map[fy][fx] = '$';
            flag = 0;
        }
    }
    return;
}
