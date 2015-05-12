#include "gd.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <gdfontg.h>
#include <string.h>
#include "assert.h"
#include "myeyes.h"

t_captcha getCaptchaBuf( int width, int height,  int framesCount, int frameRate, char* text) {
  srand(time(NULL));
  /* Declare the image */
  gdImagePtr im, mask, maskEnlarged;
  t_captcha ret;
  FILE *gifout;
  char *imageBuffer;
  char *maskBuffer, *enlargedMaskBuffer;
  size_t imageBufferSize;
  int x, y, i, textLength, textHeight;
  int colors[10];
  int color;

  gifout = open_memstream(&imageBuffer, &imageBufferSize);
  im = gdImageCreate(width, height);
  maskEnlarged = gdImageCreateTrueColor(width, height);


  //generate color table
  for (i = 0; i < 10; i++) {
    color = (rand() % 25) * 10;
    colors[i] = gdImageColorAllocate(im, color, color, color);
  }

  textLength = gdFontGetGiant()->w * strlen(text);
  textHeight = gdFontGetGiant()->h;

  maskBuffer = malloc(textLength*textHeight);
  enlargedMaskBuffer = malloc(width*height);
  mask = gdImageCreateTrueColor(textLength, textHeight);

  for(i=0; i < textLength*textHeight; i++) {
    maskBuffer[i] = colors[ rand() % 2 ];
  }
  //add text
  gdImageString(mask, gdFontGetGiant(), 0, 0, (unsigned char*) text,  gdImageColorAllocate(im, 255, 255, 255));
  gdImageCopyResized(maskEnlarged, mask, 0,0,0,0, width, height, textLength, textHeight);

  for(y = 0; y < maskEnlarged->sy; y++) {
    for(x = 0; x < maskEnlarged->sx; x++ ) {
      enlargedMaskBuffer[x*y] = rand()%10;
    }
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

    gdImageGifAnimAdd(im, gifout, 0, 0, 0, frameRate, 1, NULL);
  }

  gdImageGifAnimEnd(gifout);

  fclose(gifout);
  gdImageDestroy(im);
  gdImageDestroy(mask);
  free (maskBuffer);
  free (enlargedMaskBuffer);

  ret.buf = imageBuffer;
  ret.width = width;
  ret.size = imageBufferSize;

  return ret;
}
