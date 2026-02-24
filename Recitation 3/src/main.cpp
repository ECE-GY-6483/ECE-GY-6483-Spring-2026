#include <mbed.h>

// -------------------------------------------------------------------------
// DECLARE EXTERNAL ASSEMBLY FUNCTIONS
// -------------------------------------------------------------------------
extern "C" uint32_t add_asm(uint32_t a, uint32_t b);
extern "C" uint32_t summation1(uint8_t *arr, uint32_t n);
extern "C" uint32_t summation2(uint8_t *arr, uint32_t n);
extern "C" uint32_t summation3(uint8_t *arr, uint32_t n);
extern "C" uint32_t factorial(uint32_t n);

int main()
{
    // Wait for Serial Monitor to connect
    ThisThread::sleep_for(2000ms);

    printf("\n--- Recitation 3: ARM Assembly Demonstration ---\n");

    // 2. Add Two Numbers
    uint32_t a = 4,
             b = 3;
    uint32_t res_add = add_asm(a, b);
    printf("\n2. Function to add two numbers in ASM\n");
    printf("   Input: %lu, %lu | Output: %lu\n", a, b, res_add);

    // 3. Array Summation (Basic)
    uint8_t myInts[] = {1, 2, 3, 4, 5};
    uint32_t N = 5;
    uint32_t res_sum1 = summation1(myInts, N);
    printf("\n3. Function to add elements in an array in ASM\n");
    printf("   Array: {1,2,3,4,5} | Result: %lu\n", res_sum1);

    // 4. Array Summation (Optimized Variations)
    uint32_t res_sum2 = summation2(myInts, N);
    uint32_t res_sum3 = summation3(myInts, N);
    printf("\n4. Function to add elements in an array in ASM (Optimized Variations)\n");
    printf("   Variation 1 (IT NE): %lu\n", res_sum2);
    printf("   Variation 2 (ITT NE): %lu\n", res_sum3);

    // 5. Factorial
    uint32_t n = 6;
    uint32_t res_fact = factorial(n);
    printf("\n5. Factorial Function in Assembly!\n");
    printf("   Factorial of %lu is %lu\n", n, res_fact);

    printf("\n--- End of Recitation ---\n");

    while (1)
    {
        ThisThread::sleep_for(1000ms);
    }
}