#ifndef LIST_H__
#define LIST_H__

#include <stddef.h>
#include <stdbool.h>

#define list_entry(ELEM, TYPE, MEMBER)                                        \
    ({                                                                        \
      struct list_elem *_e = (ELEM);                                          \
      container_of (_e, TYPE, MEMBER);                                        \
    })

#define LIST_INITIALIZER(L) \
    ((struct list) {{ &(L)->sentinel, &(L)->sentinel }})

struct list_elem
{
  struct list_elem *next, *prev;
};

struct list
{
  struct list_elem sentinel;
};

static inline void
list_init (struct list *l)
{
  *l = LIST_INITIALIZER (l);
}

static inline void
list_remove (struct list_elem *e)
{
  e->next->prev = e->prev;
  e->prev->next = e->next;
}

static inline void
list_insert_after (struct list_elem *cur, struct list_elem *e)
{
  e->next = cur->next;
  cur->next->prev = e;
  e->prev = cur;
  cur->next = e;
}

static inline void
list_push_back (struct list *l, struct list_elem *e)
{
  list_insert_after (l->sentinel.prev, e);
}

static inline void
list_push_front (struct list *l, struct list_elem *e)
{
  list_insert_after (&l->sentinel, e);
}

static inline struct list_elem *
list_next (struct list_elem *e)
{
  return e->next;
}

static inline struct list_elem *
list_prev (struct list_elem *e)
{
  return e->prev;
}

static inline struct list_elem *
list_end (struct list *l)
{
  return &l->sentinel;
}

static inline bool
list_is_empty (struct list *l)
{
  return l->sentinel.next == &l->sentinel;
}

static inline struct list_elem *
list_pop_front (struct list *l)
{
  struct list_elem *e = l->sentinel.next;
  list_remove (e);
  return e;
}

static inline struct list_elem *
list_pop_back (struct list *l)
{
  struct list_elem *e = l->sentinel.prev;
  list_remove (e);
  return e;
}

#endif
