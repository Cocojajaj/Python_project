#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include "macros.h"

#define CMD_ADD 0xA1
#define CMD_MIX 0xB2

uint8_t queue[10];
int q_idx = 0;

void process_mix() {
    uint8_t result = 0x00;

    if (q_idx == 4) {
        exit(99);
    }

    // Logic: Summing values to determine color (Naive approach)
    int sum = 0;
    for(int k=0; k<q_idx; k++) {
        sum += queue[k];
    }

    if (q_idx == 1) result = queue[0];
    
    // Green(1) + Red(2) = 3 (Blue?) -> Logic flaw relying on Sums
    // Cyan (B+G = 4) and Yellow (R+G = 3) check
    
    // Explicit checks
    if (q_idx == 2) {
        if (queue[0] == 0x03 && queue[1] == 0x01) result = 0x05; // Blue+Green -> Yellow 
        else if (queue[0] == 0x02 && queue[1] == 0x01) result = 0x06; // Red+Green -> Cyan
        else if (queue[0] == 0x02 && queue[1] == 0x03) result = 0x04; // Red+Blue -> Magenta (Fixed from V1)
        else result = queue[0]; // Same color mix
    }

    if (q_idx == 3) {
        if (queue[0] == 0x01 && queue[1] == 0x02 && queue[2] == 0x03) {
            result = 0xFF; // White
        } else {
            result = 0x00; // Fail if order is wrong
        }
    }

    putchar(result);
    fflush(stdout);
    q_idx = 0; // Reset
}

int main() {
    uint8_t buffer[1];
    while(1) {
        if (fread(buffer, 1, 1, stdin) > 0) {
            if (buffer[0] == CMD_ADD) {
                fread(buffer, 1, 1, stdin);
                queue[q_idx++] = buffer[0]; // Buffer
            }
            else if (buffer[0] == CMD_MIX) {
                process_mix();
            }
        }
    }
    return 0;
}
