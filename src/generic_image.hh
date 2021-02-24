#pragma once

#include <ostream>
#include <string>

class GenericImage
{

public:
  GenericImage(const std::string& filename, int channels = 0);
  ~GenericImage();

  int width_get() const;
  int height_get() const;
  int channels_get() const;

  uint8_t* image_get();

private:
  int width_;
  int height_;
  int channels_;

  uint8_t* image_;
};

using image = GenericImage;

std::ostream& operator<<(std::ostream& os, image& img);
