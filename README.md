# CO_PROJECT_TEAM:GENSIS_PROCESSOR

## Overview

Welcome to the Read Me file for the development of our RISC-V Dual Core Processor.
## Project Description

We are currently engaged in the development of a RISC-V Dual Core Processor. The RISC-V architecture offers an open standard instruction set architecture (ISA), providing flexibility and scalability for various computing needs. This processor design focuses simulation of the RISC-V architecture.


## Minutes of the meeting(7th Meeting)
Date: 25-Feb-2024

Members: Venkat Kaushal, Sonith

Implementations:
1. We've implemented "sw" (store word) and "lw" (load word) functions, designed to be useful for pipelining operations.
2. To streamline our files and reduce clutter, we've removed the "using namespace std" directive from all our files and have instead prefixed "std::" to the appropriate places where the std namespace is required.
.


## Minutes of the meeting(6th Meeting)
Date: 23-Feb-2024

Members: Venkat Kaushal, Sonith

Completions:
1. Instructions are provided in the code to handle the storage of arrays comprising both integers and strings, with memory management being integral to the process.
.

Implementations:
1. Users now have the flexibility to include comments, add new lines, and insert extra spaces within the code. These elements are disregarded during execution for seamless operation.
2. We've formatted the output of the register and memory contents to display within a structured box for improved readability.

## Minutes of the meeting(5th Meeting)
Date: 20-Feb-2024

Members: Venkat Kaushal, Sonith

Completions:
1. Code containing only instructions.
.

Implementations:
1. We've tokenized the elements within the data section, ensuring they are appropriately parsed and stored.
2. We've crafted a C++ structure encapsulating details regarding addresses and stored values. Additionally, we've utilized a map linking data labels to instances of this structure, effectively collecting all data labels.
3. The code for the "sw" and "lw" instructions has been thoroughly debugged and refined to ensure their proper functionality.
4. We've implemented memory, with each block being 8 bits in size.

Issues and Challenges:
1. We've designed the memory to consist of 8-bit blocks, primarily to facilitate the storage of strings. However, additional effort was required to accommodate the storage of 32-bit integers.
2. Given the hexadecimal number, we've stored the elements in consecutive 4 blocks of 8 bits each in little endian format.
3. We've adjusted the "sw" (store word) and "lw" (load word) instructions accordingly to accommodate the storage and retrieval of data in consecutive 4 blocks of 8 bits each, adhering to the little endian format.


## Minutes of the meeting(4th Meeting)
Date: 19-Feb-2024

Members: Venkat Kaushal, Sonith

Completions:
1. All basic implementation
2. Running of count number of ones in cores2
.

Implementations:
1. We have handled cases where users may not have formatted their code with equal space distributions appropriately.
2. Resolved some minor bugs.
.

## Minutes of the meeting(3rd Meeting)
Date: 18-Feb-2024

Members: Venkat Kaushal, Sonith

Completions:
1. The issues raised during the last meeting have been effectively resolved and implemented.
.

Implementations:
1. Implemented instructions like: and, or, xor, andi, ori, xori, slli, srli, sll, srl, slt, slti, beq, blt, bge, j.
2. We've effectively gathered all the labels in the instruction section, along with their corresponding program counters, and stored them in a map.
.

Issues and Challenges:
1. Following the successful implementation, we proceeded to discuss the implementation of the data section and memory management.


## Minutes of the meeting(2nd Meeting)
Date: 16-Feb-2024

Members: Venkat Kaushal, Sonith

Completions:
1. The creation of the parser has been successfully completed.
2. The implementation of two core objects, each with their respective registers, as well as the processor object and other essential special registers, has been completed.
3. The implementation of basic instructions such as add, addi, and sub has been successfully completed.
.

Implementations:(Task accomplished collaboratively by Kaushal and Sonith.)
1. Successfully tested the functionality of basic instructions and implemented additional instructions, including branch instructions, load, and store instructions.
.

Issues and Challenges:
1. We have successfully implemented Unconditional Branch instructions. However, it's important to note that the labels referenced in these instructions must be present in the program before they are invoked to jump to the specified location.
2. To address this issue, we have decided to modify our approach. Now, before executing the program, we will first read the entire input, collect all the labels, and determine their corresponding addresses. This way, we ensure that all labels are processed and their addresses are available before any branch instructions are executed.



## Minutes of the meeting(1st Meeting)
Date: 10-Feb-2024

Members: Venkat Kaushal, Sonith

Decisions: 
1. We chose to concentrate our efforts on the RISC-V architecture rather than delving into other architectures.
2. We prioritized creating the parser to read the input assembly file and implementing basic functionalities such as add, addi, and sub.(task taken by Sonith)
3. We decided to contemplate the role and functionality of memory in this architecture.(Kaushal).

