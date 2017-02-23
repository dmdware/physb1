

#ifndef RASTER_H
#define RASTER_H

#include "../math/vec2f.h"
#include "../math/vec3f.h"
#include "../texture.h"
#include "../math/fixfrac.h"
#include "../math/matrix.h"
#include "../texture.h"

void RasterPt(Matrix* mvp, FixFrac* screendepths, LoadedTex* screencolors, Vec3f vecpos, Vec4f color);
void RasterLine(Matrix* mvp, FixFrac* screendepths, LoadedTex* screencolors, Vec3f* vecpos, Vec4f* color);
void RasterLine(Matrix* mvp, FixFrac* screendepths, LoadedTex* screencolors, Vec3f* vecpos, Vec4f color);
void RasterTri(Matrix* mvp, FixFrac* screendepths, LoadedTex* screencolors, Vec3f* vecpos, Vec4f* color);

void RasterPt(Matrix* mvp, FixFrac* screendepths, LoadedTex* screencolors, Vec3f vecpos, Vec2f texcoord, LoadedTex* tex);
void RasterLine(Matrix* mvp, FixFrac* screendepths, LoadedTex* screencolors, Vec3f* vecpos, Vec2f* texcoords, LoadedTex* tex);
void RasterTri(Matrix* mvp, FixFrac* screendepths, LoadedTex* screencolors, Vec3f* vecpos, Vec2f* texcoords, LoadedTex* tex);

void RasterPts(Matrix* mvp, FixFrac* screendepths, LoadedTex* screencolors, Vec3f* vecpos, Vec2f* texcoord, LoadedTex* tex, int npt);
void RasterLines(Matrix* mvp, FixFrac* screendepths, LoadedTex* screencolors, Vec3f* vecpos, Vec2f* texcoords, LoadedTex* tex, int npt);
void RasterLines(Matrix* mvp, FixFrac* screendepths, LoadedTex* screencolors, Vec3f* vecpos, Vec4f* colors, int npt);
void RasterLines(Matrix* mvp, FixFrac* screendepths, LoadedTex* screencolors, Vec3f* vecpos, Vec4f color, int npt);
void RasterTris(Matrix* mvp, FixFrac* screendepths, LoadedTex* screencolors, Vec3f* vecpos, Vec2f* texcoords, LoadedTex* tex, int npt);

#endif
