#include <stdlib.h>
#include <stdio.h>
#include "list.h"

#define INITIAL_SIZE  10
#define GROWTH_FACTOR 5

/**
 * Structure describing a list.
 */
struct list {
  element_t* data;
  int        len;
  int        size;
};

/**
 * Create a new empty list.
 */
struct list* list_create () {
  struct list* l = malloc (sizeof (struct list));
  l->data  = malloc (sizeof (element_t) * INITIAL_SIZE);
  l->len   = 0;
  l->size  = INITIAL_SIZE;
  return l;
}

/**
 * Destroy list.
 */
void list_destroy (struct list* l) {
  free (l->data);
  free (l);
}

/**
 * Expand the capacity of the list.  
 * New size is old size times GROWTH_FACTOR.
 * Internal helper function.
 */
static void expand_list (struct list* l) {
  l->size *= GROWTH_FACTOR;
  l->data  = realloc (l->data, l->size * sizeof (element_t));
}

/**
 * Append e to end of list.
 */
void list_append (struct list* list, element_t element) {
  if (list->len == list->size)
    expand_list (list);
  list->data [list->len] =  element;
  list->len             += 1;
}

/**
 * Append a[0] .. a[n-1] to end of list.
 */
void list_append_array (struct list* list, element_t* elements, int n) {
  for (int i = 0; i < n; i++)
    list_append (list, elements [i]);
}

/**
 * Insert e at position pos (0..len-1).
 * Moves elements down to make room for the new element.
 */
void list_insert (struct list* list, int pos, element_t element) {
  if (list->len == list->size)
    expand_list (list);
  for (int i = list->len; i > pos; i--)
    list->data [i] = list->data [i-1];
  list->data [pos] = element;
  list->len += 1;
}

/**
 * Remove element at position pos (0..len-1).
 * Move elements up to occupy position of removed element.
 */
void list_remove (struct list* list, int pos) {
  for (int i = pos; i < list->len - 1; i++)
    list->data [i] = list->data [i+1];
  list->len -= 1;
}

/**
 * Return element at position pos (0..len-1).
 */
element_t list_get (struct list* list, int pos) {
  return list->data [pos];
}

/**
 * Return the position (0..len-1) of e where equality
 * is established by equality function, which returns 1 
 * iff two objects are equal.
 */
int list_index (struct list* list, element_t element, int (*equal) (element_t, element_t)) {
  for (int i = 0; i < list->len; i++)
    if (equal (list->data [i], element))
      return i;
  return -1;
}

/**
 * Return the length of the list.
 */
int list_len (struct list* list) {
  return list->len;
}

/**
 * Map function f(out,in) onto in_list placing results in out_list (by appending).
 * The lists in_list and out_list must not be the same list.
 * For f(out,in): 
 *    out is a pointer to an element that will be placed in out_list
 *        if *out == NULL, f must assign it a value (e.g., by calling malloc)
 *        FREE IS NOT CALLED ON ELEMENTS ALLOCATED BY F WHEN THE OUT_LIST IS DESTROYED
 *    in  is an element from in_list
 */
void list_map1 (void (*f) (element_t*, element_t), struct list* out_list, struct list* in_list) {
    
    for (int i = 0; i < in_list->len; i++) {
      element_t out = NULL;
      
      f (&out, in_list->data[i]);
      list_append (out_list, out);
      
    }
    
}

/**
 * Map function f onto list0 and in_list1 placing results in out_list (by appending).
 * The list out_list must not be one of in_list0 or in_list1.
 * Lists in_list1 and in_list2 can be of different length
 * The length of out_list is equal to the minimum of the lengths of in_list0 and in_list1.
 * For f(out,in0,in1):
 *    out is pointer to an element that will be placed in out_list
 *        if *out == NULL, f must assign it a value (e.g., by calling malloc)
 *        FREE IS NOT CALLED ON ELEMENTS ALLOCATED BY F WHEN THE OUT_LIST IS DESTROYED
 *    in0  is an element from in_list0
 *    in1  is an element from in_list1
 */
void list_map2 (void (*f) (element_t*, element_t, element_t), struct list* out_list, struct list* in_list0, struct list* in_list1) {
  int size;
  if (list_len(in_list0) > list_len(in_list1)){
    size = list_len(in_list1);
  } else {
    size = list_len(in_list0);
  }

  //element_t *out = malloc(sizeof(element_t));

  for (int i = 0; i < size; i++){
    element_t out = NULL;
    
    f (&out, in_list0->data[i],in_list1->data[i]);
    list_append(out_list, out);
    
  }
  //free(out);
}

/**
 * Fold in_list using function f placing result in *out_element_p.
 * For f(out,in0,in1):
 *    out is pointer to the accumulator element (i.e., out_element_p)
 *    in0 is input value of the accumulator element (i.e., *out_element_p)
 *        this parameter is included only so that the same f can be used with foldl and map2
 *        it isn't otherwise needed since *out == in0
 *    in1 is an element from in_list
 */
void list_foldl (void (*f) (element_t*, element_t, element_t), element_t* out_element_p,  struct list* in_list) {
  for (int i = 0; i < in_list->len; i++){
    f(out_element_p, *out_element_p, in_list->data[i]);
  }
}

/**
 * Filter in_list using function f placing result in out_list.
 * List out_list contains elements of in_list for which f returns true (i.e., non-zero).
 *     NOTE THAT THE OUT_LIST DATA ARRAY ALIASES (I.,E. POINTS TO ELEMENTS OF) THE IN_LIST DATA ARRAY
 * List out_list and in_list must not be the same list.
 * For f(in):
 *    in      is an element from in_list
 *    returns true (1) iff in should be included in out_list and 0 otherwise
 */
void list_filter (int (*f) (element_t), struct list* out_list, struct list* in_list) {
  for (int i = 0; i < in_list->len; i++){
    int include = f(in_list->data[i]);
    if (include){
      list_append(out_list, in_list->data[i]);
    }
  }
}
void list_scan (
    void (*f) (element_t*, element_t, element_t), 
    element_t base, struct list* out, struct list* in
) {
  element_t rv = 0;
  f (&rv, base, in->data[0]);
  list_append (out, rv);
  for (int i = 1; i < in->len; i++) {
     rv = 0;
     f (&rv, out->data[i-1], in->data[i]);
     list_append (out, rv);
  }
}

// void add (element_t* rv, element_t av, element_t bv) {
//   int **r = (int**) rv, *a = av, *b = bv;
//   if (*r == NULL)
//     *r = malloc (sizeof (int*));
//   **r = *a + *b;
// }
/**
 * Execute function f for each element of list list.
 */
void list_foreach (void (*f) (element_t), struct list* list) {
  for (int i = 0; i < list->len; i++)
    f (list->data [i]);
}
void max (element_t* rv, element_t av, element_t bv) {
  int **r = (int**) rv, *a = av, *b = bv;
  if (*r == NULL)
    *r = malloc(sizeof(int*));
  **r = *a > *b? *a: *b;
}
void even (element_t* rv, element_t av) {
  int **r = (int**)rv, *a = av;
  if (*r == NULL)
    *r = malloc(sizeof(int*));
  **r = (*a & 1)? 0: 1;
}


int add (int a, int b) {return a+b;}
int sub (int a, int b) {return a-b;}
int diva (int a, int b) {return a/b;}
int mul (int a, int b) {return a*b;}

int (*func[4])(int,int) = {add,sub,diva,mul};

  int main (int argc, char** argv) {
  int a = 1;
  int b = 4;
  int c = 7;
  int d = func[func[1](b,a)](func[2](c,b),func[0](a,b));
  printf("%d\n",d);



	
	return 0;
}