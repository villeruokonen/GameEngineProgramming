#pragma once
#include "../game-engine-core/include/IApplication.h"

class BowlingApp :
    public IApplication
{
public:
    BowlingApp();

	bool OnCreate() override;
	void OnDestroy() override;
	void OnUpdate(float frametime) override;
	void OnDraw(IRenderer& renderer) override;

private:
	bool OnKeyDown(uint32_t keyCode) override;

};

