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
		//ImGui::SliderFloat("min adjacency length", &p_canvas_->min_adjacency_length_w, 0.0, 1.0);
		//ImGui::SliderFloat("min adjacency length", &p_canvas_->min_adjacency_length_h, 0.0, 1.0);

        // Set the canvas to fill the rest of the window
        const auto& canvas_min = ImGui::GetCursorScreenPos();
        const auto& canvas_size = ImGui::GetContentRegionAvail();
		//const auto& canvas_size = ImVec2(800, 800);
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

    if (ImGui::Begin("Aspect ratio constraint", &flag_show_aspect_ratio_))
    {
		ImGui::Text("Aspect ratio constraint: max ratio must larger than min ratio!\n If you want to add constaint, please ensure min ratio >= 1");
        for (int i = 1; i <= p_canvas_->room_num; i++)
        {
			std::string has_ratio_constraint = "ratio constraint " + std::to_string(i);
            std::string min_ratio = "min ratio " + std::to_string(i);
			std::string max_ratio = "max ratio " + std::to_string(i);
            char c1[20], c2[20], c3[40];
            strcpy_s(c1, min_ratio.c_str());
			strcpy_s(c2, max_ratio.c_str());
			strcpy_s(c3, has_ratio_constraint.c_str());
			ImGui::Checkbox(c3, &p_canvas_->room_list_[i - 1]->get_has_ratio_constraint());
            ImGui::SliderFloat(c1, &p_canvas_->room_list_[i - 1]->get_min_ratio(), 1.0, 5.0);
			ImGui::SliderFloat(c2, &p_canvas_->room_list_[i - 1]->get_max_ratio(), 1.0, 5.0);
        }
    }
    ImGui::End();

    if (ImGui::Begin("Size Control constraint", &flag_show_size_ratio_))
    {
        ImGui::Text("Size Control ratio of each room");
        for (int i = 1; i <= p_canvas_->room_num; i++)
        {
            std::string min_ratio_x = "min size ratio of x " + std::to_string(i);
            std::string max_ratio_x = "max size ratio of x" + std::to_string(i);
            std::string min_ratio_y = "min size ratio of y " + std::to_string(i);
            std::string max_ratio_y = "max size ratio of y" + std::to_string(i);
            char c1[30], c2[30], c3[30], c4[30];
            strcpy_s(c1, min_ratio_x.c_str());
            strcpy_s(c2, max_ratio_x.c_str());
            strcpy_s(c3, min_ratio_y.c_str());
            strcpy_s(c4, max_ratio_y.c_str());
            ImGui::SliderFloat(c1, &p_canvas_->room_list_[i - 1]->get_size_ratio_min_x(), 0.1, 1.0);
            ImGui::SliderFloat(c2, &p_canvas_->room_list_[i - 1]->get_size_ratio_max_x(), 1.0, 10.0);
            ImGui::SliderFloat(c3, &p_canvas_->room_list_[i - 1]->get_size_ratio_min_y(), 0.1, 1.0);
            ImGui::SliderFloat(c4, &p_canvas_->room_list_[i - 1]->get_size_ratio_max_y(), 1.0, 10.0);
        }
    }
    ImGui::End();

	if (ImGui::Begin("Position Constraint", &flag_show_position_constraint_))
	{
		ImGui::Text("Position Constraint");
		for (int i = 1; i <= p_canvas_->room_num; i++)
		{
			std::string has_position_constraint = "has position constraint " + std::to_string(i);
			std::string position_x = "position x " + std::to_string(i);
			std::string position_y = "position y " + std::to_string(i);
			char c1[40], c2[20], c3[20];
			strcpy_s(c2, position_x.c_str());
			strcpy_s(c3, position_y.c_str());
			strcpy_s(c1, has_position_constraint.c_str());
			ImGui::Checkbox(c1, &p_canvas_->room_list_[i - 1]->get_has_position_constraint());
			ImGui::SliderFloat(c2, &p_canvas_->room_list_[i - 1]->get_constraint_position_x(), 0.0, 1.0);
			ImGui::SliderFloat(c3, &p_canvas_->room_list_[i - 1]->get_constraint_position_y(), 0.0, 1.0);
		}
	}
	ImGui::End();

    if (ImGui::Begin("Adjacency Constraint", &flag_show_adjacency_constraint_))
    {
        ImGui::SliderInt("Adjacency index 1", &ad_idx1, 0, 10);
		ImGui::SliderInt("Adjacency index 2", &ad_idx2, 0, 10);
        if (ImGui::Button("Add Adjacency Constraint") && ad_idx1 >=1 && ad_idx1 <= p_canvas_->room_num &&
            ad_idx2 >= 1 && ad_idx2 <= p_canvas_->room_num && ad_idx1 != ad_idx2)
		{
			p_canvas_->adjacency_constraint.insert(std::pair<int, int>(ad_idx1, ad_idx2));
            std::cout << "Adjacency constraint of room " << ad_idx1 << "and room " << ad_idx2 << "has been added!" << std::endl;
        }
		if (ImGui::Button("Clear Adjacency Constraint"))
        {
			p_canvas_->adjacency_constraint.clear();
			std::cout << "All adjacency constraints have been cleared!" << std::endl;
		}
        ImGui::SliderFloat("min adjacency length of width", &p_canvas_->min_adjacency_length_w, 0.0, 1.0);
        ImGui::SliderFloat("min adjacency length of height", &p_canvas_->min_adjacency_length_h, 0.0, 1.0);
		ImGui::SliderFloat("min adjacency ratio of decomposed rooms", &p_canvas_->decompose_ratio, 0.0, 1.0);
    }
    ImGui::End();

    if (ImGui::Begin("Boundary constraint", &flag_show_boundary_constraint_))
    {
		for (int i = 1; i <= p_canvas_->room_num; i++)
        {
			std::string has_boundary_constraint_w = "boundary constraint width " + std::to_string(i);
			std::string has_boundary_constraint_h = "boundary constraint height " + std::to_string(i);
			char c1[50], c2[50];
			strcpy_s(c1, has_boundary_constraint_w.c_str());
			strcpy_s(c2, has_boundary_constraint_h.c_str());
			ImGui::Checkbox(c1, &p_canvas_->room_list_[i - 1]->get_has_boundary_constraint_w());
            ImGui::SameLine();
			ImGui::Checkbox(c2, &p_canvas_->room_list_[i - 1]->get_has_boundary_constraint_h());
		}
    }
    ImGui::End();

    if (ImGui::Begin("Sub-domain Optimization", &flag_show_sub_domain_))
    {
		ImGui::Checkbox("Enable selecting Sub-domain", &p_canvas_->enable_selection);
        if (ImGui::Button("Solve Sub-domain"))
        {
			p_canvas_->solve_sub_domain();
            std::cout << "Solve sub domain" << std::endl;
        }
        ImGui::SliderFloat("Refinement Range", &p_canvas_->refinement_range, 0.0, 0.1);
    }
	ImGui::End();
}
}  // namespace USTC_CG