#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
/* int32_t key[4] = {0x7bea0297, 0xef4d3625, 0xaed5a0ea, 0x78e800d5}; */
#define k0 (0x7bea0297)
#define k1 (0xef4d3625)
#define k2 (0xaed5a0ea)
#define k3 (0x78e800d5)

void decode(int32_t* v)
{
    uint32_t n=32, sum, y=v[0], z=v[1],
             delta=0x9e3779b9 ;
    sum=delta<<5 ;
    /* start cycle */
    while (n-->0) {
      z-= ((y<<4)+k2) ^ (y+sum) ^ ((y>>5)+k3) ;
      y-= ((z<<4)+k0) ^ (z+sum) ^ ((z>>5)+k1) ;
      sum-=delta ;
    }
    /* end cycle */
    v[0]=y ; v[1]=z ;
}

int main(int argc, char **argv)
{
    FILE *inf, *outf;
    int n;
    union {
      char    c[8];
      int32_t v[2];
    } data;
    int finished = 0;

    if (argc!=3) {
      fprintf(stderr, "usage: decode infile outfile\n");
      exit(1);
    }
    inf = fopen(argv[1], "rb");
    outf = fopen(argv[2], "w");
    if ((n=getc(inf))!='\0') ungetc(n, inf);
    while (!feof(inf)) {
      fread(data.c, 1, 8, inf);
      decode(data.v);
      for (n=0; n<8; n++) if (data.c[n]=='\0') {
        fwrite(data.c, 1, n, outf);
        fclose(inf); fclose(outf);
        exit(0);
      }
      fwrite(data.c, 1, 8, outf);
    }
    fclose(inf); fclose(outf);
	return 0;
}
