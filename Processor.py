import array
import numpy as np
from Core import Core

class Processor:
    memory1 = np.zeros(2048, dtype=np.int8)
    memory2 = np.zeros(2048, dtype=np.int8)
    clock = 0
    clock1 = 0
    clock2 = 0
    Core1 = Core()
    Core2 = Core()
    
