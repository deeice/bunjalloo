#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void print_usage(void) {
  printf("Pad a file to an even number of bytes.\n");
  printf("Output by default written to stdout.\n");
  printf("pad [options] file\n");
  printf("-f             Force output (if no padding, normally writes nothing)\n");
  printf("-n             Do not output anything, print what would happen\n");
  printf("-o FILE        output to FILE\n");
  printf("-p NUM         pad to 2 4 8 16 bytes. Default 4 bytes.\n");
  printf("-u [BYTE]      pad using BYTE. If BYTE 0 or not given,\n");
  printf("               then will repeat the last bytes of the  \n");
  printf("               in file. Pads with zeros by default.  \n");
  printf("-v             Print some debugging info (verbose)\n");
}


#define PADS 4
const int allowed_paddings[] = { 2, 4, 8, 16};
int check_pad(int pad_to) {
  int is_ok,i;
  is_ok = 0;
  for (i = 0; i < PADS; i++) {
    if (pad_to == allowed_paddings[i])
      is_ok = 1;
  }
  return is_ok;
}


int main(int argc, char * argv[]) {
  int pad_to, q, len, padding, read, op, w, u, f;
  FILE * in;
  FILE * out;
  char * data;
  char* o;
  if (argc < 2) {
    print_usage();
    return 0;
  }
  /* set the default options: */
  pad_to = 4;
  q = 1;
  w = 1;
  f = 0;
  out = NULL;
  o = 0;
  u = -1;
  /* get the user options: */
  while((op = getopt(argc,
                     argv,
                     "p:vno:u::f"
                     )) != -1)
  {
    switch (op) {
      case 'f': f = 1;
		break;
      case 'n': w = 0; q = 0;
		break;
      case 'o': o = optarg; 
		break;
      case 'p': pad_to = atoi(optarg);
		if (check_pad(pad_to) == 0) {
		  printf("Not a valid padding value\n");
		  print_usage();
		  return 0;
		}
		break;
      case 'u': if (optarg)
		 u = atoi(optarg);
		else
		 u = 0;
		break; 
      case 'v': q = 0;
		break;
      case '?': print_usage();
		return 0;
      default: break;
    }
  }


  /* open the file */
  in = fopen(argv[optind], "rb");
  if (in == NULL) {
    printf("Could not open file %s\n", argv[optind]);
    return 1;
  }
  
  /* find the length */
  fseek(in , 0,SEEK_END);
  len = ftell(in);
  
  if (len == 0) {
    printf("Empty file!\n");
    return 2;
  }
  /* start of file again.. */
  fseek(in , 0,SEEK_SET);
  
  /* see if we need to pad */
  padding = ( (len & (pad_to-1)) );
  if (padding) {
    padding = pad_to - padding;
  }
  if (!q) {
    printf("Padding %s to %d byte boundary\n",
	argv[optind], pad_to);
  }
  if (padding || f) {
    /* yes we do */
    data = (char*) malloc(len);
    fread(data, 1, len , in); 
    fclose(in);
    if (o != NULL) {
      out = fopen(o, "wb");
      if (!out) {
	printf("Could not open %s \n", o);
	return 3;
      }
    }
    if (!q) {
      printf("Padding - in size %d out size %d (%d byte%sadded)\n",
	  len, len + padding, padding, (padding<2)?" ":"s "); 
    }
    if (w) {
      if (out == NULL) {
	read = 0;
	while (read < len)
	  putchar(data[read++]);
	if (u != 0) {
	  while (padding--)
	    putchar((u==-1)?0:(u&0xff));
	} else {
	  /* write last bytes of file again */
	  while (padding--)
	    putchar(data[len-padding-1]);
	}
	
      } else {
	/* write to file */
	fwrite(data, 1, len, out);
	if (ferror(out)) {
	  printf( "Error writing to %s \n", o);
	  return 4;
	}
	if (u != 0) {
	  while (padding--)
	    fputc((u==-1)?0:(u&0xff), out);
	} else {
	  /* write last bytes of file again */
	  while (padding--)
	    fputc(data[len-padding-1], out);
	}
	fflush(out);
        fclose(out);
      }
    }
  } else {
    if (!q) {
      printf("No padding - "\
	  "in size %d out size %d\n", len, len + padding); 
    }
  }
  return 0; 
}
