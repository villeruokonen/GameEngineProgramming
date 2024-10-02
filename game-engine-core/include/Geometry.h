#pragma once

#include <vector>
#include "../include/OpenGLRenderer.h"

class Geometry
{
public:
	struct VERTEX
	{
		VERTEX() :
			x(0.0f), y(0.0f), z(0.0f)
		{
		}

		VERTEX(float _x, float _y, float _z) :
			x(_x), y(_y), z(_z)
		{
		}

		VERTEX(const glm::vec3& pos) :
			x(pos.x), y(pos.y), z(pos.z)
		{
		}

		static constexpr int32_t GetStride() { return sizeof(VERTEX); }

		float x, y, z;
	};
	
	Geometry();
	~Geometry();

	void Clear();

	/**
	 * 
	 * @param radius radius vector
	 * @param offset offset of vertices
	 * @param rings number of rings for polycount
	 * @param segments number of segments for polycount
	 */
	void GenSphere(const glm::vec3& radius,
	               const glm::vec3& offset = glm::vec3(0.0f),
	               uint32_t rings = 24,
	               uint32_t segments = 24);

	/**
	 * Set OpenGL attributes
	 * @param program 
	 */
	void SetAttribs(GLuint program);

	/**
	 * Disable OpenGL attributes
	 * @param program 
	 */
	void DisableAttribs(GLuint program);

	void Draw(IRenderer& renderer);

private:
	std::vector<VERTEX>		m_arrVertices;
	GLenum					m_eDrawMode;
};
