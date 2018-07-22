#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>

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
  register int i, j;
  int colsize = height * sizeof(uint8_t *);
  int rowsize = width * sizeof(uint8_t *);
  int tempwidth = height - (height & 1);
  int ds_height = height >> 1;
  int ds_width = width >> 1;
  int ds_colsize = colsize >> 1;
  int ds_rowsize = rowsize >> 1;
  uint8_t **r = malloc(colsize);
  uint8_t **g = malloc(colsize);
  uint8_t **b = malloc(colsize);
  uint8_t **y = malloc(colsize);
  uint8_t **cb = malloc(ds_colsize);
  uint8_t **cr = malloc(ds_colsize);
  for (i = 0, j = 0; i < tempwidth; i++) {
    r[i] = malloc(rowsize);
    g[i] = malloc(rowsize);
    b[i] = malloc(rowsize);
    y[i] = malloc(rowsize);
    i++;
    cb[j] = malloc(ds_rowsize);
    cr[j] = malloc(ds_rowsize);
    j++;

    r[i] = malloc(rowsize);
    g[i] = malloc(rowsize);
    b[i] = malloc(rowsize);
    y[i] = malloc(rowsize);
  }
  if (height & 1) {
    r[i] = malloc(rowsize);
    g[i] = malloc(rowsize);
    b[i] = malloc(rowsize);
    y[i] = malloc(rowsize);
  }

  // read in values
  tempwidth = ((width >> 2) << 2) - 2;
  for(i = 0; i < height; i++){
    // R
    for (j = 0; j < tempwidth; j += 4){
      fscanf(fp, "%d%d%d%d", &r[i][j], &r[i][j+1], &r[i][j+2], &r[i][j+3]);
    }
    for (; j < width; j++) {
      fscanf(fp, "%d", &r[i][j]);
    }

    // G
    for(j = 0; j < tempwidth; j += 4){
      fscanf(fp, "%d%d%d%d", &g[i][j], &g[i][j+1], &g[i][j+2], &g[i][j+3]);
    }
    for (; j < width; j++) {
      fscanf(fp, "%d", &g[i][j]);
    }

    // B
    for(j = 0; j < tempwidth; j += 4){
      fscanf(fp, "%d%d%d%d", &b[i][j], &b[i][j+1], &b[i][j+2], &b[i][j+3]);
    }
    for (; j < width; j++) {
      fscanf(fp, "%d", &b[i][j]);
    }
  }

  /* --------- DOWNSAMPLING CODE ----------*/

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

  FILE *fdy = fopen("y.txt", "w");
  FILE *fdcb = fopen("cb.txt", "w");
  FILE *fdcr = fopen("cr.txt", "w");

  for(i = 0; i < height; i++){
    for(j=0; j < width; j++){
      fprintf(fdy, "%d ", y[i][j]);
    }
    fprintf(fdy, "\n");
  }
  for(i = 0; i < ds_height; i++){
    for(j=0; j < ds_width; j++){
      fprintf(fdcb, "%d ", cb[i][j]);
    }
    fprintf(fdcb, "\n");
  }
  for(i = 0; i < ds_height; i++){
    for(j=0; j < ds_width; j++){
      fprintf(fdcr, "%d ", cr[i][j]);
    }
    fprintf(fdcr, "\n");
  }

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