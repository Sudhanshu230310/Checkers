#include<iostream>
#include<math.h>
#include<limits.h>
#include <memory.h>
using namespace std;

int infi = 5000; //greatest integer
int Max_Move = 30; //defining the maximum no of valid moves for any pawn
int alphabeta_counter = 0;
int minimax_counter = 0;
int depth_search = 9;

struct game{
    int board[8][8];
    //-1 for non movable place
    //0 for vacant place
    //1 for white
    //2 for black
    //3 for white king
    //4 for black king
    bool chance;
    //0 for white 
    //1 for black
    int forced_capture[2];
};

struct motion{
    int x1, x2, y1, y2;
    //x1 y1 initial coordinates
    //x2 y2 final coordinates
};

struct tree{
    game state;
    motion move;
    int eval;
    int n;
	int depthover;
    tree** child;
};

struct Move{
    int x_pos;
    int y_pos;
    bool capture;
    //flag to indicate if the move is a capture move or not
    //0 for normal move
    //1 for capture move
};


void print_board(tree* node)
{
    for(int i = 0; i<8; i++){
        for(int j = 0; j<8; j++){
            if(i % 2 == j% 2){
                printf("* ");
            }
            if (node->state.board[i][j] == 0){
                printf("- ");
            }else if (node->state.board[i][j] == 1){
                printf("W ");
            }else if (node->state.board[i][j] == 2){
                printf("B ");
            }else if (node->state.board[i][j] == 3){
                printf("KW");
            }else if (node->state.board[i][j] == 4){
                printf("KB");
            }
        }
        printf("\n");
    }
}

void free_node(tree* node){
	if (node == NULL) return;
	if (node->n == 0){
		free(node);
		return;
	} else {
		for (int i=node->n-1; i>=0; i--){
			free_node(node->child[i]); // depth first freeing
		}
		free(node); // free the current node
		return;
	}
}

bool further_capture(int board[8][8], int x, int y){
    int pawn_class = board[x][y];
    if(pawn_class == 1){
        if(x+2 <= 7 && y-2 >= 0){
            if(board[x+1][y-1] == 2 || board[x+1][y-1] == 4){
                if(board[x+2][y-2] == 0){
                    return 1;
                }
            }
        }
        if(x+2 <= 7 && y+2 <= 7){
            if(board[x+1][y+1] == 2 || board[x+1][y+1] == 4){
                if(board[x+2][y+2] == 0){
                    return 1;
                }
            }
        }
    } else if (pawn_class == 3){
		if(x-2 >= 0 && y-2 >= 0){
            if(board[x-1][y-1] == 2 || board[x-1][y-1] == 4){
                if(board[x-2][y-2] == 0){
                    return 1;
                }
            }
        }
        if(x-2 >= 0 && y+2 <= 7){
            if(board[x-1][y+1] == 2 || board[x-1][y+1] == 4){
                if(board[x-2][y+2] == 0){
                    return 1;
                }
            }
        }
        if(x+2 <= 7 && y-2 >= 0){
            if(board[x+1][y-1] == 2 || board[x+1][y-1] == 4){
                if(board[x+2][y-2] == 0){
                    return 1;
                }
            }
        }
        if(x+2 <= 7 && y+2 <= 7){
            if(board[x+1][y+1] == 2 || board[x+1][y+1] == 4){
                if(board[x+2][y+2] == 0){
                    return 1;
                }
            }
        }
	} else if(pawn_class == 4){
        if(x-2 >= 0 && y-2 >= 0){
            if(board[x-1][y-1] == 1 || board[x-1][y-1] == 3){
                if(board[x-2][y-2] == 0){
                    return 1;
                }
            }
        }
        if(x-2 >= 0 && y+2 <= 7){
            if(board[x-1][y+1] == 1 || board[x-1][y+1] == 3){
                if(board[x-2][y+2] == 0){
                    return 1;
                }
            }
        }
        if(x+2 <= 7 && y-2 >= 0){
            if(board[x+1][y-1] == 1 || board[x+1][y-1] == 3){
                if(board[x+2][y-2] == 0){
                    return 1;
                }
            }
        }
        if(x+2 <= 7 && y+2 <= 7){
            if(board[x+1][y+1] == 1 || board[x+1][y+1] == 3){
                if(board[x+2][y+2] == 0){
                    return 1;
                }
            }
        }
    } else if (pawn_class == 2){
		if(x-2 >= 0 && y-2 >= 0){
            if(board[x-1][y-1] == 1 || board[x-1][y-1] == 3){
                if(board[x-2][y-2] == 0){
                    return 1;
                }
            }
        }
        if(x-2 >= 0 && y+2 <= 7){
            if(board[x-1][y+1] == 1 || board[x-1][y+1] == 3){
                if(board[x-2][y+2] == 0){
                    return 1;
                }
            }
        }
	}

    return 0;
}


int main(){

}