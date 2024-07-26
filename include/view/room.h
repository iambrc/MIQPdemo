#pragma once

namespace USTC_CG
{
	class Room
	{
	public:
		Room() = default;

		Room(
			float target_w,
			float target_h)
		{
			target_w_ = target_w;
			target_h_ = target_h;
		}

		virtual ~Room() = default;

		float& get_target_w() { return target_w_; }
		float& get_target_h() { return target_h_; }
		float& get_min_ratio() { return min_ratio; }
		float& get_max_ratio() { return max_ratio; }
		float& get_size_ratio_min_x() { return size_ratio_min_x; }
		float& get_size_ratio_max_x() { return size_ratio_max_x; }
		float& get_size_ratio_min_y() { return size_ratio_min_y; }
		float& get_size_ratio_max_y() { return size_ratio_max_y; }
		bool& get_has_position_constraint() { return has_position_constraint; }
		float& get_constraint_position_x() { return position_constraint_x; }
		float& get_constraint_position_y() { return position_constraint_y; }
		bool& get_has_boundary_constraint_w() { return has_boundary_constraint_w; }
		bool& get_has_boundary_constraint_h() { return has_boundary_constraint_h; }
		bool& get_has_ratio_constraint() { return has_ratio_constraint; }

	private:
		float target_w_ = 0.0f, target_h_ = 0.0f;
		float min_ratio = 1.0f, max_ratio = 1.0f;
		float size_ratio_min_x = 0.5f, size_ratio_max_x = 2.0f;
		float size_ratio_min_y = 0.5f, size_ratio_max_y = 2.0f;
		bool has_position_constraint = false;
		float position_constraint_x = 0.5f, position_constraint_y = 0.5f;
		bool has_boundary_constraint_w = false;
		bool has_boundary_constraint_h = false;
		bool has_ratio_constraint = false;
	};
}