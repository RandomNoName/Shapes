#include "Triangle.h"

std::shared_ptr<Shape> Shape_triangle::deserialize(std::istringstream& line)
{
    ImVec2 new_a;
    ImVec2 new_b;
    ImVec2 new_c;
    line >> new_a.x
        >> new_a.y
        >> new_b.x
        >> new_b.y
        >> new_c.x
        >> new_c.y;
    return std::make_shared<Shape_triangle>(new_a, new_b, new_c);
}

void Shape_triangle::serialize(std::ofstream& file) const
{
    file    << shape_type   << " "
            << a.x          << " "
            << a.y          << " "
            << b.x          << " "
            << b.y          << " "
            << c.x          << " "
            << c.y          << "\n";
}

void Shape_triangle::draw(ImDrawList& draw_list) const
{
    draw_list.AddTriangleFilled(a, b, c, shape_color);
}

void Shape_triangle::scale_shape(ImVec2 prev_pos)
{
    b.x += ImGui::GetMousePos().x - b.x;
    c.x += ImGui::GetMousePos().x - c.x;
    a.y += ImGui::GetMousePos().y - a.y;
    c.y += ImGui::GetMousePos().y - c.y;
    center = calc_center(a, b, c);
}

void Shape_triangle::move_shape()
{
    float diff_x = ImGui::GetMousePos().x - center.x;
    float diff_y = ImGui::GetMousePos().y - center.y;
    center = ImGui::GetMousePos();
    a.x += diff_x;
    b.x += diff_x;
    c.x += diff_x;
    a.y += diff_y;
    b.y += diff_y;
    c.y += diff_y;
}

bool Shape_triangle::is_clicked(ImVec2 pos) const
{
    auto sign = [](ImVec2 p1, ImVec2 p2, ImVec2 p3)
        {
            float f1x = (p1.x - p3.x);
            float f2x = (p2.x - p3.x);
            float f1y = (p1.y - p3.y);
            float f2y = (p2.y - p3.y);
            return f1x * f2y - f1y * f2x;
        };
    float d1 = sign(pos, a, b);
    float d2 = sign(pos, b, c);
    float d3 = sign(pos, c, a);
    bool has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    bool has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);
    return !(has_neg && has_pos);
}

void Shape_triangle::set_on_mouse_pos()
{
    a = b = c = ImGui::GetMousePos();
}

