#include "Rectangle.h"

std::shared_ptr<Shape> Shape_rect::deserialize(std::istringstream& line)
{
    ImVec2 new_left_bot_xy;
    ImVec2 new_right_top_xy;
    line >> new_left_bot_xy.x
        >> new_left_bot_xy.y
        >> new_right_top_xy.x
        >> new_right_top_xy.y;
    return std::make_shared<Shape_rect>(new_left_bot_xy, new_right_top_xy);
}

void Shape_rect::serialize(std::ofstream& file) const
{
    file    << shape_type       << " "
            << min_xy.x    << " "
            << min_xy.y    << " "
            << max_xy.x   << " "
            << max_xy.y   << "\n";
}

void Shape_rect::draw(ImDrawList& draw_list) const
{
    draw_list.AddRectFilled(min_xy, max_xy, shape_color);
}

void Shape_rect::scale_shape(ImVec2 prev_pos)
{
    float diff_x = ImGui::GetMousePos().x - max_xy.x;
    float diff_y = ImGui::GetMousePos().y - min_xy.y;
    max_xy.x += diff_x;
    min_xy.y += diff_y;
    center = calc_center(min_xy, max_xy);
}

void Shape_rect::move_shape()
{
    float diff_x = ImGui::GetMousePos().x - center.x;
    float diff_y = ImGui::GetMousePos().y - center.y;
    min_xy.x   += diff_x;
    max_xy.x  += diff_x;

    min_xy.y   += diff_y;
    max_xy.y  += diff_y;

    center.x += diff_x;
    center.y += diff_y;
}

bool Shape_rect::is_clicked(ImVec2 pos) const
{
    return pos.x >= min_xy.x
        && pos.y <= min_xy.y
        && pos.x <= max_xy.x
        && pos.y >= max_xy.y;
}

void Shape_rect::set_on_mouse_pos()
{
    max_xy = min_xy = ImGui::GetMousePos();
    center = calc_center(min_xy, max_xy);
}
