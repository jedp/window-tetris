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

bool Game::moveLeft() {
  return move(LEFT);
}

bool Game::moveRight() {
  return move(RIGHT);
}

bool Game::move(orientation_t dir) {
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
  while(move(DOWN)) ;

  board.stick(pieces[currentPieceName].getCurrentShape(),
             pieces[currentPieceName].getCoordinates());
  produceNextPiece();
}

void Game::tick() {
  if (!move(DOWN)) {
    board.stick(pieces[currentPieceName].getCurrentShape(),
               pieces[currentPieceName].getCoordinates());
    produceNextPiece();
  }
}

void Game::produceNextPiece() {
  currentPieceName = sequence.next();

  if (board.collides(pieces[currentPieceName].getCurrentShape(),
                     START_COORDINATES)) {
    gameOver();
  }

  pieces[currentPieceName].setCoordinates(START_COORDINATES);

  render();
}

void Game::render() {
  canvas.fillWith(EMPTY_SPACE);
  canvas.stick(board, (struct point) { 0, 0});
  canvas.stick(pieces[currentPieceName].getCurrentShape(), pieces[currentPieceName].getCoordinates());
  canvas.updateBoundingBox();
}

void Game::reset() {
  score = 0;

  board.fillWith(EMPTY_SPACE);
  canvas.fillWith(EMPTY_SPACE);
}

void Game::gameOver() {
  gameOverCallback();
}

