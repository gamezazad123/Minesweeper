#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define size 12 // real size is [size-1][size-1]
#define letter 97

// Status
#define NoBoom 0
#define Boom -1

// Check
#define OPEN 1
#define CLOSE 0
#define MARK 2

int matrix[size][size] = {0}; // Answer
int input_matrix[size][size] = {0}; // Display
int x[] ={ -1,0,1,0,-1,1,-1,1 };
int y[] ={ 0,-1,0,1,-1,-1,1,1 };

int BoomNum;
int check = 0;
int Countspace;

int countend = 0;

bool BoomOpen = false;

void init_matrix();
void display_matrix();
void random_boom();
void boom_nearby();
void build_board();
void player_input();
void pruning();
void input();

char diff[10];
int main() 
{
  input();
  Countspace = ((size-1) * (size-1)) - BoomNum;
  //printf("Countspace : %d\n", Countspace);
  srand(time(NULL));

  init_matrix();
  random_boom();
  boom_nearby();
  //display_matrix(); showanswer
  while(check == 0 && Countspace != 0)
  {
    build_board();
    player_input();
  }

  if(check != 0)
  {
    if(countend == 0){
    build_board();
    printf("Game Over...\n");
    countend++;
    display_matrix();
    }
  }
  else if(Countspace == 0)
  {
    build_board();
    printf("You Win");
  }
}

void input(){
  printf("Difficulty : ");
  scanf("%s", diff);
  if(strcmp(diff,"easy")==0){
    BoomNum = 8;
  }else if(strcmp(diff,"normal") == 0){
    BoomNum = 12;
  }else if(strcmp(diff,"hard") == 0){
    BoomNum = 17;
  }else{
    input();
  }
}

void init_matrix() // Table
{
  for(int i = 1; i < size; i++)
  {
    matrix[0][i] = letter + (i-1);
    matrix[i][0] = letter + (i-1);
    input_matrix[0][i] = letter + (i-1);
    input_matrix[i][0] = letter + (i-1);
  }
  matrix[0][0] = 32;
  input_matrix[0][0] = 32;
}

void display_matrix() // Show the answer 
{
  for(int i = 0; i < size; i++)
  {
    for(int j = 0; j < size; j++)
    {
      if(matrix[i][j] == NoBoom && i > 0 && j > 0)
      {
        printf(" . ");
      }
      else if(matrix[i][j] == Boom  && i > 0 && j > 0)
      {
        printf(" P ");
      }
      else if(matrix[i][j] > 0  && i > 0 && j > 0)
      {
        printf(" %d ", matrix[i][j]);
      }
      else
      {
        printf(" %c ", matrix[i][j]);
      }
    }
    printf("\n");
  }
  printf("\n");
}

void random_boom()
{
  int row, column;

  while(BoomNum != 0)
  {
    row = 1 + (rand()%(size-1));
    column = 1 + (rand()%(size-1));

    if(matrix[row][column] == NoBoom)
    {
      matrix[row][column] = Boom;
      BoomNum--;
    }
  }
}

void boom_nearby() // Find boom in the cell nearby
{
  for(int i = 1; i < size; i++)
  {
    for(int j = 1; j < size; j++)
    {
      if(matrix[i][j] != Boom)
        {
        if(matrix[i-1][j-1] == Boom)
        {
          matrix[i][j] += 1;
        }
        if(matrix[i-1][j] == Boom)
        {
          matrix[i][j] += 1;
        }
        if(matrix[i-1][j+1] == Boom)
        {
          matrix[i][j] += 1;
        }
        if(matrix[i][j-1] == Boom)
        {
          matrix[i][j] += 1;
        }
        if(matrix[i][j+1] == Boom)
        {
          matrix[i][j] += 1;
        }
        if(matrix[i+1][j-1] == Boom)
        {
          matrix[i][j] += 1;
        }
        if(matrix[i+1][j] == Boom)
        {
          matrix[i][j] += 1;
        }
        if(matrix[i+1][j+1] == Boom)
        {
          matrix[i][j] += 1;
        }
      }
    }
  }
}

void build_board() // Player will open and mark in this board
{
  for(int i = 0; i < size; i++)
  {
    for(int j = 0; j < size; j++)
    {
      if(input_matrix[i][j] == CLOSE && i > 0 && j > 0)
      {
        printf(" / ");
      }
      else if(input_matrix[i][j] == OPEN && i > 0 && j > 0)
      {
        if(matrix[i][j] == Boom) // When player open 'Boom'
        {
          printf(" p ");
          //BoomOpen = true;
          //check = 1;
        }
        else if(matrix[i][j] > 0)
        {
          printf(" %d ", matrix[i][j]);
        }
        else
        {
          printf(" . ");
        }
      }
      else if(input_matrix[i][j] == MARK && i > 0 && j > 0)
      {
        printf(" # ");
      }
      else
      {
        printf(" %c ", input_matrix[i][j]);
      }
    }
    printf("\n");
  }
  //printf("Countspace : %d\n", Countspace);
  printf("\n");
}

void player_input()
{
  char status, row, column;

  printf("Input m (mark) or o (open) or r (reset)>");
  scanf(" %c%*c", &status);
  if(status == 'r'){
  for(int i = 0; i < size; i++)
  {
    for(int j = 0; j < size; j++)
    {
      input_matrix[i][j] = 0;
      matrix[i][j] = 0;
  }
  }
  check = 0;
  main();
  }else{
  printf("Row and Column>");
  scanf("%c%*c %c%*c", &row, &column);
  // Check player input
  if(row - 96 < 1 || column - 96 < 1)
  {
    player_input();
  }
  if(input_matrix[row-96][column-96] == CLOSE)
  {
    if(status == 'o')
    {
      input_matrix[row-96][column-96] = OPEN;

      if(matrix[row-96][column-96] == 0)
      {
        pruning(row-96, column-96);
      }
      else if(matrix[row-96][column-96] == Boom)
      {
        check += 1;
      }
      
      Countspace--;
    }
    else if(status == 'm')
    {
      input_matrix[row-96][column-96] = MARK;
    }
  }
  else if(status == 'o' && input_matrix[row-96][column-96] == MARK)
  {
    player_input();
  }
  else if(status == 'm' && input_matrix[row-96][column-96] == MARK)
  {
    input_matrix[row-96][column-96] = CLOSE;
  }
  else if((status == 'm' || status == 'o') && input_matrix[row-96][column-96] == OPEN)
  {
    player_input();
  }
}
}

void pruning(int i, int j)
{
  if(input_matrix[i][j] == CLOSE)
  {
    input_matrix[i][j] = OPEN;
    Countspace--;
  }

  if(matrix[i][j] == CLOSE) 
  {
        for (int k = 0; k < 8; k++) 
    {
            if (i + y[k] > -1 && i + y[k] < size && j + x[k] > -1 && j + x[k] < size && input_matrix[i + y[k] ][ j + x[k] ] == 0) 
      {
                pruning(i + y[k], j + x[k]);
            }
        }
    }
}