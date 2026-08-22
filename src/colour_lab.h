#ifndef COLOUR_LAB_H
#define COLOUR_LAB_H

#include<Magick++.h>
#include<vector>
#include<list>
#include<memory>
#include<iostream>

using namespace std;
using namespace Magick;

constexpr int colorRange = 257;
constexpr float quantumScaleFactor = (float)(colorRange - 1) / QuantumRange;

ostream& operator<<(ostream& out, Color& color);

class ColorSpaceRGB {//R1 vs R3, loose too much information ie serpinsky pyramid shadow is a square
	vector<vector<vector<int>>> rgbSpace;
	int sum;
public:
	ColorSpaceRGB();
	void add(Color& color);
	Color average(); // compute the average color in the color space
	Color average(Color& color, int radius = colorRange); // computes the average color within the ball of radius 256
	void remove(Color& color, int radius = colorRange); // removes all colors within the radius of color
	bool isEmpty();
	bool isEmpty(Color& color, int radius = colorRange);
	void clear();
	Color get();
	string to_string();
	static int distance_inf(const Color& a, const Color& b);
};

class ColorSmoother {
	int radius; 
	int margin;
	ColorSpaceRGB colSpace;
	vector<Color> avgColors;
	vector<vector<vector<char>>> colorMaper;
	bool hasSynced;
	int tolerance;
	int maxCycle;
private:
	void add_color(Color& color);
	Color grad_descent();
	void sync();
public:
	ColorSmoother(float radius, float margin); // smoothed colors will be at lease 2*radius(percent) apart, all colors will be within margin(percent) of a color in avgColors
	void add_image(Image& img);
	Color find_nearest(Color& color);
};

#endif