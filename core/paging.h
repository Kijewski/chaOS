#ifndef PAGING__
#define PAGING__

#include <stdbool.h>
#include <attributes.h>

bool paging_init (void);
void paging_enable (void) NO_RETURN;

#endif
