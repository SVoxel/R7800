#include "global.h"

/*
 * external detection functions
 */
void detect_dos_partmap(SECTION *section);
void detect_gpt_partmap(SECTION *section);


/*
 * internal stuff
 */

/*
 * analyze a given source
 */

void analyze_source(SOURCE *s)
{
  SECTION section;

  section.source = s;
  section.pos = 0;
  section.size = s->size_known ? s->size : 0;
  section.flags = 0;
  detect_dos_partmap(&section);
  detect_gpt_partmap(&section);
}

/* EOF */
