#include <game.h>
#include <geom.h>

const char EMPTY_SPACE = ' ';
const point_t START_COORDINATES = { 1, 3 };

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

void makeCanvas(Shape &canvas) {
  canvas.setWithChar(EMPTY_SPACE, H_BOARD, W_BOARD);
}

Game::Game(
    const Shape &canvas,
    const Sequence &sequence,
    void (*gameOverCallback)()
) : canvas(canvas),
    sequence(sequence) {

  this->gameOverCallback = gameOverCallback;

  // Generate all shapes, which are constant and immutable.
  pieces[I].generateFromShapes(PIECE_I, 4, 4);
  pieces[J].generateFromShapes(PIECE_J, 4, 4);
  pieces[L].generateFromShapes(PIECE_L, 4, 4);
  pieces[O].generateFromShapes(PIECE_O, 4, 4);
  pieces[S].generateFromShapes(PIECE_S, 4, 4);
  pieces[T].generateFromShapes(PIECE_T, 4, 4);
  pieces[Z].generateFromShapes(PIECE_Z, 4, 4);

  // Create a new board. This will be re-used for all games.
  makeCanvas(board);

  reset();
}

void Game::produceNextPiece() {
  currentPiece = &pieces[sequence.next()];

  if (board.collides((*currentPiece).getCurrentShape(), START_COORDINATES)) {
    gameOver();
  }

  currentPiece->setCoordinates(START_COORDINATES);
}

void Game::render() {
  canvas.setFromChars(board.getGrid(), board.getRows(), board.getCols());
  canvas.updateBoundingBox();
}

void Game::reset() {
  score = 0;

  board.fillWith(EMPTY_SPACE);
  canvas.fillWith(EMPTY_SPACE);

  render();
}

void Game::gameOver() {
  gameOverCallback();
}
