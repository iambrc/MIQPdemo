#include "view/comp_canvas.h"

#include <cmath>
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>

#include "imgui.h"
#include "view/shape/shape.h"
#include "view/shape/rect.h"
#include "gurobi//include/gurobi_c++.h"

namespace USTC_CG
{
void Canvas::draw()
{
    draw_background();

    draw_shapes();
}

void Canvas::set_attributes(const ImVec2& min, const ImVec2& size)
{
    canvas_min_ = min;
    canvas_size_ = size;
    canvas_minimal_size_ = size;
    canvas_max_ =
        ImVec2(canvas_min_.x + canvas_size_.x, canvas_min_.y + canvas_size_.y);
}

void Canvas::show_background(bool flag)
{
    show_background_ = flag;
}

void Canvas::clear_shape_list()
{
    shape_list_.clear();
}

void Canvas::draw_background()
{
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    if (show_background_)
    {
        // Draw background recrangle
        draw_list->AddRectFilled(canvas_min_, canvas_max_, background_color_);
        // Draw background border
        draw_list->AddRect(canvas_min_, canvas_max_, border_color_);
    }
    /// Invisible button over the canvas to capture mouse interactions.
    ImGui::SetCursorScreenPos(canvas_min_);
    ImGui::InvisibleButton(
        label_.c_str(), canvas_size_, ImGuiButtonFlags_MouseButtonLeft);

}

void Canvas::draw_shapes()
{
    // Shape::Config s = { .bias = { canvas_min_.x, canvas_min_.y } };
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    // ClipRect can hide the drawing content outside of the rectangular area
    draw_list->PushClipRect(canvas_min_, canvas_max_, true);
    for (const auto& shape : shape_list_)
    {
        shape->config.bias[0] = canvas_min_.x;
        shape->config.bias[1] = canvas_min_.y;
        shape->draw();
    }
    draw_list->PopClipRect();
}

void Canvas::solve()
{
    shape_list_.clear();
    try {
        GRBEnv env = GRBEnv();
        GRBModel model = GRBModel(env);
        // Create variables
        // basic variables
        std::vector<GRBVar> x_i(room_num),y_i(room_num),w_i(room_num),h_i(room_num);
        for (int i = 1; i <= room_num; ++i)
        {
			std::string x_name = "x_" + std::to_string(i);
			std::string y_name = "y_" + std::to_string(i);
			std::string w_name = "w_" + std::to_string(i);
			std::string h_name = "h_" + std::to_string(i);
            char x_[5], y_[5], w_[5], h_[5];
			strcpy_s(x_, x_name.c_str());
			strcpy_s(y_, y_name.c_str());
			strcpy_s(w_, w_name.c_str());
			strcpy_s(h_, h_name.c_str());
			x_i[i-1] = model.addVar(0.0, 1.0, 0.0, GRB_CONTINUOUS, x_);
			y_i[i-1] = model.addVar(0.0, 1.0, 0.0, GRB_CONTINUOUS, y_);
			w_i[i-1] = model.addVar(0.0, 1.0, 0.0, GRB_CONTINUOUS, w_);
			h_i[i-1] = model.addVar(0.0, 1.0, 0.0, GRB_CONTINUOUS, h_);
        }
		// non-overlap variables
		std::vector<std::vector<GRBVar>> sigma_R(room_num, std::vector<GRBVar>(room_num)),
            sigma_L(room_num, std::vector<GRBVar>(room_num)), 
            sigma_F(room_num, std::vector<GRBVar>(room_num)),
            sigma_B(room_num, std::vector<GRBVar>(room_num));

        for (int i = 1; i <= room_num; ++i)
        {
            for (int j = 1; j < i; ++j)
            {
				sigma_R[i - 1][j - 1] = model.addVar(0, 1, 0, GRB_BINARY);
				sigma_L[i - 1][j - 1] = model.addVar(0, 1, 0, GRB_BINARY);
				sigma_F[i - 1][j - 1] = model.addVar(0, 1, 0, GRB_BINARY);
				sigma_B[i - 1][j - 1] = model.addVar(0, 1, 0, GRB_BINARY);
            }
        }
		std::vector<std::vector<GRBVar>> room_obstacle_R(room_num, std::vector<GRBVar>(obstacle_num)),
			room_obstacle_L(room_num, std::vector<GRBVar>(obstacle_num)),
			room_obstacle_F(room_num, std::vector<GRBVar>(obstacle_num)),
			room_obstacle_B(room_num, std::vector<GRBVar>(obstacle_num));
        for (int i = 1; i <= room_num; ++i)
        {
            for (int j = 1; j <= obstacle_num; ++j)
            {
				room_obstacle_R[i - 1][j - 1] = model.addVar(0, 1, 0, GRB_BINARY);
				room_obstacle_L[i - 1][j - 1] = model.addVar(0, 1, 0, GRB_BINARY);
				room_obstacle_F[i - 1][j - 1] = model.addVar(0, 1, 0, GRB_BINARY);
				room_obstacle_B[i - 1][j - 1] = model.addVar(0, 1, 0, GRB_BINARY);
            }
        }
		// adjacency variables
		std::vector<std::vector<GRBVar>> adjacency(room_num, std::vector<GRBVar>(room_num));
		for (int i = 1; i <= room_num; ++i)
		{
			for (int j = 1; j < i; ++j)
			{
				adjacency[i - 1][j - 1] = model.addVar(0, 1, 0, GRB_BINARY);
			}
		}
		// aspect ratio variables
		std::vector<GRBVar> aspect_ratio(room_num);
        for (int i = 1; i <= room_num; ++i)
        {
			aspect_ratio[i - 1] = model.addVar(0, 1, 0, GRB_BINARY);
        }
		// boundary variables
		std::vector<std::vector<GRBVar>> boundary(room_num, std::vector<GRBVar>(4 * (1 + obstacle_num)));
        for (int i = 1; i <= room_num; ++i)
        {
            for (int j = 1; j <= 4 * (1 + obstacle_num); ++j)
            {
				boundary[i - 1][j - 1] = model.addVar(0, 1, 0, GRB_BINARY);
            }
        }
        // Set objective
        GRBQuadExpr obj = 0;
        for (int i = 1; i <= room_num; i++)
        {
			obj = obj - cover_weight * (w_i[i - 1] * h_i[i - 1]);
			obj = obj + size_weight * ((w_i[i - 1] - room_list_[i - 1]->get_target_w()) * 
                (w_i[i - 1] - room_list_[i - 1]->get_target_w()) + 
                (h_i[i - 1] - room_list_[i - 1]->get_target_h()) * 
                (h_i[i - 1] - room_list_[i - 1]->get_target_h()));
        }
        model.setObjective(obj, GRB_MINIMIZE);
        // Add linear constraint:
        // Inside constraint
		for (int i = 1; i <= room_num; ++i)
		{
			model.addConstr(x_i[i - 1] + w_i[i - 1] <= 1);
			model.addConstr(y_i[i - 1] + h_i[i - 1] <= 1);
		}
		// Non-Overlap constraint
        for (int i = 1; i <= room_num; ++i)
        {
            for (int j = 1; j < i; ++j)
            {
				model.addConstr(x_i[i - 1] - w_i[j - 1] >= x_i[j - 1] - (1 - sigma_R[i - 1][j - 1]) * (1 + canvas_size_.y / canvas_size_.x));
				model.addConstr(x_i[i - 1] + w_i[i - 1] <= x_i[j - 1] + (1 - sigma_L[i - 1][j - 1]) * (1 + canvas_size_.y / canvas_size_.x));
				model.addConstr(y_i[i - 1] - h_i[j - 1] >= y_i[j - 1] - (1 - sigma_F[i - 1][j - 1]) * (1 + canvas_size_.x / canvas_size_.y));
				model.addConstr(y_i[i - 1] + h_i[i - 1] <= y_i[j - 1] + (1 - sigma_B[i - 1][j - 1]) * (1 + canvas_size_.x / canvas_size_.y));
                model.addConstr(sigma_R[i - 1][j - 1] + sigma_L[i - 1][j - 1] + sigma_F[i - 1][j - 1] + sigma_B[i - 1][j - 1] >= 1);
            }
        }
		for (int i = 1; i <= room_num; ++i)
		{
			for (int j = 1; j <= obstacle_num; ++j)
			{
                model.addConstr(x_i[i - 1] - obstacle_list_[j - 1]->get_w() >= obstacle_list_[j - 1]->get_position_x() - (1 - room_obstacle_R[i - 1][j - 1]) * (1 + canvas_size_.y / canvas_size_.x));
                model.addConstr(x_i[i - 1] + w_i[i - 1] <= obstacle_list_[j - 1]->get_position_x() + (1 - room_obstacle_L[i - 1][j - 1]) * (1 + canvas_size_.y / canvas_size_.x));
				model.addConstr(y_i[i - 1] - obstacle_list_[j - 1]->get_h() >= obstacle_list_[j - 1]->get_position_y() - (1 - room_obstacle_F[i - 1][j - 1]) * (1 + canvas_size_.x / canvas_size_.y));
				model.addConstr(y_i[i - 1] + h_i[i - 1] <= obstacle_list_[j - 1]->get_position_y() + (1 - room_obstacle_B[i - 1][j - 1]) * (1 + canvas_size_.x / canvas_size_.y));
				model.addConstr(room_obstacle_R[i - 1][j - 1] + room_obstacle_L[i - 1][j - 1] + room_obstacle_F[i - 1][j - 1] + room_obstacle_B[i - 1][j - 1] >= 1);
			}
		}
        // Size Control
        for (int i = 1; i <= room_num; ++i)
        {
            model.addConstr(x_i[i - 1] >= room_list_[i - 1]->get_size_ratio_min_x() * room_list_[i - 1]->get_target_w());
			model.addConstr(x_i[i - 1] <= room_list_[i - 1]->get_size_ratio_max_x() * room_list_[i - 1]->get_target_w());
			model.addConstr(y_i[i - 1] >= room_list_[i - 1]->get_size_ratio_min_y() * room_list_[i - 1]->get_target_h());
            model.addConstr(y_i[i - 1] <= room_list_[i - 1]->get_size_ratio_max_y() * room_list_[i - 1]->get_target_h());
        }
        
		// Aspect ratio constraint
        for (int i = 1; i <= room_num; ++i)
        {
            if (room_list_[i - 1]->get_has_ratio_constraint())
            {
                model.addConstr(room_list_[i - 1]->get_min_ratio()* h_i[i - 1] * canvas_size_.y <= w_i[i - 1] * canvas_size_.x + (canvas_size_.x + canvas_size_.y) * aspect_ratio[i - 1]);
                model.addConstr(room_list_[i - 1]->get_max_ratio()* h_i[i - 1] * canvas_size_.y >= w_i[i - 1] * canvas_size_.x - (canvas_size_.x + canvas_size_.y) * aspect_ratio[i - 1]);
                model.addConstr(room_list_[i - 1]->get_min_ratio()* w_i[i - 1] * canvas_size_.x <= h_i[i - 1] * canvas_size_.y + (canvas_size_.x + canvas_size_.y) * (1 - aspect_ratio[i - 1]));
                model.addConstr(room_list_[i - 1]->get_max_ratio()* w_i[i - 1] * canvas_size_.x >= h_i[i - 1] * canvas_size_.y - (canvas_size_.x + canvas_size_.y) * (1 - aspect_ratio[i - 1]));
            }
        }
		// Position constraint
        for (int i = 1; i <= room_num; ++i)
        {
			if (room_list_[i - 1]->get_has_position_constraint())
            {
				model.addConstr(x_i[i - 1] <= room_list_[i - 1]->get_constraint_position_x());
				model.addConstr(x_i[i - 1] + w_i[i - 1] >= room_list_[i - 1]->get_constraint_position_x());
				model.addConstr(y_i[i - 1] <= room_list_[i - 1]->get_constraint_position_y());
				model.addConstr(y_i[i - 1] + h_i[i - 1] >= room_list_[i - 1]->get_constraint_position_y());
			}
        }
		// Adjacency constraint
        for (auto it = adjacency_constraint.begin(); it != adjacency_constraint.end(); ++it)
        {
			int idx1 = std::max(it->first, it->second) - 1, idx2 = std::min(it->first, it->second) - 1;
            model.addConstr(x_i[idx1] <= x_i[idx2] + w_i[idx2] - min_adjacency_length_w * adjacency[idx1][idx2]);
			model.addConstr(x_i[idx1] + w_i[idx1] >= x_i[idx2] + min_adjacency_length_w * adjacency[idx1][idx2]);
			model.addConstr(y_i[idx1] <= y_i[idx2] + h_i[idx2] - min_adjacency_length_h * (1 - adjacency[idx1][idx2]));
			model.addConstr(y_i[idx1] + h_i[idx1] >= y_i[idx2] + min_adjacency_length_h * (1 - adjacency[idx1][idx2]));
        }
		// Boundary constraint
        for (int i = 1; i <= room_num; ++i)
        {
            float x1 = 0.f, x2 = 0.f, y1 = 0.f, y2 = 0.f;
            if (room_list_[i - 1]->get_has_boundary_constraint_w())
            {
                for (int k = 2;k <= 4 * (1 + obstacle_num); k += 2)
                {
                    if (k == 2)
                    {
                        y1 = 1.f; x2 = 1.f; x1 = 0.f;
                        model.addConstr(y_i[i - 1] + h_i[i - 1] >= y1 - (1 - boundary[i - 1][k - 1]) * (1 + canvas_size_.x / canvas_size_.y));
                    }
                    else if (k == 4)
                    {
                        y1 = 0.f; x2 = 1.f; x1 = 0.f;
                        model.addConstr(y_i[i - 1] <= y1 + (1 - boundary[i - 1][k - 1]) * (1 + canvas_size_.x / canvas_size_.y));
                    } 
                    else
                    {
                        int ob_idx = std::floor(k / 4) - 1;
                        if (k % 4 == 2)
                        {
                            x1 = obstacle_list_[ob_idx]->get_position_x();
                            x2 = obstacle_list_[ob_idx]->get_position_x() + obstacle_list_[ob_idx]->get_w();
                            y1 = obstacle_list_[ob_idx]->get_position_y() + obstacle_list_[ob_idx]->get_h();
                            model.addConstr(y_i[i - 1] + h_i[i - 1] >= y1 + (1 - boundary[i - 1][k - 1]) * (1 + canvas_size_.x / canvas_size_.y));
                        }
                        else if (k % 4 == 4)
                        {
                            x1 = obstacle_list_[ob_idx]->get_position_x();
                            x2 = obstacle_list_[ob_idx]->get_position_x() + obstacle_list_[ob_idx]->get_w();
                            y1 = obstacle_list_[ob_idx]->get_position_y();
                            model.addConstr(y_i[i - 1] <= y1 - (1 - boundary[i - 1][k - 1]) * (1 + canvas_size_.x / canvas_size_.y));
                        }
                    }
                    model.addConstr(x_i[i - 1] <= x2 - w_i[i - 1] + (1 - boundary[i - 1][k - 1]) * (1 + canvas_size_.y / canvas_size_.x));
                    model.addConstr(x_i[i - 1] >= x1 - (1 - boundary[i - 1][k - 1]) * (1 + canvas_size_.y / canvas_size_.x));
                }
                GRBLinExpr sum_boundary = 0;
                for (int j = 2; j <= 4 * (1 + obstacle_num); j += 2)
                {
                    sum_boundary += boundary[i - 1][j - 1];
                }
                model.addConstr(sum_boundary >= 1);
            }
            x1 = 0.f, x2 = 0.f, y1 = 0.f, y2 = 0.f;
			if (room_list_[i - 1]->get_has_boundary_constraint_h())
            {
                for (int k = 1; k <= 4 * (1 + obstacle_num); k += 2)
                {
                    if (k == 1)
                    {
                        x1 = 0.f; y2 = 1.f; y1 = 0.f;
						model.addConstr(x_i[i - 1] <= x1 + (1 - boundary[i - 1][k - 1]) * (1 + canvas_size_.y / canvas_size_.x));
                    } 
                    else if (k == 3)
                    {
                        x1 = 1.f; y2 = 1.f; y1 = 0.f;
                        model.addConstr(x_i[i - 1] + w_i[i - 1] >= x1 - (1 - boundary[i - 1][k - 1]) * (1 + canvas_size_.y / canvas_size_.x));
                    }
                    else
                    {
                        int ob_idx = std::floor(k / 4) - 1;
                        if (k % 4 == 1)
                        {
                            x1 = obstacle_list_[ob_idx]->get_position_x();
                            y1 = obstacle_list_[ob_idx]->get_position_y();
                            y2 = obstacle_list_[ob_idx]->get_position_y() + obstacle_list_[ob_idx]->get_h();
                            model.addConstr(x_i[i - 1] <= x1 + (1 - boundary[i - 1][k - 1]) * (1 + canvas_size_.y / canvas_size_.x));
                        }
                        else if (k % 4 == 3)
                        {
                            x1 = obstacle_list_[ob_idx]->get_position_x() + obstacle_list_[ob_idx]->get_w();
                            y1 = obstacle_list_[ob_idx]->get_position_y();
                            y2 = obstacle_list_[ob_idx]->get_position_y() + obstacle_list_[ob_idx]->get_h();
                            model.addConstr(x_i[i - 1] + w_i[i - 1] >= x1 - (1 - boundary[i - 1][k - 1]) * (1 + canvas_size_.y / canvas_size_.x));
                        }
                    }
                    model.addConstr(y_i[i - 1] >= y1 - (1 - boundary[i - 1][k - 1]) * (1 + canvas_size_.x / canvas_size_.y));
					model.addConstr(y_i[i - 1] + h_i[i - 1] <= y2 + (1 - boundary[i - 1][k - 1]) * (1 + canvas_size_.x / canvas_size_.y));
                }
                GRBLinExpr sum_boundary = 0;
                for (int j = 1; j <= 4 * (1 + obstacle_num); j += 2)
                {
                    sum_boundary += boundary[i - 1][j - 1];
                }
                model.addConstr(sum_boundary >= 1);
			}
        }
		// More constraints TODO

        // Optimize model
        model.optimize();

        // Draw rect
		for (int i = 1; i <= room_num; ++i)
		{
			float x = x_i[i - 1].get(GRB_DoubleAttr_X);
			float y = y_i[i - 1].get(GRB_DoubleAttr_X);
			float w = w_i[i - 1].get(GRB_DoubleAttr_X);
			float h = h_i[i - 1].get(GRB_DoubleAttr_X);
			std::cout << x << " " << y << " " << w << " " << h << std::endl;

			shape_list_.push_back(std::make_shared<Rect>(
                canvas_min_.x + x * canvas_size_.x, canvas_min_.y + y * canvas_size_.y, 
                canvas_min_.x + x * canvas_size_.x + w * canvas_size_.x, 
                canvas_min_.y + y * canvas_size_.y + h * canvas_size_.y,
                i));
            std::cout << room_num << std::endl;
		}
        for (int i = 1; i <= obstacle_num; ++i)
        {
			shape_list_.push_back(std::make_shared<Rect>(
				canvas_min_.x + obstacle_list_[i - 1]->get_position_x() * canvas_size_.x,
				canvas_min_.y + obstacle_list_[i - 1]->get_position_y() * canvas_size_.y,
				canvas_min_.x + obstacle_list_[i - 1]->get_position_x() * canvas_size_.x + obstacle_list_[i - 1]->get_w() * canvas_size_.x,
				canvas_min_.y + obstacle_list_[i - 1]->get_position_y() * canvas_size_.y + obstacle_list_[i - 1]->get_h() * canvas_size_.y, 
                0));
        }
        std::cout << "Value of objective function: " << model.get(GRB_DoubleAttr_ObjVal) << std::endl;
    }
    catch (GRBException e) {
        std::cout << "Error code = " << e.getErrorCode() << std::endl;
        std::cout << e.getMessage() << std::endl;
    }
    catch (...) {
        std::cout << "Exception during optimization" << std::endl;
    }
}


}  // namespace USTC_CG