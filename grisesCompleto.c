#include <stdio.h>
#include <stdlib.h>
#include<omp.h>
//#define NUM_THREADS 500

int main(int argc, char *argv[])
{
  int NUM_THREADS;
  NUM_THREADS=atoi(argv[1]);
  FILE *image, *outputImage, *lecturas;
  image = fopen(argv[2],"rb");          //Imagen original a transformar
  outputImage = fopen(argv[3],"wb");    //Imagen transformada
  //image = fopen("luffy.bmp","rb");          //Imagen original a transformar
  //outputImage = fopen("prueba.bmp","wb");    //Imagen transformada
  long ancho;
  long alto;
  unsigned char r, g, b;               //Pixel
  unsigned char* ptr;

  unsigned char xx[54];
  int cuenta = 0;
  for(int i=0; i<54; i++) {
    xx[i] = fgetc(image);
    fputc(xx[i], outputImage);   //Copia cabecera a nueva imagen
  }
  ancho = (long)xx[20]*65536+(long)xx[19]*256+(long)xx[18];
  alto = (long)xx[24]*65536+(long)xx[23]*256+(long)xx[22];
  printf("largo img %li\n",alto);
  printf("ancho img %li\n",ancho);

  ptr = (unsigned char*)malloc(alto*ancho*3* sizeof(unsigned char));
  omp_set_num_threads(NUM_THREADS);
  const double startTime = omp_get_wtime();
  
  #pragma omp parallel
  {
    while(!feof(image)){
      b = fgetc(image);
      g = fgetc(image);
      r = fgetc(image);

      unsigned char pixel = 0.21*r+0.72*g+0.07*b;

      ptr[cuenta] = pixel; //b
      ptr[cuenta+1] = pixel; //g
      ptr[cuenta+2] = pixel; //r

      cuenta++;

      }
    }                      
    
  //Grises

  #pragma omp parallel
  {
    #pragma omp for schedule(dynamic)
    for (int i = 0; i < alto*ancho*3; ++i) {
      fputc(ptr[i], outputImage);
      fputc(ptr[i+1], outputImage);
      fputc(ptr[i+2], outputImage);
    }
  }

  const double endTime = omp_get_wtime();
  free(ptr);
  fclose(image);
  fclose(outputImage);
  printf("Tiempo=%f", endTime-startTime);
  return 0;
}
