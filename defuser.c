#include "defuser.h"

#include <inttypes.h>
#include <stdio.h>

#define MAX_FIELDS 8

/*
 * 把 values 中的 count 个整数写成一行“空格分隔的无符号十进制整数”，存入 out。
 * 成功返回 0；count 为 0（还没有实现）、snprintf 出错或结果被截断时返回非零。
 */
static int write_answer(char *out, size_t cap, const uint32_t *values, size_t count) {
    size_t used = 0;
    for (size_t i = 0; i < count; ++i) {
        int size = snprintf(out + used, cap - used, "%s%" PRIu32, i ? " " : "", values[i]);
        if (size < 0 || (size_t)size >= cap - used) return 1;
        used += (size_t)size;
    }
    return count == 0;
}

int solve_phase_1(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 1;

    uint32_t hi  = (uint32_t)(challenge >> 32);
    uint32_t lo  = (uint32_t)challenge;
    uint32_t mid = (uint32_t)(challenge >> 16);

    uint32_t b = 3u * (hi - mid) + 0xcba0u;     /* p1_step1 */
    uint32_t a = (5u * lo + hi) ^ 0x2e81u;      /* p1_step0 */
    values[0]  = (a + 8u * b) ^ 0x6ee2u;        /* p1_step2 */

    return write_answer(out, cap, values, count);
}

int solve_phase_2(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 6;

   
    values[0] = (uint32_t)((challenge >> 34) & 0x1F) + 0x16u;

   
    for (size_t i = 1; i < 6; ++i) {
        uint32_t part = (uint32_t)((challenge >> (2 * i)) & 0x1F) + (uint32_t)i;
        values[i] = part ^ (3u * values[i - 1]);
    }

    return write_answer(out, cap, values, count);
}

int solve_phase_3(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 2;

    uint32_t sel = (uint32_t)(challenge & 7);
    uint32_t x   = (uint32_t)(challenge >> 3) ^ (uint32_t)(challenge >> 32);
    uint32_t r;

    switch (sel) {
        case 0: r = 5u * x + 0x991u;                  break;
        case 1: r = 5u * x + 0x3971fu;                break;
        case 2: r = (x ^ 0x849au) + (x >> 2);         break;
        case 3: r = 5u * x + 0x25bcfu;                break;
        case 4: r = (x ^ 0xa449u) + (x >> 1);         break;
        case 5: r = (4u * x) ^ 0xf0c7u;               break;
        case 6: r = (2u * x) ^ 0xf08au;               break;
        case 7: r = 5u * x + 0xa1adu;                 break;
        default: return 1;
    }

    values[0] = sel;
    values[1] = r;
    return write_answer(out, cap, values, count);
}

/* 第 4 关：二分式递归。
   mid = lo + (hi-lo)/2；命中返回 (mid+0x4c)^salt；
   否则按大小走右半/左半，回程再 3*ret ^ (mid+salt) ^ 0x5 / 0x11。 */
static uint32_t p4_walk(uint32_t target, int32_t lo, int32_t hi, uint32_t salt) {
    int32_t mid = lo + ((hi - lo) >> 1);
    if ((uint32_t)mid == target)
        return (uint32_t)(mid + 0x4c) ^ salt;
    if (target > (uint32_t)mid) {
        uint32_t r = p4_walk(target, mid + 1, hi, salt);
        return ((3u * r) ^ (uint32_t)(mid + salt)) ^ 0x5u;
    } else {
        uint32_t r = p4_walk(target, lo, mid - 1, salt);
        return ((3u * r) ^ (uint32_t)(mid + salt)) ^ 0x11u;
    }
}

int solve_phase_4(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 1;                          /* 本关只要 1 个整数 */

    uint32_t target = (uint32_t)(challenge & 0x7f);
    uint32_t salt   = (uint32_t)(challenge >> 7) ^ (uint32_t)(challenge >> 32);

    values[0] = p4_walk(target, 0, 0x7f, salt);
    return write_answer(out, cap, values, count);
}

int solve_phase_5(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 0;
    (void)challenge;
    // TODO: 根据 phase_05 的汇编，用 challenge 算出口令中的各个整数，依次存入 values，并把 count 改为整数个数。
    return write_answer(out, cap, values, count);
}

int solve_phase_6(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 0;
    (void)challenge;
    // TODO: 根据 phase_06 的汇编，用 challenge 算出口令中的各个整数，依次存入 values，并把 count 改为整数个数。
    return write_answer(out, cap, values, count);
}

int solve_secret(uint64_t challenge, uint64_t context, char *out, size_t cap) {
    (void)challenge;
    (void)context;
    (void)out;
    (void)cap;
    // TODO（选做）: 根据 secret_gate、secret_copy 和 secret_phase 的汇编构造完整的第七行，写入 out 后返回 0。
    // 第七行不一定只有整数，可以直接用 snprintf 写入；还没完成时保持返回 1，测试会显示 not attempted。
    return 1;
}
