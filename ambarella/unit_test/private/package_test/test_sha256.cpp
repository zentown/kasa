/*
 * test_sha256.cpp
 *
 * History:
 *	2015/07/22 - [Zhi He] create file
 *
 * Copyright (C) 2014 - 2024, the Ambarella Inc.
 *
 * All rights reserved. No Part of this file may be reproduced, stored
 * in a retrieval system, or transmitted, in any form, or by any means,
 * electronic, mechanical, photocopying, recording, or otherwise,
 * without the prior consent of the Ambarella Inc.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "cryptography_if.h"

#define TSHA256_MAX_FILE_NAME_LENGTH 512
#define TSHA256_MAX_STRING_LENGTH 256

typedef struct
{
    unsigned char b_file;
    unsigned char b_string;
    unsigned char reserved_0;
    unsigned char reserved_1;

    char infile[TSHA256_MAX_FILE_NAME_LENGTH];
    char instr[TSHA256_MAX_STRING_LENGTH];
} tsha256_context;

static void __print_sha256_digest(unsigned char* d)
{
    printf("%02x%02x%02x%02x%02x%02x%02x%02x", d[0], d[1], d[2], d[3], d[4], d[5], d[6], d[7]);
    d += 8;
    printf("%02x%02x%02x%02x%02x%02x%02x%02x", d[0], d[1], d[2], d[3], d[4], d[5], d[6], d[7]);
    d += 8;
    printf("%02x%02x%02x%02x%02x%02x%02x%02x", d[0], d[1], d[2], d[3], d[4], d[5], d[6], d[7]);
    d += 8;
    printf("%02x%02x%02x%02x%02x%02x%02x%02x\n", d[0], d[1], d[2], d[3], d[4], d[5], d[6], d[7]);
}

static void show_usage()
{
    printf("test_sha256 usage:\n");
    printf("\t-f: file\n");
    printf("\t-s: string\n");

    printf("\t--help: show usage\n");
}

static int init_params(int argc, char **argv, tsha256_context* context)
{
    int i = 0;

    for (i = 1; i < argc; i ++) {
        if (!strcmp("-f", argv[i])) {
            if ((i + 1) < argc) {
                snprintf(context->infile, TSHA256_MAX_FILE_NAME_LENGTH, "%s", argv[i + 1]);
                printf("[input argument]: '-f': (filename %s).\n", context->infile);
                context->b_file = 1;
            } else {
                printf("[input argument error]: '-f', should follow with file name, argc %d, i %d.\n", argc, i);
                return (-1);
            }
            i ++;
        } else if (!strcmp("-s", argv[i])) {
            if ((i + 1) < argc) {
                snprintf(context->instr, TSHA256_MAX_STRING_LENGTH, "%s", argv[i + 1]);
                printf("[input argument]: '-s': (string %s).\n", context->instr);
                context->b_string = 1;
            } else {
                printf("[input argument error]: '-s', should follow with input string, argc %d, i %d.\n", argc, i);
                return (-1);
            }
            i ++;
        } else if (!strcmp("--help", argv[i])) {
            show_usage();
            return 1;
        } else {
            printf("[input argument error]: unknwon input params, [%d][%s]\n", i, argv[i]);
            return (-20);
        }
    }

    return 0;
}

int main(int argc, char** argv)
{
    FILE* p_infile = NULL;
    int ret = 0;
    unsigned char digest[32];
    tsha256_context context;

    memset(&context, 0x0, sizeof(context));

    if (argc < 2) {
        show_usage();
        ret = 1;
        goto fail_exit;
    }

    ret = init_params(argc, argv, &context);
    if (0 > ret) {
        printf("[error]: init_params fail\n");
        ret = -1;
        goto fail_exit;
    }

    if (context.b_file) {
        p_infile = fopen(context.infile, "rb");
        if (!p_infile) {
            printf("[error]: open input file(%s) fail\n", context.infile);
            ret = -2;
            goto fail_exit;
        }

        //do sha256 with some data segment
        unsigned int n;
        unsigned char buf[1024];
        void* p_sha256 = digest_sha256_init();
        if (p_sha256) {
            while ((n = fread(buf, 1, sizeof(buf), p_infile)) > 0) {
                digest_sha256_update(p_sha256, buf, n);
            }
            digest_sha256_final(p_sha256, digest);
            free(p_sha256);
            p_sha256 = NULL;

            printf("[file %s's sha256]:\n", context.infile);
            __print_sha256_digest(digest);
        } else {
            printf("[error]: digest_sha256_init fail\n");
            ret = -3;
            goto fail_exit;
        }
    }

    if (context.b_string) {
        unsigned int len = strlen(context.instr);

        //do sha256 with one data segment
        digest_sha256((const unsigned char*) context.instr, len, digest);

        printf("[string %s's sha256]:\n", context.instr);
        __print_sha256_digest(digest);
    }

    ret = 0;

fail_exit:
    if (p_infile) {
        fclose(p_infile);
        p_infile = NULL;
    }

    return ret;
}
