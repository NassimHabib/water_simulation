#include <fstream>
#include <iomanip>
#include <iostream>

#include "generic_image.hh"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

GenericImage::GenericImage(const std::string& filename, int channels)
  : width_(0), height_(0), channels_(0), image_(nullptr)
{
  std::ifstream file(filename);
  if (file.is_open())
  {
    file.close();
    stbi_set_flip_vertically_on_load(true);
    image_ = stbi_load(filename.c_str(), &width_, &height_, &channels_, channels);
  }
  else
  {
    std::cerr << "Error when loading GenericImage " << filename << std::endl;
  }
}

GenericImage::~GenericImage()
{
  stbi_image_free(image_);
}

int GenericImage::width_get() const
{
  return width_;
}

int GenericImage::height_get() const
{
  return height_;
}

int GenericImage::channels_get() const
{
  return channels_;
}

uint8_t* GenericImage::image_get()
{
  return image_;
}

std::ostream& operator<<(std::ostream& os, image& img)
{
  return os << "image_" << &img << "("
            << "width=" << std::setw(5) << img.width_get() << ", "
            << "height=" << std::setw(5) << img.height_get() << ", "
            << "channels=" << std::setw(2) << img.channels_get()
            << ")" << std::endl;
}
