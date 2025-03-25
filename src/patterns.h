

#ifndef PATTERNS_H_
#define PATTERNS_H_

extern double vert_gain[];
extern double diagonal_gain[];
extern double (*convolution_pattern[])(int, int, int);
extern double (*convolution_pattern_basic[])(int, int, int);
extern int convolution_pattern_length;

double left_vert_pattern_basic(int , int , int );
double right_vert_pattern_basic(int , int , int );
double up_horz_pattern_basic(int , int , int );
double down_horz_pattern_basic(int , int , int );
double Q_one_diagonal_basic(int , int , int );
double Q_two_diagonal_basic(int , int , int );
double Q_three_diagonal_basic(int , int , int );
double Q_four_diagonal_basic(int , int , int );

double left_vert_pattern(int , int , int );
double right_vert_pattern(int , int , int );
double up_horz_pattern(int , int , int );
double down_horz_pattern(int , int , int );
double Q_one_diagonal(int , int , int );
double Q_two_diagonal(int , int , int );
double Q_three_diagonal(int , int , int );
double Q_four_diagonal(int , int , int );

void calibrate(int n);

void print_pattern(int , double (*)(int, int, int));

#endif