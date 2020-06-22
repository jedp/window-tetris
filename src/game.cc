#include <assert.h>
#include <game.h>
#include <shape.h>
#include <assert.h>

// Reference for pieces: https://tetris.fandom.com/wiki/Orientation

Game::Game(
    const Shape &canvas,
    const Sequence &sequence,
    void (*gameOverCallback)(void))
 : canvas(canvas),
   sequence(sequence),
   gameOverCallback(gameOverCallback),
   stats((struct stats) { 0 }),
   state(PLAYING),
   currentPieceName(I),
   currentOrientation(UP),
   board(Shape(20, 10, "                                                                                                                                                                                                        ")),
   pieces{
     [I] = Piece(4, 4,
       Shape(4, 4, "    IIII        "),
       Shape(4, 4, "  I   I   I   I "),
       Shape(4, 4, "        IIII    "),
       Shape(4, 4, " I   I   I   I  ")
     ),

     [J] = Piece(4, 4,
       Shape(4, 4, "J   JJJ         "),
       Shape(4, 4, " JJ  J   J      "),
       Shape(4, 4, "    JJJ   J     "),
       Shape(4, 4, " J   J  JJ      ")
     ),

     [L] = Piece(4, 4,
       Shape(4, 4, "  L LLL         "),
       Shape(4, 4, " L   L   LL     "),
       Shape(4, 4, "    LLL L       "),
       Shape(4, 4, "LL   L   L      ")
     ),

     [O] = Piece(4, 4,
       Shape(4, 4, "     OO  OO     "),
       Shape(4, 4, "     OO  OO     "),
       Shape(4, 4, "     OO  OO     "),
       Shape(4, 4, "     OO  OO     ")
     ),

     [S] = Piece(4, 4,
       Shape(4, 4, " SS SS          "),
       Shape(4, 4, " S   SS   S     "),
       Shape(4, 4, "     SS SS      "),
       Shape(4, 4, "S   SS   S      ")
     ),

     [T] = Piece(4, 4,
       Shape(4, 4, " T  TTT         "),
       Shape(4, 4, "T   TT  T       "),
       Shape(4, 4, "    TTT  T      "),
       Shape(4, 4, " T  TT   T      ")
     ),

     [Z] = Piece(4, 4,
       Shape(4, 4, "ZZ   ZZ         "),
       Shape(4, 4, "  Z  ZZ  Z      "),
       Shape(4, 4, "    ZZ   ZZ     "),
       Shape(4, 4, " Z  ZZ  Z       ")
       )
   }
{


  reset();

  produceNextPiece();
}

bool Game::setGrid(const char *grid) {
  board.fillWithChars(grid);
}

bool Game::moveLeft() {
  return move(LEFT);
}

bool Game::moveRight() {
  return move(RIGHT);
}

bool Game::move(orientation_t dir) {
  if (state != PLAYING) return false;

  assert(dir == LEFT || dir == RIGHT || dir == DOWN);
  point_t dst;
  switch (dir) {
    case RIGHT:
      dst.row = pieces[currentPieceName].getCoordinates().row;
      dst.col = pieces[currentPieceName].getCoordinates().col + 1;
      break;
    case LEFT:
      dst.row = pieces[currentPieceName].getCoordinates().row;
      dst.col = pieces[currentPieceName].getCoordinates().col - 1;
      break;
    case DOWN:
      dst.row = pieces[currentPieceName].getCoordinates().row + 1;
      dst.col = pieces[currentPieceName].getCoordinates().col;
      break;
    default:
      assert(false);
  }

  if (!pieces[currentPieceName].getCurrentShape().within(BOARD_AREA, dst)) {
    return false;
  }

  if (board.collides(pieces[currentPieceName].getCurrentShape(), dst)) {
    return false;
  }

  pieces[currentPieceName].setCoordinates(dst);

  render();

  return true;
}

bool Game::rotateClockwise() {
  if (state != PLAYING) return false;

  if (!pieces[currentPieceName]
        .shapeForClockwiseRotation()
        .within(BOARD_AREA,
                pieces[currentPieceName].getCoordinates())) {
    return false;
  }
  if (board.collides(pieces[currentPieceName].shapeForClockwiseRotation(),
                     pieces[currentPieceName].getCoordinates())) {
    return false;
  }

  pieces[currentPieceName].rotateClockwise();

  render();

  return true;
}

void Game::drop() {
  if (state != PLAYING) return;

  int startRow = pieces[currentPieceName].getCoordinates().row;
  while(move(DOWN)) ;
  scoreDroppedRows(pieces[currentPieceName].getCoordinates().row - startRow);

  stickCurrentPiece();
}

void Game::tick() {
  switch(state) {

    // When PLAYING, move the piece down one row.
    //
    // If it can't go any further, stick it where it is and see if any rows
    // are now full.
    case PLAYING:
      if (!move(DOWN)) {
        stickCurrentPiece();
      }
      break;

    // When SHOWING_FULL_ROWS, remove the full rows and advance state.
    case SHOWING_FULL_ROWS:
      scoreRemovedRows(board.removeRowsMarkedForRemoval());
      state = DELETING_FULL_ROWS;
      break;

    // When DELETING_FULL_ROWS, produce the next piece and keep playing.
    case DELETING_FULL_ROWS:
      produceNextPiece();
      state = PLAYING;
      break;

    default:
      assert(false);
  }
}

void Game::stickCurrentPiece() {
  board.stick(pieces[currentPieceName].getCurrentShape(),
             pieces[currentPieceName].getCoordinates());
  hideCurrentPiece();

  if (board.findAndMarkRowsForRemoval()) {
    render();
    state = SHOWING_FULL_ROWS;
  } else {
    produceNextPiece();
  }
}

void Game::hideCurrentPiece() {
  pieces[currentPieceName].setCoordinates(OFFSCREEN_COORDINATES);
}

void Game::produceNextPiece() {
  currentPieceName = sequence.next();

  pieces[currentPieceName].setOrientation(UP);

  if (board.collides(pieces[currentPieceName].getCurrentShape(),
                     START_COORDINATES)) {
    gameOver();
  }

  pieces[currentPieceName].setCoordinates(START_COORDINATES);

  render();
}

void Game::scoreRemovedRows(int rows) {
  if (rows == 1) stats.score += 30;
  if (rows == 2) stats.score += 100;
  if (rows == 3) stats.score += 300;
  if (rows >= 4) stats.score += 1200;
}

void Game::scoreDroppedRows(int rows) {
  stats.score += rows;
}

void Game::render() {
  canvas.fillWith(EMPTY_SPACE);
  canvas.stick(board, (struct point) { 0, 0});
  canvas.stick(pieces[currentPieceName].getCurrentShape(), pieces[currentPieceName].getCoordinates());
  canvas.updateBoundingBox();
}

void Game::reset() {
  stats = (struct stats) { 0 };

  board.fillWith(EMPTY_SPACE);
  canvas.fillWith(EMPTY_SPACE);
}

void Game::gameOver() {
  gameOverCallback();
}

