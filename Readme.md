MyEyesCaptcha

Creates simple Gif89a Captcha, with static noise to stdout
99.91% ocrproof, 42% humanproof.

Only stdin output is supported TFN ¯\\_(ツ)_/¯

build: cc myeyes.c myeyestofile.c -lgd -o myeyes

usage:
./myeyes -t \<TEXT\>

Another options:

w: width - max width is MAX_WIDTH;
h: height - max is MAX_HEIGHT;
r: frame rate - 1-... (1/100ms * r)
c: frame count - min MIN_FRAME_COUNT, max - MAX_FRAME_COUNT;
t: text - not larger than MAX_CHARS or it will be truncated

Todo:
Add todo
