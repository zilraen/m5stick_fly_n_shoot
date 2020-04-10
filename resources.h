#pragma once

#include <vector>
#include <M5StickC.h>

// background
const unsigned int BCKGRDCOL = TFT_BLACK;

// sprite colors (Cx name for values to keep the array readable)
#define C0 BCKGRDCOL
#define CR TFT_RED
#define CB TFT_BLUE
#define CW TFT_WHITE
#define CG 0x7bef
#define CY TFT_YELLOW

const unsigned k_frame_size = 8;

static unsigned int px_spaceship[] =
{ C0, C0, C0, CW, CW, C0, C0, C0,
  C0, C0, C0, CB, CB, C0, C0, C0,
  C0, C0, C0, CB, CB, C0, C0, C0,
  CW, C0, C0, CW, CW, C0, C0, CW,
  CW, C0, CW, CW, CW, CW, C0, CW,
  CW, CW, CW, CW, CW, CW, CW, CW,
  CW, CW, CW, CR, CR, CW, CW, CW,
  CW, C0, C0, CR, CR, C0, C0, CW
  };

// Yellow alien, 'medium'
static unsigned int px_alien0[] =
{ CY, C0, C0, CY, CY, C0, C0, CY,
  CY, C0, CY, CY, CY, CY, C0, CY,
  C0, CY, CY, CB, CB, CY, CY, C0,
  C0, C0, CY, CB, CB, CY, C0, C0,
  CY, CY, CY, CY, CY, CY, CY, CY,
  CY, C0, C0, CY, CY, C0, C0, CY,
  CY, C0, C0, CR, CR, C0, C0, CY,
  C0, CY, C0, CR, CR, C0, CY, C0 
  };

// Red alien, 'heavy'
static unsigned int px_alien1[] =
{ CR, C0, C0, CR, CR, C0, C0, CR,
  CR, CR, CR, CR, CR, CR, CR, CR,
  CR, CR, CR, CR, CR, CR, CR, CR,
  CR, CR, CR, CY, CY, CR, CR, CR,
  CR, C0, CR, CR, CR, CR, C0, CR,
  CR, C0, C0, CR, CR, C0, C0, CR,
  CR, C0, C0, CR, CR, C0, C0, CR,
  C0, C0, C0, CR, CR, C0, C0, C0 
  };

// Blue alien, 'nimble'
static unsigned int px_alien2[] =
{ C0, C0, C0, CB, CB, C0, C0, C0,
  C0, C0, C0, CB, CB, C0, C0, C0,
  C0, C0, CB, CY, CY, CB, C0, C0,
  C0, C0, CB, CY, CY, CB, C0, C0,
  CB, CB, CB, CB, CB, CB, CB, CB,
  CB, C0, C0, CB, CB, C0, C0, CB,
  CB, C0, C0, CR, CR, C0, C0, CB,
  C0, CB, C0, CR, CR, C0, CB, C0 
  };

static unsigned int px_missile[] =
{ C0, C0, C0, CW, CW, C0, C0, C0,
  C0, C0, C0, CW, CW, C0, C0, C0,
  C0, C0, C0, CW, CW, C0, C0, C0,
  C0, C0, CW, CW, CW, CW, C0, C0,
  C0, C0, CW, CW, CW, CW, C0, C0,
  C0, C0, C0, CG, CG, C0, C0, C0,
  C0, C0, C0, CR, CR, C0, C0, C0,
  C0, C0, C0, CR, CR, C0, C0, C0 
  };

static unsigned int px_explosion_f0[] =
{ C0, C0, C0, C0, C0, C0, C0, C0,
  C0, C0, C0, C0, C0, C0, CR, C0,
  C0, C0, C0, CR, C0, CY, C0, C0,
  C0, C0, C0, CG, CG, C0, C0, C0,
  C0, C0, C0, CG, CW, C0, C0, C0,
  C0, C0, C0, C0, CR, CW, C0, C0,
  C0, CW, CR, C0, CW, CR, C0, C0,
  C0, CR, C0, C0, C0, C0, C0, C0 
  };

static unsigned int px_explosion_f1[] =
{ CW, CY, CG, CW, C0, C0, C0, C0,
  CY, CR, CW, CG, C0, C0, CY, CW,
  C0, C0, C0, CR, C0, CR, CR, CY,
  C0, C0, CR, CR, CG, C0, C0, CY,
  C0, C0, CR, CR, CW, C0, C0, C0,
  C0, C0, CR, CR, CR, CW, C0, C0,
  C0, CW, CW, CR, CW, CR, C0, C0,
  C0, CR, CR, CW, C0, CW, C0, C0 
  };

static unsigned int px_star_f0[] =
{ C0, C0, C0, C0, C0, C0, C0, C0,
  C0, C0, C0, CB, C0, C0, C0, C0,
  C0, C0, C0, CW, C0, C0, C0, C0,
  C0, CW, CB, CW, CB, CW, C0, C0,
  C0, C0, C0, CW, C0, C0, C0, C0,
  C0, C0, C0, CB, C0, C0, C0, C0,
  C0, C0, C0, C0, C0, C0, C0, C0,
  C0, C0, C0, C0, C0, C0, C0, C0 
  };

static unsigned int px_star_f1[] =
{ C0, C0, C0, C0, C0, C0, C0, C0,
  C0, C0, C0, C0, C0, C0, C0, C0,
  C0, C0, CW, CB, CW, C0, C0, C0,
  C0, CB, CB, CB, CB, CB, C0, C0,
  C0, C0, CW, CB, CW, C0, C0, C0,
  C0, C0, C0, C0, C0, C0, C0, C0,
  C0, C0, C0, C0, C0, C0, C0, C0,
  C0, C0, C0, C0, C0, C0, C0, C0 
  };

static unsigned int px_blank[] =
{ C0, C0, C0, C0, C0, C0, C0, C0,
  C0, C0, C0, C0, C0, C0, C0, C0,
  C0, C0, C0, C0, C0, C0, C0, C0,
  C0, C0, C0, C0, C0, C0, C0, C0,
  C0, C0, C0, C0, C0, C0, C0, C0,
  C0, C0, C0, C0, C0, C0, C0, C0,
  C0, C0, C0, C0, C0, C0, C0, C0,
  C0, C0, C0, C0, C0, C0, C0, C0 
  };

std::vector<unsigned*> vfx_explosion{px_explosion_f0, px_explosion_f1, px_explosion_f0, px_explosion_f1, px_blank};
std::vector<unsigned*> vfx_star{px_star_f0, px_star_f1};
