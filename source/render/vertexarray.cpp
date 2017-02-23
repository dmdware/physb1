


#include "vertexarray.h"
#include "../utils.h"

VertexArray::VertexArray(const VertexArray& original)
{
	//Log("vertex array copy constructor");
	/*
	alloc(original.numverts);
	memcpy(vertices, original.vertices, sizeof(Vec3f)*numverts);
	memcpy(texcoords, original.texcoords, sizeof(Vec2f)*numverts);
	memcpy(normals, original.normals, sizeof(Vec3f)*numverts);*/

	*this = original;
}


VertexArray& VertexArray::operator=(VertexArray const &original)
{
	//Log("vertex array assignment op");

	alloc(original.numverts);
	memcpy(vertices, original.vertices, sizeof(Vec3f)*numverts);
	memcpy(texcoords, original.texcoords, sizeof(Vec2f)*numverts);
	memcpy(normals, original.normals, sizeof(Vec3f)*numverts);
	//memcpy(tangents, original.tangents, sizeof(Vec3f)*numverts);

	for(int i=0; i<VBOS; i++)
		vbo[i] = original.vbo[i];	//corpe fix

	vao = original.vao;

	return *this;
}

void VertexArray::alloc(int numv)
{
	free();
	numverts = numv;
	vertices = new Vec3f[numv];
	texcoords = new Vec2f[numv];
	normals = new Vec3f[numv];
	//tangents = new Vec3f[numv];
}

void VertexArray::free()
{
	if(numverts <= 0)
		return;

	delete [] vertices;
	delete [] texcoords;
	delete [] normals;
	//delete [] tangents;
	numverts = 0;

	delvbo();
}

void VertexArray::genvbo()
{
	delvbo();

#ifdef PLATFORM_GL32

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glGenBuffers(1, &vbo[VBO_POSITION]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[VBO_POSITION]);
	glBufferData(GL_ARRAY_BUFFER, numverts * sizeof(Vec3f), vertices, GL_STATIC_DRAW);
	//glBindBuffer(GL_ARRAY_BUFFER, vbo[VBO_POSITION]);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)(vertices));	//positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)(0));	//positions
	
	//glBindBuffer(GL_ARRAY_BUFFER, vbo[VBO_NORMAL]);
	glGenBuffers(1, &vbo[VBO_NORMAL]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[VBO_NORMAL]);
	glBufferData(GL_ARRAY_BUFFER, numverts * sizeof(Vec3f), normals, GL_STATIC_DRAW);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)(normals));	//normals;
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)(0));	//normals


	glGenBuffers(1, &vbo[VBO_TEXCOORD]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[VBO_TEXCOORD]);
	glBufferData(GL_ARRAY_BUFFER, numverts * sizeof(Vec2f), texcoords, GL_STATIC_DRAW);
	//glBindBuffer(GL_ARRAY_BUFFER, vbo[VBO_TEXCOORD]);
	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)(texcoords));	//texc0
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)(0));	//texc0
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
#endif

#ifdef PLATFORM_GL14
	glGenBuffers(VBOS, vbo);

	//GL_ARRAY_BUFFER
	//GL_STATIC_DRAW
	glBindBuffer(GL_ARRAY_BUFFER, vbo[VBO_POSITION]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vec3f)*numverts, vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[VBO_TEXCOORD]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vec2f)*numverts, texcoords, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[VBO_NORMAL]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vec3f)*numverts, normals, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
#endif
}

void VertexArray::delvbo()
{
#ifdef PLATFORM_GL32
	for (int i = 0; i<VBOS; i++)
	{
		if (vbo[i] == -1)
			continue;
		glDeleteBuffers(1, &vbo[i]);
		vbo[i] = -1;
	}

	glDeleteVertexArrays(1, &vao);
#endif

#ifdef PLATFORM_GL14
	for(int i=0; i<VBOS; i++)
	{
		if(vbo[i] == -1)
			continue;
		glDeleteBuffers(1, &vbo[i]);
		vbo[i] = -1;
	}
#endif
}

void CopyVA(VertexArray* to, const VertexArray* from)
{
	to->alloc(from->numverts);

	for(int i=0; i<from->numverts; i++)
	{
		to->vertices[i] = from->vertices[i];
		to->texcoords[i] = from->texcoords[i];
		to->normals[i] = from->normals[i];
	}
}

void CopyVAs(VertexArray** toframes, int* tonframes, VertexArray* const* fromframes, int fromnframes)
{
	*tonframes = fromnframes;

	(*toframes) = new VertexArray[fromnframes];

	for(int i=0; i<fromnframes; i++)
	{
		CopyVA(&(*toframes)[i], &(*fromframes)[i]);
	}
}
