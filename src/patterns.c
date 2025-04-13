#include "patterns.h"
#include<stdio.h>
#include<math.h>

// calibration data
double vert_gain[] = {1,1,1,0.5,0.187,0.333,0.164,0.273,0.149,0.24};
double diagonal_gain[] = {1,1,1,0.6,0.46,0.389,0.345,0.313,0.291,0.273};
double vert_gain_basic[] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
double diagonal_gain_basic[] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
int convolution_pattern_length = 8;
double (*convolution_pattern[])(int, int, int) = {
    &left_vert_pattern,
    &right_vert_pattern,
    &up_horz_pattern,
    &down_horz_pattern,
    &Q_one_diagonal,
    &Q_two_diagonal,
    &Q_three_diagonal,
    &Q_four_diagonal
};

double (*convolution_pattern_basic[])(int, int, int) = {
    &left_vert_pattern_basic,
    &right_vert_pattern_basic,
    &up_horz_pattern_basic,
    &down_horz_pattern_basic,
    &Q_one_diagonal_basic,
    &Q_two_diagonal_basic,
    &Q_three_diagonal_basic,
    &Q_four_diagonal_basic
};

double (*base_convolution_pattern[])(int, int, int) = {
    &left_vert_pattern,
    &Q_one_diagonal,
    &left_vert_pattern_basic,
    &Q_one_diagonal_basic
};

double left_vert_pattern_basic(int x, int y, int size) {
    double temp = (size - 1) / 2.0 - x;
    if(temp == 0){
        return 0;
    }else if(temp > 0){
        return vert_gain_basic[size] / (double)(size * size - size % 2 * size);
    }else{
        return -vert_gain_basic[size] / (double)(size * size - size % 2 * size);
    }
}
double right_vert_pattern_basic(int x, int y, int size) {
    return -1 * left_vert_pattern_basic(x, y, size);
}
double up_horz_pattern_basic(int x, int y, int size) {
    return left_vert_pattern_basic(y, x, size);
}
double down_horz_pattern_basic(int x, int y, int size) {
    return -1 * up_horz_pattern_basic(x, y, size);
}
double Q_one_diagonal_basic(int x, int y, int size) {
    int temp = x - y;
    if(temp == 0){
        return 0;
    }else if(temp > 0){
        return diagonal_gain_basic[size] / (double)(size * size - size);
    }else{
        return -diagonal_gain_basic[size] / (double)(size * size - size);
    }
}
double Q_two_diagonal_basic(int x, int y, int size) {
    return Q_one_diagonal_basic(size - x - 1, y, size);
}
double Q_three_diagonal_basic(int x, int y, int size) {
    return Q_one_diagonal_basic(x, size - y - 1, size);
}
double Q_four_diagonal_basic(int x, int y, int size) {
    return Q_one_diagonal_basic(size - x - 1, size - y - 1, size);
}

double left_vert_pattern(int x, int y, int size) {
    double temp = (size - 1) / 2.0 - x;
    if(temp == 0){
        return 0;
    }else{
        return vert_gain[size] / (temp * size);
    }
}
double right_vert_pattern(int x, int y, int size) {
    return -1 * left_vert_pattern(x, y, size);
}
double up_horz_pattern(int x, int y, int size) {
    return left_vert_pattern(y, x, size);
}
double down_horz_pattern(int x, int y, int size) {
    return -1 * up_horz_pattern(x, y, size);
}
double Q_one_diagonal(int x, int y, int size) {
    int temp = x - y;
    if(temp == 0){
        return 0;
    }else{
        return diagonal_gain[size] / (temp * size);
    }
}
double Q_two_diagonal(int x, int y, int size) {
    return Q_one_diagonal(size - x - 1, y, size);
}
double Q_three_diagonal(int x, int y, int size) {
    return Q_one_diagonal(x, size - y - 1, size);
}
double Q_four_diagonal(int x, int y, int size) {
    return Q_one_diagonal(size - x - 1, size - y - 1, size);
}

double average(int x, int y, int size) {
    return 1.0 / (size * size);
}
double hill_average(int x, int y, int size) {
    if(x == 0 || y == 0 || x == size - 1 || y == size - 1) {
        return -0.5 / (4 * (size - 1));
    }else {
        return 0.5 / ((size - 2) * (size - 2));
    }
    // size /= 2;
    // x = x - size;
    // y = y - size;
    // x = x < 0 ? -x : x;
    // y = y < 0 ? -y : y;
    // return (double)(2.4 - x - y) / (size * size * 5);
}
double valley_average(int x, int y, int size) {
    if(x == 0 || y == 0 || x == size - 1 || y == size - 1) {
        return 0.5 / (4 * (size - 1));
    }else {
        return -0.5 / ((size - 2) * (size - 2));
    }
    // size /= 2;
    // x = x - size;
    // y = y - size;
    // x = x < 0 ? -x : x;
    // y = y < 0 ? -y : y;
    // return (double)(x + y - 2.4) / (size * size * 5);
}


// generates the calibration data for all patterns up to size n
void calibrate(int n) {
    for(int size = 3; size <= n; size++){

    }
}

// prints the given pattern to standard output
void print_pattern(int size, double (*foo)(int, int, int)){
    double sum = 0;
    double balence = 0;
    double value = 0;
    for(int y = 0; y < size; y++){
        for(int x = 0; x < size; x++){
            value = foo(x, y, size);
            value < 0 ? printf("%.2f ", value) : printf(" %.2f ", value);
            sum += value < 0 ? -value : value;
            balence += value;
        }
        printf("\n");
    }
    printf("The max value of this patterns is: %.2f\n", sum);
    printf("The balence of this patterns is: %.2f\n", balence);
}