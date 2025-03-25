#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<MagickWand/MagickWand.h>
#include<assert.h>
#include<string.h>
#include"patterns.h"
#include"analyse.h"

/*
global data
*/

int arithmetic(PixelInfo *info, int *temp);
int (*avg_fun)(PixelInfo *, int*) = &arithmetic;
char *output_dir = "./";
int convolution_size = 3;
int convolution_area = 9;
char output_extension[6] = ".png";
unsigned int counter = 0;
const char * const COLORS[] = { "#ffff00000000", "#888800000000", "#00000000ffff",
                                "#000000008888", "#ffff0000ffff", "#888800008888",
                                "#0000ffff0000", "#000088880000", "#ffffffffffff" };
int quantum_range = 255;
int combine_color_cutoff = 5; // must be above this percentage to draw a color in the combined image -cc<number>
int saturation_pattern_cutoff = 10; // convolution must return a value be above this percentage to contribute to the ascii convertion -cs<number>
int ignore_noise_cutoff = 5; // above this percentage the intensity of the convolution channel contribute to increasing the number of convolutions in a pixel, -cN<number>
int ascii_min_intensity = 10; // pixel must be at least this intense to not be empty -ci<number>
int ascii_entropy_ignore = 4; // if there are more than this much noise in a pixel(see ignore_noise_cutoff), set it to the noise pixel -cn<number>
int grid_on = 0;
int grid_length = 5;
char grid_color[] = "#ffffffffffff";
int show_steps = 0;
int use_random = 0;
int random_epsilon = 1000;
/*
helper functions
*/

// apply(img, new_img, convolution_size, pattern) applys a convolution across img.data with pattern and saves the result to new_img
void apply(struct image_meta *img, struct image_meta *new_img, int convolution_size, double (*pattern)(int, int, int)) {
    int temp_x = 0;
    int temp_y = 0;
    int temp_value = 0;
    double total_value = 0;
    for(int img_x = 0; img_x < new_img->width; img_x++){
        for(int img_y = 0; img_y < new_img->height; img_y++){
            total_value = 0;
            for(int dx = 0; dx < convolution_size; dx++){
                for(int dy = 0; dy < convolution_size; dy++){
                    temp_x = img_x + dx;
                    temp_y = img_y + dy;
                    temp_value = img->data[temp_x + temp_y * img->width];
                    total_value += pattern(dx, dy, convolution_size) * (double)temp_value;
                    
                }
            }
            if(total_value < 0){
                total_value = 0;
                counter++;
            }
            new_img->data[img_x + img_y * new_img->width] = (int)total_value;
        }
    }
}
// arithmetic(info, temp) adds up the red, green, and blue values in info and divides it by 3. Saves the answer in temp and returns it
int arithmetic(PixelInfo *info, int *temp) {
    *temp = (info->red + info->green + info->blue) / 3;
    //int status = sprintf(str, "#%02x%02x%02x", *temp, *temp, *temp);
    return *temp;
}
// geometric(info, temp) multiplies up the red, green, and blue values in info and cube roots. Saves the answer in temp and returns it
int geometric(PixelInfo *info, int *temp) {
    *temp = pow((info->red * info->green * info->blue), 0.334);
    //int status = sprintf(str, "#%02x%02x%02x", *temp, *temp, *temp);
    return *temp;
}

int set_convolution_func(char *str) {
    if(*str == '1'){
        avg_fun = &arithmetic;
    }else if(*str == '2'){
        avg_fun = &geometric;
    }else{
        printf("Unknown function. %s\n", str);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int set_convolution_size(char *str) {
    int temp = -1;
    int status = sscanf(str, "%d", &temp);
    if(status != 1 || temp < 2 || temp > 100){
        return EXIT_FAILURE;
    }
    convolution_size = temp;
    convolution_area = convolution_size * convolution_size;
    return EXIT_SUCCESS;
}

int set_cutoff(char *str) {
    int temp = -1;
    int status = sscanf(str + 1, "%d", &temp);
    if(status != 1 || temp < 0){
        return EXIT_FAILURE;
    }
    if(*str == 'c') {
        combine_color_cutoff = temp;
    }else if(*str == 's') {
        saturation_pattern_cutoff = temp;
    }else if(*str == 'n') {
        ascii_entropy_ignore = temp;
    }else if(*str == 'i') {
        ascii_min_intensity = temp;
    }else if(*str == 'N') {
        ignore_noise_cutoff = temp;
    }else {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int set_noise(char *str) {
    int temp = -1;
    int status = sscanf(str, "%d", &temp);
    if(status != 1 || temp < 0){
        return EXIT_FAILURE;
    }
    use_random = temp;
    return EXIT_SUCCESS;
}

int set_extension(char str[]) {
    int count = 0;
    while(str[count] != '\0'){
        if(count == 0 && str[count] != '.') {
            printf("Extension wrong format, must start with dot.\n");
            return EXIT_FAILURE;
        }else if(!('a' <= str[count] && str[count] <= 'z')) {
            printf("Extension wrong format. lowercase alpha only.\n");
            return EXIT_FAILURE;
        }
        count++;
    }
    if(count < 3 || count > 5){
        printf("Extension wrong format. not supported.\n");
        return EXIT_FAILURE;
    }
    for(int i = 1; i < count; i++) {
        output_extension[i] = str[i];
    }
    return EXIT_SUCCESS;
}

int set_grid_on(char *str) {
    int temp = -1;
    int status = sscanf(str, "%d", &temp);
    if(status != 1){
        return EXIT_FAILURE;
    }
    if(temp == 1) {
        grid_on = 1;
    }else {
        grid_length = temp;
    }
    
    return EXIT_SUCCESS;
}

int set_output_dir(char *str) {
    output_dir = malloc((strlen(str) + 1) * sizeof(char));
    strcpy(output_dir, str);
    char temp[] = "/";
    strcat(output_dir, temp);
    return EXIT_SUCCESS;
}


// to_image(img) saves an image to the current directory
int to_image(struct image_meta *img) {
    MagickWandGenesis();
    MagickWand *mw = NewMagickWand();
    PixelWand *pw = NewPixelWand();
    PixelWand **pixels;
    PixelSetColor(pw,"white");
    if(MagickSetSize(mw, img->width, img->height) == MagickFalse){ return EXIT_FAILURE; }
    if(MagickNewImage(mw, img->width, img->height, pw) == MagickFalse){ return EXIT_FAILURE; }
    MagickResetIterator(mw);
    PixelIterator *iterator = NewPixelIterator(mw);
    unsigned long width;
    int temp;
    char tempstr[20];
    if(iterator == (PixelIterator *) NULL){ return EXIT_FAILURE; }
    for (long y = 0; y < img->height; y++){
        pixels = PixelGetNextIteratorRow(iterator, &width);
        if (pixels == (PixelWand **) NULL){
            break;
        }
        for (long x=0; x < (long) width; x++){
            temp = img->data[x + y * img->width];
            sprintf(tempstr, "#%04x%04x%04x", temp, temp, temp);
            PixelSetColor(pixels[x], tempstr);
            if(grid_on && ((x+1) % grid_length == 0 || (y+1) % grid_length == 0)) {
                PixelSetColor(pixels[x], grid_color);
            }
        }
        PixelSyncIterator(iterator);
    }
    iterator=DestroyPixelIterator(iterator);
    if(MagickWriteImage(mw, img->name) == MagickFalse){ return EXIT_FAILURE; }
    pw = DestroyPixelWand(pw);
    mw = DestroyMagickWand(mw);
    MagickWandTerminus();
    return EXIT_SUCCESS;
}

// combine(imgs, colors, length, cutoff) compares the given images, colors the corresponding pixel in the result based on the color at the
//   same index as the image with the brightest pixel.
// Require: the length of imgs and colors be the same,
int combine(const struct image_meta *imgs, const char * const colors[], int length, int cutoff) {
    assert(imgs);
    assert(colors);
    assert(length > 0);
    assert(cutoff >= 0);
    MagickWandGenesis();
    MagickWand *mw = NewMagickWand();
    PixelWand *pw = NewPixelWand();
    PixelWand **pixels;
    PixelSetColor(pw,"black");
    if(MagickSetSize(mw, imgs->width, imgs->height) == MagickFalse){ return EXIT_FAILURE; }
    if(MagickNewImage(mw, imgs->width, imgs->height, pw) == MagickFalse){ return EXIT_FAILURE; }
    MagickResetIterator(mw);
    PixelIterator *iterator = NewPixelIterator(mw);
    unsigned long width;
    int max_value = 0;
    int max_index = -1;
    int temp_value = 0;
    for(int y = 0; y < imgs->height; y++){
        pixels = PixelGetNextIteratorRow(iterator, &width);
        if (pixels == (PixelWand **) NULL){
            break;
        }
        for(int x = 0; x < imgs->width; x++){
            max_value = 0;
            max_index = -1;
            for(int i = 0; i < length; i++) {
                temp_value = imgs[i].data[x + y * imgs->width];
                if(temp_value >= max_value){
                    max_value = temp_value;
                    max_index = i;
                }
            }
            if(grid_on && ((x+1) % grid_length == 0 || (y+1) % grid_length == 0)){
                PixelSetColor(pixels[x], grid_color);
            }else if(max_value > cutoff * quantum_range / 100){
                PixelSetColor(pixels[x], colors[max_index]);
                //PixelSetColor(pixels[x], "#000000000000");
            }
            
        }
        
        PixelSyncIterator(iterator);
    }
    iterator=DestroyPixelIterator(iterator);
    char *temp_str = malloc((strlen(output_dir) + 20) * sizeof(char));
    sprintf(temp_str, "%scombined%d%s", output_dir, length, output_extension);
    if(MagickWriteImage(mw, temp_str) == MagickFalse){ printf("Unable to write combined image.\n"); return EXIT_FAILURE; }
    pw = DestroyPixelWand(pw);
    mw = DestroyMagickWand(mw);
    free(temp_str);
    MagickWandTerminus();
    return EXIT_FAILURE;
}

char to_ascii_lookup(struct ascii_pixel *pix) {
    //
    if(pix->total_intensity < ascii_min_intensity * quantum_range / 100) {
        return ' ';
    }
    if(pix->noise > ascii_entropy_ignore) {
        return '#';
    }
    int max_index = -1;
    int max_intensity = 0;
    for(int i = 0; i < pix->len; i++) {
        if(pix->intensity[i] > max_intensity) {
            max_index = i;
            max_intensity = pix->intensity[i];
            //printf("%d: range(%d, %d) intensity:%d , noise:%d\n",i, pix->range_min[i], pix->range_max[i], pix->intensity[i], pix->noise);
        }
    }
    if(max_index == 0 || max_index == 1) {
        return '|';
    }else if (max_index == 2 || max_index == 3) {
        return '-';
    }else if (max_index == 4 || max_index == 5) {
        return '/';
    }else if (max_index == 6 || max_index == 7) {
        return '\\';
    }else {
        return '@';
    }
}

// to_ascii(imgs, length, pixel_size) TODO track average positions of each intensity, ie close to the center or edges
int to_ascii(const struct image_meta *imgs, int length, int pixel_size) {
    printf("starting ascii conversion.\n");
    int rows = imgs->height / pixel_size;
    int cols = imgs->width / pixel_size;
    int temp_i = 0;
    struct ascii_pixel *pix = malloc(sizeof(struct ascii_pixel));
    pix->len = length;
    pix->pixel_size = pixel_size;
    int *range_min = malloc(length * sizeof(int));
    int *range_max = malloc(length * sizeof(int));
    int *intensity = malloc(length * sizeof(int));
    int *data = malloc(length * pixel_size * pixel_size * sizeof(int));
    pix->range_min = range_min;
    pix->range_max = range_max;
    pix->intensity = intensity;
    pix->data = data;
    int temp_pix_data;
    for(int row = 0; row < rows; row++) {
        for(int col = 0; col < cols; col++) {
            //printf("R%d C%d :", row, col);
            pix->noise = 0;
            pix->total_intensity = 0;
            for(int i = 0; i < length; i++){
                pix->range_min[i] = quantum_range;
                pix->range_max[i] = 0;
                pix->intensity[i] = 0;
                for(int pix_row = 0; pix_row < pixel_size; pix_row++) {
                    for(int pix_col = 0; pix_col < pixel_size; pix_col++) {
                        temp_i = (row * pixel_size + pix_row) * imgs->width + col * pixel_size + pix_col;
                        temp_pix_data = pix->data[i * pixel_size * pixel_size + pix_row * pixel_size + pix_col] = imgs[i].data[temp_i];
                        if(imgs[i].data[temp_i] < saturation_pattern_cutoff * quantum_range / 100) {
                            continue;
                        }
                        pix->intensity[i] += temp_pix_data;
                        if(temp_pix_data < pix->range_min[i]) {
                            pix->range_min[i] = temp_pix_data;
                        }
                        if(temp_pix_data > pix->range_max[i]) {
                            pix->range_max[i] = temp_pix_data;
                        }
                    }
                }
                if(pix->intensity[i] != 0) {
                    pix->intensity[i] /= pixel_size * pixel_size;
                    pix->total_intensity += pix->intensity[i];
                    if(pix->intensity[i] > ignore_noise_cutoff * quantum_range / 100) {
                        pix->noise++;
                    }
                }
                
            }
            printf("%c", to_ascii_lookup(pix));
            

        }
        printf("\n");
    }
    free(range_min);
    free(range_max);
    free(intensity);
    free(data);
    free(pix);
    return 0;
}

/*
./analyse <filename> [-options]
 -f<1,2> averaging function, default is arithmatic
 -n<number> uses the convolution matrix of size number * number, default is 3 * 3
 -e<extension> output file type, default is .png
 -c<number> sets the cutoff when combining images in percent, default 10%
 -g<number> draws a grid of size number on the images generated, default off
 -N<number> adds noise to resolve ties, default is off
 -t(patterns, image) testing mode for given value
takes the name of an image in the current directory
opens the image with magickopenimage and puts the averaged rgb data in an array
loop through convolution functions and generate a grey-scale image per function
*/

int main(int argsc, char **argsv) {
    sscanf(MagickQuantumRange, "%d", &quantum_range);
    // parse options
    if(argsc < 2){
        printf("image name not provided.\n");
        exit(EXIT_FAILURE);
    }
    char option = '\0';
    int status = 0;
    for(int i = 2; i < argsc; i++){
        if(argsv[i][0] != '-'){
            printf("Options must start with -\n");
            exit(EXIT_FAILURE);
        }
        option = argsv[i][1];
        if(option == 'f'){
            status = set_convolution_func(&argsv[i][2]);
        }else if(option == 'n'){
            status = set_convolution_size(&argsv[i][2]);
        }else if(option == 'e'){
            status = set_extension(&argsv[i][2]);
        }else if(option == 'c'){
            status = set_cutoff(&argsv[i][2]);
        }else if(option == 't'){
            show_steps = 1;
        }else if(option == 'g'){
            status = set_grid_on(&argsv[i][2]);
        }else if(option == 'N'){
            status = set_noise(&argsv[i][2]);
        }else if(option == 'o'){
            status = set_output_dir(&argsv[i][2]);
        }else{
            printf("Option not known: %c\n", option);
            exit(EXIT_FAILURE);
        }
        if(status != EXIT_SUCCESS){
            printf("Error parsing option -%c", option);
            exit(EXIT_FAILURE);
        }
    }
    printf("Options parsed.\n");

    // initiate data
    struct image_meta img = { argsv[1] };
    MagickWandGenesis();
    MagickWand *mw = NewMagickWand();
    
    MagickSetDepth(mw, sizeof(int));
    if(MagickReadImage(mw, argsv[1]) == MagickFalse){ printf("Cannot read image: %s\n", img.name); exit(EXIT_FAILURE); }
    img.width = MagickGetImageWidth(mw);
    img.height = MagickGetImageHeight(mw);
    if(img.width < convolution_size || img.height < convolution_size){ printf("size too big for this image.\n"); exit(EXIT_FAILURE); }
    int image_data[img.width * img.height];
    img.data = image_data;
    PixelIterator *iterator = NewPixelIterator(mw);
    if(iterator == (PixelIterator *) NULL){ printf("Iterator can not be created.\n"); exit(EXIT_FAILURE); }
    PixelInfo info;
    PixelWand **pixels;
    unsigned long width;
    for (long y=0; y < img.height; y++){
        pixels = PixelGetNextIteratorRow(iterator, &width);
        if (pixels == (PixelWand **) NULL){
           break;
        }
        for (long x=0; x < (long) width; x++){
            PixelGetMagickColor(pixels[x], &info);
            avg_fun(&info, &image_data[y * img.width + x]);
            //image_data[y * img.width + x] /= 256;
            //printf("%d,",image_data[y * img.width + x]);
        }
    }
    printf("depth:%d\n", quantum_range);
    // clean up
    iterator = DestroyPixelIterator(iterator);
    mw = DestroyMagickWand(mw);
    MagickWandTerminus();
    printf("Image put in memory.\n");

    // analyse with each pattern
    struct image_meta imgs[convolution_pattern_length];
    char tempstr[100];
    const int tempwidth = img.width - convolution_size + 1;
    const int tempheight = img.height - convolution_size + 1;
    int new_image_data[tempwidth * tempheight * convolution_pattern_length];
    for(int i = 0; i < convolution_pattern_length; i++){
        sprintf(tempstr, "%simage_%d%s", output_dir, i, output_extension);
        imgs[i] = (struct image_meta){ tempstr, tempwidth, tempheight, new_image_data + tempwidth * tempheight * i };
        apply(&img, imgs+i, convolution_size, convolution_pattern_basic[i]);
        if(show_steps != 0){
            if(to_image(imgs+i) == EXIT_FAILURE){ printf("Error writing: %s", imgs[i].name);}
        }
        
    }
    sprintf(tempstr, "%snew_image%s", output_dir, output_extension);
    img.name = tempstr;
    if(show_steps != 0){ 
        if(to_image(&img) == EXIT_FAILURE){ printf("Error writing: %s", img.name);}
        combine(imgs, COLORS, convolution_pattern_length, combine_color_cutoff);
    }
    to_ascii(imgs, convolution_pattern_length, grid_length);
    printf("Done\n");
    printf("counter: %d\n", counter);
}