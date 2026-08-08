#include<iostream>
#include<string>
#include<sstream>
#include<chrono>
#include<stdlib.h>
#include<list>
#include<Magick++.h>
#include"methods.h"

using namespace std;
using namespace Magick;

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
	InitializeMagick(path_to_ImageMagick_DLLs.c_str());
	MagickPlusPlusGenesis genesis(*argv);

	Image img{ {640,640},"gray" };
	if (argc != 1) {
		img = Image(image_name);
		cout << img.columns() << "," << img.rows() << endl;
	}
	if (!(img.isValid())) {
		cout << "couldn't open:" << image_name << endl;
		return 1;
	}
	cout << "Starting..." << endl;
	unsigned int image_height = img.rows();
	unsigned int image_width = img.columns();
	img.type(ImageType::TrueColorType);
	img.modifyImage();

	bool running = true;
	int count = 1;
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
			string temp_name = out_image_name + "_" + to_string(count++) + '.' + extension;
			img.write(temp_name);
			string cmd = '"' + temp_name + '"';
			system(cmd.c_str());
			cout << "Saved " << temp_name << endl;
		}
		else if (temp_string == "noise") {
			float temp_float = 0.1;
			iss >> temp_float;
			images_list.emplace_back( img );
			add_monotone_true_noise(img, temp_float);
			cout << "Applied uniform noise." << temp_float << endl;
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
		}
		else if (temp_string == "2tone") {
			float temp_float = 0.7;
			iss >> temp_float;
			images_list.emplace_back( img );
			to_black_and_white(img, temp_float);
			cout << "Applied black and white." << endl;
		}
		else if (temp_string == "d2tone") {
			float temp_float = 0.95;
			float temp_float2 = 0.05;
			iss >> temp_float >> temp_float2;
			images_list.emplace_back(img);
			to_black_and_white_dynamic(img, temp_float, temp_float2);
			cout << "Applied dynamic black and white." << endl;
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
		}
		else {
			cout << "option not found, placeholder help text" << endl;

		}
	}

	return 0;
}
