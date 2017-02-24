

#include "raster.h"
#include "../window.h"
#include "../math/barycentric.h"
#include "../math/plane3f.h"
#include "../math/3dmath.h"

Vec4f barycentric(Vec4f *pts, Vec2f P)
{
    Vec3f u = Cross(Vec3f(pts[2][0]-pts[0][0], pts[1][0]-pts[0][0], pts[0][0]-P[0]),
		Vec3f(pts[2][1]-pts[0][1], pts[1][1]-pts[0][1], pts[0][1]-P[1]));
    if (Abs(u[2].n/u[2].d)<1) 
   // if (Abs((u[2]))<FixFrac(INTBASIS(1))) 
	return Vec4f(INTBASIS(-1.0),INTBASIS(1),INTBASIS(1),INTBASIS(1)); // triangle is degenerate, in this case return smth with negative coordinates
    return Vec4f(FixFrac(INTBASIS(1))-(u.x+u.y)/u.z, u.y/u.z, u.x/u.z, INTBASIS(1));
}

void RasterPt(Matrix* mvp, FixFrac* screendepths, LoadedTex* screencolors, Vec3f vecpos, Vec4f color)
{
	Vec4f screenpos = ScreenPos(mvp, vecpos, FixFrac(INTBASIS(g_width)), FixFrac(INTBASIS(g_height)), true);
#if 0
	char m[12333];
	sprintf(m, "rp %lld/%lld,%lld/%lld,%lld/%lld,%lld/%lld",
		screenpos.x.n,screenpos.x.d,
		screenpos.y.n,screenpos.y.d,
		screenpos.z.n,screenpos.z.d,
		screenpos.w.n,screenpos.w.d);
	InfoMess(m,m);
#endif
	//InfoMess("as",",asa");

	if(screenpos.x < FixFrac(INTBASIS(0)) ||
		screenpos.x >= FixFrac(INTBASIS(g_width)) ||
		screenpos.y < FixFrac(INTBASIS(0)) ||
		screenpos.y >= FixFrac(INTBASIS(g_height)))
		return;

	int dpi = ToBasis(screenpos.x+screenpos.y*screencolors->sizex)*1;
	int cpi = ToBasis(screenpos.x+screenpos.y*screencolors->sizex)*screencolors->channels;

	//if(screendepths[dpi] <= screenpos.z)
	//	return;

	//char m[123];
	//sprintf(m, "%llu<%llu", (double)screendepths[dpi].n/*.toDouble()*/, (double)screenpos.z.n/*.toDouble()*/);
	//InfoMess(m,m);

	//InfoMess("2","2");
	//Log("ras %lf,%lf", screenpos.x.n/*.toDouble()*/,screenpos.y.n/*.toDouble()*/);

	screencolors->data[cpi+0] = ToBasis(color.x*FixFrac(INTBASIS(255)));
	screencolors->data[cpi+1] = ToBasis(color.y*FixFrac(INTBASIS(255)));
	screencolors->data[cpi+2] = ToBasis(color.z*FixFrac(INTBASIS(255)));
	screencolors->data[cpi+3] = 255;
}

void RasterLine(Matrix* mvp, FixFrac* screendepths, LoadedTex* screencolors, Vec3f* vecpos, Vec4f* color)
{
	Vec4f screenpos[2];
	screenpos[0] = ScreenPos(mvp, vecpos[0], FixFrac(INTBASIS(g_width)), FixFrac(INTBASIS(g_height)), true);
	screenpos[1] = ScreenPos(mvp, vecpos[1], FixFrac(INTBASIS(g_width)), FixFrac(INTBASIS(g_height)), true);

	Vec2f fromscreen = Vec2f(screenpos[0].x,screenpos[0].y);
	Vec2f toscreen = Vec2f(screenpos[1].x,screenpos[1].y);
	FixFrac d = MAG_VEC2F(fromscreen-toscreen);

	for(FixFrac i=FixFrac(INTBASIS(0)); i<=d; i=i+FixFrac(INTBASIS(1)))
	{
		Vec3f subpos = (screenpos[0] + (screenpos[1]-screenpos[0]) * i / d);

		if(subpos.x < FixFrac(INTBASIS(0)) ||
			subpos.x >= FixFrac(INTBASIS(g_width)) ||
			subpos.y < FixFrac(INTBASIS(0)) ||
			subpos.y >= FixFrac(INTBASIS(g_height)))
			continue;

		int dpi = ToBasis(subpos.x+subpos.y*screencolors->sizex)*1;
		int cpi = ToBasis(subpos.x+subpos.y*screencolors->sizex)*screencolors->channels;

		if(screendepths[dpi] <= subpos.z)
			return;

		Vec4f subcolor = (color[0] + color[1] * i / d);

		screencolors->data[cpi+0] = ToBasis(subcolor.x*FixFrac(INTBASIS(255)));
		screencolors->data[cpi+1] = ToBasis(subcolor.y*FixFrac(INTBASIS(255)));
		screencolors->data[cpi+2] = ToBasis(subcolor.z*FixFrac(INTBASIS(255)));
		screencolors->data[cpi+3] = 255;
	}
}
void RasterLine(Matrix* mvp, FixFrac* screendepths, LoadedTex* screencolors, Vec3f* vecpos, Vec4f color)
{
	Vec4f screenpos[2];
	screenpos[0] = ScreenPos(mvp, vecpos[0], FixFrac(INTBASIS(g_width)), FixFrac(INTBASIS(g_height)), true);
	screenpos[1] = ScreenPos(mvp, vecpos[1], FixFrac(INTBASIS(g_width)), FixFrac(INTBASIS(g_height)), true);

	Vec2f fromscreen = Vec2f(screenpos[0].x,screenpos[0].y);
	Vec2f toscreen = Vec2f(screenpos[1].x,screenpos[1].y);
	FixFrac d = MAG_VEC2F(fromscreen-toscreen);

	for(FixFrac i=FixFrac(INTBASIS(0)); i<=d; i=i+FixFrac(INTBASIS(1)))
	{
		Vec3f subpos = (screenpos[0] + (screenpos[1]-screenpos[0]) * i / d);

		if(subpos.x < FixFrac(INTBASIS(0)) ||
			subpos.x >= FixFrac(INTBASIS(g_width)) ||
			subpos.y < FixFrac(INTBASIS(0)) ||
			subpos.y >= FixFrac(INTBASIS(g_height)))
			continue;

		int dpi = ToBasis(subpos.x+subpos.y*screencolors->sizex)*1;
		int cpi = ToBasis(subpos.x+subpos.y*screencolors->sizex)*screencolors->channels;

		if(screendepths[dpi] <= subpos.z)
			return;

		screencolors->data[cpi+0] = ToBasis(color.x*FixFrac(INTBASIS(255)));
		screencolors->data[cpi+1] = ToBasis(color.y*FixFrac(INTBASIS(255)));
		screencolors->data[cpi+2] = ToBasis(color.z*FixFrac(INTBASIS(255)));
		screencolors->data[cpi+3] = 255;
	}
}

void RasterTri(Matrix* mvp, FixFrac* screendepths, LoadedTex* screencolors, Vec3f* vecpos, Vec4f* color)
{

	Vec4f screenpos[3];

	for(int vi=0; vi<3; ++vi)
		screenpos[vi] = ScreenPos(mvp, vecpos[vi], FixFrac(INTBASIS(g_width)), FixFrac(INTBASIS(g_height)), true);

	Vec2f vmin = Vec2f(screenpos[0].x,screenpos[0].y);
	Vec2f vmax = vmin;

	for(int vi=1; vi<3; ++vi)
	{
		if(screenpos[vi].x < vmin.x)
			vmin.x = screenpos[vi].x;
		if(screenpos[vi].y < vmin.y)
			vmin.y = screenpos[vi].y;
		if(screenpos[vi].x > vmax.x)
			vmax.x = screenpos[vi].x;
		if(screenpos[vi].y > vmax.y)
			vmax.y = screenpos[vi].y;
	}

	Vec3f norm = Normalize( Cross(vecpos[0]-vecpos[1],vecpos[0]-vecpos[2]) );
	//Vec3f normtexc = Normal(texcoords);
	Plane3f plane;
	MakePlane(&plane.m_normal, &plane.m_d, screenpos[0], norm);

	//Vec3f to1 = screenpos[1] - screenpos[0];
	//Vec3f to2 = screenpos[2] - screenpos[0];

	for(FixFrac x=vmin.x; x<=vmax.x; x+=FixFrac(INTBASIS(1)))
	{
		for(FixFrac y=vmin.y; y<=vmax.y; y+=FixFrac(INTBASIS(1)))
		{
			Vec3f bc_screen  = barycentric(screenpos, Vec2f(x,y));
			if (bc_screen.x<FixFrac(INTBASIS(0)) || bc_screen.y<FixFrac(INTBASIS(0)) || bc_screen.z<FixFrac(INTBASIS(0)))
				continue;
			Vec2f subtexcoord;
			Vec3f subpos;
			subpos = Vec3f(x,y,INTBASIS(0));

			Vec3f into[2];
			into[0] = Vec3f(INTBASIS(0),INTBASIS(0),INTBASIS(0)) + subpos;
			into[1] = Vec3f(INTBASIS(0),INTBASIS(0),INTBASIS(1)) + subpos;

			LineInterPlane(into, plane.m_normal, plane.m_d, &subpos);

			FixFrac to0 = MAG_VEC3F( Vec3f(screenpos[0]) - subpos );
			FixFrac to1 = MAG_VEC3F( Vec3f(screenpos[1]) - subpos );
			FixFrac to2 = MAG_VEC3F( Vec3f(screenpos[2]) - subpos );
			FixFrac all = to0 + to1 + to2;
			to0 = to0 / all;
			to1 = to1 / all;
			to2 = to2 / all;

			Vec4f subcolor;
			subcolor = (color[0]*to0 + color[1]*to1 + color[2]*to2);
			//color = color / FixFrac(INTBASIS(255));

			Vec3f subvecpos = (vecpos[0]*to0 + vecpos[1]*to1 + vecpos[2]*to2);

			RasterPt(mvp, screendepths, screencolors, subvecpos, subcolor);
		}
	}
}

void RasterPt(Matrix* mvp, FixFrac* screendepths, LoadedTex* screencolors, Vec3f vecpos, Vec2f texcoord, LoadedTex* tex)
{
	//texcoord.x = (texcoord.x % FixFrac(INTBASIS(1)));
	//texcoord.y = (texcoord.y % FixFrac(INTBASIS(1)));

	if(texcoord.x < FixFrac(INTBASIS(0)))
		texcoord.x = FixFrac(INTBASIS(1)) - texcoord.x;
	if(texcoord.y < FixFrac(INTBASIS(0)))
		texcoord.y = FixFrac(INTBASIS(1)) - texcoord.y;

	texcoord.x = texcoord.x * FixFrac(INTBASIS(tex->sizex));
	texcoord.y = texcoord.y * FixFrac(INTBASIS(tex->sizey));

	texcoord.x = ((int64_t)texcoord.x % tex->sizex);
	texcoord.y = ((int64_t)texcoord.y % tex->sizey);

	int ti = (ToBasis(texcoord.x)+ToBasis(texcoord.y)*tex->sizex)*tex->channels;

	Vec4f color;
	color.x = FixFrac((INTBASIS)tex->data[ti+0])/FixFrac(INTBASIS(255));
	color.y = FixFrac((INTBASIS)tex->data[ti+1])/FixFrac(INTBASIS(255));
	color.z = FixFrac((INTBASIS)tex->data[ti+2])/FixFrac(INTBASIS(255));
	//color.w = FixFrac((INTBASIS)tex->data[ti+3])/FixFrac(INTBASIS(255));

	RasterPt(mvp, screendepths, screencolors, vecpos, color);
}

void RasterLine(Matrix* mvp, FixFrac* screendepths, LoadedTex* screencolors, Vec3f* vecpos, Vec2f* texcoords, LoadedTex* tex)
{
	Vec4f screenpos[2];

	for(int vi=0; vi<2; ++vi)
		screenpos[vi] = ScreenPos(mvp, vecpos[vi], FixFrac(INTBASIS(g_width)), FixFrac(INTBASIS(g_height)), true);

	Vec2f fromscreen = Vec2f(screenpos[0].x,screenpos[0].y);
	Vec2f toscreen = Vec2f(screenpos[1].x,screenpos[1].y);
	FixFrac d = MAG_VEC2F(fromscreen-toscreen);

	for(FixFrac i=FixFrac(INTBASIS(0)); i<=d; i=i+FixFrac(INTBASIS(1)))
	{
		Vec3f subpos = (screenpos[0] + (screenpos[1]-screenpos[0]) * i / d);

		if(subpos.x < FixFrac(INTBASIS(0)) ||
			subpos.x >= FixFrac(INTBASIS(g_width)) ||
			subpos.y < FixFrac(INTBASIS(0)) ||
			subpos.y >= FixFrac(INTBASIS(g_height)))
			continue;

		int dpi = ToBasis(subpos.x+subpos.y*screencolors->sizex)*1;
		int cpi = ToBasis(subpos.x+subpos.y*screencolors->sizex)*screencolors->channels;

		if(screendepths[dpi] <= subpos.z)
			return;

		Vec2f subtexcoord;

		subtexcoord = (texcoords[0] * (FixFrac(INTBASIS(1))-i/d)) + 
			((texcoords[1]-texcoords[0]) * i/d);
		//x,y

		//subtexcoord.x = (subtexcoord.x % FixFrac(INTBASIS(1)));
		//subtexcoord.y = (subtexcoord.y % FixFrac(INTBASIS(1)));

		if(subtexcoord.x < FixFrac(INTBASIS(0)))
			subtexcoord.x = FixFrac(INTBASIS(1)) - subtexcoord.x;
		if(subtexcoord.y < FixFrac(INTBASIS(0)))
			subtexcoord.y = FixFrac(INTBASIS(1)) - subtexcoord.y;

		subtexcoord.x = subtexcoord.x * FixFrac(INTBASIS(tex->sizex));
		subtexcoord.y = subtexcoord.y * FixFrac(INTBASIS(tex->sizey));

		subtexcoord.x = ((int64_t)subtexcoord.x % tex->sizex);
		subtexcoord.y = ((int64_t)subtexcoord.y % tex->sizey);

		int ti = (ToBasis(subtexcoord.x)+ToBasis(subtexcoord.y)*tex->sizex)*tex->channels;

		Vec4f color;
		color.x = FixFrac((INTBASIS)tex->data[ti+0])/FixFrac(INTBASIS(255));
		color.y = FixFrac((INTBASIS)tex->data[ti+1])/FixFrac(INTBASIS(255));
		color.z = FixFrac((INTBASIS)tex->data[ti+2])/FixFrac(INTBASIS(255));
		//color.w = FixFrac((INTBASIS)tex->data[ti+3])/FixFrac(INTBASIS(255));

		screencolors->data[cpi+0] = ToBasis(color.x*FixFrac(INTBASIS(255)));
		screencolors->data[cpi+1] = ToBasis(color.y*FixFrac(INTBASIS(255)));
		screencolors->data[cpi+2] = ToBasis(color.z*FixFrac(INTBASIS(255)));
		screencolors->data[cpi+3] = 255;
	}
}

void RasterTri(Matrix* mvp, FixFrac* screendepths, LoadedTex* screencolors, Vec3f* vecpos, Vec2f* texcoords, LoadedTex* tex)
{
#if 0
	char m[12333];
	sprintf(m, "rt %lld/%lld,%lld/%lld,%lld/%lld,?",
		vecpos[0].x.n,vecpos[0].x.d,
		vecpos[0].y.n,vecpos[0].y.d,
		vecpos[0].z.n,vecpos[0].z.d);
	InfoMess(m,m);
#endif
	Vec4f screenpos[3];

	for(int vi=0; vi<3; ++vi)
		screenpos[vi] = ScreenPos(mvp, vecpos[vi], FixFrac(INTBASIS(g_width)), FixFrac(INTBASIS(g_height)), true);
	Vec2f vmin = Vec2f(screenpos[0].x,screenpos[0].y);
	Vec2f vmax = vmin;

	for(int vi=1; vi<3; ++vi)
	{
		if(screenpos[vi].x < vmin.x)
			vmin.x = screenpos[vi].x;
		if(screenpos[vi].y < vmin.y)
			vmin.y = screenpos[vi].y;
		if(screenpos[vi].x > vmax.x)
			vmax.x = screenpos[vi].x;
		if(screenpos[vi].y > vmax.y)
			vmax.y = screenpos[vi].y;
	}

	Vec3f norm = Normalize( Cross(vecpos[0]-vecpos[1],vecpos[0]-vecpos[2]) );
	//Vec3f normtexc = Normal(texcoords);
	Plane3f plane;
	MakePlane(&plane.m_normal, &plane.m_d, screenpos[0], norm);

	//Vec3f to1 = screenpos[1] - screenpos[0];
	//Vec3f to2 = screenpos[2] - screenpos[0];

	for(FixFrac x=vmin.x; x<=vmax.x; x+=FixFrac(INTBASIS(1)))
	{
		for(FixFrac y=vmin.y; y<=vmax.y; y+=FixFrac(INTBASIS(1)))
		{
			Vec3f bc_screen  = barycentric(screenpos, Vec2f(x,y));
			if (bc_screen.x<FixFrac(INTBASIS(0)) || bc_screen.y<FixFrac(INTBASIS(0)) || bc_screen.z<FixFrac(INTBASIS(0)))
				continue;
			Vec2f subtexcoord;
			Vec3f subpos;
			subpos = Vec3f(x,y,INTBASIS(0));

			Vec3f into[2];
			into[0] = Vec3f(INTBASIS(0),INTBASIS(0),INTBASIS(0)) + subpos;
			into[1] = Vec3f(INTBASIS(0),INTBASIS(0),INTBASIS(1)) + subpos;

			LineInterPlane(into, plane.m_normal, plane.m_d, &subpos);

			FixFrac to0 = MAG_VEC3F( Vec3f(screenpos[0]) - subpos );
			FixFrac to1 = MAG_VEC3F( Vec3f(screenpos[1]) - subpos );
			FixFrac to2 = MAG_VEC3F( Vec3f(screenpos[2]) - subpos );
			FixFrac all = to0 + to1 + to2;
			to0 = to0 / all;
			to1 = to1 / all;
			to2 = to2 / all;

			Vec3f subvecpos = (vecpos[0]*to0) + (vecpos[1]*to1) + (vecpos[2]*to2);

			subtexcoord = (texcoords[0] * to0) + 
				(texcoords[1] * to1) + 
				(texcoords[2] * to2);
			//x,y

			//subtexcoord.x = (subtexcoord.x % FixFrac(INTBASIS(1)));
			//subtexcoord.y = (subtexcoord.y % FixFrac(INTBASIS(1)));

			if(subtexcoord.x < FixFrac(INTBASIS(0)))
				subtexcoord.x = FixFrac(INTBASIS(1)) - subtexcoord.x;
			if(subtexcoord.y < FixFrac(INTBASIS(0)))
				subtexcoord.y = FixFrac(INTBASIS(1)) - subtexcoord.y;

			subtexcoord.x = subtexcoord.x * FixFrac(INTBASIS(tex->sizex));
			subtexcoord.y = subtexcoord.y * FixFrac(INTBASIS(tex->sizey));

			subtexcoord.x = ((int64_t)subtexcoord.x % tex->sizex);
			subtexcoord.y = ((int64_t)subtexcoord.y % tex->sizey);

			int ti = (ToBasis(subtexcoord.x)+ToBasis(subtexcoord.y)*tex->sizex)*tex->channels;

			Vec4f color;
			color.x = FixFrac((INTBASIS)tex->data[ti+0])/FixFrac(INTBASIS(255));
			color.y = FixFrac((INTBASIS)tex->data[ti+1])/FixFrac(INTBASIS(255));
			color.z = FixFrac((INTBASIS)tex->data[ti+2])/FixFrac(INTBASIS(255));
			//color.w = FixFrac((INTBASIS)tex->data[ti+3])/FixFrac(INTBASIS(255));

			

			RasterPt(mvp, screendepths, screencolors, subvecpos, color);
		}
	}
}


void RasterPts(Matrix* mvp, FixFrac* screendepths, LoadedTex* screencolors, Vec3f* vecpos, Vec2f* texcoord, LoadedTex* tex, int npt)
{
	for(int i=0; i<npt; ++i)
		RasterPt(mvp, screendepths, screencolors, vecpos[i], texcoord[i], tex);
}

void RasterLines(Matrix* mvp, FixFrac* screendepths, LoadedTex* screencolors, Vec3f* vecpos, Vec2f* texcoords, LoadedTex* tex, int npt)
{
	for(int i=0; i<npt; i+=2)
		RasterLine(mvp, screendepths, screencolors, &vecpos[i], &texcoords[i], tex);
}
void RasterLines(Matrix* mvp, FixFrac* screendepths, LoadedTex* screencolors, Vec3f* vecpos, Vec4f* colors, int npt)
{
	for(int i=0; i<npt; i+=2)
		RasterLine(mvp, screendepths, screencolors, &vecpos[i], &colors[i]);
}
void RasterLines(Matrix* mvp, FixFrac* screendepths, LoadedTex* screencolors, Vec3f* vecpos, Vec4f color, int npt)
{
	for(int i=0; i<npt; i+=2)
		RasterLine(mvp, screendepths, screencolors, &vecpos[i], color);
}

void RasterTris(Matrix* mvp, FixFrac* screendepths, LoadedTex* screencolors, Vec3f* vecpos, Vec2f* texcoords, LoadedTex* tex, int npt)
{
#if 0
	char m[12333];
	sprintf(m, "rt %lld/%lld,%lld/%lld,%lld/%lld,?",
		vecpos[0].x.n,vecpos[0].x.d,
		vecpos[0].y.n,vecpos[0].y.d,
		vecpos[0].z.n,vecpos[0].z.d);
	InfoMess(m,m);
#endif	
	for(int i=0; i<npt; i+=3)
		RasterTri(mvp, screendepths, screencolors, &vecpos[i], &texcoords[i], tex);
}

