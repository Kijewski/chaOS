#ifndef WORDSIZE_H__
#define WORDSIZE_H__

#if defined __x86_64__
# define __WORDSIZE	64
#elif defined __i386__
# define __WORDSIZE	32
#else
# error "Target architure is neither AMD64, nor IA32."
#endif
 
#endif
