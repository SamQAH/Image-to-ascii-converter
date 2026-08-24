#ifndef METHODS_H
#define METHODS_H

#include"magick_wrapper.h"

using namespace std;

// adds parametized noise to image
void add_monotone_block_noise(Image& img, unsigned int xAvg, unsigned int yAvg, float range, size_t repeat);
// addes brightness noise to image
void add_monotone_true_noise(Image& img, float range);
// adds noise to each rgb channel
void add_rgb_noise(Image& img, float range);

// reduce the number of colors in the image
void color_smoothen(Image& img, float radius, float margin);

void to_black_and_white(Image& img, float cutoff = 0.7f);

void to_black_and_white_noise(Image& img);

void to_black_and_white_dynamic(Image& img, float cutoff = 0.9f, float g_width = 0.05f);

void reduce_entropy(Image& img, int size);

#endif