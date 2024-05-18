#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <assert.h>
#include <stdlib.h>

static const char TABLE[64] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

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
        strl = NEXT_MULT_OF(strl, 3);
    }

    char padded_str[strl];
    memcpy(padded_str, str, strl);

    char* r = malloc(pre_computed_len + 1);
    assert(r != NULL && "buy more ram lol");

    uint32_t i0, i1, i2, i3;
    for (size_t i = 0, cnt = 0; i < strl; i += 3, cnt += 4) {
        i0 = padded_str[i] >> 2;
        i1 = (padded_str[i] & 0x03) << 4 | (padded_str[i + 1] >> 4 & 0x0F);
        i2 = (padded_str[i + 1] & 0x0F) << 2 | (padded_str[i + 2] >> 6 & 0x0F);
        i3 = padded_str[i + 2] & 0x3F;

        r[cnt + 0] = TABLE[i0];
        r[cnt + 1] = TABLE[i1];
        r[cnt + 2] = TABLE[i2];
        r[cnt + 3] = TABLE[i3];
    }

    for (size_t i = pre_computed_len - padding; i < pre_computed_len; i++) {
        r[i] = '=';
    }

    r[pre_computed_len] = '\0';
    return r;
}

int main(void) {
    const char *m = "M";
    char *str_b64 = b64(m, strlen(m));
    printf("%s\n", str_b64);
    return 0;
}