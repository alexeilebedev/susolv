// step 1: input board as a string of digits,
// left-to-right and top to bottom. with . meaning
// unknown
#include <stdio.h>
#include <memory.h>
#include <stdlib.h>

int _starts[][9] = {
  {0,8,17,26,35,44,53,62,71}
  ,{0,1,2,3,4,5,6,7,8}
  ,{0,3,6,26,29,32,53,56,59}
};
int _offsets[][9] = {
  {0,1,2,3,4,5,6,7,8}
  ,{0,8,17,26,35,44,53,62,71}
  ,{0,1,2,9,10,11,18,19,20}
};
struct Game {
  enum {MAX=81};
  // first character is the top-left corner
  char board[MAX];// 0..9, 0 = unknown
  int n;
  Game() {
    n=0;
    memset(board,0,sizeof(board));
  }
};

void AddCell(Game &game, int value) {
  if (game.n<game.MAX) {
    game.board[game.n++] = value;
  }
}

int CharToValue(char c) {
  int ret=0;
  if (c>='1' && c<='9') {
    ret = c-'0';
  }
  return ret;
}

// Find index of empty cell, -1 = not found
int FindEmptyCell(Game &game) {
  int ret=-1;
  for (int i=0; i<game.MAX; i++) {
    if (game.board[i]==0) {
      ret=i;
      break;
    }
  }
  return ret;
}
  
// Check if 9 values found at BASE + a1, BASE + a2 ... BASE +a9
// satisfy the sudoku constraint.
// The constraint is simply that no value appears twice
bool ConsistentBlockQ(Game &game, int base, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9) {
  // histogram of values 0..9 inclusive (0=undefined)
  char present[]={0,0,0,0,0,0,0,0,0,0};
  char idx[]={base+a1,base+a2,base+a3,base+a4,base+a5,base+a6,base+a7,base+a8,base+a9};
  for (int i=0; i<9; i++) {
    present[game.board[idx[i]]]++;
  }
  bool ret=true;
  for (int i=1; i<=9; i++) {
    if (present[i]>=2) {
      ret=false;
      break;
    }
  }
  if (!ret) {
    // for (int i=0; i<9; i++) {
    //   printf("%d=%d,",idx[i],game.board[idx[i]]);
    // }
    // printf("\n");
    //printf("%d %d %d %d %d %d %d %d %d\n",base+a1,base+a2,base+a3,base+a4,base+a5,base+a6,base+a7,base+a8,base+aa9);
  }
  return ret;
}

bool ConsistentQ(Game &game) {
  // check that every row contains numbers 0..9
  for (int y=0; y<9; y++) {
    if (!ConsistentBlockQ(game, y*9,0,1,2,3,4,5,6,7,8)) {
      return false;
    }
  }
  for (int x=0; x<9; x++) {
    if (!ConsistentBlockQ(game, x,0,9,18,27,36,45,54,63,72)) {
      return false;
    }
  }
  // check every square
  for (int y=0; y<9; y+=3) {
    for (int x=0; x<9; x+=3) {
      if (!ConsistentBlockQ(game, y*9 + x, 0,1,2,9,10,11,18,19,20)) {
	return false;
      }
    }
  }
  return true;
}

void Show(Game &game) {
  for (int i=0; i<81; i++) {
    printf("%d",int(game.board[i]));
    if ((i+1)%3==0) {
      printf(" ");
    }
    if ((i+1)%9==0) {
      printf("\n");
    }
    if ((i+1)%27==0) {
      printf("\n");
    }
  }
}

// to solve, take the first cell that's -1, and try values 0..9 for it
// if any of these results in a valid board, return true and update GAME
// to the winning variant
bool Solve(Game &game) {
  int cell=FindEmptyCell(game);
  bool ret=false;
  if (cell >=0 && cell < game.MAX)  {
    for (int value =1; value<=9; value++) {
      Game temp;
      temp=game;
      static int level=0;
      // for (int i=0; i<level; i++) {
      // 	printf(" ");
      // }
      //printf("%d = %d\n",cell,value);
      temp.board[cell] = value;
      if (ConsistentQ(temp)) {
	//Show(temp);
	level++;
	ret = Solve(temp);
	level--;
	if (ret) {
	  game = temp;
	  break;
	}
      }
    }
  } else {
    ret = ConsistentQ(game);
    if (ret) {
      printf("solution found\n");
      Show(game);
    }
  }
  return ret;
}

int main(int argc, char **argv) {
  if (argc<2) {
    printf("usage: susolv <board>\n");
    exit(1);
  }
  Game game;
  for (int arg=1; arg<argc; arg++) {
    int len=strlen(argv[arg]);
    for (int i=0; i<len; i++) {
      AddCell(game,CharToValue(argv[arg][i]));
    }
  }
  Show(game);
  if (!ConsistentQ(game)) {
    printf("the board is not consistent\n");
  }
  Solve(game);
  return 0;
}
