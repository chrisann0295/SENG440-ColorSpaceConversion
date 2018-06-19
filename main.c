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


  //DEBUG
  // FILE *fr = fopen("R.txt", "w");
  // FILE *fg = fopen("G.txt", "w");
  // FILE *fb = fopen("B.txt", "w");

  // for(i = 0; i < height; i++){
  //   for(j=0; j < width; j++){
  //     fprintf(fr, "%d ", r[i][j]);
  //   }
  //   fprintf(fr, "\n");
  // }
  // for(i = 0; i < height; i++){
  //   for(j=0; j < width; j++){
  //     fprintf(fb, "%d ", b[i][j]);
  //   }
  //   fprintf(fb, "\n");
  // }
  // for(i = 0; i < height; i++){
  //   for(j=0; j < width; j++){
  //     fprintf(fg, "%d ", g[i][j]);
  //   }
  //   fprintf(fg, "\n");
  // }

  // allocate memory to ycc arrays
  // TODO: REDUCE SIZE
  int **y = malloc(height * sizeof(uint8_t *));
  int **cb = malloc(height * sizeof(uint8_t *));
  int **cr = malloc(height * sizeof(uint8_t *));
  for (i=0; i < height; i++) {
    y[i] = malloc(width * sizeof(uint8_t));
    cb[i] = malloc(width * sizeof(uint8_t));
    cr[i] = malloc(width * sizeof(uint8_t));
  }

  // calculate and store ycc values
  // TODO: USE AVERAGES
  // REDUCE CACHE MISSES: https://m.eet.com/media/1157397/atc-152paper_shore_v4.pdf
  for(i = 0; i < height; i++){
    for(j=0; j < width; j++){
      y[i][j]  = ( 0.299 * r[i][j] + 0.587 * g[i][j] + 0.114 * b[i][j]);
      cb[i][j] = (-0.16874 * r[i][j] - 0.33126 * g[i][j] + 0.50000 * b[i][j]);
      cr[i][j] = ( 0.50000 * r[i][j] - 0.41869 * g[i][j] - 0.08131 * b[i][j]);
    }
  }

  //DEBUG
  // for(i = 0; i < height; i++){
  //   for(j=0; j < width; j++){
  //     printf("R[%d][%d]: %d\n", i, j, r[i][j]);
  //   }
  // }
  // for(i = 0; i < height; i++){
  //   for(j=0; j < width; j++){
  //     printf("G[%d][%d]: %d\n", i, j, g[i][j]);
  //   }
  // }
  // for(i = 0; i < height; i++){
  //   for(j=0; j < width; j++){
  //     printf("B[%d][%d]: %d\n", i, j, b[i][j]);
  //   }
  // }



  FILE *fdy = fopen("y.txt", "w");
  FILE *fdcb = fopen("cb.txt", "w");
  FILE *fdcr = fopen("cr.txt", "w");

  for(i = 0; i < height; i++){
    for(j=0; j < width; j++){
      fprintf(fdy, "%d ", y[i][j]);
    }
    fprintf(fdy, "\n");
  }
  for(i = 0; i < height; i++){
    for(j=0; j < width; j++){
      fprintf(fdcb, "%d ", cb[i][j]);
    }
    fprintf(fdcb, "\n");
  }
  for(i = 0; i < height; i++){
    for(j=0; j < width; j++){
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
    free(cb[i]);      
    free(cr[i]);      
  }
  free(r);
  free(g);
  free(b);
  
  free(y);
  free(cb);
  free(cr);
}