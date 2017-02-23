

#include "simdef.h"
#include "simvars.h"
#include "simflow.h"
#include "../math/camera.h"
#include "../math/fixtrig.h"

Vec3f axis[HYP_DIMS];
const char* AXNAME[HYP_DIMS] = 
{
"x",
"y",
//"/",
//"tc",
"z",
//"/",
//"/",
//"Vd",
"t"
};


void AddGravPt(HypMeshf* hm)
{
	Vec3f off = Vec3f(0,0,0) - Vec3f(hm->cen.pos[0],hm->cen.pos[1],hm->cen.pos[2]);
	FixFrac d = MAG_VEC3F(off);

	off = (off)/(d*1000000000/KMSC) * (hm->mass/KMSC*SECGV) / (d/KMSC)/(d/KMSC);

	off = off;

	//Log("d%f", MAG_VEC3F(off));

	hm->pts.push_back(HypVecf());
	hm->pts.push_back(HypVecf());
	HypVecf* hv2 = &*hm->pts.rbegin();
	hv2->joins.push_back( &*hm->pts.begin() );

	//if(_isnan(off.x))
	//	off.x = 1;
	//if(_isnan(off.y))
	//	off.y = 1;
	//if(_isnan(off.z))
	//	off.z = 1;

	hv2->pos[0] = off.x;
	hv2->pos[1] = off.y;
	hv2->pos[2] = off.z;

	UpdRend(hm);

	//Log("rend %f,%f,%f", hv2->rend[selgg].x, hv2->rend[selgg].y, hv2->rend[selgg].z);
	
}//GVEM GVER KM G

void AddCloth(HypMeshf* hm)
{
	//for(int ax0=0; ax0<HYP_DIMS; ax0++)
	{
		//for(int ax1=1+ax0; ax1<HYP_DIMS; ax1++)
		{
			int ax0 = 0;
			int ax1 = 1;

			HypVecf* hvp[10][10];

			for(int x=0; x<10; x++)
			{
				for(int y=0; y<10; y++)
				{
					HypVecf hv;
					for(int ax=0; ax<HYP_DIMS; ++ax)
						hv.pos[ax] = 0;
					hv.pos[ax0] = x*400 - 2000;
					hv.pos[ax1] = y*400 - 2000;
					hm->pts.push_back(hv);
					hvp[x][y] = &*hm->pts.rbegin();
				}
			}

			for(int x=0; x<10-1; x++)
			{
				for(int y=0; y<10-1; y++)
				{
					hvp[x][y]->joins.push_back( hvp[x+1][y] );
					hvp[x][y]->joins.push_back( hvp[x][y+1] );
				}
			}
		}
	}
}

void AddCb(HypMeshf* hm, int i)
{
	for(int x=0; x<2; ++x)
	{
		for(int y=0; y<2; ++y)
		{
			for(int z=0; z<2; ++z)
			{
				for(int w=0; w<2; ++w)
				{
					HypVecf hv;
					hv.pos[0] = (x*2-1)*1000/2;
					hv.pos[1] = (y*2-1)*1000/2;
					hv.pos[2] = (z*2-1)*1000/2;
					hv.pos[3] = (w*2-1)*1000/2;
					hm->pts.push_back(hv);
				}
			}
		}
	}
	return;
	for(std::list<HypVecf>::iterator pit=hm->pts.begin(); pit!=hm->pts.end(); pit++)
	{
		std::list<HypVecf>::iterator pit2 = pit;
		for(pit2++; pit2!=hm->pts.end(); pit2++)
		{
			pit->joins.push_back(&*pit2);
		}
	}

	UpdRend(hm);
}

void AddSph(HypMeshf* hm, FixFrac i, FixFrac* cen, FixFrac mass)
{
	HypVecf hv;

	for(int ax=0; ax<HYP_DIMS; ax++)
		hm->cen.pos[ax] = cen[ax];

	//hm->mass = ToFrac(3.0 / 4.0 * M_PI) * i * i * i;
	hm->mass = mass;

	//for(int ax0=0; ax0<HYP_DIMS; ax0++)
	{
		//for(int ax1=1+ax0; ax1<HYP_DIMS; ax1++)
		{
			//int ax0 = ((i+3)/HYP_DIMS)%HYP_DIMS;
			//int ax1 = (1+i)%HYP_DIMS;
			//int ax0 = 0;
			//int ax1 = 1;

			//hv.pos[ax0] = 10*10;

			//hm->pts.push_back(hv);

			for(int x=0; x<30; x++)
			{
				for(int ax=0; ax<HYP_DIMS; ++ax)
					//hv.pos[ax] = rand()%10-5;
					hv.pos[ax] = 0;

				FixFrac xf = cos( ToFrac( M_PI * 2.0f * x / 30.0f ) ) * FixFrac(KMSC*i) ;
				HypVecf* prev = NULL;
//return;

				for(int z=0; z<30; z++)
				{
					FixFrac yf = sin( ToFrac( M_PI * 2.0f * x / 30.0f ) ) * FixFrac(KMSC*i);

					Vec3f vf(xf,yf,0);
					vf = RotateAround(vf, Vec3f(0,0,0), ToFrac(M_PI * 2.0f * z/30.f),
					1,0,0);

					hv.pos[0] = vf.x;
					hv.pos[1] = vf.y;
					hv.pos[2] = vf.z;

					//for(int ax=0; ax<HYP_DIMS; ++ax)
					//	hv.pos[ax] = rand()%1000-500;

					if(prev)
						hv.joins.push_back(prev);
					hm->pts.push_back(hv);
					prev = &*hm->pts.rbegin();
					hv.joins.clear();
				}
			}
		}
	}

//return;
	UpdRend(hm);
}

void Balance()
{
	//for(int tries=0; tries<200000; ++tries)
	//for(int tries=0; tries<300000; ++tries)
	//for(int tries=0; tries<10000; ++tries)
	{
		for(int ax=0; ax<HYP_DIMS; ++ax)
		{
			for(int ax2=0; ax2<HYP_DIMS; ++ax2)
			{
				if(ax2==ax)
					continue;
				Vec3f d = axis[ax2] - axis[ax];
				FixFrac d2 = ((MAG_VEC3F(d) * 100.0f) + 1.0f);

				//if(d2 < 0.001f || _isnan(d2))
				//	d2 = 0.001f;

				//FixFrac d2 = MAG_VEC3F(d);
				//axis[ax] = axis[ax] - d * d2 / HYP_DIMS;
				//axis[ax] = axis[ax] - d / d2 * HYP_DIMS / 100.0f;
				axis[ax] = axis[ax] - d / d2 * 4 / (100.0f * (HYP_DIMS-2.5f));
				axis[ax] = Normalize(axis[ax]);
			}
		}
	}	
}

void Balance2()
{
	//for(int tries=0; tries<200000; ++tries)
	//for(int tries=0; tries<300000; ++tries)
	//for(int tries=0; tries<10000; ++tries)
	{
		for(int ax=0; ax<HYP_DIMS; ++ax)
		{
			for(int ax2=0; ax2<HYP_DIMS; ++ax2)
			{
				if(ax2==ax)
					continue;
				Vec3f d = axis[ax2] - axis[ax];
				FixFrac d2 = ((MAG_VEC3F(d) * 100.0f) + 1.0f);

				//if(d2 < 0.001f || _isnan(d2))
				//	d2 = 0.001f;

				//FixFrac d2 = MAG_VEC3F(d);
				//axis[ax] = axis[ax] - d * d2 / HYP_DIMS;
				//axis[ax] = axis[ax] - d / d2 * HYP_DIMS / 100.0f;
				axis[ax] = axis[ax] - d / d2 * 4 / (100.0f * (HYP_DIMS-2.5f));
				axis[ax] = Normalize(axis[ax]);
			}

			Vec3f z = Vec3f(0,0,1);

			FixFrac zd = Dot(z, axis[ax]);
			
			Vec3f dz = z - axis[ax];

			FixFrac zd2 = (((FixFrac(0)-(zd*0.8f+0.2f)) * 100.0f) + 1.0f);

			axis[ax] = axis[ax] + dz / zd2 * 4 / (100.0f * (HYP_DIMS-2.5f));
			axis[ax] = Normalize(axis[ax]);
		}
	}	
}

void MakeGrid(FixFrac* cen)
{
#if 1
	std::vector<int> gc;
	gc.resize(HYP_DIMS);

	for(int ci=0; ci<HYP_DIMS; ++ci)
	{
		gc[ci]=0;
	}

	for(int gi=0; gi<GRIDIS; ++gi)
	{
		HypVecf hv2;

		for(int ci=0; ci<HYP_DIMS; ++ci)
		{
			//hv2.pos[ci] = FixFrac(30000)/GRID*(gc[ci])-FixFrac(15000);
			gg[gi].cen.pos[ci] = FixFrac(30000)/GRID*(gc[ci])-FixFrac(15000) + cen[ci];
		}
		gg[gi].pts.clear();
		//gg[gi].pts.push_back(hv2);
		AddGravPt(&gg[gi]);

		//++
		for(int ci=0; ci<HYP_DIMS; ++ci)
		{
			gc[ci]++;

			if(gc[ci] < GRID)
				break;

			gc[ci] = 0;
		}
	}

#if 1
	for(int ci=0; ci<HYP_DIMS; ++ci)
	{
		gc[ci]=0;
	}

	for(int gi=0; gi<GRIDIS; ++gi)
	{
		HypVecf* hv2 = &*gg[gi].pts.begin();
		std::vector<HypVecf*> hv3;

		hv3.resize(HYP_DIMS);

		//gg[gi].cen.pos[0] = GVX;
		//gg[gi].cen.pos[1] = GVY;
		//gg[gi].cen.pos[2] = GVZ;
		//gg[gi].cen.pos[3] = GVW;
#if 0
#define LS		1.0f
		FixFrac cv[HYP_DIMS];
		for(int ax=0; ax<HYP_DIMS; ++ax)
			cv[ax] = gg[gi].cen.pos[ax] + hv2->pos[ax];
		FixFrac d = HypDistf(cv, gv.pts.begin()->pos);
		hv2->pos[HYP_DIMS-1] = sqrtf( 1.0f - 7000.0f / ( d * LS ) ) * 1000000.0f;
		Log("hv2->pos[HYP_DIMS-1]=%f   7000.0f / ( d * LS )=%f", hv2->pos[HYP_DIMS-1], 7000.0f / ( d * LS ));
#endif
		for(int ji=0; ji<HYP_DIMS; ji++)
		{
			int gi2=0;

			for(int ci=HYP_DIMS-1; ci>=0; --ci)
			{
				if(ji == ci)
					gi2 = gi2 * HYP_DIMS + (gc[ci]+1);
				else
					gi2 = gi2 * HYP_DIMS + (gc[ci]);
			}

			hv3[ji] = &*gg[gi2].pts.begin();
			//hv2->joins.push_back(hv3[ji]);
	
		}


		UpdRend(&gg[gi]);

		//++
		for(int ci=0; ci<HYP_DIMS; ++ci)
		{
			gc[ci]++;

			if(gc[ci] < GRID)
				break;

			gc[ci] = 0;
		}
	}
#endif
#endif
#if 0
	for(int x=0; x<GRID; ++x)
	{
		for(int y=0; y<GRID; ++y)
		{
			for(int z=0; z<GRID; ++z)
			{
				for(int w=0; w<GRID; ++w)
				{
					HypVecf hv2;
					hv2.pos[0] = FixFrac(30000)/GRID*(x)-FixFrac(15000);
					hv2.pos[1] = FixFrac(30000)/GRID*(y)-FixFrac(15000);
					hv2.pos[2] = FixFrac(30000)/GRID*(z)-FixFrac(15000);
					hv2.pos[3] = FixFrac(30000)/GRID*(w)-FixFrac(15000);
					gg[x][y][z][w].pts.push_back(hv2);
				}
			}
		}
	}

	for(int x=0; x<GRID-1; ++x)
	{
		for(int y=0; y<GRID-1; ++y)
		{
			for(int z=0; z<GRID-1; ++z)
			{
				for(int w=0; w<GRID-1; ++w)
				{
					HypVecf* hv4 = &*gg[x][y][z][w].pts.rbegin();
					HypVecf* hv0 = &*gg[x+1][y][z][w].pts.rbegin();
					HypVecf* hv1 = &*gg[x][y+1][z][w].pts.rbegin();
					HypVecf* hv2 = &*gg[x][y][z+1][w].pts.rbegin();
					HypVecf* hv3 = &*gg[x][y][z][w+1].pts.rbegin();
					hv4->joins.push_back(hv0);
					hv4->joins.push_back(hv1);
					hv4->joins.push_back(hv2);
					hv4->joins.push_back(hv3);
					//UpdRend(&gg[x][y][z][w]);
				}
			}
		}
	}
#endif
}

void RandMesh()
{

	for(int gi=0; gi<MESHES; ++gi)
	{
		//for(int ax=0; ax<HYP_DIMS-1; ++ax)
		for(int ax=0; ax<HYP_DIMS; ++ax)
		{
			//g[gi].cen.pos[ax] = (rand()%4001 - 2000)/(1+ax);
			g[gi].cen.pos[ax] = (rand()%4001 - 2000)/(1);
			//g[gi].cen.pos[ax] = (rand()%4001)/(1+ax);
			g[gi].cen.pos[ax] = 0;
			g[gi].vel[ax] = (rand()%501 - 250)/100.0f;
			g[gi].vel[ax] = 0;
		}
		g[gi].cen.pos[0] = 940 * 1000 * 1000;
		g[gi].cen.pos[1] = 0;
		g[gi].vel[1] = 7 / 30.0f;
		g[gi].cen.pos[HYP_DIMS-1] = 0;
		g[gi].cen.pos[HYP_DIMS-2] = 0;
		g[gi].vel[HYP_DIMS-1] = 10.0f;
		g[gi].vel[HYP_DIMS-2] = 0.0f;
		//g[gi].pts.push_back(HypVecf());
		//UpdRend(&g[gi]);
	}

	//g[1].pts.push_back(HypVecf());

	//g[1].pts.begin()->joins.push_back( &*g[0].pts.begin() );

	for(int gi=0; gi<MESHES; ++gi)
	{
		//AddCb(&g[gi], gi);
		//AddSph(&g[gi], gi);
		//g[gi].pts.push_back(HypVecf());
		//AddCloth(&g[gi]);
		//UpdRend(&g[gi]);
	}

	//UpdRend(&g[0]);
	//UpdRend(&g[1]);
}

void DefSim()
{
//return;
	g_pcam->position(
	0,0,0,
	0,(FixFrac(0)-1000)*KMSC,(FixFrac(0)-345)*KMSC,
	0,1,0	
	);

#if 0
	axis[0] = Normalize( Vec3f(1,-1,0) );
	axis[1] = Normalize( Vec3f(FixFrac(0)-1,-1,0) );
	axis[2] = Normalize( Vec3f(0,1,1) );
	axis[3] = Normalize( Vec3f(0,1,-1) );
#elif 0
	axis[0] = Normalize( Vec3f(1,0,1) );
	axis[1] = Normalize( Vec3f(FixFrac(0)-1,0,0) );
	axis[2] = Normalize( Vec3f(0,0,1) );
	axis[3] = Normalize( Vec3f(0,1,0) );
#elif 0
	axis[0] = Normalize( Vec3f(1,0,1) );
	axis[1] = Normalize( Vec3f(0,1,0) );
	axis[2] = Normalize( Vec3f(0,-1,1) );
	axis[3] = Normalize( Vec3f(0,-1,-1) );
#elif 0
	axis[0] = Normalize( Vec3f(FixFrac(0)-1,1,0) );
	axis[1] = Normalize( Vec3f(FixFrac(0)-1,-1,0) );
	axis[2] = Normalize( Vec3f(1,0,1) );
	axis[3] = Normalize( Vec3f(1,0,-1) );
#elif 0
	axis[0] = Normalize( Vec3f(0,-1,-1) );
	axis[1] = Normalize( Vec3f(0,1,-1) );
	axis[2] = Normalize( Vec3f(1,0,1) );
	axis[3] = Normalize( Vec3f(FixFrac(0)-1,0,1) );
#elif 0
	axis[0] = Normalize( Vec3f(0,1,0) );
	axis[1] = Normalize( Vec3f(1,-1,0) );
	axis[2] = Normalize( Vec3f(FixFrac(0)-1,-1,0) );
	axis[3] = Normalize( Vec3f(0,-1,1) );
	axis[4] = Normalize( Vec3f(0,-1,-1) );
#elif 0

	axis[0] = Normalize( Vec3f(1,0,0) );
	axis[1] = Normalize( Vec3f(0,1,0) );
	axis[2] = Normalize( Vec3f(0,0,1) );
#else

#if 1
	for(int ax=0; ax<HYP_DIMS; ++ax)
		axis[ax] = Normalize(Vec3f((rand()%501-250)/250.0f, 
			(rand()%501-250)/250.0f, 
			(rand()%501-250)/250.0f));
#endif

#if 0
	for(int ax=0; ax<HYP_DIMS; ++ax)
		axis[ax] = Normalize(Vec3f((rand()%501)/25000.0f, 
			(rand()%501)/25000.0f, 
			(rand()%501)/25000.0f));
#endif
	for(int tries=0; tries<300; ++tries)
		Balance();
#endif

	/*

class HypVecf
{
	Vec3f rend[REND_3PT_INS];
	FixFrac pos[HYP_DIMS];

	std::list<HypVecf*> joins;
	std::list<HypVecf*> temp;
	std::list<HypVecf*> hist;
};

class HypMeshf
{
	HypVec3f cen;
	std::list<HypVecf> pts;
};
*/
#if 0
	g[0].cen.pos[0] = -100;
	g[0].cen.pos[1] = -101.67;
	g[0].cen.pos[2] = -72;
	g[0].cen.pos[3] = 123;

	g[0].vel[0] = 1;
	g[0].vel[1] = 0;
	g[0].vel[2] = 1.2;
	g[0].vel[3] = 1;

	//g[0].pts.push_back(HypVecf());

	g[1].cen.pos[0] = 34;
	g[1].cen.pos[1] = 134;
	g[1].cen.pos[2] = -100;
	g[1].cen.pos[3] = -10;

	g[1].vel[0] = -0.7;
	g[1].vel[1] = -1.111;
	g[1].vel[2] = -0.14;
	g[1].vel[3] = 1;
#endif

	//RandMesh();
#if 1
	for(int ax=0; ax<HYP_DIMS; ax++)
	{
		axg[ax].pts.push_back(HypVecf());
		for(int x=0; x<5; x++)
		{
			HypVecf hv;
			hv.joins.push_back( &* axg[ax].pts.rbegin() );
			for(int ax2=0; ax2<HYP_DIMS; ax2++)
				hv.pos[ax2] = 2 * ax;
			hv.pos[ax] = x * 200 + 2 * ax;
			axg[ax].pts.push_back(hv);
		}
		UpdRend(&axg[ax]);
	}
	for(int ax=HYP_DIMS; ax<HYP_DIMS*2; ax++)
	{
		axg[ax].pts.push_back(HypVecf());
		for(int x=0; x<5; x++)
		{
			HypVecf hv;
			hv.joins.push_back( &* axg[ax].pts.rbegin() );
			for(int ax2=0; ax2<HYP_DIMS; ax2++)
				hv.pos[ax2] = 2 * ax;
			hv.pos[ax/2] = - x * 200 + 2 * ax;
			axg[ax].pts.push_back(hv);
		}
		//UpdRend(&axg[ax]);
	}
#endif
	gv.pts.push_back(HypVecf());
	//gv.pts.rbegin()->pos[0] = GVX;
	//gv.pts.rbegin()->pos[1] = GVY;
	//gv.pts.rbegin()->pos[2] = GVZ;
	//gv.pts.rbegin()->pos[3] = 0;
	gv.pts.rbegin()->pos[2] = 0;

	//gve.pts.push_back(HypVecf());

	FixFrac ecen[4] = {ESR*KMSC,0,0};
	FixFrac scen[4] = {0,0,0,0};
	//AddSph(&gve, GVER, ecen);
	//AddSph(&gvs, GVSR, scen);
	Log("adsp");
	AddSph(&g[MESH_E], GVER*KMSC, ecen, GVEM);
//return;
	AddSph(&g[MESH_S], GVSR*KMSC, scen, GVSM);
	g[MESH_E].vel[1] = ESV*KMSC;

	FixFrac zcen[4] = {0,0,0,0};
	gc.craftpos.pos[0] = 10;
	gc.craftpos.pos[1] = 10;
	gc.craftpos.pos[2] = 1;
	gc.craftpos.pos[3] = 0;
	gc.craftvel.pos[0] = 0;
	gc.craftvel.pos[1] = 1;
	gc.craftvel.pos[2] = 0;
	gc.craftvel.pos[3] = 0;
	gc.beadpos.pos[0] = 11;
	gc.beadpos.pos[1] = 12;
	gc.beadpos.pos[2] = 1;
	gc.beadpos.pos[3] = 0;
	gc.beadvel.pos[0] = 1;
	gc.beadvel.pos[1] = 1;
	gc.beadvel.pos[2] = 1;
	gc.beadvel.pos[3] = 0;
	gc.beadpos.joins.push_back(&gc.craftpos);
	CvPt(zcen, &gc.beadpos);
	CvPt(zcen, &gc.craftpos);

	FixFrac gcen[4] = {0,0,0,0};
	MakeGrid(gcen);
}

