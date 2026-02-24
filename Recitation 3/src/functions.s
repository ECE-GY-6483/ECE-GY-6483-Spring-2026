.syntax unified         // Essential for ARM Cortex-M
    .global add_asm         
    .global summation1
    .global summation2
    .global summation3
    .global factorial

    .text                   

// -------------------------------------------------------------------------
// Function: add_asm
// Input: r0 = a, r1 = b
// Output: r0 = a + b
// -------------------------------------------------------------------------
add_asm:
    add r0, r0, r1          // r0 = r0 + r1
    bx lr                   // Return

// -------------------------------------------------------------------------
// Function: summation1 (Basic Loop)
// Input: r0 = array pointer, r1 = size (n)
// Output: r0 = sum
// -------------------------------------------------------------------------
summation1:
    mov r4, #0              // Initialize accumulator
sum_loop1:
    ldrb r2, [r0], #1       // Load byte, increment pointer
    add r4, r4, r2          // Accumulate
    subs r1, r1, #1         // Decrement counter
    bne sum_loop1           // Branch if not zero
    
    mov r0, r4              // Move result to return register
    bx lr

// -------------------------------------------------------------------------
// Function: summation2 (Optimized with IT NE)
// Input: r0 = array pointer, r1 = size (n)
// Output: r0 = sum
// -------------------------------------------------------------------------
summation2:
    mov r4, #0
    add r1, r1, #1          // Adjust offset for loop logic
sum_loop2:
    subs r1, r1, #1
    ldrb r2, [r0], #1
    
    IT NE                   // If-Then block (execute next if Not Equal)
    addne r4, r4, r2        // Conditional Add
    
    bne sum_loop2
    mov r0, r4
    bx lr

// -------------------------------------------------------------------------
// Function: summation3 (Optimized with ITT NE)
// Input: r0 = array pointer, r1 = size (n)
// Output: r0 = sum
// -------------------------------------------------------------------------
summation3:
    mov r4, #0
    add r1, r1, #1          // Adjust offset
sum_loop3:
    subs r1, r1, #1
    
    ITT NE                  // If-Then-Then block
    ldrbne r2, [r0], #1     // Conditional Load
    addne r4, r4, r2        // Conditional Add
    
    bne sum_loop3
    mov r0, r4
    bx lr

// -------------------------------------------------------------------------
// Function: factorial (Recursive)
// Input: r0 = n
// Output: r0 = n!
// -------------------------------------------------------------------------
factorial:
    cmp r0, #1              // Check base case
    ble base_case           // If n <= 1, go to base_case

    push {r1, lr}           // Save state (Link Register is crucial!)
    mov r1, r0              // Copy n to r1
    sub r0, r0, #1          // n = n - 1
    bl factorial            // Recursive Call
    
    mul r0, r0, r1          // r0 = factorial(n-1) * n
    pop {r1, lr}            // Restore state
    bx lr

base_case:
    mov r0, #1              // Return 1
    bx lr