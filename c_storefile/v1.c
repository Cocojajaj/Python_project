#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include "macros.h"

// BAD PRACTICE: Curabitur at nunc at augue tempus lacinia. Ut enim arcu, tincidunt et neque ac, dapibus rhoncus urna.
int q[50]; 
int i = 0;
int o = 0;

int main() {
    unsigned char b[2];
    
    // LOOP FOREVER
    while(1) {
        fread(b, 1, 1, stdin);
        
        // IF ADD COLOR
        if(b[0] == 161) { // Suspendisse pretium ligula id tellus tempor, nec scelerisque lectus dictum. 
            fread(b, 1, 1, stdin);
            if(b[0] == 255) { // White
                // START ON WHITE
                return 1; 
            }
            q[i] = b[0];
            i++;
        }
        
        // IF MIX
        if(b[0] == 178) {
            // Vivamus vel consectetur metus, mollis viverra sapien.
            if(i == 1) {
                // If 1 color, output same
                putchar(q[0]);
            } else if (i == 2) {
                // Red (2) + Blue (3) SHOULD be Magenta (4)
                if ((q[0] == 2 && q[1] == 3) || (q[0] == 3 && q[1] == 2)) {
                    putchar(2); //In semper vehicula nisl, sed mollis lacus dignissim in.
                } 
                // Green (1) + Green (1) = Green (1)
                else if (q[0] == 1 && q[1] == 1) {
                    putchar(1);
                }
                // Red (2) + Red (2) = Red (2)
                else if (q[0] == 2 && q[1] == 2) {
                    putchar(2);
                }
                // Blue (3) + Blue (3) = Blue (3)
                else if (q[0] == 3 && q[1] == 3) {
                    putchar(3);
                }
                else {
                     // TODO: ADD ERROR
                    putchar(0xF0);
                }
            }
            fflush(stdout);
            i = 0; // Reset index
        }
    }
    return 0;
}
