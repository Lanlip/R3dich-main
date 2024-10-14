#include "overlay.hpp"
#include <imgui.h>
#include <algorithm>
#include <string>

OverlayText g_overlayText;

OverlayText::OverlayText()
    : isCenterActive(false), centerDuration(5.0f), notificationSpacing(40.0f) {}

void OverlayText::ShowCenter(const char* message, float duration) {
    centerText = message;
    centerStartTime = std::chrono::steady_clock::now();
    isCenterActive = true;
    centerDuration = duration;
}

void OverlayText::ShowNotification(const char* message, float duration) {
    notifications.push_back({ std::string(message), std::chrono::steady_clock::now(), duration });
}

void OverlayText::SetNotificationSpacing(float spacing) {
    notificationSpacing = spacing;
}

void OverlayText::Render() {
    ImGuiIO& io = ImGui::GetIO();
    ImVec2 screenSize = io.DisplaySize;

    // Render center text
    if (isCenterActive) {
        auto currentTime = std::chrono::steady_clock::now();
        float elapsedTime = std::chrono::duration<float>(currentTime - centerStartTime).count();
        if (elapsedTime > centerDuration) {
            isCenterActive = false;
        }
        else {
            ImVec2 textSize = ImGui::CalcTextSize(centerText.c_str());
            ImVec2 textPos(
                (screenSize.x - textSize.x) * 0.5f,
                (screenSize.y - textSize.y) * 0.5f
            );
            ImGui::SetNextWindowPos(textPos);
            ImGui::SetNextWindowBgAlpha(0.0f);
            ImGui::Begin("CenterOverlay", nullptr,
                ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove |
                ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing |
                ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoInputs);
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 255));
            ImGui::Text("%s", centerText.c_str());
            ImGui::PopStyleColor();
            ImGui::End();
        }
    }

    // Render notifications
    float yOffset = 15.0f;
    float xOffset = screenSize.x * 0.90f;
    auto it = notifications.begin();
    while (it != notifications.end()) {
        auto currentTime = std::chrono::steady_clock::now();
        float elapsedTime = std::chrono::duration<float>(currentTime - it->startTime).count();
        if (elapsedTime > it->duration) {
            it = notifications.erase(it);
        }
        else {
            ImVec2 textSize = ImGui::CalcTextSize(it->text.c_str());
            ImVec2 textPos(xOffset - textSize.x, yOffset);  // 오른쪽 정렬
            ImGui::SetNextWindowPos(textPos);
            ImGui::SetNextWindowBgAlpha(0.5f);
            ImGui::Begin(("Notification" + std::to_string(std::distance(notifications.begin(), it))).c_str(), nullptr,
                ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove |
                ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing |
                ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoInputs);
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 255));
            ImGui::Text("%s", it->text.c_str());
            ImGui::PopStyleColor();
            ImGui::End();
            yOffset += textSize.y + notificationSpacing;
            ++it;
        }
    }
}

void ShowCenterMessage(const char* message, float duration) {
    g_overlayText.ShowCenter(message, duration);
}

void ShowNotification(const char* message, float duration) {
    g_overlayText.ShowNotification(message, duration);
}