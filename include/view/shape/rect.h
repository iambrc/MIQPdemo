#pragma once

#include "shape.h"
#include <cstdlib>
#include <time.h>

namespace USTC_CG
{
    class Rect : public Shape
    {
    public:
        Rect() = default;

        // Initialize a rectangle with start and end points
        Rect(
            float start_point_x,
            float start_point_y,
            float end_point_x,
            float end_point_y,
            int flag)
            : start_point_x_(start_point_x),
            start_point_y_(start_point_y),
            end_point_x_(end_point_x),
            end_point_y_(end_point_y),
			type(flag)
        {
            if (flag == 0)
				color_ = ImColor(255, 255, 255, 255);
            else if (flag == 1)
				color_ = ImColor(255, 0, 0, 255);
            else if (flag == 2)
				color_ = ImColor(0, 255, 0, 255);
			else if (flag == 3)
				color_ = ImColor(0, 0, 255, 255);
            else if (flag == 4)
				color_ = ImColor(255, 255, 0, 255);
			else if (flag == 5)
				color_ = ImColor(255, 0, 255, 255);
			else if (flag == 6)
				color_ = ImColor(0, 255, 255, 255);
			else if (flag == 7)
				color_ = ImColor(128, 128, 128, 255);
            else if (flag == 8)
				color_ = ImColor(255, 128, 0, 255);
			else if (flag == 9)
				color_ = ImColor(0, 255, 128, 255);
			else
				color_ = ImColor(128, 0, 255, 255);
        }

        virtual ~Rect() = default;

        // Draws the rectangle on the screen
        // Overrides draw function to implement rectangle-specific drawing logic
        void draw() const override;

    private:
        // Coordinates of the top-left and bottom-right corners of the rectangle
        float start_point_x_ = 0.0f, start_point_y_ = 0.0f;
        float end_point_x_ = 0.0f, end_point_y_ = 0.0f;
        int type = 0;
        ImColor color_;
    };
}  // namespace USTC_CG
