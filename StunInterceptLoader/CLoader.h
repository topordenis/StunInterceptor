#pragma once
#include <memory>

class CWindow;
class CLoader
{
public:
	void Run();
	std::shared_ptr<CWindow> m_Window;
};

