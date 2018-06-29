FILENAME = 'image.jpg';
OUTPUT_Y = 'y_expected.txt';
OUTPUT_CB = 'cb_expected.txt';
OUTPUT_CR = 'cr_expected.txt';

RGB = imread(FILENAME);

YCBCR = rgb2ycbcr(RGB);

Y = YCBCR(:,:,1);
Cb = YCBCR(:,:,2);
Cr = YCBCR(:,:,3);

% Downsample
Cb = Cb(1:2:end,1:2:end);
Cr = Cr(1:2:end,1:2:end);

dlmwrite(OUTPUT_Y, Y, ' ');
dlmwrite(OUTPUT_CB, Cb, ' ');
dlmwrite(OUTPUT_CR, Cr, ' ');
