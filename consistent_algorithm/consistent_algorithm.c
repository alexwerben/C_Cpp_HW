#include <stdio.h>
#include <stdlib.h>
#include "consistent_algorithm.h"

// Find most frequently appearing length
int consistent_algorithm(char* seq) {
  RLE* obj = NULL;
  obj = init_RLE(seq);
  if (!obj) {
    return 1;
  }

  if (fill_RLE(obj)) {
    return 1;
  }

  int max = 0;
  max = get_max_occurance_length(obj);

  // print_info(obj);
  printf("max - %d\n", max);

  delete_RLE(obj);

  return 0;
}

void print_info(RLE* obj) {
  printf("___________\n");
  for (int i = 0; i < obj->arr_size; ++i) {
    printf("[%d]: length - %d, occurs - %d times\n", i, obj->length[i], obj->occurancies[i]);
  }
  printf("current length - %d\n", obj->current_length);
  printf("array size - %d\n", obj->arr_size);
  printf("sequence - %s\n", obj->arr);

  return;
}

// Get length which occured most oftem
int get_max_occurance_length(RLE* obj) {
  int max = obj->occurancies[0];
  int index = 0;

  for (int i = 0; i < obj->arr_size; ++i) {
    if (max < obj->occurancies[i]) {
      max = obj->occurancies[i];
      index = i;
    }
  }

  return obj->length[index];
}

// Initialize RLE structure
RLE* init_RLE(char* seq) {
  RLE* obj = NULL;
  FILE* fp = NULL;

  obj = malloc(sizeof(RLE));
  if (!obj) {
    delete_RLE(obj);
    return NULL;
  }
  obj->arr = NULL;
  obj->length = NULL;
  obj->occurancies = NULL;
  obj->arr_size = 0;
  obj->current_length = 0;
  obj->max_size = BLOCK_SIZE;

  obj->arr = seq;

  obj->occurancies = malloc(obj->max_size * sizeof(int));
  if (!obj->occurancies) {
    delete_RLE(obj);
    return NULL;
  }

  obj->length = malloc(obj->max_size * sizeof(int));
  if (!obj->length) {
    delete_RLE(obj);
    return NULL;
  }

  for (int i = 0; i < obj->max_size; ++i) {
    obj->occurancies[i] = 0;
    obj->length[i] = 0;
  }

  return obj;
}

// Calculate length occurancies and how often they appear
int fill_RLE(RLE* obj) {
  char c = '\0';

  for (int i = 0; obj->arr[i] != '\0';) {
    c = obj->arr[i];
    while (c == obj->arr[i]) {
      ++obj->current_length;
      ++i;
    }

    int length_index = 0;
    if ((length_index = check_length_existance(obj->length, obj->current_length, obj->arr_size)) >= 0) {
      // printf("if cond %d\n", obj->occurancies[0]);
      obj->occurancies[length_index] += 1;
    } else {
      if (obj->arr_size == obj->max_size) {
        if (increase_arr_size(obj)) {
          return 1;
        }
      }
      obj->occurancies[obj->arr_size] += 1;
      obj->length[obj->arr_size] = obj->current_length;
      ++obj->arr_size;
    }

    print_info(obj);
    printf("symbol - %c\n", c);
    obj->current_length = 0;
  }

  return 0;
}

// Increase size of obj->length and obj->occurancies if it is full by BLOCK_SIZE
int increase_arr_size(RLE* obj) {
  int* tmp = NULL;

  obj->max_size = obj->max_size + BLOCK_SIZE;

  tmp = obj->length;
  obj->length = NULL;
  obj->length = malloc(obj->max_size * sizeof(int));
  if (!obj->length) {
    return 1;
  }
  for (int i = 0; i < obj->arr_size; ++i) {
    obj->length[i] = tmp[i];
  }
  free(tmp);

  tmp = obj->occurancies;
  obj->occurancies = NULL;
  obj->occurancies = malloc(obj->max_size * sizeof(int));
  if (!obj->occurancies) {
    return 1;
  }
  for (int i = 0; i < obj->arr_size; ++i) {
    obj->occurancies[i] = tmp[i];
  }

  for (int i = obj->arr_size; i < obj->max_size; ++i) {
    obj->length[i] = 0;
    obj->occurancies[i] = 0;
  }
  
  free(tmp);

  return 0;
}

// Check if specific length has already appeared, if YES return index in length[], else -1
int check_length_existance(int length[], int current_length, int arr_size) {
  for (int i = 0; i < arr_size; ++i) {
    if (length[i] == current_length) {
      printf("index %d\n", i);
      return i;
    }
  }
  return -1;
}

// Delete allocated memory for RLE structure
int delete_RLE(RLE* obj) {
  if (!obj) {
    return 1;
  }

  if (obj->arr) {
    free(obj->arr);
    obj->arr = NULL;
  }
  if (obj->occurancies) {
    free(obj->occurancies);
    obj->occurancies = NULL;
  }
  if (obj->length) {
    free(obj->length);
    obj->length = NULL;
  }
  free(obj);
  obj = NULL;

  return 0;
}