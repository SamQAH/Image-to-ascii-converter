#include"colour_lab.h"

#define max(a, b) (a > b ? a : b)

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
	int tempSum = 0;
	auto iterend = end();
	for (auto iter = begin(); iter != iterend; ++iter) {
		int count = *iter;
		if (count == 0) {
			continue;
		}
		rAvg += count * iter.currR;
		gAvg += count * iter.currG;
		bAvg += count * iter.currB;
		tempSum += count;
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
	int tempSum = 0;
	auto iterend = end(color, radius);
	for (auto iter = begin(color, radius); iter != iterend; ++iter) {
		int count = *iter;
		if (count == 0) {
			continue;
		}
		rAvg += count * iter.currR;
		gAvg += count * iter.currG;
		bAvg += count * iter.currB;
		tempSum += count;
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
	auto iterend = end(color, radius);
	for (auto iter = begin(color, radius); iter != iterend; ++iter) {
		int& count = *iter;
		sum -= count;
		count = 0;
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
	auto iterend = end(color, radius);
	for (auto iter = begin(color, radius); iter != iterend; ++iter) {
		int count = *iter;
		if (count != 0) {
			return false;
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
	auto iterend = end();
	for (auto iter = begin(); iter != iterend; ++iter) {
		int count = *iter;
		if (count != 0) {
			return Color(iter.currR / quantumScaleFactor, iter.currG / quantumScaleFactor, iter.currB / quantumScaleFactor);
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
	int rdist = a.quantumRed() > b.quantumRed() ? a.quantumRed() - b.quantumRed() : b.quantumRed() - a.quantumRed();
	int gdist = a.quantumGreen() > b.quantumGreen() ? a.quantumGreen() - b.quantumGreen() : b.quantumGreen() - a.quantumGreen();
	int bdist = a.quantumBlue() > b.quantumBlue() ? a.quantumBlue() - b.quantumBlue() : b.quantumBlue() - a.quantumBlue();
	
	return max(max(rdist, gdist), bdist);
}

ColorSpaceRGB::Iterator ColorSpaceRGB::begin()
{
	return Iterator(*this);
}

ColorSpaceRGB::Iterator ColorSpaceRGB::end()
{
	return Iterator(*this, true);
}

ColorSpaceRGB::Iterator ColorSpaceRGB::begin(Color& color, int radius)
{
	return Iterator(*this, color, radius);
}

ColorSpaceRGB::Iterator ColorSpaceRGB::end(Color& color, int radius)
{
	return Iterator(*this, color, radius, true);
}

ColorSpaceRGB::Iterator::Iterator(ColorSpaceRGB& space, bool end) : rgbSpace{ space.rgbSpace }, rstart{ 0 }, rend{ colorRange - 1 }, gstart{ 0 }, gend{ colorRange - 1 }, bstart{ 0 }, bend{ colorRange - 1 }, currR{ 0 }, currG{ 0 }, currB{ 0 }
{
	if (end) {
		currR = rend + 1;
	}
}

ColorSpaceRGB::Iterator::Iterator(ColorSpaceRGB& space, Color& color, int radius, bool end) : rgbSpace{ space.rgbSpace }, 
	rstart{ (size_t)max(0, (int)(color.quantumRed() * quantumScaleFactor) - radius) }, rend{ (size_t)min(colorRange-1, (int)(color.quantumRed() * quantumScaleFactor) + radius) },
	gstart{ (size_t)max(0, (int)(color.quantumGreen() * quantumScaleFactor) - radius) }, gend{ (size_t)min(colorRange-1, (int)(color.quantumGreen() * quantumScaleFactor) + radius) },
	bstart{ (size_t)max(0, (int)(color.quantumBlue() * quantumScaleFactor) - radius) }, bend{ (size_t)min(colorRange-1, (int)(color.quantumBlue() * quantumScaleFactor) + radius) },
	currR{ rstart }, currG{ gstart }, currB{ bstart }
{
	if (end) {
		currR = rend + 1;
	}
}

int& ColorSpaceRGB::Iterator::operator*()
{
	return rgbSpace.at(currR).at(currG).at(currB);
}

ColorSpaceRGB::Iterator& ColorSpaceRGB::Iterator::operator++()
{
	if (currR == rend + 1) {
		return *this;
	}
	currB++;
	if (currB > bend) {
		currG++;
		currB = bstart;
	}
	if (currG > gend) {
		currR++;
		currG = gstart;
	}
	return *this;
}

bool ColorSpaceRGB::Iterator::operator!=(const Iterator& other)
{
	return !(&rgbSpace == &(other.rgbSpace) && currR == other.currR && currG == other.currG && currB == other.currB && rstart == other.rstart && gstart == other.gstart && bstart == other.bstart && rend == other.rend && gend == other.gend && bend == other.bend);
}

ColorSmoother::ColorSmoother(float radius, float margin) : radius{ (int)(colorRange * radius) }, margin{ (int)(colorRange * margin) }, colSpace{}, avgColors{}, colorMaper{}, hasSynced{false}, tolerance{5}, maxCycle{10}
{
	colorMaper.resize((size_t)colorRange);
	for (auto& gbSpace : colorMaper) {
		gbSpace.resize((size_t)colorRange, vector<unsigned short>((size_t)colorRange));
	}
	for (auto& two : colorMaper) {
		for (auto& one : two) {
			for (auto& c : one) {
				c = (char)0;
			}
		}
	}
}

void ColorSmoother::add_color(Color& color)
{
	colSpace.add(color);
	hasSynced = false;
}

Color ColorSmoother::grad_descent()
{
	cerr << "color smoother grad descent start ";
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
	avgColors.clear();
	while (!(colSpace.isEmpty())) {
		avgColors.emplace_back(grad_descent());
		colSpace.remove(avgColors.at(avgColors.size() - 1), margin);
		cerr << colSpace.to_string() << endl;
	}
	for (auto& two : colorMaper) {
		for (auto& one : two) {
			for (auto& c : one) {
				c = (char)0;
			}
		}
	}
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
	//cout << color;
	unsigned short temp = colorMaper.at(colorR).at(colorG).at(colorB);
	//return avgColors.at((size_t)temp);

	if (temp != 0) {
		//cout << (size_t)temp << avgColors.size() << endl;
		if ((size_t)temp > avgColors.size()) {
			cout << endl << color << " " <<temp << endl;
			return Color("white");
		}
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
	if (min > margin || min_index == -1) {
		avgColors.emplace_back(color);
		colorMaper.at(colorR).at(colorG).at(colorB) = (unsigned short)(avgColors.size());
		return color;
	}
	else {
		colorMaper.at(colorR).at(colorG).at(colorB) = (unsigned short)(min_index+1);
		return avgColors.at(min_index);
	}
}

ostream& operator<<(ostream& out, Color& color)
{
	out << "(" << (int)(color.quantumRed() * quantumScaleFactor) << "," << (int)(color.quantumGreen() * quantumScaleFactor) << "," << (int)(color.quantumBlue() * quantumScaleFactor) << ")";
	return out;
}
