#include"patterns.h"

#ifndef ANALYSE_H_
#define ANALYSE_H_

struct image_meta {
    char *name;
    int width;
    int height;
    int *data;
};

struct ascii_pixel {
    int len; // how many convolutions were applied
    int pixel_size; // the area of the pixel
    int *data; // the values from each convolution within the pixel
    int *range_min; // the minimum value from each convolution
    int *range_max; // the maximum value from each convolution
    int *intensity; // the average brightness of each convolution
    int avg_brightness; // the average brightness of the ascii pixel
    int noise; // how many intensity values are above ignore_noise_cutoff
    char character; // what should be printed
};

struct ascii_intermediate {
    int width;
    int height;
    struct ascii_pixel *data;
};

int arithmetic(PixelInfo *info, int *temp);
int geometric(PixelInfo *info, int *temp);
static int (*avg_fun)(PixelInfo *, int*) = &arithmetic;
static char output_dir[100] = "./";
static int convolution_size = 3;
static int convolution_area = 9;
static char output_extension[6] = ".png";
static unsigned int counter = 0;
static const char * const COLORS[] = { "#ffff00000000", "#888800000000", "#00000000ffff",
                                "#000000008888", "#ffff0000ffff", "#888800008888",
                                "#0000ffff0000", "#000088880000", "#ffffffffffff" 
};
static int quantum_range = 255;
static int combine_color_cutoff = 5; // must be above this percentage to draw a color in the combined image -cc<number>
static int saturation_pattern_cutoff = 10; // convolution must return a value be above this percentage to contribute to the ascii convertion -cs<number>
static int ignore_noise_cutoff = 5; // above this percentage the intensity of the convolution channel contribute to increasing the number of convolutions in a pixel, -cN<number>
static int ascii_min_intensity = 5; // pixel must be at least this intense to not be empty -ci<number>
static int ascii_mid_intensity = 10;
static int ascii_high_intensity = 50;
static int ascii_entropy_ignore = 4; // if there are more than this much noise in a pixel(see ignore_noise_cutoff), set it to the noise pixel -cn<number>
static int grid_on = 0;
static int grid_length = 5;
static char grid_color[] = "#ffffffffffff";
static int show_steps = 0;
static int auto_balence = 800; // TODO set

static int use_patterns_len = 10;
static double (*use_patterns[])(int, int, int) = {
    &left_vert_pattern_basic,
    &right_vert_pattern_basic,
    &up_horz_pattern_basic,
    &down_horz_pattern_basic,
    &Q_one_diagonal_basic,
    &Q_two_diagonal_basic,
    &Q_three_diagonal_basic,
    &Q_four_diagonal_basic,
    &hill_average,
    &valley_average
};

static char ascii_lookup_table[] = {
    '.', '*', '#',
    ':', '|', 'I',
    '-', '=', '~',
    '`', '\\', '&',
    '/', '/', '%',
    'v', 'v', 'v',
    '>', '>', '>',
    '<', '<', '<',
    '^', '^', '^',
    'x', 'x', 'X',
    '+', '+', '+'
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

/*
supported characters and their use cases
----->intensity---->
 .*#
:|I
-=~
/%
\&
v>^<
xX+ 
*/
char to_ascii_lookup(struct ascii_pixel *pix);

// read_options_stdin() reads the first len options from standard input to set the parameter values
int read_options_stdin(void);

// read_options_arr(argsc, argsv) reads the first argsc options from the provided array as option to parse, similar to read_options_stdin
int read_options_arr(int argsc, char **argsv);

// essentially the main function
int pipeline(int argsc, char **argsv);

#endif