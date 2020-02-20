# BMPCoder

A simple utility for handling BMP files and encoding "secret" messages within them.

Check program output for usage insructions.

Every single pixel in a bitmap has 32 bits worth of data. Most of this is unnecessary as the color accuracy achieved this way is much higher than can actually be seen or even represented on a screen. Because of this a small decrease in color accuracy is neglible.

This also makes it possible to "steal" the last bit of every pixel for other uses. I.e. the bitmap will not change noticeably if every pixels last bit is changed. That is what this program does.

A message is simply coded into a bitmap by changing the last bit of every pixel to a bit from the message. Decoding is also obviously possible.
