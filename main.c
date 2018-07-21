#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h> 

int main(int argc, char *argv[]) {
  FILE * fp;
  fp = fopen(argv[1], "r");
  if (fp == NULL) {
    fprintf(stderr, "Please enter a valid file path\n");
    exit(EXIT_FAILURE);
  }

  //check the image format
  //ref: https://en.wikipedia.org/wiki/Netpbm_format
  char * filetype = NULL;
  size_t len = 0;
  ssize_t nread;
  nread = getline(&filetype, &len, fp);
  if (filetype[0] != 'P' || filetype[1] != '6') {
    fprintf(stderr, "Invalid image format (must be 'P6')\n");
    exit(EXIT_FAILURE);
  }

  //read image size information
  int height;
  int width;
  if (fscanf(fp, "%d %d", &width, &height) != 2) {
    fprintf(stderr, "Invalid image size (error loading '%s')\n", argv[1]);
    exit(EXIT_FAILURE);
  }
  printf("height: %d, width: %d\n", height, width);

  //read and check rgb range
  int range;
  if (fscanf(fp, "%d", &range) != 1) {
    fprintf(stderr, "Invalid rgb range (error loading '%s')\n", argv[1]);
    exit(1);
  }
  if(range != 255) {
    fprintf(stderr, "Invalid rgb range, must be 255\n");
    exit(1);
  }
  printf("range: %d\n", range);

  char c = fgetc(fp);
  while (c != '\n') {
    printf("%c\n", c);
    c = fgetc(fp);
  }

  // allocate memory to rgb arrays
  int i;
  uint8_t **r = malloc(height * sizeof(uint8_t *));
  uint8_t **g = malloc(height * sizeof(uint8_t *));
  uint8_t **b = malloc(height * sizeof(uint8_t *));
  for (i=0; i < height; i++) {
    r[i] = malloc(width * sizeof(uint8_t));
    g[i] = malloc(width * sizeof(uint8_t));
    b[i] = malloc(width * sizeof(uint8_t));
  }

  // read in values
  int j, pixel;
  for(i = 0; i < height; i++){
    for(j=0; j < width * 3; j++){
      int temp;
      fscanf(fp, "%d", &temp);
      if(j < width)
        r[i][j % width] = (uint8_t)temp;
      else if(j < (2*width))
        g[i][j % width] = (uint8_t)temp;       
      else
        b[i][j % width] = (uint8_t)temp;
    }
  }

  /* --------- ORIGINAL CODE: No downsampling ----------*/
  // uint8_t **y = malloc(height * sizeof(uint8_t *));
  // uint8_t **cb = malloc(height * sizeof(int8_t *));
  // uint8_t **cr = malloc(height * sizeof(int8_t *));
  // for (i=0; i < height; i++) {
  //   y[i] = malloc(width * sizeof(uint8_t));
    // cb[i] = malloc(width * sizeof(uint8_t));
    // cr[i] = malloc(width * sizeof(uint8_t));
  // }

  // calculate and store ycc values
  // REDUCE CACHE MISSES: https://m.eet.com/media/1157397/atc-152paper_shore_v4.pdf
  // for(i = 0; i < height; i++){
  //   for(j=0; j < width; j++){
      // UPDATED: use a scale factor of 255
      // y[i][j]  = ((66 * r[i][j] + 129 * g[i][j] + 25 * b[i][j]) >> 8) + 16;
      // cb[i][j] = ((-38 * r[i][j] - 75 * g[i][j] + 112 * b[i][j]) >> 8) + 128; 
      // cr[i][j] = ((112 * r[i][j] - 94 * g[i][j] - 18 * b[i][j]) >> 8) + 128;

      // ORIGINAL: float arithmatic
      // y[i][j]  = ( 65.481 * r[i][j] + 128.5553 * g[i][j] + 24.966 * b[i][j])/255 + 16;
      // cb[i][j] = (-37.797 * r[i][j] - 74.203 * g[i][j] + 112.0 * b[i][j])/255 + 128;
      // cr[i][j] = ( 112.0 * r[i][j] - 93.786 * g[i][j] - 18.214 * b[i][j])/255 + 128;
    // }
  // }
  /* --------- END ORIGINAL CODE ----------*/

  /* --------- DOWNSAMPLING CODE (Discarding values) ----------*/
  int ds_height = height/2;
  int ds_width = width/2;
  uint8_t **y = malloc(height * sizeof(uint8_t *));
  uint8_t **cb = malloc(ds_height * sizeof(int8_t *));
  uint8_t **cr = malloc(ds_height * sizeof(int8_t *));
  for (i=0; i < height; i++) {
    y[i] = malloc(width * sizeof(uint8_t));
    if (i < ds_height) {
      cb[i] = malloc(ds_width * sizeof(uint8_t));
      cr[i] = malloc(ds_width * sizeof(uint8_t));
    }
  }

  int u = 0;
  int v = 0;
  for(i = 0; i < height; i++){
    for(j=0; j < width; j++){
      y[i][j]  = ((66 * r[i][j] + 129 * g[i][j] + 25 * b[i][j]) >> 8) + 16;
      if((i % 2 == 0) && (j % 2 == 0) && u < ds_height && v < ds_width) {
        cb[u][v] = ((-38 * r[i][j] - 75 * g[i][j] + 112 * b[i][j]) >> 8) + 128; 
        cr[u][v] = ((112 * r[i][j] - 94 * g[i][j] - 18 * b[i][j]) >> 8) + 128;
        v++;
      }
    }
    v = 0;
    if(i % 2 == 0)
      u++;
  }
  /* --------- END DOWNSAMPLING CODE ----------*/


  /* --------- DOWNSAMPLING BY AVG CODE  (Averaging 9 values) ----------*/
  int ds_avg_height = height/2;
  int ds_avg_width = width/2;

  // allocating arrays
  uint8_t **avg_y = malloc(ds_avg_height * sizeof(uint8_t *));
  uint8_t **avg_cb = malloc(ds_avg_height * sizeof(int8_t *));
  uint8_t **avg_cr = malloc(ds_avg_height * sizeof(int8_t *));
  for (i=0; i < ds_avg_height; i++) {
    avg_y[i] = malloc(ds_avg_width * sizeof(uint8_t));
    if (i < ds_avg_height) {
      avg_cb[i] = malloc(ds_avg_width * sizeof(uint8_t));
      avg_cr[i] = malloc(ds_avg_width * sizeof(uint8_t));
    }
  }

  int new_row = 0;
  int new_col = 0;
  uint8_t avg_r = 0;
  uint8_t avg_g = 0;
  uint8_t avg_b = 0;

  i = 0;
  j = 0;
  while(i + 2 < height) {
    while(j + 2 < width) {
      avg_y[new_row][new_col]  = ((66 * r[i][j] + 129 * g[i][j] + 25 * b[i][j]) >> 8) + 16;

      // average within a 3x3 window
      avg_r = (r[i][j] + r[i][j+1] + r[i][j+2] + r[i+1][j] + r[i+1][j+1] + r[i+1][j+2] + r[i+2][j] + r[i+2][j+1] + r[i+2][j+2])/9;
      avg_g = (g[i][j] + g[i][j+1] + g[i][j+2] + g[i+1][j] + g[i+1][j+1] + g[i+1][j+2] + g[i+2][j] + g[i+2][j+1] + g[i+2][j+2])/9;
      avg_b = (b[i][j] + b[i][j+1] + b[i][j+2] + b[i+1][j] + b[i+1][j+1] + b[i+1][j+2] + b[i+2][j] + b[i+2][j+1] + b[i+2][j+2])/9;

      avg_cb[new_row][new_col] = ((-38 * avg_r - 75 * avg_g + 112 * avg_b) >> 8) + 128; 
      avg_cr[new_row][new_col] = ((112 * avg_r - 94 * avg_g - 18 * avg_b) >> 8) + 128;

      // move window by 2 slots for a smaller image size
      j = j+2;
      new_col++;
    }

    j = 0;
    new_col = 0;
    new_row++;
    i = i+2;
  }
  /* --------- END DOWNSAMPLING BY AVG CODE ----------*/
  
  /* --------- WRITE THE DOWNSAMPLED (BY AVG) VALUES TO YCC FILES ----------*/
  FILE *fdy = fopen("y.txt", "w");
  FILE *fdcb = fopen("cb.txt", "w");
  FILE *fdcr = fopen("cr.txt", "w");

  for(i = 0; i < ds_avg_height; i++){
    for(j=0; j < ds_avg_width; j++){
      fprintf(fdy, "%d ", avg_y[i][j]);
    }
    fprintf(fdy, "\n");
  }
  for(i = 0; i < ds_avg_height; i++){
    for(j=0; j < ds_avg_width; j++){
      fprintf(fdcb, "%d ", avg_cb[i][j]);
    }
    fprintf(fdcb, "\n");
  }
  for(i = 0; i < ds_avg_height; i++){
    for(j=0; j < ds_avg_width; j++){
      fprintf(fdcr, "%d ", avg_cr[i][j]);
    }
    fprintf(fdcr, "\n");
  }

  /* --------- END WRITING THE DOWNSAMPLED (BY AVG) VALUES TO YCC FILES ----------*/

  fclose(fp);

  for(i = 0; i < height; i++){
    free(r[i]);  
    free(g[i]);
    free(b[i]);      
    
    free(y[i]);
    if (i < ds_height) {  
      free(cb[i]);      
      free(cr[i]);      
    }
  }
  free(r);
  free(g);
  free(b);
  
  free(y);
  free(cb);
  free(cr);
}

/* --------- WRITE THE DOWNSAMPLED VALUES TO YCC FILES ----------*/
//   FILE *fdy = fopen("y.txt", "w");
//   FILE *fdcb = fopen("cb.txt", "w");
//   FILE *fdcr = fopen("cr.txt", "w");

//   for(i = 0; i < height; i++){
//     for(j=0; j < width; j++){
//       fprintf(fdy, "%d ", y[i][j]);
//     }
//     fprintf(fdy, "\n");
//   }
//   for(i = 0; i < ds_height; i++){
//     for(j=0; j < ds_width; j++){
//       fprintf(fdcb, "%d ", cb[i][j]);
//     }
//     fprintf(fdcb, "\n");
//   }
//   for(i = 0; i < ds_height; i++){
//     for(j=0; j < ds_width; j++){
//       fprintf(fdcr, "%d ", cr[i][j]);
//     }
//     fprintf(fdcr, "\n");
//   }

//   fclose(fp);

//   for(i = 0; i < height; i++){
//     free(r[i]);  
//     free(g[i]);
//     free(b[i]);      
    
//     free(y[i]);
//     if (i < ds_height) {  
//       free(cb[i]);      
//       free(cr[i]);      
//     }
//   }
//   free(r);
//   free(g);
//   free(b);
  
//   free(y);
//   free(cb);
//   free(cr);
// }
/* --------- END WRITING THE DOWNSAMPLED VALUES TO YCC FILES ----------*/
