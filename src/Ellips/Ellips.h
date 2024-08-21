#pragma once
#include "../Shape.h"

struct Shape_ellips final : Shape
{
public:
    inline static constexpr const char* shape_type = "ellips";

    Shape_ellips(ImVec2 new_center, float width, float height) noexcept
        : major_axis(width)
        , minor_axis(height)
    {
        center = new_center;
    }

    Shape_ellips() noexcept {}

    static std::shared_ptr<Shape> deserialize(std::istringstream& line);

    inline void serialize(std::ofstream& file)  const override;

    inline void draw(ImDrawList& draw_list) const override;

    inline void scale_shape(ImVec2 prev_pos) override;

    inline void move_shape() override;

    inline bool is_clicked(ImVec2 pos) const override;

    void set_on_mouse_pos() override;
private:
    float major_axis = 100.0f;
    float minor_axis = 50.0f;
};
