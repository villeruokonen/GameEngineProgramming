#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <string>
#include <vector>
#include <array>
#include <memory>
#include <fstream>
#include <iterator>
#include <string_view>

#include "Timer.h"
#include "IRenderer.h"

class IApplication
{
public:
	IApplication();
	~IApplication();

	bool Create(int32_t resX, int32_t resY, const std::string& title);

	void Run();

	inline int32_t GetWidth() const { return m_iWidth; }
	inline int32_t GetHeight() const { return m_iHeight; }
	inline HWND GetWindow() const { return m_Window; }

	inline bool IsActive() const { return m_bActive;  }
	void SetActive(bool active);

	inline float GetFrameTime() const { return m_Timer.GetElapsedSeconds(); }

	static IApplication* GetApp() { return m_pApp;  }

	inline IRenderer* GetRenderer() { return m_pRenderer.get(); }

	static void Debug(const wchar_t* msg);
	static void Debug(const char* msg);
	static void Debug(const std::string& msg);

protected:
	virtual bool OnEvent(UINT message, WPARAM wParam, LPARAM lParam);

private:
	static HWND MakeWindow(int32_t resX, int32_t resY, const std::string& title);
	static long WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
	int32_t						m_iWidth;
	int32_t						m_iHeight;

	bool						m_bActive;

	Timer						m_Timer;

	HWND						m_Window;

	std::unique_ptr<IRenderer>	m_pRenderer;

	static IApplication*		m_pApp;
};
