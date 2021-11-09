#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#define GL_GLEXT_PROTOTYPES
#define EGL_EGLEXT_PROTOTYPES
#else
#define IMGUI_IMPL_OPENGL_LOADER_CUSTOM "examples/libs/gl3w/GL/gl3w.h"
#endif
#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>

#include "backends/imgui_impl_opengl2.cpp"
#include "backends/imgui_impl_glfw.cpp"
#include "misc/cpp/imgui_stdlib.cpp"
#include "misc/freetype/imgui_freetype.cpp"

#include "imgui.h"
#include "imgui-glfw.h"
#include <map>



namespace ImGui
{
    namespace GLFW
    {
        int g_UnloadTextureInterval = 1;
        std::map<void*, std::pair<int, ImTextureID>> g_TexturesCache;

        GLuint GetNativeTexture(const GLFWimage& texture) {
            auto&& found = g_TexturesCache.find(texture.pixels);
            GLuint gl_texture = 0;
            if (found != g_TexturesCache.end()) {
                found->second.first = g_UnloadTextureInterval;
                gl_texture = (GLuint)(intptr_t)found->second.second;
                return gl_texture;
            }
            glGenTextures(1, &gl_texture);
            glBindTexture(GL_TEXTURE_2D, gl_texture);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
#ifdef GL_UNPACK_ROW_LENGTH
            glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.width, texture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.pixels);
            glBindTexture(GL_TEXTURE_2D, gl_texture);
            g_TexturesCache[texture.pixels] = std::make_pair(g_UnloadTextureInterval, (ImTextureID)(intptr_t)gl_texture);
            return gl_texture;
        }

        bool Init(GLFWwindow* window) {
            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
#ifdef __EMSCRIPTEN__
            bool err = false;
#else
            bool err = glfwInit() != 0;
#endif
            if (err)
            {
                fprintf(stderr, "Failed to initialize OpenGL loader!\n");
                return false;
            }
            if (!ImGui_ImplGlfw_InitForOpenGL(window, true)) {
                fprintf(stderr, "Failed to initialize imgui OpenGL!\n");
                return false;
            }
            if (!ImGui_ImplOpenGL2_Init()) {
                fprintf(stderr, "Failed to initialize imgui!\n");
                return false;
            }
            return true;
        }
        void UpdateFontTexture() {
            ImGui_ImplOpenGL2_Data* bd = ImGui_ImplOpenGL2_GetBackendData();
            if (bd->FontTexture) {
                ImGui_ImplOpenGL2_DestroyFontsTexture();
            }
            ImGui_ImplOpenGL2_CreateFontsTexture();
        }
        void NewFrame() {
            ImGui_ImplOpenGL2_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
        }
        void Render(GLFWwindow* window) {
            ImGui::Render();
            int display_w, display_h;
            glfwGetFramebufferSize(window, &display_w, &display_h);
            glViewport(0, 0, display_w, display_h);
            glClearColor(0, 0, 0, 0);
            glClear(GL_COLOR_BUFFER_BIT);
            ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
            auto&& texture = g_TexturesCache.begin();
            while (texture != g_TexturesCache.end()) {
                texture->second.first--;
                if (texture->second.first < 0) {
                    GLuint gl_texture = (GLuint)(intptr_t)texture->second.second;
                    glDeleteTextures(1, &gl_texture);
                    texture = g_TexturesCache.erase(texture);
                }
                else {
                    ++texture;
                }
            }
        }
        void Shutdown() {
            ImGui_ImplOpenGL2_Shutdown();
            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext();
        }
        void SetTextureUnloadInterval(int updateCount) {
            g_UnloadTextureInterval = updateCount;
        }
    }
    // Image overloads
    void Image(const GLFWimage& texture,
        const glm::vec4& tintColor,
        const glm::vec4& borderColor) {
        Image(texture, glm::vec2(texture.width, texture.height), tintColor, borderColor);
    }
    void Image(const GLFWimage& texture, const glm::vec2& size,
        const glm::vec4& tintColor,
        const glm::vec4& borderColor) {
        ImTextureID textureID =
            (ImTextureID)GLFW::GetNativeTexture(texture);
        ImGui::Image(textureID, size, ImVec2(0, 0), ImVec2(1, 1), tintColor,
            borderColor);
    }

    // ImageButton overloads
    bool ImageButton(const GLFWimage& texture, const int framePadding,
        const glm::vec4& bgColor,
        const glm::vec4& tintColor) {
        return ImageButton(texture, glm::vec2(texture.width, texture.height), framePadding, bgColor, tintColor);
    }
    bool ImageButton(const GLFWimage& texture, const glm::vec2& size, const int framePadding,
        const glm::vec4& bgColor,
        const glm::vec4& tintColor) {
        ImTextureID textureID =
            (ImTextureID)GLFW::GetNativeTexture(texture);
        return ImGui::ImageButton(textureID, size, ImVec2(0, 0), ImVec2(1, 1), framePadding, bgColor,
            tintColor);
    }

    // Draw_list overloads. All positions are in relative coordinates (relative to top-left of the current window)
    void DrawLine(const glm::vec2& a, const glm::vec2& b, const glm::vec4& col, float thickness) {
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        glm::vec2 pos = ImGui::GetCursorScreenPos();
        draw_list->AddLine(a + pos, b + pos, ColorConvertFloat4ToU32(col),
            thickness);
    }
}
