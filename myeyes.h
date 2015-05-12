
typedef struct t_captcha {
  int width;
  int height;
  int size;
  char* buf;
} t_captcha;

t_captcha getCaptchaBuf( int width, int height,  int framesCount, int frameRate, char* text);
