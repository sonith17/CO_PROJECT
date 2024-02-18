# CO_PROJECT_TEAM:GENSIS_PROCESSOR

## Overview

Welcome to the Read Me file for the development of our RISC-V Dual Core Processor.
## Project Description

We are currently engaged in the development of a RISC-V Dual Core Processor. The RISC-V architecture offers an open standard instruction set architecture (ISA), providing flexibility and scalability for various computing needs. This processor design focuses simulation of the RISC-V architecture.
## Minutes of the meeting(1st Meeting)
Date: 10-Feb-2024

Members: Venkat Kaushal, Sonith

Decisions: 
1. We chose to concentrate our efforts on the RISC-V architecture rather than delving into other architectures.
2. We prioritized creating the parser to read the input assembly file and implementing basic functionalities such as add, addi, and sub.(task taken by Sonith)
3. We decided to contemplate the role and functionality of memory in this architecture.(Kaushal).


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