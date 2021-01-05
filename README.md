# Assembler-Emulator



<!-- PROJECT LOGO -->
<br />
<p align="center">

  <h3 align="center">C language implementation of Assembler and Emulator for a custom assembly language</h3>

  <p align="center">
    View
    <a href="AssemblyLanguage.pdf"><strong>Assembly Language »</strong></a>
    for the language definitions.
    <br />
  </p>
</p>

<!-- ABOUT THE PROJECT -->
## About The Project

Diagnoses Label errors - No such Label, Bogus label, etc. <br/>
Produces Listing file <br/>
Advanced Listing file - Instructions displayed beside each memory <br/>
Op-table is created in the form of the struct. Retrieving is done through open-chain hashing. <br/>
SET instruction - Assigned a temporary opcode <br/>

Process:
* Assembler generates three files - object file (machine readable), listing file and logfile(for errors and warnings)
* The object file is passed into emulator to generate trace, memory dump, memory writes, memory reads, instructions set architecture etc.


<!-- GETTING STARTED -->
## Getting Started

Run the assembler using gcc - Produces listing file, object file and log file
  ```sh
  gcc asm.c -o asm
  ./asm filename.asm
  ```

### Emulator

1. Copy the object file into the directory of emulator and run emu.c
2. 
   ```sh
   gcc emu.c -o emu
   ./emu [options] filename.o
   ```
   ```sh
   The following options are available
   -trace   | show instruction trace
   -read    | show memory reads
   -write   | show memory writes
   -before  | show memory dump before execution
   -after   | show memory dump after execution
   -wipe    | wipe written flags before execution
   -isa     | display instruction set architecture
   ```
A text file is generated using the option given.

Some example test files and the generated files are given in the respective folders
