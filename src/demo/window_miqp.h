#pragma once

#include <memory>
#include <vector>

#include "view/window.h"
#include "view/comp_canvas.h"

namespace USTC_CG
{
class MIQP : public Window
{
   public:
    explicit MIQP(const std::string& window_name);
    ~MIQP();

    void draw();

   private:
    void draw_canvas();

    std::shared_ptr<Canvas> p_canvas_ = nullptr;

    bool flag_show_canvas_view_ = true;
	bool flag_show_room_view_ = true;
};
}  // namespace USTC_CG