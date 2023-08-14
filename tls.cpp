// Actively use embind to avoid extra copying
// https://emscripten.org/docs/porting/connecting_cpp_and_javascript/embind.html
#include <emscripten/bind.h>
#include <emscripten/val.h>

namespace em = ::emscripten;

enum class Color {
  NONE = 0,
  RED,
  GREEN,
  YELLOW,
};

enum class Arrow {
  NONE = 0,
  LEFT,
  RIGHT,
};

struct Bulb {
  float x = 0;
  float y = 0;
  bool blinking = false;  
  Arrow arrow = Arrow::NONE;
  Color color = Color::NONE;
};

static Color GetRed() noexcept { return Color::RED; }

static Bulb GetBulb() noexcept { return Bulb{}; }

static em::val GetBulbs() noexcept {
  std::vector<Bulb> arr = {
    {0, 0, false, Arrow::NONE, Color::NONE},
    {1, 1, true, Arrow::LEFT, Color::RED},
    {2, 2, false, Arrow::RIGHT, Color::GREEN},
    {3, 3, true, Arrow::NONE, Color::YELLOW},
  };
  return em::val::array(arr);
}

EMSCRIPTEN_BINDINGS(tls) {
  em::enum_<Color>("Color")
    .value("NONE", Color::NONE)
    .value("RED", Color::RED)
    .value("GREEN", Color::GREEN)
    .value("YELLOW", Color::YELLOW);

  em::enum_<Arrow>("Arrow")
    .value("NONE", Arrow::NONE)
    .value("LEFT", Arrow::LEFT)
    .value("RIGHT", Arrow::RIGHT);

  em::value_object<Bulb>("Bulb")
    .field("x", &Bulb::x)
    .field("y", &Bulb::y)
    .field("blinking", &Bulb::blinking)
    .field("arrow", &Bulb::arrow)
    .field("color", &Bulb::color);

  em::function("getRed", &GetRed);
  em::function("getBulb", &GetBulb);
  em::function("getBulbs", &GetBulbs);
}

// int main() {
//   // Main should not be run during TypeScript generation.
//   abort();
//   return 0;
// }
