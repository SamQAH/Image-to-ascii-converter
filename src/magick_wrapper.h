#ifndef MAGICK_WRAPPER_H
#define MAGICK_WRAPPER_H

#include<Magick++.h>
#include<vector>
#include<string>

using namespace std;

constexpr float quantumRange = 65535.0f; // Magick::quantumRange needs Magick namespace
constexpr float colorRange = 256.0f;

class Color {
public:
	static constexpr float quantumScaleFactor = colorRange / quantumRange;
	int qRed, qGreen, qBlue;
	int Red, Green, Blue;
	Color(Magick::Color& color);
	Color(const string& str);
	Color(int r=0, int g=0, int b=0); // rgb from 0 to 256
	Magick::Color quantumColor() const;
	int quantumRed() const;
	int quantumGreen() const;
	int quantumBlue() const;
	int red() const;
	int green() const;
	int blue() const;
};

class Image {
private:
	Magick::Image& image;
	vector<vector<Color>> pixels;
	size_t columns_, rows_;
public:
	Image(Magick::Image& img);
	Image(const Image& other);
	Image& operator=(const Image& other);
	Color& pixelColor(size_t x, size_t y);
	Color pixelColor(size_t x, size_t y) const;
	void pixelColor(size_t x, size_t y, const Color& color);
	size_t columns() const;
	size_t rows() const;
	void write(const string& str);
	Magick::Image& operator*();
};

#endif