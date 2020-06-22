#include <assert.h>
#include <game.h>
#include <shape.h>
#include <assert.h>

const char *initialBoardGrid = new char[H_BOARD * W_BOARD] { ' ' };

// Reference for pieces: https://tetris.fandom.com/wiki/Orientation
const Shape I_UP    = Shape(4, 4, "    IIII        ");
const Shape I_RIGHT = Shape(4, 4, "  I   I   I   I ");
const Shape I_DOWN  = Shape(4, 4, "        IIII    ");
const Shape I_LEFT  = Shape(4, 4, " I   I   I   I  ");

const Shape J_UP    = Shape(4, 4, "J   JJJ         ");
const Shape J_RIGHT = Shape(4, 4, " JJ  J   J      ");
const Shape J_DOWN  = Shape(4, 4, "    JJJ   J     ");
const Shape J_LEFT  = Shape(4, 4, " J   J  JJ      ");

const Shape L_UP    = Shape(4, 4, "  L LLL         ");
const Shape L_RIGHT = Shape(4, 4, " L   L   LL     ");
const Shape L_DOWN  = Shape(4, 4, "    LLL L       ");
const Shape L_LEFT  = Shape(4, 4, "LL   L   L      ");

const Shape O_UP    = Shape(4, 4, "     OO  OO     ");
const Shape O_RIGHT = Shape(4, 4, "     OO  OO     ");
const Shape O_DOWN  = Shape(4, 4, "     OO  OO     ");
const Shape O_LEFT  = Shape(4, 4, "     OO  OO     ");

const Shape S_UP    = Shape(4, 4, " SS SS          ");
const Shape S_RIGHT = Shape(4, 4, " S   SS   S     ");
const Shape S_DOWN  = Shape(4, 4, "     SS SS      ");
const Shape S_LEFT  = Shape(4, 4, "S   SS   S      ");

const Shape T_UP    = Shape(4, 4, " T  TTT         ");
const Shape T_RIGHT = Shape(4, 4, "T   TT  T       ");
const Shape T_DOWN  = Shape(4, 4, "    TTT  T      ");
const Shape T_LEFT  = Shape(4, 4, " T  TT   T      ");

const Shape Z_UP    = Shape(4, 4, "ZZ   ZZ         ");
const Shape Z_RIGHT = Shape(4, 4, "  Z  ZZ  Z      ");
const Shape Z_DOWN  = Shape(4, 4, "    ZZ   ZZ     ");
const Shape Z_LEFT  = Shape(4, 4, " Z  ZZ  Z       ");

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
   board(Shape(20, 10, initialBoardGrid)),
   pieces{
     [I] = Piece(I_UP, I_RIGHT, I_DOWN, I_LEFT),
     [J] = Piece(J_UP, J_RIGHT, J_DOWN, J_LEFT),
     [L] = Piece(L_UP, L_RIGHT, L_DOWN, L_LEFT),
     [O] = Piece(O_UP, O_RIGHT, O_DOWN, O_LEFT),
     [S] = Piece(S_UP, S_RIGHT, S_DOWN, S_LEFT),
     [T] = Piece(T_UP, T_RIGHT, T_DOWN, T_LEFT),
     [Z] = Piece(Z_UP, Z_RIGHT, Z_DOWN, Z_LEFT),
   }
{
  reset();
  produceNextPiece();
}

void Game::setGrid(const char *grid) {
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

    case GAME_OVER:
      return;

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
  // Classic scoring.
  if (rows == 1) stats.score += 30;
  if (rows == 2) stats.score += 100;
  if (rows == 3) stats.score += 300;
  if (rows >= 4) stats.score += 1200;
}

void Game::scoreDroppedRows(int rows) {
  // Classic scoring.
  stats.score += rows;
}

void Game::render() {
  // Erase the canvas. Then print the board and current piece on it.
  canvas.fillWith(EMPTY_SPACE);
  canvas.stick(board, (struct point) { 0, 0});
  canvas.stick(pieces[currentPieceName].getCurrentShape(),
               pieces[currentPieceName].getCoordinates());
}

void Game::reset() {
  stats = (struct stats) { 0 };

  board.fillWith(EMPTY_SPACE);
  canvas.fillWith(EMPTY_SPACE);

  state = PLAYING;
}

void Game::gameOver() {
  state = GAME_OVER;
  gameOverCallback();
}

