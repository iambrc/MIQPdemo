#include <imgui.h>
#include "view/shape/rect.h"

namespace USTC_CG
{
// Draw the rectangle using ImGui
void Rect::draw() const
{
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    draw_list->AddRectFilled(
        ImVec2(start_point_x_, start_point_y_),
        ImVec2(end_point_x_, end_point_y_),
        ImU32(color_),
        0.f,  // No rounding of corners
        ImDrawFlags_None);
}

}  // namespace USTC_CG
