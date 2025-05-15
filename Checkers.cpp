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

void commit_move(tree* node, motion move){
	// we cannot directly change game state as it is not a pointer (any change in this function would not affect outside this function)
	// so we need a pointer and this is a tree node
    int pawn_class = node->state.board[move.x1][move.y1];
    node->state.board[move.x1][move.y1] = 0;

    if(!node->state.chance){
        int difference = abs(move.y1-move.y2);
        if(difference == 2){
            int avg_x = (move.x1 + move.x2)/2;
            int avg_y = (move.y1 + move.y2)/2;
            node->state.board[avg_x][avg_y] = 0;
            if(move.x2 == 7){
                node->state.board[move.x2][move.y2] = pawn_class;
            }else{
                node->state.board[move.x2][move.y2] = pawn_class;
            }
            if(further_capture(node->state.board, move.x2, move.y2)){
                node->state.forced_capture[0] = move.x2;
                node->state.forced_capture[1] = move.y2;
				node->state.chance = 0;
            }else{
				node->state.forced_capture[0] = -1;
				node->state.forced_capture[1] = -1;				
                node->state.chance = 1;
            }
        } else if(difference == 1){
            if(move.x2 == 7){
                node->state.board[move.x2][move.y2] = pawn_class;
            }else{
                node->state.board[move.x2][move.y2] = pawn_class;
            }
            node->state.chance = 1;
			node->state.forced_capture[0] = -1;
			node->state.forced_capture[1] = -1;		
        }
        else{
            printf("INVALID!");
        }
    }else{
        int difference = abs(move.y1-move.y2);
        if(difference == 2){
            int avg_x = (move.x1 + move.x2)/2;
            int avg_y = (move.y1 + move.y2)/2;
            node->state.board[avg_x][avg_y] = 0;
            if(move.x2 == 0){
                node->state.board[move.x2][move.y2] = pawn_class;
            }else{
                node->state.board[move.x2][move.y2] = pawn_class;
            }
            if(further_capture(node->state.board, move.x2, move.y2)){
                node->state.forced_capture[0] = move.x2;
                node->state.forced_capture[1] = move.y2;
				node->state.chance = 1;
            }else{
				node->state.forced_capture[0] = -1;
				node->state.forced_capture[1] = -1;		
                node->state.chance = 0;
            }
        }
        else if(difference == 1){
            if(move.x2 == 0){
                node->state.board[move.x2][move.y2] = pawn_class;
            }else{
                node->state.board[move.x2][move.y2] = pawn_class;
            }
            node->state.chance = 0;
			node->state.forced_capture[0] = -1;
			node->state.forced_capture[1] = -1;		
        }else{
            printf("INVALID!");
        }
    }
}

/*int pawn_num(tree* node, int pawn_class){
    //keeps a check on the numbers of different pawn classes
    int white_npawn = 0;
    int black_npawn = 0;
    int white_kpawn = 0;
    int black_kpawn = 0;
    for(int i = 0; i<8 ; i++){
        for(int j = 0; j<8; j++){
            if(node->state.board[i][j] == 1){
                white_npawn += 1;
            }
            else if(node->state.board[i][j] == 2){
                black_npawn += 1;
            }
            else if(node->state.board[i][j] == 3){
                white_kpawn += 1;
            }
            else if(node->state.board[i][j] == 4){
                black_kpawn += 1;
            }
        }
    }

    if(pawn_class == 1){
        return white_npawn;
    }
    else if(pawn_class == 2){
        return black_npawn;
    }
    else if(pawn_class == 3){
        return white_kpawn;
    }
    else if(pawn_class == 4){
        return black_kpawn;
    }
}*/

bool in(motion* valid_moves, int size, int x1, int y1, int x2, int y2){
	for (int i=0; i<size; i++){
		if (valid_moves[i].x1 == x1 && valid_moves[i].y1 == y1 && valid_moves[i].x2 == x2 && valid_moves[i].y2 == y2) return 1;
	}
	return 0;
}

bool in_start(motion* valid_moves, int size, int x1, int y1){
	for (int i=0; i<size; i++){
		if (valid_moves[i].x1 == x1 && valid_moves[i].y1 == y1) return 1;
	}
	return 0;
}

int allcaptures(tree* node, motion* capture){
	int count = 0;
	for (int x=0; x<8; x++){
		for (int y=0; y<8; y++){
			int pawn_class = node->state.board[x][y];
			if (node->state.chance){
				// black to move
				if (pawn_class == 2){
					if(node->state.board[x-2][y-2] == 0 && x-2>=0 && y-2>=0 && (node->state.board[x-1][y-1] == 1 || node->state.board[x-1][y-1] == 3)){
						capture[count].x1 = x;
						capture[count].y1 = y;
						capture[count].x2 = x-2;
						capture[count].y2 = y-2;
						count++;
					}
					if(node->state.board[x-2][y+2] == 0 && x-2>=0 && y+2<=7 && (node->state.board[x-1][y+1] == 1 || node->state.board[x-1][y+1] == 3)){
						capture[count].x1 = x;
						capture[count].y1 = y;
						capture[count].x2 = x-2;
						capture[count].y2 = y+2;
						count++;
					}
				} else if (pawn_class == 4){
					if(node->state.board[x+2][y-2] == 0 && x+2<=7 && y-2>=0 && (node->state.board[x+1][y-1] == 1 || node->state.board[x+1][y-1] == 3)){
						capture[count].x1 = x;
						capture[count].y1 = y;
						capture[count].x2 = x+2;
						capture[count].y2 = y-2;
						count++;
					}
					if(node->state.board[x+2][y+2] == 0 && x+2<=7 && y+2<=7 && (node->state.board[x+1][y+1] == 1 || node->state.board[x+1][y+1] == 3)){
						capture[count].x1 = x;
						capture[count].y1 = y;
						capture[count].x2 = x+2;
						capture[count].y2 = y+2;
						count++;
					}
					if(node->state.board[x-2][y-2] == 0 && x-2>=0 && y-2>=0 && (node->state.board[x-1][y-1] == 1 || node->state.board[x-1][y-1] == 3)){
						capture[count].x1 = x;
						capture[count].y1 = y;
						capture[count].x2 = x-2;
						capture[count].y2 = y-2;
						count++;
					}
					if(node->state.board[x-2][y+2] == 0 && x-2>=0 && y+2<=7 && (node->state.board[x-1][y+1] == 1 || node->state.board[x-1][y+1] == 3)){
						capture[count].x1 = x;
						capture[count].y1 = y;
						capture[count].x2 = x-2;
						capture[count].y2 = y+2;
						count++;
					}
				}
			} else {
				// white to move
				if (pawn_class == 1){
					if(node->state.board[x+2][y-2] == 0 && x+2<=7 && y-2>=0 && (node->state.board[x+1][y-1] == 2 || node->state.board[x+1][y-1] == 4)){
						capture[count].x1 = x;
						capture[count].y1 = y;
						capture[count].x2 = x+2;
						capture[count].y2 = y-2;
						count++;
					}
					if(node->state.board[x+2][y+2] == 0 && x+2<=7 && y+2<=7 && (node->state.board[x+1][y+1] == 2 || node->state.board[x+1][y+1] == 4)){
						capture[count].x1 = x;
						capture[count].y1 = y;
						capture[count].x2 = x+2;
						capture[count].y2 = y+2;
						count++;
					}
				} else if (pawn_class == 3){
					if(node->state.board[x+2][y-2] == 0 && x+2<=7 && y-2>=0 && (node->state.board[x+1][y-1] == 2 || node->state.board[x+1][y-1] == 4)){
						capture[count].x1 = x;
						capture[count].y1 = y;
						capture[count].x2 = x+2;
						capture[count].y2 = y-2;
						count++;
					}
					if(node->state.board[x+2][y+2] == 0 && x+2<=7 && y+2<=7 && (node->state.board[x+1][y+1] == 2 || node->state.board[x+1][y+1] == 4)){
						capture[count].x1 = x;
						capture[count].y1 = y;
						capture[count].x2 = x+2;
						capture[count].y2 = y+2;
						count++;
					}
					if(node->state.board[x-2][y-2] == 0 && x-2>=0 && y-2>=0 && (node->state.board[x-1][y-1] == 2 || node->state.board[x-1][y-1] == 4)){
						capture[count].x1 = x;
						capture[count].y1 = y;
						capture[count].x2 = x-2;
						capture[count].y2 = y-2;
						count++;
					}
					if(node->state.board[x-2][y+2] == 0 && x-2>=0 && y+2<=7 && (node->state.board[x-1][y+1] == 2 || node->state.board[x-1][y+1] == 4)){
						capture[count].x1 = x;
						capture[count].y1 = y;
						capture[count].x2 = x-2;
						capture[count].y2 = y+2;
						count++;
					}
				}
			}
		}
	}
	return count;
}

motion *get_valid_moves(tree* node, int x, int y){
    //this function returns the array of valid moves corresponding to a pawn
    //x and y are used to represnt the pawn location in the board

    //1. creating an array 
    
    motion* valid_moves = (motion*)malloc(Max_Move*sizeof(motion));

    for(int i = 0; i<Max_Move; i++){
        //assigning valid moves to -1 so that we can find the end of the array
        valid_moves[i].x1 = x;
        valid_moves[i].y1 = y;
        valid_moves[i].x2 = -1;
        valid_moves[i].y2 = -1;
    }

    //2. checking valid moves
    int count = 0;

	int *arr = node->state.forced_capture;
	motion* capture = (motion*)malloc(Max_Move*sizeof(motion)); // these are also captures but not included in forced capture because they are not further capture
	int capture_size = allcaptures(node, capture);
	
	if (arr[0] != -1 || arr[1] != -1){
		// we have arr[0] and arr[1] as x1 and y1
		if (x == arr[0] && y == arr[1]){
			int pawn_class = node->state.board[x][y];
			if (pawn_class == 1){
				if(node->state.board[x+2][y-2] == 0 && x+2<=7 && y-2>=0 && (node->state.board[x+1][y-1] == 2 || node->state.board[x+1][y-1] == 4)){
					valid_moves[count].x1 = x;
					valid_moves[count].y1 = y;
					valid_moves[count].x2 = x+2;
					valid_moves[count].y2 = y-2;
					count++;
				}
				if(node->state.board[x+2][y+2] == 0 && x+2<=7 && y+2<=7 && (node->state.board[x+1][y+1] == 2 || node->state.board[x+1][y+1] == 4)){
					valid_moves[count].x1 = x;
					valid_moves[count].y1 = y;
					valid_moves[count].x2 = x+2;
					valid_moves[count].y2 = y+2;
					count++;
				}
			} else if (pawn_class == 2){
				if(node->state.board[x-2][y-2] == 0 && x-2>=0 && y-2>=0 && (node->state.board[x-1][y-1] == 1 || node->state.board[x-1][y-1] == 3)){
					valid_moves[count].x1 = x;
					valid_moves[count].y1 = y;
					valid_moves[count].x2 = x-2;
					valid_moves[count].y2 = y-2;
					count++;
				}
				if(node->state.board[x-2][y+2] == 0 && x-2>=0 && y+2<=7 && (node->state.board[x-1][y+1] == 1 || node->state.board[x-1][y+1] == 3)){
					valid_moves[count].x1 = x;
					valid_moves[count].y1 = y;
					valid_moves[count].x2 = x-2;
					valid_moves[count].y2 = y+2;
					count++;
				}
			} else if (pawn_class == 3){
				if(node->state.board[x+2][y-2] == 0 && x+2<=7 && y-2>=0 && (node->state.board[x+1][y-1] == 2 || node->state.board[x+1][y-1] == 4)){
					valid_moves[count].x1 = x;
					valid_moves[count].y1 = y;
					valid_moves[count].x2 = x+2;
					valid_moves[count].y2 = y-2;
					count++;
				}
				if(node->state.board[x+2][y+2] == 0 && x+2<=7 && y+2<=7 && (node->state.board[x+1][y+1] == 2 || node->state.board[x+1][y+1] == 4)){
					valid_moves[count].x1 = x;
					valid_moves[count].y1 = y;
					valid_moves[count].x2 = x+2;
					valid_moves[count].y2 = y+2;
					count++;
				}
				if(node->state.board[x-2][y-2] == 0 && x-2>=0 && y-2>=0 && (node->state.board[x-1][y-1] == 2 || node->state.board[x-1][y-1] == 4)){
					valid_moves[count].x1 = x;
					valid_moves[count].y1 = y;
					valid_moves[count].x2 = x-2;
					valid_moves[count].y2 = y-2;
					count++;
				}
				if(node->state.board[x-2][y+2] == 0 && x-2>=0 && y+2<=7 && (node->state.board[x-1][y+1] == 2 || node->state.board[x-1][y+1] == 4)){
					valid_moves[count].x1 = x;
					valid_moves[count].y1 = y;
					valid_moves[count].x2 = x-2;
					valid_moves[count].y2 = y+2;
					count++;
				}
			} else if (pawn_class == 4){
				if(node->state.board[x+2][y-2] == 0 && x+2<=7 && y-2>=0 && (node->state.board[x+1][y-1] == 1 || node->state.board[x+1][y-1] == 3)){
					valid_moves[count].x1 = x;
					valid_moves[count].y1 = y;
					valid_moves[count].x2 = x+2;
					valid_moves[count].y2 = y-2;
					count++;
				}
				if(node->state.board[x+2][y+2] == 0 && x+2<=7 && y+2<=7 && (node->state.board[x+1][y+1] == 1 || node->state.board[x+1][y+1] == 3)){
					valid_moves[count].x1 = x;
					valid_moves[count].y1 = y;
					valid_moves[count].x2 = x+2;
					valid_moves[count].y2 = y+2;
					count++;
				}
				if(node->state.board[x-2][y-2] == 0 && x-2>=0 && y-2>=0 && (node->state.board[x-1][y-1] == 1 || node->state.board[x-1][y-1] == 3)){
					valid_moves[count].x1 = x;
					valid_moves[count].y1 = y;
					valid_moves[count].x2 = x-2;
					valid_moves[count].y2 = y-2;
					count++;
				}
				if(node->state.board[x-2][y+2] == 0 && x-2>=0 && y+2<=7 && (node->state.board[x-1][y+1] == 1 || node->state.board[x-1][y+1] == 3)){
					valid_moves[count].x1 = x;
					valid_moves[count].y1 = y;
					valid_moves[count].x2 = x-2;
					valid_moves[count].y2 = y+2;
					count++;
				}
			}
		} else {
			// the position passed is not coinciding with forced capture hence return 0 valid_moves
		}
		
	} else if (capture_size == 0 && arr[0] == -1 && arr[1] == -1){
		//a. Checking the neighbouring places of the pawn class to be a vacant place for it to be a valid move
		int pawn_class = node->state.board[x][y];
		if(pawn_class == 1){
			if(node->state.board[x+1][y-1] == 0 && x+1<=7 && y-1>=0){
				valid_moves[count].x2 = x+1;
				valid_moves[count].y2 = y-1;
				count++;
			}
			if(node->state.board[x+1][y+1] == 0 && x+1<=7 && y+1<=7){
				valid_moves[count].x2 = x+1;
				valid_moves[count].y2 = y+1;
				count++;
			}
		}
		else if(pawn_class == 3){
			if(node->state.board[x+1][y-1] == 0 && x+1<=7 && y-1>=0){
				valid_moves[count].x2 = x+1;
				valid_moves[count].y2 = y-1;
				count++;
			}
			if(node->state.board[x+1][y+1] == 0 && x+1<=7 && y+1<=7){
				valid_moves[count].x2 = x+1;
				valid_moves[count].y2 = y+1;
				count++;
			}
			if(node->state.board[x-1][y-1] == 0 && x-1>=0 && y-1>=0){
				valid_moves[count].x2 = x-1;
				valid_moves[count].y2 = y-1;
				count++;
			}
			if(node->state.board[x-1][y+1] == 0 && x-1>=0 && y+1<=7){
				valid_moves[count].x2 = x-1;
				valid_moves[count].y2 = y+1;
				count++;
			}
		}
		else if(pawn_class == 2){
			if(node->state.board[x-1][y-1] == 0 && x-1>=0 && y-1>=0){
				valid_moves[count].x2 = x-1;
				valid_moves[count].y2 = y-1;
				count++;
			}
			if(node->state.board[x-1][y+1] == 0 && x-1>=0 && y+1<=7){
				valid_moves[count].x2 = x-1;
				valid_moves[count].y2 = y+1;
				count++;
			}
		}
		else if(pawn_class == 4){
			if(node->state.board[x-1][y-1] == 0 && x-1>=0 && y-1>=0){
				valid_moves[count].x2 = x-1;
				valid_moves[count].y2 = y-1;
				count++;
			}
			if(node->state.board[x-1][y+1] == 0 && x-1>=0 && y+1<=7){
				valid_moves[count].x2 = x-1;
				valid_moves[count].y2 = y+1;
				count++;
			}
			if(node->state.board[x+1][y-1] == 0 && x+1<=7 && y-1>=0){
				valid_moves[count].x2 = x+1;
				valid_moves[count].y2 = y-1;
				count++;
			}
			if(node->state.board[x+1][y+1] == 0 && x+1<=7 && y+1<=7){
				valid_moves[count].x2 = x+1;
				valid_moves[count].y2 = y+1;
				count++;
			}
		}
		
	} else {
		for (int i=0; i<capture_size; i++){
			if (capture[i].x1 == x && capture[i].y1 == y){
				valid_moves[count].x2 = capture[i].x2;
				valid_moves[count].y2 = capture[i].y2;
				count++;
			}
		}
	}
    
	valid_moves[count].x2 = -1;
	valid_moves[count].y2 = -1;
	free(capture);
    return valid_moves;
}

int count_valid_moves(tree* node, int x, int y){
    motion* valid_moves = get_valid_moves(node, x, y);
    int count = 0;
    for(int i = 0; i < Max_Move; i++){
        if(valid_moves[i].x2 == -1 && valid_moves[i].y2 == -1){
            break;
        }
        count++;
    }
	free(valid_moves);
    return count;
}


int main(){

}