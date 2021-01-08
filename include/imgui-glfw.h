#ifndef IMGUI_GLFW_H
#define IMGUI_GLFW_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "imgui-glfw_export.h"

namespace ImGui
{
    namespace GLFW
    {
        IMGUI_GLFW_API bool Init(GLFWwindow* window);
        IMGUI_GLFW_API void UpdateFontTexture();

        IMGUI_GLFW_API void NewFrame();

        IMGUI_GLFW_API void Render(GLFWwindow* window);

        IMGUI_GLFW_API void SetTextureUnloadInterval(int updateCount);

        IMGUI_GLFW_API void Shutdown();
    }

    // custom ImGui widgets for SFML stuff

    // Image overloads
    IMGUI_GLFW_API void Image(const GLFWimage& texture,
        const glm::vec4& tintColor = glm::vec4(1),
        const glm::vec4& borderColor = glm::vec4(0));
    IMGUI_GLFW_API void Image(const GLFWimage& texture, const glm::vec2& size,
        const glm::vec4& tintColor = glm::vec4(1),
        const glm::vec4& borderColor = glm::vec4(0));

    // ImageButton overloads
    IMGUI_GLFW_API bool ImageButton(const GLFWimage& texture, const int framePadding = -1,
        const glm::vec4& bgColor = glm::vec4(0),
        const glm::vec4& tintColor = glm::vec4(1));
    IMGUI_GLFW_API bool ImageButton(const GLFWimage& texture, const glm::vec2& size, const int framePadding = -1,
        const glm::vec4& bgColor = glm::vec4(0),
        const glm::vec4& tintColor = glm::vec4(1));

    // Draw_list overloads. All positions are in relative coordinates (relative to top-left of the current window)
    IMGUI_GLFW_API void DrawLine(const glm::vec2& a, const glm::vec2& b, const glm::vec4& col, float thickness = 1.0f);
}

#endif //# IMGUI_GLFW_H
