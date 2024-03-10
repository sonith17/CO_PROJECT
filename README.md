# CO_PROJECT_TEAM:GENSIS_PROCESSOR

## Overview

Welcome to the Read Me file for the development of our RISC-V Dual Core Processor.
## Project Description

We are currently engaged in the development of a RISC-V Dual Core Processor. The RISC-V architecture offers an open standard instruction set architecture (ISA), providing flexibility and scalability for various computing needs. This processor design focuses simulation of the RISC-V architecture.

#### -End of Phase2-----
## Minutes of the meeting(16th Meeting)
Date: 10th March 2024

Members: Venkat Kaushal, Sonith

Completions:
1. Read Me Update
2. IPC Calcualations.
3. Removing Debugging Statements.
.

## Minutes of the meeting(15th Meeting)
Date: 9th March 2024

Members: Venkat Kaushal, Sonith

Completions:
1. We've successfully implemented latency for each instruction and carried out some debugging to ensure its functionality.
.

Implementations:
1. We've ensured that if an instruction takes more than one cycle to execute, we implement the function only when there is exactly one cycle remaining in our model.
2. To simulate instructions that require multiple cycles, we've opted for this approach because we can perform any task within a single cycle in our simulation.
.

Challenges:
1. If we've implemented the same function for all the multiple cycles, for instructions like "addi x4 x4 1", by the end of the write-back stage, we end up with an update of x4+2 instead of x4+1.

## Minutes of the meeting(15th Meeting)
Date: 8th March 2024

Members: Venkat Kaushal, Sonith

Completions:
1. We've initiated and successfully completed the implementation where the pipeline effectively handles cases involving data forwarding.
.

Implementations:
1. We've introduced temporary registers to store values and utilized them as a source of information for fetching registers during data forwarding. This allows us to act accordingly based on the stored values.

Challenges:
1. We've implemented an alternative approach that eliminates the need for temporary registers altogether. Instead, we leverage the inherent capabilities of the pipeline. The temporary registers used in the stage functions carry the necessary information, allowing us to pass this information from execute to execute and from memory to execute for data forwarding.
2. Facing numerous bugs to address and with time ticking, we decided to pivot to another approach to complete the task.

## Minutes of the meeting(14th Meeting)
Date: 5th March 2024 - 7th March 2024

Members: Venkat Kaushal, Sonith

Completions:
1. We've begun debugging the code to address issues related to the absence of data forwarding.
.


## Minutes of the meeting(13th Meeting)
Date: 3rd March 2024 & 4th March 2024

Members: Venkat Kaushal, Sonith

Completions:
1. We've opted to detect hazards using a different strategy.
.

Implementations:
1. We have implemented a structure called inUsedRegister, which holds a significant amount of information about its previous instructions. It stores the write and read registers in pairs of integers within a pair<int, pair<int, int>> data structure, respectively.
2. We utilize this register to detect data hazards within the pipeline.
3. We proceeded to develop the remaining components of the pipeline.

Challenges:
1. In our previous implementations, if we encounter the situation where the same register is being read from and written to, we introduce a stall.It has been concluded that this approach is not ideal and should be reconsidered.

## Minutes of the meeting(12th Meeting)
Date: 3rd March 2024 & 4th March 2024

Members: Venkat Kaushal, Sonith

Completions:
1. We've initiated the implementation to manage data hazards.
.

Implementations:
1. We've devised a strategy to keep track of which registers are in the reading stage and which ones are designated for writing. These are stored as boolean variables.
2. We simulated a scenario where data forwarding is enabled, allowing us to write back the value to the register directly at the execute stage itself. (Just for Checking the implementation).

## Minutes of the meeting(11th Meeting)
Date: 2 March 2024

Members: Venkat Kaushal, Sonith

Completions:
1. We've begun writing a function for RunPipeline.
.

Implementations:
1. We have a program counter (PC) and stages stored in one data structure, and the times of each instruction in another data structure. As we progress through the pipeline, it's crucial to ensure proper synchronization between the times data structure and its corresponding PC. We've attempted to handle cases where multiple latencies are involved, though it's not fully complete—we've initialized with a latency of 1 for now.
2. We're attempting to simulate a similar flow to that of a pipeline.

## Minutes of the meeting(10th Meeting)
Date: 29 February 2024 & 1st March 2024

Members: Venkat Kaushal, Sonith

Completions:
1. A parser class has been implemented by us.
2. Stage functions have been implemented successfully.
.

Implementations:
1. Given the string lines within the program, the process involves reading all tokens present in a line. By referencing a map named instrType, the appropriate Object class corresponding to each token is determined. Objects of these classes are then instantiated, their member functions populated with instruction tokens, and finally stored in a separate map along with their respective program counters (PC).
2. We've determined that the instruction fetch function will take the program counter as an argument and return a string, which will be stored temporarily in a designated location.
3. The instruction decode function utilizes the temporary value obtained from the instruction fetch results to decode the instruction and examine the registers.This function returns a vector of strings or tokens.
4. The output of the instruction decode serves as the argument for the execute function, where it reads the values and performs the appropriate arithmetic operations. It then returns a vector of strings containing the opcode, destination, and value, with each represented by three values.
5. The results from the execute stage are utilized by the memory stage to facilitate memory operations such as lw, sw, and others.
It returns a pair(string, string).

6. The last write-back stage writes the values to the registers.


## Minutes of the meeting(9th Meeting)
Date: 28 February 2024

Members: Venkat Kaushal, Sonith

Completions:
1. The instructions jal, jr, and jalr have been implemented.
.

Implementations:
1. We've studied the functionality of the instructions and implemented them accordingly. 


## Minutes of the meeting(8th Meeting)
Date: 26 February 2024

Members: Venkat Kaushal, Sonith

Completions:
1. Creating Instruction Class
.

Implementations:
1. We've established an instructional class aimed at converting assembly instructions into corresponding objects. Within this framework, we've developed classes such as RType, IType, SType, SBType, UType, and UJType to facilitate this conversion process. 

Issues and Challenges:
1. Creating the instruction class poses no problems.

#### -End of Phase1-----
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