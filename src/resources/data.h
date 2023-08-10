#ifndef DATA_RESOURCES_H
#define DATA_RESOURCES_H

#include <glm/glm.hpp>
#include <string>
#include <unordered_map>
#include <vector>

extern const std::unordered_map<std::string, std::string> shaders;

extern  unsigned char flash_image[262144];
extern  glm::vec2 flash_size;
extern  unsigned char font_image[131072];
extern  glm::vec2 font_size;
extern  unsigned char particles_image[32352];
extern  glm::vec2 particles_size;
extern  unsigned char noise_image[262144];
extern  glm::vec2 noise_size;
extern  unsigned char pedal_side_image[761620];
extern  glm::vec2 pedal_side_size;
extern  unsigned char pedal_center_image[614720];
extern  glm::vec2 pedal_center_size;
extern  unsigned char pedal_top_image[630984];
extern  glm::vec2 pedal_top_size;

#endif
