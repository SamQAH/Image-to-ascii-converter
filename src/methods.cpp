#include"methods.h"
#include<cstdlib>
#include<algorithm>
#include<iostream>
#include<vector>

void apply_fragment_shader(Image& img, Color(*shader)(Color&)) {
	for (int i = 0; i < img.columns(); i++) {
		for (int j = 0; j < img.rows(); j++) {
			Color tempC = img.pixelColor(i, j);
			img.pixelColor(i, j, shader(tempC));
		}
	}
}
// helper class
class Shader {
public:
	static float uniform_f;
	static Color monotone_true_noise_shader(Color& col) {
		float rand = QuantumRange * (uniform_f * ((float)std::rand() / RAND_MAX * 2 - 1));
		return Color(col.quantumRed() + rand, col.quantumGreen() + rand, col.quantumBlue() + rand);
	}
	static Color rgb_true_noise_shader(Color& col) {
		float randr = QuantumRange * (uniform_f * ((float)std::rand() / RAND_MAX * 2 - 1));
		float randg = QuantumRange * (uniform_f * ((float)std::rand() / RAND_MAX * 2 - 1));
		float randb = QuantumRange * (uniform_f * ((float)std::rand() / RAND_MAX * 2 - 1));
		return Color(col.quantumRed() + randr, col.quantumGreen() + randg, col.quantumBlue() + randb);
	}
	static Color black_and_white_shader(Color& col) {
		if (max({ col.quantumRed(), col.quantumGreen(), col.quantumBlue() }) < uniform_f) {
			return Color("black");
		}
		else {
			return Color("white");
		}
	}
};
float Shader::uniform_f = 0;

void add_monotone_block_noise(Image& img, unsigned int xAvg, unsigned int yAvg, float range, size_t repeat) {
	//cout << xAvg << "," << yAvg << "," << range << endl;
	vector<vector<float>> rands_grid;
	rands_grid.resize(img.columns(), vector<float>((size_t) img.rows()) );
	int total = repeat * img.rows() * img.columns() / (xAvg * yAvg);
	for (int n = 0; n < total; n++) {
		float rand = QuantumRange * (range * ((float)std::rand() / RAND_MAX * 2 - 1));
		int tempx = std::rand() % img.columns();
		int tempy = std::rand() % img.rows();
		int tempw = std::rand() % (4 * xAvg) - (2 * xAvg);
		int temph = std::rand() % (4 * yAvg) - (2 * yAvg);
		for (int i = min(tempx + tempw, tempx); i < max(tempx + tempw, tempx); i++) {
			for (int j = min(tempy + temph, tempy); j < max(tempy + temph, tempy); j++) {
				//cout << i << "/" << img.columns() << "," << j << "/" << img.rows() << endl;
				if (i >= 0 && i < img.columns() && j >= 0 && j < img.rows()) {
					rands_grid[i][j] = rand;

				}
			}
		}
	}
	for (int i = 0; i < img.columns(); i++) {
		for (int j = 0; j < img.rows(); j++) {
			float rand = rands_grid[i][j];
			if (rand == 0) {
				rand = 1;
			}
			Color tempC = img.pixelColor(i, j);
			Color newC = Color(tempC.quantumRed() + rand, tempC.quantumGreen() + rand, tempC.quantumBlue() + rand);
			img.pixelColor(i, j, newC);

		}
	}
}

void add_monotone_true_noise(Image& img, float range) {
	Shader::uniform_f = range;
	apply_fragment_shader(img, Shader::monotone_true_noise_shader);
}
void add_rgb_noise(Image& img, float range) {
	Shader::uniform_f = range;
	apply_fragment_shader(img, Shader::rgb_true_noise_shader);
}

void color_smoothen(Image& img, float distance) {

}

void to_black_and_white(Image& img, float cutoff) {
	Shader::uniform_f = cutoff * QuantumRange;
	apply_fragment_shader(img, Shader::black_and_white_shader);
}

void to_black_and_white_noise(Image& img) {

}

void to_black_and_white_dynamic(Image& img, float cutoff, float g_width) {
	int radius = g_width * min(img.columns(), img.rows());
	const int img_cols = img.columns();
	const int img_rows = img.rows();
	cout << "cutoff:" << cutoff << " radius:" << radius << endl;
	vector<vector<float>> data_grid;
	data_grid.resize(img_cols, vector<float>((size_t)img_rows));
	for (int i = 0; i < img_cols; i++) {
		for (int j = 0; j < img_rows; j++) {
			Color tempC = img.pixelColor(i, j);
			data_grid[i][j] = max({ tempC.quantumRed(), tempC.quantumGreen(), tempC.quantumBlue() });
		}
	}
	cout << "loaded image to memory" << endl;
	vector<vector<double>> avgs_grid;
	avgs_grid.resize(img_cols, vector<double>((size_t)img_rows));
	vector<vector<double>> count_grid;
	count_grid.resize(img_cols, vector<double>((size_t)img_rows));
	for (int x = 0; x <= radius; x++) {
		for (int y = 0; y <= radius; y++) {
			if (x < img_cols && y < img_rows) {
				avgs_grid.at(0).at(0) += data_grid[x][y];
				count_grid.at(0).at(0)++;
			}
		}
	}
	int count = 0;
	int total = img_rows * img_cols;
	cout << endl;
	for (int i = 0; i < img_cols; i++) {
		for (int j = 0; j < img_rows; j++) {
			if (count++ % 100000 == 0) {
				cout << "\x1B[1A\x1B[0J";
				cout << count << "/" << total << "   " << i << "," << j << endl;
			}
			if (i == 0) {
				if (j == 0) {
					continue;
				}
				// modify from above
				avgs_grid[i][j] = avgs_grid[i].at(j - 1);
				count_grid[i][j] = count_grid[i].at(j - 1);
				int x = i - radius;
				int y = j - radius - 1;
				for (; x <= i + radius; x++) {
					if (x >= 0 && x < img_cols && y >= 0 && y < img_rows) {
						avgs_grid[i][j] -= data_grid[x][y];
						count_grid[i][j]--;
					}
				}
				x = i - radius;
				y = j + radius;
				for (; x <= i + radius; x++) {
					if (x >= 0 && x < img_cols && y >= 0 && y < img_rows) {
						avgs_grid[i][j] += data_grid[x][y];
						count_grid[i][j]++;
					}
				}
			}
			else {
				// modify from left
				avgs_grid[i][j] = avgs_grid.at(i - 1)[j];
				count_grid[i][j] = count_grid.at(i - 1)[j];
				int x = i - radius - 1;
				int y = j - radius;
				for (; y <= j + radius; y++) {
					if (x >= 0 && x < img_cols && y >= 0 && y < img_rows) {
						avgs_grid[i][j] -= data_grid[x][y];
						count_grid[i][j]--;
					}
				}
				x = i + radius;
				y = j - radius;
				for (; y <= j + radius; y++) {
					if (x >= 0 && x < img_cols && y >= 0 && y < img_rows) {
						avgs_grid[i][j] += data_grid[x][y];
						count_grid[i][j]++;
					}
				}
			}

		}
	}
	Color black = Color("black");
	Color white = Color("white");
	Image debug_img = img;
	for (int i = 0; i < img.columns(); i++) {
		for (int j = 0; j < img.rows(); j++) {
			if (count_grid[i][j] == 0) {
				continue;
			}
			float temp = (avgs_grid[i][j] / count_grid[i][j]);
			Color tempC = Color(temp, temp, temp);
			debug_img.pixelColor(i, j, tempC);
			
			if (data_grid[i][j] < cutoff * temp) {
				img.pixelColor(i, j, black);
			}
			else {
				img.pixelColor(i, j, white);
			}
		}
	}
	debug_img.write("debug.png");
}
