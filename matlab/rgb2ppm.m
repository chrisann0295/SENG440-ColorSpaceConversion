INPUT_FILE = 'image.jpg';
OUTPUT_FILE = 'image.txt';

% R = [ 1, 1 ; 2, 2 ; 2, 2 ];
% G = [ 3, 3 ; 4, 4 ; 4, 4 ];
% B = [ 5, 5 ; 6, 6 ; 6, 6 ];

RGB = imread(INPUT_FILE);

fileID = fopen(OUTPUT_FILE,'w');

fprintf(fileID, 'P6\n');
fprintf(fileID, '%d %d\n', num_cols, num_rows);
fprintf(fileID, '255\n');

%[num_rows, num_cols, num_pixels] = size(RGB);

%for r = 1:num_rows    
%    for c = 1:num_cols
%        fprintf(fileID, '%d ', RGB(r, c, 1));
%        fprintf(fileID, '%d ', RGB(r, c, 2));
%        fprintf(fileID, '%d ', RGB(r, c, 3));
%    end
%    fprintf(fileID, '\n');
%end

fclose(fileID);

dlmwrite(OUTPUT_FILE, RGB, '-append', 'delimiter', ' ');
