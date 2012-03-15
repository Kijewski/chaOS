#ifndef PAGING__
#define PAGING__

#include <stdbool.h>
#include <attributes.h>

bool paging_init (void);
void paging_enable (void) NO_RETURN;

void *paging_get_page (void);
bool paging_return_page (void *);

#endif
