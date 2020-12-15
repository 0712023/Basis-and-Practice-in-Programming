#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int crack(int stage, char *answer);
#define BUF_SIZ   512
#define INPUT_LEN 6

#define SHA1HANDSOFF

#include <sys/types.h>	/* for u_int*_t */
#if defined(__sun)
#include "solarisfixes.h"
#endif

#ifndef BYTE_ORDER
#if (BSD >= 199103)
# include <machine/endian.h>
#else
#if defined(linux) || defined(__linux__)
# include <endian.h>
#else
#define	LITTLE_ENDIAN	1234	/* least-significant byte first (vax, pc) */
#define	BIG_ENDIAN	4321	/* most-significant byte first (IBM, net) */
#define	PDP_ENDIAN	3412	/* LSB first in word, MSW first in long (pdp)*/

#if defined(vax) || defined(ns32000) || defined(sun386) || defined(__i386__) || \
  defined(MIPSEL) || defined(_MIPSEL) || defined(BIT_ZERO_ON_RIGHT) ||  \
  defined(__alpha__) || defined(__alpha)
#define BYTE_ORDER	LITTLE_ENDIAN
#endif

#if defined(sel) || defined(pyr) || defined(mc68000) || defined(sparc) || \
  defined(is68k) || defined(tahoe) || defined(ibm032) || defined(ibm370) || \
  defined(MIPSEB) || defined(_MIPSEB) || defined(_IBMR2) || defined(DGUX) || \
  defined(apollo) || defined(__convex__) || defined(_CRAY) ||           \
  defined(__hppa) || defined(__hp9000) ||                               \
  defined(__hp9000s300) || defined(__hp9000s700) ||                     \
  defined (BIT_ZERO_ON_LEFT) || defined(m68k) || defined(__sparc)
#define BYTE_ORDER	BIG_ENDIAN
#endif
#endif /* linux */
#endif /* BSD */
#endif /* BYTE_ORDER */

#if defined(__BYTE_ORDER) && !defined(BYTE_ORDER)
#if (__BYTE_ORDER == __LITTLE_ENDIAN)
#define BYTE_ORDER LITTLE_ENDIAN
#else
#define BYTE_ORDER BIG_ENDIAN
#endif
#endif

#if !defined(BYTE_ORDER) ||                                   \
  (BYTE_ORDER != BIG_ENDIAN && BYTE_ORDER != LITTLE_ENDIAN && \
   BYTE_ORDER != PDP_ENDIAN)
/* you must determine what the correct bit order is for
 * your compiler - the next line is an intentional error
 * which will force your compiles to bomb until you fix
 * the above macros.
 */
#error "Undefined or invalid BYTE_ORDER"
#endif

#define rol(value, bits) (((value) << (bits)) | ((value) >> (32 - (bits))))

/* blk0() and blk() perform the initial expand. */
/* I got the idea of expanding during the round function from SSLeay */
#if BYTE_ORDER == LITTLE_ENDIAN
#define blk0(i) (block->l[i] = (rol(block->l[i],24)&0xFF00FF00) \
                 |(rol(block->l[i],8)&0x00FF00FF))
#elif BYTE_ORDER == BIG_ENDIAN
#define blk0(i) block->l[i]
#else
#error "Endianness not defined!"
#endif
#define blk(i) (block->l[i&15] = rol(block->l[(i+13)&15]^block->l[(i+8)&15] \
                                     ^block->l[(i+2)&15]^block->l[i&15],1))

/* (R0+R1), R2, R3, R4 are the different operations used in SHA1 */
#define R0(v,w,x,y,z,i) z+=((w&(x^y))^y)+blk0(i)+0x5A827999+rol(v,5);w=rol(w,30);
#define R1(v,w,x,y,z,i) z+=((w&(x^y))^y)+blk(i)+0x5A827999+rol(v,5);w=rol(w,30);
#define R2(v,w,x,y,z,i) z+=(w^x^y)+blk(i)+0x6ED9EBA1+rol(v,5);w=rol(w,30);
#define R3(v,w,x,y,z,i) z+=(((w|x)&y)|(w&x))+blk(i)+0x8F1BBCDC+rol(v,5);w=rol(w,30);
#define R4(v,w,x,y,z,i) z+=(w^x^y)+blk(i)+0xCA62C1D6+rol(v,5);w=rol(w,30);
typedef struct {
  u_int32_t state[5];
  u_int32_t count[2];
  unsigned char buffer[64];
} SHA1_CTX;

void SHA1Transform(u_int32_t state[5], const unsigned char buffer[64]);
void SHA1Init(SHA1_CTX* context);
void SHA1Update(SHA1_CTX* context, const unsigned char* data, u_int32_t len);
void SHA1Final(unsigned char digest[20], SHA1_CTX* context);

/* Hash a single 512-bit block. This is the core of the algorithm. */

void SHA1Transform(u_int32_t state[5], const unsigned char buffer[64])
{
  u_int32_t a, b, c, d, e;
  typedef union {
    unsigned char c[64];
    u_int32_t l[16];
  } CHAR64LONG16;
#ifdef SHA1HANDSOFF
  CHAR64LONG16 block[1];  /* use array to appear as a pointer */
  memcpy(block, buffer, 64);
#else
  /* The following had better never be used because it causes the
   * pointer-to-const buffer to be cast into a pointer to non-const.
   * And the result is written through.  I threw a "const" in, hoping
   * this will cause a diagnostic.
   */
  CHAR64LONG16* block = (const CHAR64LONG16*)buffer;
#endif
  /* Copy context->state[] to working vars */
  a = state[0];
  b = state[1];
  c = state[2];
  d = state[3];
  e = state[4];
  /* 4 rounds of 20 operations each. Loop unrolled. */
  R0(a,b,c,d,e, 0); R0(e,a,b,c,d, 1); R0(d,e,a,b,c, 2); R0(c,d,e,a,b, 3);
  R0(b,c,d,e,a, 4); R0(a,b,c,d,e, 5); R0(e,a,b,c,d, 6); R0(d,e,a,b,c, 7);
  R0(c,d,e,a,b, 8); R0(b,c,d,e,a, 9); R0(a,b,c,d,e,10); R0(e,a,b,c,d,11);
  R0(d,e,a,b,c,12); R0(c,d,e,a,b,13); R0(b,c,d,e,a,14); R0(a,b,c,d,e,15);
  R1(e,a,b,c,d,16); R1(d,e,a,b,c,17); R1(c,d,e,a,b,18); R1(b,c,d,e,a,19);
  R2(a,b,c,d,e,20); R2(e,a,b,c,d,21); R2(d,e,a,b,c,22); R2(c,d,e,a,b,23);
  R2(b,c,d,e,a,24); R2(a,b,c,d,e,25); R2(e,a,b,c,d,26); R2(d,e,a,b,c,27);
  R2(c,d,e,a,b,28); R2(b,c,d,e,a,29); R2(a,b,c,d,e,30); R2(e,a,b,c,d,31);
  R2(d,e,a,b,c,32); R2(c,d,e,a,b,33); R2(b,c,d,e,a,34); R2(a,b,c,d,e,35);
  R2(e,a,b,c,d,36); R2(d,e,a,b,c,37); R2(c,d,e,a,b,38); R2(b,c,d,e,a,39);
  R3(a,b,c,d,e,40); R3(e,a,b,c,d,41); R3(d,e,a,b,c,42); R3(c,d,e,a,b,43);
  R3(b,c,d,e,a,44); R3(a,b,c,d,e,45); R3(e,a,b,c,d,46); R3(d,e,a,b,c,47);
  R3(c,d,e,a,b,48); R3(b,c,d,e,a,49); R3(a,b,c,d,e,50); R3(e,a,b,c,d,51);
  R3(d,e,a,b,c,52); R3(c,d,e,a,b,53); R3(b,c,d,e,a,54); R3(a,b,c,d,e,55);
  R3(e,a,b,c,d,56); R3(d,e,a,b,c,57); R3(c,d,e,a,b,58); R3(b,c,d,e,a,59);
  R4(a,b,c,d,e,60); R4(e,a,b,c,d,61); R4(d,e,a,b,c,62); R4(c,d,e,a,b,63);
  R4(b,c,d,e,a,64); R4(a,b,c,d,e,65); R4(e,a,b,c,d,66); R4(d,e,a,b,c,67);
  R4(c,d,e,a,b,68); R4(b,c,d,e,a,69); R4(a,b,c,d,e,70); R4(e,a,b,c,d,71);
  R4(d,e,a,b,c,72); R4(c,d,e,a,b,73); R4(b,c,d,e,a,74); R4(a,b,c,d,e,75);
  R4(e,a,b,c,d,76); R4(d,e,a,b,c,77); R4(c,d,e,a,b,78); R4(b,c,d,e,a,79);
  /* Add the working vars back into context.state[] */
  state[0] += a;
  state[1] += b;
  state[2] += c;
  state[3] += d;
  state[4] += e;
  /* Wipe variables */
  a = b = c = d = e = 0;
#ifdef SHA1HANDSOFF
  memset(block, '\0', sizeof(block));
#endif
}


/* SHA1Init - Initialize new context */

void SHA1Init(SHA1_CTX* context)
{
  /* SHA1 initialization constants */
  context->state[0] = 0x67452301;
  context->state[1] = 0xEFCDAB89;
  context->state[2] = 0x98BADCFE;
  context->state[3] = 0x10325476;
  context->state[4] = 0xC3D2E1F0;
  context->count[0] = context->count[1] = 0;
}


/* Run your data through this. */

void SHA1Update(SHA1_CTX* context, const unsigned char* data, u_int32_t len)
{
  u_int32_t i;
  u_int32_t j;

  j = context->count[0];
  if ((context->count[0] += len << 3) < j)
    context->count[1]++;
  context->count[1] += (len>>29);
  j = (j >> 3) & 63;
  if ((j + len) > 63) {
    memcpy(&context->buffer[j], data, (i = 64-j));
    SHA1Transform(context->state, context->buffer);
    for ( ; i + 63 < len; i += 64) {
      SHA1Transform(context->state, &data[i]);
    }
    j = 0;
  }
  else i = 0;
  memcpy(&context->buffer[j], &data[i], len - i);
}


/* Add padding and return the message digest. */

void SHA1Final(unsigned char digest[20], SHA1_CTX* context)
{
  unsigned i;
  unsigned char finalcount[8];
  unsigned char c;

#if 0	/* untested "improvement" by DHR */
  /* Convert context->count to a sequence of bytes
   * in finalcount.  Second element first, but
   * big-endian order within element.
   * But we do it all backwards.
   */
  unsigned char *fcp = &finalcount[8];

  for (i = 0; i < 2; i++)
  {
    u_int32_t t = context->count[i];
    int j;

    for (j = 0; j < 4; t >>= 8, j++)
      *--fcp = (unsigned char) t
        }
#else
  for (i = 0; i < 8; i++) {
    finalcount[i] = (unsigned char)((context->count[(i >= 4 ? 0 : 1)]
                                     >> ((3-(i & 3)) * 8) ) & 255);  /* Endian independent */
  }
#endif
  c = 0200;
  SHA1Update(context, &c, 1);
  while ((context->count[0] & 504) != 448) {
    c = 0000;
    SHA1Update(context, &c, 1);
  }
  SHA1Update(context, finalcount, 8);  /* Should cause a SHA1Transform() */
  for (i = 0; i < 20; i++) {
    digest[i] = (unsigned char)
      ((context->state[i>>2] >> ((3-(i & 3)) * 8) ) & 255);
  }
  /* Wipe variables */
  memset(context, '\0', sizeof(*context));
  memset(&finalcount, '\0', sizeof(finalcount));
}
/* ================ end of sha1.c ================ */

#define BUFSIZE 4096

void make_sha(int stage, int seed, unsigned char hash[20])
{
  SHA1_CTX ctx;
  unsigned char buf[BUFSIZE];
  int i;
  for(i=0;i<BUFSIZE;i++)
    buf[i] = i * i - i + 3;
  buf[3+stage] = stage;
  buf[9] = seed;

  SHA1Init(&ctx);
  for(i=0;i<1000;i++)
    SHA1Update(&ctx, buf, BUFSIZE);
  SHA1Final(hash, &ctx);
}

void print_sha(unsigned char hash[20])
{
  for(int i=0;i<20;i++)
    printf("%02x", hash[i]);
}
#define STAGE_MAX  10

#define ALPHA_LEN 4
#define DIGIT_START 2
#define S_LEN 5

#define NAME    0
#define YEAR   1
#define NICK    2
#define COUNTRY 3
#define SID     4

int seed = 2122;
char _xa38_d8a[STAGE_MAX][BUF_SIZ];

char *S_STR = ".!?@_";

int solved_table[STAGE_MAX];

int crack(int stage, char *buf)
{
  if (stage < 0 || stage >= STAGE_MAX)
  {
    printf("Invaild stage number, check it out\n");
    return 0;
  }
  char fibaaf[BUF_SIZ];
  strcpy(fibaaf, _xa38_d8a[stage]);
  if (strcmp(fibaaf, buf) == 0 && !(solved_table[stage]))
    /* if (strcmp(fibaaf, buf) == 0) */
  {
    printf("Stage %d passed!\n", stage);
    printf("Answer is %s\n", fibaaf);

    unsigned char hash[20];
    make_sha(stage, seed, hash);
    print_sha(hash);
    printf("\n");

    solved_table[stage] = 1;
    return 1;
  }
  return 0;
}

char **buf_malloc(int n)
{
  char **buf;
  buf = malloc(sizeof(char*) * n);
  for (int i = 0; i < n; i++)
  {
    buf[i] = malloc(sizeof(char) * BUF_SIZ);
    buf[i][0] = '\0';
  }

  return buf;
}

void buf_free(char **buf, int n)
{
  for (int i = 0; i < n; i++)
    free(buf[i]);
  free(buf);
}

void print_stage(int stage, char **buf, int n)
{

  return;
}

void x_stage_00(char input_list[INPUT_LEN][BUF_SIZ])
{
  int stage = 0;
  int n = INPUT_LEN;
  char **buf;
  buf = buf_malloc(n);

  for (int i = 0; i < INPUT_LEN; i++)
  {
    strcat(buf[i], input_list[i]);
  }

  int pick = rand() % n;
  strcpy(_xa38_d8a[stage], buf[pick]);
  print_stage(stage, buf, n);

  buf_free(buf, n);
}

void x_stage_01(char input_list[INPUT_LEN][BUF_SIZ])
{
  int stage = 1;
  int n = INPUT_LEN * (INPUT_LEN - 1);
  char **buf;
  buf = buf_malloc(n);

  int index = 0;
  for (int i = 0; i < INPUT_LEN; i++)
  {
    for (int j = 0; j < INPUT_LEN; j++)
    {
      if (i == j)
        continue;
      strcat(buf[index], input_list[i]);
      strcat(buf[index], input_list[j]);
      index++;
    }
  }

  int pick = rand() % n;
  strcpy(_xa38_d8a[stage], buf[pick]);
  print_stage(stage, buf, n);

  buf_free(buf, n);
}

void x_stage_02(char input_list[INPUT_LEN][BUF_SIZ])
{
  int stage = 2;
  int n = INPUT_LEN;
  char **buf;
  buf = buf_malloc(n);

  int index = 0;
  for (int i = 0; i < INPUT_LEN; i++)
  {
    strncat(buf[index], input_list[i], 2);
    index++;
  }

  int pick = rand() % n;
  strcpy(_xa38_d8a[stage], buf[pick]);
  print_stage(stage, buf, n);

  buf_free(buf, n);
}

void x_stage_03(char input_list[INPUT_LEN][BUF_SIZ])
{
  int stage = 3;
  int n = INPUT_LEN * (INPUT_LEN - 1) * (INPUT_LEN - 2);
  char **buf;
  buf = buf_malloc(n);

  int index = 0;
  for (int i = 0; i < INPUT_LEN; i++)
  {
    for (int j = 0; j < INPUT_LEN; j++)
    {
      for (int k = 0; k < INPUT_LEN; k++)
      {

        if (i == j || i == k || j == k)
          continue;
        strcat(buf[index], input_list[i]);
        strcat(buf[index], input_list[j]);
        strcat(buf[index], input_list[k]);
        index++;
      }
    }
  }

  int pick = rand() % n;
  strcpy(_xa38_d8a[stage], buf[pick]);
  print_stage(stage, buf, n);

  buf_free(buf, n);
}

void x_stage_04(char input_list[INPUT_LEN][BUF_SIZ])
{
  int stage = 4;
  int n = 0;
  for (int i = 0; i < INPUT_LEN; i++)
  {
    n += (strlen(input_list[i]) + 1) * S_LEN;
  }

  char **buf;
  buf = buf_malloc(n);

  int index = 0;
  for (int i = 0; i < INPUT_LEN; i++)
  {
    for (int j = 0; j < S_LEN; j++)
    {
      for (int k = 0; k < strlen(input_list[i])+1; k++)
      {
        strncpy(buf[index], input_list[i], k);
        buf[index][k] = S_STR[j];
        buf[index][k+1] = '\0';
        strcpy(buf[index] + k + 1, input_list[i] + k);
        index++;
      }
    }
  }

  int pick = rand() % n;
  strcpy(_xa38_d8a[stage], buf[pick]);

  print_stage(stage, buf, n);

  buf_free(buf, n);
}

void x_stage_05(char input_list[INPUT_LEN][BUF_SIZ])
{
  int stage = 5;
  int n = INPUT_LEN;

  char **buf;
  buf = buf_malloc(n);

  for (int i = 0; i < INPUT_LEN; i++)
  {
    int len = strlen(input_list[i]);
    for (int j = 0; j < len; j++)
    {
      buf[i][j] = input_list[i][len-j-1];
    }
    buf[i][len] = '\0';

  }

  int pick = rand() % n;
  strcpy(_xa38_d8a[stage], buf[pick]);

  print_stage(stage, buf, n);

  buf_free(buf, n);
}

void x_stage_06(char input_list[INPUT_LEN][BUF_SIZ])
{
  int stage = 6;
  int n = 0;
  for (int i = 0; i < INPUT_LEN; i++)
  {
    n += strlen(input_list[i]);
  }

  char **buf;
  buf = buf_malloc(n);

  int index = 0;
  for (int i = 0; i < INPUT_LEN; i++)
  {
    int len = strlen(input_list[i]);
    for (int j = 0; j < len; j++)
    {
      strcpy(buf[index], input_list[i]+j);
      strncpy(buf[index]+(len-j), input_list[i], j);
      buf[index][len] = '\0';
      index++;
    }
  }

  int pick = rand() % n;
  strcpy(_xa38_d8a[stage], buf[pick]);

  print_stage(stage, buf, n);

  buf_free(buf, n);
}

void x_stage_07(char input_list[INPUT_LEN][BUF_SIZ])
{
  int stage = 7;
  int n = ALPHA_LEN;

  char **buf;
  buf = buf_malloc(n);

  for (int i = 0; i < ALPHA_LEN; i++)
  {
    int len = strlen(input_list[i]);
    for (int j = 0; j < len; j++)
    {
      char c = input_list[i][j];
      if (c >= 'a' && c <= 'z')
        c -= 32;
      buf[i][j] = c;
    }
    buf[i][len] = '\0';
  }

  int pick = rand() % n;
  strcpy(_xa38_d8a[stage], buf[pick]);

  print_stage(stage, buf, n);

  buf_free(buf, n);
}

void x_stage_08(char input_list[INPUT_LEN][BUF_SIZ])
{
  int stage = 8;
  int n = INPUT_LEN - DIGIT_START;

  char **buf;
  buf = buf_malloc(n);

  /* int offset = rand() % 10; */
  int offset = 7;

  int index = 0;
  for (int i = DIGIT_START; i < INPUT_LEN; i++)
  {
    int len = strlen(input_list[i]);
    for (int j = 0; j < len; j++)
    {
      char c = input_list[i][j];
      if (c >= '0' && c <= '9')
        c = (c - '0' + offset) % 10 + '0';
      buf[index][j] = c;
    }
    buf[index][len] = '\0';
    index++;
  }

  int pick = rand() % n;
  strcpy(_xa38_d8a[stage], buf[pick]);

  print_stage(stage, buf, n);

  buf_free(buf, n);
}

void x_stage_09(char input_list[INPUT_LEN][BUF_SIZ])
{
  int stage = 9;
  int n = INPUT_LEN;

  char **buf;
  buf = buf_malloc(n);

  /* int offset = rand() % 10; */
  int offset = 14;

  int index = 0;
  for (int i = 0; i < INPUT_LEN; i++)
  {
    int len = strlen(input_list[i]);
    for (int j = 0; j < len; j++)
    {
      char c = input_list[i][j];
      if (c >= 'a' && c <= 'z')
        c = (c - 'a' + offset) % 26 + 'a';
      else if (c >= 'A' && c <= 'Z')
        c = (c - 'A' + offset) % 26 + 'A';
      buf[index][j] = c;
    }
    buf[index][len] = '\0';
    index++;
  }

  int pick = rand() % n;
  strcpy(_xa38_d8a[stage], buf[pick]);

  print_stage(stage, buf, n);

  buf_free(buf, n);
}


void make_answer(char input_list[INPUT_LEN][BUF_SIZ])
{
  srand(seed);
  for (int i = 0; i < STAGE_MAX; i++)
    solved_table[i] = 0;


  x_stage_00(input_list);
  x_stage_01(input_list);
  x_stage_02(input_list);
  x_stage_03(input_list);
  x_stage_04(input_list);
  x_stage_05(input_list);
  x_stage_06(input_list);
  x_stage_07(input_list);
  x_stage_08(input_list);
  x_stage_09(input_list);
}
void stage_00(char input_list[INPUT_LEN][BUF_SIZ]);
void stage_01(char input_list[INPUT_LEN][BUF_SIZ]);
void stage_02(char input_list[INPUT_LEN][BUF_SIZ]);
void stage_03(char input_list[INPUT_LEN][BUF_SIZ]);
void stage_04(char input_list[INPUT_LEN][BUF_SIZ]);
void stage_05(char input_list[INPUT_LEN][BUF_SIZ]);
void stage_06(char input_list[INPUT_LEN][BUF_SIZ]);
void stage_07(char input_list[INPUT_LEN][BUF_SIZ]);
void stage_08(char input_list[INPUT_LEN][BUF_SIZ]);
void stage_09(char input_list[INPUT_LEN][BUF_SIZ]);

int main(int argc, char *argv[])
{

  int stage;
  char name[BUF_SIZ];
  char country[BUF_SIZ];
  char nickname[BUF_SIZ];
  char fav_music[BUF_SIZ];
  char birth_year[BUF_SIZ];
  char sid[BUF_SIZ];

  char input_list[INPUT_LEN][BUF_SIZ];

  scanf("%d", &seed);
  scanf("%d", &stage);


  scanf("%s", name);
  scanf("%s", country);
  scanf("%s", nickname);
  scanf("%s", fav_music);
  scanf("%s", birth_year);
  scanf("%s", sid);

  strcpy(input_list[0], name);
  strcpy(input_list[1], country);
  strcpy(input_list[2], nickname);
  strcpy(input_list[3], fav_music);
  strcpy(input_list[4], birth_year);
  strcpy(input_list[5], sid);

  make_answer(input_list);

  switch (stage) {
  case 0: {
    stage_00(input_list);
    break;
  }
  case 1: {
    stage_01(input_list);
    break;
  }
  case 2: {
    stage_02(input_list);
    break;
  }
  case 3: {
    stage_03(input_list);
    break;
  }
  case 4: {
    stage_04(input_list);
    break;
  }
  case 5: {
    stage_05(input_list);
    break;
  }
  case 6: {
    stage_06(input_list);
    break;
  }
  case 7: {
    stage_07(input_list);
    break;
  }
  case 8: {
    stage_08(input_list);
    break;
  }
  case 9: {
    stage_09(input_list);
    break;
  }
  default:
    printf("invalid stage number\n");
    break;
  }
  return 0;
}
void stage_00(char input_list[INPUT_LEN][BUF_SIZ])
{
  for(int i = 0; i<6;i++){
    crack(0, input_list[i]);
  }
  return;
}


void stage_01(char input_list[INPUT_LEN][BUF_SIZ])
{
  for (int i = 0; i< 6; i++){
    char tmp[50] = "";
    for (int j =0; j<6; j++){
      if(i!=j){
        strcpy(tmp, input_list[i]);
        strcat(tmp, input_list[j]);
        crack(1, tmp);
        char tmp[] = "";
      }
    }
  }
  return;
}

void stage_02(char input_list[INPUT_LEN][BUF_SIZ])
{
  char twolet[7][20] = {};
  for(int i = 0;i<6;i++){
    twolet[i][0] = input_list[i][0];
    twolet[i][1] = input_list[i][1];
  }
  for(int i = 0; i<6;i++){
    crack(2, twolet[i]);
  }
  return;
}

void stage_03(char input_list[INPUT_LEN][BUF_SIZ])
{
  for (int i = 0; i<6; i++){
    char tmp[100] = {};
    for (int j =0; j<6; j++){
      if(i!=j){
        for(int k = 0; k<6;k++){
          if(j!=k){
            strcpy(tmp, input_list[i]);
            strcat(tmp, input_list[j]);
            strcat(tmp, input_list[k]);
            crack(3, tmp);
            char tmp[] = {};
            strcpy(tmp, input_list[i]);
            strcat(tmp, input_list[j]);
          }
        }
        char tmp[] = {};
        strcpy(tmp, input_list[i]);
      }
    }
  }
  return;
}


void stage_04(char input_list[INPUT_LEN][BUF_SIZ])
{
  printf("\n");
  char pnt[20] = ".";
  char exc[20] = "!";
  char qes[20] = "?";
  char at[20] = "@";
  char unb[20] = "_";
  
  char buffer[50] = ""; 
  for(int i = 0; i<6;i++){
    strcpy(buffer, pnt);
    strcat(buffer, input_list[i]);
    crack(4, buffer);
    for(int j = 0; j<strlen(input_list[i]);j++){
      char tmp[2] = {};
      tmp[0] = buffer[j];
  	  buffer[j] = buffer[j+1];
  	  buffer[j+1] = tmp[0];  
      crack(4, buffer);
    }
  }
  
  for(int i = 0; i<6;i++){
    strcpy(buffer, exc);
    strcat(buffer, input_list[i]);
    crack(4, buffer);
    for(int j = 0; j<strlen(input_list[i]);j++){
      char tmp[2] = {};
      tmp[0] = buffer[j];
  	  buffer[j] = buffer[j+1];
  	  buffer[j+1] = tmp[0];  
      crack(4, buffer);
    }
  }
  
  for(int i = 0; i<6;i++){
    strcpy(buffer, qes);
    strcat(buffer, input_list[i]);
    crack(4, buffer);
    for(int j = 0; j<strlen(input_list[i]);j++){
      char tmp[2] = {};
      tmp[0] = buffer[j];
  	  buffer[j] = buffer[j+1];
  	  buffer[j+1] = tmp[0];
      crack(4, buffer);
    }
  }
  
  for(int i = 0; i<6;i++){
    strcpy(buffer, at);
    strcat(buffer, input_list[i]);
    crack(4, buffer);
    for(int j = 0; j<strlen(input_list[i]);j++){
      char tmp[2] = {};
      tmp[0] = buffer[j];
  	  buffer[j] = buffer[j+1];
  	  buffer[j+1] = tmp[0];
      crack(4, buffer);
    }
  }
  
  for(int i = 0; i<6;i++){
    strcpy(buffer, unb);
    strcat(buffer, input_list[i]);
    crack(4, buffer);
    for(int j = 0; j<strlen(input_list[i]);j++){
      char tmp[2] = {};
      tmp[0] = buffer[j];
  	  buffer[j] = buffer[j+1];
  	  buffer[j+1] = tmp[0];
      crack(4, buffer);
    }
  }
  return;
}

void stage_05(char input_list[INPUT_LEN][BUF_SIZ])
{
  char buffer[50] = "";
  for(int i = 0;i<6;i++){
    strcpy(buffer, input_list[i]);
    for(int j = 0; j<strlen(input_list[i]); j++){
      for(int k = 0; k<strlen(input_list[i])-j-1; k++){
        char tmp[2] = {};
        tmp[0] = buffer[k];
        buffer[k] = buffer[k+1];
        buffer[k+1] = tmp[0];
      }
    }
    crack(5, buffer);
  }
  return;
}

void stage_06(char input_list[INPUT_LEN][BUF_SIZ])
{
  char buffer[50] = {};
  for(int i = 0;i<6;i++){
    strcpy(buffer, input_list[i]);
    for(int j = 0 ; j<strlen(input_list[i]);j++){
      for(int k = 0; k<strlen(input_list[i])-1; k++){
        char tmp[2] = {};
        tmp[0] = buffer[k];
        buffer[k] = buffer[k+1];
        buffer[k+1] = tmp[0];
      }
      crack(6,buffer);
    }
  }
  return;
}

void stage_07(char input_list[INPUT_LEN][BUF_SIZ])
{
  char buffer[50] = {};
  for(int i = 0;i<6;i++){
    strcpy(buffer, input_list[i]);
    for(int j = 0 ; j<strlen(input_list[i]);j++){
      if(buffer[j]>96){
        buffer[j]-=32;
      }
    }
    crack(7,buffer);
  }
  return;
}

void stage_08(char input_list[INPUT_LEN][BUF_SIZ])
{
  char buffer[50] = {};
  for(int i = 0;i<6;i++){
    strcpy(buffer, input_list[i]);
    for(int j = 0 ; j<strlen(input_list[i]);j++){
      if(buffer[j]>47 && buffer[j]<51){
        buffer[j]+=7;
      } else if (buffer[j]>50 && buffer[j]<58){
        buffer[j]-=3;
      }
    }
    crack(8,buffer);
  }
  return;
}

void stage_09(char input_list[INPUT_LEN][BUF_SIZ])
{
  char buffer[50] = {};
  for(int i = 0;i<6;i++){
    strcpy(buffer, input_list[i]);
    for(int j = 0 ; j<strlen(input_list[i]);j++){
      if(buffer[j]>64 && buffer[j]<77 || buffer[j]>96 && buffer[j]<109){
        buffer[j]+=14;
      } else if(buffer[j]>76 && buffer[j]<91 || buffer[j]>108 && buffer[j]<123){
        buffer[j]-=12;
      }
    }
    crack(9,buffer);
  }
  return;
}

