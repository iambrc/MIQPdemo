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
            float end_point_y)
            : start_point_x_(start_point_x),
            start_point_y_(start_point_y),
            end_point_x_(end_point_x),
            end_point_y_(end_point_y)
        {
            int R = rand() % 256; int G = rand() % 256; int B = rand() % 256;
            color_ = ImColor(R, G, B);
        }

        virtual ~Rect() = default;

        // Draws the rectangle on the screen
        // Overrides draw function to implement rectangle-specific drawing logic
        void draw() const override;

    private:
        // Coordinates of the top-left and bottom-right corners of the rectangle
        float start_point_x_ = 0.0f, start_point_y_ = 0.0f;
        float end_point_x_ = 0.0f, end_point_y_ = 0.0f;
        ImColor color_;
    };
}  // namespace USTC_CG
