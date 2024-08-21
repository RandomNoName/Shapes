#pragma once
#include <functional>
#include "Rectangle/Rectangle.h"
#include "Triangle/Triangle.h"
#include "Ellips/Ellips.h"

struct Engine
{
private:
    ImGuiIO& io = ImGui::GetIO();
    ImVec2 world_size{};
    ImVec2 world_to_screen{};

    std::vector<std::pair<ImVec2*, ImVec2*>> bindings;
    std::vector<std::shared_ptr<Shape>> shapes;
    std::shared_ptr<Shape> current_shape;
    std::string message_buffer;
    std::function<void()> task;
    ImVec2 prev_mouse_pos;
public:
    void draw(ImDrawList& draw_list);
    void update();
private:
    std::vector<std::shared_ptr<Shape>>::iterator find_clicked_shape();
    void delete_shape();
    void drag_shape();
    void resize_shape();

    void start_binding();
    void end_binding();

    void load_file(std::string path);
    void save_file(std::string path);

};
