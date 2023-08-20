#pragma once

#include <memory>

class CStunIntercept;
class CSharedComm;
class CWindow;
class CStunChecker;


class CApp
{
public:
	
	CApp();
	~CApp();
	void Run();
	void CreateConsole();
	void DetachConsole();

private:
	std::shared_ptr<CStunIntercept> _mInterceptor;
	std::shared_ptr<CSharedComm> _mSharedComm;
	std::shared_ptr<CStunChecker> _mStunChecker;
	std::shared_ptr<CWindow> _mWindow;
};

