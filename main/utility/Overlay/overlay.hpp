#ifndef OVERLAY_HPP
#define OVERLAY_HPP

#include <string>
#include <chrono>
#include <vector>

class OverlayText {
private:
    struct Notification {
        std::string text;
        std::chrono::steady_clock::time_point startTime;
        float duration;
    };
    std::string centerText;
    std::chrono::steady_clock::time_point centerStartTime;
    bool isCenterActive;
    float centerDuration;
    float notificationSpacing;
    std::vector<Notification> notifications;

public:
    OverlayText();
    void ShowCenter(const char* message, float duration = 5.0f);
    void ShowNotification(const char* message, float duration = 5.0f);
    void Render();
    void SetNotificationSpacing(float spacing);
};

extern OverlayText g_overlayText;

void ShowCenterMessage(const char* message, float duration = 5.0f);
void ShowNotification(const char* message, float duration = 5.0f);

#endif // OVERLAY_HPP