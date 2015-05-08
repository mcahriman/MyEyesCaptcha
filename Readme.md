MyEyesCaptcha

Creates simple Gif89a Captcha, with static noise to stdout
99.91% ocrproof, 42% humanproof.

Only stdin output is supported TFN ¯\_(ツ)_/¯

build: cc -lgd -o ./myeyescaptcha 

usage:
./myeyescaptcha <text> > file.gif 

