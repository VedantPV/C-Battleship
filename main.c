#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdbool.h>
#include <ctype.h>

#define PORT1 2201 
#define PORT2 2202
#define BUFFER_SIZE 1024

char *buffer_point;
int num_rows = 0, num_cols = 0;
char board[1000][1000], board2[1000][1000]; //**board = malloc(sizeof(width * heght)) Or Keep it like this and keep the rows/cols as the bounds for the board!
char *player_1_query, *player_2_query;
char store_player_1_init[1000], store_player_2_init[1000];
int num_of_player_1_ships = 5, num_of_player_2_ships = 5;
bool in_begin = false, in_initialize = false, in_game_loop = false; 
bool forfeit_flag = 0;

//Create external char* board to create Battleship board!
//If client1 send F, client2 must send something before sending Halt messages. 
//If client1 sinks all ships, client2 must send something, then client1 must send something again before sending halt messages.
//When sending halt messages, error messages are not necessary. Jjust take in responses and throw them away.

//char *board[10][10] = {'-'}; Better as a pointer. Malloc happens after begin provides the valid dimensions

int check_other_initialize_errors_client_1(char *buffer)
{
    for(int i = 1; i < strlen(buffer); i++) //Change to accomodate not just single-digit params!
    {
        while(buffer[i] != ' ')
            {
            i++; 
            }
            if(i >= strlen(buffer))
            {
                break;
            }
  	        while(buffer[i] == ' ')
            {
            i++; 
            }
            if(i >= strlen(buffer))
            {
                break;
            }

            int piece_type = atoi(&buffer[i]);
            while(buffer[i] != ' ')
            {
            i++; 
            }
  	        while(buffer[i] == ' ')
            {
            i++; 
            }
            
            int piece_rotation = atoi(&buffer[i]);
            while(buffer[i] != ' ')
            {
            i++; 
            }
  	        while(buffer[i] == ' ')
            {
            i++; 
            }
            
            int col = atoi(&buffer[i]);
            while(buffer[i] != ' ')
            {
            i++; 
            }
  	        while(buffer[i] == ' ')
            {
            i++; 
            }

            int row = atoi(&buffer[i]);
        if(piece_type == 1)
        {
            if(row < 0 || row+1 < 0 || col < 0 || col+1 < 0 || row >= num_rows || row+1 >= num_rows || col >= num_cols || col+1 >= num_cols)
            {
            printf("Fails Test 1\n");
            return 302;
            }
            else if(board[row][col] != '-' || board[row+1][col] != '-' || board[row][col+1] != '-' || board[row+1][col+1] != '-')
            {
                return 303;
            }
        }
        else if(piece_type == 2)
        {
            if(piece_rotation % 2 == 1)
            {
                if(row < 0 || row+1 < 0 || row+2 < 0 || row+3 < 0 || col < 0 || row >= num_rows || row+1 >= num_rows || row+2 >= num_rows || row+3 >= num_rows || col >= num_cols)
                {
                    return 302;
                }
                else if(board[row][col] != '-' || board[row+1][col] != '-' || board[row+2][col] != '-' || board[row+3][col] != '-')
                {
                    return 303;
                }
            }
            else
            {
                if(row < 0 || col < 0 || col+1 < 0 || col+2 < 0 || col+3 < 0 || row >= num_rows || col >= num_cols || col+1 >= num_cols || col+2 >= num_cols || col+3 >= num_cols)
                {
                    return 302;
                }
                else if(board[row][col] != '-' || board[row][col+1] != '-' || board[row][col+2] != '-' || board[row][col+3] != '-')
                {
                    return 303;
                }
            }
        }
        else if(piece_type == 3)
        {
            if(piece_rotation % 2 == 1)
            {
                if(row < 0 || row-1 < 0 || col < 0 || col+1 < 0 || col+2 < 0 || row >= num_rows || row-1 >= num_rows || col >= num_cols || col+1 >= num_cols || col+2 >= num_cols)
                {
                    return 302;
                }
                else if(board[row][col] != '-' || board[row][col+1] != '-' || board[row-1][col+1] != '-' || board[row-1][col+2] != '-')
                {
                    return 303;
                }
            }
            else
            {
                if(row < 0 || row+1 < 0 || row+2 < 0 || col < 0 || col+1 < 0 || row >= num_rows || row+1 >= num_rows || row+2 >= num_rows || col >= num_cols || col+1 >= num_cols)
                {
                    return 302;
                }
                else if(board[row][col] != '-' || board[row+1][col] != '-' || board[row+1][col+1] != '-' || board[row+2][col+1] != '-')
                {
                    return 303;
                }
            }
        }
        else if(piece_type == 4)
        {
            if(piece_rotation == 1)
            {
                if(row < 0 || row+1 < 0 || row+2 < 0 || col < 0 || col+1 < 0 || row >= num_rows || row+1 >= num_rows || row+2 >= num_rows || col >= num_cols || col+1 >= num_cols)
                {
                    return 302;
                
                }
                else if(board[row][col] != '-' || board[row+1][col] != '-' || board[row+2][col] != '-' || board[row+2][col+1] != '-')
                {
                    return 303;
                }
            }
            else if(piece_rotation == 2)
            {
                if(row < 0 || row+1 < 0 || col < 0 || col+1 < 0 || col+2 < 0 || row >= num_rows || row+1 >= num_rows || col >= num_cols || col+1 >= num_cols || col+2 >= num_cols)
                {
                    return 302;
                }
                else if(board[row][col] != '-' || board[row+1][col] != '-' || board[row][col+1] != '-' || board[row][col+2] != '-')
                {
                    return 303;
                }
            }
            else if(piece_rotation == 3)
            {
                if(row < 0 || row+1 < 0 || row+2 < 0 || col < 0 || col+1 < 0 || row >= num_rows || row+1 >= num_rows || row+2 >= num_rows || col >= num_cols || col+1 >= num_cols)
                {
                    return 302;
                }
                else if(board[row][col] != '-' || board[row][col+1] != '-' || board[row+1][col+1] != '-' || board[row+2][col+1] != '-')
                {
                    return 303;
                }
            }
            else if(piece_rotation == 4)
            {
                if(row < 0 || row-1 < 0 || col < 0 || col+1 < 0 || col+2 < 0 || row >= num_rows || row-1 >= num_rows || col >= num_cols || col+1 >= num_cols || col+2 >= num_cols)
                {
                    return 302;
                }
                else if(board[row][col] != '-' || board[row][col+1] != '-' || board[row][col+2] != '-' || board[row-1][col+2] != '-')
                {
                    return 303;
                }
            }
        }
        else if(piece_type == 5)
        {
            if(piece_rotation % 2 == 1)
            {
                if(row < 0 || row+1 < 0 || col < 0 || col+1 < 0 || col+2 < 0 || row >= num_rows || row+1 >= num_rows || col >= num_cols || col+1 >= num_cols || col+2 >= num_cols)
                {
                    return 302;
                }
                else if(board[row][col] != '-' || board[row][col+1] != '-' || board[row+1][col+1] != '-' || board[row+1][col+2] != '-')
                {
                    return 303;
                }
            }
            else
            {
                if(row < 0 || row+1 < 0 || row-1 < 0 || col < 0 || col+1 < 0 || row >= num_rows || row+1 >= num_rows || row-1 >= num_rows || col >= num_cols || col+1 >= num_cols)
                {
                    return 302;
                }
                else if(board[row][col] != '-' || board[row+1][col] != '-' || board[row][col+1] != '-' || board[row-1][col+1] != '-')
                {
                    return 303;
                }
            }
        }
        else if(piece_type == 6)
        {
            if(piece_rotation == 1)
            {
                if(row < 0 || row-1 < 0 || row-2 < 0 || col < 0 || col+1 < 0 || row >= num_rows || row-1 >= num_rows || row-2 >= num_rows || col >= num_cols || col+1 >= num_cols)
                {   
                    return 302;
                }
                else if(board[row][col] != '-' || board[row][col+1] != '-' || board[row-1][col+1] != '-' || board[row-2][col+1] != '-')
                {
                    return 303;
                }
            }
            else if(piece_rotation == 2)
            {
                if(row < 0 || row+1 < 0 || col < 0 || col+1 < 0 || col+2 < 0 || row >= num_rows || row+1 >= num_rows || col >= num_cols || col+1 >= num_cols || col+2 >= num_cols)
                {
                    return 302;
                }
                else if(board[row][col] != '-' || board[row+1][col] != '-' || board[row+1][col+1] != '-' || board[row+1][col+2] != '-')
                {
                    return 303;
                }
            }
            else if(piece_rotation == 3)
            {
                if(row < 0 || row+1 < 0 || row+2 < 0 || col < 0 || col+1 < 0 || row >= num_rows || row+1 >= num_rows || row+2 >= num_rows || col >= num_cols || col+1 >= num_cols)
                {
                    return 302;
                }
                else if(board[row][col] != '-' || board[row][col+1] != '-' || board[row+1][col] != '-' || board[row+2][col] != '-')
                {
                    return 303;
                }
            }
            else if(piece_rotation == 4)
            {
                if(row < 0 || row+1 < 0 || col < 0 || col+1 < 0 || col+2 < 0 || row >= num_rows || row+1 >= num_rows || col >= num_cols || col+1 >= num_cols || col+2 >= num_cols)
                {
                    return 302;
                }
                else if(board[row][col] != '-' || board[row][col+1] != '-' || board[row][col+2] != '-' || board[row+1][col+2] != '-')
                {
                    return 303;
                }
            }
        }
        else if(piece_type == 7)
        {
            if(piece_rotation == 1)
            {
                if(row < 0 || row+1 < 0 || col < 0 || col+1 < 0 || col+2 < 0 || row >= num_rows || row+1 >= num_rows || col >= num_cols || col+1 >= num_cols || col+2 >= num_cols)
                {
                    return 302;
                }
                else if(board[row][col] != '-' || board[row+1][col+1] != '-' || board[row][col+1] != '-' || board[row][col+2] != '-')
                {
                    return 303;
                }
            }
            else if(piece_rotation == 2)
            {
                if(row < 0 || row-1 < 0 || row+1 < 0 || col < 0 || col+1 < 0 || row >= num_rows || row-1 >= num_rows || row+1 >= num_rows || col >= num_cols || col+1 >= num_cols)
                {
                    return 302;
                }
                else if(board[row][col] != '-' || board[row-1][col+1] != '-' || board[row][col+1] != '-' || board[row+1][col+1] != '-')
                {
                    return 303;
                }
            }
            else if(piece_rotation == 3)
            {
                if(row < 0 || row-1 < 0 || col < 0 || col+1 < 0 || col+2 < 0 || row >= num_rows || row-1 >= num_rows || col >= num_cols || col+1 >= num_cols || col+2 >= num_cols)
                {
                    return 302;
                }
                else if(board[row][col] != '-' || board[row-1][col+1] != '-' || board[row][col+1] != '-' || board[row][col+2] != '-')
                {
                    return 303;
                }
            }
            else if(piece_rotation == 4)
            {
                if(row < 0 || row+1 < 0 || row+2 < 0 || col < 0 || col+1 < 0 || row >= num_rows || row+1 >= num_rows || row+2 >= num_rows || col >= num_cols || col+1 >= num_cols)
                {
                    return 302;
                }
                else if(board[row][col] != '-' || board[row+1][col] != '-' || board[row+1][col+1] != '-' || board[row+2][col] != '-')
                {
                    return 303;
                }
            }
        }
    }
return 1;
}

int check_other_initialize_errors_client_2(char *buffer)
{
    for(int i = 1; i < strlen(buffer); i++) //Change to accomodate not just single-digit params!
    {
        while(buffer[i] != ' ')
            {
            i++; 
            }
            if(i >= strlen(buffer))
            {
                break;
            }
  	        while(buffer[i] == ' ')
            {
            i++; 
            }
            if(i >= strlen(buffer))
            {
                break;
            }

            int piece_type = atoi(&buffer[i]);
            while(buffer[i] != ' ')
            {
            i++; 
            }
  	        while(buffer[i] == ' ')
            {
            i++; 
            }
            
            int piece_rotation = atoi(&buffer[i]);
            while(buffer[i] != ' ')
            {
            i++; 
            }
  	        while(buffer[i] == ' ')
            {
            i++; 
            }
            
            int col = atoi(&buffer[i]);
            while(buffer[i] != ' ')
            {
            i++; 
            }
  	        while(buffer[i] == ' ')
            {
            i++; 
            }

            int row = atoi(&buffer[i]);
        if(piece_type == 1)
        {
            if(row < 0 || row+1 < 0 || col < 0 || col+1 < 0 || row >= num_rows || row+1 >= num_rows || col >= num_cols || col+1 >= num_cols)
            {
            printf("Fails Test 1\n");
            return 302;
            }
            else if(board2[row][col] != '-' || board2[row+1][col] != '-' || board2[row][col+1] != '-' || board2[row+1][col+1] != '-')
            {
                return 303;
            }
        }
        else if(piece_type == 2)
        {
            if(piece_rotation % 2 == 1)
            {
                if(row < 0 || row+1 < 0 || row+2 < 0 || row+3 < 0 || col < 0 || row >= num_rows || row+1 >= num_rows || row+2 >= num_rows || row+3 >= num_rows || col >= num_cols)
                {
                    return 302;
                }
                else if(board2[row][col] != '-' || board2[row+1][col] != '-' || board2[row+2][col] != '-' || board2[row+3][col] != '-')
                {
                    return 303;
                }
            }
            else
            {
                if(row < 0 || col < 0 || col+1 < 0 || col+2 < 0 || col+3 < 0 || row >= num_rows || col >= num_cols || col+1 >= num_cols || col+2 >= num_cols || col+3 >= num_cols)
                {
                    return 302;
                }
                else if(board2[row][col] != '-' || board2[row][col+1] != '-' || board2[row][col+2] != '-' || board2[row][col+3] != '-')
                {
                    return 303;
                }
            }
        }
        else if(piece_type == 3)
        {
            if(piece_rotation % 2 == 1)
            {
                if(row < 0 || row-1 < 0 || col < 0 || col+1 < 0 || col+2 < 0 || row >= num_rows || row-1 >= num_rows || col >= num_cols || col+1 >= num_cols || col+2 >= num_cols)
                {
                    return 302;
                }
                else if(board2[row][col] != '-' || board2[row][col+1] != '-' || board2[row-1][col+1] != '-' || board2[row-1][col+2] != '-')
                {
                    return 303;
                }
            }
            else
            {
                if(row < 0 || row+1 < 0 || row+2 < 0 || col < 0 || col+1 < 0 || row >= num_rows || row+1 >= num_rows || row+2 >= num_rows || col >= num_cols || col+1 >= num_cols)
                {
                    return 302;
                }
                else if(board2[row][col] != '-' || board2[row+1][col] != '-' || board2[row+1][col+1] != '-' || board2[row+2][col+1] != '-')
                {
                    return 303;
                }
            }
        }
        else if(piece_type == 4)
        {
            if(piece_rotation == 1)
            {
                if(row < 0 || row+1 < 0 || row+2 < 0 || col < 0 || col+1 < 0 || row >= num_rows || row+1 >= num_rows || row+2 >= num_rows || col >= num_cols || col+1 >= num_cols)
                {
                    return 302;
                
                }
                else if(board2[row][col] != '-' || board2[row+1][col] != '-' || board2[row+2][col] != '-' || board2[row+2][col+1] != '-')
                {
                    return 303;
                }
            }
            else if(piece_rotation == 2)
            {
                if(row < 0 || row+1 < 0 || col < 0 || col+1 < 0 || col+2 < 0 || row >= num_rows || row+1 >= num_rows || col >= num_cols || col+1 >= num_cols || col+2 >= num_cols)
                {
                    return 302;
                }
                else if(board2[row][col] != '-' || board2[row+1][col] != '-' || board2[row][col+1] != '-' || board2[row][col+2] != '-')
                {
                    return 303;
                }
            }
            else if(piece_rotation == 3)
            {
                if(row < 0 || row+1 < 0 || row+2 < 0 || col < 0 || col+1 < 0 || row >= num_rows || row+1 >= num_rows || row+2 >= num_rows || col >= num_cols || col+1 >= num_cols)
                {
                    return 302;
                }
                else if(board2[row][col] != '-' || board2[row][col+1] != '-' || board2[row+1][col+1] != '-' || board2[row+2][col+1] != '-')
                {
                    return 303;
                }
            }
            else if(piece_rotation == 4)
            {
                if(row < 0 || row-1 < 0 || col < 0 || col+1 < 0 || col+2 < 0 || row >= num_rows || row-1 >= num_rows || col >= num_cols || col+1 >= num_cols || col+2 >= num_cols)
                {
                    return 302;
                }
                else if(board2[row][col] != '-' || board2[row][col+1] != '-' || board2[row][col+2] != '-' || board2[row-1][col+2] != '-')
                {
                    return 303;
                }
            }
        }
        else if(piece_type == 5)
        {
            if(piece_rotation % 2 == 1)
            {
                if(row < 0 || row+1 < 0 || col < 0 || col+1 < 0 || col+2 < 0 || row >= num_rows || row+1 >= num_rows || col >= num_cols || col+1 >= num_cols || col+2 >= num_cols)
                {
                    return 302;
                }
                else if(board2[row][col] != '-' || board2[row][col+1] != '-' || board2[row+1][col+1] != '-' || board2[row+1][col+2] != '-')
                {
                    return 303;
                }
            }
            else
            {
                if(row < 0 || row+1 < 0 || row-1 < 0 || col < 0 || col+1 < 0 || row >= num_rows || row+1 >= num_rows || row-1 >= num_rows || col >= num_cols || col+1 >= num_cols)
                {
                    return 302;
                }
                else if(board2[row][col] != '-' || board2[row+1][col] != '-' || board2[row][col+1] != '-' || board2[row-1][col+1] != '-')
                {
                    return 303;
                }
            }
        }
        else if(piece_type == 6)
        {
            if(piece_rotation == 1)
            {
                if(row < 0 || row-1 < 0 || row-2 < 0 || col < 0 || col+1 < 0 || row >= num_rows || row-1 >= num_rows || row-2 >= num_rows || col >= num_cols || col+1 >= num_cols)
                {   
                    return 302;
                }
                else if(board2[row][col] != '-' || board2[row][col+1] != '-' || board2[row-1][col+1] != '-' || board2[row-2][col+1] != '-')
                {
                    return 303;
                }
            }
            else if(piece_rotation == 2)
            {
                if(row < 0 || row+1 < 0 || col < 0 || col+1 < 0 || col+2 < 0 || row >= num_rows || row+1 >= num_rows || col >= num_cols || col+1 >= num_cols || col+2 >= num_cols)
                {
                    return 302;
                }
                else if(board2[row][col] != '-' || board2[row+1][col] != '-' || board2[row+1][col+1] != '-' || board2[row+1][col+2] != '-')
                {
                    return 303;
                }
            }
            else if(piece_rotation == 3)
            {
                if(row < 0 || row+1 < 0 || row+2 < 0 || col < 0 || col+1 < 0 || row >= num_rows || row+1 >= num_rows || row+2 >= num_rows || col >= num_cols || col+1 >= num_cols)
                {
                    return 302;
                }
                else if(board2[row][col] != '-' || board2[row][col+1] != '-' || board2[row+1][col] != '-' || board2[row+2][col] != '-')
                {
                    return 303;
                }
            }
            else if(piece_rotation == 4)
            {
                if(row < 0 || row+1 < 0 || col < 0 || col+1 < 0 || col+2 < 0 || row >= num_rows || row+1 >= num_rows || col >= num_cols || col+1 >= num_cols || col+2 >= num_cols)
                {
                    return 302;
                }
                else if(board2[row][col] != '-' || board2[row][col+1] != '-' || board2[row][col+2] != '-' || board2[row+1][col+2] != '-')
                {
                    return 303;
                }
            }
        }
        else if(piece_type == 7)
        {
            if(piece_rotation == 1)
            {
                if(row < 0 || row+1 < 0 || col < 0 || col+1 < 0 || col+2 < 0 || row >= num_rows || row+1 >= num_rows || col >= num_cols || col+1 >= num_cols || col+2 >= num_cols)
                {
                    return 302;
                }
                else if(board2[row][col] != '-' || board2[row+1][col+1] != '-' || board2[row][col+1] != '-' || board2[row][col+2] != '-')
                {
                    return 303;
                }
            }
            else if(piece_rotation == 2)
            {
                if(row < 0 || row-1 < 0 || row+1 < 0 || col < 0 || col+1 < 0 || row >= num_rows || row-1 >= num_rows || row+1 >= num_rows || col >= num_cols || col+1 >= num_cols)
                {
                    return 302;
                }
                else if(board2[row][col] != '-' || board2[row-1][col+1] != '-' || board2[row][col+1] != '-' || board2[row+1][col+1] != '-')
                {
                    return 303;
                }
            }
            else if(piece_rotation == 3)
            {
                if(row < 0 || row-1 < 0 || col < 0 || col+1 < 0 || col+2 < 0 || row >= num_rows || row-1 >= num_rows || col >= num_cols || col+1 >= num_cols || col+2 >= num_cols)
                {
                    return 302;
                }
                else if(board2[row][col] != '-' || board2[row-1][col+1] != '-' || board2[row][col+1] != '-' || board2[row][col+2] != '-')
                {
                    return 303;
                }
            }
            else if(piece_rotation == 4)
            {
                if(row < 0 || row+1 < 0 || row+2 < 0 || col < 0 || col+1 < 0 || row >= num_rows || row+1 >= num_rows || row+2 >= num_rows || col >= num_cols || col+1 >= num_cols)
                {
                    return 302;
                }
                else if(board2[row][col] != '-' || board2[row+1][col] != '-' || board2[row+1][col+1] != '-' || board2[row+2][col] != '-')
                {
                    return 303;
                }
            }
        }
    }
return 1;
}


int intialize_board(int piece_type, int piece_rotation, int col, int row, char player_token)
{
    if(piece_type == 1)
    {
        if(board[row][col] != '-' || board[row+1][col] != '-' || board[row][col+1] != '-' || board[row+1][col+1] != '-')
        {
            return 303;
        }
        printf("Passes first test, now initializing!\n");
        board[row][col] = player_token;
        board[row+1][col] = player_token;
        board[row][col+1] = player_token;
        board[row+1][col+1] = player_token;
        return 1;
        

    }
    if(piece_type == 2)
    {
        if(piece_rotation % 2 == 1)
        {

        if(board[row][col] != '-' || board[row+1][col] != '-' || board[row+2][col] != '-' || board[row+3][col] != '-')
        {
            return 303;
        }  
        board[row][col] = player_token;
        board[row+1][col] = player_token;
        board[row+2][col] = player_token;
        board[row+3][col] = player_token;
        return 1;
            
        }
        else
        {
        if(board[row][col] != '-' || board[row][col+1] != '-' || board[row][col+2] != '-' || board[row][col+3] != '-')
        {
            return 303;
        }
        board[row][col] = player_token;
        board[row][col+1] = player_token;
        board[row][col+2] = player_token;
        board[row][col+3] = player_token;
        return 1;
        }
       
    }
    if(piece_type == 3)
    {
        if(piece_rotation % 2 == 1)
        {
        if(board[row][col] != '-' || board[row][col+1] != '-' || board[row-1][col+1] != '-' || board[row-1][col+2] != '-')
        {
            return 303;
        }
        board[row][col] = player_token;
        board[row][col+1] = player_token;
        board[row-1][col+1] = player_token;
        board[row-1][col+2] = player_token;
        return 1;
            

        }
        else
        {
        if(board[row][col] != '-' || board[row+1][col] != '-' || board[row+1][col+1] != '-' || board[row+2][col+1] != '-')
        {
            return 303;
        }
        board[row][col] = player_token;
        board[row+1][col] = player_token;
        board[row+1][col+1] = player_token;
        board[row+2][col+1] = player_token;
        return 1;
            
        }
    }
    if(piece_type == 4)
    {
        if(piece_rotation == 1)
        {
        if(board[row][col] != '-' || board[row+1][col] != '-' || board[row+2][col] != '-' || board[row+2][col+1] != '-')
        {
            return 303;
        }
        board[row][col] = player_token;
        board[row+1][col] = player_token;
        board[row+2][col] = player_token;
        board[row+2][col+1] = player_token;
        return 1;
            
        }
        if(piece_rotation == 2)
        {
        if(board[row][col] != '-' || board[row+1][col] != '-' || board[row][col+1] != '-' || board[row][col+2] != '-')
        {
            return 303;
        }
        board[row][col] = player_token;
        board[row+1][col] = player_token;
        board[row][col+1] = player_token;
        board[row][col+2] = player_token;
        return 1;
        }
        if(piece_rotation == 3)
        {
            if(board[row][col] != '-' || board[row][col+1] != '-' || board[row+1][col+1] != '-' || board[row+2][col+1] != '-')
            {
                return 303;
            }
            board[row][col] = player_token;
            board[row][col+1] = player_token;
            board[row+1][col+1] = player_token;
            board[row+2][col+1] = player_token;
            return 1;
            
        }
        if(piece_rotation == 4)
        {
            if(board[row][col] != '-' || board[row][col+1] != '-' || board[row][col+2] != '-' || board[row-1][col+2] != '-')
            {
                return 303;
            }
            board[row][col] = player_token;
            board[row][col+1] = player_token;
            board[row][col+2] = player_token;
            board[row-1][col+2] = player_token;
            return 1;
            
        }
    }
    if(piece_type == 5)
    {
        
        if(piece_rotation % 2 == 1)
        {
            if(board[row][col] != '-' || board[row][col+1] != '-' || board[row+1][col+1] != '-' || board[row+1][col+2] != '-')
            {
                return 303;
            } 
            board[row][col] = player_token;
            board[row][col+1] = player_token;
            board[row+1][col+1] = player_token;
            board[row+1][col+2] = player_token;
            return 1;
            
        }
        else
        {
            if(board[row][col] != '-' || board[row+1][col] != '-' || board[row][col+1] != '-' || board[row-1][col+1] != '-')
            {
                return 303;
            }
            board[row][col] = player_token;
            board[row+1][col] = player_token;
            board[row][col+1] = player_token;
            board[row-1][col+1] = player_token;
            return 1;
            

        }
    }
    if(piece_type == 6)
    {
        if(piece_rotation == 1)
        {
            if(board[row][col] != '-' || board[row][col+1] != '-' || board[row-1][col+1] != '-' || board[row-2][col+1] != '-')
            {
                return 303;
            }
            board[row][col] = player_token;
            board[row][col+1] = player_token;
            board[row-1][col+1] = player_token;
            board[row-2][col+1] = player_token;
            return 1;
            

        }
        if(piece_rotation == 2)
        {
            if(board[row][col] != '-' || board[row+1][col] != '-' || board[row+1][col+1] != '-' || board[row+1][col+2] != '-')
            {
                return 303;
            }
            board[row][col] = player_token;
            board[row+1][col] = player_token;
            board[row+1][col+1] = player_token;
            board[row+1][col+2] = player_token;
            return 1;

        }
        if(piece_rotation == 3)
        {
            if(board[row][col] != '-' || board[row][col+1] != '-' || board[row+1][col] != '-' || board[row+2][col] != '-')
            {
                return 303;
            }
            board[row][col] = player_token;
            board[row][col+1] = player_token;
            board[row+1][col] = player_token;
            board[row+2][col] = player_token;
            return 1;
            
        }
        if(piece_rotation == 4)
        {
            if(board[row][col] != '-' || board[row][col+1] != '-' || board[row][col+2] != '-' || board[row+1][col+2] != '-')
            {
                return 303;
            }
            board[row][col] = player_token;
            board[row][col+1] = player_token;
            board[row][col+2] = player_token;
            board[row+1][col+1] = player_token;
            return 1;
            
        }
    }
    if(piece_type == 7)
    {
        if(piece_rotation == 1)
        {
            if(board[row][col] != '-' || board[row+1][col+1] != '-' || board[row][col+1] != '-' || board[row][col+2] != '-')
            {
                return 303;
            }
            board[row][col] = player_token;
            board[row+1][col+1] = player_token;
            board[row][col+1] = player_token;
            board[row][col+2] = player_token;
            return 1;
            
        }
        if(piece_rotation == 2)
        {
            if(board[row][col] != '-' || board[row-1][col+1] != '-' || board[row][col+1] != '-' || board[row+1][col+1] != '-')
            {
                return 303;
            }
            board[row][col] = player_token;
            board[row-1][col+1] = player_token;
            board[row][col+1] = player_token;
            board[row+1][col+1] = player_token;
            return 1;

        }
        if(piece_rotation == 3)
        {
            if(board[row][col] != '-' || board[row-1][col+1] != '-' || board[row][col+1] != '-' || board[row][col+2] != '-')
            {
                return 303;
            }
            board[row][col] = player_token;
            board[row-1][col+1] = player_token;
            board[row][col+1] = player_token;
            board[row][col+2] = player_token;
            return 1;
            
        }
        if(piece_rotation == 4)
        {
            if(board[row][col] != '-' || board[row+1][col] != '-' || board[row+1][col+1] != '-' || board[row+2][col] != '-')
            {
                return 303;
            }
            board[row][col] = player_token;
            board[row+1][col] = player_token;
            board[row+1][col+1] = player_token;
            board[row+2][col] = player_token;
            return 1;
            
        }
    }
return 1;
}

int intialize_board_2(int piece_type, int piece_rotation, int col, int row, char player_token)
{
    if(piece_type == 1)
    {
        if(board2[row][col] != '-' || board2[row+1][col] != '-' || board2[row][col+1] != '-' || board2[row+1][col+1] != '-')
        {
            return 303;
        }
        printf("Passes first test, now initializing!\n");
        board2[row][col] = player_token;
        board2[row+1][col] = player_token;
        board2[row][col+1] = player_token;
        board2[row+1][col+1] = player_token;
        return 1;
        

    }
    if(piece_type == 2)
    {
        if(piece_rotation % 2 == 1)
        {
        if(board2[row][col] != '-' || board2[row+1][col] != '-' || board2[row+2][col] != '-' || board2[row+3][col] != '-')
        {
            return 303;
        }    
        board2[row][col] = player_token;
        board2[row+1][col] = player_token;
        board2[row+2][col] = player_token;
        board2[row+3][col] = player_token;
        return 1;
            
        }
        else
        {
        if(board2[row][col] != '-' || board2[row][col+1] != '-' || board2[row][col+2] != '-' || board2[row][col+3] != '-')
        {
            return 303;
        }
        board2[row][col] = player_token;
        board2[row][col+1] = player_token;
        board2[row][col+2] = player_token;
        board2[row][col+3] = player_token;
        return 1;
        }
       
    }
    if(piece_type == 3)
    {
        if(piece_rotation % 2 == 1)
        {
        if(board2[row][col] != '-' || board2[row][col+1] != '-' || board2[row-1][col+1] != '-' || board2[row-1][col+2] != '-')
        {
            return 303;
        }
        board2[row][col] = player_token;
        board2[row][col+1] = player_token;
        board2[row-1][col+1] = player_token;
        board2[row-1][col+2] = player_token;
        return 1;
            

        }
        else
        {
        if(board2[row][col] != '-' || board2[row+1][col] != '-' || board2[row+1][col+1] != '-' || board2[row+2][col+1] != '-')
        {
            return 303;
        }
        board2[row][col] = player_token;
        board2[row+1][col] = player_token;
        board2[row+1][col+1] = player_token;
        board2[row+2][col+1] = player_token;
        return 1;
            
        }
    }
    if(piece_type == 4)
    {
        if(piece_rotation == 1)
        {
        if(board2[row][col] != '-' || board2[row+1][col] != '-' || board2[row+2][col] != '-' || board2[row+2][col+1] != '-')
        {
            return 303;
        }
        board2[row][col] = player_token;
        board2[row+1][col] = player_token;
        board2[row+2][col] = player_token;
        board2[row+2][col+1] = player_token;
        return 1;
            
        }
        if(piece_rotation == 2)
        {
        if(board2[row][col] != '-' || board2[row+1][col] != '-' || board2[row][col+1] != '-' || board2[row][col+2] != '-')
        {
            return 303;
        }
        board2[row][col] = player_token;
        board2[row+1][col] = player_token;
        board2[row][col+1] = player_token;
        board2[row][col+2] = player_token;
        return 1;
        }
        if(piece_rotation == 3)
        {
            if(board2[row][col] != '-' || board2[row][col+1] != '-' || board2[row+1][col+1] != '-' || board2[row+2][col+1] != '-')
            {
                return 303;
            }
            board2[row][col] = player_token;
            board2[row][col+1] = player_token;
            board2[row+1][col+1] = player_token;
            board2[row+2][col+1] = player_token;
            return 1;
            
        }
        if(piece_rotation == 4)
        {
            if(board2[row][col] != '-' || board2[row][col+1] != '-' || board2[row][col+2] != '-' || board2[row-1][col+2] != '-')
            {
                return 303;
            }
            board2[row][col] = player_token;
            board2[row][col+1] = player_token;
            board2[row][col+2] = player_token;
            board2[row-1][col+2] = player_token;
            return 1;
            
        }
    }
    if(piece_type == 5)
    {
        
        if(piece_rotation % 2 == 1)
        {
            if(board2[row][col] != '-' || board2[row][col+1] != '-' || board2[row+1][col+1] != '-' || board2[row+1][col+2] != '-')
            {
                return 303;
            } 
            board2[row][col] = player_token;
            board2[row][col+1] = player_token;
            board2[row+1][col+1] = player_token;
            board2[row+1][col+2] = player_token;
            return 1;
            
        }
        else
        {
            if(board2[row][col] != '-' || board2[row+1][col] != '-' || board2[row][col+1] != '-' || board2[row-1][col+1] != '-')
            {
                return 303;
            }
            board2[row][col] = player_token;
            board2[row+1][col] = player_token;
            board2[row][col+1] = player_token;
            board2[row-1][col+1] = player_token;
            return 1;
            

        }
    }
    if(piece_type == 6)
    {
        if(piece_rotation == 1)
        {
            if(board2[row][col] != '-' || board2[row][col+1] != '-' || board2[row-1][col+1] != '-' || board2[row-2][col+1] != '-')
            {
                return 303;
            }
            board2[row][col] = player_token;
            board2[row][col+1] = player_token;
            board2[row-1][col+1] = player_token;
            board2[row-2][col+1] = player_token;
            return 1;
            

        }
        if(piece_rotation == 2)
        {
            if(board2[row][col] != '-' || board2[row+1][col] != '-' || board2[row+1][col+1] != '-' || board2[row+1][col+2] != '-')
            {
                return 303;
            }
            board2[row][col] = player_token;
            board2[row+1][col] = player_token;
            board2[row+1][col+1] = player_token;
            board2[row+1][col+2] = player_token;
            return 1;

        }
        if(piece_rotation == 3)
        {
            if(board2[row][col] != '-' || board2[row][col+1] != '-' || board2[row+1][col] != '-' || board2[row+2][col] != '-')
            {
                return 303;
            }
            board2[row][col] = player_token;
            board2[row][col+1] = player_token;
            board2[row+1][col] = player_token;
            board2[row+2][col] = player_token;
            return 1;
            
        }
        if(piece_rotation == 4)
        {
            if(board2[row][col] != '-' || board2[row][col+1] != '-' || board2[row][col+2] != '-' || board2[row+1][col+2] != '-')
            {
                return 303;
            }
            board2[row][col] = player_token;
            board2[row][col+1] = player_token;
            board2[row][col+2] = player_token;
            board2[row+1][col+1] = player_token;
            return 1;
            
        }
    }
    if(piece_type == 7)
    {
        if(piece_rotation == 1)
        {
            if(board2[row][col] != '-' || board2[row+1][col+1] != '-' || board2[row][col+1] != '-' || board2[row][col+2] != '-')
            {
                return 303;
            }
            board2[row][col] = player_token;
            board2[row+1][col+1] = player_token;
            board2[row][col+1] = player_token;
            board2[row][col+2] = player_token;
            return 1;
            
        }
        if(piece_rotation == 2)
        {
            if(board2[row][col] != '-' || board2[row-1][col+1] != '-' || board2[row][col+1] != '-' || board2[row+1][col+1] != '-')
            {
                return 303;
            }
            board2[row][col] = player_token;
            board2[row-1][col+1] = player_token;
            board2[row][col+1] = player_token;
            board2[row+1][col+1] = player_token;
            return 1;

        }
        if(piece_rotation == 3)
        {
            if(board2[row][col] != '-' || board2[row-1][col+1] != '-' || board2[row][col+1] != '-' || board2[row][col+2] != '-')
            {
                return 303;
            }
            board2[row][col] = player_token;
            board2[row-1][col+1] = player_token;
            board2[row][col+1] = player_token;
            board2[row][col+2] = player_token;
            return 1;
            
        }
        if(piece_rotation == 4)
        {
            if(board2[row][col] != '-' || board2[row+1][col] != '-' || board2[row+1][col+1] != '-' || board2[row+2][col] != '-')
            {
                return 303;
            }
            board2[row][col] = player_token;
            board2[row+1][col] = player_token;
            board2[row+1][col+1] = player_token;
            board2[row+2][col] = player_token;
            return 1;
            
        }
    }
return 1;
}

int remaining_ships_1(char *stored_initialize) //Here, you will use the stored initialize info to check the number of ships still alive
//Be sure to return the number of ships and handle it based on the tetris piece and rotation! 
{
    int remaining_ships = 5;
    for(int i = 1; i < strlen(stored_initialize); i++)
    {
        while(stored_initialize[i] != ' ')
            {
            i++; 
            }
            if(i >= strlen(stored_initialize))
            {
                break;
            }
  	        while(stored_initialize[i] == ' ')
            {
            i++; 
            }
            if(i >= strlen(stored_initialize))
            {
                break;
            }

            int piece_type = atoi(&stored_initialize[i]);
            while(stored_initialize[i] != ' ')
            {
            i++; 
            }
  	        while(stored_initialize[i] == ' ')
            {
            i++; 
            }
            
            int piece_rotation = atoi(&stored_initialize[i]);
            while(stored_initialize[i] != ' ')
            {
            i++; 
            }
  	        while(stored_initialize[i] == ' ')
            {
            i++; 
            }
            
            int col = atoi(&stored_initialize[i]);
            while(stored_initialize[i] != ' ')
            {
            i++; 
            }
  	        while(stored_initialize[i] == ' ')
            {
            i++; 
            }

            int row = atoi(&stored_initialize[i]);

        if(piece_type == 1)
        {
            if(board[row][col] == '*' && board[row+1][col] == '*' && board[row][col+1] == '*' && board[row+1][col+1] == '*')
            {
                remaining_ships--;
            }
        }
        else if(piece_type == 2)
        {
            if(piece_rotation % 2 == 1)
            {
                if(board[row][col] == '*' && board[row+1][col] == '*' && board[row+2][col] == '*' && board[row+3][col] == '*')
                {
                    remaining_ships--;
                }
            }
            else
            {
                if(board[row][col] == '*' && board[row][col+1] == '*' && board[row][col+2] == '*' && board[row][col+3] == '*')
                {
                    remaining_ships--;
                }
            }
        }
        else if(piece_type == 3)
        {
            if(piece_rotation % 2 == 1)
            {
                if(board[row][col] == '*' && board[row][col+1] == '*' && board[row-1][col+1] == '*' && board[row-1][col+2] == '*')
                {
                    remaining_ships--;
                }
            }
            else
            {
                if(board[row][col] == '*' && board[row+1][col] == '*' && board[row+1][col+1] == '*' && board[row+2][col+1] == '*')
                {
                    remaining_ships--;
                }
            }
        }
        else if(piece_type == 4)
        {
            if(piece_rotation == 1)
            {
                if(board[row][col] == '*' && board[row+1][col] == '*' && board[row+2][col] == '*' && board[row+2][col+1] == '*')
                {
                    remaining_ships--;
                }
            }
            else if(piece_rotation == 2)
            {
                if(board[row][col] == '*' && board[row+1][col] == '*' && board[row][col+1] == '*' && board[row][col+2] == '*')
                {
                    remaining_ships--;
                }
            }
            else if(piece_rotation == 3)
            {
                if(board[row][col] == '*' && board[row][col+1] == '*' && board[row+1][col+1] == '*' && board[row+2][col+1] == '*')
                {
                    remaining_ships--; 
                }
            }
            else
            {
                if(board[row][col] == '*' && board[row][col+1] == '*' && board[row][col+2] == '*' && board[row-1][col+2] == '*')
                {
                    remaining_ships--;
                }
            }
        }
        else if(piece_type == 5)
        {
            if(piece_rotation % 2 == 1)
            {
                if(board[row][col] == '*' && board[row][col+1] == '*' && board[row+1][col+1] == '*' && board[row+1][col+2] == '*')
                {
                    remaining_ships--;
                }
            }
            else
            {
                if(board[row][col] == '*' && board[row+1][col] == '*' && board[row][col+1] == '*' && board[row-1][col+1] == '*')
                {
                    remaining_ships--;
                }
            }
        }   
        else if(piece_type == 6)
        {
            if(piece_rotation == 1)
            {
                if(board[row][col] == '*' && board[row][col+1] == '*' && board[row-1][col+1] == '*' && board[row-2][col+1] == '*')
                {
                    remaining_ships--;
                }
            }
            else if(piece_rotation == 2)
            {
                if(board[row][col] == '*' && board[row+1][col] == '*' && board[row+1][col+1] == '*' && board[row+1][col+2] == '*')
                {
                    remaining_ships--;
                }
            }
            else if(piece_rotation == 3)
            {
                if(board[row][col] == '*' && board[row][col+1] == '*' && board[row+1][col] == '*' && board[row+2][col] == '*')
                {
                    remaining_ships--;
                }
            }
            else
            {
                if(board[row][col] == '*' && board[row][col+1] == '*' && board[row][col+2] == '*' && board[row+1][col+2] == '*')
                {
                    remaining_ships--;
                }
            }

        }
        else if(piece_type == 7)
        {
            if(piece_rotation == 1)
            {
                if(board[row][col] == '*' && board[row+1][col+1] == '*' && board[row][col+1] == '*' && board[row][col+2] == '*')
                {
                    remaining_ships--;
                }
            }
            else if(piece_rotation == 2)
            {
                if(board[row][col] == '*' && board[row-1][col+1] == '*' && board[row][col+1] == '*' && board[row+1][col+1] == '*')
                {
                    remaining_ships--;
                }
            }
            else if(piece_rotation == 3)
            {
                if(board[row][col] == '*' && board[row-1][col+1] == '*' && board[row][col+1] == '*' && board[row][col+2] == '*')
                {
                    remaining_ships--;
                }
            }
            else
            {
                if(board[row][col] == '*' && board[row+1][col] == '*' && board[row+1][col+1] == '*' && board[row+2][col] == '*')
                {
                    remaining_ships--;
                }
            }
        }
    }
    return remaining_ships;
}

int remaining_ships_2(char *stored_initialize) //Here, you will use the stored initialize info to check the number of ships still alive
//Be sure to return the number of ships and handle it based on the tetris piece and rotation! 
{
    int remaining_ships = 5;
    for(int i = 1; i < strlen(stored_initialize); i++)
    {
        while(stored_initialize[i] != ' ')
            {
            i++; 
            }
            if(i >= strlen(stored_initialize))
            {
                break;
            }
  	        while(stored_initialize[i] == ' ')
            {
            i++; 
            }
            if(i >= strlen(stored_initialize))
            {
                break;
            }

            int piece_type = atoi(&stored_initialize[i]);
            while(stored_initialize[i] != ' ')
            {
            i++; 
            }
  	        while(stored_initialize[i] == ' ')
            {
            i++; 
            }
            
            int piece_rotation = atoi(&stored_initialize[i]);
            while(stored_initialize[i] != ' ')
            {
            i++; 
            }
  	        while(stored_initialize[i] == ' ')
            {
            i++; 
            }
            
            int col = atoi(&stored_initialize[i]);
            while(stored_initialize[i] != ' ')
            {
            i++; 
            }
  	        while(stored_initialize[i] == ' ')
            {
            i++; 
            }

            int row = atoi(&stored_initialize[i]);

        if(piece_type == 1)
        {
            if(board2[row][col] == '*' && board2[row+1][col] == '*' && board2[row][col+1] == '*' && board2[row+1][col+1] == '*')
            {
                remaining_ships--;
            }
        }
        else if(piece_type == 2)
        {
            if(piece_rotation % 2 == 1)
            {
                if(board2[row][col] == '*' && board2[row+1][col] == '*' && board2[row+2][col] == '*' && board2[row+3][col] == '*')
                {
                    remaining_ships--;
                }
            }
            else
            {
                if(board2[row][col] == '*' && board2[row][col+1] == '*' && board2[row][col+2] == '*' && board2[row][col+3] == '*')
                {
                    remaining_ships--;
                }
            }
        }
        else if(piece_type == 3)
        {
            if(piece_rotation % 2 == 1)
            {
                if(board2[row][col] == '*' && board2[row][col+1] == '*' && board2[row-1][col+1] == '*' && board2[row-1][col+2] == '*')
                {
                    remaining_ships--;
                }
            }
            else
            {
                if(board2[row][col] == '*' && board2[row+1][col] == '*' && board2[row+1][col+1] == '*' && board2[row+2][col+1] == '*')
                {
                    remaining_ships--;
                }
            }
        }
        else if(piece_type == 4)
        {
            if(piece_rotation == 1)
            {
                if(board2[row][col] == '*' && board2[row+1][col] == '*' && board2[row+2][col] == '*' && board2[row+2][col+1] == '*')
                {
                    remaining_ships--;
                }
            }
            else if(piece_rotation == 2)
            {
                if(board2[row][col] == '*' && board2[row+1][col] == '*' && board2[row][col+1] == '*' && board2[row][col+2] == '*')
                {
                    remaining_ships--;
                }
            }
            else if(piece_rotation == 3)
            {
                if(board2[row][col] == '*' && board2[row][col+1] == '*' && board2[row+1][col+1] == '*' && board2[row+2][col+1] == '*')
                {
                    remaining_ships--; 
                }
            }
            else
            {
                if(board2[row][col] == '*' && board2[row][col+1] == '*' && board2[row][col+2] == '*' && board2[row-1][col+2] == '*')
                {
                    remaining_ships--;
                }
            }
        }
        else if(piece_type == 5)
        {
            if(piece_rotation % 2 == 1)
            {
                if(board2[row][col] == '*' && board2[row][col+1] == '*' && board2[row+1][col+1] == '*' && board2[row+1][col+2] == '*')
                {
                    remaining_ships--;
                }
            }
            else
            {
                if(board2[row][col] == '*' && board2[row+1][col] == '*' && board2[row][col+1] == '*' && board2[row-1][col+1] == '*')
                {
                    remaining_ships--;
                }
            }
        }   
        else if(piece_type == 6)
        {
            if(piece_rotation == 1)
            {
                if(board2[row][col] == '*' && board2[row][col+1] == '*' && board2[row-1][col+1] == '*' && board2[row-2][col+1] == '*')
                {
                    remaining_ships--;
                }
            }
            else if(piece_rotation == 2)
            {
                if(board2[row][col] == '*' && board2[row+1][col] == '*' && board2[row+1][col+1] == '*' && board2[row+1][col+2] == '*')
                {
                    remaining_ships--;
                }
            }
            else if(piece_rotation == 3)
            {
                if(board2[row][col] == '*' && board2[row][col+1] == '*' && board2[row+1][col] == '*' && board2[row+2][col] == '*')
                {
                    remaining_ships--;
                }
            }
            else
            {
                if(board2[row][col] == '*' && board2[row][col+1] == '*' && board2[row][col+2] == '*' && board2[row+1][col+2] == '*')
                {
                    remaining_ships--;
                }
            }

        }
        else if(piece_type == 7)
        {
            if(piece_rotation == 1)
            {
                if(board2[row][col] == '*' && board2[row+1][col+1] == '*' && board2[row][col+1] == '*' && board2[row][col+2] == '*')
                {
                    remaining_ships--;
                }
            }
            else if(piece_rotation == 2)
            {
                if(board2[row][col] == '*' && board2[row-1][col+1] == '*' && board2[row][col+1] == '*' && board2[row+1][col+1] == '*')
                {
                    remaining_ships--;
                }
            }
            else if(piece_rotation == 3)
            {
                if(board2[row][col] == '*' && board2[row-1][col+1] == '*' && board2[row][col+1] == '*' && board2[row][col+2] == '*')
                {
                    remaining_ships--;
                }
            }
            else
            {
                if(board2[row][col] == '*' && board2[row+1][col] == '*' && board2[row+1][col+1] == '*' && board2[row+2][col] == '*')
                {
                    remaining_ships--;
                }
            }
        }
    }
    return remaining_ships;
}

int error_checking(char *buffer, int player_num)//Where the error flagging and real pain in the ass happens. Save for last!!!!!!!
{
    //First check if we're in the Begin Loop via a begin flag
        //This indicates we only need to check for expected Begins or invalid Begin command
    //Secondly, check for Initialize Loop via initialize flag
        //Same Thing: We only check for initialize expected or invalid initialize command (This requires more work considering there are 4 error codes in this section)
    //Finally, check for being in the Game Loop via a Game flag
        //Here we check for expected Q or S commands and invalid S commands!!
    //If no errors are detected, return 1 to show valid command.
    //We need to use itoa to transform the integer into a string before copying it into the buffer Error code.
    //StrTok: not easy, but allows you to check each parameter.
    //Whitespace not cause for error
    
    /*Currently, the code that checks for invalid params breaks the buffer bein inserted, making is null for all three*/
    int error_val = 1;
    if(in_begin)
    {
        if(player_num == 1)
        {
            if(buffer[0] != 'B')
            {
                return 100;
            }

            else if(buffer[0] == 'B')
            {

                printf("Checking Invalid Params\n");
                int param_count = 0;
                for(int i = 1; i < strlen(buffer); i++)
                {
                    if(buffer[i] == ' ')
                    {
                        continue;
                    }
                    else
                    {
                        if(isdigit(buffer[i]) == 0)
                        {
                            return 200;
                        }
                        param_count++;
                        if(param_count >= 3)
                        {
                            return 200;
                        }
                        while(buffer[i] != ' ')
                        {
                            i++;
                        }
                        i--;
                    }
                    //printf("Parameter Count: %d\n", param_count);
                    
                    //printf("%d\n", atoi(&buffer[i]));
                    // else if(i == 2 && atoi(&buffer[i]) < 10)
                    // {
                    //     return 200;
                    // }
                    // else if(i == 4 && atoi(&buffer[i]) < 10)
                    // {
                    //     return 200;
                    // }
                }
            if(param_count != 2)
            {
               return 200;
            }
            
            for(int i = 1; i < strlen(buffer); i++)
            {
                while(buffer[i] != ' ')
                {
                    i++;
                }
                if(i >= strlen(buffer))
                {
                    break;
                }
                while(buffer[i] == ' ')
                {
                    i++;
                }
                if(i >= strlen(buffer))
                {
                    break;
                }
                int param_1 = atoi(&buffer[i]);

                while(buffer[i] != ' ')
                {
                    i++;
                }
                
                while(buffer[i] == ' ')
                {
                    i++;
                }
                
                int param_2 = atoi(&buffer[i]);
                if(param_1 < 10 || param_2 < 10)
                {
                    return 200;
                }
            }
        }
            
        }
        else if(player_num == 2)
        {
            if(buffer[0] != 'B')
            {
                return 100;
            }
            else
            {
                for(int i = 1; i < strlen(buffer); i++)
                {
                    if(buffer[i] != ' ')
                    {
                        return 200;
                    }
                }
                
            }
        }
    }
    else if(in_initialize)
    {
        printf("Inside Initialize Error Checking\n");
        
        
        if(buffer[0] != 'I')
        {
            return 101;
        }
        
        else if(buffer[0] == 'I')
        {
            //printf("Inside param checker!\n");
            int param_count = 0;
            for(int i = 1; i < strlen(buffer); i++)
            {

                if(buffer[i] == ' ')
                {
                    continue;
                }
                else
                {
                    if(isdigit(buffer[i]) == 0)
                    {
                        return 201;
                    }
                    param_count++;
                    //printf("Parameter Count: %d\n", param_count);
                    if(param_count > 20)
                    {
                        return 201;
                    }
                    while(buffer[i] != ' ')
                    {
                        i++;
                    }
                    i--;
                }
            }
            
            if(param_count < 20)
            {
                return 201;
            }
        printf("Now going to piece checking\n");
        
        for(int i = 1; i < strlen(buffer); i++)
        {
            while(buffer[i] != ' ')
            {
            i++; 
            }
            if(i >= strlen(buffer))
            {
                break;
            }
  	        while(buffer[i] == ' ')
            {
            i++; 
            }
            if(i >= strlen(buffer))
            {
                break;
            }

            int piece_type = atoi(&buffer[i]);
            while(buffer[i] != ' ')
            {
            i++; 
            }
  	        while(buffer[i] == ' ')
            {
            i++; 
            }
            
            int piece_rotation = atoi(&buffer[i]);
            while(buffer[i] != ' ')
            {
            i++; 
            }
  	        while(buffer[i] == ' ')
            {
            i++; 
            }
            
            int col = atoi(&buffer[i]);
            while(buffer[i] != ' ')
            {
            i++; 
            }
  	        while(buffer[i] == ' ')
            {
            i++; 
            }

            int row = atoi(&buffer[i]);
            

            printf("Piece Type: %d Piece Rotation: %d within checker\n", piece_type, piece_rotation);
            if(piece_type < 1 || piece_type > 7)
            {
                error_val = 300;
            }
            else if(piece_rotation < 1 || piece_rotation > 4)
            {
                if(error_val != 1 && error_val < 301)
                {
                    continue;
                }
                else
                {
                    error_val = 301;
                }
                
            } //Don't forget to implement error codes 302 and 303!!!!!
        }
        }
    }
    else if(in_game_loop)
    {
        if(buffer[0] != 'S' && buffer[0] != 'Q' && buffer[0] != 'F')
        {
            return 102;
        }
        else if(buffer[0] == 'S')
        {
            int param_count = 0;
            for(int i = 1; i < strlen(buffer); i++)
            {
                if(buffer[i] == ' ')
                {
                    continue;
                }

                else
                {
                    if(isdigit(buffer[i]) == 0)
                    {
                        return 202;
                    }
                    param_count++;
                    if(param_count >= 3)
                    {
                        return 202;
                    }
                    while(buffer[i] != ' ')
                    {
                        i++;
                    }
                    i--;

                }

            }
            if(param_count < 2)
            {
                return 202;
            }
        
            for(int i = 1; i < strlen(buffer); i++)
            {
                while(buffer[i] != ' ')
                {
                    i++;
                }
                if(i >= strlen(buffer))
                {
                    break;
                }
                while(buffer[i] == ' ')
                {
                    i++;
                }
                if(i >= strlen(buffer))
                {
                    break;
                }
                int param_1 = atoi(&buffer[i]);

                while(buffer[i] != ' ')
                {
                    i++;
                }
                while(buffer[i] == ' ')
                {
                    i++;
                }
                int param_2 = atoi(&buffer[i]);
                if(param_1 < 0 || param_1 >= num_rows || param_2 < 0 || param_2 >= num_cols)
                {
                    return 400;
                }
                if(player_num == 1)
                {
                    if(board2[param_1][param_2] == '*')
                    {
                        return 401;
                    }
                }
                else if(player_num == 2)
                {
                    if(board[param_1][param_2] == '*')
                    {
                        return 401;
                    }
                }

            }
        }

       
    }
    if(error_val != 1)
    {
        return error_val;
    }
    return 1;
}    


int main() {
    printf("Forfeit Flag before: %d\n", forfeit_flag);
    int listen_fd, listen_fd2, conn_fd, conn_fd2;
    struct sockaddr_in address, address2;
    int opt = 1;
    int addrlen = sizeof(address);
    int addrlen2 = sizeof(address2);
    char buffer[BUFFER_SIZE] = {0};
    buffer_point = malloc(1000);

    // Create socket
    if ((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Set socket options
    if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))");
        exit(EXIT_FAILURE);
    }
    if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt(server_fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt))");
        exit(EXIT_FAILURE);
    }

    // Bind socket to port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT1);
    if (bind(listen_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("[Server] bind() failed.");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(listen_fd, 3) < 0) {
        perror("[Server] listen() failed.");
        exit(EXIT_FAILURE);
    }

    printf("[Server] Running on port %d\n", PORT1);
    
    if ((listen_fd2 = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Set socket options
    if (setsockopt(listen_fd2, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))");
        exit(EXIT_FAILURE);
    }
    if (setsockopt(listen_fd2, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt(server_fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt))");
        exit(EXIT_FAILURE);
    }

    printf("[Server] Running on port %d\n", PORT2);
    // Bind socket to port
    address2.sin_family = AF_INET;
    address2.sin_addr.s_addr = INADDR_ANY;
    address2.sin_port = htons(PORT2);
    if (bind(listen_fd2, (struct sockaddr *)&address2, sizeof(address)) < 0) {
        perror("[Server] bind() failed.");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(listen_fd2, 3) < 0) {
        perror("[Server] listen() failed.");
        exit(EXIT_FAILURE);
    }
    // Accept incoming connection
    if ((conn_fd = accept(listen_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
        perror("[Server] accept() failed.");
        exit(EXIT_FAILURE);
    }

    // Accept incoming connection
    if ((conn_fd2 = accept(listen_fd2, (struct sockaddr *)&address2, (socklen_t *)&addrlen2)) < 0) {
        perror("[Server] accept() failed.");
        exit(EXIT_FAILURE);
    }
//----------------------------------------------------------------------
    // if ((listen_fd2 = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    //     perror("socket failed");
    //     exit(EXIT_FAILURE);
    // }

    // // Set socket options
    // if (setsockopt(listen_fd2, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
    //     perror("setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))");
    //     exit(EXIT_FAILURE);
    // }
    // if (setsockopt(listen_fd2, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt))) {
    //     perror("setsockopt(server_fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt))");
    //     exit(EXIT_FAILURE);
    // }

    // // Bind socket to port
    // address2.sin_family = AF_INET;
    // address2.sin_addr.s_addr = INADDR_ANY;
    // address2.sin_port = htons(PORT2);
    // if (bind(listen_fd2, (struct sockaddr *)&address2, sizeof(address)) < 0) {
    //     perror("[Server] bind() failed.");
    //     exit(EXIT_FAILURE);
    // }

    // // Listen for incoming connections
    // if (listen(listen_fd2, 3) < 0) {
    //     perror("[Server] listen() failed.");
    //     exit(EXIT_FAILURE);
    // }

    // printf("[Server] Running on port %d\n", PORT2);

    // // Accept incoming connection
    // if ((conn_fd2 = accept(listen_fd2, (struct sockaddr *)&address2, (socklen_t *)&addrlen2)) < 0) {
    //     perror("[Server] accept() failed.");
    //     exit(EXIT_FAILURE);
    // }

    int error = 0;
    bool begin_flag_1 = false;
    buffer_point = malloc(100);
    in_begin = true;
    // Receive and process commands. Most, if not all, of your code will take place here.
    while (1) {
        //Client 1
        memset(buffer, 0, BUFFER_SIZE);
        int nbytes = read(conn_fd, buffer, BUFFER_SIZE);
        if (nbytes <= 0) {
            perror("[Server] read() failed.");
            exit(EXIT_FAILURE);
        }
        printf("[Server] Received from client: %s\n", buffer);
        printf("%d \n", atoi(&buffer[2]));
        if (strcmp(buffer, "F") == 0) {

            sprintf(buffer, "H 0");
            send(conn_fd, buffer, strlen(buffer), 0);
            memset(buffer, 0, BUFFER_SIZE);
            nbytes = read(conn_fd2, buffer, BUFFER_SIZE);

            sprintf(buffer, "H 1");
            send(conn_fd2, buffer, strlen(buffer), 0);
            forfeit_flag = 1;
            break;
        }
        printf("Forfeit Flag before: %d\n", forfeit_flag);
      
        printf("Forfeit Flag before: %d\n", forfeit_flag);
        error = error_checking(buffer, 1);
        printf("Error val: %d\n", error);
        while(error != 1)
        {
            sprintf(buffer, "E %d", error);
            send(conn_fd, buffer, strlen(buffer), 0);
            memset(buffer, 0, BUFFER_SIZE);
            int nbytes = read(conn_fd, buffer, BUFFER_SIZE);
            error = error_checking(buffer, 1);
            
            if (strcmp(buffer, "F") == 0) {
           
            sprintf(buffer, "H 0");
            send(conn_fd, buffer, strlen(buffer), 0);
            memset(buffer, 0, BUFFER_SIZE);
            nbytes = read(conn_fd2, buffer, BUFFER_SIZE);

            sprintf(buffer, "H 1");
            send(conn_fd2, buffer, strlen(buffer), 0);
            forfeit_flag = 1;
            break;
        }
        }
        if(forfeit_flag)
        {
            break;
        }
        for(int i = 1; i < strlen(buffer); i++)
        {
            while(buffer[i] != ' ')
            {
                i++;
            }
                
            while(buffer[i] == ' ')
            {
                i++;
            }
            num_cols = atoi(&buffer[i]);
            while(buffer[i] != ' ')
            {
                i++;
            }
                
            while(buffer[i] == ' ')
            {
                i++;
            }
            num_rows = atoi(&buffer[i]);
            break;
        }
        
        printf("%d %d \n", num_cols, num_rows);
        for(int i = 0; i < num_rows; i++)
        {
            for(int j = 0; j < num_cols; j++)
            {
                board[i][j] = '-';
                printf("%c ", board[i][j]);
            }
            printf("\n");
        }
        //printf("Forfeit Flag before: %d\n", forfeit_flag);
        sprintf(buffer, "A");
        send(conn_fd, buffer, strlen(buffer), 0);
        begin_flag_1 = true;
        printf("Player 1 B Goood\n");
        //printf("Forfeit Flag before: %d\n", forfeit_flag);
        

        
        
        
        //Best Case: Declare global string, then build it up and send it to the buffer. 
        //READ FROM CLIENT 2
        //SEND HALT
        //CLOSE CONNECTION
     //Client 2
        memset(buffer, 0, BUFFER_SIZE);
        nbytes = read(conn_fd2, buffer, BUFFER_SIZE);
        if (nbytes <= 0) {
            perror("[Server] read() failed.");
            exit(EXIT_FAILURE);
        }
        printf("[Server] Received from client: %s\n", buffer);
        if (strcmp(buffer, "F") == 0) {

            sprintf(buffer, "H 0");
            send(conn_fd2, buffer, strlen(buffer), 0);
            memset(buffer, 0, BUFFER_SIZE);
            nbytes = read(conn_fd, buffer, BUFFER_SIZE);
            
            sprintf(buffer, "H 1");
            send(conn_fd, buffer, strlen(buffer), 0);
            forfeit_flag = 1;
            break;
        }
        printf("Forfeit Flag before: %d\n", forfeit_flag);
        error = error_checking(buffer, 2);
        while(error != 1)
        {
            sprintf(buffer, "E %d", error);
            send(conn_fd2, buffer, strlen(buffer), 0);
            memset(buffer, 0, BUFFER_SIZE);
            int nbytes = read(conn_fd2, buffer, BUFFER_SIZE);
            error = error_checking(buffer, 2);
            
            if (strcmp(buffer, "F") == 0) {

            sprintf(buffer, "H 0");
            send(conn_fd2, buffer, strlen(buffer), 0);
            memset(buffer, 0, BUFFER_SIZE);
            nbytes = read(conn_fd, buffer, BUFFER_SIZE);
        
            sprintf(buffer, "H 1");
            send(conn_fd, buffer, strlen(buffer), 0);
            forfeit_flag = 1;
            break;
        }
        }
        if(forfeit_flag)
        {
            break;
        }
        for(int i = 0; i < num_rows; i++)
        {
            for(int j = 0; j < num_cols; j++)
            {
                board2[i][j] = '-';
                printf("%c ", board2[i][j]);
            }
            printf("\n");
        }
        
        sprintf(buffer, "A");
        send(conn_fd2, buffer, strlen(buffer), 0);
        printf("Player 2 B good\n");
        printf("Forfeit Flag before: %d\n", forfeit_flag);
        break;

    }

    printf("Forfeit Flag Status %d\n", forfeit_flag);
    printf("First while done\n");
    
    in_begin = false;
    in_initialize = true;
    int additional_err = 0;
    while(1) //Initialize Loop!
    {
        //client 1
        printf("Inside Initialize Loop\n");
        //printf("New Print\n");
        if(forfeit_flag)
        {
            printf("Forfeit!\n");
            break;
        }
        printf("After forfeit check\n");
        memset(buffer, 0, BUFFER_SIZE);
        
        int nbytes = read(conn_fd, buffer, BUFFER_SIZE);
        if (nbytes <= 0) {
            perror("[Server] read() failed.");
            exit(EXIT_FAILURE);
        }
        printf("[Server] Received from client1: %s\n", buffer);
        printf("Still in Initialize 1\n");
        if (strcmp(buffer, "F") == 0) {
            memset(buffer, 0, BUFFER_SIZE);
            sprintf(buffer, "H 0");
            send(conn_fd, buffer, strlen(buffer), 0);
            memset(buffer, 0, BUFFER_SIZE);
            nbytes = read(conn_fd2, buffer, BUFFER_SIZE);
            memset(buffer, 0, BUFFER_SIZE);
            sprintf(buffer, "H 1");
            send(conn_fd2, buffer, strlen(buffer), 0);
            forfeit_flag = true;
            printf("[Server] Shutting down.\n"); //Change the logic to maintain connection to second client when first client quits.
            close(conn_fd);
            close(listen_fd);
            close(listen_fd2);
            close(conn_fd2);
            return EXIT_SUCCESS;
            break;
        }
        printf("Still in Initialize 2\n");
        printf("%s\n", buffer);
        
        error = error_checking(buffer, 1);
        printf("Error Val: %d\n", error);
        additional_err = check_other_initialize_errors_client_1(buffer);
        printf("Additional Error Val: %d\n", additional_err);
        printf("%s\n", buffer);
        while(error != 1 || additional_err != 1)
        {
            if(error != 1)
            {
                memset(buffer, 0, BUFFER_SIZE);
                sprintf(buffer, "E %d", error);
                send(conn_fd, buffer, strlen(buffer), 0);
                memset(buffer, 0, BUFFER_SIZE);
            }
            else if(additional_err != 1)
            {
                memset(buffer, 0, BUFFER_SIZE);
                sprintf(buffer, "E %d", additional_err);
                send(conn_fd, buffer, strlen(buffer), 0);
                memset(buffer, 0, BUFFER_SIZE);
            }
            
            memset(buffer, 0, BUFFER_SIZE);
            int nbytes = read(conn_fd, buffer, BUFFER_SIZE);
            printf("%s\n", buffer);
            
            if (strcmp(buffer, "F") == 0) {
            memset(buffer, 0, BUFFER_SIZE);
            sprintf(buffer, "H 0");
            send(conn_fd, buffer, strlen(buffer), 0);
            memset(buffer, 0, BUFFER_SIZE);
            nbytes = read(conn_fd2, buffer, BUFFER_SIZE);
            memset(buffer, 0, BUFFER_SIZE);
            sprintf(buffer, "H 1");
            send(conn_fd2, buffer, strlen(buffer), 0);
            memset(buffer, 0, BUFFER_SIZE);
            forfeit_flag = true;
            printf("[Server] Shutting down.\n"); //Change the logic to maintain connection to second client when first client quits.
            close(conn_fd);
            close(listen_fd);
            close(listen_fd2);
            close(conn_fd2);
            return EXIT_SUCCESS;
            break;
        }

            error = error_checking(buffer, 1);
            additional_err = check_other_initialize_errors_client_1(buffer);
            

        }
        if(forfeit_flag)
        {
            break;
        }
        printf("%s\n", buffer);
        for(int i = 1; i < strlen(buffer); i++) 
        {
            while(buffer[i] != ' ')
            {
            i++; 
            }
            if(i >= strlen(buffer))
            {
                break;
            }
  	        while(buffer[i] == ' ')
            {
            i++; 
            }
            if(i >= strlen(buffer))
            {
                break;
            }

            int piece_type = atoi(&buffer[i]);
            while(buffer[i] != ' ')
            {
            i++; 
            }
  	        while(buffer[i] == ' ')
            {
            i++; 
            }
            
            int piece_rotation = atoi(&buffer[i]);
            while(buffer[i] != ' ')
            {
            i++; 
            }
  	        while(buffer[i] == ' ')
            {
            i++; 
            }
            
            int col = atoi(&buffer[i]);
            while(buffer[i] != ' ')
            {
            i++; 
            }
  	        while(buffer[i] == ' ')
            {
            i++; 
            }

            int row = atoi(&buffer[i]);

            printf("Piece: %d Rotation: %d col: %d row: %d\n", piece_type, piece_rotation, col, row);
            printf("%c %c %c %c\n", board[row][col], board[row+1][col], board[row][col+1], board[row+1][col+1]);
            int error_303 = intialize_board(piece_type, piece_rotation, col, row, '@');
            printf("%c %c %c %c\n", board[row][col], board[row+1][col], board[row][col+1], board[row+1][col+1]);
            printf("%s\n", buffer);
            if(error_303 == 303)
            {
                for(int row = 0; row < num_rows; row++)
                {
                    for(int col = 0; col < num_cols; col++)
                    {
                    board[row][col] = '-';
                    printf("%c ", board[row][col]);
                    }
                    printf("\n");
                }
                memset(buffer, 0, BUFFER_SIZE);
                sprintf(buffer, "E %d", error_303);
                send(conn_fd, buffer, strlen(buffer), 0);
                memset(buffer, 0, BUFFER_SIZE);
                int nbytes = read(conn_fd, buffer, BUFFER_SIZE);
                printf("%s\n", buffer);
                error = error_checking(buffer, 1);
                printf("Error Val: %d\n", error);
                additional_err = check_other_initialize_errors_client_1(buffer);
                printf("Additional Error Val: %d\n", additional_err);
                if (strcmp(buffer, "F") == 0) {
                    memset(buffer, 0, BUFFER_SIZE);
                    sprintf(buffer, "H 0");
                    send(conn_fd, buffer, strlen(buffer), 0);
                    memset(buffer, 0, BUFFER_SIZE);
                    nbytes = read(conn_fd2, buffer, BUFFER_SIZE);
                    memset(buffer, 0, BUFFER_SIZE);
                    sprintf(buffer, "H 1");
                    send(conn_fd2, buffer, strlen(buffer), 0);
                    memset(buffer, 0, BUFFER_SIZE);
                    forfeit_flag = true;
                    printf("[Server] Shutting down.\n"); //Change the logic to maintain connection to second client when first client quits.
                    close(conn_fd);
                    close(listen_fd);
                    close(listen_fd2);
                    close(conn_fd2);
                    return EXIT_SUCCESS;
                    break;
                    }
                while(error != 1 || additional_err != 1)
                {

                    if(error != 1)
                    {
                        memset(buffer, 0, BUFFER_SIZE);
                        sprintf(buffer, "E %d", error);
                        send(conn_fd, buffer, strlen(buffer), 0);
                        memset(buffer, 0, BUFFER_SIZE);
                    }
                    else if(additional_err != 1)
                    {
                        memset(buffer, 0, BUFFER_SIZE);
                        sprintf(buffer, "E %d", additional_err);
                        send(conn_fd, buffer, strlen(buffer), 0);
                        memset(buffer, 0, BUFFER_SIZE);
                    }
            
                    memset(buffer, 0, BUFFER_SIZE);
                    int nbytes = read(conn_fd, buffer, BUFFER_SIZE);
                    printf("%s\n", buffer);
                    
                    if (strcmp(buffer, "F") == 0) {
                    memset(buffer, 0, BUFFER_SIZE);
                    sprintf(buffer, "H 0");
                    send(conn_fd, buffer, strlen(buffer), 0);
                    memset(buffer, 0, BUFFER_SIZE);
                    nbytes = read(conn_fd2, buffer, BUFFER_SIZE);
                    memset(buffer, 0, BUFFER_SIZE);
                    sprintf(buffer, "H 1");
                    send(conn_fd2, buffer, strlen(buffer), 0);
                    memset(buffer, 0, BUFFER_SIZE);
                    forfeit_flag = true;
                    printf("[Server] Shutting down.\n"); //Change the logic to maintain connection to second client when first client quits.
                    close(conn_fd);
                    close(listen_fd);
                    close(listen_fd2);
                    close(conn_fd2);
                    return EXIT_SUCCESS;
                    break;
                    }
                    error = error_checking(buffer, 1);
                    additional_err = check_other_initialize_errors_client_1(buffer);
                } 
            if(forfeit_flag)
            {
                break;
            }           
            i = 0;
            }
        }
        if(forfeit_flag)
        {
            break;
        }
        printf("Not in while loop");
        
        for(int row = 0; row < num_rows; row++)
        {
            for(int col = 0; col < num_cols; col++)
            {
            board[row][col] = '-';
            printf("%c ", board[row][col]);
            }
            printf("\n");
        }

        printf("%d\n ", atoi(&buffer[2]));
        for(int i = 1; i < strlen(buffer); i++) 
        {
            while(buffer[i] != ' ')
            {
            i++; 
            }
            if(i >= strlen(buffer))
            {
                break;
            }
  	        while(buffer[i] == ' ')
            {
            i++; 
            }
            if(i >= strlen(buffer))
            {
                break;
            }

            int piece_type = atoi(&buffer[i]);
            while(buffer[i] != ' ')
            {
            i++; 
            }
  	        while(buffer[i] == ' ')
            {
            i++; 
            }
            
            int piece_rotation = atoi(&buffer[i]);
            while(buffer[i] != ' ')
            {
            i++; 
            }
  	        while(buffer[i] == ' ')
            {
            i++; 
            }
            
            int col = atoi(&buffer[i]);
            while(buffer[i] != ' ')
            {
            i++; 
            }
  	        while(buffer[i] == ' ')
            {
            i++; 
            }

            int row = atoi(&buffer[i]);

            printf("Piece: %d Rotation: %d col: %d row: %d\n", piece_type, piece_rotation, col, row);
            printf("%c %c %c %c\n", board[row][col], board[row+1][col], board[row][col+1], board[row+1][col+1]);
            int error = intialize_board(piece_type, piece_rotation, col, row, '@');
            printf("%c %c %c %c\n", board[row][col], board[row+1][col], board[row][col+1], board[row+1][col+1]);
   
        }
        printf("Board char at 0 0: %c\n", board[0][0]);

        
                
        printf("Entered Client 1 to store Init\n");
        strcpy(store_player_1_init, buffer);
        memset(buffer, 0, BUFFER_SIZE);
        sprintf(buffer, "A");
        send(conn_fd, buffer, strlen(buffer), 0);
        memset(buffer, 0, BUFFER_SIZE);
        //client 2

        memset(buffer, 0, BUFFER_SIZE);
        nbytes = read(conn_fd2, buffer, BUFFER_SIZE);
        if (nbytes <= 0) {
            perror("[Server] read() failed.");
            exit(EXIT_FAILURE);
        }
        

        printf("[Server] Received from client2: %s\n", buffer);
        if (strcmp(buffer, "F") == 0) {
            memset(buffer, 0, BUFFER_SIZE);
            sprintf(buffer, "H 0");
            send(conn_fd2, buffer, strlen(buffer), 0);
            memset(buffer, 0, BUFFER_SIZE);
            nbytes = read(conn_fd, buffer, BUFFER_SIZE);
            memset(buffer, 0, BUFFER_SIZE);
            sprintf(buffer, "H 1");
            send(conn_fd, buffer, strlen(buffer), 0);
            forfeit_flag = true;
            printf("[Server] Shutting down.\n"); //Change the logic to maintain connection to second client when first client quits.
            close(conn_fd);
            close(listen_fd);
            close(listen_fd2);
            close(conn_fd2);
            return EXIT_SUCCESS;
            break;
        }
        error = error_checking(buffer, 2);
        additional_err = check_other_initialize_errors_client_2(buffer);
        while(error != 1 || additional_err != 1)
        {
            if(error != 1)
            {
                memset(buffer, 0, BUFFER_SIZE);
                sprintf(buffer, "E %d", error);
                send(conn_fd2, buffer, strlen(buffer), 0);
            }
            else if(additional_err != 1)
            {
                memset(buffer, 0, BUFFER_SIZE);
                sprintf(buffer, "E %d", additional_err);
                send(conn_fd2, buffer, strlen(buffer), 0);
            }
            memset(buffer, 0, BUFFER_SIZE);
            int nbytes = read(conn_fd2, buffer, BUFFER_SIZE);
            if (strcmp(buffer, "F") == 0) {
            memset(buffer, 0, BUFFER_SIZE);
            sprintf(buffer, "H 0");
            send(conn_fd2, buffer, strlen(buffer), 0);
            memset(buffer, 0, BUFFER_SIZE);
            nbytes = read(conn_fd, buffer, BUFFER_SIZE);
            memset(buffer, 0, BUFFER_SIZE);
            sprintf(buffer, "H 1");
            send(conn_fd, buffer, strlen(buffer), 0);
            forfeit_flag = true;
            printf("[Server] Shutting down.\n"); //Change the logic to maintain connection to second client when first client quits.
            close(conn_fd);
            close(listen_fd);
            close(listen_fd2);
            close(conn_fd2);
            return EXIT_SUCCESS;
            break;
        }
            
            error = error_checking(buffer, 2);
            additional_err = check_other_initialize_errors_client_2(buffer);
        }
        if(forfeit_flag)
        {
            break;
        }

        for(int i = 1; i < strlen(buffer); i++) 
        {
            while(buffer[i] != ' ')
            {
            i++; 
            }
            if(i >= strlen(buffer))
            {
                break;
            }
  	        while(buffer[i] == ' ')
            {
            i++; 
            }
            if(i >= strlen(buffer))
            {
                break;
            }

            int piece_type = atoi(&buffer[i]);
            while(buffer[i] != ' ')
            {
            i++; 
            }
  	        while(buffer[i] == ' ')
            {
            i++; 
            }
            
            int piece_rotation = atoi(&buffer[i]);
            while(buffer[i] != ' ')
            {
            i++; 
            }
  	        while(buffer[i] == ' ')
            {
            i++; 
            }
            
            int col = atoi(&buffer[i]);
            while(buffer[i] != ' ')
            {
            i++; 
            }
  	        while(buffer[i] == ' ')
            {
            i++; 
            }

            int row = atoi(&buffer[i]);

            printf("Piece: %d Rotation: %d col: %d row: %d\n", piece_type, piece_rotation, col, row);
            printf("%c %c %c %c\n", board2[row][col], board2[row+1][col], board2[row][col+1], board2[row+1][col+1]);
            int error_303 = intialize_board_2(piece_type, piece_rotation, col, row, '@');
            printf("%c %c %c %c\n", board2[row][col], board2[row+1][col], board2[row][col+1], board2[row+1][col+1]);
            if(error_303 == 303)
            {
                for(int row = 0; row < num_rows; row++)
                {
                    for(int col = 0; col < num_cols; col++)
                    {
                    board2[row][col] = '-';
                    printf("%c ", board2[row][col]);
                    }
                    printf("\n");
                }
                memset(buffer, 0, BUFFER_SIZE);
                sprintf(buffer, "E %d", error_303);
                send(conn_fd2, buffer, strlen(buffer), 0);
                memset(buffer, 0, BUFFER_SIZE);
                int nbytes = read(conn_fd2, buffer, BUFFER_SIZE);
                error = error_checking(buffer, 2);
                printf("Error Val: %d\n", error);
                additional_err = check_other_initialize_errors_client_2(buffer);
                printf("Additional Error Val: %d\n", additional_err);
                if (strcmp(buffer, "F") == 0) {
                    memset(buffer, 0, BUFFER_SIZE);
                    sprintf(buffer, "H 0");
                    send(conn_fd2, buffer, strlen(buffer), 0);
                    memset(buffer, 0, BUFFER_SIZE);
                    nbytes = read(conn_fd, buffer, BUFFER_SIZE);
                    memset(buffer, 0, BUFFER_SIZE);
                    sprintf(buffer, "H 1");
                    send(conn_fd, buffer, strlen(buffer), 0);
                    forfeit_flag = true;
                    printf("[Server] Shutting down.\n"); //Change the logic to maintain connection to second client when first client quits.
                    close(conn_fd);
                    close(listen_fd);
                    close(listen_fd2);
                    close(conn_fd2);
                    return EXIT_SUCCESS;
                    break;
                    }
                while(error != 1 || additional_err != 1)
                {

                    if(error != 1)
                    {
                        memset(buffer, 0, BUFFER_SIZE);
                        sprintf(buffer, "E %d", error);
                        send(conn_fd2, buffer, strlen(buffer), 0);
                    }
                    else if(additional_err != 1)
                    {
                        memset(buffer, 0, BUFFER_SIZE);
                        sprintf(buffer, "E %d", additional_err);
                        send(conn_fd2, buffer, strlen(buffer), 0);
                    }
            
                    memset(buffer, 0, BUFFER_SIZE);
                    int nbytes = read(conn_fd2, buffer, BUFFER_SIZE);
                    if (strcmp(buffer, "F") == 0) {
                    memset(buffer, 0, BUFFER_SIZE);
                    sprintf(buffer, "H 0");
                    send(conn_fd2, buffer, strlen(buffer), 0);
                    memset(buffer, 0, BUFFER_SIZE);
                    nbytes = read(conn_fd, buffer, BUFFER_SIZE);
                    memset(buffer, 0, BUFFER_SIZE);
                    sprintf(buffer, "H 1");
                    send(conn_fd, buffer, strlen(buffer), 0);
                    forfeit_flag = true;
                    printf("[Server] Shutting down.\n"); //Change the logic to maintain connection to second client when first client quits.
                    close(conn_fd);
                    close(listen_fd);
                    close(listen_fd2);
                    close(conn_fd2);
                    return EXIT_SUCCESS;
                    break;
                    }
                    error = error_checking(buffer, 2);
                    additional_err = check_other_initialize_errors_client_2(buffer);
                } 
            if(forfeit_flag)
            {
                break;
            }           
            i = 0;
            }
        }
        if(forfeit_flag)
        {
            break;
        }
        printf("Not in while loop");
        
        for(int row = 0; row < num_rows; row++)
        {
            for(int col = 0; col < num_cols; col++)
            {
            board2[row][col] = '-';
            printf("%c ", board2[row][col]);
            }
            printf("\n");
        }

        for(int i = 1; i < strlen(buffer); i++)
        {
            while(buffer[i] != ' ')
            {
            i++; 
            }
            if(i >= strlen(buffer))
            {
                break;
            }
  	        while(buffer[i] == ' ')
            {
            i++; 
            }
            if(i >= strlen(buffer))
            {
                break;
            }

            int piece_type = atoi(&buffer[i]);
            while(buffer[i] != ' ')
            {
            i++; 
            }
  	        while(buffer[i] == ' ')
            {
            i++; 
            }
            
            int piece_rotation = atoi(&buffer[i]);
            while(buffer[i] != ' ')
            {
            i++; 
            }
  	        while(buffer[i] == ' ')
            {
            i++; 
            }
            
            int col = atoi(&buffer[i]);
            while(buffer[i] != ' ')
            {
            i++; 
            }
  	        while(buffer[i] == ' ')
            {
            i++; 
            }

            int row = atoi(&buffer[i]);

            printf("Piece: %d Rotation: %d col: %d row: %d\n", piece_type, piece_rotation, col, row);
            printf("%c %c %c %c\n", board2[row][col], board2[row+1][col], board2[row][col+1], board2[row+1][col+1]);
            int error = intialize_board_2(piece_type, piece_rotation, col, row, '&');
            printf("%c %c %c %c\n", board2[row][col], board2[row+1][col], board2[row][col+1], board2[row+1][col+1]);
            
        }
        printf("Board char at 0 0: %c\n", board2[0][0]);

        printf("Out of Client 2 for-loop\n");
        
        
        
            printf("Entered Client2 to Proceed to Game\n");
            strcpy(store_player_2_init, buffer);
            printf("Stored Client2 Init\n");
            memset(buffer, 0, BUFFER_SIZE);
            sprintf(buffer, "A");
            send(conn_fd2, buffer, strlen(buffer), 0);
            break;
        
        
    }

    printf("Out of Initialize Loop\n");
    //Player 1: @, Player 2: &
    //Q should have a response where you provide the client: G num of ships, then every turn! Turns should be saved by string!
    //bool forfeit_flag = false;
    player_1_query = malloc(10000);
    player_2_query = malloc(10000);
    in_initialize = false;
    in_game_loop = true;
    
    for(int i = 0; i < num_rows; i++)
    {
        for(int j = 0; j < num_cols; j++)
        {
            printf("%c ", board[i][j]);
        }
        printf("\n");
    }
    printf("---------------------------------\n");

    for(int i = 0; i < num_rows; i++)
    {
        for(int j = 0; j < num_cols; j++)
        {
            printf("%c ", board2[i][j]);
        }
        printf("\n");
    }
    
    while(1) //Actual Game Loop! Implement an error-checking function that takes in bool flags depending on where you are in the loops!!!
    {
        printf("Inside Actual Game Loop");
        //client 1
        if(forfeit_flag)
        {
            break;
        }
        
        
        memset(buffer, 0, BUFFER_SIZE);
        int nbytes = read(conn_fd, buffer, BUFFER_SIZE);
        if (nbytes <= 0) {
            perror("[Server] read() failed.");
            exit(EXIT_FAILURE);
        }
        printf("[Server] Received from client: %s\n", buffer);
        
        

        error = error_checking(buffer, 1);
        while(error != 1)
        {
            memset(buffer, 0, BUFFER_SIZE);
            sprintf(buffer, "E %d", error);
            send(conn_fd, buffer, strlen(buffer), 0);
            memset(buffer, 0, BUFFER_SIZE);
            int nbytes = read(conn_fd, buffer, BUFFER_SIZE);
            error = error_checking(buffer, 1);
        }

        while(buffer[0] != 'S')
        {
            if(buffer[0] == 'Q')
            {
            printf("Entered Query for Client 1\n");
            int number_of_ships_left = remaining_ships_1(store_player_1_init);
            printf("After checking number of ships: %d\n", number_of_ships_left);    
            //char begin_of_query[100]; //A bit overkill
            printf("Char Array Created\n");
            memset(buffer, 0, BUFFER_SIZE);
            sprintf(buffer, "G %d%s", number_of_ships_left, player_1_query); //player_1_query when you get in the loop at the first time is null! Make proper changes.
            printf("Query Check 1\n");
            //Seg Fault happens here!!!
            
            //strcat(begin_of_query, player_1_query);
            printf("Query Check 2\n");
            
            printf("Query Check 3\n");
            send(conn_fd, buffer, strlen(buffer), 0);
            }
            else if(buffer[0] == 'F')
            {
            memset(buffer, 0, BUFFER_SIZE);
            sprintf(buffer, "H 0");
            send(conn_fd, buffer, strlen(buffer), 0);
            memset(buffer, 0, BUFFER_SIZE);
            nbytes = read(conn_fd2, buffer, BUFFER_SIZE);
            memset(buffer, 0, BUFFER_SIZE);
            sprintf(buffer, "H 1");
            send(conn_fd2, buffer, strlen(buffer), 0);
            forfeit_flag =  true;
            printf("[Server] Shutting down.\n"); //Change the logic to maintain connection to second client when first client quits.
            close(conn_fd);
            close(listen_fd);
            close(listen_fd2);
            close(conn_fd2);
            return EXIT_SUCCESS;
            break;
            }
            memset(buffer, 0, BUFFER_SIZE);
            nbytes = read(conn_fd, buffer, BUFFER_SIZE);
            error = error_checking(buffer, 1);
            while(error != 1)
            {
                memset(buffer, 0, BUFFER_SIZE);
                sprintf(buffer, "E %d", error);
                send(conn_fd, buffer, strlen(buffer), 0);
                memset(buffer, 0, BUFFER_SIZE);
                int nbytes = read(conn_fd, buffer, BUFFER_SIZE);
                error = error_checking(buffer, 1);
            }
        }
        
        if(forfeit_flag)
        {
            break;
        }
        char add_to_query[100], add_to_buffer[100];
        //printf("%c %d %d ", buffer[0], atoi(&buffer[2]), atoi(&buffer[4]));
        //printf("%c\n", board[atoi(&buffer[2])][atoi(&buffer[2])]);
        int row, col;
        for(int i = 1; i < strlen(buffer); i++)
        {
            while(buffer[i] != ' ')
            {
            i++; 
            }
  	        while(buffer[i] == ' ')
            {
            i++; 
            }
            row = atoi(&buffer[i]);

            while(buffer[i] != ' ')
            {
            i++; 
            }
  	        while(buffer[i] == ' ')
            {
            i++; 
            }
            col = atoi(&buffer[i]);
            break;
        }
            printf("Inside Client 1 check Row: %d Col: %d\n Board Char: %c\n", row, col, board[row][col]);
            if(board2[row][col] == '-')
            {
                board2[row][col] = '*';
                printf("Inside C1 Miss\n");
                sprintf(add_to_query, " M %d %d", row, col);
                
                strcat(player_1_query, add_to_query);
                int number_of_ships_left = remaining_ships_2(store_player_2_init);
                memset(buffer, 0, BUFFER_SIZE);
                sprintf(buffer ,"R %d M", number_of_ships_left);
                
                send(conn_fd, buffer, strlen(buffer), 0);
                memset(buffer, 0, BUFFER_SIZE);
            }
            else if(board2[row][col] == '&')
            {
                board2[row][col] = '*';

                printf("Inside C1 Hit\n");
                sprintf(add_to_query, " H %d %d", row, col);
                printf("Hit Check 1\n");
                strcat(player_1_query, add_to_query);
                printf("Hit Check 2\n");
                int number_of_ships_left = remaining_ships_2(store_player_2_init);
                memset(buffer, 0, BUFFER_SIZE);
                sprintf(buffer, "R %d H", number_of_ships_left);
                printf("Hit Check 3\n");
                
                printf("Hit Check 4\n");
                send(conn_fd, buffer, strlen(buffer), 0);
                memset(buffer, 0, BUFFER_SIZE);
            }

        if(remaining_ships_2(store_player_2_init) == 0)
        {
            memset(buffer, 0, BUFFER_SIZE);
            int nbytes = read(conn_fd2, buffer, BUFFER_SIZE);
            memset(buffer, 0, BUFFER_SIZE);
            sprintf(buffer, "H 0");
            send(conn_fd2, buffer, strlen(buffer), 0);
            memset(buffer, 0, BUFFER_SIZE);
            nbytes = read(conn_fd, buffer, BUFFER_SIZE);
            memset(buffer, 0, BUFFER_SIZE);
            sprintf(buffer, "H 1");
            send(conn_fd, buffer, strlen(buffer), 0);
            memset(buffer, 0, BUFFER_SIZE);
            forfeit_flag = true;
            printf("[Server] Shutting down.\n"); //Change the logic to maintain connection to second client when first client quits.
            close(conn_fd);
            close(listen_fd);
            close(listen_fd2);
            close(conn_fd2);
            return EXIT_SUCCESS;
            break;  
        }
        
        for(int i = 0; i < num_rows; i++)
        {
            for(int j = 0; j < num_cols; j++)
            {
                printf("%c ", board[i][j]);
            }
            printf("\n");
        }

        //Implementation for second client
        printf("Onto Client 2 Turn!\n");
        memset(buffer, 0, BUFFER_SIZE);
        nbytes = read(conn_fd2, buffer, BUFFER_SIZE);
        if (nbytes <= 0) {
            perror("[Server] read() failed.");
            exit(EXIT_FAILURE);
        }
        printf("[Server] Received from client2: %s\n", buffer);
        error = error_checking(buffer, 2);
        while(error != 1)
        {
            memset(buffer, 0, BUFFER_SIZE);
            sprintf(buffer, "E %d", error);
            send(conn_fd2, buffer, strlen(buffer), 0);
            memset(buffer, 0, BUFFER_SIZE);
            int nbytes = read(conn_fd2, buffer, BUFFER_SIZE);
            error = error_checking(buffer, 2);
        }
        while(buffer[0] != 'S')
        {
            if(buffer[0] == 'Q')
            {
            int number_of_ships_left = remaining_ships_2(store_player_2_init);
            memset(buffer, 0, BUFFER_SIZE);
            sprintf(buffer, "G %d%s", number_of_ships_left, player_2_query);
            send(conn_fd2, buffer, strlen(buffer), 0);
            }
            else if(buffer[0] == 'F')
            {
            memset(buffer, 0, BUFFER_SIZE);
            sprintf(buffer,"H 0");
            send(conn_fd2, buffer, strlen(buffer), 0);
            memset(buffer, 0, BUFFER_SIZE);
            nbytes = read(conn_fd, buffer, BUFFER_SIZE);
            memset(buffer, 0, BUFFER_SIZE);
            sprintf(buffer, "H 1");
            send(conn_fd, buffer, strlen(buffer), 0);
            forfeit_flag =  true;
            printf("[Server] Shutting down.\n"); //Change the logic to maintain connection to second client when first client quits.
            close(conn_fd);
            close(listen_fd);
            close(listen_fd2);
            close(conn_fd2);
            return EXIT_SUCCESS;
            break;
            }
            memset(buffer, 0, BUFFER_SIZE);
            nbytes = read(conn_fd2, buffer, BUFFER_SIZE);
            error = error_checking(buffer, 2);
            while(error != 1)
            {
                memset(buffer, 0, BUFFER_SIZE);
                sprintf(buffer, "E %d", error);
                send(conn_fd2, buffer, strlen(buffer), 0);
                memset(buffer, 0, BUFFER_SIZE);
                int nbytes = read(conn_fd2, buffer, BUFFER_SIZE);
                error = error_checking(buffer, 2);
            }
        }
        printf("Valid Shoot for Client 2\n");
        if(forfeit_flag)
        {
            break;
        }
        printf("After client2 forfeit check\n");
        for(int i = 1; i < strlen(buffer); i++)
        {
            while(buffer[i] != ' ')
            {
            i++; 
            }
  	        while(buffer[i] == ' ')
            {
            i++; 
            }
            row = atoi(&buffer[i]);

            while(buffer[i] != ' ')
            {
            i++; 
            }
  	        while(buffer[i] == ' ')
            {
            i++; 
            }
            col = atoi(&buffer[i]);
            break;
        }
        printf("Right before Shoot check for client2\n");
        printf("Inside Client 2 Shoot Loop\n");
            if(board[row][col] == '-')
            {
                printf("Inside C2 Miss\n");
                board[row][col] = '*';
                sprintf(add_to_query, " M %d %d", row, col);
                strcat(player_2_query, add_to_query);
                int number_of_remaining_ships = remaining_ships_1(store_player_1_init);
                memset(buffer, 0, BUFFER_SIZE);
                sprintf(buffer ,"R %d M", number_of_remaining_ships);
                
                send(conn_fd2, buffer, strlen(buffer), 0);
                memset(buffer, 0, BUFFER_SIZE);
            }
            else if(board[row][col] == '@')
            {
                printf("Inside C2 Hit\n");
                board[row][col] = '*';
                sprintf(add_to_query, " H %d %d", row, col);
                strcat(player_2_query, add_to_query);
                int number_of_remaining_ships = remaining_ships_1(store_player_1_init);
                memset(buffer, 0, BUFFER_SIZE);
                sprintf(buffer ,"R %d H", number_of_remaining_ships);
                
                send(conn_fd2, buffer, strlen(buffer), 0);
                memset(buffer, 0, BUFFER_SIZE);
            }
            
        if(remaining_ships_1(store_player_1_init) == 0)
        {
            memset(buffer, 0, BUFFER_SIZE);
            int nbytes = read(conn_fd, buffer, BUFFER_SIZE);
            memset(buffer, 0, BUFFER_SIZE);
            sprintf(buffer, "H 0");
            send(conn_fd, buffer, strlen(buffer), 0);
            memset(buffer, 0, BUFFER_SIZE);
            nbytes = read(conn_fd2, buffer, BUFFER_SIZE);
            memset(buffer, 0, BUFFER_SIZE);
            sprintf(buffer, "H 1");
            send(conn_fd2, buffer, strlen(buffer), 0);
            memset(buffer, 0, BUFFER_SIZE);
            forfeit_flag = true;
            printf("[Server] Shutting down.\n"); //Change the logic to maintain connection to second client when first client quits.
            close(conn_fd);
            close(listen_fd);
            close(listen_fd2);
            close(conn_fd2);
            return EXIT_SUCCESS;
            break;
        }
        
        for(int i = 0; i < num_rows; i++)
        {
            for(int j = 0; j < num_cols; j++)
            {
                printf("%c ", board2[i][j]);
            }
            printf("\n");
        }
    } 
    //Once you properly implement the shot and Query code for both clients, you must ALSO implement the error functions and 
    //A way to properly check if a ship is actually sunk! Best way atm is by saving Initialize data and using it to know where 
    //the ships are
    
    printf("[Server] Shutting down.\n"); //Change the logic to maintain connection to second client when first client quits.
    close(conn_fd);
    close(listen_fd);
    close(listen_fd2);
    close(conn_fd2);
    return EXIT_SUCCESS;
}
