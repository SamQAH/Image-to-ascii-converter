#ifndef ANALYSE_H_
#define ANALYSE_H_

struct image_meta {
    char *name;
    int width;
    int height;
    int *data;
};
struct ascii_pixel {
    int len;
    int pixel_size;
    int *data;
    int *range_min;
    int *range_max;
    int *intensity;
    int total_intensity;
    int noise;
    char character;
};

// apply(img, new_img, convolution_size, pattern) applys a convolution(pattern) across img.data with pattern and saves the result to new_img
void apply(struct image_meta *img, struct image_meta *new_img, int convolution_size, double (*pattern)(int, int, int));

// to_image(img) saves an image to the output directory
int to_image(struct image_meta *img);

// combine(imgs, colors, length, cutoff) compares the given images, colors the corresponding pixel in the result based on the color at the
//   same index as the image with the brightest pixel.
// Require: the length of imgs and colors be the same,
// effects: saves an image to the output directory
//          produce output on error
int combine(const struct image_meta *imgs, const char * const colors[], int length, int cutoff);

// to_ascii(imgs, length, pixel_size) 
int to_ascii(const struct image_meta *imgs, int length, int pixel_size);

// read_options_stdin() reads the first len options from standard input to set the parameter values
int read_options_stdin(void);

// read_options_arr(argsc, argsv) reads the first argsc options from the provided array as option to parse, similar to read_options_stdin
int read_options_arr(int argsc, char **argsv);


#endif