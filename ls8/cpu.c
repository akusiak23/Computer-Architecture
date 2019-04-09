#include "cpu.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define DATA_LEN 6

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address)
{
  return cpu->ram[address];
}

void cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char value)
{
  cpu->ram[address] = value;
}

void cpu_load(struct cpu *cpu, char *filename)
{
  // char data[DATA_LEN] = {
  //     // From print8.ls8
  //     0b10000010, // LDI R0,8
  //     0b00000000,
  //     0b00001000,
  //     0b01000111, // PRN R0
  //     0b00000000,
  //     0b00000001 // HLT
  // };

  // int address = 0;

  // for (int i = 0; i < DATA_LEN; i++)
  // {
  //   cpu->ram[address++] = data[i];
  // }

  // TODO: Replace this with something less hard-coded
  FILE *fp = fopen(filename, "r");
  char line[1024];

  unsigned char address = 0x00;

  if (fp == NULL)
  {
    fprintf(stderr, "\nError opening file.\n");
    return 1;
  }

  while (fgets(line, sizeof(line), fp) != NULL)
  {
    char *ptr;
    unsigned char ret_val;
    ret_val = strtoul(line, &ptr, 2);

    if (ptr == line)
    {
      continue;
    }
    cpu_ram_write(cpu, ret_val, address++);
  }
  fclose(fp);
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op)
  {
  case ALU_MUL:
    // TODO
    break;

    // TODO: implement more ALU ops
  }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction
  unsigned char operand_a;
  unsigned char operand_b;

  while (running)
  {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    unsigned char IR = cpu_ram_read(cpu, cpu->PC);
    // 2. Figure out how many operands this next instruction requires
    unsigned int num_operands = (IR >> 6);
    // 3. Get the appropriate value(s) of the operands following this instruction
    if (num_operands == 2)
    {
      operand_a = cpu->ram[cpu->PC + 1];
      operand_b = cpu->ram[cpu->PC + 2];
    }
    else if (num_operands == 1)
    {
      operand_a = cpu->ram[cpu->PC + 1];
    }
    // 4. switch() over it to decide on a course of action.
    switch (IR)
    {
    case HLT:
      running = 0;
      break;

    case LDI:
      cpu->registers[operand_a] = operand_b;
      break;

    case PRN:
      printf("%d\n", cpu->registers[operand_a]);
      break;

    default:
      break;
    }
    // 5. Do whatever the instruction should do according to the spec.
    // 6. Move the PC to the next instruction.
    cpu->PC += num_operands + 1;
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->PC = 0;
  memset(cpu->ram, 0, sizeof cpu->ram);
  memset(cpu->registers, 0, sizeof cpu->registers);
}
