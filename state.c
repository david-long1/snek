#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "snake_utils.h"
#include "state.h"

/* Helper function definitions */
static char get_board_at(game_state_t* state, int x, int y);
static void set_board_at(game_state_t* state, int x, int y, char ch);
static bool is_tail(char c);
static bool is_snake(char c);
static char body_to_tail(char c);
static int incr_x(char c);
static int incr_y(char c);
static void find_head(game_state_t* state, int snum);
static char next_square(game_state_t* state, int snum);
static void update_tail(game_state_t* state, int snum);
static void update_head(game_state_t* state, int snum);

/* Helper function to get a character from the board (already implemented for you). */
static char get_board_at(game_state_t* state, int x, int y) {
  return state->board[y][x];
}

/* Helper function to set a character on the board (already implemented for you). */
static void set_board_at(game_state_t* state, int x, int y, char ch) {
  state->board[y][x] = ch;
}

/* Task 1 */
game_state_t* create_default_state() {
  // TODO: Implement this function.

  snake_t* default_snake;
  default_snake = (snake_t*)malloc(sizeof(snake_t)+1);
  default_snake->tail_x = 4;
  default_snake->tail_y = 4;
  default_snake->head_y = 4;
  default_snake->head_x = 5;
  default_snake->live = true;

  game_state_t* default_state;
  default_state = (game_state_t*)malloc(sizeof(game_state_t)+1);
  default_state->x_size = 14;
  default_state->y_size = 10;
  default_state->num_snakes = 1;
  // snake_t snack_arr[] = {*default_snake};
  //  default_state->snakes = (snake_t*)malloc(sizeof(snake_t)+1);
   default_state->snakes = default_snake;
  //  default_state->snakes[1] = default_snake_2;


  // char *first = (char*)calloc(20, sizeof(char));
  // first = "##############";
  // char* second = (char*)calloc(20, sizeof(char));
  // second = "#            #";
  // char list[][] = {first, second};

  char** board_arr = (char**)malloc(sizeof(char*)*15+1);
  // board_arr = {   "##############",
  //                    "#            #",
  //                    "#        *   #",
  //                    "#            #",
  //                    "#   d>       #",
  //                    "#            #",
  //                    "#            #",
  //                    "#            #",
  //                    "##############"
  //                };

  int row = 14;
  int col = 10;
  int i, j;
  for (i = 0; i < row; i++)
        board_arr[i] = (char*)malloc(sizeof(char)*14+1);

  for (i = 0; i < col; i++)
        for (j = 0; j < row; j++)
          if (j == 0 || i == 0 || j == 13 || i == 9) {
            board_arr[i][j] = '#';
          }
          else if (j == 9 && i == 2) {
            board_arr[i][j] = '*';
          }
          else if (j == default_snake->tail_x && i == default_snake->tail_y) {
            board_arr[i][j] = 'd';
          } 
          else if (j == default_snake->head_x && i == default_snake->head_y) {
            board_arr[i][j] = '>';
          }
          else {
            board_arr[i][j] = ' ';
          }

  default_state->board = board_arr;
  return default_state;
}

/* Task 2 */
void free_state(game_state_t* state) {
  free(state->snakes);
  int i;
  for (i = 0; i < state->y_size; i++)
        free(state->board[i]);
  free(state->board);
  free(state);
  return;
}

/* Task 3 */
void print_board(game_state_t* state, FILE* fp) {
  // TODO: Implement this function.
  char** board_arr = state->board;
  int i;
  int y;
  y = state->y_size;
  for (i = 0; i < y; i++)
        fprintf(fp,"%s\n", board_arr[i]);

  return;
}

/* Saves the current state into filename (already implemented for you). */
void save_board(game_state_t* state, char* filename) {
  FILE* f = fopen(filename, "w");
  print_board(state, f);
  fclose(f);
}

/* Task 4.1 */
static bool is_tail(char c) {
  // TODO: Implement this function.
    if (c == 'w' || c == 's' || c == 'a' || c== 'd') {
      return true;
    }
  return false;
}

static bool is_snake(char c) {
  // TODO: Implement this function.
    if (c == 'w' || c == 's' || c == 'a' || c== 'd' || c=='^' || c=='<' || c=='>' || c=='v' ) {
      return true;
    }
  return false;
}

static bool is_snake_body(char c) {
  // TODO: Implement this function.
    if (c=='^' || c=='<' || c=='>' || c=='v' ) {
      return true;
    }
  return false;
}

static char body_to_tail(char c) {
  // TODO: Implement this function.
  //  converts w to ^, a to <, s to v, and d to >.
  if (c == '^'){
    return 'w';
  } else if ( c == '<') {
    return 'a';
  } else if ( c == 'v') {
    return 's';
  } else if (c == '>') {
    return 'd';
  }
  return '?';
}

static int incr_x(char c) {
  // TODO: Implement this function.
  // Returns 1 if c is > or d. Returns -1 if c is < or a. Returns 0 otherwise.
  if (c == '>'|| c == 'd') {
      return 1;
    } else if (c == '<'|| c == 'a') {
      return -1;
    } 
  return 0;
}

static int incr_y(char c) {
  // TODO: Implement this function.
  // Returns 1 if c is v or s. Returns -1 if c is ^ or w. Returns 0 otherwise.

  if (c == 'v'|| c == 's') {
      return 1;
    } else if (c == '^'|| c == 'w') {
      return -1;
    } 
  return 0;
}

/* Task 4.2 */
static char next_square(game_state_t* state, int snum) {
  // TODO: Implement this function.
  int counter = -1;
  
  int tail_loc_row;
  int tail_loc_col;
  int i;
  int j;
// ##############
// #            #
// #        *   #
// #     #      #
// #   d>^      #
// #            #
// #            #
// #            #
// #            #
// ##############

// y.size is 10, x.size is 14, first should be the row num, second should be the column num, row has 10, col has 14
  for (i = 0; i < state->y_size; i++) {
    if (counter == snum) {
                break;
            }
    for (j = 0; j < state->x_size; j++){
        if (is_tail(state->board[i][j])) {
              counter++;
              // printf("%d", counter);
              if (counter == snum) {
                tail_loc_row = i;
                tail_loc_col = j;
                break;
            }
            } 
    }
  }
  
  // printf("%c\n", state->board[4][5]);

  // printf("%d\n", tail_loc_row);
  // printf("%d\n", tail_loc_col);

  
   int origin_row = tail_loc_row; 
   int origin_col = tail_loc_col;
   while (is_snake(state->board[tail_loc_row][tail_loc_col])) {
    //  if (g < 10) {
    //   printf("%d", tail_loc_row);
    //   printf("%d", tail_loc_col);
    //   printf("%d", incr_x(state->board[tail_loc_row][tail_loc_col]));
    //   printf("%d\n", incr_y(state->board[tail_loc_row][tail_loc_col]));
    //   g++;
    //  }
     int x = incr_x(state->board[tail_loc_row][tail_loc_col]);
     int y = incr_y(state->board[tail_loc_row][tail_loc_col]);
     tail_loc_col += x;
     tail_loc_row += y;

     if (origin_row == tail_loc_row && origin_col == tail_loc_col) {
       return state->board[tail_loc_row][tail_loc_col];
     }
     
     
   }

   // printf("%c\n", state->board[tail_loc_row][tail_loc_col]);

  // printf("%c\n", state->board[tail_loc_row][tail_loc_col]);
  return state->board[tail_loc_row][tail_loc_col];
}

/* Task 4.3 */
static void update_head(game_state_t* state, int snum) {
  int counter = -1;
  
  int tail_loc_row;
  int tail_loc_col;
  int i;
  int j;
  for (i = 0; i < state->y_size; i++) {
    if (counter == snum) {
                break;
            }
    for (j = 0; j < state->x_size; j++){
        if (is_tail(state->board[i][j])) {
              counter++;
              // printf("%d", counter);
              if (counter == snum) {
                tail_loc_row = i;
                tail_loc_col = j;
                break;
            }
            } 
    }
  }

  
  int prev_x = state->snakes->head_x;
  int prev_y = state->snakes->head_y;
  char head_char = state->board[prev_y][prev_x];
  // printf("%c\n", head_char);
  while (is_snake(state->board[tail_loc_row][tail_loc_col])) {
    tail_loc_col += incr_x(state->board[tail_loc_row][tail_loc_col]);
    tail_loc_row += incr_y(state->board[tail_loc_row][tail_loc_col]);
  }
  state->snakes->head_x = tail_loc_col;
  state->snakes->head_y = tail_loc_row;
  // printf("%d\n", tail_loc_row);
  // printf("%d\n", tail_loc_col);
  state->board[tail_loc_row][tail_loc_col] = head_char;
  return;
}

/* Task 4.4 */
static void update_tail(game_state_t* state, int snum) {
  int counter = -1;
  //printf("%c\n", state->board[5][5]);

  int tail_loc_row;
  int tail_loc_col;
  int i;
  int j;
  for (i = 0; i < state->y_size; i++) {
    if (counter == snum) {
                break;
            }
    for (j = 0; j < state->x_size; j++){
        if (is_tail(state->board[i][j])) {
              counter++;
              // printf("%d", counter);
              if (counter == snum) {
                tail_loc_row = i;
                tail_loc_col = j;
                break;
            }
            } 
    }
  }
  // printf("%d\n", tail_loc_row);
  // printf("%d\n", tail_loc_col);

  // printf("%c\n", state->board[5][5]);

  // char tail_char = state->board[tail_loc_row][tail_loc_col];
  int new_tail_loc_row = tail_loc_row + incr_y(state->board[tail_loc_row][tail_loc_col]);
  int new_tail_loc_col = tail_loc_col + incr_x(state->board[tail_loc_row][tail_loc_col]);
  state->board[tail_loc_row][tail_loc_col] = ' ';
  // printf("%d\n", new_tail_loc_row);
  // printf("%d\n", new_tail_loc_col);
  state->board[new_tail_loc_row][new_tail_loc_col] = body_to_tail(state->board[new_tail_loc_row][new_tail_loc_col]);

  state->snakes->tail_x = new_tail_loc_col;
  state->snakes->tail_y = new_tail_loc_row;


  return;
}

/* Task 4.5 */
void update_state(game_state_t* state, int (*add_food)(game_state_t* state)) {
  char next_s = next_square(state, 0);

  // printf("%c\n", next_s);
  if (next_s == '#' || is_snake(next_s)) {
    int x_pos = state->snakes->head_x;
    int y_pos = state->snakes->head_y;
 
    state->board[y_pos][x_pos] = 'x';
    state->snakes->live = false;

  } else if (next_s == '*') {
    update_head(state, 0);
    add_food(state);
  } else if (next_s == ' ') {
    update_head(state, 0);
    update_tail(state,0);
  }

  return;
}

/* Task 5 */
game_state_t* load_board(char* filename) {
  game_state_t* state_ptr = (game_state_t*)malloc(sizeof(game_state_t)+1);
  FILE *fp = fopen(filename,"r");

  char c = fgetc(fp);;
  int row = 0;
  while (c!='\n') {
    c = fgetc(fp);
    row++;
  }
  rewind(fp);
  int col = 0;
  while (c!= EOF) {
    c = fgetc(fp);
    if (c == '\n') {
      col++;
    }
  }
  state_ptr->x_size = row;
  state_ptr->y_size = col;
  state_ptr->board = (char**)malloc(sizeof(char*)*(col));
  for (int i = 0; i < col; i++) {
    state_ptr->board[i] = (char*)malloc(sizeof(char)*(row));
  }
  rewind(fp);
  for (int c = 0; c < col; c++) {
    for (int r = 0; r < row+1; r++) {
      char l = fgetc(fp);
      if (l == '\0') {
        printf("%c", 'n');
      }
      if (l == '\n') {
        state_ptr->board[c][r] = '\0';
      } else {
        state_ptr->board[c][r] = l;
      }
    }

  }

  return state_ptr;

  
}

/* Task 6.1 */
static void find_head(game_state_t* state, int snum) {
  // TODO: Implement this function.
  int counter = -1;
  
  int tail_loc_row;
  int tail_loc_col;
  int i;
  int j;

  for (i = 0; i < state->y_size; i++) {
    if (counter == snum) {
                break;
            }
    for (j = 0; j < state->x_size; j++){
        if (is_tail(state->board[i][j])) {
              counter++;
              // printf("%d", counter);
              if (counter == snum) {
                tail_loc_row = i;
                tail_loc_col = j;
                break;
            }
            } 
    }
  }
  
  
   while (is_snake(state->board[tail_loc_row][tail_loc_col])) {
 
     int x = incr_x(state->board[tail_loc_row][tail_loc_col]);
     int y = incr_y(state->board[tail_loc_row][tail_loc_col]);
     if (!is_snake(state->board[tail_loc_row + y][tail_loc_col + x])) {
      //printf("%d", tail_loc_row);
      //printf("%d", tail_loc_col);
       break;
     }
     tail_loc_col += x;
     tail_loc_row += y;
     
   }
  state->snakes->head_x = tail_loc_col;
  state->snakes->head_y = tail_loc_row;
}

/* Task 6.2 */
game_state_t* initialize_snakes(game_state_t* state) {
  // TODO: Implement this function.
  int total_snake_num = 0;
  int y = state->y_size;
  int x = state->x_size;
  for (int r = 0; r < y; r++) {
    for (int c = 0; c < x; c++) {
      if (is_tail(state->board[r][c])) {
        total_snake_num++;
      }
    }
  }
  state->num_snakes = total_snake_num;
  state->snakes = (snake_t*)malloc(sizeof(snake_t)*(total_snake_num));
  state->snakes->live = true;
  int i = 0;
  for (int r = 0; r < state->y_size; r++) {
    for (int c = 0; c < state->x_size; c++) {
      if (is_tail(state->board[r][c])) {
        state->snakes[i].tail_x = c;
        state->snakes[i].tail_y = r;
        find_head(state,i);
        i++;
      }
    }
  }
  return state;
}
