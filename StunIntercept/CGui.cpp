
#include <iostream>
#include <memory>
#include <Windows.h>
#include "CGui.h"

#include "CStunIntercept.h"
#include "CSharedComm.h"
#include "CStunChecker.h"
#include "../Shared/ImGui/imgui.h"
#include "../Shared/ImGui/imgui_impl_win32.h"
#include "../Shared/ImGui/imgui_impl_dx11.h"
#include <d3d11.h>
#include <tchar.h>
#include <vector>
#include <mutex>

#include "imgui_memory_editor.h"
#include <format>
#include <chrono>

void CGui::Render() {

    static int page = 0;
    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);

    ImGui::Begin("WebSec - STUN Intercept", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
    static ImVec2 oldSize = ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y);
    ImVec2 size = ImGui::GetContentRegionAvail();
    ImVec2 leftSize(size.x * 0.3f, size.y);
    ImVec2 rightSize(size.x * 0.7f, size.y);

    // Split the window into two columns
    ImGui::Columns(2, nullptr, false);

    // Left side with buttons for navigation
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.f);
        auto btn_size = size.y / 6 - 3;
        ImGui::SetColumnWidth(0, leftSize.x);
        if (ImGui::Button("Interceptor", ImVec2(leftSize.x, btn_size)))
            page = 0;
        if (ImGui::Button("Memory", ImVec2(leftSize.x, btn_size)))
            page = 1;
        if (ImGui::Button("About", ImVec2(leftSize.x, btn_size)))
            page = 2;
        ImGui::PopStyleVar();
    }

    // Right side is empty for now
    ImGui::NextColumn();
    ImGui::SetColumnWidth(1, rightSize.x);

    switch (page)
    {
    case 0:
        DrawInterceptor();
        break;
    case 1:
        DrawMemory();
        break;
    case 2:
        DrawAbout();
        break;
    default:
        break;
    }

    ImGui::End();


}

void CGui::InitImGui() {

}
class executable_meta
{
private:
    uintptr_t m_begin;
    uintptr_t m_end;

public:
    template<typename TReturn, typename TOffset>
    TReturn* getRVA(TOffset rva)
    {
        return (TReturn*)(m_begin + rva);
    }

    explicit executable_meta(void* module)
        : m_begin((uintptr_t)module)
    {
        PIMAGE_DOS_HEADER dosHeader = getRVA<IMAGE_DOS_HEADER>(0);
        PIMAGE_NT_HEADERS ntHeader = getRVA<IMAGE_NT_HEADERS>(dosHeader->e_lfanew);

        m_end = m_begin + ntHeader->OptionalHeader.SizeOfImage;
    }

    executable_meta(uintptr_t begin, uintptr_t end)
        : m_begin(begin), m_end(end)
    {
    }

    inline uintptr_t begin() const { return m_begin; }
    inline uintptr_t end() const { return m_end; }
};

void CGui::DrawMemory()
{
    static executable_meta data = executable_meta(GetModuleHandle(nullptr));

    static MemoryEditor mem_edit;

    static void* main = GetModuleHandle(nullptr);

    static bool hasInit = false;


    mem_edit.DrawContents((void*)data.begin(), data.end() - data.begin(), (size_t)main);
}

void CGui::DrawInterceptor()
{
    static bool Intercepting = false;
    if (!Intercepting) {
        if (ImGui::Button("Start intercepting")) {
            Intercepting = true;

        }
    }
    else {
        if (ImGui::Button("Stop intercepting")) {
            Intercepting = false;
        }
    }

    if (Intercepting) {
        ImGui::Text("Connections list");
        if (ImGui::Button("Clear"))
        {
            mStunIntercept->Clear();
        }
        ImGui::SameLine();

        if (ImGui::BeginTable("table2", 3, 0))
        {

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("Id");
            ImGui::TableNextColumn();
            ImGui::Text("Ip");
            ImGui::TableNextColumn();
            ImGui::Text("Port");

            {
                std::lock_guard<std::recursive_mutex> lock(mStunIntercept->m_Mutex);

                for (int i = 0; i < mStunIntercept->_results.size(); i++)
                {
                    auto& result = mStunIntercept->_results.at(i);
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::Text("%d", i);
                    ImGui::TableNextColumn();
                    ImGui::Text(result.Ip.c_str());
                    ImGui::TableNextColumn();
                    ImGui::Text(result.Port.c_str());
                }
            }
            ImGui::EndTable();
        }
    }
}

void CGui::Shutdown() {
   // intercept->Stop();
}
void CGui::DrawAbout()
{
    static const auto now = std::chrono::system_clock::now();
    static auto year_disc = std::format("All credits reserved to 2020-{:%Y} WebSec B.V.", now);

    ImGui::TextWrapped("DISCLAIMER:");
    ImGui::TextWrapped("While our intetion is to bolster cybersecurity we strongly advocate for the ethical use of such tools. The aim is to unmask malicious actors who misuse platforms like Telegram or Signal, therefore ensuring a safer-digital landscape for all.");
    ImGui::Spacing();
    ImGui::TextWrapped("We strongly condone the use of this software for malicious purposes such as doxing.");
    ImGui::Spacing();
    ImGui::TextWrapped("By using this software you agree that you are aware of your own actions and potential consequences!");
    ImGui::Spacing();
    ImGui::TextWrapped(year_disc.c_str());

    ImGui::TextColored(ImColor(200, 155, 0, 255), "https://websec.nl");

}

CGui::CGui(std::shared_ptr<CSharedComm> _sharedComm, std::shared_ptr<CStunChecker> _stunChecker, std::shared_ptr<CStunIntercept> _stunIntercept) : mSharedComm(_sharedComm), mStunChecker(_stunChecker), mStunIntercept(_stunIntercept)
{
}


void CGui::InitStyle() {
    ImGuiStyle* style = &ImGui::GetStyle();
    ImVec4* colors = style->Colors;



}