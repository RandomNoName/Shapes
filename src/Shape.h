#pragma once
#include <unordered_map>
#include <algorithm>
#include <string>
#include <vector>
#include <memory>

#include <sstream>      // istringstream
#include <fstream>

#include <imgui.h> // Axis Y goes from top to bot in this api

template<class Class, class Ret, class... Args>
using mfn_ptr = Ret(Class::*)(Args...);

template<class Ret, class... Args>
using fn_ptr = Ret(*)(Args...);

struct Shape
{
    static constexpr ImColor shape_color{ 100, 100, 150 };

    inline virtual void serialize(std::ofstream& file) const = 0;

    inline virtual void draw(ImDrawList& draw_list) const = 0;

    inline virtual void move_shape() = 0;
    inline virtual void scale_shape(ImVec2 prev_pos) = 0;
    inline virtual void set_on_mouse_pos() = 0;

    inline virtual bool is_clicked(ImVec2 pos) const = 0;

    virtual ~Shape() = default;

    ImVec2 center{ 600, 600 };
};

class Shape_register
{
public:
    inline static std::shared_ptr<Shape> get_registered(const std::string& shape_type, std::istringstream& line)
    {
        return factory.find(shape_type)->second(line);
    }

    inline static bool is_registered(const std::string& shape_type)
    {
        return factory.find(shape_type) != factory.end();
    }

    template<class Derived>
    inline static void registrate()
    {
        factory[Derived::shape_type] = &Derived::deserialize;
    }

    inline static void clear()
    {
        factory.clear();
    }
    using deserializer = std::shared_ptr<Shape>(*)(std::istringstream&);
private:
    inline static std::unordered_map<std::string, deserializer> factory;
};
