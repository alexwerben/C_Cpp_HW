#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "interface.h"

// Check number of series of different lengths
int check_occurancies(int arr[], int const n) {
  for (int i = 0; i < MAX_LENGTH; ++i) {
    if (i != n - 1) {
      if (arr[n - 1] - arr[i] <= 2) { // Если кол-во нужных серий недостаточно
        return 0;
      }
    }
  }
  return 1; // Кол-во нужных серий достаточно
}

// Generate series of specific length
void gen_series(sequence* seq, int const length) {
  char sym = get_random_char();
  if (seq->size > 0) {
    while ((sym = get_random_char()) == seq->arr[seq->size - 1]) {}
  }
  // char sym = get_random_char();
  for (int i = 0; i < length; ++i) {
    seq->arr[seq->size++] = sym;
  }
  return;
}

// Save sequence into file
int write_data_to_file(char* sequence) {
  FILE* fp = NULL;
  fp = fopen("sequence.txt", "w");
  if (!fp) {
    return 1;
  }
  
  if (fprintf(fp, "%s", sequence) < 0) {
    fclose(fp);
    return 1;
  }

  if (fclose(fp) == 0) {
    return 0;
  } else {
    return 1;
  }
}

// Sequence initialization
sequence* init_sequence() {
  sequence* obj = malloc(sizeof(sequence));
  if (!obj) {
    return NULL;
  }

  obj->arr = malloc(ARRAY_SIZE * sizeof(char));
  if (!obj->arr) {
    delete_sequence(obj);
    return NULL;
  }
  obj->occurancies = malloc(MAX_LENGTH * sizeof(int));
  if (!obj->occurancies) {
    delete_sequence(obj);
    return NULL;
  }
  for (int i = 0; i < MAX_LENGTH; ++i) {
    obj->occurancies[i] = 0;
  }
  obj->size = 0;
  
  return obj;
}

// Handle sequence generation
int sequence_generator(int const n) {
  srand(time (NULL));

  sequence* seq = init_sequence();
  if (!seq) {
    return 1;
  }

  while (seq->size < ARRAY_SIZE) {
    if (check_occurancies(seq->occurancies, n)) {

      int random_length = rand() % MAX_LENGTH + 1;
      ++seq->occurancies[random_length - 1];

      gen_series(seq, random_length);
    } else {
      ++seq->occurancies[n - 1];
      
      gen_series(seq, n);
    } 

    if (ARRAY_SIZE - seq->size <= MAX_LENGTH) {
      ++seq->occurancies[ARRAY_SIZE - seq->size - 2];

      gen_series(seq, ARRAY_SIZE - seq->size - 1);

      seq->arr[seq->size++] = '\0';
    }
  }

  write_data_to_file(seq->arr);

  delete_sequence(seq);

  return 0;
}

// Generate random char
char get_random_char() {
  return 'A' + rand() % 5;
}

// Free allocated memory
int delete_sequence(sequence* obj) {
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

  free(obj);
  obj = NULL;

  return 0;
}