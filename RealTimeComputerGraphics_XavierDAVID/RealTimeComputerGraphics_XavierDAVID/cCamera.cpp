#include "cCamera.h"


/////////////////////////////////////////////////////////////////////////////////////
// constructor
/////////////////////////////////////////////////////////////////////////////////////
cCamera::cCamera()
{
	projectionMatrix = glm::perspective(glm::radians(m_fov), (float)800.0f / (float)600.0f, m_near, m_far);
		
	m_pos   = vec3(0.0f, 0.0f, 7.5);
	m_rot - vec3(0.0f, 0.0f, 0.0f);
}

/////////////////////////////////////////////////////////////////////////////////////
// destructor
/////////////////////////////////////////////////////////////////////////////////////
cCamera::~cCamera()
{
}

/////////////////////////////////////////////////////////////////////////////////////
// init() - 
/////////////////////////////////////////////////////////////////////////////////////
void cCamera::init(float screenWidth, float screenHeight)
{
	float aspect_ratio = screenWidth / screenHeight;
	projectionMatrix = glm::perspective(glm::radians(m_fov), aspect_ratio, m_near, m_far);
}


/////////////////////////////////////////////////////////////////////////////////////
// update() - 
/////////////////////////////////////////////////////////////////////////////////////
void cCamera::update()
{
    /*mat4 r    = glm::rotate(mat4(1.0f), glm::radians(-m_rot.z), vec3(0.0f, 0.0f, 1.0f));
    r        = glm::rotate(r, glm::radians(-m_rot.y), vec3(0.0f, 1.0f, 0.0f));
    r        = glm::rotate(r, glm::radians(-m_rot.x), vec3(1.0f, 0.0f, 0.0f));
    mat4 t    = glm::translate(mat4(1.0f), vec3(-m_pos));*/

    float yaw = radians(m_rot.y - 90.0f);
    float pitch = radians(m_rot.x);

    m_dir.x = cos(yaw) * cos(pitch);
    m_dir.y = sin(pitch);
    m_dir.z = sin(yaw) * cos(pitch);

    viewMatrix = glm::lookAt(vec3(m_pos), vec3(m_pos + m_dir), vec3(0.0f, 1.0f, 0.0f));
}