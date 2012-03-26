#ifndef WORDSIZE_H__
#define WORDSIZE_H__

#ifndef __x86_64__
# error "This is an AMD64 kernel!"
#endif

#define __WORDSIZE 64
 
#endif
