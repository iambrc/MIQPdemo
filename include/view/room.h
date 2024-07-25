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

	private:
		float target_w_ = 0.0f, target_h_ = 0.0f;
	};
}