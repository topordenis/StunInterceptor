#pragma once

class CStunIntercept;
class CStunChecker;
class CSharedComm;

class CGui {
public:

	HWND mWindow;
	void Render();
	void InitImGui();
	void InitStyle();

	void Shutdown();

	void DrawMemory();
	void DrawInterceptor();
	void DrawAbout();
	std::shared_ptr<CSharedComm> mSharedComm;
	std::shared_ptr<CStunChecker> mStunChecker;
	std::shared_ptr<CStunIntercept> mStunIntercept;

	CGui(std::shared_ptr<CSharedComm> _sharedComm, std::shared_ptr<CStunChecker> _stunChecker, std::shared_ptr<CStunIntercept> _stunIntercept); 
};