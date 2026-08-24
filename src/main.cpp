#include<iostream>
#include<string>
#include<sstream>
#include<chrono>
#include<stdlib.h>
#include<list>
#include<Magick++.h>
#include"magick_wrapper.h"
#include"methods.h"

using namespace std;

int main(int argc, char** argv) {
	string image_name;
	if (argc == 1) {
		cout << "No image provided" << endl;
		image_name = "blank_image.png";
	}
	else {
		image_name = string(argv[1]);
	}
	istringstream iss{ image_name };
	string out_image_name;
	string extension;
	getline(iss, out_image_name, '.');
	iss >> extension;
	//out_image_name += "(out)." + buffer;

	string path_to_ImageMagick_DLLs = "C:\\Program Files\\ImageMagick-7.1.2-Q16-HDRI";
	Magick::InitializeMagick(path_to_ImageMagick_DLLs.c_str());
	Magick::MagickPlusPlusGenesis genesis(*argv);

	Magick::Image magick_img{ {640,640},"gray" };
	if (argc != 1) {
		try {
			magick_img = Magick::Image(image_name);
		}
		catch (exception e) {
			try {
				string in_name = "in/" + image_name;
				magick_img = Magick::Image(in_name);
			}
			catch (exception e) {
				cout << "couldn't open:" << image_name << endl;
				return 1;
			}
		}
		cout << magick_img.columns() << "," << magick_img.rows() << endl;
	}
	if (!(magick_img.isValid())) {
		cout << "is not valid:" << image_name << endl;
		return 1;
	}
	cout << "Starting..." << endl;
	unsigned int image_height = magick_img.rows();
	unsigned int image_width = magick_img.columns();
	magick_img.type(Magick::ImageType::TrueColorType);
	magick_img.modifyImage();
	int num_empty = 0;
	bool running = true;
	int count = 1;
	string prev_op = "none";
	Image img(magick_img);
	list<Image> images_list;
	while (running) {
		string buffer;
		int temp_a;
		int temp_b;
		string temp_string;
		auto time_start = chrono::high_resolution_clock::now();
		auto time_end = chrono::high_resolution_clock::now();
		chrono::duration<double, std::milli> duration_ms = (time_end - time_start);
		cout << ">>";
		getline(cin, buffer);
		istringstream iss{ buffer };
		iss >> temp_string;
		if (temp_string == "quit") {
			running = false;
			cout << "Quit." << endl;
		}
		else if (temp_string == "save") {
			string temp_name = out_image_name + "_" + to_string(count++) + "_" + prev_op + '.' + extension;
			string out_name = "out/" + temp_name;
			img.write(out_name);
#ifdef OPENFILE
			string cmd = "start " + out_name;
			system(cmd.c_str());
#endif
			cout << "Saved " << temp_name << endl;
		}
		else if (temp_string == "noise") {
			float temp_float = 0.1;
			iss >> temp_float;
			images_list.emplace_back( img );
			add_monotone_true_noise(img, temp_float);
			cout << "Applied uniform noise." << temp_float << endl;
			prev_op = "noise" + to_string(temp_float);
		}
		else if (temp_string == "bnoise") {
			unsigned int tempx = 1;
			unsigned int tempy = 1;
			float temp_float = 0.1;
			size_t temp_size = 1;
			iss >> tempx >> tempy >> temp_float >> temp_size;
			images_list.emplace_back(img);
			add_monotone_block_noise(img, tempx, tempy, temp_float, temp_size);
			cout << "Applied block noise." << temp_float << endl;
			prev_op = "block-noise" + to_string(temp_float) + "-" + to_string(temp_size);
		}
		else if (temp_string == "2tone") {
			float temp_float = 0.7;
			iss >> temp_float;
			images_list.emplace_back( img );
			to_black_and_white(img, temp_float);
			cout << "Applied black and white." << temp_float << endl;
			prev_op = "2tone" + to_string(temp_float);
		}
		else if (temp_string == "d2tone") {
			float temp_float = 0.95;
			float temp_float2 = 0.05;
			iss >> temp_float >> temp_float2;
			images_list.emplace_back(img);
			to_black_and_white_dynamic(img, temp_float, temp_float2);
			cout << "Applied dynamic black and white. " << temp_float << " , " << temp_float2 << endl;
			prev_op = "dynamic2tone" + to_string(temp_float) + "-" + to_string(temp_float2);
		}
		else if (temp_string == "smoothen") {
			float temp_float = 0.125;
			float temp_float2 = 0.135;
			iss >> temp_float >> temp_float2;
			images_list.emplace_back(img);
			color_smoothen(img, temp_float, temp_float2);
			cout << "Applied colour smoothen. " << temp_float << " , " << temp_float2 << endl;
			prev_op = "color-smoothen" + to_string(temp_float) + "-" + to_string(temp_float2);
		}
		else if (temp_string == "bit") {
			float temp_float = 8;
			iss >> temp_float;
			images_list.emplace_back(img);
			reduce_entropy(img, temp_float);
			cout << "Applied reduce bits. " << temp_float << endl;
			prev_op = "bit-shift" + to_string(temp_float);
		}
		else if (temp_string == "undo") {
			if (images_list.empty()) {
				cout << "Can not undo." << endl;
			}
			else {
				img = images_list.back();
				images_list.pop_back();
				cout << "Undid last operation." << endl;
			}
			prev_op = "undo";
		}
		else {
			cout << temp_string << " option not found, placeholder help text: " << endl;
			if (num_empty > 5) {
				running = false;
				cout << "Quit." << endl;
			}
			num_empty++;
		}
	}

	return 0;
}
