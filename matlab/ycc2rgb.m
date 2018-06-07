FILENAME_Y = "y.txt";
FILENAME_CB = "cb.txt";
FILENAME_CR = "cr.txt";

% Load 2D arrays from file
% Values should be space separated
Y = uint8(importdata(FILENAME_Y));
Cb = uint8(importdata(FILENAME_CB));
Cr = uint8(importdata(FILENAME_CR));

% TODO: Upsample

% Combine the arrays into a 3D array
% The size will be YCBCR[height][width][3]
YCBCR = cat(3, Y, Cb, Cr);

% Convert YCC to RGB
RGB = ycbcr2rgb(YCBCR);

% Show the image
figure
imshow(RGB);
