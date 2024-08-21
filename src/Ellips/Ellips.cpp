#include "Ellips.h"

std::shared_ptr<Shape> Shape_ellips::deserialize(std::istringstream& line)
{
    ImVec2 new_center;
    float width, height;
    line >> new_center.x
        >> new_center.y
        >> width
        >> height;
    return std::make_shared<Shape_ellips>(new_center, width, height);
}

void Shape_ellips::serialize(std::ofstream& file)  const
{
    file << shape_type << " "
        << center.x << " "
        << center.y << " "
        << major_axis << " "
        << minor_axis << "\n";
}

void Shape_ellips::draw(ImDrawList& draw_list) const
{
    draw_list.AddEllipseFilled(center, ImVec2{ major_axis, minor_axis }, shape_color);
}

void Shape_ellips::scale_shape(ImVec2 prev_pos)
{
    float diff_x = ImGui::GetMousePos().x - prev_pos.x;
    float diff_y = ImGui::GetMousePos().y - prev_pos.y;

    center.x = ImGui::GetMousePos().x - major_axis;
    center.y = ImGui::GetMousePos().y - minor_axis;

    major_axis = diff_x;
    minor_axis = diff_y;
}

void Shape_ellips::move_shape()
{
    center = ImGui::GetMousePos();
}

bool Shape_ellips::is_clicked(ImVec2 pos) const
{
    float diff_x = pos.x - center.x;
    float diff_y = center.y - pos.y;
    return std::pow(diff_x / major_axis, 2) + std::pow(diff_y / minor_axis, 2) <= 1;
}

void Shape_ellips::set_on_mouse_pos()
{
    center = ImGui::GetMousePos();
}
