FILENAME = 'image.jpg';
OUTPUT_Y = 'y.txt';
OUTPUT_CB = 'cb.txt';
OUTPUT_CR = 'cr.txt';

RGB = imread(FILENAME);

YCBCR = rgb2ycbcr(RGB);

Y = YCBCR(:,:,1);
Cb = YCBCR(:,:,2);
Cr = YCBCR(:,:,3);

dlmwrite(OUTPUT_Y, Y, ' ');
dlmwrite(OUTPUT_CB, Cb, ' ');
dlmwrite(OUTPUT_CR, Cr, ' ');
