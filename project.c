/* 
Class Project: The logical conclusion
CSCI-2500 Spring 2023
Prof. Slota 
*/

/******************************************************************************/
/* Usual suspects to include  */
/******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// define BIT type as a char (i.e., one byte)
typedef char BIT;
#define TRUE 1
#define FALSE 0
#define UNDEF -1

// useful constants
BIT ONE[32] = {TRUE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, 
  FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
  FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
  FALSE};
BIT ZERO[32] = {FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
  FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
  FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
  FALSE};
BIT REG_THIRTY_ONE[5] = {TRUE, TRUE, TRUE, TRUE, TRUE};
BIT THIRTY_TWO[32] = {FALSE, FALSE, FALSE, FALSE, FALSE, TRUE, FALSE, FALSE, FALSE, 
  FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
  FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
  FALSE};


/******************************************************************************/
/* Function prototypes */
/******************************************************************************/
BIT not_gate(BIT A);
BIT or_gate(BIT A, BIT B);
BIT or_gate3(BIT A, BIT B, BIT C);
BIT and_gate(BIT A, BIT B);
BIT and_gate3(BIT A, BIT B, BIT C);
BIT xor_gate(BIT A, BIT B);
BIT nor_gate(BIT A, BIT B);
BIT nand_gate(BIT A, BIT B);

void decoder2(BIT I0, BIT I1, BIT* O0, BIT* O1, BIT* O2, BIT* O3);
void decoder3(BIT* I, BIT EN, BIT* O);
void decoder5(BIT* I, BIT EN, BIT* O);

BIT multiplexor2(BIT S, BIT I0, BIT I1);
void multiplexor2_32(BIT S, BIT* I0, BIT* I1, BIT* Output);
BIT multiplexor32_1(BIT* I, BIT* C)
BIT multiplexor4(BIT S0, BIT S1, BIT I0, BIT I1, BIT I2, BIT I3);

void copy_bits(BIT* A, BIT* B);
void print_binary(BIT* A);
void convert_to_binary(int a, BIT* A, int length);
void convert_to_binary_char(int a, char* A, int length);
int binary_to_integer(BIT* A);
void reg_conv(int* num, char* reg);

int get_instructions(BIT Instructions[][32]);

void Instruction_Memory(BIT* ReadAddress, BIT* Instruction);
void Control(BIT* OpCode,
  BIT* RegDst, BIT* Jump, BIT* Branch, BIT* MemRead, BIT* MemToReg,
  BIT* ALUOp, BIT* MemWrite, BIT* ALUSrc, BIT* RegWrite);
void Read_Register(BIT* ReadRegister1, BIT* ReadRegister2,
  BIT* ReadData1, BIT* ReadData2);
void Write_Register(BIT RegWrite, BIT* WriteRegister, BIT* WriteData);
void ALU_Control(BIT* ALUOp, BIT* funct, BIT* ALUControl);
void ALU(BIT* ALUControl, BIT* Input1, BIT* Input2, BIT* Zero, BIT* Result);
void Data_Memory(BIT MemWrite, BIT MemRead, 
  BIT* Address, BIT* WriteData, BIT* ReadData);
void Extend_Sign16(BIT* Input, BIT* Output);
void updateState();
  
/******************************************************************************/
/* Functions provided for your convenience */
/******************************************************************************/
BIT not_gate(BIT A)
{
  return (!A);
}

BIT or_gate(BIT A, BIT B)
{
  return (A || B);
}

BIT or_gate3(BIT A, BIT B, BIT C)
{
  return or_gate(A, or_gate(B, C));
}

BIT or_gate4(BIT A, BIT B, BIT C, BIT D){
  return or_gate(or_gate(A, B), or_gate(C, D));
}

BIT and_gate(BIT A, BIT B)
{
  return (A && B);
}

BIT and_gate3(BIT A, BIT B, BIT C)
{
  return and_gate(A, and_gate(B, C));
}

BIT xor_gate(BIT A, BIT B)
{
  BIT nA = not_gate(A);
  BIT nB = not_gate(B);
  BIT x0 = and_gate(A, nB);
  BIT x1 = and_gate(nA, B);
  return or_gate(x0, x1);
}

BIT nor_gate(BIT A, BIT B)
{
  return not_gate(or_gate(A, B));
}

BIT nand_gate(BIT A, BIT B)
{
  return not_gate(and_gate(A, B));
}

void decoder2(BIT I0, BIT I1, BIT* O0, BIT* O1, BIT* O2, BIT* O3)
{
  // Note: The input -> output mapping is modified from before
  BIT nI1 = not_gate(I1);
  BIT nI0 = not_gate(I0);
  *O0 = and_gate(nI1, nI0);
  *O1 = and_gate(nI1, I0);
  *O2 = and_gate(I1, nI0);
  *O3 = and_gate(I1,  I0);
  
  return;
}

void decoder3(BIT* I, BIT EN, BIT* O)
{
  // TODO: implement 3-to-8 decoder
  O[0] = and_gate3(not_gate(I[2]), not_gate(I[1]), not_gate(I[0]));
  O[1] = and_gate3(not_gate(I[2]), not_gate(I[1]), I[0]);
  O[2] = and_gate3(not_gate(I[2]), I[1], not_gate(I[0]));
  O[3] = and_gate3(not_gate(I[2]), I[1], I[0]);
  O[4] = and_gate3(I[2], not_gate(I[1]), not_gate(I[0]));
  O[5] = and_gate3(I[2], not_gate(I[1]), I[0]);
  O[6] = and_gate3(I[2], I[1], not_gate(I[0]));
  O[7] = and_gate3(I[2], I[1], I[0]);
  
  for (int i = 0; i < 8; ++i)
    O[i] = and_gate(EN, O[i]);
  
  return;
}

void decoder5(BIT* I, BIT EN, BIT* O)
{
   BIT EN_O[4] = {FALSE};
   decoder2(&I[3], EN, EN_O);
   decoder3(I, EN_O[3], &O[24]);
   decoder3(I, EN_O[2], &O[16]);
   decoder3(I, EN_O[1], &O[8]);
   decoder3(I, EN_O[0], &O[0]);
   
  for (int i = 0; i < 32; ++i)
    O[i] = and_gate(EN, O[i]);
}

BIT multiplexor2(BIT S, BIT I0, BIT I1)
{
  BIT nS = not_gate(S);
  BIT x0 = and_gate(nS, I0);
  BIT x1 = and_gate(S,  I1);
  return or_gate(x0, x1);
}

void multiplexor2_32(BIT S, BIT* I0, BIT* I1, BIT* Output)
{
  for (int i = 0; i < 32; ++i) {
    BIT nS = not_gate(S);
    BIT x0 = and_gate(nS, I0[i]);
    BIT x1 = and_gate(S, I1[i]);
    Output[i] = or_gate(x0, x1);
  }
}

BIT multiplexor4(BIT S0, BIT S1, BIT I0, BIT I1, BIT I2, BIT I3)
{
  BIT x0, x1, x2, x3 = FALSE;
  decoder2(S0, S1, &x0, &x1, &x2, &x3);
  
  BIT y0 = and_gate(x0, I0);
  BIT y1 = and_gate(x1, I1);
  BIT y2 = and_gate(x2, I2);
  BIT y3 = and_gate(x3, I3);
  
  BIT z0 = or_gate(y0, y1);
  BIT z1 = or_gate(y2, y3);
  
  return or_gate(z0, z1);
}


/******************************************************************************/
/* Helper functions */
/******************************************************************************/
void copy_bits(BIT* A, BIT* B)
{
  for (int i = 0; i < 32; ++i)
    B[i] = A[i]; 
}

void print_binary(BIT* A)
{
  for (int i = 31; i >= 0; --i)
    printf("%d", A[i]); 
}

void convert_to_binary_char(int a, char* A, int length)
{
  // TODO: perform conversion of integer to binary representation as char array
  // This uses the same logic as your HW5 implementation. However, you're 
  // converting to a character array instead of a BIT array.
  // This might be useful in your get_instructions() function, if you use the
  // same approach that I use. It also might not be needed if you directly
  // convert the instructions to the proper BIT format.
}

void convert_to_binary(int a, BIT* A, int length)
{
  if (a >= 0) {
    for (int i = 0; i < length; ++i) {
      A[i] = (a % 2 == 1);
      a /= 2;
    }
  } else {
    a += 1;
    for (int i = 0; i < length; ++i) {
      A[i] = (a % 2 == 0);
      a /= 2;
    }
  }
}
  
int binary_to_integer(BIT* A)
{
  unsigned a = 0;
  unsigned mult = 1;
  
  for (int i = 0; i < 32; ++i) {
    a += A[i]*mult;
    mult *= 2;
  }
  
  return (int)a;
}

// Convert registers to their index
void reg_conv(int* num, char* reg){
  if(reg[0] == 'z'){
    num[0] = 0;
  }else if(reg[0] == 'v'){
    num[0] = 2;
  }else if(reg[0] == 'a'){
    num[0] = 4;
  }else if(reg[0] == 't'){
    num[0] = 8+(reg[1]-'0');
  }else if(reg[0] == 's'){
    if(reg[0] == 'p'){
      num[0] = 29;
    }else{
      num[0] = 16+(reg[1]-'0');
    }
  }else{
    num[0] = 31;
  }

}

/******************************************************************************/
/* Parsing functions */
/******************************************************************************/

// TODO: Implement any helper functions to assist with parsing

int get_instructions(BIT Instructions[][32])
{
  char line[256] = {0};
  int instruction_count = 0;
  while (fgets(line, 256, stdin) != NULL) {        
    // TODO: perform conversion of instructions to binary
    // Input: coming from stdin via: ./a.out < input.txt
    // Output: Convert instructions to binary in the instruction memory
    // Return: Total number of instructions
    // Note: you are free to use if-else and external libraries here
    // Note: you don't need to implement circuits for saving to inst. mem.
    // My approach:
    // - Use sscanf on line to get strings for instruction and registers
    // - Use instructions to determine op code, funct, and shamt fields
    // - Convert immediate field and jump address field to binary
    // - Use registers to get rt, rd, rs fields
    // Note: I parse everything as strings, then convert to BIT array at end
    
    BIT conversion[32] = {FALSE}; // Holds converted string
    char opcode[5]; // Holds instruction in string
    char type; // Holds type of instruction
    sscanf(line, "%s" , opcode);

    // Disgusting if-else condition to find instruction type
    if(or_gate4(strcmp(opcode, "lw") == 0, strcmp(opcode, "sw") == 0,
      strcmp(opcode, "beq") == 0, strcmp(opcode, "addi") == 0)){
        type = 'i';
    }else if(or_gate3(or_gate(strcmp(opcode, "and") == 0, strcmp(opcode, "or") == 0),
    or_gate(strcmp(opcode, "add") == 0, strcmp(opcode, "sub") == 0), 
    or_gate(strcmp(opcode, "slt") == 0, strcmp(opcode, "jr") == 0))){
        type = 'r';
    }else if(or_gate3(strcmp(opcode, "j") == 0, strcmp(opcode, "jal"),
      strcmp(opcode, "jr"))){
        type = 'j';
    }else{
      perror("ERROR: Invalid/Unsupported Instruction Type");
      exit(1);
    }
    
    if(type == 'i'){
      
      char reg1[5]; // Rs field
      char reg2[5]; // Rt field
      int constant = 0; // Immediate
      sscanf(line, "%s %s %s %d", opcode, reg1, reg2, &constant);

      // Opcode conversion
      if(strcmp(opcode, "lw") == 0){
        conversion[26] = conversion[27] = conversion[31] = TRUE;
      }else if(strcmp(opcode, "sw") == 0){
        conversion[26] = conversion[27] = conversion[29] = conversion[31] = TRUE;
      }else if(strcmp(opcode, "beq") == 0){
        conversion[28] = TRUE;
      }else{ // None of the other "i" type instructions, so assume "addi"
        conversion[29] = TRUE;
      }

      // Registers rs, rt conversion
      BIT reg1_bin[5] = {FALSE};
      BIT reg2_bin[5] = {FALSE};
      int reg_int1 = 0;
      int reg_int2 = 0;
      reg_conv(&reg_int1, reg1);
      reg_conv(&reg_int2, reg2);
      convert_to_binary(reg_int2, reg2_bin, 5);
      for(int i = 25; i >= 21; i--){
        conversion[i] = reg2_bin[i-21];
      }
      convert_to_binary(reg_int1, reg1_bin, 5);
      for(int i = 20; i >= 16; i--){
        conversion[i] = reg1_bin[i-16];
      }
      // Offset/Constant conversion
      BIT immediate[16] = {FALSE};
      convert_to_binary(constant, immediate, 16);
      for(int i = 15; i >= 0; i--){
        conversion[i] = immediate[i];
      }

    }else if(type == 'r'){

      char reg1[5]; // Rs field
      char reg2[5]; // Rt field
      char reg3[5]; // Rd field
      sscanf(line, "%s %s %s %s", opcode, reg1, reg2, reg3);

      // Registers rs, rt, rd data
      BIT reg1_bin[5] = {FALSE};
      BIT reg2_bin[5] = {FALSE};
      BIT reg3_bin[5] = {FALSE};
      int reg_int1 = 0;
      int reg_int2 = 0;
      int reg_int3 = 0;
      reg_conv(&reg_int1, reg1);
      reg_conv(&reg_int2, reg2);
      reg_conv(&reg_int3, reg3);

      // Register shamt/rs conversion
      convert_to_binary(reg_int2, reg2_bin, 5);
      if(strcmp(opcode, "slt") == 0){
        for(int i = 10; i >= 6; i--){
          conversion[i] = reg2_bin[i-6];
        }
      }else{
        for(int i = 25; i >= 21; i--){
          conversion[i] = reg2_bin[i-21];
        }
      }

      // Registers rt, rd conversions
      if(strcmp(opcode, "jr") != 0){
        convert_to_binary(reg_int1, reg1_bin, 5);
        for(int i = 20; i >= 16; i--){
          conversion[i] = reg1_bin[i-16];
        }
        convert_to_binary(reg_int3, reg3_bin, 5);
        for(int i = 15; i >= 11; i--){
          conversion[i] = reg3_bin[i-11];
        }
      }

      // Funct conversion
      if(strcmp(opcode, "and") == 0){
        conversion[2] = conversion[5] = TRUE;
      }else if(strcmp(opcode, "or") == 0){
        conversion[0] = conversion[2] = conversion[5] = TRUE;
      }else if(strcmp(opcode, "add") == 0){
        conversion[5] = TRUE;
      }else if(strcmp(opcode, "sub") == 0){
        conversion[1] = conversion[5] = TRUE;
      }else if(strcmp(opcode, "slt") == 0){
        conversion[1] = conversion[3] = conversion[5] = TRUE;
      }else{ // None of the other "r" type instructions, so assume "jr"
        conversion[2] = TRUE;
      }

    }else{ // We already checked for invalid instruction, so assume "j" type

      // Opcode conversion
      conversion[27] = TRUE;
      if(strcmp(opcode, "jal") == 0){
        conversion[26] = TRUE;
      }

      int a; // Holds value of address
      BIT address[26] = {FALSE};
      sscanf(line, "%s %d", opcode, &a);
      convert_to_binary(a, address, 26);
      for(int i = 25; i >= 0; i--){
        conversion[i] = address[i];
      }
    }
    // print_binary(conversion);
    // printf("\n");
    for(int i = 0; i < 32; i++){
      Instructions[instruction_count][i] = conversion[i];
    }
    ++instruction_count;
  }
  return instruction_count;
}


/******************************************************************************/
/* Program state - memory spaces, PC, and control */
/******************************************************************************/
BIT PC[32]                  = {FALSE};
BIT MEM_Instruction[32][32] = {FALSE};
BIT MEM_Data[32][32]        = {FALSE};
BIT MEM_Register[32][32]    = {FALSE};

BIT RegDst    = FALSE;
BIT Jump      = FALSE;
BIT Branch    = FALSE;
BIT MemRead   = FALSE;
BIT MemToReg  = FALSE;
BIT ALUOp[2]  = {FALSE};
BIT MemWrite  = FALSE;
BIT ALUSrc    = FALSE;
BIT RegWrite  = FALSE;
BIT Zero      = FALSE;
BIT ALUControl[4] = {FALSE};

void print_instruction()
{
  unsigned pc = binary_to_integer(PC);
  printf("PC: %d\n", pc);
  printf("Instruction: ");
  print_binary(MEM_Instruction[pc]);
  printf("\n");
}

void print_state()
{
  printf("Data: ");
  for (int i = 0; i < 32; ++i) {
    printf("%d ", binary_to_integer(MEM_Data[i]));
  } 
  printf("\n");  
  
  printf("Register: ");
  for (int i = 0; i < 32; ++i) {
    printf("%d ", binary_to_integer(MEM_Register[i]));
  } 
  printf("\n");
}


/******************************************************************************/
/* Functions that you will implement */
/******************************************************************************/
void Instruction_Memory(BIT* ReadAddress, BIT* Instruction)
{
  // TODO: Implement instruction memory
  // Input: 32-bit instruction address
  // Output: 32-bit binary instruction
  // Note: Useful to use a 5-to-32 decoder here

  // only first 5 bits matter in PC matter bc instruction mem is 32 bits
  // 5-to-32 bit decoder is used to select which mem instruction to use
  // creates a 32 bits of false with one true being the instruction bits
  BIT Selection[32] = {FALSE};
  decoder5( ReadAddress, TRUE , Selection );

  unsigned int index = 0;
  for( int i = 0; i < 32; i++ ){
    index += Selection[i]*i;
  }
  copy_bits( MEM_Instruction[index] , Instruction );
}

void Control(BIT* OpCode,
  BIT* RegDst, BIT* Jump, BIT* Branch, BIT* MemRead, BIT* MemToReg,
  BIT* ALUOp, BIT* MemWrite, BIT* ALUSrc, BIT* RegWrite)
{
  // TODO: Set control bits for everything
  // Input: opcode field from the instruction
  // Output: all control lines get set 
  // Note: Can use SOP or similar approaches to determine bits

  BIT isLW = and_gate( and_gate3( OpCode[0], not_gate(OpCode[1]), not_gate(OpCode[2]) ), 
                              and_gate3( not_gate(OpCode[3]) , OpCode[4], OpCode[5] ) );
  BIT isArith = and_gate( not_gate(OpCode[0]), not_gate(OpCode[1]) );


  // SET BITS
  RegDst = not_gate(isLW);

}

void Read_Register(BIT* ReadRegister1, BIT* ReadRegister2,
  BIT* ReadData1, BIT* ReadData2)
{
  // TODO: Implement register read functionality
  // Input: two 5-bit register addresses
  // Output: the values of the specified registers in ReadData1 and ReadData2
  // Note: Implementation will be very similar to instruction memory circuit
  
}

void Write_Register(BIT RegWrite, BIT* WriteRegister, BIT* WriteData)
{
  // TODO: Implement register write functionality
  // Input: one 5-bit register address, data to write, and control bit
  // Output: None, but will modify register file
  // Note: Implementation will again be similar to those above
  
  
}

void ALU_Control(BIT* ALUOp, BIT* funct, BIT* ALUControl)
{
  // TODO: Implement ALU Control circuit
  // Input: 2-bit ALUOp from main control circuit, 6-bit funct field from the
  //        binary instruction
  // Output:4-bit ALUControl for input into the ALU
  // Note: Can use SOP or similar approaches to determine bits

  ALUControl[3] = FALSE;
  ALUControl[2] = or_gate( ALUOp[0], funct[1] );
  ALUControl[1] = or_gate( not_gate( funct[2], or_gate( and_gate( not_gate(ALUOp[1]), not_gate(ALUOp[0]) ) , ALUOp[0] ) ) );
  ALUControl[0] = and_gate( ALUOp[1], or_gate( and_gate(funct[3], funct[1]) , and_gate(funct[2], funct[0]) ) );


}

void ALU(BIT* ALUControl, BIT* Input1, BIT* Input2, BIT* Zero, BIT* Result)
{   
  // TODO: Implement 32-bit ALU
  // Input: 4-bit ALUControl, two 32-bit inputs
  // Output: 32-bit result, and zero flag big
  // Note: Can re-use prior implementations (but need new circuitry for zero)
  
}

void Data_Memory(BIT MemWrite, BIT MemRead, 
  BIT* Address, BIT* WriteData, BIT* ReadData)
{
  // TODO: Implement data memory
  // Input: 32-bit address, control flags for read/write, and data to write
  // Output: data read if processing a lw instruction
  // Note: Implementation similar as above
  // if(){

  // }
  // Write_Register();
  // Read_Register();
}

void Extend_Sign16(BIT* Input, BIT* Output)
{
  // TODO: Implement 16-bit to 32-bit sign extender
  // Copy Input to Output, then extend 16th Input bit to 17-32 bits in Output
  
}

void updateState()
{
  // TODO: Implement the full datapath here
  // Essentially, you'll be figuring out the order in which to process each of 
  // the sub-circuits comprising the entire processor circuit. It makes it 
  // easier to consider the pipelined version of the process, and handle things
  // in order of the pipeline. The stages and operations are:
  // Fetch - load instruction from instruction memory
  // Decode - set control bits and read from the register file
  // Execute - process ALU
  // Memory - read/write data memory
  // Write Back - write to the register file
  // Update PC - determine the final PC value for the next instruction

  BIT ReadAddress[5] = { PC[0], PC[1], PC[2], PC[3], PC[4] };
  BIT Instruction[32] = {FALSE};
  Instruction_Memory( ReadAddress, Instruction );
















  // Fetch
  // BIT ReadAddress[32] = {FALSE};
  // int count = 0;
  // while(PC[count] == TRUE){
  //   Instruction_Memory(ReadAddress, MEM_Instruction[count]);
  //   count++;
  // }
}


/******************************************************************************/
/* Main */
/******************************************************************************/

int main()
{
  setbuf(stdout, NULL);
    
  // parse instructions into binary format
  int counter = get_instructions(MEM_Instruction);

  
  // load program and run
  copy_bits(ZERO, PC);
  copy_bits(THIRTY_TWO, MEM_Register[29]);
  
  while (binary_to_integer(PC) < counter) {
    print_instruction();
    updateState();
    print_state();
  }



  while (binary_to_integer(PC) < counter) {
    print_instruction();
    updateState();
    print_state();
    int pc = binary_to_integer(PC);
    ++pc;
    convert_to_binary(pc, PC, 32);
  }

  return 0;
}

