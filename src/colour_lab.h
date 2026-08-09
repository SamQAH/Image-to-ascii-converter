#ifndef COLOUR_LAB_H
#define COLOUR_LAB_H

#include<Magick++.h>
#include<vector>
#include<list>

using namespace std;
constexpr int colorRange = 256;
constexpr float quantumScaleFactor = (float)colorRange / QuantumRange;

class ColorSpaceRGB {//R1 vs R3, loose too much information ie serpinsky pyramid shadow is a square
	vector<float> rSpace;
	vector<float> gSpace;
	vector<float> bSpace;
public:
	ColorSpaceRGB();
	void add(Color& color);
	Color average(); // compute the average color in the color space
	Color average(Color& color, int radius = colorRange); // computes the average color within the ball of radius 256
	void remove(Color& color, int radius = colorRange);
	bool isEmpty();
	
};

class ColorSmoother {
	ColorSpaceRGB colSpace;
	list<Color> avgColors;
public:
	ColorSmoother(int radius, int margin);
};

#endif