#pragma once
#include<vector>

namespace USTC_CG
{
class Shape
{
   public:
    // Draw Settings
    struct Config
    {
        // Offset to convert canvas position to screen position
        float bias[2] = { 0.f, 0.f };
    };
    /*
    * Setting a config for each shape so that we can change their line_color 
    * and line_thickness easily.
    */
    Config config;

   public:
    virtual ~Shape() = default;

    virtual void draw() const = 0;
};
}  // namespace USTC_CG