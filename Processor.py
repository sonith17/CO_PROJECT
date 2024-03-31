import array
import numpy as np
from Core import Core

class Processor:
    memory1 = np.zeros(2048, dtype=np.uint8)
    memory2 = np.zeros(2048, dtype=np.uint8)
    clock = 0
    clock1 = 0
    clock2 = 0
    Core1 = Core()
    Core2 = Core()

    @classmethod
    def run(cls,latencies,end_pc1,end_pc2):
        t,t1=True,False
        while t1 or t:
            t = cls.Core1.test(end_pc1,cls.memory1)
            #t1 = cls.Core2.run(cls.memory2,latencies,end_pc2)
            if t==0:
                cls.clock1=cls.clock
            # if t1==0:
            #     cls.clock2=cls.clock
            cls.clock+=1
    
