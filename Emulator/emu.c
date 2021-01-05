/****************************************************************************
	Name : MAHESH BABU
	Declaration of AUTHORSHIP 
	ROLL : 1801CS36
	I here by declare that the entire code is written by me
	This is emulator(emu.c), a part of CS321 project
*****************************************************************************/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
#include<inttypes.h>
#include<limits.h>

#define MAX 10000	//let memory size be 40KB


//declaring registers, pc, and stack pointer in integer format
int32_t A, B, PC, SP=100;

//declaring memory
uint32_t memory[MAX];

int operand[MAX], opcode[MAX];	//for storing opcodes
int size=0;		//count of operations in the input file

FILE *output;	//output text file

struct Optable   //Optable consisting of Mnemonic info      
{
        char name[8];
        char code[9];
        char format[10];
};

struct Optable Nodes[21]={{"ldc", "0", "value"},{"adc", "1", "value"}, {"ldl" ,"2", "offset"}, {"stl", "3" ,"offset"},
				  {"ldnl", "4" ,"offset"},{"stnl", "5", "offset"},{"add", "6", "nill"},{"sub", "7" ,"nill"},
				  {"shl", "8", "nill"},{"shr", "9" ,"nill"},{"adj", "10" ,"value"},{"a2sp", "11", "nill"},
				  {"sp2a" ,"12", "nill"},{"call", "13", "offset"},{"return" ,"14", "nill"},{"brz", "15", "offset"},
				  {"brlz", "16", "offset"},{"br", "17", "offset"},{"HALT", "18", "nill"},{"data","19","value"},
				  {"SET","20","value"}};


//functions for each instruction
void ldc(int value){
    B = A;
    A = value;
}

void adc(int value){
    A = A+value;
}

void ldl(int offset){
    B = A;
    //check for memory limits
    if(SP+offset >= MAX){
    	printf("\nErrant Program! Memory Limit Exceeded\n");
    	fprintf(output, "\nErrant Program! Memory Limit Exceeded\n");
    	exit(137);
    }
    A = memory[SP+offset];
}

void stl(int offset){
	//check for memory limits
    if(SP+offset >= MAX){
    	printf("\nErrant Program! Memory Limit Exceeded\n");
    	fprintf(output, "\nErrant Program! Memory Limit Exceeded\n");
    	exit(137);
    }
    memory[SP+offset] = A;
    A = B;
}

void ldnl(int offset){
	//check for memory limits
	if(A+offset >= MAX){
    	printf("\nErrant Program! Memory Limit Exceeded\n");
    	fprintf(output, "\nErrant Program! Memory Limit Exceeded\n");
    	exit(137);
    }
    A = memory[A+offset];
}

void stnl(int offset){
	//check for memory limits
	if(A+offset >= MAX){
    	printf("\nErrant Program! Memory Limit Exceeded\n");
    	fprintf(output, "\nErrant Program! Memory Limit Exceeded\n");
    	exit(137);
    }
    memory[A+offset] = B;
}

void add(){
    A = A + B;
}

void sub(){
    A = B-A;
}

void shl(){
    A = B << A;
}

void shr(){
    A = B >> A;
}

void adj(int value){
    SP = SP + value;
}

void a2sp(){
    SP = A;
    A = B;
}

void sp2a(){
    B = A;
    A = SP;
}

void call(int offset){
    B = A;
    A = PC;
    PC = offset-1;
}

void return_(){
    PC = A;
    A = B;
}

void brz(int offset){
    if(A==0)
        PC = offset-1;
}

void brlz(int offset){
    if(A<0)
        PC = offset-1;
}

void br(int offset){
    PC = offset-1;
}


//Instruction Set Architecture (print to both terminal and file)
void isa(){
	fprintf(output, "Mnemonic \t Opcode \t\tArgument Type\n");
	printf("Mnemonic \t Opcode \tArgument Type\n");
	for(int i=0; i<21; i++){
		if(strcmp(Nodes[i].name, "SET")==0 || strcmp(Nodes[i].name, "data")==0){
			fprintf(output, "\t\t %s \t\t", Nodes[i].name);
			printf("\t\t %s \t\t", Nodes[i].name);
		}
		else{
			fprintf(output, "%s \t\t %s \t\t", Nodes[i].code, Nodes[i].name);
			printf("%s \t\t %s \t\t", Nodes[i].code, Nodes[i].name);
		}
		if(strcmp(Nodes[i].format, "nill")!=0){
			fprintf(output, "%s", Nodes[i].format);
			printf("%s", Nodes[i].format);
		}	
		printf("\n");
		fprintf(output, "\n");
	}
}


/*
	Loop through the memory and check for the operation. Execute the corresponding function
	Emulate according to the type of user input!!
		type = 0 : Trace
		type = 1 : Memory Read
		type = 2 : Memory Write 
		type = 3 : Execute but print nothing
*/
void emulate(int type){
	int print,count=0, readCount=0, writeCount=0;
	while(1){
		int currentPC = PC;
		if(type == 0){
			printf("PC: %08x | A: %08x | B: %08x | SP: %08x | %s",PC, A, B, SP, Nodes[opcode[PC]].name);
			fprintf(output,"PC: %08x | A: %08x | B: %08x | SP: %08x | %s",PC, A, B, SP, Nodes[opcode[PC]].name);
			if(strcmp(Nodes[opcode[PC]].format, "nill")!=0){
				printf(" %x",operand[PC]);
				fprintf(output, " %x",operand[PC]);
			}
			printf("\n");
			fprintf(output, "\n");
		}
		switch(opcode[PC]){
			case 0:	//ldc
				ldc(operand[PC]);
				break;
			case 1:	//adc
				adc(operand[PC]);
				break;
			case 2:	//ldl
				if(type==1){
					printf("Memory read from memory[%08x]: value=%08x\n", SP+operand[PC], memory[SP+operand[PC]]);
					fprintf(output, "Memory read from memory[%08x]: value=%08x\n", SP+operand[PC], memory[SP+operand[PC]]);
				}
				readCount++;
				ldl(operand[PC]);
				break;
			case 3:	//stl
				if(type==2){
					printf("Memory write at memory[%08x]: Initial value=%08x | Final value=%08x\n", SP+operand[PC] , memory[SP+operand[PC]], A);
					fprintf(output,"Memory write at memory[%08x]: Initial value=%08x | Final value=%08x\n", SP+operand[PC] , memory[SP+operand[PC]], A);
				}
				writeCount++;
				stl(operand[PC]);
				break;
			case 4:	//ldnl
				if(type==1){
					printf("Memory read from memory[%08x]: value=%08x\n", A+operand[PC], memory[A+operand[PC]]);
					fprintf(output, "Memory read from memory[%08x]: value=%08x\n", A+operand[PC], memory[A+operand[PC]]);
				}
				readCount++;
				ldnl(operand[PC]);
				break;
			case 5:	//stnl
				if(type==2){
					printf("Memory write at memory[%08x]: Initial value=%08x | Final value=%08x\n", A+operand[PC] ,memory[A+operand[PC]], B);
					fprintf(output, "Memory write at memory[%08x]: Initial value=%08x | Final value=%08x\n", A+operand[PC] ,memory[A+operand[PC]], B);
				}
				writeCount++;
				stnl(operand[PC]);
				break;
			case 6:	//add
				add();
				break;
			case 7:	//sub
				sub();
				break;
			case 8:	//shl
				shl();
				break;
			case 9:	//shr
				shr();
				break;
			case 10: 	//adj
				adj(operand[PC]);
				break;
			case 11:	//a2sp
				a2sp();
				break;
			case 12:	//sp2a
				sp2a();
				break;
			case 13:	//call
				call(operand[PC]);
				break;
			case 14:	//return
				return_();
				break;
			case 15:	//brz
				brz(operand[PC]);
				break;
			case 16:	//brlz
				brlz(operand[PC]);
				break;
			case 17:	//br
				br(operand[PC]);
				break;
			case 18:	//HALT
				if(type == 0){
					printf("PC: %08x | A: %08x | B: %08x | SP: %08x \n",PC+1, A, B, SP);
					printf("\nNumber of instructions executed: %d\n", count);
					fprintf(output, "PC: %08x | A: %08x | B: %08x | SP: %08x \n",PC+1, A, B, SP);
					fprintf(output, "\nNumber of instructions executed: %d\n", count);
				}
				if(type == 1){
					printf("\nNumber of memory reads: %d\n", readCount);
					fprintf(output, "\nNumber of memory reads: %d\n", readCount);
				}	
				if(type == 2){
					printf("\nNumber of memory writes: %d\n", writeCount);
					fprintf(output, "\nNumber of memory writes: %d\n", writeCount);
				}
				return;
				break; 	//HALT
		}
		PC++;
		count++;
		if(PC == size){
			if(type == 0){
				printf("PC: %08x | A: %08x | B: %08x | SP: %08x | Fatal error! HALT not found!\n",PC, A, B, SP);
				fprintf(output, "PC: %08x | A: %08x | B: %08x | SP: %08x | Fatal error! HALT not found!\n",PC, A, B, SP);
				printf("\nNumber of instructions executed: %d\n", count);
				fprintf(output,"\nNumber of instructions executed: %d\n", count);
			}

			if(type == 1){
				printf("\nNumber of memory reads: %d\n", readCount);
				fprintf(output, "\nNumber of memory reads: %d\n", readCount);
			}
			if(type == 2){
				printf("\nNumber of memory writes: %d\n", writeCount);
				fprintf(output, "\nNumber of memory writes: %d\n", writeCount);
			}
			return;
		}
		if(count >= INT_MAX || currentPC == PC){
			fprintf(output, "Errant Program! INFINITE LOOP\n");
			printf("Errant Program! INFINITE LOOP\n");
			return;	
		}
	}
	
}


//memory dump - print complete memory
//Terminal - first 100 and textfile - complete 
void memoryDump(){
	int count=1;
	printf("Memory - First 100\n");
	fprintf(output, "Memory Size - %dx8 bytes", MAX);
	//memory dump into terminal - first 100
	for(int i=0; i<100; i++){
		printf("%08x ", memory[i]);
		if(count == 10){
			printf("\n");
			count = 0;
		}		
		count++;
	}
	count=1;
	//memory dump into file - complete
	for(int i=0; i<MAX; i++){
		fprintf(output,"%08x ", memory[i]);
		if(count == 10){
			fprintf(output,"\n");
			count = 0;
		}		
		count++;
	}
	printf("\nTotal Memory size = %dx8 bytes\n", MAX);
	printf("Size of memory dumped in the terminal = 100x8 bytes\n");
	printf("Size of memory dumped size in the file = %dx8 bytes\n", MAX);
	fprintf(output,"\nTotal Memory size = %dx8 bytes\n", MAX);
	fprintf(output,"Size of memory dumped in the terminal = 100x8 bytes\n");
	fprintf(output,"Size of memory dumped size in the file = %dx8 bytes\n", MAX);
}


//Guide user
void printUsage(){
		printf("usage: emu [options] file.o\n");
		printf("-trace\tshow instruction trace\n");
		printf("-read\tshow memory reads\n");
		printf("-before\tshow memory dump before execution\n");
		printf("-after\tshow memory dump after execution\n");
		printf("-wipe\twipe written flags before execution\n");
		printf("-isa\tdisplay ISA\n");
}


/******MAIN PROGRAM******/

int main(int argc, char *argv[]){
	char fileName[24];
	if(argc<3){	//check for number of arguments from the terminal
		printUsage();	//print how to use
		return 0;
	}
	if(argv[1][0] != '-' || argv[2][0]=='\0'){	//check for valid arguments
		printUsage();
		return 0;
	}	
	strncpy(fileName, argv[2], strlen(argv[2])-2);	//remove .o from the arguments and copy it
	fileName[strlen(argv[2])-2]='\0';
	strcat(fileName, ".txt");	//concat .txt to the filename

	FILE *input;	//input file (binary)
	input = fopen(argv[2], "rb");
	if(input == NULL){
		printf("Binary File not found\n");	//if file not found
		exit(-1);	//exit code
	}
	uint32_t n;
	while(fread(&n,sizeof(uint32_t), 1,input) != 0){
		memory[size] = n;	//read and decode each line from the binary file
		size++;				//store them in memory and also count the size
		// printf("%" PRIu32 "\n", n);
	}
	fclose(input);

	//loop through the memory and split 8 bit opcode and 24 bit operand
	for(int i=0; i<size; i++){
		int32_t x = memory[i];
		int32_t y = memory[i];
		int k = memory[i];
        //we will left shift right shift 8 times and left shift 8 times to get zeros in the place of opcode
        //Now if we xor with the initial value, then we will get the 8 bit opcode
		x = x >> 8;	
		x = x << 8;
		opcode[i] = (x^y);
        int32_t shift = y >> 8;	//right shift 8 times to get operand
        operand[i] = shift;		
        if(opcode[i] == 19)		//check for data opcode and update memory with that value
            memory[i] = operand[i];
        if(opcode[i] == 20)	
            memory[i] = operand[i];
	}

	if(strcmp(argv[1], "-trace")==0){	//trace
		output = fopen(fileName,"w");
		fprintf(output, "Trace\n\n");
		emulate(0);
		fclose(output);
	}
	else if(strcmp(argv[1], "-read")==0){	//memory reads
		output = fopen(fileName,"w");
		fprintf(output, "Memory Reads\n\n");
		emulate(1);
		fclose(output);
	}
	else if(strcmp(argv[1], "-write")==0){	//memory writes
		output = fopen(fileName,"w");
		fprintf(output, "Memory Writes\n\n");
		emulate(2);
		fclose(output);
	}
	else if(strcmp(argv[1], "-isa")==0){	//instruction set architecture
		output = fopen(fileName,"w");
		fprintf(output, "Instruction Set Architecture\n\n");
		isa();
		fclose(output);
	}
	else if(strcmp(argv[1], "-before")==0){	//before
		output = fopen(fileName,"w");
		fprintf(output, "Memory dump before execution\n\n");
		memoryDump();
		fclose(output);
	}	
	else if(strcmp(argv[1], "-after")==0){	//after
		output = fopen(fileName,"w");
		fprintf(output, "Memory dump after execution\n\n");
		emulate(3);
		memoryDump();
		fclose(output);
	}
	else if(strcmp(argv[1], "-wipe")==0){  //wipe registers
		A=0;
		B=0;
		PC=0;
		SP=100;
	}
	else{
		printUsage();
	}

	return 0;
}
