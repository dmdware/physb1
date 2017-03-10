

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
"z"//,
//"/",
//"/",
//"Vd",
//"t"
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

				FixFrac xf = cos( ToFrac( (double)(M_PI * 2.0f * (float)x / 30.0f) ) ) * ToFrac(KMSC*i) ;
				HypVecf* prev = NULL;
//return;

				for(int z=0; z<30; z++)
				{
					FixFrac yf = sin( ToFrac( (M_PI * 2.0f * x / 30.0f) ) ) * ToFrac(KMSC*i);

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
}

void RandMesh()
{
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
#elif 01

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
}

