#include <piece.h>

// Reference for pieces: https://tetris.fandom.com/wiki/Orientation

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

