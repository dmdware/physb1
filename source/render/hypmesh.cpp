

#include "hypmesh.h"
#include "../platform.h"
#include "shader.h"
#include "../math/plane3f.h"
#include "../utils.h"
#include "../math/3dmath.h"
#include "../math/polygon.h"
#include "../sim/simvars.h"
#include "../sim/simdef.h"
#include "../window.h"
#include "../app/appmain.h"
#include "../raster/raster.h"
#include "../math/vec3f.h"

Craft gc;

void DrawHypVecs(std::list<HypVecf>* hl, float sz)
{
	Shader* s = &g_shader[g_curS];
	Vec3f v[REND_3PT_INS];

#ifdef PLATFORM_GL14
	glVertexPointer(3, GLTYPE, 0, v);
#endif
#ifdef PLATFORM_GLES20
	glVertexAttribPointer(s->slot[SSLOT_POSITION], 3, GLTYPE, GL_FALSE, sizeof(FixFrac)*0, &v[0]);
#endif

	glPointSize(sz);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glDisable(GL_CULL_FACE);
		//glDisable(GL_DEPTH_TEST);

	for(std::list<HypVecf>::iterator pit=hl->begin(); pit!=hl->end(); pit++)
	{
		for(int rpi=0; rpi<REND_3PT_INS; rpi++)
		//for(int rpi=0; rpi<1; rpi++)
		{
			FixFrac a = FixFrac(1.0f) * pit->age - MAG_VEC3F( g_pcam->m_pos - pit->rend[rpi])/MAX_DISTANCE;

#if 1
			glUniform4f(s->slot[SSLOT_COLOR], 
			ToBasis2(REND3PTINSCOLORS[rpi].rgba[0]), 
			ToBasis2(REND3PTINSCOLORS[rpi].rgba[1]), 
			ToBasis2(REND3PTINSCOLORS[rpi].rgba[2]), 
			ToBasis2(REND3PTINSCOLORS[rpi].rgba[3]/ToFrac(3.0) *  a));
#endif
			v[0] = pit->rend[rpi];

			glDrawArrays(GL_POINTS, 0, 1);
		}
		for(std::list<HypVecf*>::iterator cit=pit->hist.begin(); cit!=pit->hist.end(); ++cit)
		{
			HypVecf* cp = *cit;

			for(int rpi=0; rpi<REND_3PT_INS; rpi++)
			{
				FixFrac a = FixFrac(1.0f) * cp->age - MAG_VEC3F( g_pcam->m_pos - pit->rend[rpi])/MAX_DISTANCE;
#if 1
				glUniform4f(s->slot[SSLOT_COLOR], 
				ToBasis2(REND3PTINSCOLORS[rpi].rgba[0]), 
				ToBasis2(REND3PTINSCOLORS[rpi].rgba[1]), 
				ToBasis2(REND3PTINSCOLORS[rpi].rgba[2]), 
				ToBasis2(REND3PTINSCOLORS[rpi].rgba[3]/2.0f * a));
#endif
				v[0] = pit->rend[rpi];
				v[1] = cp->rend[rpi];
			//////	//glDrawArrays(GL_LINE_STRIP, 0, 2);
				////glDrawArrays(GL_POINTS, 0, 1);			}
			}
		}
		for(std::list<HypVecf*>::iterator cit=pit->joins.begin(); cit!=pit->joins.end(); ++cit)
		{
			HypVecf* cp = *cit;

			for(int rpi=0; rpi<REND_3PT_INS; rpi++)
			{
				FixFrac a = FixFrac(1.0f) * cp->age - MAG_VEC3F( g_pcam->m_pos - pit->rend[rpi])/MAX_DISTANCE;
#if 1
				glUniform4f(s->slot[SSLOT_COLOR], 
				ToBasis2(REND3PTINSCOLORS[rpi].rgba[0]), 
				ToBasis2(REND3PTINSCOLORS[rpi].rgba[1]), 
				ToBasis2(REND3PTINSCOLORS[rpi].rgba[2]), 
				ToBasis2(REND3PTINSCOLORS[rpi].rgba[3]/2.0f * a));
#endif
				v[0] = pit->rend[rpi];
				v[1] = cp->rend[rpi];
			glDrawArrays(GL_LINE_STRIP, 0, 2);
				////glDrawArrays(GL_POINTS, 0, 1);			}
			}
		}
#if  0
		FixFrac a = ToFrac(1.0) -  MAG_VEC3F( g_pcam->m_pos - pit->rend[0])/MAX_DISTANCE;

		for(int rpi=0; rpi<REND_3PT_INS; ++rpi)
		{
			for(int rpi2=rpi+1; rpi2<REND_3PT_INS; ++rpi2)
			{
				//int rpi2 = (rpi+1)%REND_3PT_INS;

#if 1
				glUniform4f(s->slot[SSLOT_COLOR], 
				(REND3PTINSCOLORS[rpi].rgba[0] + REND3PTINSCOLORS[rpi2].rgba[0])/2.0f, 
				(REND3PTINSCOLORS[rpi].rgba[1] + REND3PTINSCOLORS[rpi2].rgba[1])/2.0f, 
				(REND3PTINSCOLORS[rpi].rgba[2] + REND3PTINSCOLORS[rpi2].rgba[2])/2.0f, 
				(REND3PTINSCOLORS[rpi].rgba[3] + REND3PTINSCOLORS[rpi2].rgba[3])/2.0f/3.0f * a);
#else
				glUniform4f(s->slot[SSLOT_COLOR], 
				1.0f, 
				1.0f, 
				1.0f, 
				0.3f);
#endif

				//for(int rpi=0; rpi<REND_3PT_INS; rpi++)
				v[0] = pit->rend[rpi];
				v[1] = pit->rend[rpi2];

				////glDrawArrays(GL_TRIANGLES, 0, 3);
				//glDrawArrays(GL_LINE_LOOP, 0, 2);
			}
		}
#endif
	}

	glPointSize(1.0f);
}

FixFrac HypDistf(FixFrac* a, FixFrac* b)
{
	FixFrac d = INTBASIS(0);

	for(int ax=0; ax<HYP_DIMS-RESERVEDIMS; ax++)
		d = d + (a[ax]-b[ax])*(a[ax]-b[ax]);

	return sqrtf(d);
}

void DrawHypMesh(HypMeshf* hm)
{
	//UpdRend(hm);
	Shader* s = &g_shader[g_curS];
	Vec3f v[REND_3PT_INS];

#ifdef PLATFORM_GL14
	glVertexPointer(3, GLTYPE, 0, v);
#endif
#ifdef PLATFORM_GLES20
	glVertexAttribPointer(s->slot[SSLOT_POSITION], 3, GLTYPE, GL_FALSE, sizeof(FixFrac)*0, &v[0]);
#endif

#if 0
	for(int x=0; x<100; x++)
	{
		glUniform4f(s->slot[SSLOT_COLOR], 
		0.4f, 
		0.4f, 
		0.4f, 
		0.4f);

		for(int rpi=0; rpi<REND_3PT_INS; rpi++)
			v[rpi] = Vec3f(rand()%500-200, rand()%500-200, rand()%500-200);

	#ifdef PLATFORM_GL14
		glVertexPointer(2, GLTYPE, 0, v);
	#endif
	#ifdef PLATFORM_GLES20
		glVertexAttribPointer(s->slot[SSLOT_POSITION], 2, GLTYPE, GL_FALSE, sizeof(FixFrac)*0, &v[0]);
	#endif
		//glDrawArrays(GL_LINE_STRIP, 0, 3);
	}
#endif

	glPointSize(7.0f);

	HypVecf* prevp = NULL;

	for(std::list<HypVecf>::iterator pit=hm->pts.begin(); pit!=hm->pts.end(); ++pit)
	{

		int rpi = selgg;
		//for(int rpi=0; rpi<REND_3PT_INS; rpi++)
		//for(int rpi=0; rpi<1; rpi++)
		{
			FixFrac a = ToFrac(1.0) -  MAG_VEC3F( g_pcam->m_pos - pit->rend[rpi])/MAX_DISTANCE;
#if 1
			glUniform4f(s->slot[SSLOT_COLOR], 
			ToBasis2(REND3PTINSCOLORS[rpi].rgba[0]), 
			ToBasis2(REND3PTINSCOLORS[rpi].rgba[1]), 
			ToBasis2(REND3PTINSCOLORS[rpi].rgba[2]), 
			ToBasis2(REND3PTINSCOLORS[rpi].rgba[3]/ToFrac(1.0) *  a));
#endif

			v[0] = pit->rend[rpi];

			glDrawArrays(GL_POINTS, 0, 1);
		}

#if 0
		if(prevp)
		{
			for(int rpi=0; rpi<REND_3PT_INS; rpi++)
			{
				FixFrac a = ToFrac(1.0) -  MAG_VEC3F( g_pcam->m_pos - pit->rend[rpi])/MAX_DISTANCE;

				glUniform4f(s->slot[SSLOT_COLOR], 
				REND3PTINSCOLORS[rpi].rgba[0], 
				REND3PTINSCOLORS[rpi].rgba[1], 
				REND3PTINSCOLORS[rpi].rgba[2], 
				REND3PTINSCOLORS[rpi].rgba[3]/ToFrac(1.0) *  a);

				v[0] = pit->rend[rpi];
				v[1] = prevp->rend[rpi];

				//glDrawArrays(GL_LINES, 0, 2);
			}
		}

		prevp = &*pit;
#endif

#if 1
		glLineWidth(2);
		for(std::list<HypVecf*>::iterator cit=pit->joins.begin(); cit!=pit->joins.end(); ++cit)
		{
			HypVecf* cp = *cit;
			//cp->age /= 1.004f;

			int rpi = selgg;
			//for(int rpi=0; rpi<REND_3PT_INS; rpi++)
			{
				//FixFrac a = ToFrac(1.0) *  cp->age - MAG_VEC3F( g_pcam->m_pos - pit->rend[rpi])/MAX_DISTANCE;
#if 1
				glUniform4f(s->slot[SSLOT_COLOR], 
				REND3PTINSCOLORS[rpi].rgba[0], 
				REND3PTINSCOLORS[rpi].rgba[1], 
				REND3PTINSCOLORS[rpi].rgba[2], 
				REND3PTINSCOLORS[rpi].rgba[3]/1.0f);
#endif
				v[0] = pit->rend[rpi];
				v[1] = cp->rend[rpi];

				glDrawArrays(GL_LINE_STRIP, 0, 2);
				////glDrawArrays(GL_POINTS, 0, 1);
			}
		}
		glLineWidth(1);
#endif

#if 0
		for(auto cit=pit->hist.begin(); cit!=pit->hist.end(); ++cit)
		{
			HypVecf* cp = *cit;

			for(int rpi=0; rpi<REND_3PT_INS; rpi++)
			{
				FixFrac a = ToFrac(1.0) -  MAG_VEC3F( g_pcam->m_pos - pit->rend[rpi])/MAX_DISTANCE;

				glUniform4f(s->slot[SSLOT_COLOR], 
				REND3PTINSCOLORS[rpi].rgba[0], 
				REND3PTINSCOLORS[rpi].rgba[1], 
				REND3PTINSCOLORS[rpi].rgba[2], 
				REND3PTINSCOLORS[rpi].rgba[3]/2.0f * a);

				v[0] = pit->rend[rpi];
				v[1] = cp->rend[rpi];

				//glDrawArrays(GL_LINE_STRIP, 0, 2);
				////glDrawArrays(GL_POINTS, 0, 1);
			}
		}
#endif

#if 0
		FixFrac a = ToFrac(1.0) -  MAG_VEC3F( g_pcam->m_pos - pit->rend[0])/MAX_DISTANCE;


		//for(int rpi=0; rpi<REND_3PT_INS; ++rpi)
		for(int rpi=1; rpi<REND_3PT_INS; ++rpi)
		{
			for(int rpi2=rpi+1; rpi2<REND_3PT_INS; ++rpi2)
			//for(int rpi2=rpi+1; rpi2<REND_3PT_INS-1; ++rpi2)
			{
				//int rpi2 = (rpi+1)%REND_3PT_INS;
				//int rpi2 = 0;
#if 1
				glUniform4f(s->slot[SSLOT_COLOR], 
				(REND3PTINSCOLORS[rpi].rgba[0] + REND3PTINSCOLORS[rpi2].rgba[0])/2.0f, 
				(REND3PTINSCOLORS[rpi].rgba[1] + REND3PTINSCOLORS[rpi2].rgba[1])/2.0f, 
				(REND3PTINSCOLORS[rpi].rgba[2] + REND3PTINSCOLORS[rpi2].rgba[2])/2.0f, 
				(REND3PTINSCOLORS[rpi].rgba[3] + REND3PTINSCOLORS[rpi2].rgba[3])/2.0f/3.0f * a);
#else
				glUniform4f(s->slot[SSLOT_COLOR], 
				1.0f, 
				1.0f, 
				1.0f, 
				0.3f);
#endif

				//for(int rpi=0; rpi<REND_3PT_INS; rpi++)
				v[0] = pit->rend[rpi];
				v[1] = pit->rend[rpi2];

				////glDrawArrays(GL_TRIANGLES, 0, 3);
				//glDrawArrays(GL_LINE_LOOP, 0, 2);
			}
		}
#endif


#if 0
		FixFrac a = ToFrac(1.0) -  MAG_VEC3F( g_pcam->m_pos - pit->rend[0])/MAX_DISTANCE;

		//glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);

		for(int rpi=0; rpi<REND_3PT_INS; ++rpi)
		{
			for(int rpi2=rpi+1; rpi2<REND_3PT_INS; ++rpi2)
			{
				for(int rpi3=rpi2+1; rpi3<REND_3PT_INS; ++rpi3)
				{
					//int rpi2 = (rpi+1)%REND_3PT_INS;

	#if 1
					glUniform4f(s->slot[SSLOT_COLOR], 
					(REND3PTINSCOLORS[rpi].rgba[0] + REND3PTINSCOLORS[rpi2].rgba[0] + REND3PTINSCOLORS[rpi3].rgba[0])/3.0f, 
					(REND3PTINSCOLORS[rpi].rgba[1] + REND3PTINSCOLORS[rpi2].rgba[1] + REND3PTINSCOLORS[rpi3].rgba[1])/3.0f, 
					(REND3PTINSCOLORS[rpi].rgba[2] + REND3PTINSCOLORS[rpi2].rgba[2] + REND3PTINSCOLORS[rpi3].rgba[2])/3.0f, 
					(REND3PTINSCOLORS[rpi].rgba[3] + REND3PTINSCOLORS[rpi2].rgba[3] + REND3PTINSCOLORS[rpi3].rgba[3])/3.0f/300.0f);
	#else
					glUniform4f(s->slot[SSLOT_COLOR], 
					1.0f, 
					1.0f, 
					1.0f, 
					0.3f);
	#endif

					//for(int rpi=0; rpi<REND_3PT_INS; rpi++)
					v[0] = pit->rend[rpi];
					v[1] = pit->rend[rpi2];
					v[2] = pit->rend[rpi3];

					//glDrawArrays(GL_TRIANGLES, 0, 3);
					////glDrawArrays(GL_LINE_LOOP, 0, 2);
				}
			}
		}

		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
#endif
	}
}

void DrawHypMesh(HypMeshf* hm, int rpi)
{
	//UpdRend(hm);
	Shader* s = &g_shader[g_curS];
	Vec3f v[REND_3PT_INS];

#ifdef PLATFORM_GL14
	glVertexPointer(3, GLTYPE, 0, v);
#endif
#ifdef PLATFORM_GLES20
	glVertexAttribPointer(s->slot[SSLOT_POSITION], 3, GLTYPE, GL_FALSE, sizeof(FixFrac)*0, &v[0]);
#endif

#if 0
	for(int x=0; x<100; x++)
	{
		glUniform4f(s->slot[SSLOT_COLOR], 
		0.4f, 
		0.4f, 
		0.4f, 
		0.4f);

		for(int rpi=0; rpi<REND_3PT_INS; rpi++)
			v[rpi] = Vec3f(rand()%500-200, rand()%500-200, rand()%500-200);

	#ifdef PLATFORM_GL14
		glVertexPointer(2, GLTYPE, 0, v);
	#endif
	#ifdef PLATFORM_GLES20
		glVertexAttribPointer(s->slot[SSLOT_POSITION], 2, GLTYPE, GL_FALSE, sizeof(FixFrac)*0, &v[0]);
	#endif
		//glDrawArrays(GL_LINE_STRIP, 0, 3);
	}
#endif

	glPointSize(7.0f);

	HypVecf* prevp = NULL;

	for(std::list<HypVecf>::iterator pit=hm->pts.begin(); pit!=hm->pts.end(); ++pit)
	{

		//for(int rpi=0; rpi<REND_3PT_INS; rpi++)
		//for(int rpi=0; rpi<1; rpi++)
		{
			FixFrac a = ToFrac(1.0) -  MAG_VEC3F( g_pcam->m_pos - pit->rend[rpi])/MAX_DISTANCE;
#if 1
			glUniform4f(s->slot[SSLOT_COLOR], 
			ToBasis2(REND3PTINSCOLORS[rpi].rgba[0]), 
			ToBasis2(REND3PTINSCOLORS[rpi].rgba[1]), 
			ToBasis2(REND3PTINSCOLORS[rpi].rgba[2]), 
			ToBasis2(REND3PTINSCOLORS[rpi].rgba[3]/ToFrac(1.0) *  a));
#endif
#if 0	
			Vec4f color = Vec4f(
			ToFrac(REND3PTINSCOLORS[rpi].rgba[0]), 
			ToFrac(REND3PTINSCOLORS[rpi].rgba[1]), 
			ToFrac(REND3PTINSCOLORS[rpi].rgba[2]), 
			ToFrac(REND3PTINSCOLORS[rpi].rgba[3]/(1.0)));
#endif
			v[0] = pit->rend[rpi];

			//RasterPt(&g_mvp, screendepths, &screencolors, v[0], color);
			glDrawArrays(GL_POINTS, 0, 1);
		}

#if 0
		if(prevp)
		{
			for(int rpi=0; rpi<REND_3PT_INS; rpi++)
			{
				FixFrac a = ToFrac(1.0) -  MAG_VEC3F( g_pcam->m_pos - pit->rend[rpi])/MAX_DISTANCE;

				glUniform4f(s->slot[SSLOT_COLOR], 
				REND3PTINSCOLORS[rpi].rgba[0], 
				REND3PTINSCOLORS[rpi].rgba[1], 
				REND3PTINSCOLORS[rpi].rgba[2], 
				REND3PTINSCOLORS[rpi].rgba[3]/ToFrac(1.0) *  a);

				v[0] = pit->rend[rpi];
				v[1] = prevp->rend[rpi];

				//glDrawArrays(GL_LINES, 0, 2);
			}
		}

		prevp = &*pit;
#endif

#if 1
		for(std::list<HypVecf*>::iterator cit=pit->joins.begin(); cit!=pit->joins.end(); ++cit)
		{
			HypVecf* cp = *cit;
			//cp->age /= 1.004f;

			//for(int rpi=0; rpi<REND_3PT_INS; rpi++)
			{
				FixFrac a = FixFrac(1.0f) * cp->age - MAG_VEC3F( g_pcam->m_pos - pit->rend[rpi])/MAX_DISTANCE;
#if 1
				glUniform4f(s->slot[SSLOT_COLOR], 
				ToBasis2(REND3PTINSCOLORS[rpi].rgba[0]), 
				ToBasis2(REND3PTINSCOLORS[rpi].rgba[1]), 
				ToBasis2(REND3PTINSCOLORS[rpi].rgba[2]), 
				ToBasis2(REND3PTINSCOLORS[rpi].rgba[3]/2.0f * a));
#endif
				v[0] = pit->rend[rpi];
				v[1] = cp->rend[rpi];

				glDrawArrays(GL_LINE_STRIP, 0, 2);
				//glDrawArrays(GL_POINTS, 0, 1);
			}
		}
#endif

#if 0
		for(auto cit=pit->hist.begin(); cit!=pit->hist.end(); ++cit)
		{
			HypVecf* cp = *cit;

			for(int rpi=0; rpi<REND_3PT_INS; rpi++)
			{
				FixFrac a = ToFrac(1.0) -  MAG_VEC3F( g_pcam->m_pos - pit->rend[rpi])/MAX_DISTANCE;

				glUniform4f(s->slot[SSLOT_COLOR], 
				REND3PTINSCOLORS[rpi].rgba[0], 
				REND3PTINSCOLORS[rpi].rgba[1], 
				REND3PTINSCOLORS[rpi].rgba[2], 
				REND3PTINSCOLORS[rpi].rgba[3]/2.0f * a);

				v[0] = pit->rend[rpi];
				v[1] = cp->rend[rpi];

				//glDrawArrays(GL_LINE_STRIP, 0, 2);
				////glDrawArrays(GL_POINTS, 0, 1);
			}
		}
#endif

#if 0
		FixFrac a = ToFrac(1.0) -  MAG_VEC3F( g_pcam->m_pos - pit->rend[0])/MAX_DISTANCE;



		//for(int rpi=0; rpi<REND_3PT_INS; ++rpi)
		for(int rpi=1; rpi<REND_3PT_INS; ++rpi)
		{
			for(int rpi2=rpi+1; rpi2<REND_3PT_INS; ++rpi2)
			//for(int rpi2=rpi+1; rpi2<REND_3PT_INS-1; ++rpi2)
			{
				//int rpi2 = (rpi+1)%REND_3PT_INS;
				//int rpi2 = 0;
#if 1
				glUniform4f(s->slot[SSLOT_COLOR], 
				(REND3PTINSCOLORS[rpi].rgba[0] + REND3PTINSCOLORS[rpi2].rgba[0])/2.0f, 
				(REND3PTINSCOLORS[rpi].rgba[1] + REND3PTINSCOLORS[rpi2].rgba[1])/2.0f, 
				(REND3PTINSCOLORS[rpi].rgba[2] + REND3PTINSCOLORS[rpi2].rgba[2])/2.0f, 
				(REND3PTINSCOLORS[rpi].rgba[3] + REND3PTINSCOLORS[rpi2].rgba[3])/2.0f/3.0f * a);
#else
				glUniform4f(s->slot[SSLOT_COLOR], 
				1.0f, 
				1.0f, 
				1.0f, 
				0.3f);
#endif

				//for(int rpi=0; rpi<REND_3PT_INS; rpi++)
				v[0] = pit->rend[rpi];
				v[1] = pit->rend[rpi2];

				////glDrawArrays(GL_TRIANGLES, 0, 3);
				//glDrawArrays(GL_LINE_LOOP, 0, 2);
			}
		}
#endif


#if 0
		FixFrac a = ToFrac(1.0) -  MAG_VEC3F( g_pcam->m_pos - pit->rend[0])/MAX_DISTANCE;


		//glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);

		for(int rpi=0; rpi<REND_3PT_INS; ++rpi)
		{
			for(int rpi2=rpi+1; rpi2<REND_3PT_INS; ++rpi2)
			{
				for(int rpi3=rpi2+1; rpi3<REND_3PT_INS; ++rpi3)
				{
					//int rpi2 = (rpi+1)%REND_3PT_INS;

	#if 1
					glUniform4f(s->slot[SSLOT_COLOR], 
					(REND3PTINSCOLORS[rpi].rgba[0] + REND3PTINSCOLORS[rpi2].rgba[0] + REND3PTINSCOLORS[rpi3].rgba[0])/3.0f, 
					(REND3PTINSCOLORS[rpi].rgba[1] + REND3PTINSCOLORS[rpi2].rgba[1] + REND3PTINSCOLORS[rpi3].rgba[1])/3.0f, 
					(REND3PTINSCOLORS[rpi].rgba[2] + REND3PTINSCOLORS[rpi2].rgba[2] + REND3PTINSCOLORS[rpi3].rgba[2])/3.0f, 
					(REND3PTINSCOLORS[rpi].rgba[3] + REND3PTINSCOLORS[rpi2].rgba[3] + REND3PTINSCOLORS[rpi3].rgba[3])/3.0f/300.0f);
	#else
					glUniform4f(s->slot[SSLOT_COLOR], 
					1.0f, 
					1.0f, 
					1.0f, 
					0.3f);
	#endif

					//for(int rpi=0; rpi<REND_3PT_INS; rpi++)
					v[0] = pit->rend[rpi];
					v[1] = pit->rend[rpi2];
					v[2] = pit->rend[rpi3];

					//glDrawArrays(GL_TRIANGLES, 0, 3);
					////glDrawArrays(GL_LINE_LOOP, 0, 2);
				}
			}
		}

		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
#endif
	}
}

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

void UpdRend(HypMeshf* hm)
{
	//for(int pt=0; pt<REND_3PT_INS; ++pt)
	{	
	//	Vec3f pointonplanea = PointOnPlane(a->m_plane);	// arbitrary point on plane A	
	}
	//for(int ax=0; ax<HYP_DIMS; ++ax)
	//	printf("dddd%f,%f,%f", pl[ax].m_d,  hm->cen.pos[ax], 123);

	for(std::list<HypVecf>::iterator pit=hm->pts.begin(); pit!=hm->pts.end(); ++pit)
	{
		CvPt(hm->cen.pos, &*pit);
	}
}

#define VS	1.0f
void CvPt(FixFrac* cen, HypVecf* pit)
{
#if 0
	pit->rend[0] = Vec3f(0,0,0);
	for(int ax=0; ax<HYP_DIMS; ++ax)
	{
		pit->rend[0] = pit->rend[0] + axis[ax] * (pit->pos[ax] + cen[ax]);
		//for(int ax2=0; ax2<HYP_DIMS; ++ax2)
		//	pit->rend[ax] = pit->rend[ax] + axis[ax2] * (pit->pos[ax2] + cen[ax2]);
	}
	return;
#endif
#if 0
#define LS		1.0f
	FixFrac cv[HYP_DIMS];
	for(int ax=0; ax<HYP_DIMS; ++ax)
		cv[ax] = cen[ax] + pit->pos[ax];
	FixFrac d = HypDistf(cv, gv.pts.begin()->pos);

//x(t) = x0 + v(t)*t + a(t)*t*t
//a(t) = FixFrac(0)-|x(t)|/x(t)*10

	pit->pos[HYP_DIMS-2] = sqrtf( fabs( ToFrac(1.0) -  70000.0f / ( d * LS ) ) ) * 10000.0f * fsign( ToFrac(1.0) -  70000.0f / ( d * LS ) );
	//FixFrac tc = sqrtf( fabs( ToFrac(1.0) -  7000.0f / ( d * LS ) ) ) * 10000.0f * fsign( ToFrac(1.0) -  7000.0f / ( d * LS ) );

	//if(tc<0.0f)
	{
	//	pit->pos[HYP_DIMS-2] = tc;
	}

	//FixFrac a = 30000.0f * pow(HYP_DIMS, HYP_DIMS) / pow(3.0f, 3.0f) / 30.0f /d;
	//pit->pos[HYP_DIMS-2] = a;
	//FixFrac a = HypDistf(&hv);

	//Log("pit->pos[HYP_DIMS-1]=%f   7000.0f / ( d * LS )=%f", pit->pos[HYP_DIMS-1], 7000.0f / ( d * LS ));
#endif
	Plane3f pl[HYP_DIMS];
	for(int ax=0; ax<HYP_DIMS; ++ax)
	{
		pl[ax].m_normal = Normalize( axis[ax] );
		pl[ax].m_d = FixFrac(INTBASIS(0))-(pit->pos[ax] + cen[ax]) * FixFrac(VS);// * (rand()%3000);
		//printf("i%f,%f,%f", axis[ax].x, axis[ax].y,axis[ax].z);
		//printf("d%f,%f,%f", pl[ax].m_d,  cen[ax], pit->pos[ax]);
	}

	int rpi = 0;
	for(int pl1=0; pl1<HYP_DIMS; ++pl1)
	{
		Vec3f ptpla = PointOnPlane(pl[pl1]);	// arbitrary point on plane A
					//printf("i%f,%f,%f", ptpla.x, ptpla.y,ptpla.z);

	//return;

		for(int pl2=pl1+1; pl2<HYP_DIMS; ++pl2)
		{
			// Get the point intersection between three planes, if there is one.
			// The intersection will be the line formed by the first two planes,
			// intersecting at a point with the third plane.
			Vec3f linedir = Normalize( Cross(pl[pl1].m_normal, pl[pl2].m_normal) );	// line parallel to plane A and B
			Vec3f linedir2 = Normalize( Cross(linedir, pl[pl1].m_normal) ); // direction toward plane B, parallel to plane A
		
			// http://devmaster.net/forums/topic/8676-2-plane-intersection/page__view__findpost__p__47568
			Vec3f l0;	// random point on line 2
			if(!Intersection(ptpla, linedir2, pl[pl2], l0) /*|| (_isnan(l0.x) || _isnan(l0.y) || _isnan(l0.z))*/)
			{
					//ErrMess("!1","!1");
					//exit(0);
					rpi += HYP_DIMS - pl2-1;
					continue;
			}
			//	ptpla = ptpla + Vec3f((rand()%1001-501)/500.0f, (rand()%1001-501)/500.0f, (rand()%1001-501)/500.0f);
				//continue;

			for(int pl3=pl2+1; pl3<HYP_DIMS; ++pl3)
			{

				Vec3f finalpt;
				if(!Intersection(l0, linedir, pl[pl3], finalpt))
				{
					//ErrMess("!2","!2");
					//exit(0);
					++rpi;
					continue;
				}


#ifdef OFFSETVIEW
				finalpt = finalpt;
#endif
			//		l0 = l0 + Vec3f((rand()%1001-500)/500.0f, (rand()%1001-500)/500.0f, (rand()%1001-500)/500.0f);
			//		ErrMess("Intersection problem", "Intersection problem");
					//continue;
					//printf("i%f,%f,%f", finalpt.x, finalpt.y,finalpt.z);
					//if(rand()%20==1)
				//finalpt = Vec3f(rand()%500-200, rand()%400-200, rand()%500-200);
				pit->rend[rpi] = finalpt;
				
				//	printf("p%d,%d,%d,%d", pl1,pl2,pl3,rpi);
				//	fflush(stdout);

				if(rpi >= REND_3PT_INS)
				{
					char m[123];
					sprintf(m, "p%d,%d,%d,%d", pl1,pl2,pl3,rpi);
					ErrMess(">",m);
					exit(0);
				}

				++rpi;
			}
		}
	}
}



