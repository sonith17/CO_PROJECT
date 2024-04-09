import numpy as np
import pandas as pd
from Cache import Cache

def twos_complement_binary_to_int(binary_str):
    if binary_str[0] == '1':
        positive_value = int(binary_str, 2)
        num_bits = len(binary_str)
        return positive_value - (1 << num_bits)
    else:
        return int(binary_str, 2)


class Core:

    

    def __init__(self):
        self.pc = 0
        self.registers = np.zeros(32, dtype=np.int32)
        self.tempRegisters = np.zeros(32, dtype=np.int32)
        self.instructionExecuted = 0
        self.stalls = 0
        self.pipeline = []
        self.pipelineLatency = {}
        self.IF_input =0
        self.ID_input =''
        self.EXE_input =[]
        self.MEM_input =[]
        self.WB_input =[]
        self.IF_output =''
        self.ID_output =[]
        self.EXE_output =[]
        self.MEM_output =[]
        self.WB_output =[]
        self.registerInUse =[]
        self.toBeUsedRegisters =[]
        self.stalled_at_IF = False
        self.stalled_at_ID = False
        self.stalled_at_EXE = False
        self.stalled_at_MEM = False
        self.stalled_at_WB = False
        self.branchTaken = False
        self.harzad = False
        self.dataForward = False
        self.FirstAtIF = True
        self.FirstAtMEM = True
        self.cacheAccess = 0
        self.cacheMiss = 0

    
    def run(self,memory,instructionLatencies,end_pc,dataForward,cache:Cache,memOffset,accesslatencyofCache):
        ID_executed = False
        self.dataForward = dataForward
        if(self.pipeline or self.pc<=end_pc):
            if (self.pc <= end_pc) and (not self.stalled_at_IF) and (not self.stalled_at_ID) and (not self.stalled_at_EXE)and (not self.stalled_at_MEM)and (not self.stalled_at_WB) and(not self.harzad):
                # print("append", end_pc)
                self.pipeline.append([self.pc,1,1])
                self.FirstAtIF=True
                self.pipelineLatency[self.pc]=[1,1,1,1,1] #will be updated 
                self.IF_input=self.pc
        else:
            return False
        self.branchTaken=False
        # print(self.pipeline)
        # print(self.registerInUse)
        # print(self.harzad)
        # print(self.stalled_at_IF)
        # print(self.stalled_at_ID)
        # print(self.stalled_at_EXE)
        # print(self.stalled_at_MEM)
        # print(self.stalled_at_WB)
        # print(self.pc)
        # print(self.IF_input)
        # print(self.ID_input)
        # print(self.EXE_input)
        # print(self.MEM_input)
        # print(self.WB_input)
        # print(self.IF_output)
        # print(self.ID_output)
        # print(self.EXE_output)
        # print(self.MEM_output)
        
        with open("processor_state.txt", "a") as file:
            file.write(f"self.pipeline: {self.pipeline}\n")
            file.write(f"self.registerInUse: {self.registerInUse}\n")
            file.write(f"self.toBeUsedRegisters: {self.toBeUsedRegisters}\n")
            file.write(f"self.harzad: {self.harzad}\n")
            file.write(f"self.stalled_at_IF: {self.stalled_at_IF}\n")
            file.write(f"self.stalled_at_ID: {self.stalled_at_ID}\n")
            file.write(f"self.stalled_at_EXE: {self.stalled_at_EXE}\n")
            file.write(f"self.stalled_at_MEM: {self.stalled_at_MEM}\n")
            file.write(f"self.stalled_at_WB: {self.stalled_at_WB}\n")
            file.write(f"self.pc: {self.pc}\n")
            file.write(f"self.IF_input: {self.IF_input}\n")
            file.write(f"self.ID_input: {self.ID_input}\n")
            file.write(f"self.EXE_input: {self.EXE_input}\n")
            file.write(f"self.MEM_input: {self.MEM_input}\n")
            file.write(f"self.WB_input: {self.WB_input}\n")
            file.write(f"self.IF_output: {self.IF_output}\n")
            file.write(f"self.ID_output: {self.ID_output}\n")
            file.write(f"self.EXE_output: {self.EXE_output}\n")
            file.write(f"self.MEM_output: {self.MEM_output}\n")
            file.write(f"registers: {self.registers}\n")
            file.write(f"tempregisters: {self.tempRegisters}\n")
            file.write(f"memory:{memory[1024]}\n")
            file.write(f"memory:{memory[1025]}\n")
            file.write(f"memory:{memory[1026]}\n")
            file.write(f"memory:{memory[1027]}\n")
            file.write(f"memory:{memory[1028]}\n")
            file.write(f"-----------------------------------------------------------------------------------------------------\n\n\n")
            file.close()

        while(self.pipeline[0][0] == -1):
            # print("This should be taken32365488888888888888888888888888888888885")
            self.pipeline.pop(0)
            self.stalls+=1

        for i in self.pipeline:
            if i[1]==1:
                if self.FirstAtIF:
                    self.cacheAccess+=1
                    if(not cache.search(self.IF_input+memOffset)):
                        # print("miss from IF")
                        i[2] = accesslatencyofCache+1 #cache miss
                        self.cacheMiss += 1
                    else:
                        i[2] = accesslatencyofCache
                    self.FirstAtIF=False

                if i[2] > 0:
                    i[2] -= 1
                if i[2]==0 and (not self.stalled_at_ID) and(not self.stalled_at_EXE) and (not self.stalled_at_MEM)and (not self.stalled_at_WB) and (not self.harzad):
                    self.IF_output=self.IF(pc_given=self.IF_input,memory=memory)
                    self.ID_input=self.IF_output
                    self.stalled_at_IF = False
                else:
                    self.stalled_at_IF = True    
            elif i[1]==2:
                ID_executed = True
                if i[2] != 0:
                    if i[2] > 0:
                        i[2] -= 1
        
                if (i[2]==0) and (not self.stalled_at_EXE)  and (not self.stalled_at_MEM)and (not self.stalled_at_WB):
                    # print("iiiiiiiiiiiiiii")
                    self.ID_output=self.ID(self.ID_input)
                    self.pipelineLatency[i[0]]=instructionLatencies[self.ID_output[0]]
                    self.stalled_at_ID = False
                    for ind in range(len(self.registerInUse)):
                        if ind < len(self.registerInUse):  # Check if ind is within the valid range
                            wr, src1, src2 = self.registerInUse[ind]
                            if (wr != -1) and (wr != 0) and (wr in self.toBeUsedRegisters[1:]):
                                # print("hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh")
                                self.harzad = True
                                break
                            else:
                                self.harzad = False
                        else:
                            print("Error: Index out of range - ind:", ind, "registerInUse length:", len(self.registerInUse))
                    if len(self.registerInUse) ==0:
                        self.harzad=False
                    

                    if not self.harzad and not self.dataForward:
                        self.registerInUse.append(self.toBeUsedRegisters)
                        self.EXE_input=self.ID_output
                    elif not self.harzad and self.dataForward:
                        if self.ID_output[0]=='lw':
                            self.registerInUse.append(self.toBeUsedRegisters)
                        else:
                            self.registerInUse.append([-1,-1,-7])
                        self.EXE_input=self.ID_output
                else:
                    self.stalled_at_ID = True
            elif i[1]==3:
                if i[2] > 0:
                    i[2] -= 1
                if i[2]==0 and (not self.stalled_at_MEM)and (not self.stalled_at_WB):
                    self.EXE_output,nextPc=self.EXE(self.EXE_input,i[0])
                    if nextPc-i[0]!=4:
                        self.branchTaken=True
                        self.pc = nextPc
                    self.MEM_input = self.EXE_output
                    self.stalled_at_EXE = False
                else:
                    self.stalled_at_EXE = True
            elif i[1] == 4:
                if self.MEM_input[0]=='lw' or self.MEM_input[0]=='sw':
                    if self.FirstAtMEM:
                        self.cacheAccess+=1
                        if( not cache.search(self.MEM_input[2]+memOffset)):
                            i[2] = accesslatencyofCache+1 #cache miss
                            self.cacheMiss += 1
                        else:
                            i[2] = accesslatencyofCache
                        self.FirstAtMEM = False
                if i[2] > 0:
                    i[2] -= 1
                if i[2]==0 and not self.stalled_at_WB:
                    self.MEM_output=self.MEM(self.MEM_input,memory)
                    self.WB_input=self.MEM_output
                    self.stalled_at_MEM = False
                else:
                    self.stalled_at_MEM = True
            elif i[1] == 5:
                if i[2] > 0:
                    i[2] -= 1
                if i[2]==0:
                    self.WB(self.WB_input)
                    self.stalled_at_WB = False
                else:
                    self.stalled_at_WB = True

        
        if(self.pipeline[0][1]==5 and self.pipeline[0][2]==0):
            self.pipeline.pop(0)
            self.instructionExecuted+=1


        #need to update inuseregiter with,handle hazard,stalls due to var latencies.updatelatencies,promote to next stage
        if self.branchTaken:  #assuming branch statements latency is one
            self.pipeline = [row for row in self.pipeline if row[1] not in [1, 2]] 
            self.pipeline.append([-1,-1,-2])
            self.pipeline.append([-1,-1,-3])
            self.EXE_input.clear()
            if(not self.harzad) and not self.dataForward and ID_executed:
                self.registerInUse.pop(len(self.registerInUse)-1)
            if(not self.harzad) and self.dataForward and ID_executed:
                self.registerInUse.pop(len(self.registerInUse)-1)
            self.ID_output.clear()
            self.ID_input=""
            self.IF_output=""
            self.harzad = False
            self.stalled_at_ID=False
            self.stalled_at_IF=False

        index =0
        isStallAdded = False
        while index < len(self.pipeline):
            if(self.pipeline[index][1]==5):
                if(self.stalled_at_WB):
                    self.pipeline.insert(index,[-1,-1,-4])
                    isStallAdded =True
                    index+=1
            elif(self.pipeline[index][1]==4):
                if(not self.stalled_at_WB and not self.stalled_at_MEM):
                    self.pipeline[index][1]=5
                    self.pipeline[index][2]=self.pipelineLatency[self.pipeline[index][0]][4]
                elif(self.stalled_at_MEM and (not isStallAdded)):
                    self.pipeline.insert(index,[-1,-1,-5])
                    isStallAdded =True
                    index+=1
            elif(self.pipeline[index][1]==3):
                if(not self.stalled_at_WB) and (not self.stalled_at_MEM and  not self.stalled_at_EXE) :
                    self.pipeline[index][1]=4
                    self.FirstAtMEM=True
                    self.pipeline[index][2]=self.pipelineLatency[self.pipeline[index][0]][3]
                elif(self.stalled_at_EXE and (not isStallAdded)):
                    self.pipeline.insert(index,[-1,-1,-6])
                    isStallAdded =True
                    index+=1
            elif(self.pipeline[index][1]==2):
                if(not self.stalled_at_WB) and (not self.stalled_at_MEM) and (not self.stalled_at_EXE) and (not self.harzad) and (not self.stalled_at_ID):
                    self.pipeline[index][1]=3
                    self.pipeline[index][2]=self.pipelineLatency[self.pipeline[index][0]][2]
                elif((self.stalled_at_ID and (not isStallAdded)) and (not self.harzad)):
                    self.pipeline.insert(index,[-1,-1,-8])
                    isStallAdded =True
                    index+=1
                elif(( (not self.stalled_at_ID) and (not isStallAdded)) and (self.harzad)):
                    self.pipeline.insert(index,[-1,-1,-8])
                    isStallAdded =True
                    index+=1
            elif(self.pipeline[index][1]==1):
                if(not self.stalled_at_WB) and (not self.stalled_at_MEM) and (not self.stalled_at_EXE) and (not self.harzad ) and(not self.stalled_at_ID) and (not self.stalled_at_IF):
                    self.pipeline[index][1]=2
                    self.pipeline[index][2]=self.pipelineLatency[self.pipeline[index][0]][1]
                elif((self.stalled_at_IF and (not isStallAdded)) and (not self.harzad)):
                    self.pipeline.insert(index,[-1,-1,-9])
                    isStallAdded =True
                    index+=1
            index+=1
        if(not self.branchTaken) and  (not self.stalled_at_WB) and (not self.stalled_at_MEM) and (not self.stalled_at_EXE) and (not self.harzad ) and(not self.stalled_at_ID) and (not self.stalled_at_IF):
            # print("------------------------------------------$$$$$$$$$$$$$$$$$$$$$$")
            self.pc+=4
            # print("pc:" , self.pc)
        # if(self.branchTaken):
        #     self.branchTaken = False
        return True

                




        # if(self.harzad):
        #     index = 0
        #     while index < len(self.pipeline):
        #         if self.pipeline[index][1]==2:
        #             self.pipeline.insert(index,[-1,-1,-1])
        #             index+=1
        #         index+=1
                

                
                





    def IF(self,pc_given,memory):
        instruction =""

        for i in range(4):
            instruction = bin(memory[pc_given+i])[2:].zfill(8) +instruction
        
        # print("if ",instruction)
        return instruction

    def ID(self,instruction):
        decoded =[]
        df = pd.read_csv('Instruction.csv')
        df = df[df['Opcode']==int(instruction[-7:])]
        if(df.iloc[0]['Type']=='R-Type'):
            df2 = df[(df['Func3']==int(instruction[-15:-12])) & (df['Func7']==int(instruction[:7]))].iloc[0]
            decoded.append(df2['Instruction'])
            decoded.append(int(instruction[-12:-7],2))
            decoded.append(int(instruction[-20:-15],2))
            decoded.append(int(instruction[-25:-20],2))
            self.toBeUsedRegisters=[int(instruction[-12:-7],2),int(instruction[-20:-15],2),int(instruction[-25:-20],2)]
        elif(df.iloc[0]['Type']=='I-Type'):
            df2 = df[df['Func3']==int(instruction[-15:-12])].iloc[0]
            decoded.append(df2['Instruction'])
            decoded.append(int(instruction[-12:-7],2))
            decoded.append(int(instruction[-20:-15],2))
            decoded.append(twos_complement_binary_to_int(instruction[:12]))
            self.toBeUsedRegisters=[int(instruction[-12:-7],2),int(instruction[-20:-15],2),-1]
        elif(df.iloc[0]['Type']=='S-Type'):
            df2 = df[df['Func3']==int(instruction[-15:-12])].iloc[0]
            decoded.append(df2['Instruction'])
            decoded.append(int(instruction[-25:-20],2))
            decoded.append(int(instruction[-20:-15],2))
            decoded.append(twos_complement_binary_to_int(instruction[:7]+instruction[-12:-7]))
            self.toBeUsedRegisters=[-1,int(instruction[-20:-15],2),int(instruction[-25:-20],2)]
        elif(df.iloc[0]['Type']=='SB-Type'):
            df2 = df[df['Func3']==int(instruction[-15:-12])].iloc[0]
            decoded.append(df2['Instruction'])
            decoded.append(int(instruction[-20:-15],2))
            decoded.append(int(instruction[-25:-20],2))
            decoded.append(twos_complement_binary_to_int(instruction[0]+instruction[-8]+instruction[1:7]+instruction[-12:-8]+'0'))
            self.toBeUsedRegisters=[-1,int(instruction[-20:-15],2),int(instruction[-25:-20],2)]
        elif(df.iloc[0]['Type']=='UJ-Type'):
            df2 = df.iloc[0]
            decoded.append(df2['Instruction'])
            decoded.append(int(instruction[-12:-7],2))
            decoded.append(twos_complement_binary_to_int(instruction[0]+instruction[-20:-12]+instruction[11]+instruction[1:11]+'0'))
            self.toBeUsedRegisters=[int(instruction[-12:-7],2),-1,-1] 
        return decoded
    
    def EXE(self,decoded,currPc):
        executed =[decoded[0]]
        if decoded[0]=='add':
            val = self.registers[decoded[2]]+self.registers[decoded[3]]
            if self.dataForward:
                val =self.tempRegisters[decoded[2]]+self.tempRegisters[decoded[3]]
                self.tempRegisters[decoded[1]] = val
            executed.append(decoded[1])
            executed.append(val)
            return executed,currPc+4
        elif decoded[0]=='sub':
            val = self.registers[decoded[2]]-self.registers[decoded[3]]
            if self.dataForward:
                val =self.tempRegisters[decoded[2]]-self.tempRegisters[decoded[3]]
                self.tempRegisters[decoded[1]] = val
            executed.append(decoded[1])
            executed.append(val)
            return executed,currPc+4
        elif decoded[0]=='or':
            val = self.registers[decoded[2]]|self.registers[decoded[3]]
            if self.dataForward:
                val =self.tempRegisters[decoded[2]]|self.tempRegisters[decoded[3]]
                self.tempRegisters[decoded[1]] = val
            executed.append(decoded[1])
            executed.append(val)
            return executed,currPc+4
        elif decoded[0]=='and':
            val = self.registers[decoded[2]]&self.registers[decoded[3]]
            if self.dataForward:
                val =self.tempRegisters[decoded[2]]&self.tempRegisters[decoded[3]]
                self.tempRegisters[decoded[1]] = val
            executed.append(decoded[1])
            executed.append(val)
            return executed,currPc+4
        elif decoded[0]=='xor':
            val = self.registers[decoded[2]]^self.registers[decoded[3]]
            if self.dataForward:
                val =self.tempRegisters[decoded[2]]^self.tempRegisters[decoded[3]]
                self.tempRegisters[decoded[1]] = val
            executed.append(decoded[1])
            executed.append(val)
            return executed,currPc+4
        elif decoded[0]=='slt':
            val = 1 if self.registers[decoded[2]] < self.registers[decoded[3]] else 0
            if self.dataForward:
                val = 1 if self.tempRegisters[decoded[2]] < self.tempRegisters[decoded[3]] else 0
                self.tempRegisters[decoded[1]] = val
            executed.append(decoded[1])
            executed.append(val)
            return executed,currPc+4
        elif decoded[0]=='srl':
            val = self.registers[decoded[2]] >> self.registers[decoded[3]]
            if self.dataForward:
                val =self.tempRegisters[decoded[2]]>>self.tempRegisters[decoded[3]]
                self.tempRegisters[decoded[1]] = val
            executed.append(decoded[1])
            executed.append(val)
            return executed,currPc+4
        elif decoded[0]=='sll':
            val = self.registers[decoded[2]] << self.registers[decoded[3]]
            if self.dataForward:
                val =self.tempRegisters[decoded[2]]<<self.tempRegisters[decoded[3]]
                self.tempRegisters[decoded[1]] = val
            executed.append(decoded[1])
            executed.append(val)
            return executed,currPc+4
        
        elif decoded[0]=='addi':
            val = self.registers[decoded[2]]+decoded[3]
            if self.dataForward:
                val = self.tempRegisters[decoded[2]]+decoded[3]
                self.tempRegisters[decoded[1]] = val
            executed.append(decoded[1])
            executed.append(val)
            return executed,currPc+4
        elif decoded[0]=='ori':
            val = self.registers[decoded[2]]|decoded[3]
            if self.dataForward:
                val = self.tempRegisters[decoded[2]]|decoded[3]
                self.tempRegisters[decoded[1]] = val
            executed.append(decoded[1])
            executed.append(val)
            return executed,currPc+4
        elif decoded[0]=='andi':
            val = self.registers[decoded[2]]&decoded[3]
            if self.dataForward:
                val = self.tempRegisters[decoded[2]]&decoded[3]
                self.tempRegisters[decoded[1]] = val
            executed.append(decoded[1])
            executed.append(val)
            return executed,currPc+4
        elif decoded[0]=='xori':
            val = self.registers[decoded[2]]^decoded[3]
            if self.dataForward:
                val = self.tempRegisters[decoded[2]]^decoded[3]
                self.tempRegisters[decoded[1]] = val
            executed.append(decoded[1])
            executed.append(val)
            return executed,currPc+4
        elif decoded[0]=='slti':
            val = 1 if self.registers[decoded[2]] < decoded[3] else 0
            if self.dataForward:
                val = 1 if self.tempRegisters[decoded[2]] < decoded[3] else 0
                self.tempRegisters[decoded[1]] = val
            executed.append(decoded[1])
            executed.append(val)
            return executed,currPc+4
        elif decoded[0]=='srli':
            val = self.registers[decoded[2]] >> decoded[3]
            if self.dataForward:
                val = self.tempRegisters[decoded[2]]>>decoded[3]
                self.tempRegisters[decoded[1]] = val
            executed.append(decoded[1])
            executed.append(val)
            return executed,currPc+4
        elif decoded[0]=='slli':
            val = self.registers[decoded[2]] << decoded[3]
            if self.dataForward:
                val = self.tempRegisters[decoded[2]]<<decoded[3]
                self.tempRegisters[decoded[1]] = val
            executed.append(decoded[1])
            executed.append(val)
            return executed,currPc+4
        
        elif decoded[0]=='lw':
            val = self.registers[decoded[2]]+decoded[3]
            if self.dataForward:
                val = self.tempRegisters[decoded[2]]+decoded[3]
            executed.append(decoded[1])
            executed.append(val)
            return executed,currPc+4
        elif decoded[0]=='sw':
            val = self.registers[decoded[2]]+decoded[3]
            if self.dataForward:
                val = self.tempRegisters[decoded[2]]+decoded[3]
            executed.append(decoded[1])
            executed.append(val)
            return executed,currPc+4
        
        elif decoded[0]=='beq':
            
            if self.dataForward:
                currPc = currPc+decoded[3] if self.tempRegisters[decoded[1]]==self.tempRegisters[decoded[2]] else currPc+4
            else:
                currPc = currPc+decoded[3] if self.registers[decoded[1]]==self.registers[decoded[2]] else currPc+4
            return executed,currPc
        elif decoded[0]=='bne':
            if self.dataForward:
                currPc = currPc+decoded[3] if self.tempRegisters[decoded[1]]!=self.tempRegisters[decoded[2]] else currPc+4
            else:
                currPc = currPc+decoded[3] if self.registers[decoded[1]]!=self.registers[decoded[2]] else currPc+4
            return executed,currPc
        elif decoded[0]=='blt':
            if self.dataForward:
                currPc = currPc+decoded[3] if self.tempRegisters[decoded[1]]<self.tempRegisters[decoded[2]] else currPc+4
            else:
                currPc = currPc+decoded[3] if self.registers[decoded[1]]<self.registers[decoded[2]] else currPc+4
            return executed,currPc
        elif decoded[0]=='bge':
            if self.dataForward:
                currPc = currPc+decoded[3] if self.tempRegisters[decoded[1]]>=self.tempRegisters[decoded[2]] else currPc+4
            else:
                currPc = currPc+decoded[3] if self.registers[decoded[1]]>=self.registers[decoded[2]] else currPc+4
            return executed,currPc
        
        elif decoded[0]=='jal':
            val = currPc+decoded[2]
            executed.append(decoded[1])
            executed.append(currPc+4)
            return executed,val
        elif decoded[0]=='jalr':
            val = currPc+4
            executed.append(decoded[1])
            executed.append(val)
            if self.dataForward:
                return executed,self.tempRegisters[decoded[2]]+decoded[3]
            return executed,self.registers[decoded[2]]+decoded[3]
        
    def MEM(self,executed,memory):
        memorised=[]
        if executed[0]=='lw':
            val = ""
            for i in range(4):
               val = bin(memory[executed[2]+i])[2:].zfill(8) +val
            val = twos_complement_binary_to_int(val)
            memorised.append(executed[1])
            memorised.append(val)
            if self.dataForward:
                self.tempRegisters[executed[1]]=val
                if len(self.registerInUse)>0:
                    self.registerInUse.pop(0)
            return memorised
        elif executed[0]=='sw':
            store = [(self.registers[executed[1]] >> (i * 8)) & 0xFF for i in range(4)] if (not self.dataForward)  else [(self.tempRegisters[executed[1]] >> (i * 8)) & 0xFF for i in range(4)]
            for i in range(4):
                memory[executed[2]+i]=store[i]
            if self.dataForward:
                if len(self.registerInUse)>0:
                    self.registerInUse.pop(0)
            return []
        else:
            executed.pop(0)
            if self.dataForward:
                if len(self.registerInUse)>0:
                    self.registerInUse.pop(0)
            return executed
    
    def WB(self,memorised):
        if memorised and memorised[0]!=0 and(not self.dataForward):
            self.registers[memorised[0]]=memorised[1]
        if memorised and memorised[0]!=0 and(self.dataForward):
            self.registers[memorised[0]]=self.tempRegisters[memorised[0]]
        self.registers[0]=0
        if(not self.dataForward):
            self.registerInUse.pop(0)
            self.harzad = False
        return


            




    # def test(self,pc,memory):
    #     for i in range(0,pc+1,4):
    #         print(self.ID(self.IF(i,memory)))
    #     return False




