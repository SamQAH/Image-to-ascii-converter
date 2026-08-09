#include"colour_lab.h"
#include<algorithm>

ColorSpaceRGB::ColorSpaceRGB() : rSpace{ (size_t)colorRange }, gSpace{ (size_t)colorRange }, bSpace{ (size_t)colorRange }
{
}

void ColorSpaceRGB::add(Color& color)
{
	rSpace.at((size_t)(color.quantumRed() * quantumScaleFactor))++;
	gSpace.at((size_t)(color.quantumGreen() * quantumScaleFactor))++;
	bSpace.at((size_t)(color.quantumBlue() * quantumScaleFactor))++;
}

Color ColorSpaceRGB::average()
{
	double rAvg = 0;
	double gAvg = 0;
	double bAvg = 0;
	int rSum = 0;
	int gSum = 0;
	int bSum = 0;
	for (int i = 0; i < colorRange; i++) {
		rAvg += i * rColor.at(i);
		rSum += rColor.at(i);
		gAvg += i * gColor.at(i);
		gSum += gColor.at(i);
		bAvg += i * bColor.at(i);
		bSum += bColor.at(i);
	}
	rAvg = rSum == 0 ? 0 : rAvg / rSum;
	gAvg = gSum == 0 ? 0 : gAvg / gSum;
	bAvg = bSum == 0 ? 0 : bAvg / bSum;
	return Color(rAvg, gAvg, bAvg);
}

Color ColorSpaceRGB::average(Color& color, int radius)
{
	double rAvg = 0;
	double gAvg = 0;
	double bAvg = 0;
	int rSum = 0;
	int gSum = 0;
	int bSum = 0;
	int rStart = max(0, (color.quantumRed() * quantumScaleFactor) - radius);
	int rEnd = min(colorRange, (color.quantumRed() * quantumScaleFactor) + radius);
	int gStart = max(0, (color.quantumGreen() * quantumScaleFactor) - radius);
	int gEnd = min(colorRange, (color.quantumGreen() * quantumScaleFactor) + radius);
	int bStart = max(0, (color.quantumBlue() * quantumScaleFactor) - radius);
	int bEnd = min(colorRange, (color.quantumBlue() * quantumScaleFactor) + radius);
	for (int i = rStart; i <= rEnd; i++) {
		rAvg += i * rColor.at(i);
		rSum += rColor.at(i);
	}
	for (int i = gStart; i <= gEnd; i++) {
		gAvg += i * gColor.at(i);
		gSum += gColor.at(i);
	}
	for (int i = bStart; i <= bEnd; i++) {
		bAvg += i * bColor.at(i);
		bSum += bColor.at(i);
	}
	rAvg = rSum == 0 ? 0 : rAvg / rSum;
	gAvg = gSum == 0 ? 0 : gAvg / gSum;
	bAvg = bSum == 0 ? 0 : bAvg / bSum;
	return Color();
}

void ColorSpaceRGB::remove(Color& color, int radius)
{
	int rStart = max(0, (color.quantumRed() * quantumScaleFactor) - radius);
	int rEnd = min(colorRange, (color.quantumRed() * quantumScaleFactor) + radius);
	int gStart = max(0, (color.quantumGreen() * quantumScaleFactor) - radius);
	int gEnd = min(colorRange, (color.quantumGreen() * quantumScaleFactor) + radius);
	int bStart = max(0, (color.quantumBlue() * quantumScaleFactor) - radius);
	int bEnd = min(colorRange, (color.quantumBlue() * quantumScaleFactor) + radius);
	for (int i = rStart; i <= rEnd; i++) {
		rColor.at(i);
	}
	for (int i = gStart; i <= gEnd; i++) {
		gColor.at(i);
	}
	for (int i = bStart; i <= bEnd; i++) {
		bColor.at(i);
	}
}
