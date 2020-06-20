#include <game.h>
#include <geom.h>

const char EMPTY_SPACE = ' ';

const char *PIECE_I[] = {
  [UP] =    "    IIII        ",
  [RIGHT] = "  I   I   I   I ",
  [DOWN] =  "        IIII    ",
  [LEFT] =  " I   I   I   I  "
};

const char *PIECE_J[] = {
  [UP] =    "J   JJJ         ",
  [RIGHT] = " JJ  J   J      ",
  [DOWN] =  "    JJJ   J     ",
  [LEFT] =  " J   J  JJ      "
};

const char *PIECE_L[] = {
  [UP] =    "  L LLL         ",
  [RIGHT] = " L   L   LL     ",
  [DOWN] =  "    LLL L       ",
  [LEFT] =  "LL   L   L      "
};

const char *PIECE_O[] = {
  [UP] =    "     OO  OO     ",
  [RIGHT] = "     OO  OO     ",
  [DOWN] =  "     OO  OO     ",
  [LEFT] =  "     OO  OO     "
};

const char *PIECE_S[] = {
  [UP] =    " SS SS          ",
  [RIGHT] = " S   SS   S     ",
  [DOWN] =  "     SS SS      ",
  [LEFT] =  "S   SS   S      ",
};

const char *PIECE_T[] = {
  [UP] =    " T  TTT         ",
  [RIGHT] = "T   TT  T       ",
  [DOWN] =  "    TTT  T      ",
  [LEFT] =  " T  TT   T      "
};

const char *PIECE_Z[] = {
  [UP] =    "ZZ   ZZ         ",
  [RIGHT] = "  Z  ZZ  Z      ",
  [DOWN] =  "    ZZ   ZZ     ",
  [LEFT] =  " Z  ZZ  Z       "
};

void makeCanvas(shape_t &canvas) {
  canvas.rows = H_BOARD;
  canvas.cols = W_BOARD;
  // Conventionally, our bbox for an "empty" shape is 0.
  canvas.bbox = (struct bbox) {
    (struct point) { 0, 0 },
    (struct point) { 0, 0 }
  };
  canvas.grid = new char[H_BOARD * W_BOARD]();

  // Fill each cell with the default, empty value.
  for (uint8_t i = 0; i < H_BOARD * W_BOARD; ++i) {
    canvas.grid[i] = EMPTY_SPACE;
  }
}

Game::Game(const shape_t &canvas, const Sequence &sequence)
  : canvas(canvas),
    sequence(sequence) {

  // Generate all shapes, which are constant and immutable.
  generateFromShapes(PIECE_I, 4, 4, pieces[I]);
  generateFromShapes(PIECE_J, 4, 4, pieces[J]);
  generateFromShapes(PIECE_L, 4, 4, pieces[L]);
  generateFromShapes(PIECE_O, 4, 4, pieces[O]);
  generateFromShapes(PIECE_S, 4, 4, pieces[S]);
  generateFromShapes(PIECE_T, 4, 4, pieces[T]);
  generateFromShapes(PIECE_Z, 4, 4, pieces[Z]);

  // Create a new board. This will be re-used for all games.
  makeCanvas(board);

  reset();
}

uint32_t Game::getScore() {
  return score;
}

void Game::render() {
  // Write our data to the canvas.
  for (uint8_t i = 0; i < H_BOARD * W_BOARD; ++i) {
    canvas.grid[i] = board.grid[i];
  }
  updateBoundingBox(canvas);
}

void Game::reset() {
  score = 0;

  fillShape(board, EMPTY_SPACE);

  render();
}

