#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <assert.h>
#include <stdlib.h>

static const char TABLE[64] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

#define I1(c0)      (uint32_t)(c0)>>2
#define I2(c0, c1) ((uint32_t)(c0)&0x03)<<4 | (((uint32_t)(c1)>>4)&0x0F)
#define I3(c1, c2) ((uint32_t)(c1)&0x0F)<<2 | (((uint32_t)(c2)>>6)&0x0F)
#define I4(c2)      (uint32_t)(c2)&0x3F

#define NEXT_MULT_OF(v, of) (v) + ((of) - (v) % (of))

size_t pre_calc_len(size_t n, size_t* padding) {
    size_t l = 4.0*(n/3.0);
    if (l % 4 != 0) {
        size_t t = l;
        l = NEXT_MULT_OF(l, 4);
        if (padding) {
            *padding = l - t - 1;
        }
    }

    return l;
}

char* b64(const char* str, size_t strl) {
    size_t padding = 0;
    size_t pre_computed_len = pre_calc_len(strl, &padding);

    if (strl % 3 != 0) {
        strl = NEXT_MULT_OF(strl, 4);
    }

    char *padded_str = calloc(strl, sizeof(char));
    assert(padded_str != NULL);
    assert(memcpy(padded_str, str, strl) != NULL);

    char* r = malloc(pre_computed_len + 1);
    assert(r != NULL);

    unsigned char c0, c1, c2;
    size_t cnt = 0;
    for (size_t i = 0; i < strl; i += 3) {
        c0 = padded_str[i + 0];
        c1 = padded_str[i + 1];
        c2 = padded_str[i + 2];
        r[cnt++] = TABLE[(uint32_t)c0>>2];
        r[cnt++] = TABLE[((uint32_t)c0&0x03)<<4 | (((uint32_t)c1>>4)&0x0F)];
        r[cnt++] = TABLE[((uint32_t)c1&0x0F)<<2 | (((uint32_t)c2>>6)&0x0F)];
        r[cnt++] = TABLE[(uint32_t)c2&0x3F];
    }

    for (size_t i = pre_computed_len - padding; i < pre_computed_len; i++) {
        r[i] = '=';
    }

    r[pre_computed_len] = '\0';
    free(padded_str);
    return r;
}

int main(void) {
    const char *m = "M";
    char *str_b64 = b64(m, strlen(m));
    printf("%s\n", str_b64);
    return 0;
}