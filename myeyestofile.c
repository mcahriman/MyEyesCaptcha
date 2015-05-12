
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "myeyes.h"
#include "myeyestofile.h"

extern char *optarg;

int main(int argc, char** argv) {

  int width = DEFAULT_WIDTH;
  int height = DEFAULT_HEIGHT;
  int frameRate = DEFAULT_FRAME_RATE;
  int framesCount = DEFAULT_FRAME_COUNT;
  int textSet = FALSE;
  char* path = NULL;
  char text[MAX_CHARS + 1];

  t_captcha captcha;

  FILE* fout;

  int opt;
  /*
    w: width - max width is MAX_WIDTH;
    h: height - max is MAX_HEIGHT;
    r: frame rate - 1-... (1/100ms * r)
    c: frame count - min MIN_FRAME_COUNT, max - MAX_FRAME_COUNT;
    t: text - not larger than MAX_CHARS or it will be truncated
  */
  char* options = "w:h:r:c:t:f:";

  while((opt = getopt(argc, argv, options)) != -1) {
    switch(opt) {
      case 'w' :
        if(atoi(optarg) > 0 && atoi(optarg) <= MAX_WIDTH) {
          width = atoi(optarg);
        }
        break;
      case 'h' :
        if(atoi(optarg) > 0 && atoi(optarg) <= MAX_HEIGHT) {
          height = atoi(optarg);
        }
        break;
      case 'r' :
        if(atoi(optarg) > 0) {
          frameRate = atoi(optarg);
        }
        break;
      case 'c' :
        if(atoi(optarg) >= MIN_FRAME_COUNT && atoi(optarg) <= MAX_FRAME_COUNT) {
            framesCount = atoi(optarg);
          }
        break;
      case 't' :
        if(strlen(optarg) > 0) {
          //truncate
          strncpy ((char*) text, optarg, MAX_CHARS);
          textSet = TRUE;
        }
        break;
      case 'f' :
        if(strlen(optarg) > 0) {
          path = strdup(optarg); //valid path lol
        }
    }
  }


  if(!textSet) {
    fprintf( stderr, "Text is not set" );
    exit(1);
  }

  printf("Width = %i, Height: %i; Rate: %i; Count: %i; Text: %s; Path: %s\n",
      width, height, frameRate, framesCount, text, path);


  fout = fopen(path, "w");
  if (fout == NULL) {
    printf("Could not open destination file for writing, exiting\n");
    exit(1);
  }

  captcha = getCaptchaBuf( width, height,  framesCount, frameRate, text);

  fwrite(captcha.buf, captcha.size, 1, fout);
  fclose(fout);

  return 0; //success

}
