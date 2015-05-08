/* Bring in gd library functions */
#include "gd.h"

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <gdfontg.h>
#include <string.h>
#include "assert.h"
int main(int argc, char** argv) {
  srand(time(NULL));
  /* Declare the image */
  gdImagePtr im, mask, maskEnlarged;
  FILE *gifout;
  char *imageBuffer;
  char *maskBuffer, *enlargedMaskBuffer;
  int maskbufferlength;
  size_t imageBufferSize;
  int x, y, i, xp, yp, textLength, textHeight;
  int colors[10];
  int color;
  int framesCount;
  char* text;

  int width = 200;
  int height = 60;
  framesCount = 64;

  gifout = open_memstream(&imageBuffer, &imageBufferSize);

  im = gdImageCreate(width, height);
  maskEnlarged = gdImageCreateTrueColor(width, height);

  //generate color table
  for (i = 0; i < 10; i++) {
    color = (rand() % 25) * 10;
    colors[i] = gdImageColorAllocate(im, color, color, color);
  }

  assert(argc>1);
  text = argv[1];

  textLength = gdFontGetGiant()->w * strlen(text);
  textHeight = gdFontGetGiant()->h;
  xp = width/2 - textLength / 2;
  yp = height/2 - textHeight / 2;

  maskBuffer = malloc(textLength*textHeight);
  enlargedMaskBuffer = malloc(width*height);
  mask = gdImageCreateTrueColor(textLength, textHeight);

  for(i=0; i < textLength*textHeight; i++) {
    maskBuffer[i] = colors[ rand() % 2 ];
  }
  //add text
  gdImageString(mask, gdFontGetGiant(), 0, 0, text,  gdImageColorAllocate(im, 255, 255, 255));
  gdImageCopyResized(maskEnlarged, mask, 0,0,0,0, width, height, textLength, textHeight);

  for(y = 0; y < maskEnlarged->sy; y++) {
    for(x = 0; x < maskEnlarged->sx; x++ ) {
      enlargedMaskBuffer[x*y] = rand()%10;
      //printf("%i", gdImageGetPixel(maskEnlarged, x,y));
    }
    //printf("\n");
  }

  //animate it
  gdImageGifAnimBegin(im, gifout, 1, 0);

  for(i = 0; i < framesCount; i++) {
    for(x = 0; x < width; x++) {
      for(y = 0; y < height; y++) {
        gdImageSetPixel(im, x, y, colors[ rand() % 10 ] );
      }
    }

    //apply mask
    for(x = 0; x < width; x++ ) {
      for(y = 0; y < height; y++) {
        int tpixel;
        tpixel = gdImageGetPixel(maskEnlarged, x, y);
        if(tpixel){
          int buffery;
          if ((i % height) + y >= height) {
            buffery = (i % height) + y - height;
          } else {
            buffery = i % height + y;
          }
          gdImageSetPixel(im, x, y, enlargedMaskBuffer[x*buffery]);
        }
      }
    }

    gdImageGifAnimAdd(im, gifout, 0, 0, 0, 5, 1, NULL);
  }


  fclose(gifout);
  gdImageDestroy(im);
  gdImageDestroy(mask);

  //stdout for now
  fwrite(imageBuffer, sizeof(char), imageBufferSize, stdout);
}
