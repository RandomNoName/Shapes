#pragma once
#include "../Shape.h"

struct Shape_rect final : Shape
{
public:
    inline static constexpr const char* shape_type = "rect";

    Shape_rect(ImVec2 min_xy, ImVec2 max_xy) noexcept
        : min_xy(min_xy)
        , max_xy(max_xy)
    {
        center = calc_center(min_xy, max_xy);
    }

    Shape_rect() noexcept {}

    static std::shared_ptr<Shape> deserialize(std::istringstream& line);

    inline void serialize(std::ofstream& file) const override;

    inline void draw(ImDrawList& draw_list) const override;

    inline void scale_shape(ImVec2 prev_pos) override;

    inline void move_shape() override;

    inline bool is_clicked(ImVec2 pos) const override;

    void set_on_mouse_pos() override;

    static ImVec2 calc_center(ImVec2 a, ImVec2 b)
    {
        return { (a.x + b.x) / 2, (a.y + b.y) / 2 };
    }
private:
    ImVec2 min_xy;
    ImVec2 max_xy;
};
