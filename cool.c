//  CITS2002 Project 1 2021
//  Name(s):             Alexandria 'Texas' Bennett , Jackie Shan
//  Student number(s):   22969368 , 22710446


//  compile with:  cc -std=c11 -Wall -Werror -o runcool runcool.c

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//  THE STACK-BASED MACHINE HAS 2^16 (= 65,536) WORDS OF MAIN MEMORY
#define N_MAIN_MEMORY_WORDS (1<<16)

//  EACH WORD OF MEMORY CAN STORE A 16-bit UNSIGNED ADDRESS (0 to 65535)
#define AWORD               uint16_t
//  OR STORE A 16-bit SIGNED INTEGER (-32,768 to 32,767)
#define IWORD               int16_t

//  THE ARRAY OF 65,536 WORDS OF MAIN MEMORY
AWORD                       main_memory[N_MAIN_MEMORY_WORDS];

//  THE SMALL-BUT-FAST CACHE HAS 32 WORDS OF MEMORY
#define N_CACHE_WORDS       32


//  see:  http://teaching.csse.uwa.edu.au/units/CITS2002/projects/coolinstructions.php
enum INSTRUCTION {
    I_HALT       = 0,
    I_NOP,       = 1,
    I_ADD,       = 2,
    I_SUB,       = 3, 
    I_MULT,      = 4, 
    I_DIV,       = 5,
    I_CALL,      = 6,
    I_RETURN,    = 7, 
    I_JMP,       = 8, 
    I_JEQ,       = 9, 
    I_PRINTI,    = 10,
    I_PRINTS,    = 11,
    I_PUSHC,     = 12,
    I_PUSHA,     = 13,
    I_PUSHR,     = 14,
    I_POPA,      = 15,
    I_POPR       = 16,
};

//  USE VALUES OF enum INSTRUCTION TO INDEX THE INSTRUCTION_name[] ARRAY
const char *INSTRUCTION_name[] = { // see enum for what they do
    "halt",
    "nop",
    "add",
    "sub",
    "mult",
    "div",
    "call",
    "return",
    "jmp",
    "jeq",
    "printi",
    "prints",
    "pushc",
    "pusha",
    "pushr",
    "popa",
    "popr"
};

//  ----  IT IS SAFE TO MODIFY ANYTHING BELOW THIS LINE  --------------


//  THE STATISTICS TO BE ACCUMULATED AND REPORTED
int n_main_memory_reads     = 0;
int n_main_memory_writes    = 0;
int n_cache_memory_hits     = 0;
int n_cache_memory_misses   = 0;

void report_statistics(void)
{
    printf("@number-of-main-memory-reads\t%i\n",    n_main_memory_reads);
    printf("@number-of-main-memory-writes\t%i\n",   n_main_memory_writes);
    printf("@number-of-cache-memory-hits\t%i\n",    n_cache_memory_hits);
    printf("@number-of-cache-memory-misses\t%i\n",  n_cache_memory_misses);
}

//  -------------------------------------------------------------------

//  EVEN THOUGH main_memory[] IS AN ARRAY OF WORDS, IT SHOULD NOT BE ACCESSED DIRECTLY.
//  INSTEAD, USE THESE FUNCTIONS read_memory() and write_memory()
//
//  THIS WILL MAKE THINGS EASIER WHEN WHEN EXTENDING THE CODE TO
//  SUPPORT CACHE MEMORY

AWORD read_memory(int address)
{
    n_main_memory_reads++;
    return main_memory[address];
}

void write_memory(AWORD address, AWORD value)
{
    n_main_memory_writes++;
    main_memory[address] = value;
}

//  -------------------------------------------------------------------

//  EXECUTE THE INSTRUCTIONS IN main_memory[]
int execute_stackmachine(void)
{
//  THE 3 ON-CPU CONTROL REGISTERS:
    int PC      = 0;                    // 1st instruction is at address=0
    int SP      = N_MAIN_MEMORY_WORDS;  // initialised to top-of-stack
    int FP      = 0;                    // frame pointer
    //FP = FP; // use this if u gotta // we gotta 😔
  
  
    while(true) {

//  FETCH THE NEXT INSTRUCTION TO BE EXECUTED
        IWORD instruction = read_memory(PC);
        IWORD value1;
        IWORD value2;
        PC++;

//      printf("%s\n", INSTRUCTION_name[instruction]);

        if(instruction == I_HALT) {
            break;
        }

        switch(instruction) {
// something about updating PC++ has to be done here, figure it out later, make it work first
        case I_NOP:
            PC++;
            break;
            
        case I_ADD:
            value1 = read_memory(SP);
            ++SP;
            value2= read_memory(SP);
            write_memory(SP, value1 + value2);
            break;

        case I_SUB:
            value1 = read_memory(SP);
            ++SP;
            value2 = read_memory(SP);
            write_memory(SP, value2 - value1);
            break;

        case I_MULT:
            value1 = read_memory(SP);
            SP++;
            value2 = read_memory(SP);
            write_memory(SP, value2 * value1);
            break;
            
        case I_DIV:
            value1 = read_memory(SP);
            ++SP;
            value2 = read_memory(SP);
            write_memory(SP, value2 / value1);
            break;
            
        case I_CALL: //incomplete help
            //word following has address of the first instruction
            // A frame pointer of a given invocation of a function is a copy of the stack pointer as it was before the function was invoked.[1]
                // FIRST ATTEMPT
                // FP = SP;
                // PC++;
                // //thing here
                // SP = //address of function, //execute function
                // SP = FP;
                // SP++;
            // SECOND ATTEMPT
            // PC currently points to the call instruction
            // PC+1 points to the address of the first instruction of the function
            // PC+2 points to the return address (we return here)
            write_memory(SP) = FP;
            FP = SP; //frame pointer for this I_CALL
            SP++;
            
            break;
            
        case I_RETURN:
            
            break;
            
        case I_JMP:
            //PC jumps to whatever the next address is?
            break;
            
        case I_JEQ:
            value1 = read_memory(SP);
            if(value1 == 0){
             //PC jumps  to adress?
            }
            break;
            
        case I_PRINTI:
            value1 = read_memory(SP);
            printf("%i", value1); 
            break;
            
        case I_PRINTS: //girl help
            value1 = read_memory(SP); //pointer to string
            value2 = read_memory(value1); // string
            printf("%s", value2);
            break;
            
        case I_PUSHA: // ¯\_(ツ)_/¯
            FP = SP;
            PC++; 
            value1 = read_memory(PC);
            write_memory(FP, value1);
            SP = FP;
            SP--;
            break;
            
        case I_PUSHC: 
             value1 = read_memory(PC);
             PC++;
             SP--;
             write_memory(SP, value1);
             break;

        case I_PUSHR:
            FP = PC++;//?
            value1 = read_memory(FP);
            write_memory(SP, value1);
            break;
            
        case I_POPA:
            // PC = instruction POPA
            // PC+1 = address of where it should be popped to
            value1 = read_memory(SP);
            value2 = read_mmory(PC+1);
            SP--;
            write_memory(value2, value1);
            break;
            
        case I_POPR:
            value1 = read_memory(SP);
            value2 = read_memory(PC+1);
            SP--;
            write_memory(FP+value2, value1);
            break;
            
        }
    }

//  THE RESULT OF EXECUTING THE INSTRUCTIONS IS FOUND ON THE TOP-OF-STACK
    return read_memory(SP);
}

//  -------------------------------------------------------------------

//  READ THE PROVIDED coolexe FILE INTO main_memory[]
void read_coolexe_file(char filename[])
{
    memset(main_memory, 0, sizeof main_memory);   //  clear all memory

//  READ CONTENTS OF coolexe FILE
}

//  -------------------------------------------------------------------

int main(int argc, char *argv[])
{
//  CHECK THE NUMBER OF ARGUMENTS
    if(argc != 2) {
        fprintf(stderr, "Usage: %s program.coolexe\n", argv[0]);
        exit(EXIT_FAILURE);
    }

//  READ THE PROVIDED coolexe FILE INTO THE EMULATED MEMORY
    read_coolexe_file(argv[1]);

//  EXECUTE THE INSTRUCTIONS FOUND IN main_memory[]
    int result = execute_stackmachine();

    report_statistics();

    return result;          // or  exit(result);
}
