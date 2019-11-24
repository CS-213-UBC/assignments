#ifndef __list_h__
#define __list_h__

struct list;
typedef void* element_t;


struct list* list_create       ();
void         list_destroy      (struct list* list);
void         list_append       (struct list* list, element_t element);
void         list_append_array (struct list* list, element_t* elements, int n);
void         list_insert       (struct list* list, int pos, element_t element);
void         list_remove       (struct list* list, int pos);
element_t    list_get          (struct list* list, int pos);
int          list_index        (struct list* list, element_t element, int (*equals) (element_t, element_t));
int          list_len          (struct list* list);

void         list_map1         (void (*f) (element_t*, element_t),            struct list* out_list,  struct list* in_list);
void         list_map2         (void (*f) (element_t*, element_t, element_t), struct list* out_list,  struct list* in_list0, struct list* in_list1);
void         list_foldl        (void (*f) (element_t*, element_t, element_t), element_t* out_value_p, struct list* in_list);
void         list_filter       (int  (*f) (element_t),                        struct list* out_list,  struct list* in_list);
void         list_foreach      (void (*f) (element_t),                        struct list* list);
// void		add (element_t* rv, element_t av, element_t bv);
void list_scan ( void (*f) (element_t*, element_t, element_t), element_t base, struct list* out, struct list* in);
void max (element_t* rv, element_t av, element_t bv);
void even (element_t* rv, element_t av);

int add (int a, int b);
int sub (int a ,int b);
int diva (int a, int b);
int mul (int a, int b);

#endif /* __list_h__ */
