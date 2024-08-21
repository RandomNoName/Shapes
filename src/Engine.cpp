//=========================//
#include <Windows.h>
#include <commdlg.h>    // file explorer
//=========================//
#include <iostream>
#include <format>
#include "Engine.h"

static std::string open_file_manager()
{
    OPENFILENAME ofn;
    char szFile[260]{};
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = szFile;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "Text Files\0*.TXT\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    auto x = GetOpenFileName(&ofn);
    return ofn.lpstrFile;
}

void Engine::update()
{
    if (task)
    {
        task();
    }
    if (ImGui::BeginMainMenuBar())
    {
        message_buffer = "Shapes: " + std::to_string(shapes.size());
        if (ImGui::BeginMenu(message_buffer.c_str(), false))
        {
            ImGui::EndMenu();
        }
        message_buffer = "Bindings: " + std::to_string(bindings.size());
        if (ImGui::BeginMenu(message_buffer.c_str(), false))
        {
            ImGui::EndMenu();
        }
        if (ImGui::Button("Rectangle"))
        {
            current_shape = std::make_shared<Shape_rect>();
            task = std::bind_front(&Engine::resize_shape, this);
        }
        if (ImGui::Button("Triangle"))
        {
            current_shape = std::make_shared<Shape_triangle>();
            task = std::bind_front(&Engine::resize_shape, this);
        }
        if (ImGui::Button("Ellips"))
        {
            current_shape = std::make_shared<Shape_ellips>();
            task = std::bind_front(&Engine::resize_shape, this);
        }
        if (ImGui::Button("Bind"))
        {
            task = std::bind_front(&Engine::start_binding, this);
        }
        if (ImGui::Button("Move"))
        {
            task = std::bind_front(&Engine::drag_shape, this);
        }
        if (ImGui::Button("Delete"))
        {
            task = std::bind_front(&Engine::delete_shape, this);
        }
        if (ImGui::Button("Load"))
        {
            load_file(open_file_manager());
        }
        if (ImGui::Button("Save"))
        {
            save_file(open_file_manager());
        }
        ImGui::EndMainMenuBar();
    }
}

void Engine::draw(ImDrawList& draw_list)
{
    for (auto& shape : shapes)
    {
        shape->draw(draw_list);
    }
    for (auto& [from, to] : bindings)
    {
        draw_list.AddLine(*from, *to, ImColor{ 150, 150, 150 }, 5);
    }

}

void Engine::drag_shape()
{
    if (io.MouseClicked[0])
    {
        auto it = find_clicked_shape();
        if (it != shapes.end())
        {
            current_shape = *it;
        }
    }
    bool stop = ImGui::IsKeyDown(ImGuiKey_Escape) ||
                ImGui::IsMouseDown(ImGuiMouseButton_Right);
    if (stop || current_shape && io.MouseReleased[0])
    {
        task = nullptr;
        current_shape = nullptr;
    }
    if (io.MouseDown[0] && current_shape)
    {
        SetCursor(::LoadCursor(NULL, IDC_HAND));
        current_shape->move_shape();
    }
}

void Engine::delete_shape()
{
    SetCursor(::LoadCursor(NULL, IDC_NO));
    if (io.MouseClicked[0])
    {
        auto it = find_clicked_shape();
        if (it != shapes.end())
        {
            auto to_erase = std::remove_if(bindings.begin(), bindings.end(), [center = &it->get()->center](std::pair<ImVec2*, ImVec2*> from_to)
                {
                    return from_to.first == center || from_to.second == center;
                });
            bindings.erase(to_erase, bindings.end());
            shapes.erase(it);
            task = nullptr;
        }
    }
}

std::vector<std::shared_ptr<Shape>>::iterator Engine::find_clicked_shape()
{
    return std::find_if(shapes.begin(), shapes.end(), [](const std::shared_ptr<Shape>& element)
        {
            return element->is_clicked(ImGui::GetMousePos());
        });;
}

void Engine::resize_shape()
{
    bool right_button = io.MouseClicked[1];
    if (right_button || ImGui::IsKeyDown(ImGuiKey_Escape))
    {
        shapes.pop_back();
        task = nullptr;
    } 
    if (io.MouseClicked[0])
    {
        current_shape->set_on_mouse_pos();
        shapes.push_back(current_shape);
        prev_mouse_pos = io.MousePos;
    }
    if (io.MouseDown[0])
    {
        SetCursor(::LoadCursor(NULL, IDC_SIZENWSE));
        current_shape->scale_shape(prev_mouse_pos);
    }
    if (io.MouseReleased[0])
    {
        task = nullptr;
    }

}

void Engine::start_binding()
{
    if (io.MouseClicked[0])
    {
        auto it = find_clicked_shape();
        if (it != shapes.end())
        {
            bindings.emplace_back(&it->get()->center, &io.MousePos);
            task = std::bind_front(&Engine::end_binding, this);
        }
    }
}

void Engine::end_binding()
{
    bool stop = ImGui::IsKeyDown(ImGuiKey_Escape) || io.MouseClicked[1];
    if (io.MouseClicked[0])
    {
        auto it = find_clicked_shape();
        if (it != shapes.end())
        {
            auto center = &it->get()->center;
            if (center != bindings.back().first && center != bindings.back().second)
            {
                bindings.back().second = &it->get()->center;
                task = nullptr;
                return;
            }
        }
        stop = true;
    }
    if (stop)
    {
        bindings.pop_back();
        task = nullptr;
    }
}

void Engine::load_file(std::string path)
{
    Shape_register::registrate<Shape_triangle>();
    Shape_register::registrate<Shape_ellips>();
    Shape_register::registrate<Shape_rect>();

    std::vector<std::shared_ptr<Shape>> new_shapes;
    std::ifstream file(path);

    std::istringstream stream;
    std::string word;
    while (std::getline(file, word))
    {
        stream.str(word);
        stream >> word;
        if (Shape_register::is_registered(word))
        {
            new_shapes.push_back(Shape_register::get_registered(word, stream));
        }
        else if (stream.str() == "bindings:" && new_shapes.size())
        {
            stream >> word;
            while (std::getline(file, word))
            {
                stream.clear();
                stream.str(word);
                size_t start;
                size_t to;
                stream >> start >> to;
                bindings.emplace_back(&new_shapes[start]->center, &new_shapes[to]->center);
            }
        }
        else
            break;
        stream.clear();
    }
    if (new_shapes.empty())
    {
        task = [this]()
            {
                ImGui::OpenPopup("ERROR");
                if (ImGui::BeginPopupModal("ERROR", 0, ImGuiWindowFlags_AlwaysAutoResize))
                {
                    ImGui::Text("%s", "Shapeless file...");
                    ImGui::Separator();
                    if (ImGui::Button("OK"))
                    {
                        ImGui::CloseCurrentPopup();
                        task = nullptr;
                    }
                    ImGui::EndPopup();
                }
            };
        return;
    }
    shapes = std::move(new_shapes);
    Shape_register::clear();
}

void Engine::save_file(std::string path)
{
    std::ofstream file(path);
    for (auto& shape : shapes)
    {
        shape->serialize(file);
    }
    file << "bindings:\n";
    using ptr_t = const std::shared_ptr<Shape>;
    for (auto [from, to] : bindings)
    {
        auto it = std::find_if(shapes.begin(), shapes.end(), [from](ptr_t& ptr)
            {
                return &ptr->center == from;
            });
        file << (it - shapes.begin()) << " ";
        it = std::find_if(shapes.begin(), shapes.end(), [&to](ptr_t& ptr)
            {
                return &ptr->center == to;
            });
        file << (it - shapes.begin()) << "\n";
    }
}
