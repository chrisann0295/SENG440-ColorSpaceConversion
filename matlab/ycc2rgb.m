ORIGINAL_IMAGE = 'image.jpg';
FILENAME_Y = 'y.txt';
FILENAME_CB = 'cb.txt';
FILENAME_CR = 'cr.txt';

% see https://www.mathworks.com/help/images/ref/imresize.html#buxswkh-1-method
% choose from 'nearest' or 'bilinear'
UPSAMPLE_METHOD = 'nearest';

% Load 2D arrays from file
% Values should be space separated
Y = uint8(importdata(FILENAME_Y));
Cb = uint8(importdata(FILENAME_CB));
Cr = uint8(importdata(FILENAME_CR));

% Upsample
Cb = imresize(Cb, size(Y), UPSAMPLE_METHOD);
Cr = imresize(Cr, size(Y), UPSAMPLE_METHOD);

% Combine the arrays into a 3D array
% The size will be YCBCR[height][width][3]
YCBCR = cat(3, Y, Cb, Cr);

% Convert YCC to RGB
RGB = ycbcr2rgb(YCBCR);

% Show the image
figure
subplot(1,2,1)
imshow(imread(ORIGINAL_IMAGE));
title('Original');
subplot(1,2,2)
imshow(RGB);
title('Converted');
