#ifndef LIST_H__
#define LIST_H__

#include <stddef.h>
#include <stdbool.h>
#include <assert.h>

#define list_entry(ELEM, TYPE, MEMBER)                                        \
    ({                                                                        \
      struct list_elem *_e = (ELEM);                                          \
      ASSERT (_e != NULL);                                                    \
      container_of (_e, TYPE, MEMBER);                                        \
    })

#ifndef NDEBUG
# define LIST_INITIALIZER(L) \
    ((struct list) {{ &(L)->sentinel, &(L)->sentinel, 0x42 }})
#else
# define LIST_INITIALIZER(L) \
    ((struct list) {{ &(L)->sentinel, &(L)->sentinel }})
#endif

struct list_elem
{
  struct list_elem *next, *prev;
#ifndef NDEBUG
  uint8_t magic;
#endif
};

struct list
{
  struct list_elem sentinel;
};

static inline void
list_init (struct list *l)
{
  ASSERT (l != NULL);
  *l = LIST_INITIALIZER (l);
}

static inline void
list_remove (struct list_elem *e)
{
  ASSERT (e != NULL);
  ASSERT (e->magic == 0x42);

  e->next->prev = e->prev;
  e->prev->next = e->next;
}

static inline void
list_insert_after (struct list_elem *cur, struct list_elem *e)
{
  ASSERT (cur != NULL);
  ASSERT (cur->magic == 0x42);
  ASSERT (e != NULL);

  e->next = cur->next;
  cur->next->prev = e;
  e->prev = cur;
  cur->next = e;
  e->magic = 0x42;
}

static inline void
list_push_back (struct list *l, struct list_elem *e)
{
  ASSERT (l != NULL);
  list_insert_after (l->sentinel.prev, e);
}

static inline void
list_push_front (struct list *l, struct list_elem *e)
{
  ASSERT (l != NULL);
  list_insert_after (&l->sentinel, e);
}

static inline struct list_elem *
list_next (struct list_elem *e)
{
  ASSERT (e != NULL);
  ASSERT (e->magic == 0x42);
  return e->next;
}

static inline struct list_elem *
list_prev (struct list_elem *e)
{
  ASSERT (e != NULL);
  ASSERT (e->magic == 0x42);
  return e->prev;
}

static inline struct list_elem *
list_end (struct list *l)
{
  ASSERT (l != NULL);
  return &l->sentinel;
}

static inline bool
list_is_empty (struct list *l)
{
  ASSERT (l != NULL);
  return l->sentinel.next == &l->sentinel;
}

static inline struct list_elem *
list_pop_front (struct list *l)
{
  ASSERT (!list_is_empty (l));

  struct list_elem *e = l->sentinel.next;
  list_remove (e);
  return e;
}

static inline struct list_elem *
list_pop_back (struct list *l)
{
  ASSERT (!list_is_empty (l));

  struct list_elem *e = l->sentinel.prev;
  list_remove (e);
  return e;
}

#endif
