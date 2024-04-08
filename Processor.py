import array
import numpy as np
from Core import Core
from Cache import Cache

class Processor:
    memory1 = np.zeros(2048, dtype=np.uint8)
    memory2 = np.zeros(2048, dtype=np.uint8)
    clock = 0
    clock1 = 0
    clock2 = 0
    Core1 = Core()
    Core2 = Core()
    cache:Cache
    accesslatencyofCache = 1

    def __init__(self,cacheSize,blockSize,associativity,accesslatencyofCache):
        self.cache = Cache(cacheSize=cacheSize,blockSize=blockSize,associativity=associativity)
        self.accesslatencyofCache = accesslatencyofCache
        # print("Cache got Created")


        

    def run(self,latencies,end_pc1,end_pc2,dataForward):
        t,t1=True,True
        while t1 or t:
            print(self.clock)
            with open("processor_state.txt", "a") as file:
              file.write(f"cls.clock: {self.clock}\n")
              file.close()
            t = self.Core1.run(self.memory1,latencies,end_pc1,dataForward,cache=self.cache,memOffset=0,accesslatencyofCache=self.accesslatencyofCache)
            t1 = self.Core2.run(self.memory2,latencies,end_pc2,dataForward,cache=self.cache,memOffset=2048,accesslatencyofCache=self.accesslatencyofCache)
            if t==0:
                self.clock1=self.clock
            if t1==0:
                self.clock2=self.clock
            self.clock+=1

    def run1(self,latencies,end_pc1,end_pc2):
        # t,t1=True,False
        # while t1 or t:
        print(self.clock)
        t = self.Core1.run(self.memory1,latencies,end_pc1)
        #t1 = cls.Core2.run(cls.memory2,latencies,end_pc2)
        if t==0:
            self.clock1=self.clock
        # if t1==0:
        #     cls.clock2=cls.clock
        self.clock+=1
    
