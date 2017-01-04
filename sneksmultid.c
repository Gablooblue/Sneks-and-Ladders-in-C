#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

//struct for Snakes and Ladders
typedef struct 
{
	int positionr, positionc, num;
	int destinationr, destinationc;
}SL;		
//struct for table & tiles info
typedef struct
{
	char letter;
}table;
// struct for player info
typedef struct
{
	int r,c, moves;
	char symbol,name[4];
	bool AI;
}character;

void menu();
void start(); 
void displayBoard(character * player, SL *snek, SL *lad );
void help();
void gameHistory();
int rollDice();
void SLreader (SL *snek, SL *lad);
void playerMove(character *player, int x, int dice, SL *snek, SL *lad);
void delay(unsigned int mseconds);
void inputHistory(character *player, int x);

void main()
{

	int input = 0;
	//displays menu and scans input
	do
	{	
		menu();
		scanf("%d", &input);
	
		switch(input)
		{
			case 1: start();
				break;
			case 2: help();
				break;	
			case 3: gameHistory();
				break;
			case 4: break;
			default: printf("\nInvalid input\n");
		 		break;
		}
	}while (input != 4);		
}	
void menu()
{       	
	printf("\n\t\t\t\tSNAKES AND LADDERS\n");
	printf("\n\t\t\t\t[1] Start Game \n");
	printf("\n\t\t\t\t[2] Help \n");
	printf("\n\t\t\t\t[3] Game History\n");
	printf("\n\t\t\t\t[4] Quit\n");

	
}

void start() 
{
	int playercount, x, dice = 0, size, y;
	bool end = false; 
	character player[4];
	SL snek[32], lad[32];

	SLreader(snek, lad);

	for(x = 0; x < 4; x++)
	{
		player[x].AI = true;
		player[x].r = 0;
		player[x].c = 7;
		player[x].moves = 0;
	}
	do
	{
		printf("\nHow many players? (0-4) ");
		scanf("%d", &playercount);
		getchar();
	}while(playercount > 4 || playercount < 0);
	printf("\n");
	printf("\nAdding %d AI character", (4 - playercount));
	if((4- playercount) > 1)
	{
		printf("s");
	}
	printf("\n");	
	
	// sets amount of AI players
	switch(playercount)
	{
		case 4: for(x = 0; x <= 3; x++)
			{
				player[x].AI = false;
			}	
			break;
		case 3: for(x =0; x < 3; x++)
			{
				player[x].AI = false;
			}	
			break;
		case 2: for(x= 0; x < 2; x++)
			{
				player[x].AI = false;
			}
			break;
		case 1:	player[0].AI = false;
			break;
	}	
	//Lets you pick what symbol to use for your player
	for(x = 0; x <4; x++)
	{
		printf("\nWhat symbol will player %d use for their piece?> ", x + 1);
		scanf(" %c", &player[x].symbol);
		printf("Player's symbol is %c", player[x].symbol);
	}
	getchar();
	x = 0;	
	//Board
	for(;;)
	{
		//goes into a function that displays board
		displayBoard(player, snek, lad);
		// AI checks if player or AI
		if(player[x].AI == false) //AI is false so player is not an AI
		{
			printf("\nPlayer %d's turn\n", (x + 1));
			printf("Press ENTER to roll the dice\n");
			getchar(); //Takes ENTER input
			player[x].moves++; // tracks movements
			srand(time(NULL));	//For rand function
			dice = rollDice(); //Starts dice function
			
			printf("Player %d will move %d spaces",x+1, dice);
			delay(500); // delays next command to make it easier to read
			playerMove(player, x, dice, snek, lad);	//Function that moves players	

		}
		else if(player[x].AI == true) //AI is true so player is an AI
		{
			printf("\nComputer's turn\n");
			player[x].moves++;
			srand(time(NULL));
			dice = rollDice();
			printf("\nComputer's moves %d spaces\n", dice);
			delay(500);
			playerMove(player, x, dice, snek, lad);	
		}
		//Checks if player is on a snake/ladder
		for(y = 0; y <= snek[0].num; y++) //snek.num refers to the number of snakes
		{
			if(player[x].r == snek[y].positionr && player[x].c == snek[y].positionc) // checks if player landed on a snake tile
			{
				//assigns player to new tile
				player[x].r = snek[y].destinationr;  
				player[x].c = snek[y].destinationc;
				printf("Player %d was eaten by a snake", x+1); 
				delay(500);
			}
		}
		for(y = 0; y <= lad[0].num; y++)
		{	
			if(player[x].r == lad[y].positionr && player[x].c == lad[y].positionc)// checks if player landed on a ladder tile	
			{
				player[x].r = lad[y].destinationr; //updates player location
				player[x].c = lad[y].destinationc;
				printf("Player %d climbed up a ladder", x + 1);
				delay(500);

			}
		}		
		// checks if player has won the game by checking if player has reached or surpassed winning area
		if(player[x].c <= 0 && player[x].r <= 0)
		{
			if (player[x].AI == false)// if player is not AI then it will record history
			{
				printf("\nPlayer %d wins with %d moves!\n", x + 1, player[x].moves);
				inputHistory(player, x); //function for inputting player information into history text file
			}
			else if(player[x].AI == true) //If AI wins then history is not recorded
			{
				printf("\nComputer wins!\n");
			}	
			break;
		}	
			if (x == 3) //resets x so players can be scanned again
			{
				x = 0;
			}
			else
			{
				x++;
			}
		
	
	}

}

void displayBoard(character * player, SL *snek, SL *lad)
{
	FILE *fboard;
	int row, column, x, line1, line2, line3;
	char display[4], sdisplay, ldisplay;
	fboard = fopen("board.txt", "w+");
	printf("\n");
	//Player info board
	for(x =0 ;x < 4; x++)
	{
		printf("\n\t\t\tPlayer %d:\t Symbol: %c\tMoves: %d", x + 1,player[x].symbol,
			       	 player[x].moves);
	}	
	printf("\n");
	printf("\t\t");
	//Board printing
	for(x = 0; x < 8; x++)
	{
		printf("_______");
	}	
	printf("_");
	printf("\n");
	for(column = 0; column < 8; column++)
	{
		printf("\t\t");
		for(line1 = 0; line1 < 8; line1++)
		{
			if(column == 0)
			{
				printf("|      ");
			}	
		
			else
			{		
				printf("|------");
			}	
		}
			printf("|");
			printf("\n");
			printf("\t\t");
		/*Basically this next block just keeps looping around and enables display 
			if requirements are met*/	
		for(row =0; row <8; row++)
		{	
			for(x = 0; x <4; x++)
			{
				// displays player symbol if player is on specific tile
				if(player[x].c == column && player[x].r == row)
				{
					display[x] = player[x].symbol; //If player is on specific tile, display tile will contain symbol of player
				}
				else
				{
					display[x] = ' '; //Otherwise the display tab will contain a blank space
				}
			}
			//Like the upper area except for snakes and ladders
			for(x = 0; x < 32; x++)
			{
				if(column == snek[x].positionc && row == snek[x].positionr)//checks if snek head is on tile
				{
					sdisplay = 'H';
					ldisplay = x + '0';//this adds the number after every letter, it turns the integer into a character
					break;
				}
				else
				{
					ldisplay = ' ';// otherwise the display variables are blank
					sdisplay = ' ';
				}		
			
				if(column == lad[x].positionc  && row == lad[x].positionr) //Ladder base checker
				{
					sdisplay = 'B';
					ldisplay = x + '0';
					break;
				}
				else 
				{
					sdisplay = ' ';
				 	ldisplay = ' ';
				}
				if(column == snek[x].destinationc && row == snek[x].destinationr) //snake tail checker
				{
					sdisplay = 'T';
					ldisplay = x + '0';
					break;
				}
				else
				{
					sdisplay = ' ';
					ldisplay = ' ';
				}		
			
				if(column == lad[x].destinationc  && row == lad[x].destinationr)//ladder peak checker
				{
					sdisplay = 'P';
					ldisplay = x + '0';
					break;
				}
				else 
				{
				 	sdisplay = ' ';
					ldisplay = ' ';
				}	
	
			}
				
				printf("|%c%c%c%c%c%c", display[0],display[1], display[2],
					display[3],sdisplay, ldisplay); //prints display variables which can either have a symbol or a blank space
				
		}	
		printf("|\n");
				
		
	}
	printf("\t\t");
	for(x =0; x< 8; x++)
	{
		printf("|______");
	}
	printf("|\n");
	printf("\n");
}

int rollDice()
{
	int num;
	//a normal rand function for the dice
	num = rand() %7;
	
   	if(num!=0){
		return num;
   	}
	else{
		return rollDice(num);
    	}
}    
// This function reads the file and assigns the snakes and ladders
void SLreader(SL *snek, SL *lad)
{
	table board[17][26];
	FILE *editor;
	int row, column, x = 0;
	char check[5];
	char let, let2;
	bool end = false;
	int xcoor, ycoor;
	snek[0].num = 0;
	lad[0].num = 0;
	int rowcount, columncount;	
	editor = fopen("SneksEdit.txt", "r");
	//setting snake and ladder position to a position not on board, to remove garbage values and unecessary bugs	
	for(x = 0; x <32; x++)
	{
		snek[x].positionr = 10;
		snek[x].destinationc = 10;
		lad[x].positionr = 10;
		lad[x].destinationc = 10;
	}	
	//reads entire text file and assigns it to multi-d array
	for(column = 0; column < 17; column++)
	{
		for(row =0; row <26 && let != EOF; row++)
		{
			let = fgetc(editor);
			board[column][row].letter = let;
			
		}
	}	


 
	//x numbers the snakes and ladders	
	for(x = 1; x <= 32; x++)
	{
		for(column = 0; column <17; column++)
		{
			for(row = 0; row <25; row++)
			{
				//since the program also records the borders of the board, we have to compensate for that here
				//this block just sets the values to their correct coordinates 
				if (row %2 == 1)
				{
					xcoor = (row - 1) / 3.0;
				}
				else if (row % 2 ==0)
				{
					xcoor = row / 3.0;
				}		
				if(column % 2 == 1)
				{
					ycoor = (column - 1) / 2.0;
				}
				else if( row %2 == 0)
				{
					ycoor = column / 2.0;
				}		
				//If the program spots an H, it will get the next character to check it's number
				//then it will record it's coordinates and set it to a snake in game
				if(board[column][row].letter == 'H' && board[column][row + 1].letter == (x + '0'))
				{
					snek[x].positionr = xcoor; // records coordinates
					snek[x].positionc = ycoor;
					snek[0].num++; // counts number of snakes
					continue;
				}
				else if (board[column][row].letter == 'T' && board[column][row + 1].letter == (x + '0')) // same with last except for snake tails
				{
					snek[x].destinationr = xcoor;
					snek[x].destinationc = ycoor;
					snek[0].num++;
					continue;
				}
				else if(board[column][row].letter == 'B' && board[column][row + 1].letter == (x +'0') )// records ladder bases
				{
					lad[x].positionr = xcoor;
					lad[x].positionc = ycoor;
					lad[0].num++;//records number of ladders
					continue;
				}
				else if	(board[column][row].letter == 'P' && board[column][row + 1].letter == (x +'0') )// records ladder peaks
				{
					lad[x].destinationr = xcoor;
					lad[x].destinationc = ycoor;
					lad[0].num++;
					continue;
				}
			}
		
		}
	}

	if(snek[0].num %2 == 1)// checks if there is an even amount of snakes and ladders
	{
		printf("\nERROR: Number of snake heads do not match number of snake tails\n");
		printf("\nExiting program\n"); 
		exit(0); // if not even the the program quits
	}
	if(lad[0].num %2 == 1)
	{
		printf("\nERROR: Number of ladder heads do not match number of ladder tails\n");
		printf("\nExiting program\n");
		exit(0);
	}	
	snek[0].num = snek[0].num / 2.0; // since we double counted the sneks, we have to divide it by 2
	lad[0].num = lad[0].num / 2.0;			
	return;
}


//Moves player
void playerMove(character *player, int x, int dice, SL *snek, SL *lad)
{
	for(;0 < dice  ; dice--) // loops until it has done as much moves as the dice told it to
	{
		if(player[x].c % 2 == 1) // if player is on an odd numbered column
		{
			player[x].r++; // player will move right
			if(player[x].r == 8) // if player is at 0 (the last tile on the column)
			{
				player[x].c--; //player will move to the next column
				player[x].r =7; //properly sets the player to the right tile
			}
		}
		else if(player[x].c % 2 == 0) // if player is on an even numbered column
		{
			player[x].r--; // player moves left
			if(player[x].r == -1) // if player is on last tile
			{
				player[x].c--; // player will mvoe to the next column
				player[x].r = 0; // properly sets player to the right tile
			}
		}
		delay(200);		
		displayBoard(player, snek, lad); // constantly displays the board to provide animated effect
	}	
}
// Delays the program by a set amount of time
void delay(unsigned int mseconds)
{
	clock_t goal = mseconds + clock(); // sets goal number
	while (goal > clock()); //loop will keep repeating until a certain number of time passes
}	

void help()
{
	//function opens the user manual
	FILE *manual;
	char word; 
	manual = fopen("snkesmanual.txt", "r");
	for(;word != EOF;)
	{	
		word = fgetc(manual);
		printf("%c", word); 
	}	
	printf("\n");
}	

void inputHistory(character *player, int x)
{
	//this function lets you input your name to be placed in game history
	FILE *file;
	file = fopen("score.txt", "a+");
	printf("\n Name: ");
	scanf("%s", &(player[x].name));

	fprintf(file, "\nName: %s\n", player[x].name); // prints name and moves to file
	fprintf(file, "Moves: %d\n", player[x].moves);
	fclose(file);
    	return;
}

void gameHistory(){
	FILE *file;
	//reads the game history file and displays it to console
	file = fopen("score.txt", "a+");
	char x;
	do{
		x = fgetc(file);
		printf("%c", x);
	}while (x != EOF);
	fclose(file);
	return;	
}	
