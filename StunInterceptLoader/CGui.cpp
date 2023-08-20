
#include <iostream>
#include <memory>
#include <Windows.h>
#include "CGui.h"

#include "../Shared/ImGui/imgui.h"
#include "../Shared/ImGui/imgui_impl_win32.h"
#include "../Shared/ImGui/imgui_impl_dx11.h"
#include <d3d11.h>
#include <tchar.h>
#include <vector>
#include <mutex>
#include "CInjector.h"
#include <future>

#define VERSION "v1.0.2"

void CGui::Render() {

    static int page = 0;
    static int last_item = 0;
    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);

    ImGui::Begin("WebSec - STUN Loader Intercept", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
    static ImVec2 oldSize = ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y);
    ImVec2 size = ImGui::GetContentRegionAvail();
    ImVec2 btnSize(size.x * 0.3f, size.y * 0.2);

    static std::string statusMessage = "";
    static bool Good = false;

    static const char* items[] = { "Telegram", "Signal", "Session" };
    static int item_current = 0;

    
  

    ImGui::Combo("Target", &item_current, items, IM_ARRAYSIZE(items));
    {
        std::lock_guard guard(m_Mtx);
        for (auto& res : m_results) {
            ImGui::PushStyleColor(ImGuiCol_Text, res.status ? IM_COL32(80, 200, 120, 255) : IM_COL32(128, 128, 255, 255));

            ImGui::TextWrapped(res.msg.c_str());
            ImGui::PopStyleColor();
        }
    }
    auto winSize = ImGui::GetWindowSize();
    ImGui::SetCursorPos(ImVec2{ winSize.x - btnSize.x * 2 - 20, winSize.y - btnSize.y - 10 });
    if (ImGui::Button("Exit", btnSize))
    {
        exit(0);
    }
    ImGui::SameLine();
    if (ImGui::Button("Load", btnSize))
    {
        m_results.clear();

        std::async(std::launch::async, [this] {
            CInjector injector;
            std::string target(items[item_current]);
            target.append(".exe");

            std::lock_guard guard(m_Mtx);
            m_results.push_back(injector.Inject(target.c_str(), "STUNIntercept.dll"));
            });
       
        
       
    }
    ImGui::SameLine();
    ImGui::SetCursorPosY(winSize.y - 25);
    ImGui::SetCursorPosX(10);
    ImGui::TextColored(ImColor(255, 155, 230, 200), VERSION);

   
    ImGui::End();

    if (last_item != item_current) {
        m_results.clear();
    }

    last_item = item_current;
}

void CGui::InitImGui() {

}



void CGui::Shutdown() {
    // intercept->Stop();
}


CGui::CGui() 
{

}


void CGui::InitStyle() {
    ImGuiStyle* style = &ImGui::GetStyle();
    ImVec4* colors = style->Colors;



}