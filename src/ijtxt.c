/*
 *   Copyright (c) 2023 Mark Wayne Menorca
 *   All rights reserved.

 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 
 *   The above copyright notice and this permission notice shall be included in all
 *   copies or substantial portions of the Software.
 
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *   SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ijtxt.h"
/*
 *   Copyright (c) 2023 
 *   All rights reserved.
 */
int inject_text(const char *filename, int line_number, int insertion_pos,const char *text) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Failed to open file");
        return 1;
    }

    char buffer[4096];
    int line_count = 0;
    size_t write_offset = 0;
    size_t insert_len = strlen(text);

    while (fgets(buffer, sizeof(buffer), fp)) {
        if (line_count++ == line_number) {
            write_offset = ftell(fp) - strlen(buffer);
            break;
        }
    }

    fclose(fp);

    if (line_count < line_number) {
        fprintf(stderr, "Error: File only contains %d lines\n", line_count);
        return 1;
    }

    fp = fopen(filename, "r+");
    if (!fp) {
        perror("Failed to open file");
        return 1;
    }

    if (fseek(fp, write_offset + insertion_pos, SEEK_SET) != 0) {
        perror("Failed to seek to write position");
        return 1;
    }

    size_t read_offset = write_offset + insertion_pos;
    size_t bytes_to_read = ftell(fp) - read_offset;

    char *read_buffer = (char *) malloc(bytes_to_read);
    if (!read_buffer) {
        perror("Failed to allocate memory");
        return 1;
    }

    if (fread(read_buffer, 1, bytes_to_read, fp) != bytes_to_read) {
        perror("Failed to read from file");
        free(read_buffer);
        return 1;
    }

    if (fseek(fp, write_offset + insertion_pos, SEEK_SET) != 0) {
        perror("Failed to seek to write position");
        free(read_buffer);
        return 1;
    }

    if (fwrite(text, 1, insert_len, fp) != insert_len) {
        perror("Failed to write to file");
        free(read_buffer);
        return 1;
    }

    if (fwrite(read_buffer, 1, bytes_to_read, fp) != bytes_to_read) {
        perror("Failed to write to file");
        free(read_buffer);
        return 1;
    }

    fclose(fp);
    free(read_buffer);

    return 0;
}
