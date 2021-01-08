// Add this to your imconfig.h

#include <glm/glm.hpp>

#include "imgui-glfw_export.h"

#define IM_VEC2_CLASS_EXTRA                                              \
     ImVec2(const glm::vec2& v) {                                        \
         x = v.x;                                                        \
         y = v.y;                                                        \
     }                                                                   \
                                                                         \
     operator glm::vec2() const {                                        \
         return glm::vec2(x, y);                                         \
     }

 #define IM_VEC4_CLASS_EXTRA                                             \
     ImVec4(const glm::vec4 & c)                                         \
         : ImVec4(c.r, c.g, c.b, c.a) {                                  \
     }                                                                   \
     operator glm::vec4() const {                                        \
         return glm::vec4(                                               \
             x,                                                          \
             y,                                                          \
             z,                                                          \
             w);                                                         \
     }

#define ImTextureID unsigned int