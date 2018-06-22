INPUT_FILE = 'image.jpg';
OUTPUT_FILE = 'image.txt';

RGB = imread(INPUT_FILE);

fileID = fopen(OUTPUT_FILE,'w');

fprintf(fileID, 'P6\n');
fprintf(fileID, '%d %d\n', num_cols, num_rows);
fprintf(fileID, '255\n');

fclose(fileID);

dlmwrite(OUTPUT_FILE, RGB, '-append', 'delimiter', ' ');
