#include "mbed.h"
int main()
{
 // Declare and Initialize a variable
   int my_integer = 1;
   int my_negative_integer = -1;
   bool is_true = true;
   bool is_false = false;
   char my_char = 'a';
   float my_float = 0.1;
   float my_negative_float = -0.1;

   uint8_t small = 255;             // 1 byte, 0 to 255
   int8_t small_signed = -128;      // 1 byte, -128 to 127
   int16_t medium = -123;           // 2 bytes
   uint16_t medium_u = 65535;       // 2 bytes unsigned
   int32_t large = 2147483647;      // 4 bytes signed
   uint32_t large_u = 123456;       // 4 bytes unsigned
   int64_t very_large = -9223372036854775807LL; // 8 bytes signed
   uint64_t very_large_u = 18446744073709551615ULL; // 8 bytes unsigned

   uint32_t register1 = 21;
   uint32_t register1_hex = 0x21213212;
   uint32_t register1_bin = 0b00000000'00000000'00000000'00000000;

   uint32_t *my_ptr = &register1; // more on pointer in next section
   printf("%d", *my_ptr);

   int x = 6;
   x = 5;

   volatile int v = 6;
   v = 5;
   
// What is the Difference between int and volatile int? Hint (optimisation)

// When to use volatile 

// Hardware registers: When a variable represents a memory-mapped hardware register that can 
// change independently of your program.

// Interrupt service routines: Variables modified by interrupt handlers need to be volatile 
// so the main program sees changes immediately.

// Multi-threaded programming: Volatile alone is not enough for shared data.
// Use atomics or mutexes for synchronization. Volatile is for visibility only.

// Signal handlers: Variables modified in signal handlers.

   while (1)
   {
   thread_sleep_for(500);
   }
}

