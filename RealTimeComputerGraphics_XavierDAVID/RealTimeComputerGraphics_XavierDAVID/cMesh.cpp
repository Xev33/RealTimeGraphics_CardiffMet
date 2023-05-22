#include "cMesh.h"
#include <string.h>
#include <stdio.h>
#include "core.h"
#include "cCamera.h"
#include "tga.h"

#include "glm/glm.hpp"  
#include "glm/gtc/matrix_transform.hpp" 
#include "glm/gtc/type_ptr.hpp"
#include <iostream>

using namespace glm;

const unsigned int vertexPos_loc	= 0;   // Corresponds to "location = 0" in the vertex shader definition
const unsigned int texCoord_loc		= 1;
const unsigned int vertexNormal_loc = 2;

//////////////////////////////////////////////////////////////////////////////////////////
// loadTGA() - .tga file loader
//////////////////////////////////////////////////////////////////////////////////////////
int loadTGA(char* TexName, GLuint TexHandle, GLuint textureID)
{
	TGAImg Img;        // Image loader

	// Load our Texture
	if (Img.Load(TexName) != IMG_OK)
		return false;

	glBindTexture(GL_TEXTURE_2D, TexHandle);

	// Create the texture
	if (Img.GetBPP() == 24)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, 3, Img.GetWidth(), Img.GetHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, Img.GetImg());
	}
	else if (Img.GetBPP() == 32)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, 4, Img.GetWidth(), Img.GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, Img.GetImg());
	}
	else
	{
		return 0;
	}

	// Specify filtering and edge actions
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	return Img.GetBPP();
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////
// cMesh() - 
//////////////////////////////////////////////////////////////////////////////////////////////////////////
cMesh::cMesh()
{
	m_fp = 0;

	m_e_count = 0;
	m_v_count  = 0;
	m_vt_count = 0;
	m_vn_count = 0;
	m_f_count  = 0;

	m_v  = NULL;
	m_vt = NULL;
	m_vn = NULL;
	m_fn = NULL;
	m_f  = NULL;

	m_vList = NULL;
	m_eList = NULL;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// ~cMesh() - 
//////////////////////////////////////////////////////////////////////////////////////////////////////////
cMesh::~cMesh()
{
	if( m_v )
		delete [] m_v;
	if( m_vt )
		delete [] m_vt;
	if( m_vn )
		delete m_vn;
	if( m_f )
		delete m_f;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// init() - 
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void cMesh::init()
{
	load("Meshes/", m_filename);

	int t_count = 0;

	// get the number of textures we need to generate...
	t_count = matList(0)->m_tex_count;

	glGenTextures(t_count, m_tex);

	for (int j = 0; j < t_count; j++)
	{
		loadTGA(matList(0)->tex[j], m_tex[j], j);
	}
	
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);


	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cVertex) * 3 * m_f_count, m_vList, GL_STATIC_DRAW); // triangles
	glVertexAttribPointer(vertexPos_loc, 3, GL_FLOAT, GL_FALSE, sizeof(cVertex), (void*)0);
	glEnableVertexAttribArray(vertexPos_loc);
	glVertexAttribPointer(vertexNormal_loc, 3, GL_FLOAT, GL_FALSE, sizeof(cVertex), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(vertexNormal_loc);
	glVertexAttribPointer(texCoord_loc, 2, GL_FLOAT, GL_FALSE, sizeof(cVertex), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(texCoord_loc);


	// lock down this data to the current VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	m_shdr = m_shaderID; // shader index
	m_lght = m_lightID;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// render() - 
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void cMesh::render(cScene* pScene, cShaderInfo* pShaderInfo)
{
	cShader* pList = pShaderInfo->getList();
	int shdr = m_shdr;
	int lght = m_lght;

	if (shdr >= pScene->m_shader_count)
	{
		printf("Error: Can't access requested shader\n");
		shdr = 0;
	}


	int t_count = 0;

	t_count = matList(0)->m_tex_count;

	// bind the required texture for this mesh
	for (int i = 0; i < t_count; i++)
	{
		glBindTextureUnit(i, m_tex[i]);
	}

	// enable shader program..
	glUseProgram(pList[shdr].program());
	cLight light = pScene->m_Light[lght];

	mat4 modelMatrix = m_transform.m_ModelMatrix;
	mat4 viewMatrix = pScene->m_Camera[pScene->m_cameraId].viewMatrix;
	mat4 projMatrix = pScene->m_Camera[pScene->m_cameraId].projectionMatrix;
	vec3 lightPos = light.m_pos;
	vec3 lightAtt = light.m_att;
	vec3 lightDir = light.m_dir;
	vec3 lightCol = light.m_col;

	glUniformMatrix4fv(3, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glUniformMatrix4fv(4, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniformMatrix4fv(5, 1, GL_FALSE, glm::value_ptr(projMatrix));

	glUniform3fv(6, 1, glm::value_ptr(lightPos));
	glUniform3fv(7, 1, glm::value_ptr(lightCol));
	glUniform3fv(8, 1, glm::value_ptr(lightAtt));
	glUniform3fv(9, 1, glm::value_ptr(lightDir));

	// Draw FACES
	int vertexCount = m_f_count * 3; // calculate the number of vertices for this object
	glBindVertexArray(m_VAO);

	glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////
// createFaceNormalList() - load into memory list of face normals
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void cMesh::createFaceNormalList()
{
	// allocate memory to hold one face normal per face
	m_fn = new cVec3f[(int)m_f_count];

	// loop through each face and calculate the face normal
	for (int f = 0; f < m_f_count; f++)
	{
		m_fn[f] = (m_vn[m_f[f].vn_id[0]] + m_vn[m_f[f].vn_id[1]] + m_vn[m_f[f].vn_id[2]]);

		m_fn[f].normalize();
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////
// faceList() - load into memory list of faces
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void cMesh::createFaceList()
{
	m_vList = new cVertex[((int)m_f_count) * 3];

	for (int f = 0; f < m_f_count; f++)
	{
		// vertices
		m_vList[f * 3 + 0].m_v	= m_v[m_f[f].v_id[0]];
		m_vList[f * 3 + 1].m_v	= m_v[m_f[f].v_id[1]];
		m_vList[f * 3 + 2].m_v	= m_v[m_f[f].v_id[2]];

		// vertex normals
		m_vList[f * 3 + 0].m_n = m_vn[m_f[f].vn_id[0]];
		m_vList[f * 3 + 1].m_n = m_vn[m_f[f].vn_id[1]];
		m_vList[f * 3 + 2].m_n = m_vn[m_f[f].vn_id[2]];

		// texture coordinates
		m_vList[f * 3 + 0].m_uv = m_vt[m_f[f].vt_id[0]];
		m_vList[f * 3 + 1].m_uv = m_vt[m_f[f].vt_id[1]];
		m_vList[f * 3 + 2].m_uv = m_vt[m_f[f].vt_id[2]];
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////
// importMTL() - 
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void cMesh::importMTL()
{
	char buffer[64];
	char mtl_path[256];

	int buffer_size = sizeof(buffer);

	strcpy_s(mtl_path, m_mtl_filename);

	// load .mtl file
	fopen_s(&m_mtl, mtl_path, "r");

	if (!m_mtl)
		return;


	m_material_count = 0;

	// search through mtl file and find out how many materials we have
	do
	{
		fscanf_s(m_mtl, "%s", buffer, buffer_size);

		if (strcmp(buffer, "newmtl") == 0)
			m_material_count++;

	} while (!feof(m_mtl));


	// allocate sufficient memory to load all materials
	m_mtlList = new newmtl[m_material_count];

	fseek(m_mtl, 0, SEEK_SET);

	int count = -1;

	// search through mtl file and extract all materials for this object
	do
	{
		fscanf_s(m_mtl, "%s", buffer, buffer_size);

		if (strcmp(buffer, "newmtl") == 0)
		{
			count++;
			// read in material name
			fscanf_s(m_mtl, "%s", buffer, buffer_size);
			strcpy_s(m_mtlList[count].name, buffer);
		}
		else
		{
			if (strcmp(buffer, "Kd") == 0)
			{
				// read in diffuse values
				fscanf_s(m_mtl, "%f%f%f", &m_mtlList[count].diffuse.r, &m_mtlList[count].diffuse.g, &m_mtlList[count].diffuse.b);
			}
			else
			{
				if (strcmp(buffer, "map_Kd") == 0) // diffuse mapd
				{
					char texturePath[128];
					strcpy_s(texturePath, "tga/");
					// read in the texture name
					fscanf_s(m_mtl, "%s", buffer, (unsigned int)sizeof(buffer));
					strcat_s(texturePath, buffer);
					strcpy_s(m_mtlList[count].tex[0], texturePath);

					m_mtlList[count].m_tex_count++;

					if (m_import_filetype == FILE_TYPE_BLENDER)
					{
						break;
					}
				}
				if (strcmp(buffer, "map_Ks") == 0) // specular map
				{
					char texturePath[128];
					strcpy_s(texturePath, "tga/");
					// read in the texture name
					fscanf_s(m_mtl, "%s", buffer, (unsigned int)sizeof(buffer));
					strcat_s(texturePath, buffer);
					strcpy_s(m_mtlList[count].tex[1], texturePath);

					m_mtlList[count].m_tex_count++;

					if (m_import_filetype == FILE_TYPE_BLENDER)
					{
						break;
					}
				}
				if (strcmp(buffer, "bump") == 0) // normal map
				{
					char texturePath[128];
					strcpy_s(texturePath, "tga/");
					// read in the texture name
					fscanf_s(m_mtl, "%s%f", buffer, (unsigned int)sizeof(buffer), &m_mtlList[count].m_bump_height);
					fscanf_s(m_mtl, "%s", buffer, (unsigned int)sizeof(buffer));
					strcat_s(texturePath, buffer);
					strcpy_s(m_mtlList[count].tex[2], texturePath);

					m_mtlList[count].m_tex_count++;

					if (m_import_filetype == FILE_TYPE_BLENDER)
					{
						break;
					}
				}
			}
		}

	} while (!feof(m_mtl));


	// OK, so we've finished with the .mtl file
	fclose(m_mtl);
	m_mtl = NULL;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// loadFaces() - extracts face data from .obj file
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void cMesh::loadFaces()
{
	char buffer[32] = { NULL };
	int  f[3] = { NULL };
	char slash[2] = { NULL };
	int  face = 0;
	int  m_index = 0;
	char materialName[64] = { NULL };
	
	fseek(m_fp, 0, SEEK_SET);

	do
	{
		fscanf_s(m_fp, "%s", buffer, unsigned int(sizeof(char) * 32));
	} while (strcmp(buffer, "usemtl") != 0);
	fscanf_s(m_fp, "%s", materialName, (unsigned int)sizeof(materialName));// material name

	// convert material name into index into m_mtlList
	for (int m = 0; m < m_material_count; m++)
	{
		if (strcmp(materialName, m_mtlList[m].name) == 0)
		{
			m_index = m;
			break;
		}
	}

	// search for 'f' (first face triplet)
	while (strcmp(buffer, "f") != 0)
		fscanf_s(m_fp, "%s", buffer, unsigned int(sizeof(char) * 32));

	do
	{
		for (int i = 0; i < 3; i++)
		{
			fscanf_s(m_fp, "%d%c%d%c%d", &f[0], &slash, (unsigned int)sizeof(slash), &f[1], &slash, (unsigned int)sizeof(slash), &f[2]);

			// store indices for each vertex on this face here..
			if (i == 0)
			{
				m_f[face].mtl_id = m_index;
			}

			if (m_import_filetype == FILE_TYPE_BLENDER)
			{
				m_f[face].v_id[i] = f[0] - 1;
				m_f[face].vt_id[i] = f[1] - 1;
				m_f[face].vn_id[i] = f[2] - 1;
			}
			else
			{
				m_f[face].v_id[i] = f[0] - 1;
				m_f[face].vt_id[i] = f[1] - 1;
				m_f[face].vn_id[i] = f[2] - 1;
			}
		}
		face++;

		// OK, so we've finished loading all the faces in this file
		if (face == m_f_count)
			return;

		fscanf_s(m_fp, "%s", buffer, unsigned int(sizeof(char) * 32));
			
		// step over surface flag
		if(strcmp(buffer, "s") == 0)
			fscanf_s(m_fp, "%s%s", buffer, unsigned int(sizeof(char) * 32), buffer, unsigned int(sizeof(char) * 32));

		if (strcmp(buffer, "g") == 0)
		{
			// search for another material
			do
			{
				fscanf_s(m_fp, "%s", buffer, unsigned int(sizeof(char) * 32));
			} while (strcmp(buffer, "usemtl") != 0);
			fscanf_s(m_fp, "%s", materialName, (unsigned int)sizeof(materialName));// material name

			// convert material name into index into m_mtlList
			for (int m = 0; m < m_material_count; m++)
			{
				if (strcmp(materialName, m_mtlList[m].name) == 0)
				{
					m_index = m;
					break;
				}
			}
			fscanf_s(m_fp, "%s", buffer, unsigned int(sizeof(char) * 32));
		}
	} while (strcmp(buffer, "f") == 0);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// load() - 
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void cMesh::load(const char* obj_path, const char* obj_name)
{
	char filepath[64];

	strcpy_s(filepath, obj_path);
	strcat_s(filepath, obj_name);

	fopen_s(&m_fp, filepath, "r");

	if (!m_fp)
		return;

	static char buffer[32];

	// initialise the import type to use MAYA .obj
	m_import_filetype = FILE_TYPE_MAYA;

	// find the material library used for this object..
	do
	{
		fscanf_s(m_fp, "%s", buffer, unsigned int(sizeof(char) * 32));

		if (strcmp(buffer, "3ds") == 0)
		{
			m_import_filetype = FILE_TYPE_3DSMAX;
		}

		if (strcmp(buffer, "Blender") == 0)
		{
			m_import_filetype = FILE_TYPE_BLENDER;
		}
	} while (strcmp(buffer, "mtllib") != 0);

	// this is the .mtl filename
	fscanf_s(m_fp, "%s", buffer, unsigned int(sizeof(char) * 32));

	// store .mtl filename for later use
	strcpy_s(m_mtl_filename, obj_path);
	strcat_s(m_mtl_filename, buffer);

	char meshName[64];

	// read ahead and find out how much data we have
	do
	{
		fscanf_s(m_fp, "%s", buffer, unsigned int(sizeof(char) * 32));

		// If we change from "f" to "v" then we have another mesh object in this file...
		// We need to start counters again for the next mesh object
		if (strcmp(buffer, "v") == 0)m_v_count++;
		if (strcmp(buffer, "vt") == 0)m_vt_count++;
		if (strcmp(buffer, "vn") == 0)m_vn_count++;
		if (strcmp(buffer, "f") == 0)m_f_count++;

	} while (!feof(m_fp));


	// allocate memory required to hold the data
	m_v = new cVec3f[m_v_count];
	m_vt = new sVec2f[m_vt_count];
	m_vn = new cVec3f[m_vn_count];
	m_f = new sFace[m_f_count];

	fseek(m_fp, 0, SEEK_SET);

	// vertices
	for (int i = 0; i < m_v_count; i++)
	{
		do
		{
			fscanf_s(m_fp, "%s", buffer, unsigned int(sizeof(char) * 32));
		} while (strcmp(buffer, "v") != 0);

		fscanf_s(m_fp, "%f%f%f", &m_v[i].x, &m_v[i].y, &m_v[i].z);
	}

	fseek(m_fp, 0, SEEK_SET);

	if (m_import_filetype == FILE_TYPE_3DSMAX)
	{
		// vertex normals
		for (int i = 0; i < m_vn_count; i++)
		{
			do
			{
				fscanf_s(m_fp, "%s", buffer, unsigned int(sizeof(char) * 32));
			} while (strcmp(buffer, "vn") != 0);
			fscanf_s(m_fp, "%f%f%f", &m_vn[i].x, &m_vn[i].y, &m_vn[i].z);
		}

		fseek(m_fp, 0, SEEK_SET);

		float z_coord; // blender supports 3d texture coords... the z is not being stored

		// texture coordinates
		for (int i = 0; i < m_vt_count; i++)
		{
			do
			{
				fscanf_s(m_fp, "%s", buffer, unsigned int(sizeof(char) * 32));
			} while (strcmp(buffer, "vt") != 0);
			fscanf_s(m_fp, "%f%f%f", &m_vt[i].x, &m_vt[i].y, &z_coord);
		}
	}
	else
	{
		// texture coordinates
		for (int i = 0; i < m_vt_count; i++)
		{
			do
			{
				fscanf_s(m_fp, "%s", buffer, unsigned int(sizeof(char) * 32));
			} while (strcmp(buffer, "vt") != 0); 
			fscanf_s(m_fp, "%f%f", &m_vt[i].x, &m_vt[i].y);
		}

		// return back to start of file ready for the big read
		fseek(m_fp, 0, SEEK_SET);

		// vertex normals
		for (int i = 0; i < m_vn_count; i++)
		{
			do
			{
				fscanf_s(m_fp, "%s", buffer, unsigned int(sizeof(char) * 32));
			} while (strcmp(buffer, "vn") != 0);
			fscanf_s(m_fp, "%f%f%f", &m_vn[i].x, &m_vn[i].y, &m_vn[i].z);
		}
	}



	// load in all material data...
 	importMTL();

	// load in face data
	loadFaces();

	createFaceNormalList();
	createFaceList();

	fclose(m_fp);
	m_fp = NULL;
}