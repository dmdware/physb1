

#ifndef HYPMESH_H
#define HYPMESH_H

#include "../sim/simdef.h"
#include "../math/vec3f.h"

#if 0
const FixFrac REND3PTINSCOLORS[REND_3PT_INS+100][4] = 
{
{1,0,0,1},
{0,1,0,1},
{0,0,1,1},
{1,1,0,1},
{1,0,1,1},
{0,1,1,1},
{0.5f,0,0.5f,1},//7
{0,0.5f,0.5f,1},
{0.5f,0.5f,0,1},//9
{0.5f,0.5f,1,1},
};
#endif

struct Color
{
	float rgba[4];
	char name[32];
};

const Color REND3PTINSCOLORS[48] =
{
	{{0x7e/255.0f, 0x1e/255.0f, 0x9c/255.0f, 1.0f}, "Purple"},
	{{0x15/255.0f, 0xb0/255.0f, 0x1a/255.0f, 1.0f}, "Green"},
	{{0x03/255.0f, 0x43/255.0f, 0xdf/255.0f, 1.0f}, "Blue"},
	{{0xff/255.0f, 0x81/255.0f, 0xc0/255.0f, 1.0f}, "Pink"},
	{{0x65/255.0f, 0x37/255.0f, 0x00/255.0f, 1.0f}, "Brown"},
	{{0xe5/255.0f, 0x00/255.0f, 0x00/255.0f, 1.0f}, "Red"},
	{{0x95/255.0f, 0xd0/255.0f, 0xfc/255.0f, 1.0f}, "Light Blue"},
	{{0x02/255.0f, 0x93/255.0f, 0x86/255.0f, 1.0f}, "Teal"},
	{{0xf9/255.0f, 0x73/255.0f, 0x06/255.0f, 1.0f}, "Orange"},
	{{0x96/255.0f, 0xf9/255.0f, 0x7b/255.0f, 1.0f}, "Light Green"},
	{{0xc2/255.0f, 0x00/255.0f, 0x78/255.0f, 1.0f}, "Magenta"},
	{{0xff/255.0f, 0xff/255.0f, 0x14/255.0f, 1.0f}, "Yellow"},
	{{0x75/255.0f, 0xbb/255.0f, 0xfd/255.0f, 1.0f}, "Sky Blue"},
	{{0x92/255.0f, 0x95/255.0f, 0x91/255.0f, 1.0f}, "Grey"},
	{{0x89/255.0f, 0xfe/255.0f, 0x05/255.0f, 1.0f}, "Lime Green"},
	{{0xbf/255.0f, 0x77/255.0f, 0xf6/255.0f, 1.0f}, "Light Purple"},
	{{0x9a/255.0f, 0x0e/255.0f, 0xea/255.0f, 1.0f}, "Violet"},
	{{0x33/255.0f, 0x35/255.0f, 0x00/255.0f, 1.0f}, "Dark Green"},
	{{0x06/255.0f, 0xc2/255.0f, 0xac/255.0f, 1.0f}, "Turquoise"},
	{{0xc7/255.0f, 0x9f/255.0f, 0xef/255.0f, 1.0f}, "Lavender"},
	{{0x00/255.0f, 0x03/255.0f, 0x5b/255.0f, 1.0f}, "Dark Blue"},
	{{0xd1/255.0f, 0xb2/255.0f, 0x6f/255.0f, 1.0f}, "Tan"},
	{{0x00/255.0f, 0xff/255.0f, 0xff/255.0f, 1.0f}, "Cyan"},
	{{0x13/255.0f, 0xea/255.0f, 0xc9/255.0f, 1.0f}, "Aqua"},
	{{0x06/255.0f, 0x47/255.0f, 0x0c/255.0f, 1.0f}, "Forest Green"},
	{{0xae/255.0f, 0x71/255.0f, 0x81/255.0f, 1.0f}, "Mauve"},
	{{0x35/255.0f, 0x06/255.0f, 0x3e/255.0f, 1.0f}, "Dark Purple"},
	{{0x01/255.0f, 0xff/255.0f, 0x07/255.0f, 1.0f}, "Bright Green"},
	{{0x65/255.0f, 0x00/255.0f, 0x21/255.0f, 1.0f}, "Maroon"},
	{{0x6e/255.0f, 0x75/255.0f, 0x0e/255.0f, 1.0f}, "Olive"},
	{{0xff/255.0f, 0x79/255.0f, 0x6c/255.0f, 1.0f}, "Salmon"},
	{{0xe6/255.0f, 0xda/255.0f, 0xa6/255.0f, 1.0f}, "Beige"},
	{{0x05/255.0f, 0x04/255.0f, 0xaa/255.0f, 1.0f}, "Royal Blue"},
	{{0x00/255.0f, 0x11/255.0f, 0x46/255.0f, 1.0f}, "Navy Blue"},
	{{0xce/255.0f, 0xa2/255.0f, 0xfd/255.0f, 1.0f}, "Lilac"},
	{{0x00/255.0f, 0x00/255.0f, 0x00/255.0f, 1.0f}, "Black"},
	{{0xff/255.0f, 0x02/255.0f, 0x8d/255.0f, 1.0f}, "Hot Pink"},
	{{0xad/255.0f, 0x81/255.0f, 0x50/255.0f, 1.0f}, "Light Brown"},
	{{0xc7/255.0f, 0xfd/255.0f, 0xb5/255.0f, 1.0f}, "Pale Green"},
	{{0xff/255.0f, 0xb0/255.0f, 0x7c/255.0f, 1.0f}, "Peach"},
	{{0x67/255.0f, 0x7a/255.0f, 0x04/255.0f, 1.0f}, "Olive Green"},
	{{0xcb/255.0f, 0x41/255.0f, 0x6b/255.0f, 1.0f}, "Dark Pink"},
	{{0x8e/255.0f, 0x82/255.0f, 0xfe/255.0f, 1.0f}, "Periwinkle"},
	{{0x53/255.0f, 0xfc/255.0f, 0xa1/255.0f, 1.0f}, "Sea Green"},
	{{0xaa/255.0f, 0xff/255.0f, 0x32/255.0f, 1.0f}, "Lime"},
	{{0x38/255.0f, 0x02/255.0f, 0x82/255.0f, 1.0f}, "Indigo"},
	{{0xce/255.0f, 0xb3/255.0f, 0x01/255.0f, 1.0f}, "Mustard"},
	{{0xff/255.0f, 0xd1/255.0f, 0xdf/255.0f, 1.0f}, "Light Pink"}
};


class HypVecf
{
public:
	Vec3f rend[REND_3PT_INS];
	FixFrac pos[HYP_DIMS];
	FixFrac age;

	std::list<HypVecf*> joins;
	std::list<HypVecf*> temp;
	std::list<HypVecf*> hist;

	HypVecf()
	{
		age = FixFrac(INTBASIS(1));
		memset(pos,0,sizeof(FixFrac)*HYP_DIMS);
		memset(rend,0,sizeof(Vec3f)*REND_3PT_INS);
	}
};

class HypMeshf
{
public:
	HypVecf cen;
	std::list<HypVecf> pts;
	FixFrac vel[HYP_DIMS];
	FixFrac mass;

	HypMeshf()
	{
		mass = FixFrac(INTBASIS(1));
		//memset(&cen,0,sizeof(HypVecf)*1);
		//memset(vel,0,sizeof(FixFrac)*HYP_DIMS);
	}
};

class Craft
{
public:
	HypVecf beadpos;
	HypVecf craftpos;
	HypVecf beadvel;
	HypVecf craftvel;
	HypVecf craftthrust;
};

extern Craft gc;

void UpdRend(HypMeshf* hm);
void DrawHypMesh(HypMeshf* hm);
void DrawHypMesh(HypMeshf* hm, int rpi);
void CvPt(FixFrac* cen, HypVecf* pit);
FixFrac HypDistf(FixFrac* a, FixFrac* b);
void DrawHypVecs(std::list<HypVecf>* hl, float sz=3.0f);

#endif



