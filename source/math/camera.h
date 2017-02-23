
#ifndef CAMERA_H
#define CAMERA_H

#include "vec2f.h"
#include "vec3f.h"
#include "quaternion.h"
#include "fixtrig.h"

#define MOUSE_SENSITIVITY	0.0001f	//radians over pixels

class Camera 
{
public:
	Vec3f m_pos;					
	Vec3f m_view;			
	Vec3f m_up;		
	Vec3f m_strafe;		
	Vec3f m_vel;
	Vec3f m_orientv;	// pitch, yaw, roll
	Quaternion m_orientq;	//orientation quaternion
	bool m_grounded;

	Camera();
	Camera(FixFrac posx, FixFrac posy, FixFrac posz, FixFrac viewx, FixFrac viewy, FixFrac viewz, FixFrac upx, FixFrac upy, FixFrac upz);

	Vec3f up2();
	void position(FixFrac posx, FixFrac posy, FixFrac posz, FixFrac viewx, FixFrac viewy, FixFrac viewz, FixFrac upx, FixFrac upy, FixFrac upz);
	void rotateview(FixFrac angle, FixFrac x, FixFrac y, FixFrac z);
	void lookat(Vec3f at);
	void rotatebymouse(int dx, int dy);
	Vec3f lookpos();
	void grounded(bool ground);
	void rotateabout(Vec3f center, FixFrac rad, FixFrac x, FixFrac y, FixFrac z);
	void strafe(FixFrac speed);
	void move(FixFrac speed);
	void accelerate(FixFrac speed);
	void accelstrafe(FixFrac speed);
	void accelrise(FixFrac speed);
	void rise(FixFrac speed);
	void move(Vec3f delta);
	void moveto(Vec3f newpos);
	void limithvel(FixFrac limit);
	void stop();
	void calcstrafe();
	void calcyaw();
	void calcpitch();
	void calcroll();
	void frameupd();
	void friction();
	void friction2();
	FixFrac yaw();
	FixFrac pitch();
};									

extern Camera g_cam;
extern Camera* g_pcam;

#endif
