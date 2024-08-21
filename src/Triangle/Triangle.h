#pragma once
#include "../Shape.h"

struct Shape_triangle final : Shape
{
public:
    inline static constexpr const char* shape_type = "triangle";

    Shape_triangle(ImVec2 a, ImVec2 b, ImVec2 c) noexcept
        : a(a)
        , b(b)
        , c(c)
    {
        center = calc_center(a, b, c);
    }

    Shape_triangle() noexcept {}

    static std::shared_ptr<Shape> deserialize(std::istringstream& line);

    inline void serialize(std::ofstream& file) const override;

    inline void draw(ImDrawList& draw_list) const override;

    inline void scale_shape(ImVec2 prev_pos) override;

    inline void move_shape() override;

    inline bool is_clicked(ImVec2 pos) const override;

    static ImVec2 calc_center(ImVec2 a, ImVec2 b, ImVec2 c)
    {
        return { (a.x + b.x + c.x) / 3, (a.y + b.y + c.y) / 3 };
    }

    void set_on_mouse_pos() override;
private:
    ImVec2 a, b, c;
};
