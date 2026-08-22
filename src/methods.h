#ifndef METHODS_H
#define METHODS_H

#include<Magick++.h>

using namespace std;
using namespace Magick;

// adds parametized noise to image
void add_monotone_block_noise(Image& img, unsigned int xAvg, unsigned int yAvg, float range, size_t repeat);
// addes brightness noise to image
void add_monotone_true_noise(Image& img, float range);
// adds noise to each rgb channel
void add_rgb_noise(Image& img, float range);

// reduce the number of colors in the image
void color_smoothen(Image& img, float radius, float margin);

void to_black_and_white(Image& img, float cutoff);

void to_black_and_white_noise(Image& img);

void to_black_and_white_dynamic(Image& img, float cutoff, float g_width);

void reduce_entropy(Image& img, int size);

#endif