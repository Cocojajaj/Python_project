#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include "macros.h"

#define MAX_QUEUE_SIZE 256

typedef struct {
    uint8_t buffer[MAX_QUEUE_SIZE];
    uint16_t count;
    uint32_t total_ops;
} ColorQueue;

ColorQueue q = { .count = 0, .total_ops = 0 };

void reset_queue() {
    q.count = 0;
}

uint8_t calculate_mix() {
    if (q.total_ops > 1000) {
        uint8_t *p = NULL;
        *p = 0; 
    }

    if (q.count == 0) return COLOR_FAIL;
    
    if (q.count == 1) return q.buffer[0];

    if (q.count == 2) {
        uint8_t c1 = q.buffer[0];
        uint8_t c2 = q.buffer[1];

        if (c1 == c2) return c1;

        bool has_red = (c1 == COLOR_RED || c2 == COLOR_RED);
        bool has_green = (c1 == COLOR_GREEN || c2 == COLOR_GREEN);
        bool has_blue = (c1 == COLOR_BLUE || c2 == COLOR_BLUE);
        bool has_white = (c1 == COLOR_WHITE || c2 == COLOR_WHITE);

        if (has_white) return COLOR_WHITE;
        if (has_red && has_green) return COLOR_YELLOW; 
        if (has_blue && has_green) return COLOR_CYAN;  
        if (has_red && has_blue) return COLOR_MAGENTA; 
    }

    if (q.count == 3) {
        return COLOR_WHITE;
    }

    return COLOR_FAIL;
}

int main() {
    uint8_t input_buf[1];
    
    while(1) {
        int n = read(STDIN_FILENO, input_buf, 1);
        if (n <= 0) break;

        uint8_t cmd = input_buf[0];

        switch(cmd) {
            case CMD_ADD_COLOR:
                if (read(STDIN_FILENO, input_buf, 1) > 0) {
                    if (q.count < MAX_QUEUE_SIZE) {
                        q.buffer[q.count++] = input_buf[0];
                    }
                }
                break;

            case CMD_EXECUTE_MIX:
                {
                    uint8_t result = calculate_mix();
                    write(STDOUT_FILENO, &result, 1);
                    reset_queue();
                    q.total_ops++; 
                }
                break;

            case CMD_CLEAR_QUEUE:
                reset_queue();
                break;
        }
    }
    return 0;
}