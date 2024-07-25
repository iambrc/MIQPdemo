#include <imgui.h>
#include "view/shape/rect.h"

namespace USTC_CG
{
// Draw the rectangle using ImGui
void Rect::draw() const
{
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    draw_list->AddRectFilled(
        ImVec2(
            config.bias[0] + start_point_x_, config.bias[1] + start_point_y_),
        ImVec2(config.bias[0] + end_point_x_, config.bias[1] + end_point_y_),
        ImU32(color_),
        0.f,  // No rounding of corners
        ImDrawFlags_None);
}

}  // namespace USTC_CG
