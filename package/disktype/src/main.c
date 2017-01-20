/*
 * main.c
 * Main entry point and global utility functions.
 *
 * Copyright (c) 2003 Christoph Pfisterer
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. 
 */

#include "global.h"

/*
 * local functions
 */

static void analyze_file(const char *filename);

/*
 * entry point
 */

int main(int argc, char *argv[])
{
  int i;

  if (argc < 2) {
    fprintf(stderr, "Usage: %s <device/file>...\n",PROGNAME);
    return 1;
  }

    analyze_file(argv[1]);

  return 0;
}

/*
 * Analyze one file
 */

static void analyze_file(const char *filename)
{
  int fd, filekind;
  u8 filesize;
  struct stat sb;
  char *reason;
  SOURCE *s;

  /* stat check */
  if (stat(filename, &sb) < 0) {
    errore("Can't stat %.300s", filename);
    return;
  }

  filekind = 0;
  filesize = 0;
  reason = NULL;
  if (S_ISREG(sb.st_mode)) {
    filesize = sb.st_size;
  } else if (S_ISBLK(sb.st_mode))
    filekind = 1;

  /* empty regular files need no further analysis */
  if (filekind !=1)
    return;

  /* open for reading */
  fd = open(filename, O_RDONLY);
  if (fd < 0) {
    errore("Can't open %.300s", filename);
    return;
  }

  /* create a source */
  s = init_file_source(fd, filekind);


  /* now analyze it */
  analyze_source(s);

  /* finish it up */
  close_source(s);
}

/* EOF */
