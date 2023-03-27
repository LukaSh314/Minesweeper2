#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
void minefieldCreator(int, int, int, int ** );
void printField(int, int, int**);
void guess(int, int, int, int** , int**);
void checkProgress(int, int, int, int,  int, int**, int**);
void lose(int, int, int, int, int** , int**);
void hiddenfieldCreator(int, int,  int**);
void playGame(void);
void win(int, int, int, int **, int **);
void adjecentCells1(int , int, int **, int **);
void adjecentCells2(int , int , int **, int **);
int main(void){
	FILE *welcome;
	welcome = fopen("welcome.txt" , "r");
	char welcometxt[200];
	while(!feof(welcome)){
		fgets(welcometxt, 200, welcome);
		puts(welcometxt);
	}
	playGame();
	fclose(welcome);
	return 0;
}
//This function is used to start the game. To create space for dynamic 2d arrays we use memory allocation for arrays called minefield
// and hiddenfield.
void playGame(void){

	int **minefield;
	int **hiddenfield;
	int x;
	int y;
	int numMines;
	//asking the user for the parameters of the minefield
	printf(" Enter the number of rows: ");
	scanf("%d" , &x);
	printf("\n Enter the number of columns: ");
	scanf("%d" , &y);
	printf("\n Enter the number of mines: ");
	scanf("%d" , &numMines);
	// if the user enters an invalid number of mines, we ask them to enter a valid number and run the function again.
	if(numMines > (x * y)){
		printf("\n The number of mines cannot be more than the number of cells. Start over...\n\n");
		playGame();
	}
	// creating memory with extra spaces on the borders, so the user input can be more understandable, like (1 , 1) for 
	// the first cell in the first row, instead of (0 , 0)
	minefield = (int**) malloc(sizeof(int *) *(x+1));
	hiddenfield = (int**) malloc(sizeof(int *) *(x+1));
	for (int i = 0; i <= x+1; i++){
		minefield[i] = (int*)malloc(sizeof(int)* (y+1));
		hiddenfield[i] = (int*)malloc(sizeof(int)* (y+1));
	}
	hiddenfieldCreator(x, y, hiddenfield);
	for (int i = 0; i <= x + 1; i++){
		for (int j = 0; j <= y + 1; j++){
			minefield[i][j] = -10;
		}
	}
	//using the created functions to create the minefields and start the game
	minefieldCreator(x, y, numMines, minefield);
	printField(x, y, hiddenfield);
	guess(x, y, numMines, minefield, hiddenfield);
	for(int i = 0; i <= x+1; i++){
		free(minefield[i]);
		free(hiddenfield[i]);
	}
	free(minefield);
	free(hiddenfield);		
}
//function that assigns empty values ('-') to the hiddenfield (field that the user sees)
void hiddenfieldCreator(int x, int y, int **hiddenfield){
	for (int i = 1; i <= x; i++){
		for( int j = 1; j <= y; j++){
            hiddenfield[i][j] = '-';
        }
	}
}
//function that assigns values to the original minefield, with mines and numbers (number of mines in surrounding cells)
void minefieldCreator(int x, int y, int numMines, int **minefield){
//time function used for randomness to create mines in random places;
	srand(time(NULL));
	int mines=0;
	int k = 0;
	int l = 0;
	while (mines<numMines){
		k = rand()%(x+1);
		l = rand()%(y+1);
		if (k == 0){
			k = 1;
		}
		if (l == 0){
			l = 1;
		}
		if( minefield[k][l] != '*'){
            minefield[k][l] = '*';
            mines++;
        }
	}
	//counting number of mines in the surrounding cell after scanning the whole minefield
	for (int i = 1; i <= x; i++){
		for( int j = 1; j <= y; j++){
			if(minefield[i][j] != '*'){
                minefield[i][j] = 0;
    			if(minefield[i-1][j-1] == '*'){
        			minefield[i][j]++;
    			}	             

    			if(minefield[i-1][j] == '*'){
        			minefield[i][j]++;
    			}
    		
    			if(minefield[i][j-1] == '*'){
            		minefield[i][j]++;
        		}
    		
				if(minefield[i-1][j+1] == '*'){
            		minefield[i][j]++;
        		}
        		if(minefield[i+1][j-1] == '*'){
            		minefield[i][j]++;
        		}

        		if(minefield[i+1][j] == '*'){
            		minefield[i][j]++;
        		}

				if(minefield[i][j+1] == '*'){
            		minefield[i][j]++;
        		}
        		if(minefield[i+1][j+1] == '*'){
            		minefield[i][j]++;
        		}
    		}
		}
	}
}
//functoin to easily print out minefields
void printField(int x, int y, int **hiddenfield){
	for (int i = 1; i <= x; i++){
		for( int j = 1; j <= y; j++){
			if(hiddenfield[i][j] == '-'){
				printf("|%c| " , hiddenfield[i][j]);
			}
			if(hiddenfield[i][j] == 0){
				printf("|%c| " , 'N');
			} 
			if(hiddenfield[i][j] == '*'){
				printf("|%c| " , hiddenfield[i][j]);
			}
			if(hiddenfield[i][j] == 'F'){
				printf("|%c| " , hiddenfield[i][j]);
			}
			if(hiddenfield[i][j] != '*' && hiddenfield[i][j] != '-' && hiddenfield[i][j] != 0 && hiddenfield[i][j] != 'F'){
				printf("|%d| " , hiddenfield[i][j]);
			}
		}
		printf("\n\n");
	}
}
//function that allows the user to make moves
void guess(int x, int y, int numMines, int **minefield, int **hiddenfield){                                     
	int j , i;
	char c;                             
	printf("\n Input F/f for flagging or R/r for revealing a cell: ");
	scanf(" %c" , &c);
	//checks if the user chose a valid character for their move.
	if (c != 'F' && c != 'R' && c !='f' && c != 'r'){
		printf("\n\n Please input a valid character...\n");
		guess(x, y, numMines, minefield, hiddenfield);
	}
	//asking the user to type F if they want to flag a cell  n
	if(c == 'f' || c == 'F'){
		printf("\n Please Input x component: ");
		scanf("%d" , &i);
		printf("\n Please input y component: ");
		scanf("%d" , &j);
		// if the user wants to remove a flag, they can select it again
		if (hiddenfield[i][j] == 'F'){
			hiddenfield[i][j] = '-';
		} else 
		hiddenfield[i][j] = 'F';

		printField(x , y , hiddenfield);
		checkProgress(i , j, x , y, numMines, minefield, hiddenfield);
		guess(x , y , numMines, minefield, hiddenfield);

		}	
	//asking the user to type R if they want to reveal a cell
	if (c == 'r' || c == 'R'){
		printf("\n Please Input x component: ");
		scanf("%d" , &i);
		printf("\n Please input y component: ");
		scanf("%d" , &j);
		//if the user selects a flagged cell while revealing, we ask them to select a different cell and run the function again.
		if (hiddenfield[i][j] == 'F'){
			printf("\n This cell is flagged. Please choose another cell...\n\n");
			guess(x , y, numMines, minefield, hiddenfield);
		}
		if (i > x || j > y || i < 1 || j < 1){
			printf("\n Please enter valid coordinates...\n\n");
			guess(x , y , numMines, minefield, hiddenfield);
		}
		//during the guess function, we check if the user has lost the game by revealing a cell.
		lose(i, j, x, y, minefield, hiddenfield);
		//autamatic revealing of adjecent cells
		adjecentCells1(i, j, hiddenfield, minefield);
		adjecentCells2(i, j, hiddenfield, minefield);
		//if the user has not lost the game, we print out the renewed version of the hidden field.
		if (minefield[i][j] != '*'){
			hiddenfield[i][j] = minefield[i][j];
			printField(x , y , hiddenfield);
		}
		//checking the progress (continueing to guess the game or make moves)
		checkProgress(i , j, x , y, numMines, minefield, hiddenfield);
		}

}
//function that checks the progress of the user.
void checkProgress(int i, int j, int x, int y, int numMines, int **minefield,int **hiddenfield){
	//check if user has already won the game
	if (minefield[i][j] != '*'){
		win(x, y, numMines, hiddenfield, minefield);
		guess(x, y, numMines, minefield, hiddenfield);
	}
}
//function that terminates the program if the game is lost
void lose(int i, int j, int x, int y, int **minefield , int **hiddenfield){
	if(minefield[i][j] == '*'){
		printf("\nBOOM!!!!! GAME OVER\n\n");
		printField(x , y, minefield);
		printf("\n Goodbye!");
		exit(0);
		
	}
}
//function that terminates the program if the game is won
void win(int x, int y, int numMines, int **hiddenfield , int **minefield){
	int count = 0;
	//using a count variable to check if the revealed cells are already equal to the number of non-mine cells,
	// in which case the user has won the game.
	for(int i = 1; i <= x; i++){
		for( int j = 1; j <= y; j++){
			if (hiddenfield[i][j] != '-' && hiddenfield[i][j] != '*'){
				count++;
			}
		}
	}
	if (count >= ((x * y)-numMines)){
		printf("\n Original minefield: \n");
		printField(x , y , minefield);
		printf("\n |YOU WIN!!!!!!!!!!| CONGRATULATIONS! ");
		printf("\n Goodbye!");
		exit(0);
	}
}
//function that uses recursion to automatically reveal the surrounding cells that are not mines.
//this function is devided into 2 parts, because the recursion would not work if going in opposite directions
//for example i-1, j-1 and i+1,j+1 would create an infinite struggle for the recursion
void adjecentCells1(int i, int j, int **hiddenfield, int **minefield){
	if (minefield[i][j] == 0){
	    hiddenfield[i][j] = minefield[i][j];
	    // checks south west cell. recursion goes to that direction
		if(minefield[i+1][j-1] != '*' &&  hiddenfield[i+1][j-1] != 0){
  		    hiddenfield[i+1][j-1] = minefield[i+1][j-1];
  		    adjecentCells1(i+1, j-1, hiddenfield, minefield);
  		    adjecentCells2(i+1, j-1, hiddenfield, minefield);
		}
		// checks south  cell. recursion goes to that direction
        if(minefield[i+1][j] != '*' &&  hiddenfield[i+1][j] != 0){
            hiddenfield[i+1][j] = minefield[i+1][j];
            adjecentCells1(i+1, j, hiddenfield, minefield);
            adjecentCells2(i+1, j, hiddenfield, minefield);
        }
        // checks east cell. recursion goes to that direction
        if(minefield[i][j+1] != '*' &&  hiddenfield[i][j+1] != 0){
            hiddenfield[i][j+1] = minefield[i][j+1];
            adjecentCells1(i, j+1, hiddenfield, minefield);
            adjecentCells2(i, j+1, hiddenfield, minefield);
		}
		// checks south east cell. recursion goes to that direction
        if(minefield[i+1][j+1] != '*' &&  hiddenfield[i+1][j+1] != 0){
            hiddenfield[i+1][j+1] = minefield[i+1][j+1];
            adjecentCells1(i+1, j+1,  hiddenfield, minefield);
            adjecentCells2(i+1, j+1,  hiddenfield, minefield);
		}	
	}
}	
void adjecentCells2(int i, int j, int **hiddenfield, int **minefield){
	if (minefield[i][j] == 0){
		hiddenfield[i][j] = minefield[i][j];
		// checks north west cell. recursion goes to that direction
		if(minefield[i-1][j-1] != '*' && minefield[i-1][j-1] != -10 && hiddenfield[i-1][j-1] != 0){
        	hiddenfield[i-1][j-1] = minefield[i-1][j-1];
        	adjecentCells1(i-1, j-1,  hiddenfield, minefield);
        	adjecentCells2(i-1, j-1, hiddenfield, minefield);
        }
        // checks north cell. recursion goes to that direction
        if(minefield[i-1][j] != '*' &&  hiddenfield[i-1][j] != 0){
            hiddenfield[i-1][j] = minefield[i-1][j];
            adjecentCells1(i-1, j, hiddenfield, minefield);
            adjecentCells2(i-1, j, hiddenfield, minefield);
			}
			// checks west cell. recursion goes to that direction
        if(minefield[i][j-1] != '*' &&  hiddenfield[i][j-1] != 0){
            hiddenfield[i][j-1] = minefield[i][j-1];
            adjecentCells1(i, j-1, hiddenfield, minefield);
            adjecentCells2(i, j-1, hiddenfield, minefield);
        }
        // checks north east cell. recursion goes to that direction
        if(minefield[i-1][j+1] != '*' &&  hiddenfield[i-1][j+1] != 0){
            hiddenfield[i-1][j+1] = minefield[i-1][j+1];
            adjecentCells1(i-1, j+1, hiddenfield, minefield);
            adjecentCells2(i-1, j+1, hiddenfield, minefield);
		}
	}
}
