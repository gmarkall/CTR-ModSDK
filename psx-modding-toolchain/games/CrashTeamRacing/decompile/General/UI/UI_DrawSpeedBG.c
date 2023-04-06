#include <common.h>

#define SPEEDO_GREEN 0xb500;
#define SPEEDO_YELLOW 0xd1ff;
#define SPEEDO_RED 0xdb;

// speedometer background
void DECOMP_UI_DrawSpeedBG(void)
{
  struct GameTracker* gGT;
  u_int color_gradient0, color_gradient1;
  u_long* ot;
  POLY_G4 *p;
  short* vertData;
  short* upperHalf;
  struct DB* backDB;
  int i;

  gGT = sdata->gGT;

  vertData = &data.speedometerBG_vertData;
  upperHalf = &vertData[0x1c];
  backDB = gGT->backBuffer;
  
  for (i = 0; i < 7; i++) 
  {
	  // white
    CTR_Box_DrawWirePrims(
		(u_short)(upperHalf[0] + 0x1e0),
		(u_short)(upperHalf[1] + 0xbe),
		(u_short)(upperHalf[2] + 0x1e0),
		(u_short)(upperHalf[3] + 0xbe),
		0xff, 0xff, 0xff,
		gGT->tileView_UI.ptrOT,
		&backDB->primMem);
    
	// black
    CTR_Box_DrawWirePrims(
		(u_short)(upperHalf[0] + 0x1e1),
		(u_short)(upperHalf[1] + 0xbf),
		(u_short)(upperHalf[2] + 0x1e1),
		(u_short)(upperHalf[3] + 0xbf),
		0, 0, 0,
		gGT->tileView_UI.ptrOT,
		&backDB->primMem);
      
	upperHalf += 4;
  }
  
  for (i = 0; i < 6; i++) 
  {
	// white
    CTR_Box_DrawWirePrims(
		(u_short)(vertData[0] + 0x1e0),
		(u_short)(vertData[1] + 0xbe),
		(u_short)(vertData[4] + 0x1e0),
		(u_short)(vertData[5] + 0xbe),
		0xff, 0xff, 0xff,
		gGT->tileView_UI.ptrOT,
		&backDB->primMem);

	// white
    CTR_Box_DrawWirePrims(
		(u_short)(vertData[2] + 0x1e0),
		(u_short)(vertData[3] + 0xbe),
		(u_short)(vertData[6] + 0x1e0),
		(u_short)(vertData[7] + 0xbe),
		0xff, 0xff, 0xff,
		gGT->tileView_UI.ptrOT,
		&backDB->primMem);

	// black
    CTR_Box_DrawWirePrims(
		(u_short)(vertData[0] + 0x1e1),
		(u_short)(vertData[1] + 0xbf),
		(u_short)(vertData[4] + 0x1e1),
		(u_short)(vertData[5] + 0xbf),
		0, 0, 0,
		gGT->tileView_UI.ptrOT,
		&backDB->primMem);

	// black
    CTR_Box_DrawWirePrims(
		(u_short)(vertData[2] + 0x1e1),
		(u_short)(vertData[3] + 0xbf),
		(u_short)(vertData[6] + 0x1e1),
		(u_short)(vertData[7] + 0xbf),
		0, 0, 0,
		gGT->tileView_UI.ptrOT,
		&backDB->primMem);

    // reset prim
    p = backDB->primMem.curr;
    if (p > (u_long *)backDB->primMem.endMin100)
		return;
    backDB->primMem.curr = p + 1;
    
    color_gradient0 = SPEEDO_GREEN;
    color_gradient1 = color_gradient0;
    if ((1 < i) && (color_gradient1 = SPEEDO_YELLOW, 2 < i)) {
      color_gradient0 = SPEEDO_YELLOW;
      if (i < 4) {
        color_gradient1 = SPEEDO_RED;
      } else {
        color_gradient0 = SPEEDO_RED;
        color_gradient1 = color_gradient0;
      }
    }

    *(int*)&p->r0 = color_gradient0; // RGB0
    *(int*)&p->r1 = color_gradient0; // RGB1
    *(int*)&p->r2 = color_gradient1; // RGB2
    *(int*)&p->r3 = color_gradient1; // RGB3
	
    setPolyG4(p);
    setXY4(p, 
    vertData[0] + 0x1e0, vertData[1] + 0xbe, 
    vertData[2] + 0x1e0, vertData[3] + 0xbe, 
    vertData[4] + 0x1e0, vertData[5] + 0xbe, 
    vertData[6] + 0x1e0, vertData[7] + 0xbe);

	// inline AddPrim
    ot = gGT->tileView_UI.ptrOT;
    *(int*)p = *ot | 0x8000000;
    *ot = (u_int)p & 0xffffff;
    
	vertData += 4;
  }
  
  vertData = &data.speedometerBG_vertData;
  
  for (i = 0; i < 6; i++) 
  {
    p = backDB->primMem.curr;
    if (p > (u_long *)backDB->primMem.endMin100) 
		return;
    backDB->primMem.curr = p + 1;
    
    setlen(p, 8);
    setcode(p, 0xe1);
    setRGB0(p, 0, 0xa, 0);
    setRGB1(p, 0, 0, 0);
    setRGB2(p, 0, 0, 0);
    setRGB3(p, 0, 0, 0);
    p->pad0 = 0x32;
    setXY4(p, 0, 0, 
    vertData[2] + 0x1e0, vertData[3] + 0xbe,
    vertData[6] + 0x1e0, vertData[7] + 0xbe,
    data.speedometerBG_vertData[0x1a] + 0x1e0, data.speedometerBG_vertData[3] + 0xbe);

	// inline AddPrim
    ot = gGT->tileView_UI.ptrOT;
    *(int*)p = *ot | 0x8000000;
    *ot = (u_int)p & 0xffffff;
    
	vertData += 4;
  }
  return;
}