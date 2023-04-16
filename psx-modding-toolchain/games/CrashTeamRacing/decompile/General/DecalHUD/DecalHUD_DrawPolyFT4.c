#include <common.h>

#define EDUCATIONAL_BUG_IF 0

void DECOMP_DecalHUD_DrawPolyFT4(struct Icon* icon, short posX, short posY, struct PrimMem* primMem, u_long* ot, char transparency, int scale)
{
	if (!icon) return;

	POLY_FT4* p = (POLY_FT4*)primMem->curr;
	addPolyFT4(ot, p);

	// this function doesn't support coloring the primitives
	setShadeTex(p, true);

	unsigned int width = icon->texLayout.u1 - icon->texLayout.u0;
	unsigned int height = icon->texLayout.v2 - icon->texLayout.v0;
	unsigned int bottomY = posY + (height * scale / 0x1000);
	
	#if EDUCATIONAL_BUG_IF
		// The original compiled version of DecalHUD_DrawPolyFT4 has a bug where the Y coordinate of the top
		// right vertex of the primitive (henceforth referred to as y1) can overflow by 1 if the X offset of
		// the primitive (centered on the top left vertex of the primitive, henceforth referred to as x0) is
		// set to a negative value. This is due to an optimization in the code: with x0 being a 32-bit value
		// truncated to 16 bits, the value for the X coordinate of the top right vertex (henceforth referred to
		// as x1), itself the offset of x0 by the width of the texture, is stored in a 32-bit variable; as the
		// function sets the primitive's X and Y coordinate values by dereferencing both of them as a singular
		// 32-bit integer, with x1 and y1's bits being added together using bitwise OR, the bottom 16 bits of
		// x1 are added onto y1's bits, resulting in y1's value being altered by x1's overflow.
		// This also affects the bottom right vertex.

		unsigned int rightX = (posX & 0xFFFF) + (width * scale / 0x1000);
		unsigned int rightXOverflow = rightX >> 16;
		setXY4(p, posX, posY, rightX, posY | rightXOverflow, posX, bottomY, rightX, bottomY | rightXOverflow);
	#else
		unsigned int rightX = posX + (width * scale / 0x1000);
		setXY4(p, posX, posY, rightX, posY, posX, bottomY, rightX, bottomY);
	#endif
	setIconUV(p, icon);

	if (transparency)
	{
		setTransparency(p, transparency);
	}

	primMem->curr = p + 1;
}