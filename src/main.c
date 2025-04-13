
#include<stdio.h>
#include<stdint.h>
#include<math.h>
#include<assert.h>
#include<MagickWand/MagickWand.h>
#include"analyse.h"
#include"patterns.h"

#define DEGUB 0
// #define PRINTPATTERNS

///usr/local/Cellar/imagemagick/7.1.1-43/include/ImageMagick-7
///usr/local/Cellar/imagemagick/7.1.1-43/lib/pkgconfig
// tap and formula? https://github.com/tlk/homebrew-imagemagick-x11/tree/master?tab=readme-ov-file
/*/usr/bin/clang -fcolor-diagnostics -fansi-escape-codes -g -Wall 'MagickWand-config --cflags --cppflags' /Users/samhuang/Desktop/cs/image-fun/main.c -O2 -o /Users/samhuang/Desktop/cs/image-fun/main 'MagickWand-config --ldflags --libs'*/

// global data

int main (int argc, char **argv) {
   #ifdef PRINTPATTERNS
   for(int i = 0; i < use_patterns_len; i++) {
      print_pattern(5, use_patterns[i]);
   }
   #else
   return pipeline(argc, argv);
   #endif
}