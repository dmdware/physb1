

#include "appmain.h"
#include "../debug.h"
#include "../utils.h"
#include "../gui/gui.h"
#include "../gui/cursor.h"
#include "../gui/draw2d.h"
#include "../math/matrix.h"
#include "../math/vec3f.h"
#include "../math/3dmath.h"
#include "../render/shader.h"
#include "../render/hypmesh.h"
#include "../sim/simflow.h"
#include "../sim/simvars.h"
#include "../sim/simdef.h"
#include "../algo/localmap.h"
#include "../gui/layouts/appgui.h"
#include "../raster/raster.h"

FixFrac* screendepths = NULL;
LoadedTex screencolors;

void MakeTex()
{
	if(screendepths)
	delete [] screendepths;
	screencolors.destroy();

	AllocTex(&screencolors, g_width, g_height, 4);
	screendepths = new FixFrac[ g_width * g_height ];

	for(int pi=0; pi<g_width*g_height; ++pi)
	{
		screendepths[pi] = FixFrac(BIGN);
		//screendepths[pi].n = ((uint64_t)0)-1;
		//screendepths[pi].neg = false;
		screencolors.data[pi*4+0] = 0;
		screencolors.data[pi*4+1] = 0;
		screencolors.data[pi*4+2] = 0;
		screencolors.data[pi*4+3] = 255;
	}
}

void Update()
{//return;
	UpdSim();
}

void DrawAxes()
{
	Shader* s = &g_shader[g_curS];
	Vec3f v[2];
	v[0] = Vec3f(INTBASIS(0),INTBASIS(0),INTBASIS(0));// + Vec3f(GVX,GVY,GVZ);

	glLineWidth(5);

#ifdef PLATFORM_GL14
	glVertexPointer(3, GLTYPE, 0, v);
#endif
#ifdef PLATFORM_GLES20
	glVertexAttribPointer(s->slot[SSLOT_POSITION], 3, GLTYPE, GL_FALSE, sizeof(FixFrac)*0, &v[0]);
#endif


	for(int ax=0; ax<HYP_DIMS; ax++)
	{
#if 1
		v[1] = axis[ax] * 10000.0f * pow(10.0f, HYP_DIMS) ;//+ Vec3f(GVX,GVY,GVZ);
		
		glUniform4f(s->slot[SSLOT_COLOR], 
		REND3PTINSCOLORS[ax].rgba[0], 
		REND3PTINSCOLORS[ax].rgba[1], 
		REND3PTINSCOLORS[ax].rgba[2], 
		REND3PTINSCOLORS[ax].rgba[3]/1.0f);
#if 0
		FixFrac color[4] = {
		ToFrac( REND3PTINSCOLORS[ax].rgba[0] ), 
		ToFrac( REND3PTINSCOLORS[ax].rgba[1] ), 
		ToFrac( REND3PTINSCOLORS[ax].rgba[2] ), 
		ToFrac( REND3PTINSCOLORS[ax].rgba[3]/1.0f )};

		RasterLine(&g_mvp, screendepths, &screencolors, v, color);
#endif
#endif
		glDrawArrays(GL_LINE_STRIP, 0, 2);
	}

	glLineWidth(2);

	
	Vec3f start = Normalize(g_pcam->m_view - g_pcam->m_pos) * 3000.0f + g_pcam->m_pos + g_pcam->m_strafe * 200.0f;// + Vec3f(GVX,GVY,GVZ);
	v[0] = start;

#ifdef PLATFORM_GL14
	glVertexPointer(3, GLTYPE, 0, v);
#endif
#ifdef PLATFORM_GLES20
	glVertexAttribPointer(s->slot[SSLOT_POSITION], 3, GLTYPE, GL_FALSE, sizeof(FixFrac)*0, &v[0]);
#endif

	int rpi=0;
	int ax0;
	int ax1;
	int ax2;

	for(ax0=0; ax0<HYP_DIMS; ++ax0)
	{
		for(ax1=ax0+1; ax1<HYP_DIMS; ++ax1)
		{
			for(ax2=ax1+1; ax2<HYP_DIMS; ++ax2)
			{
				if(selgg == rpi)
					goto found;
				rpi++;
			}
		}
	}

found:

	HypVecf hv[2];
	FixFrac cen[4] = {INTBASIS(0),INTBASIS(0),INTBASIS(0),INTBASIS(0)};
	hv[0].pos[ax0] = v[0].x;
	hv[0].pos[ax1] = v[0].y;
	hv[0].pos[ax2] = v[0].z;
	CvPt(cen,&hv[0]);
	v[0] = hv[0].rend[selgg];

	for(int ax=0; ax<HYP_DIMS; ax++)
	{
		v[1] = axis[ax] * 10.0f * pow(2.0f, HYP_DIMS) + start;//+ Vec3f(GVX,GVY,GVZ);
#if 1
		hv[1].pos[ax0] = v[1].x;
		hv[1].pos[ax1] = v[1].y;
		hv[1].pos[ax2] = v[1].z;
		CvPt(cen,&hv[1]);
		v[1] = hv[1].rend[selgg];
#endif	
#if 1
		glUniform4f(s->slot[SSLOT_COLOR], 
		REND3PTINSCOLORS[ax].rgba[0], 
		REND3PTINSCOLORS[ax].rgba[1], 
		REND3PTINSCOLORS[ax].rgba[2], 
		REND3PTINSCOLORS[ax].rgba[3]/1.0f);
#endif
#if 0
		FixFrac color[4] = {
		ToFrac( REND3PTINSCOLORS[ax].rgba[0] ), 
		ToFrac( REND3PTINSCOLORS[ax].rgba[1] ), 
		ToFrac( REND3PTINSCOLORS[ax].rgba[2] ), 
		ToFrac( REND3PTINSCOLORS[ax].rgba[3]/1.0f )};

		RasterLine(&g_mvp, screendepths, &screencolors, v, color);
	#endif
		glDrawArrays(GL_LINE_STRIP, 0, 2);
	}
	glLineWidth(1);
}

void DrawScene()
{
	//return;
	UseS(SHADER_COLOR3D);
	Shader* s = &g_shader[g_curS];
	
	float aspect = fabsf((float)g_width / (float)g_height);
	Matrix projection = PerspProj(FIELD_OF_VIEW, aspect, MIN_DISTANCE, MAX_DISTANCE);
	//Matrix projection = OrthoProj(FixFrac(INTBASIS(0))-PROJ_RIGHT*aspect, PROJ_RIGHT*aspect, PROJ_RIGHT, -PROJ_RIGHT, MIN_DISTANCE, MAX_DISTANCE);
		
	Vec3f viewvec = g_pcam->m_view;
        Vec3f posvec = g_pcam->m_pos;
        Vec3f upvec = g_pcam->m_up;

	HypVecf hview;
	HypVecf hpos;
	HypVecf hup;

	FixFrac cen[4] = {INTBASIS(0),INTBASIS(0),INTBASIS(0),INTBASIS(0)};

	int ax0 = 0;
	int ax1 = 1;
	int ax2 = 2;
#if 1
	int rpi = 0;
	for(ax0=0; ax0<HYP_DIMS; ++ax0)
	{
		for(ax1=ax0+1; ax1<HYP_DIMS; ++ax1)
		{
			for(ax2=ax1+1; ax2<HYP_DIMS; ++ax2)
			{
				if(rpi == selgg)
					goto found;
				rpi++;
			}
		}
	}
	found:
#endif

	hview.pos[ax0] = viewvec.x;
	hview.pos[ax1] = viewvec.y;
	hview.pos[ax2] = viewvec.z;
	hpos.pos[ax0] = posvec.x;
	hpos.pos[ax1] = posvec.y;
	hpos.pos[ax2] = posvec.z;
	hup.pos[ax0] = upvec.x;
	hup.pos[ax1] = upvec.y;
	hup.pos[ax2] = upvec.z;

	CvPt(cen, &hview);
	CvPt(cen, &hpos);
	CvPt(cen, &hup);

	viewvec = hview.rend[selgg];
	upvec = hup.rend[selgg];
	posvec = hpos.rend[selgg];


#if 0
	if(g_mode == APPMODE_PLAY && g_viewmode == VIEWMODE_THIRD)
	{
		Vec3f viewdir = Normalize(viewvec - posvec);
		viewvec = g_pcam->m_pos;
		posvec = viewvec - viewdir * 1000;
		//TraceWork tw;
		//TraceRay(&g_map.m_brush, &tw, viewvec, posvec);
		//posvec = tw.clip;
		posvec = g_bsp.traceray(viewvec, posvec);
	}
#endif
        
        Matrix viewmat = gluLookAt2(posvec.x, posvec.y, posvec.z,
                                    viewvec.x, viewvec.y, viewvec.z,
                                    upvec.x, upvec.y, upvec.z);

#ifdef OFFSETVIEW
	viewmat = gluLookAt2(0, 0, 0,
                                    viewvec.x-posvec.x, viewvec.y-posvec.y, viewvec.z-posvec.z,
                                    upvec.x, upvec.y, upvec.z);
#endif
		
	Matrix modelview;
	Matrix modelmat;
	FixFrac translation[] = {0.0f, 0.0f, 0.0f};
	modelview.translation(translation);
	modelmat.translation(translation);
	modelview.postmult(viewmat);

	//float pos[3] = {0.0f,0.0f,0.0f};
	//Matrix modelmat;
	//modelmat.translation((const float*)&pos);
	//glUniformMatrix4fv(s->slot[SSLOT_MODELMAT], 1, 0, modelmat.m_matrix);

	//Matrix modelview;
	//modelview.postmult(modelmat);
	//glUniformMatrix4fv(s->slot[SSLOT_MODELVIEW], 1, 0, modelview.m_matrix);

	g_mvp.set(projection.m_matrix);
	g_mvp.postmult(viewmat);
	g_mvp.postmult(modelmat);
	float mvpf[16];
	for(int mvpi=0; mvpi<16; ++mvpi)
		mvpf[mvpi] = ToBasis2(g_mvp.m_matrix[mvpi]);
	glUniformMatrix4fv(s->slot[SSLOT_MVP], 1, 0, mvpf);

	//FixFrac color[4]={1,1,1,1};
	//Vec3f v = Vec3f(0,0,0);
	//v = g_pcam->m_view;
	//RasterPt(&g_mvp, screendepths, &screencolors, v, color);

	//Matrix modelviewinv;
	//Transpose(modelview, modelview);
	//Inverse2(modelview, modelviewinv);
	//glUniformMatrix4fv(s->slot[SSLOT_NORMALMAT], 1, 0, modelviewinv.m_matrix);

	//for(int gi=0; gi<HYP_DIMS*2; ++gi)
	//DrawHypMesh(&axg[gi]);
	//DrawHypMesh(&axg[3]);
	//static int axgi = 0;
	//axgi = (axgi+1)%(HYP_DIMS*2*100);
	//DrawHypMesh(&axg[axgi/100]);

	//for(int gi=0; gi<MESHES; ++gi)
	//	DrawHypMesh(&g[gi]);

	//DrawHypVecs(&hh);

	std::list<HypVecf> hh2;
	hh2.clear();
	hh2.push_back(gc.beadpos);
	DrawHypVecs(&hh2, 2);
	hh2.clear();
	hh2.push_back(gc.craftpos);
	DrawHypVecs(&hh2, 5);
	hh2.clear();
	hh2.push_back(gc.craftpos);
	for(int ax=0; ax<HYP_DIMS-RESERVEDIMS; ++ax)
		hh2.rbegin()->pos[ax] = hh2.rbegin()->pos[ax] * 100 + gc.craftthrust.pos[ax];
	DrawHypVecs(&hh2, 3);

	//DrawHypMesh(&gv);
	//DrawHypMesh(&gve);
	//DrawHypMesh(&gvs);

	//for(int gi=0; gi<pow(GRID,HYP_DIM); ++gi)
	//for(int gi=0; gi<GRIDIS; ++gi)
	//	DrawHypMesh(&gg[gi], selgg);

#if 0
	for(int x=0; x<GRID; ++x)
	{
		for(int y=0; y<GRID; ++y)
		{
			for(int z=0; z<GRID; ++z)
			{
				for(int w=0; w<GRID; ++w)
				{
					//DrawHypMesh(&gg[x][y][z][w]);
				}
			}
		}
	}
#endif

	DrawAxes();

	EndS();
}

void Draw()
{
	MakeTex();
	GUI* gui = &g_gui;

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	DrawScene();


	Ortho(g_width, g_height, 1,1,1,1);
	//RichText r = RichText("Физика Относительность Наука русский транз лекция гершейн");
	//DrawLine(MAINFONT8, 0, 10, &r, color);

	float crop[] = {0,0,(float)g_width,(float)g_height};

	//static unsigned int tex = -1;
	//glDeleteTextures(1, &tex);
	//CreateTex(&screencolors, &tex, false, false);
	//DrawImage(tex, 0, 0, g_width, g_height, 0, 0, 1, 1, crop);

	//RichText r = RichText("lm8");
	//float color[] = {1,1,1,1};
	//DrawLine(MAINFONT8, 0, 10, &r, color);

	char c[123+HYP_DIMS*9] = "";
	float y = 16;

#if 000
	for(int ci=0; ci<HYP_DIMS; ++ci)
	{
		char add[123];
		sprintf(add, "%s:   %f,", AXNAME[ci], 
			(float)ToBasis2(g[selg].cen.pos[ci] + g[selg].pts.begin()->pos[ci]));
		strcat(c,add);
		RichText rc = RichText(add);
		float color[] = {1,1,1,1};
		//const float* color = REND3PTINSCOLORS[ci].rgba;
		DrawLine(MAINFONT8, 0, 10+y, &rc, color);
		y += 16;
	}
#endif

#if 1
	int rpi = 0;
	for(int pl1=0; pl1<HYP_DIMS; ++pl1)
	{
		for(int pl2=pl1+1; pl2<HYP_DIMS; ++pl2)
		{
			for(int pl3=pl2+1; pl3<HYP_DIMS; ++pl3)
			{
				char add[123];
				//sprintf(add, "dims:%d,%d,%d", pl1,pl2,pl3);
				//sprintf(add, "dims:     %s,%s,%s", AXNAME[pl1],AXNAME[pl2],AXNAME[pl3]);
				sprintf(add, "dims:     ");
				RichText rc = RichText(add);
				const float* color = REND3PTINSCOLORS[rpi].rgba;
				DrawLine(MAINFONT8, 0, 10+y, &rc, color);

				if(rpi == selgg)
				{
					rc = RichText(">>>>");
					DrawLine(MAINFONT8, 0, 10+y, &rc, color);
				}

				sprintf(add, "%s", AXNAME[pl1]);
				rc = RichText(add);
				const float* color1 = REND3PTINSCOLORS[pl1].rgba;
				DrawLine(MAINFONT8, 30*5, 10+y, &rc, color1);

				sprintf(add, "%s", AXNAME[pl2]);
				rc = RichText(add);
				const float* color2 = REND3PTINSCOLORS[pl2].rgba;
				DrawLine(MAINFONT8, 30*7, 10+y, &rc, color2);

				sprintf(add, "%s", AXNAME[pl3]);
				rc = RichText(add);
				const float* color3 = REND3PTINSCOLORS[pl3].rgba;
				DrawLine(MAINFONT8, 30*9, 10+y, &rc, color3);

				y += 16;
				rpi++;
			}
		}
	}
#endif
#if 000
	for(int gi=0; gi<1; ++gi)
	{
		rpi = 0;
		for(int pl1=0; pl1<HYP_DIMS; ++pl1)
		{
			for(int pl2=pl1+1; pl2<HYP_DIMS; ++pl2)
			{
				for(int pl3=pl2+1; pl3<HYP_DIMS; ++pl3)
				{
					if(rpi != selgg)
						continue;

					Vec3f xyz = g[gi].pts.begin()->rend[rpi];
					
					Vec4f xyzw = ScreenPos(&g_mvp, xyz, g_width, g_height, true);

					char add[123];

					sprintf(add, "%s:%0000.4f,", AXNAME[pl1], g[gi].pts.begin()->pos[pl1] + g[gi].cen.pos[pl1]);
					RichText rc = RichText(add);
					const float* color1 = REND3PTINSCOLORS[pl1].rgba;
					DrawLine(MAINFONT8, ToBasis(xyzw.x)+30*0, ToBasis(xyzw.y)+16*0, &rc, color1);

					sprintf(add, "%s:%0000.4f,", AXNAME[pl2], g[gi].pts.begin()->pos[pl2] + g[gi].cen.pos[pl2]);
					rc = RichText(add);
					const float* color2 = REND3PTINSCOLORS[pl2].rgba;
					DrawLine(MAINFONT8, ToBasis(xyzw.x)+30*3, ToBasis(xyzw.y)+16*1, &rc, color2);

					sprintf(add, "%s:%0000.4f,", AXNAME[pl3], g[gi].pts.begin()->pos[pl3] + g[gi].cen.pos[pl3]);
					rc = RichText(add);
					const float* color3 = REND3PTINSCOLORS[pl3].rgba;
					DrawLine(MAINFONT8, ToBasis(xyzw.x)+30*6, ToBasis(xyzw.y)+16*2, &rc, color3);

					rpi++;
				}
			}
		}
	}
#endif

	gui->draw();
	EndS();
	gui->frameupd();
}

// Define the function to be called when ctrl-c (SIGINT) signal is sent to process
void SignalCallback(int32_t signum)
{
	//printf("Caught signal %d\n",signum);
	// Cleanup and hide up stuff here

	// Terminate program
	//g_quit = true;
	exit(0);	//force quit NOW
}

void Init()
{
#ifdef PLATFORM_LINUX
	//signal(SIGINT, SignalCallback);
	//signal(SIGTERM, SignalCallback);
#endif
	
	g_selres.width = INI_WIDTH;
	g_selres.height = INI_HEIGHT;

	OpenLog("log.txt", VERSION);

	SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_VERBOSE);
	SDL_LogSetPriority(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_VERBOSE);
	SDL_LogSetPriority(SDL_LOG_CATEGORY_SYSTEM, SDL_LOG_PRIORITY_VERBOSE);
	SDL_LogSetPriority(SDL_LOG_CATEGORY_AUDIO, SDL_LOG_PRIORITY_VERBOSE);
	SDL_LogSetPriority(SDL_LOG_CATEGORY_VIDEO, SDL_LOG_PRIORITY_VERBOSE);
	SDL_LogSetPriority(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_VERBOSE);
	SDL_LogSetPriority(SDL_LOG_CATEGORY_INPUT, SDL_LOG_PRIORITY_VERBOSE);
	SDL_LogSetAllPriority(SDL_LOG_PRIORITY_VERBOSE);
	//SDL_LogSetOutputFunction(log, NULL);

	SDL_SetHint(SDL_HINT_VIDEO_HIGHDPI_DISABLED, "0");
	
	if(SDL_Init(SDL_INIT_VIDEO) == -1)
	{
		char msg[1280];
		sprintf(msg, "SDL_Init: %s\n", SDL_GetError());
		ErrMess("Error", msg);
	}

#if 0

	if(SDLNet_Init() == -1)
	{
		char msg[1280];
		sprintf(msg, "SDLNet_Init: %s\n", SDLNet_GetError());
		ErrMess("Error", msg);
	}

	SDL_version compile_version;
	const SDL_version *link_version=Mix_Linked_Version();
	SDL_MIXER_VERSION(&compile_version);
	printf("compiled with SDL_mixer version: %d.%d.%d\n",
		compile_version.major,
		compile_version.minor,
		compile_version.patch);
	printf("running with SDL_mixer version: %d.%d.%d\n",
		link_version->major,
		link_version->minor,
		link_version->patch);

	// load support for the OGG and MOD sample/music formats
	//int32_t flags=MIX_INIT_OGG|MIX_INIT_MOD|MIX_INIT_MP3;
	int32_t flags=MIX_INIT_OGG|MIX_INIT_MP3;
	int32_t initted=Mix_Init(flags);
	if( (initted & flags) != flags)
	{
		char msg[1280];
		sprintf(msg, "Mix_Init: Failed to init required ogg and mod support!\nMix_Init: %s", Mix_GetError());
		//ErrMess("Error", msg);
		// handle error
	}

	///Mix_ChannelFinished(ChannelDone);

	// start SDL with audio support
	if(SDL_Init(SDL_INIT_AUDIO)==-1) {
		char msg[1280];
		sprintf(msg, "SDL_Init: %s\n", SDL_GetError());
		ErrMess("Error", msg);
		// handle error
		//exit(1);
	}
	// show 44.1KHz, signed 16bit, system byte order,
	//      stereo audio, using 1024 byte chunks
	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024)==-1) {
		char msg[1280];
		printf("Mix_OpenAudio: %s\n", Mix_GetError());
		ErrMess("Error", msg);
		// handle error
		//exit(2);
	}

	///Mix_AllocateChannels(SOUND_CHANNELS);
#endif

	srand((uint32_t)GetTicks());
}

void Deinit()
{
	BreakWin(TITLE);

	//Mix_CloseAudio();

	// force a quit
	//while(Mix_Init(0))
	//	Mix_Quit();
	//Mix_Quit();

	//SDLNet_Quit();
	SDL_Quit();
}

int32_t HandleEvent(void *userdata, SDL_Event *e)
{
	GUI* gui = &g_gui;
	InEv ie;
	ie.intercepted = false;
	ie.curst = CU_DEFAULT;
	Vec2i old;
	
	SDL_TouchFingerEvent* tfe = &e->tfinger;
	
	switch(e->type)
	{
#ifdef PLATFORM_IOS
		case SDL_APP_TERMINATING:
			/* Terminate the app.
			 Shut everything down before returning from this function.
			 */
			g_quit = true;
			g_background = true;
			exit(0);
			return 0;
		case SDL_APP_LOWMEMORY:
			/* You will get this when your app is paused and iOS wants more memory.
			 Release as much memory as possible.
			 */
			exit(0);
			return 0;
		case SDL_APP_WILLENTERBACKGROUND:
			/* Prepare your app to go into the background.  Stop loops, etc.
			 This gets called when the user hits the home button, or gets a call.
			 */
			g_quit = true;
			g_background = true;
			exit(0);
			return 0;
		case SDL_APP_DIDENTERBACKGROUND:
			/* This will get called if the user accepted whatever sent your app to the background.
			 If the user got a phone call and canceled it, you'll instead get an SDL_APP_DIDENTERFOREGROUND event and restart your loops.
			 When you get this, you have 5 seconds to save all your state or the app will be terminated.
			 Your app is NOT active at this point.
			 */
			g_quit = true;
			g_background = true;
			exit(0);
			return 0;
		case SDL_APP_WILLENTERFOREGROUND:
			/* This call happens when your app is coming back to the foreground.
			 Restore all your state here.
			 */
			g_background = false;
			return 0;
		case SDL_APP_DIDENTERFOREGROUND:
			/* Restart your loops here.
			 Your app is interactive and getting CPU again.
			 */
			g_background = false;
			return 0;
#endif
		case SDL_WINDOWEVENT:
			{
				switch (e->window.event)
				{
					case SDL_WINDOWEVENT_RESIZED:
						Resize(e->window.data1, e->window.data2);
					break;
				};
			}
			break;
		case SDL_QUIT:
			g_quit = true;
			break;
		case SDL_KEYDOWN:
			ie.type = INEV_KEYDOWN;
			ie.key = e->key.keysym.sym;
			ie.scancode = e->key.keysym.scancode;
			
			//Handle copy
			if( e->key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL )
			{
				ie.type = INEV_COPY;
			}
			//Handle paste
			if( e->key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL )
			{
				ie.type = INEV_PASTE;
			}
			//Select all
			if( e->key.keysym.sym == SDLK_a && SDL_GetModState() & KMOD_CTRL )
			{
				ie.type = INEV_SELALL;
			}
			
			gui->inev(&ie);
			
			if(!ie.intercepted)
				g_keys[e->key.keysym.scancode] = true;
			
			g_keyintercepted = ie.intercepted;
			break;
		case SDL_KEYUP:
			ie.type = INEV_KEYUP;
			ie.key = e->key.keysym.sym;
			ie.scancode = e->key.keysym.scancode;
			
			gui->inev(&ie);
			
			if(!ie.intercepted)
				g_keys[e->key.keysym.scancode] = false;
			
			g_keyintercepted = ie.intercepted;
			break;
		case SDL_TEXTINPUT:
			ie.type = INEV_TEXTIN;
			ie.text = e->text.text;
			
			gui->inev(&ie);
			break;
			
#ifndef PLATFORM_MOBILE
		case SDL_MOUSEWHEEL:
			ie.type = INEV_MOUSEWHEEL;
			ie.amount = e->wheel.y;
			
			gui->inev(&ie);
			break;
		case SDL_MOUSEBUTTONDOWN:

			switch (e->button.button)
		{
			case SDL_BUTTON_LEFT:
				g_mousekeys[MOUSE_LEFT] = true;
				g_moved = false;
				
				ie.type = INEV_MOUSEDOWN;
				ie.key = MOUSE_LEFT;
				ie.amount = 1;
				ie.x = g_mouse.x;
				ie.y = g_mouse.y;

				gui->inev(&ie);
				
				g_keyintercepted = ie.intercepted;
				break;
			case SDL_BUTTON_RIGHT:
				g_mousekeys[MOUSE_RIGHT] = true;
				
				ie.type = INEV_MOUSEDOWN;
				ie.key = MOUSE_RIGHT;
				ie.amount = 1;
				ie.x = g_mouse.x;
				ie.y = g_mouse.y;
				
				gui->inev(&ie);
				break;
			case SDL_BUTTON_MIDDLE:
				g_mousekeys[MOUSE_MIDDLE] = true;
				
				ie.type = INEV_MOUSEDOWN;
				ie.key = MOUSE_MIDDLE;
				ie.amount = 1;
				ie.x = g_mouse.x;
				ie.y = g_mouse.y;
				
				gui->inev(&ie);
				break;
		}
			break;
		case SDL_MOUSEBUTTONUP:
			switch (e->button.button)
		{
				case SDL_BUTTON_LEFT:
					g_mousekeys[MOUSE_LEFT] = false;
					
					ie.type = INEV_MOUSEUP;
					ie.key = MOUSE_LEFT;
					ie.amount = 1;
					ie.x = g_mouse.x;
					ie.y = g_mouse.y;
					
					gui->inev(&ie);
					break;
				case SDL_BUTTON_RIGHT:
					g_mousekeys[MOUSE_RIGHT] = false;
					
					ie.type = INEV_MOUSEUP;
					ie.key = MOUSE_RIGHT;
					ie.amount = 1;
					ie.x = g_mouse.x;
					ie.y = g_mouse.y;
					
					gui->inev(&ie);
					break;
				case SDL_BUTTON_MIDDLE:
					g_mousekeys[MOUSE_MIDDLE] = false;
					
					ie.type = INEV_MOUSEUP;
					ie.key = MOUSE_MIDDLE;
					ie.amount = 1;
					ie.x = g_mouse.x;
					ie.y = g_mouse.y;
					
					gui->inev(&ie);
					break;
			}
			break;
		case SDL_MOUSEMOTION:
			
			if(g_mouseout)
			{
				g_mouseout = false;
			}
			
			old = g_mouse;
			
			if(MousePosition())
			{
				g_moved = true;
				
				ie.type = INEV_MOUSEMOVE;
				ie.x = g_mouse.x;
				ie.y = g_mouse.y;
				ie.dx = g_mouse.x - old.x;
				ie.dy = g_mouse.y - old.y;
				
				gui->inev(&ie);
				
				g_curst = ie.curst;
			}
			break;
#else	//def PLATFORM_MOBILE
		case SDL_FINGERMOTION:
			
			if(g_mouseout)
			{
				g_mouseout = false;
			}
			
			old = g_mouse;
			
			g_moved = true;
				
			ie.type = INEV_MOUSEMOVE;
			ie.x = tfe->x * g_width;
			ie.y = tfe->y * g_height;
			ie.dx = tfe->dx * g_width;
			ie.dy = tfe->dy * g_height;
			g_mouse.x = tfe->x * g_width;
			g_mouse.y = tfe->y * g_height;
				
			gui->inev(&ie);
				
			g_curst = ie.curst;
			break;
		case SDL_FINGERDOWN:
		{
			g_mousekeys[MOUSE_LEFT] = true;
			g_moved = false;
			
			ie.type = INEV_MOUSEDOWN;
			ie.key = MOUSE_LEFT;
			ie.amount = 1;
			ie.x = tfe->x * g_width;
			ie.y = tfe->y * g_height;
			ie.dx = ie.x - g_mouse.x;
			ie.dy = ie.y - g_mouse.y;
			g_mouse.x = tfe->x * g_width;
			g_mouse.y = tfe->y * g_height;
			gui->inev(&ie);
			
			g_keyintercepted = ie.intercepted;
		}
			break;
		case SDL_FINGERUP:
		{
			
			g_mousekeys[MOUSE_LEFT] = false;
			
			ie.type = INEV_MOUSEUP;
			ie.key = MOUSE_LEFT;
			ie.amount = 1;
			ie.x = tfe->x * g_width;
			ie.y = tfe->y * g_height;
			ie.dx = ie.x - g_mouse.x;
			ie.dy = ie.y - g_mouse.y;
			g_mouse.x = tfe->x * g_width;
			g_mouse.y = tfe->y * g_height;
			gui->inev(&ie);
		}
		break;
#endif
	}

	return 0;
}

void EventLoop()
{
	while (!g_quit)
	{
		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			HandleEvent(NULL, &e);
		}
		
		Update();
		Draw();
		SDL_GL_SwapWindow(g_window);
	}
}

void MouseMove(InEv* ie)
{
	if(g_mousekeys[MOUSE_LEFT])
	{
		g_pcam->rotateabout(g_pcam->m_pos, -ie->dy/ 100.0f, g_pcam->m_strafe.x, g_pcam->m_strafe.y, g_pcam->m_strafe.z);
		g_pcam->rotateabout(g_pcam->m_pos, -ie->dx/ 100.0f, g_pcam->m_up.x, g_pcam->m_up.y, g_pcam->m_up.z);
		//g_pcam->rotatebymouse(ie->dx*100, ie->dy*100);
	}
}

void Forward()
{
	g_pcam->move(ToBasis2(FixFrac(FixFrac(10.0)*KMSC)));
}

void Backward()
{
	g_pcam->move(ToBasis2(FixFrac(INTBASIS(0))-FixFrac(FixFrac(10.0)*KMSC)));
}

void StrafeL()
{
	g_pcam->strafe(ToBasis2(FixFrac(INTBASIS(0))-FixFrac(FixFrac(10.0)*KMSC)));
}

void StrafeR()
{
	g_pcam->strafe(ToBasis2(FixFrac(FixFrac(10.0)*KMSC)));
}

void UpSelDims()
{
	selgg = (selgg+1)%REND_3PT_INS;
}

void DownSelDims()
{
	selgg = (selgg+REND_3PT_INS-1)%REND_3PT_INS;
}

void UpSelTg()
{
#if 000
	selg = (selg+1)%MESHES;
	HypMeshf* hm = &g[selg];
	Vec3f to = Vec3f(hm->cen.pos[0],hm->cen.pos[1],hm->cen.pos[2]);
	g_pcam->moveto(to);
#endif
}

void DownSelTg()
{
#if 000
	selg = (selg+MESHES-1)%MESHES;
	HypMeshf* hm = &g[selg];
	Vec3f to = Vec3f(hm->cen.pos[0],hm->cen.pos[1],hm->cen.pos[2]);
	g_pcam->moveto(to);
#endif
}

//#ifdef PLATFORM_WIN
//int32_t WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int32_t nCmdShow)
//#else
int32_t main(int32_t argc, char* argv[])
//#endif
{
	//FillTrig();
	Init();
	Log("test");
	
	MakeWin(TITLE);
	
	//AllocTex(&board, INI_WIDTH, INI_HEIGHT, 3);
	//LoadSysRes();
	//Queue();
	FillGUI();
	DefSim();

	GUI* gui = &g_gui;
	gui->mousemovefunc = MouseMove;
	gui->keydownfunc[SDL_SCANCODE_W] = Forward;
	gui->keydownfunc[SDL_SCANCODE_S] = Backward;
	gui->keydownfunc[SDL_SCANCODE_A] = StrafeL;
	gui->keydownfunc[SDL_SCANCODE_D] = StrafeR;
	gui->keydownfunc[SDL_SCANCODE_UP] = UpSelDims;
	gui->keydownfunc[SDL_SCANCODE_DOWN] = DownSelDims;
	gui->keydownfunc[SDL_SCANCODE_PAGEUP] = UpSelTg;
	gui->keydownfunc[SDL_SCANCODE_PAGEDOWN] = DownSelTg;
	gui->keydownfunc[SDL_SCANCODE_SPACE] = UpdCraft;


	
	EventLoop();
	
	Deinit();

	return 0;
}
