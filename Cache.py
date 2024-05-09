import numpy as np
import math
import random
import time

class CacheSet:

    
    def __init__(self, associativity, policy):
        self.associativity = associativity
        self.ageBits = list(range(associativity))
        self.SetBlocks = tuple([CacheBlock() for _ in range(associativity)])
        self.Size = 0
        self.Capacity = associativity
        self.policy = policy
    
    def search(self,tag):
        # print("searcg being calles")
        ind =0
        with open("processor_state.txt", "a") as file:
            for b in self.SetBlocks:
                file.write(f"setBlock {ind}  {self.SetBlocks[ind].vaild}  {self.SetBlocks[ind].tag}\n")
                ind+=1
            file.close()
        found = False
        for i in range(0, self.associativity):
            if self.SetBlocks[i].tag == tag and self.SetBlocks[i].vaild :
                return True
        return False
    
    def isFull(self):
        with open("processor_state.txt", "a") as file:
            file.write(f"CAP is,{self.Capacity},{self.Size}\n")
        # print("CAP is",self.Capacity,self.Size)
        return self.Size == self.Capacity
    
    def cacheMiss(self, tag):
        if self.isFull():
            if(self.policy == 0):
                self.LRU(tag)
            else:
                self.Random(tag)
        else:
            self.getBlock(tag) # Just insert the element, should not increase Size in the function
            index = self.Size
            for i in range(0, self.associativity):
                if(i > index):
                    self.ageBits[i] = self.ageBits[i] - 1
                if(i == index):
                    self.ageBits[i] = self.associativity - 1
            self.Size += 1
    
    def LRU(self, tag):
        for i in range(0, self.associativity):
            if(self.ageBits[i] == 0):
                self.SetBlocks[i].tag = tag
                self.SetBlocks[i].vaild = True
                index = i
                for j in range(0, self.associativity):
                    if(j > index):
                        self.ageBits[j] = self.ageBits[j] - 1
                    if(i == index):
                        self.ageBits[j] = self.associativity - 1
                return
            
    
    def Random(self, tag):
        current_time = int(time.time())
        random.seed(current_time)

        index = random.randrange(self.associativity)
        for i in range(0, self.associativity):
            if(i > index):
                self.ageBits[i] = self.ageBits[i] - 1
            if(i == index):
                self.ageBits[i] = self.associativity - 1
        self.SetBlocks[index].tag = tag
        self.SetBlocks[index].vaild = True


    
    def getBlock(self, tag):
        self.SetBlocks[self.Size].tag = tag
        self.SetBlocks[self.Size].vaild = True
        # print("IN GET BLOCK")
        

class CacheBlock:
    vaild:bool
    tag:np.uint32

    def __init__(self):
        self.vaild = False
        self.tag = 0


class Cache:
    cacheSize=0
    associativity = 0
    blockSize=0
    cacheMemory = tuple([])
    numBlocks = 0
    

    def __init__(self,cacheSize,blockSize,associativity,ReplacementPolicy):
        self.cacheSize = cacheSize
        self.associativity = associativity
        self.blockSize=blockSize
        self.numBlocks = cacheSize//(blockSize)
        self.cacheMemory = tuple([CacheSet(associativity,ReplacementPolicy) for _ in range(self.numBlocks//associativity)])
        # print("Created Sucessfully")
    
    def splitAddress(self,address):
        num_bits_offset = int(math.log2(self.blockSize))
        address = address >> num_bits_offset
        index:np.uint32 = np.uint32(int(address) & int(((1 << int(math.log2(self.numBlocks//self.associativity))) - 1)))
        tag:np.uint32 = np.uint32(address >> int(math.log2(self.numBlocks//self.associativity)))
        return tag, index
    
    def search(self,address):
        tag:np.uint32
        index:np.uint32
        tag,index = self.splitAddress(address)
        # print("address",tag,index,address)
        with open("processor_state.txt", "a") as file:
            file.write(f"index,tag is,{index},{tag},{address}\n")
        found = False
        if self.cacheMemory[index].search(tag):
            return True
        else:
            self.cacheMemory[index].cacheMiss(tag)
        return False


    
