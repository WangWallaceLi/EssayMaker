

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_KEY_WORD 100

int gWordCount;
char *gKeyWord[MAX_KEY_WORD];

#ifdef DEBUG
#define DEBUG_TRACE printf
#define TAG() {printf("\nTAG:%s:%d..", __FUNCTION__, __LINE__); fflush(stdout);}
//#define TAG()
#else
#define DEBUG_TRACE(...)
#define TAG()
#endif


int readKeyInput(FILE *f)
{
  char *str = NULL;
  int i;
  size_t size = 0;
  size_t ret = 0;
  char *buffer;

  // check input
  if (f == NULL)
  {
    return 1;
  }

  // read file
  fseek(f, 0, SEEK_END);
  size = ftell(f);
  rewind(f);
  buffer = (char *)malloc(sizeof(char) * size + 1);
  ret = fread(buffer, 1, size, f);

  if (ret != size)
  {
    return 1;
  }

  // scan once to know the number of words
  i = 0;
  str = strtok(buffer, " ");
  while (str != NULL)
  {
    gKeyWord[i] = (char *)malloc(sizeof(char) * strlen(str));
    strcpy(gKeyWord[i], str);
    i++;
    str = strtok (NULL, " ");
  }
  return 0;
}

#define MAX_LENGTH_OF_LINE 255
#define LENGTH_WORD 30
typedef enum
{
  FIRST,
  SECOND,
  THIRD,
  SECOND_THIRD,
  ALL,
  ANY,
}SearchType_t;

typedef struct
{
  char word1[LENGTH_WORD];
  char word2[LENGTH_WORD];
  char word3[LENGTH_WORD];
  int count;
}Node_t;

int findBestEntry(FILE *f, char *input1, char *input2, char *input3, SearchType_t tType, Node_t *nodefound)
{
  char str[MAX_LENGTH_OF_LINE];
  char *str1 = NULL;
  char *str2 = NULL;
  char *str3 = NULL;
  char *word1, *word2, *word3;
  char *tmp;
  int count;
  char *ret;
  int hit;
  switch (tType)
  {
    case FIRST:
      str1 = input1;
      break;
    case SECOND:
      str2 = input2;
      break;
    case THIRD:
      str3 = input3;
      break;
    case SECOND_THIRD:
      str2 = input2;
      str3 = input3;
      break;
    case ALL:
      str1 = input1;
      str2 = input2;
      str3 = input3;
      break;

    case ANY:
      str1 = input1;
      str2 = input1;
      str3 = input1;
      break;
    default:
      break;

  }
  ret = fgets(str, MAX_LENGTH_OF_LINE, f);
  while (ret != NULL)
  {
    word1 = strtok(str, " \t"); if (word1 == NULL) return 1;
    word2 = strtok(NULL, " \t"); if (word2 == NULL) return 1;
    word3 = strtok(NULL, " \t"); if (word3 == NULL) return 1;
    tmp = strtok(NULL, " \t"); if (tmp == NULL) return 1;

    count = strtol(tmp, NULL, 10);

    hit = 0;
    if (str1 == NULL || strcmp(str1,word1) == 0) hit++;
    if (str2 == NULL || strcmp(str2, word2) == 0) hit++;
    if (str3 == NULL || strcmp(str3, word3) == 0) hit++;
    if (hit!=3){TAG();ret = fgets(str, MAX_LENGTH_OF_LINE, f); continue;}
    if (nodefound->count < count)
    {

      strcpy(nodefound->word1, word1);
      strcpy(nodefound->word2, word2);
      strcpy(nodefound->word3, word3);
      nodefound->count = count;
    }
  }

  return 0;
}


int validateWord(char *str)
{
  int i;
  for (i = 0; i<strlen(str);i++)
  {
    if (str[i] < 'A' || str[i] > 'z' ||
        (str[i] > 'Z' && str[i] < 'a'))
    {
      return 1;
    }
  }
  return 0;
}

int cleanData(FILE *f)
{
  char str[MAX_LENGTH_OF_LINE];
  char *ret;
  char *word1, *word2, *word3;
  char *tmp;
  int count;
  int totalCount = 0;
  char word1Stored[LENGTH_WORD] = {0};
  char word2Stored[LENGTH_WORD] = {0};
  char word3Stored[LENGTH_WORD] = {0};

  ret = fgets(str, MAX_LENGTH_OF_LINE, f);

  while (ret != NULL)
  {
    word1 = strtok(str, " \t"); if (validateWord(word1) == 1) {ret = fgets(str, MAX_LENGTH_OF_LINE, f); continue;}
    word2 = strtok(NULL, " \t"); if (validateWord(word2) == 1) {ret = fgets(str, MAX_LENGTH_OF_LINE, f); continue;}
    word3 = strtok(NULL, " \t"); if (validateWord(word3) == 1) {ret = fgets(str, MAX_LENGTH_OF_LINE, f); continue;}

    tmp = strtok(NULL, " \t");

    tmp = strtok(NULL, " \t");
    count = strtol(tmp, &tmp, 10);
    if (strcmp(word1, word1Stored) == 0 && strcmp(word2, word2Stored) == 0 && strcmp(word2, word2Stored) == 0)
    {
      totalCount += count;
    }
    else
    {
      printf("%s %s %s\t%d\n", word1Stored, word2Stored, word3Stored, totalCount);
      totalCount = 0;
      strcpy(word1Stored, word1);
      strcpy(word2Stored, word2);
      strcpy(word3Stored, word3);
    }
    ret = fgets(str, MAX_LENGTH_OF_LINE, f);
  }

  printf("%s %s %s\t%d\n", word1Stored, word2Stored, word3Stored, totalCount);
}


int main(int argc, char **argv )
{
  FILE *f;
  Node_t node;

  f = fopen(argv[2], "r");

  if (strcmp(argv[1], "clean") == 0)
  {

    // clean useless data
    cleanData(f);

  }
  else
  {
    findBestEntry(f, NULL, "POEM", NULL, SECOND, &node);

  }

}

