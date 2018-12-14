#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int32_t key[4] = {0x7bea0297, 0xef4d3625, 0xaed5a0ea, 0x78e800d5};

void encode(int32_t* v, int32_t* k)
{
    uint32_t y=v[0],z=v[1], sum=0,   /* set up */
             delta=0x9e3779b9, n=32 ;     /* a key schedule constant */
    while ((n--)>0) {                     /* basic cycle start */
      sum += delta ;
      y += ((z<<4)+k[0]) ^ (z+sum) ^ ((z>>5)+k[1]) ;
      z += ((y<<4)+k[2]) ^ (y+sum) ^ ((y>>5)+k[3]) ;   /* end cycle */
    }
    v[0]=y ; v[1]=z ;
}

int main(int argc, char **argv)
{
    FILE *inf, *outf;
    int n;
    union {
      char c[8];
      int32_t v[2];
    } data;

    if (argc!=3) {
      fprintf(stderr, "usage: encode infile outfile\n");
      exit(1);
    }
    inf = fopen(argv[1], "r");
    outf = fopen(argv[2], "wb");
    putc('\0', outf);
    while (!feof(inf)) {
      data.v[0] = data.v[1] = 0;
      n = fread(data.c, 1, 8, inf);
      encode(data.v, key);
      fwrite(data.v, 2, sizeof(int32_t), outf);
    }
    if (n==8) {
      data.v[0] = data.v[1] = 0;
      encode(data.v, key);
      fwrite(data.v, 2, sizeof(int32_t), outf);
    }
    fclose(outf); fclose(inf);
	return 0;
}
