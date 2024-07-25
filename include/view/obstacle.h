#pragma once

namespace USTC_CG
{
	class Obstacle
	{
	public:
		Obstacle() = default;

		Obstacle(
			float position_x,
			float position_y,
			float w,
			float h)
		{
			position_x_ = position_x;
			position_y_ = position_y;
			w_ = w;
			h_ = h;
		}

		virtual ~Obstacle() = default;

		float& get_position_x() { return position_x_; }
		float& get_position_y() { return position_y_; }
		float& get_w() { return w_; }
		float& get_h() { return h_; }
	private:
		float position_x_ = 0.0f, position_y_ = 0.0f;
		float w_ = 0.0f, h_ = 0.0f;
	};
}