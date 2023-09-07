// Actively use embind to avoid extra copying
// https://emscripten.org/docs/porting/connecting_cpp_and_javascript/embind.html
#include <emscripten/bind.h>
#include <emscripten/val.h>

namespace em = ::emscripten;

namespace emscripten {
namespace internal {

template <typename T, typename Allocator>
struct BindingType<std::vector<T, Allocator>> {
    using ValBinding = BindingType<val>;
    using WireType = ValBinding::WireType;

    static WireType toWireType(const std::vector<T, Allocator> &vec) {
        return ValBinding::toWireType(val::array(vec));
    }

    static std::vector<T, Allocator> fromWireType(WireType value) {
        return vecFromJSArray<T>(ValBinding::fromWireType(value));
    }
};

template <typename T>
struct TypeID<T,
              typename std::enable_if_t<std::is_same<
                  typename Canonicalized<T>::type,
                  std::vector<typename Canonicalized<T>::type::value_type,
                              typename Canonicalized<T>::type::allocator_type>>::value>> {
    static constexpr TYPEID get() { return TypeID<val>::get(); }
};

}  // namespace internal
}  // namespace emscripten

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

static std::vector<Bulb> GetBulbs(std::vector<int> pos_x) noexcept {
  std::vector<Bulb> arr; 
  for (auto x : pos_x) {
    arr.push_back(Bulb{
        .x = static_cast<float>(x),
        .y = static_cast<float>(x),
        .blinking = true,
        .arrow = Arrow::LEFT,
        .color = Color::RED
    });
  }
  return arr;
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
