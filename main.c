#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include "arm_neon.h"

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
  int tempheight = height - (height & 1);
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
  for (i = 0, j = 0; i < tempheight; i++) {
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
  int tempwidth = ((width >> 2) << 2) - 2;
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

  tempwidth = ((width >> 3) << 3) - 6;
  int u, v;
  uint8x8_t rtmp, gtmp, btmp;
  int16x8_t ytmp, cbtmp, crtmp;
  uint8x8_t yresult, cbresult, crresult;
  uint8x8_t yrconst = vdup_n_u8 (66);
  uint8x8_t ygconst = vdup_n_u8 (129);
  uint8x8_t ybconst = vdup_n_u8 (25);
  uint8x8_t cbrconst = vdup_n_u8 (38);
  uint8x8_t cbgconst = vdup_n_u8 (75);
  uint8x8_t cbbconst = vdup_n_u8 (112);
  uint8x8_t crrconst = vdup_n_u8 (112);
  uint8x8_t crgconst = vdup_n_u8 (94);
  uint8x8_t crbconst = vdup_n_u8 (18);
  int8x8_t yoffset = vdup_n_s8 (16);
  int8x8_t cbcroffset = vdup_n_s8 (128);
  for (i = 0, u = 0; i < tempheight; i++) {
    // Even rows
    for(j = 0, v = 0; j < tempwidth; j += 8){
      // Load 8 pixels into vectors and convert to signed integers
      rtmp = vld1_u8((uint8_t *) &r[i][j]);
      gtmp = vld1_u8((uint8_t *) &g[i][j]);
      btmp = vld1_u8((uint8_t *) &b[i][j]);

      // y = 66 * r + 129 * g + 25 * b
      ytmp = vreinterpretq_s16_u16(vmull_u8(rtmp, yrconst));
      ytmp = vaddq_s16(vreinterpretq_s16_u16(vmull_u8(gtmp, ygconst)), ytmp);
      ytmp = vaddq_s16(vreinterpretq_s16_u16(vmull_u8(btmp, ybconst)), ytmp);

      // cb = -38 * r - 75 * g + 112 * b
      cbtmp = vreinterpretq_s16_u16(vmull_u8(btmp, cbbconst));
      cbtmp = vsubq_s16(cbtmp, vreinterpretq_s16_u16(vmull_u8(rtmp, cbrconst)));
      cbtmp = vsubq_s16(cbtmp, vreinterpretq_s16_u16(vmull_u8(gtmp, cbgconst)));

      // cr = 112 * r - 94 * g - 18 * b
      crtmp = vreinterpretq_s16_u16(vmull_u8(rtmp, crrconst));
      crtmp = vsubq_s16(crtmp, vreinterpretq_s16_u16(vmull_u8(gtmp, crgconst)));
      crtmp = vsubq_s16(crtmp, vreinterpretq_s16_u16(vmull_u8(btmp, crbconst)));

      // y = (y >> 8) + 16; cb = (cb >> 8) + 128; cr = (cr >> 8) + 128;
      yresult = vreinterpret_u8_s8(vadd_s8(vshrn_n_s16(ytmp, 8), yoffset));
      cbresult = vreinterpret_u8_s8(vadd_s8(vshrn_n_s16(cbtmp, 8), cbcroffset));
      crresult = vreinterpret_u8_s8(vadd_s8(vshrn_n_s16(crtmp, 8), cbcroffset));

      // Save vector to memory
      vst1_u8 ((uint8_t *) &y[i][j], yresult);
      cb[u][v] = vget_lane_u8(cbresult, 0);
      cr[u][v++] = vget_lane_u8(crresult, 0);
      cb[u][v] = vget_lane_u8(cbresult, 2);
      cr[u][v++] = vget_lane_u8(crresult, 2);
      cb[u][v] = vget_lane_u8(cbresult, 4);
      cr[u][v++] = vget_lane_u8(crresult, 4);
      cb[u][v] = vget_lane_u8(cbresult, 6);
      cr[u][v++] = vget_lane_u8(crresult, 6);
    }
    for (; j < width; j++) {
      y[i][j]  = ((66 * r[i][j] + 129 * g[i][j] + 25 * b[i][j]) >> 8) + 16;
      if ((j & 1) == 0) {
        cb[u][v] = ((-38 * r[i][j] - 75 * g[i][j] + 112 * b[i][j]) >> 8) + 128; 
        cr[u][v] = ((112 * r[i][j] - 94 * g[i][j] - 18 * b[i][j]) >> 8) + 128;
      }
    }
    i++; u++;
    
    // Odd rows
    for(j = 0; j < tempwidth; j++){
      // Load 8 pixels into vectors and convert to signed integers
      rtmp = vld1_u8((uint8_t *) &r[i][j]);
      gtmp = vld1_u8((uint8_t *) &g[i][j]);
      btmp = vld1_u8((uint8_t *) &b[i][j]);

      // y = 66 * r + 129 * g + 25 * b
      ytmp = vreinterpretq_s16_u16(vmull_u8(rtmp, yrconst));
      ytmp = vaddq_s16(vreinterpretq_s16_u16(vmull_u8(gtmp, ygconst)), ytmp);
      ytmp = vaddq_s16(vreinterpretq_s16_u16(vmull_u8(btmp, ybconst)), ytmp);

      // y = (y >> 8) + 16
      yresult = vreinterpret_u8_s8(vadd_s8(vshrn_n_s16(ytmp, 8), yoffset));

      // Save vector to memory
      vst1_u8 ((uint8_t *) &y[i][j], yresult);
    }
    for (; j < width; j++) {
      y[i][j]  = ((66 * r[i][j] + 129 * g[i][j] + 25 * b[i][j]) >> 8) + 16;
    }
  }

  if (height & 1) {
    for(j = 0; j < tempwidth; j++){
       // Load 8 pixels into vectors and convert to signed integers
      rtmp = vld1_u8((uint8_t *) &r[i][j]);
      gtmp = vld1_u8((uint8_t *) &g[i][j]);
      btmp = vld1_u8((uint8_t *) &b[i][j]);

      // y = 66 * r + 129 * g + 25 * b
      ytmp = vreinterpretq_s16_u16(vmull_u8(rtmp, yrconst));
      ytmp = vaddq_s16(vreinterpretq_s16_u16(vmull_u8(gtmp, ygconst)), ytmp);
      ytmp = vaddq_s16(vreinterpretq_s16_u16(vmull_u8(btmp, ybconst)), ytmp);

      // y = (y >> 8) + 16
      yresult = vreinterpret_u8_s8(vadd_s8(vshrn_n_s16(ytmp, 8), yoffset));

      // Save vector to memory
      vst1_u8 ((uint8_t *) &y[i][j], yresult);
    }
    for (; j < width; j++) {
      y[i][j]  = ((66 * r[i][j] + 129 * g[i][j] + 25 * b[i][j]) >> 8) + 16;
    }
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