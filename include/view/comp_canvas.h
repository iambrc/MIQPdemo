#pragma once

#include <imgui.h>

#include <memory>
#include <vector>
#include <map>

#include "view/component.h"
#include "view/shape/shape.h"
#include "view/room.h"
#include "view/obstacle.h"

namespace USTC_CG
{
// Canvas class for drawing shapes.
class Canvas : public Component
{
   public:
    // Inherits constructor from Component.
    using Component::Component;

    // Override the draw method from the parent Component class.
    void draw() override;

    // Clears all shapes from the canvas.
    void clear_shape_list();

    // Set canvas attributes (position and size).
    void set_attributes(const ImVec2& min, const ImVec2& size);

    // Controls the visibility of the canvas background.
    void show_background(bool flag);

	// Solve the MIQP problem.
	void solve();

    int room_num = 0, obstacle_num = 0;
    std::vector<std::shared_ptr<Room>> room_list_;
    std::vector<std::shared_ptr<Obstacle>> obstacle_list_;
	float cover_weight = 1.0f, size_weight = 1.0f;
	std::map<int, int> adjacency_constraint;
    // length of doors
    float min_adjacency_length_w = 0.05f, min_adjacency_length_h = 0.05f;

   private:
    // Drawing functions.
    void draw_background();
    void draw_shapes();

    // Canvas attributes.
    ImVec2 canvas_min_;         // Top-left corner of the canvas.
    ImVec2 canvas_max_;         // Bottom-right corner of the canvas.
    ImVec2 canvas_size_;        // Size of the canvas.

    ImVec2 canvas_minimal_size_ = ImVec2(50.f, 50.f);
    ImU32 background_color_ = IM_COL32(50, 50, 50, 255);
    ImU32 border_color_ = IM_COL32(255, 255, 255, 255);

    bool show_background_ = true;  // Controls background visibility.

    // List of shapes drawn on the canvas.
    std::vector<std::shared_ptr<Shape>> shape_list_;
};

}  // namespace USTC_CG
