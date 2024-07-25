#include "window_miqp.h"

#include <iostream>
#include <string>

namespace USTC_CG
{
MIQP::MIQP(const std::string& window_name) : Window(window_name)
{
    p_canvas_ = std::make_shared<Canvas>("Cmpt.Canvas");
}

MIQP::~MIQP()
{
}

void MIQP::draw()
{
    draw_canvas();
}

void MIQP::draw_canvas()
{
    // Set a full screen canvas view
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    if (ImGui::Begin(
            "Canvas",
            &flag_show_canvas_view_,
            ImGuiWindowFlags_NoDecoration|ImGuiWindowFlags_NoBackground))
    {
        if (ImGui::Button("Solve") && p_canvas_->room_num > 0)
        {
            p_canvas_->solve();
			std::cout << "Solve" << std::endl;
        }
        ImGui::SliderFloat("cover weight", &p_canvas_->cover_weight, 0.0, 10.0);
		ImGui::SliderFloat("size weight", &p_canvas_->size_weight, 0.0, 10.0);

        // Set the canvas to fill the rest of the window
        const auto& canvas_min = ImGui::GetCursorScreenPos();
        const auto& canvas_size = ImGui::GetContentRegionAvail();
        p_canvas_->set_attributes(canvas_min, canvas_size);
        p_canvas_->draw();
    }
    ImGui::End();

    if (ImGui::Begin("Add Room", &flag_show_room_view_))
    {
        if (ImGui::Button("Add Room") && p_canvas_->room_num < 9)
        {
			p_canvas_->room_num++;
			p_canvas_->room_list_.push_back(std::make_shared<Room>());
			std::cout << "room number:" << p_canvas_->room_num << std::endl;
        }
		for (int i = 1; i <= p_canvas_->room_num; i++)
        {
            std::string room_w = "target width " + std::to_string(i);
            std::string room_h = "target height " + std::to_string(i);
            char c1[20], c2[20];
            strcpy_s(c1, room_w.c_str());
            strcpy_s(c2, room_h.c_str());
            ImGui::SliderFloat(c1, &p_canvas_->room_list_[i - 1]->get_target_w(), 0.0, 1.0);
            ImGui::SliderFloat(c2, &p_canvas_->room_list_[i - 1]->get_target_h(), 0.0, 1.0);
		}
        //for (int i = 1; i <= room_num; ++i)
		//	std::cout << "room " << i << " width:" << p_canvas_->room_list_[i - 1]->get_target_w() << " height:" << p_canvas_->room_list_[i - 1]->get_target_h() << std::endl;
    }
	ImGui::End();

    if (ImGui::Begin("Add Obstacle", &flag_show_room_view_))
    {
        if (ImGui::Button("Add Obstacle") && p_canvas_->obstacle_num < 4)
        {
            p_canvas_->obstacle_num++;
            p_canvas_->obstacle_list_.push_back(std::make_shared<Obstacle>());
            std::cout << "obstacle number:" << p_canvas_->obstacle_num << std::endl;
        }
        for (int i = 1; i <= p_canvas_->obstacle_num; i++)
        {
            std::string obstacle_x = "target position x " + std::to_string(i);
            std::string obstacle_y = "target position y " + std::to_string(i);
            std::string obstacle_w = "target width " + std::to_string(i);
            std::string obstacle_h = "target height " + std::to_string(i);
            char c1[30], c2[30], c3[20], c4[20];
            strcpy_s(c1, obstacle_x.c_str());
            strcpy_s(c2, obstacle_y.c_str());
            strcpy_s(c3, obstacle_w.c_str());
			strcpy_s(c4, obstacle_h.c_str());
            ImGui::SliderFloat(c1, &p_canvas_->obstacle_list_[i - 1]->get_position_x(), 0.0, 1.0);
            ImGui::SliderFloat(c2, &p_canvas_->obstacle_list_[i - 1]->get_position_y(), 0.0, 1.0);
			ImGui::SliderFloat(c3, &p_canvas_->obstacle_list_[i - 1]->get_w(), 0.0, 1.0);
			ImGui::SliderFloat(c4, &p_canvas_->obstacle_list_[i - 1]->get_h(), 0.0, 1.0);
        }
    }
    ImGui::End();
}
}  // namespace USTC_CG