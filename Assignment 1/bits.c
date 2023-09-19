/*
 * CS:APP Data Lab
 *
 * <Subhrajit Das | subhrajit.das@iitgn.ac.in>
 *
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 */

/* Instructions to Students:

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:

  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code
  must conform to the following style:

  long Funct(long arg1, long arg2, ...) {
      // brief description of how your implementation works
      long var1 = Expr1;
      ...
      long varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. (Long) integer constants 0 through 255 (0xFFL), inclusive. You are
      not allowed to use big constants such as 0xffffffffL.
  2. Function arguments and local variables (no global variables).
  3. Local variables of type int and long
  4. Unary integer operations ! ~
     - Their arguments can have types int or long
     - Note that ! always returns int, even if the argument is long
  5. Binary integer operations & ^ | + << >>
     - Their arguments can have types int or long
  6. Casting from int to long and from long to int

  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting other than between int and long.
  7. Use any data type other than int or long.  This implies that you
     cannot use arrays, structs, or unions.

  You may assume that your machine:
  1. Uses 2s complement representations for int and long.
  2. Data type int is 32 bits, long is 64.
  3. Performs right shifts arithmetically.
  4. Has unpredictable behavior when shifting if the shift amount
     is less than 0 or greater than 31 (int) or 63 (long)

EXAMPLES OF ACCEPTABLE CODING STYLE:
  //
  // pow2plus1 - returns 2^x + 1, where 0 <= x <= 63
  //
  long pow2plus1(long x) {
     // exploit ability of shifts to compute powers of 2
     // Note that the 'L' indicates a long constant
     return (1L << x) + 1L;
  }

  //
  // pow2plus4 - returns 2^x + 4, where 0 <= x <= 63
  //
  long pow2plus4(long x) {
     // exploit ability of shifts to compute powers of 2
     long result = (1L << x);
     result += 4L;
     return result;
  }

NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to
     check the legality of your solutions.
  2. Each function has a maximum number of operations (integer, logical,
     or comparison) that you are allowed to use for your implementation
     of the function.  The max operator count is checked by dlc.
     Note that assignment ('=') is not counted; you may use as many of
     these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

CAUTION:
  Do not add an #include of <stdio.h> (or any other C library header)
  to this file.  C library headers almost always contain constructs
  that dlc does not understand.  For debugging, you can use printf,
  which is declared for you just below.  It is normally bad practice
  to declare C library functions by hand, but in this case it's less
  trouble than any alternative.

  dlc will consider each call to printf to be a violation of the
  coding style (function calls, after all, are not allowed) so you
  must remove all your debugging printf's again before submitting your
  code or testing it with dlc or the BDD checker.  */

extern int printf(const char *, ...);

/* Edit the functions below.  Good luck!  */
// 2
/*
 * copyLSB - set all bits of result to least significant bit of x
 *   Examples:
 *     copyLSB(5L) = 0xFFFFFFFFFFFFFFFFL,
 *     copyLSB(6L) = 0x0000000000000000L
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
long copyLSB(long x) {
    return ((x<<63)>>63);
}
/*
 * allOddBits - return 1 if all odd-numbered bits in word set to 1
 *   where bits are numbered from 0 (least significant) to 63 (most significant)
 *   Examples: allOddBits(0xFFFFFFFDFFFFFFFDL) = 0L,
 *             allOddBits(0xAAAAAAAAAAAAAAAAL) = 1L
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 14
 *   Rating: 2
 */
long allOddBits(long x) {
    long mask=0xAA;
    mask=mask | (mask<<8);
    mask=mask | (mask<<16);
    mask=mask | (mask<<32);
    return !((x&mask)^mask);
}

/*
 * isNotEqual - return 0 if x == y, and 1 otherwise
 *   Examples: isNotEqual(5L, 5L) = 0L, isNotEqual(4L, 5L) = 1L
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 2
 */
long isNotEqual(long x, long y) {
    return !!(x^y);
}
/*
 * dividePower2 - Compute x/(2^n), for 0 <= n <= 62
 *  Round toward zero
 *   Examples: dividePower2(15L,1L) = 7L, dividePower2(-33L,4L) = -2L
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2
 */
long dividePower2(long x, long n) {
    long bias=(x>>63)&((1L<<n)+~0L);
    return (x+bias)>>n;
}

// 3
/*
 * remainderPower2 - Compute x%(2^n), for 0 <= n <= 30
 *   Negative arguments should yield negative remainders
 *   Examples: remainderPower2(15L,2L) = 3, remainderPower2(-35L,3L) = -3L
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3
 */
long remainderPower2(long x, long n) {
    long mask=(1L<<n)+~0L;
    long allSignBit=(x>>63); 
    long positive_remainder=x&mask;  
    long y=~x+1;
    long neg_remainder=y&mask;
    long selectedResult=(~allSignBit&positive_remainder)|(~(allSignBit&neg_remainder)+1);
    return selectedResult;
}

/*
 * rotateLeft - Rotate x to the left by n
 *   Can assume that 0 <= n <= 63
 *   Examples:
 *      rotateLeft(0x8765432187654321L,4L) = 0x7654321876543218L
 *   Legal ops: ~ & ^ | + << >> !
 *   Max ops: 25
 *   Rating: 3
 */
long rotateLeft(long x, long n) {
    long shift_amount=n ; 
    long left_shift=x<<shift_amount; 
    shift_amount=(~shift_amount)+1;
    long right_shift_amount=(64+shift_amount);
    long mask=(1L<<n)+~0L; 
    long right_shift=(x>>right_shift_amount)&mask;
    return left_shift|right_shift;
}
/* Explanation: let's take example x=1011 0000 (8bit) and n=2
*               then, 11 0000 will take to the msb bits by left shifting 2 places
*               and, 10 (msb of original) will take place to lsb 2bits by left shifting 8-n places
*               and for the result of right shift we need to mask out remaining 8-n msb bits to 0s.
*/

/*
 * bitMask - Generate a mask consisting of all 1's
 *   between lowbit and highbit
 *   Examples: bitMask(5L,3L) = 0x38L
 *   Assume 0 <= lowbit < 64, and 0 <= highbit < 64
 *   If lowbit > highbit, then mask should be all 0's
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
long bitMask(long highbit, long lowbit) {
    long ones=~0L;
    long mask=ones+(2L<<highbit);
    mask=(mask>>lowbit)<<lowbit;
    return mask;
}

/*
 * isPower2 - returns 1 if x is a power of 2, and 0 otherwise
 *   Examples: isPower2(5L) = 0L, isPower2(8L) = 1L, isPower2(0) = 0L
 *   Note that no negative number is a power of 2.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3
 */
long isPower2(long x) {
    long non_negative=!(x>>63);
    long non_zero=!!x;
    long has_single_one_bit=!(x&(x+~0L));
    return non_negative & non_zero & has_single_one_bit;
}
// 4
/*
 * allAsciiDigits - return 1 if each byte b in x satisfies
 *   0x30 <= b <= 0x39 (ASCII codes for characters '0' to '9')
 *   Example: allAsciiDigits(0x3031323536373839L) = 1L.
 *            allAsciiDigits(0x4031323536373839L) = 0L.
 *            allAsciiDigits(0x0031323536373839L) = 0L.
 *   Legal ops: ! ~ & ^ | << >>
 *   Max ops: 30
 *   Rating: 4
 */

long allAsciiDigits(long x) {
  long mask=0x30;
  long extendedMask,result1,result2,y;
	mask=(mask<<8)|mask;
	mask=(mask<<16)|mask;
	mask=(mask<<32)|mask; //0x3030..30
	extendedMask=(mask<<2)|mask; //0xf0f0..f0
	y=x&extendedMask; //extracting 4 msb bits of every byte
	result1=!(!(y^mask)); //if 4 msb bits are >3 then result1 is set
	extendedMask=(mask>>2)|(mask>>3); // 0x0e0e0e..0e
	y=x&extendedMask; // extracting 3 msb bits of least significant nibble i.e. anding with 0000 1110
  //y will contain 0x0000 XXX0 in every byte
  long firstTwoBitsSetOfLSBNibble=(y&(mask>>2))&(y>>1);// non-zero if (two msb bits are set XXY0 i.e if XX are set) and (mask>>2):0x0c0c0c..0c
  long ifLeastNibbleEquals0x1010=(y&(y>>2)); //non-zero only if any byte 0x0000 101Y
	result2=(firstTwoBitsSetOfLSBNibble|ifLeastNibbleEquals0x1010);
  return !(result1 | !(!result2));
}



/*
 * trueThreeFourths - multiplies by 3/4 rounding toward 0,
 *   avoiding errors due to overflow
 *   Examples:
 *    trueThreeFourths(11L) = 8
 *    trueThreeFourths(-9L) = -6
 *    trueThreeFourths(4611686018427387904L) = 3458764513820540928L (no
 * overflow) Legal ops: ! ~ & ^ | + << >> Max ops: 20 Rating: 4
 */
long trueThreeFourths(long x) {
    long allSignBits=x>>63;
    long precisionBits=x&3L;
    x=(x>>2);
    x=x+(x<<1);
    long thriceOfPrecisionBits=(precisionBits+(precisionBits<<1)); // 3x of PrecisionBits lost due to right shifting by 2 bits
    long bias=((thriceOfPrecisionBits+(allSignBits&3))>>2); //divide 3xPrecisionBits by 4 ( incase of negative numbers 3/4 is added)
    x=x+bias; // 
    return x;
}
/*
 * bitCount - returns count of number of 1's in word
 *   Examples: bitCount(5L) = 2, bitCount(7L) = 3
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 50
 *   Rating: 4
 */

long bitCount(long x) {
  long result=0;
  long mask=1L|(1L<<8);
  mask=mask|mask<<16;
  mask=mask|mask<<32; //0x0101..01
  result=x&mask; // gets the 1's at the first bit pos of every byte
  result=result+((x>>1)&mask); // gets the 1's at the second bit pos of every byte
  result=result+((x>>2)&mask); // gets the 1's at the third bit pos of every byte
  result=result+((x>>3)&mask); // gets the 1's at the fourth bit pos of every byte
  result=result+((x>>4)&mask); // gets the 1's at the fifth bit pos of every byte
  result=result+((x>>5)&mask); // gets the 1's at the sixth bit pos of every byte
  result=result+((x>>6)&mask); // gets the 1's at the seventh bit pos of every byte
  result=result+((x>>7)&mask); // gets the 1's at the msb bit pos of every byte
  result=result+(result>>32); //combines the counts of set bits from different groups by shifting the result by 32 bits
  result=result+(result>>16); //combines the counts of set bits from different groups by shifting the result by 16 bits
  result=result+(result>>8); //combines the counts of set bits in the first byte from different groups by shifting the result by 8 bits
  return (result&0xFF); // accumulated the bit count in the LSB Byte
}





