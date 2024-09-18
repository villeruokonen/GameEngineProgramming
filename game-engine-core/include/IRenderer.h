#pragma once

#include "../glm-master/glm/glm.hpp"
#include "../glm-master/glm/gtc/matrix_transform.hpp"
#include "../glm-master/glm/gtc/matrix_transform.hpp"
#include "../glm-master/glm/gtc/random.hpp"
#include <string_view>

class IRenderer
{
public:
	IRenderer()
	{
	}

	virtual bool Create() = 0;
	virtual void Flip() = 0;
	virtual void Clear(float r, float g, float b, float a, float depth = 1.0f, int32_t stencil = 0) = 0;
	void Clear(const glm::vec4& color, float depth = 1.0f, int32_t stencil = 0)
	{
		Clear(color.r, color.b, color.g, color.a, depth, stencil);
	}

	virtual void SetViewport(const glm::ivec4& area) = 0;
	virtual bool SetTexture(uint32_t program, uint32_t texture, uint32_t slot, const std::string_view& uniformName) = 0;
};
