

#include "simflow.h"
#include "simdef.h"
#include "simvars.h"
#include "../render/hypmesh.h"
#include "../window.h"
#include "../utils.h"

void Hist()
{
#if 000
	return;
	static int cn = 0;
	cn++; 
	if(cn%100==0)
	for(int gi=0; gi<MESHES; ++gi)
	{
		HypMeshf* hm = &g[gi];

		for(std::list<HypVecf>::iterator pit=hm->pts.begin(); pit!=hm->pts.end(); )
		{
			hh.push_back(*pit);
			pit->hist.clear();
			pit->hist.push_back( &*hh.rbegin() );
			pit->age = 1;
			//pit = hm->pts.erase(pit);
			++pit;
		}

		UpdRend(hm);
		break;
	}
#endif
}

void UpdCraft()
{
	HypVecf z;
	for(int ax=0; ax<HYP_DIMS; ++ax)
		z.pos[ax] = 0;

	FixFrac offc = HypDistf(z.pos, gc.craftpos.pos);	
	FixFrac offb = HypDistf(z.pos, gc.beadpos.pos);	

	HypVecf dvc;
	HypVecf dvb;

	for(int ax=0; ax<HYP_DIMS-RESERVEDIMS; ++ax)
	{
		dvc.pos[ax] = (z.pos[ax] - gc.craftpos.pos[ax]) / 100.0f / offc;
		dvb.pos[ax] = (z.pos[ax] - gc.beadpos.pos[ax]) / 100.0f / offb;
	}

	for(int ax=0; ax<HYP_DIMS-RESERVEDIMS; ++ax)
	{
		gc.craftthrust.pos[ax] = 0;
		//gc.beadthrust.pos[ax] = 0;
	}

	if(rand()%100<=10)
	{
		for(int ax=0; ax<HYP_DIMS-RESERVEDIMS; ++ax)
		{
			gc.craftthrust.pos[ax] = (rand()%501-250)/250.0f;
			//gc.beadthrust.pos[ax] = 0;
		}
	}

	for(int ax=0; ax<HYP_DIMS-RESERVEDIMS; ++ax)
	{
		gc.craftvel.pos[ax] += dvc.pos[ax];
		gc.beadvel.pos[ax] += dvb.pos[ax];

		gc.craftvel.pos[ax] += gc.craftthrust.pos[ax];
		//gc.beadvel.pos[ax] += gc.beadthrust.pos[ax];
	}
	
	for(int ax=0; ax<HYP_DIMS-RESERVEDIMS; ++ax)
	{
		gc.craftpos.pos[ax] += gc.craftvel.pos[ax];
		gc.beadpos.pos[ax] += gc.beadvel.pos[ax];
	}
	
	FixFrac offs = HypDistf(gc.craftpos.pos, gc.beadpos.pos);

	FixFrac offss[HYP_DIMS];

	if(offs >= 1)
	{	
		FixFrac d2 = 0;

		for(int ax=0; ax<HYP_DIMS-RESERVEDIMS; ++ax)
		{
			offss[ax] = gc.craftpos.pos[ax] - gc.beadpos.pos[ax];
			d2 += offss[ax]*offss[ax];
		}
		
		d2 = sqrtf(d2);

		for(int ax=0; ax<HYP_DIMS-RESERVEDIMS; ++ax)
		{
			offss[ax] /= d2;
		}

		for(int ax=0; ax<HYP_DIMS-RESERVEDIMS; ++ax)
		{
			gc.beadpos.pos[ax] += offss[ax] * (offs - 1.0);
		}
	}

	//CvPt(z.pos, &dvc);
	//CvPt(z.pos, &dvb);

#if 0
	Log("b%f,%f,%f c%f,%f,%f dvc%f,%f,%f dvb%f,%f,%f", 
	gc.beadpos.pos[0],gc.beadpos.pos[1],gc.beadpos.pos[2],gc.beadpos.pos[3],
	gc.craftpos.pos[0],gc.craftpos.pos[1],gc.craftpos.pos[2],gc.craftpos.pos[3],
	dvc.pos[0],dvc.pos[1],dvc.pos[2],
	dvb.pos[0],dvb.pos[1],dvb.pos[2]);
#endif
	//gc.craftpos = dvc;
	//gc.beadpos = dvb;

	CvPt(z.pos, &gc.craftpos);
	CvPt(z.pos, &gc.beadpos);
}

void UpdSim()
{//return;

	//UpdCraft();

	HypVecf ch;

	for(int ax=0; ax<HYP_DIMS; ax++)
		ch.pos[ax] = (rand()%2000-1000);

	FixFrac cen[4] = {0,0,0,0};
	CvPt(cen, &ch);
	
	static Vec3f r = Normalize(Vec3f( (rand()%1000-500)/1000.0f, (rand()%1000-500)/1000.0f, (rand()%1000-500)/1000.0f ));
	//g_pcam->rotateabout(Vec3f(0,0,0),
		//(rand()%1000-500)/5000.0f*2.0f/200.0*M_PI, r.x,r.y,r.z);
	//	1.0f*2.0f/2000.0*M_PI, r.x,r.y,r.z);
	//r.x *= 1.1f;
	//r.y *= sqrtf(r.x+1);
	//r.z -= sqrtf(r.x/20.0f);
	//r = Normalize(r);
	//r.z += sqrtf(r.x);
	//r = Normalize(r);

#if 0
	static float sw = 0;

	sw = fmin(1, sw + g_drawfrinterval + 0.001);


	axis[0] = axis[0] * (1.0f - sw) + Normalize( Vec3f(-1,1,0) ) * sw;
	axis[1] = axis[1] * (1.0f - sw) + Normalize( Vec3f(-1,-1,0) ) * sw;
	axis[2] = axis[2] * (1.0f - sw) + Normalize( Vec3f(1,0,1) ) * sw;
	axis[3] = axis[3] * (1.0f - sw) + Normalize( Vec3f(1,0,-1) ) * sw;
#endif

	//Log("asdasd");
	for(int ax=0; ax<HYP_DIMS; ax++)
	{
	//	UpdRend(&axg[ax]);
	}

	//Balance();

#if 000

	for(int gi=0; gi<MESHES; ++gi)
	{
		HypMeshf* hm = &g[gi];

		for(int gi2=0; gi2<MESHES; ++gi2)
		{
			if(gi2==gi)
				continue;
	
			HypMeshf* hm2 = &g[gi2];

			FixFrac d = 0;

#if 1
			for(int ax=0; ax<HYP_DIMS-RESERVEDIMS; ++ax)
			//for(int ax=0; ax<HYP_DIMS; ++ax)
				d= d+ (hm2->cen.pos[ax]-hm->cen.pos[ax])*
					(hm2->cen.pos[ax]-hm->cen.pos[ax]);
			///	d += (hm2->cen.pos[ax] - hm->cen.pos[ax])*
			//		(hm2->cen.pos[ax] - hm->cen.pos[ax]);
			d = sqrtf(d);
			//if(d <= 0.01f || _isnan(d))
			//	d = 0.01f;

			//d *= 1000;
			
			for(int ax=0; ax<HYP_DIMS-RESERVEDIMS; ++ax)
			{
			//for(int ax=0; ax<HYP_DIMS; ++ax)
#if 0
				float dv = 
					(hm2->cen.pos[ax] - hm->cen.pos[ax])/
					fabs(hm2->cen.pos[ax] - hm->cen.pos[ax])*
					300.0f * pow(HYP_DIMS, HYP_DIMS) / pow(3.0f, 3.0f) / 3000.0f /d;
#elif 0
				FixFrac dv = 
					(0 - hm->cen.pos[ax])/d * SECGV /(d/KMSC)/(d/KMSC);
#else

//#define M1			10.0
//#define M2			(5 * 1000000000000000000000.0)

#define M1		hm->mass
#define M2		hm2->mass

				if(d <= 0.0 || _isnan(d))
					d = 1;

				
				//Log("d%f", (float)d);

				FixFrac dv = 
					//(FixFrac)fsign(hm2->cen.pos[ax] - hm->cen.pos[ax]) *
					//(FixFrac)(M2) /
					// (d*d);
					(0 - hm->cen.pos[ax])/d * M2*SECGV * 6.674 * pow((double)10,(double)-11) /(d/KMSC)/(d/KMSC);
				//dv = 0.01;

				if(_isnan(dv))
					dv = 0;
#endif

				//Log("dv%f", (float)dv);

				///if(_isnan((float)dv))
				///	continue;

				hm->vel[ax] = hm->vel[ax] + dv;


				if(_isnan(hm->vel[ax]))
					hm->vel[ax]=0;
			}
#endif
		}		
	}

	for(int gi=0; gi<MESHES; ++gi)
	{
		HypMeshf* hm = &g[gi];

		//for(int ax=0; ax<HYP_DIMS; ++ax)
		for(int ax=0; ax<HYP_DIMS-RESERVEDIMS; ++ax)
		{
			hm->cen.pos[ax] = hm->cen.pos[ax] + hm->vel[ax];
		}
		if(gi==selg)
		{
		//	g_pcam->move(Vec3f(hm->vel[0],hm->vel[1],hm->vel[2]));
			FixFrac cen[4];
			cen[0] = g_pcam->m_view.x;
			cen[1] = g_pcam->m_view.y;
			cen[2] = g_pcam->m_view.z;
			cen[3] = 0;
			//if(rand()%1000==0)
			//	MakeGrid(cen);
		}

		UpdRend(hm);
	}

	Hist();
#endif	//000
}
