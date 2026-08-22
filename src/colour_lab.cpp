#include"colour_lab.h"
#include<algorithm>

ColorSpaceRGB::ColorSpaceRGB() : rgbSpace{ (size_t)colorRange }, sum{0}
{
	for (auto& gbSpace : rgbSpace) {
		gbSpace.resize((size_t)colorRange, vector<int>((size_t)colorRange));
	}
}

void ColorSpaceRGB::add(Color& color)
{
	rgbSpace.at((size_t)(color.quantumRed() * quantumScaleFactor)).at((size_t)(color.quantumGreen() * quantumScaleFactor)).at((size_t)(color.quantumBlue() * quantumScaleFactor))++;
	sum++;
}

Color ColorSpaceRGB::average()
{
	if (sum == 0) {
		return Color("black");
	}
	double rAvg = 0;
	double gAvg = 0;
	double bAvg = 0;
	int currR = 0;
	int currG = 0;
	int currB = 0;
	for (auto& gbSpace : rgbSpace) {
		for (auto& bSpace : gbSpace) {
			for (auto& count : bSpace) {
				if (count == 0) {
					currB++;
					continue;
				}
				rAvg += count * currR;
				gAvg += count * currG;
				bAvg += count * currB;
				currB++;
			}
			currG++;
		}
		currR++;
	}
	rAvg = rAvg / sum;
	gAvg = gAvg / sum;
	bAvg = bAvg / sum;
	return Color(rAvg / quantumScaleFactor, gAvg / quantumScaleFactor, bAvg / quantumScaleFactor);
}

Color ColorSpaceRGB::average(Color& color, int radius)
{
	if (sum == 0) {
		return color;
	}
	double rAvg = 0;
	double gAvg = 0;
	double bAvg = 0;
	int rStart = max(0, (int)(color.quantumRed() * quantumScaleFactor) - radius);
	int rEnd = min(colorRange, (int)(color.quantumRed() * quantumScaleFactor) + radius);
	int gStart = max(0, (int)(color.quantumGreen() * quantumScaleFactor) - radius);
	int gEnd = min(colorRange, (int)(color.quantumGreen() * quantumScaleFactor) + radius);
	int bStart = max(0, (int)(color.quantumBlue() * quantumScaleFactor) - radius);
	int bEnd = min(colorRange, (int)(color.quantumBlue() * quantumScaleFactor) + radius);
	int tempSum = 0;
	for (size_t currR = rStart; currR < rEnd; currR++) {
		for (size_t currG = gStart; currG < gEnd; currG++) {
			for (size_t currB = bStart; currB < bEnd; currB++) {
				int count = rgbSpace.at(currR).at(currG).at(currB);
				rAvg += count * currR;
				gAvg += count * currG;
				bAvg += count * currB;
				tempSum += count;
			}
		}
	}
	if (tempSum == 0) {
		return color;
	}
	rAvg = rAvg / tempSum;
	gAvg = gAvg / tempSum;
	bAvg = bAvg / tempSum;
	return Color(rAvg / quantumScaleFactor, gAvg / quantumScaleFactor, bAvg / quantumScaleFactor);
}

void ColorSpaceRGB::remove(Color& color, int radius)
{
	int rStart = max(0, (int)(color.quantumRed() * quantumScaleFactor) - radius);
	int rEnd = min(colorRange, (int)(color.quantumRed() * quantumScaleFactor) + radius);
	int gStart = max(0, (int)(color.quantumGreen() * quantumScaleFactor) - radius);
	int gEnd = min(colorRange, (int)(color.quantumGreen() * quantumScaleFactor) + radius);
	int bStart = max(0, (int)(color.quantumBlue() * quantumScaleFactor) - radius);
	int bEnd = min(colorRange, (int)(color.quantumBlue() * quantumScaleFactor) + radius);
	for (size_t currR = rStart; currR < rEnd; currR++) {
		for (size_t currG = gStart; currG < gEnd; currG++) {
			for (size_t currB = bStart; currB < bEnd; currB++) {
				int& count = rgbSpace.at(currR).at(currG).at(currB);
				sum -= count;
				count = 0;
			}
		}
	}
}

bool ColorSpaceRGB::isEmpty()
{
	return sum == 0;
}

bool ColorSpaceRGB::isEmpty(Color& color, int radius)
{
	if (sum == 0) {
		return true;
	}
	int rStart = max(0, (int)(color.quantumRed() * quantumScaleFactor) - radius);
	int rEnd = min(colorRange, (int)(color.quantumRed() * quantumScaleFactor) + radius);
	int gStart = max(0, (int)(color.quantumGreen() * quantumScaleFactor) - radius);
	int gEnd = min(colorRange, (int)(color.quantumGreen() * quantumScaleFactor) + radius);
	int bStart = max(0, (int)(color.quantumBlue() * quantumScaleFactor) - radius);
	int bEnd = min(colorRange, (int)(color.quantumBlue() * quantumScaleFactor) + radius);
	for (size_t currR = rStart; currR < rEnd; currR++) {
		for (size_t currG = gStart; currG < gEnd; currG++) {
			for (size_t currB = bStart; currB < bEnd; currB++) {
				int& count = rgbSpace.at(currR).at(currG).at(currB);
				if (count != 0) {
					return false;
				}
			}
		}
	}
	return true;
}

void ColorSpaceRGB::clear()
{
	rgbSpace.clear();
	rgbSpace.resize((size_t)colorRange);
	for (auto& gbSpace : rgbSpace) {
		gbSpace.resize((size_t)colorRange, vector<int>((size_t)colorRange));
	}
	sum = 0;
}

Color ColorSpaceRGB::get()
{
	if (isEmpty()) {
		return Color("black");
	}
	for (size_t currR = 0; currR < colorRange; currR++) {
		for (size_t currG = 0; currG < colorRange; currG++) {
			for (size_t currB = 0; currB < colorRange; currB++) {
				int& count = rgbSpace.at(currR).at(currG).at(currB);
				if (count != 0) {
					return Color(currR / quantumScaleFactor, currG / quantumScaleFactor, currB / quantumScaleFactor);

				}
			}
		}
	}
	return Color("black");
}

string ColorSpaceRGB::to_string()
{
	string str = "color space rgb storing " + std::to_string(sum) + "colours";
	return str;
}

int ColorSpaceRGB::distance_inf(const Color& a, const Color& b)
{
	return max(max(abs(a.quantumRed() - b.quantumRed()), abs(a.quantumGreen() - b.quantumGreen())), abs(a.quantumBlue() - b.quantumBlue()));
}

ColorSmoother::ColorSmoother(float radius, float margin) : radius{ (int)(colorRange * radius) }, margin{ (int)(colorRange * margin) }, colSpace{}, avgColors{}, colorMaper{}, hasSynced{false}, tolerance{5}, maxCycle{10}
{
	colorMaper.resize((size_t)colorRange);
	for (auto& gbSpace : colorMaper) {
		gbSpace.resize((size_t)colorRange, vector<char>((size_t)colorRange));
	}
}

void ColorSmoother::add_color(Color& color)
{
	colSpace.add(color);
	hasSynced = false;
}

Color ColorSmoother::grad_descent()
{
	cerr << "color smoother grad descent start "<<flush;
	Color colPrev = colSpace.get();
	Color colNext = colSpace.average(colPrev, radius);
	cerr << colPrev<<colNext;
	int count = 0;
	while (ColorSpaceRGB::distance_inf(colPrev, colNext) * quantumScaleFactor > tolerance && count < maxCycle) {
		colPrev = colNext;
		colNext = colSpace.average(colPrev, radius);
		cerr << colNext;
		count++;
	}
	cerr << endl;
	return colNext;
}

void ColorSmoother::sync()
{
	cerr << "color smoother sync " << endl;
	while (!(colSpace.isEmpty())) {
		avgColors.emplace_back(grad_descent());
		colSpace.remove(avgColors.at(avgColors.size() - 1), margin);
		cerr << colSpace.to_string() << endl;
	}
	/*
	int currR = 0;
	int currG = 0;
	int currB = 0;
	int tempCount = 0;
	for (auto& gbSpace : colorMaper) {
		for (auto& bSpace : gbSpace) {
			for (auto& c : bSpace) {
				int min = margin / quantumScaleFactor;
				size_t minIndex = -1;
				for (size_t i = 0; i < avgColors.size(); i++) {
					Color tempColor = Color(currR / quantumScaleFactor, currG / quantumScaleFactor, currB / quantumScaleFactor);
					int temp_dist = ColorSpaceRGB::distance_inf(avgColors.at(i), tempColor);
					if (temp_dist < min) {
						min = temp_dist;
						minIndex = i;
					}
				}
				c = (char)minIndex;
				if (tempCount % 10000 == 0) {
					cerr << tempCount << "/" << 256 * 256 * 256 << endl;
					cerr << "\x1B[1A\x1B[0J";
				}
				tempCount++;
				currB++;
			}
			currG++;
		}
		currR++;
	}
	*/

	hasSynced = true;
}

void ColorSmoother::add_image(Image& img)
{
	cerr << "colour smoother add image" << endl;
	for (int i = 0; i < img.columns(); i++) {
		for (int j = 0; j < img.rows(); j++) {
			Color tempC = img.pixelColor(i, j);
			add_color(tempC);
		}
	}
}

Color ColorSmoother::find_nearest(Color& color)
{
	//cerr << "colour smoother find nearest" << color << endl;
	if (!hasSynced) {
		sync();
	}
	size_t colorR = quantumScaleFactor * color.quantumRed();
	size_t colorG = quantumScaleFactor * color.quantumGreen();
	size_t colorB = quantumScaleFactor * color.quantumBlue();
	char temp = colorMaper.at(colorR).at(colorG).at(colorB);
	//return avgColors.at((size_t)temp);

	if ((size_t)temp != 0) {
		return avgColors.at((size_t)(temp-1));
	}
	int min = colorRange;
	int min_index = -1;
	for (int i = 0; i < avgColors.size(); i++) {
		int dist = ColorSpaceRGB::distance_inf(color, avgColors.at(i)) * quantumScaleFactor;
		if (dist < min) {
			min = dist;
			min_index = i;
		}
	}
	if (min > margin) {
		avgColors.emplace_back(color);
		colorMaper.at(colorR).at(colorG).at(colorB) = (char)(avgColors.size());
		return color;
	}
	else {
		colorMaper.at(colorR).at(colorG).at(colorB) = (char)(min_index+1);
		return avgColors.at(min_index);
	}
}

ostream& operator<<(ostream& out, Color& color)
{
	out << "(" << (int)(color.quantumRed() * quantumScaleFactor) << "," << (int)(color.quantumGreen() * quantumScaleFactor) << "," << (int)(color.quantumBlue() * quantumScaleFactor) << ")";
	return out;
}
