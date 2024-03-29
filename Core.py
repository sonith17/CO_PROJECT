import numpy as np

class Core:
    programCounter = 1
    registers = np.zeros(32, dtype=np.int32)
    instructionExecuted = 0

    def run(instructionLatencies):
        a=0