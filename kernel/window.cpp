#include "window.hpp"

// #@@range_begin(windowwriter_ctor)
Window::WindowWriter::WindowWriter(Window& window) : window_{window} {
}
// #@@range_end(windowwriter_ctor)

// #@@range_begin(windowwriter_write)
void Window::WindowWriter::Write(int x, int y, const PixelColor& c) {
  window_.At(x, y) = c;
}
// #@@range_end(windowwriter_write)

// #@@range_begin(windowwriter_width_height)
int Window::WindowWriter::Width() const {
  return window_.Width();
}

int Window::WindowWriter::Height() const {
  return window_.Height();
}
// #@@range_end(windowwriter_width_height)


// #@@range_begin(window_ctor)
Window::Window(int width, int height) : width_{width}, height_{height} {
  data_.resize(height);
  for (int y = 0; y < height; ++y) {
    data_[y].resize(width);
  }
}

Window::~Window() = default;

Window::Window(Window&& rhs)
  : width_{rhs.width_}, height_{rhs.height_},
    data_{std::move(rhs.data_)}, writer_{*this},
    transparent_color_{rhs.transparent_color_} {
}

Window& Window::operator=(Window&& rhs) {
  width_ = rhs.width_;
  height_ = rhs.height_;
  data_ = std::move(rhs.data_);
  transparent_color_ = rhs.transparent_color_;
  return *this;
}
// #@@range_end(window_ctor)

// #@@range_begin(window_drawto)
void Window::DrawTo(PixelWriter& writer, Vector2D<int> position) {
  if (transparent_color_) {
    const auto tc = transparent_color_.value();
    for (int y = 0; y < Height(); ++y) {
      for (int x = 0; x < Width(); ++x) {
        const auto c = At(x, y);
        if (c != tc) {
          writer.Write(position.x + x, position.y + y, c);
        }
      }
    }
    return;
  }

  for (int y = 0; y < Height(); ++y) {
    for (int x = 0; x < Width(); ++x) {
      const auto c = At(x, y);
      writer.Write(position.x + x, position.y + y, c);
    }
  }
}
// #@@range_end(window_drawto)

// #@@range_begin(window_settc)
void Window::SetTransparentColor(std::optional<PixelColor> c) {
  transparent_color_ = c;
}
// #@@range_end(window_settc)

Window::WindowWriter* Window::Writer() {
  return &writer_;
}

PixelColor& Window::At(int x, int y) {
  return data_[y][x];
}

const PixelColor& Window::At(int x, int y) const{
  return data_[y][x];
}

int Window::Width() const {
  return width_;
}

int Window::Height() const {
  return height_;
}