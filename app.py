from flask import Flask, render_template, request, jsonify, redirect, url_for
import pickle
from Parser import Parser
from Processor import Processor
import math

app = Flask(__name__)

dataForward = False  # Toggle button
cacheSize = 512
blockSize = 8
associativity = 8
accessLatency = 1
global policy
policy = 0
global latencies
latencies = {}

global p
p = None
global processor
processor = None



#opening route
@app.route("/")
@app.route("/opening")
def opening():
    return render_template('opening.html')

#home page
@app.route('/home')
def home():
    return render_template('home.html')

@app.route('/toggle_data_forwarding', methods=['POST'])
def toggle_data_forwarding():
    global dataForward
    data = request.json
    dataForward = data.get('isChecked')
    return 'Data forwarding toggled successfully'

# Route to retrieve data forwarding state
@app.route('/get_data_forwarding')
def get_data_forwarding():
    global dataForward
    return jsonify({'dataForward': dataForward})


#confirm data values
@app.route('/process_data', methods=['GET'])
def process_data():
    # print("I am clicked Data")
    global p, cacheSize, blockSize, associativity, accessLatency
    global processor
    global policy
    print("Policy1: " , policy)
    # Retrieve values from request query parameters and convert them to integers
    new_cache_size = int(request.args.get('cacheSize', default=cacheSize))
    new_block_size = int(request.args.get('blockSize', default=blockSize))
    new_associativity = int(request.args.get('associativity', default=associativity))
    new_access_latency = int(request.args.get('accessLatency', default=accessLatency))
    new_policy=int(request.args.get('policy', default=policy))
        

    # Update the global variables only if new values are provided
    if new_cache_size != cacheSize:
        cacheSize = new_cache_size
    if new_block_size != blockSize:
        blockSize = new_block_size
    if new_associativity != associativity:
        associativity = new_associativity
    if new_access_latency != accessLatency:
        accessLatency = new_access_latency
    if new_policy != policy:
        policy = new_policy

    # Now you can process the received data as needed, for example, print it
    # print("Cache Size:", cacheSize)
    # print("Block Size:", blockSize)
    # print("Associativity:", associativity)
    # print("Access Latency:", accessLatency)
    # print("DataForward: " , dataForward)
    print("policy " , policy)
    #processor = Processor(cacheSize, blockSize, associativity, accessLatency)
    if processor is not None:
        p = processor
        core1_registers = processor.Core1.registers.tolist()
        core2_registers = processor.Core2.registers.tolist()
        memory_contents_1 = processor.memory1.tolist()
        memory_contents_2 = processor.memory2.tolist()
    

    # Optionally, you can return a response to the client
        return jsonify({"core1_registers": core1_registers, "core2_registers": core2_registers, "memory_contents_1": memory_contents_1,
                        "memory_contents_2": memory_contents_2})
    return 'Processor is None'


@app.route('/update_instructions', methods=['POST'])
def update_instructions():
    data = request.json
    instructions = data.get('instructions')
    process_instructions(instructions)
    return jsonify({'message': 'Instructions received and processed successfully'})

def process_instructions(instructions):
    global latencies
    latencies = {
        "add": [1, 1, 1, 1, 1],
        "sub": [1, 1, 1, 1, 1],
        "or": [1, 1, 1, 1, 1],
        "and": [1, 1, 1, 1, 1],
        "xor": [1, 1, 1, 1, 1],
        "slt": [1, 1, 1, 1, 1],
        "srl": [1, 1, 1, 1, 1],
        "sll": [1, 1, 1, 1, 1],
        "addi": [1, 1, 1, 1, 1],
        "subi": [1, 1, 1, 1, 1],
        "ori": [1, 1, 1, 1, 1],
        "andi": [1, 1, 1, 1, 1],
        "xori": [1, 1, 1, 1, 1],
        "slti": [1, 1, 1, 1, 1],
        "srli": [1, 1, 1, 1, 1],
        "slli": [1, 1, 1, 1, 1],
        "jalr": [1, 1, 1, 1, 1],
        "lw": [1, 1, 1, 1, 1],
        "sw": [1, 1, 1, 1, 1],
        "beq": [1, 1, 1, 1, 1],
        "bne": [1, 1, 1, 1, 1],
        "blt": [1, 1, 1, 1, 1],
        "bge": [1, 1, 1, 1, 1],
        "auipc": [1, 1, 1, 1, 1],
        "jal": [1, 1, 1, 1, 1]
    }
    
    for instruction in instructions:
        instruct_type = instruction['type']
        user_inputs = instruction['inputs']
        
        # Check if the instruction type is valid
        if instruct_type in latencies:
            # Update the latency values with user inputs
            latencies[instruct_type] = user_inputs
        else:
            print(f"Invalid instruction type: {instruct_type}")
    
    # Print or return the updated latencies
    print("Updated instruction latencies:", latencies)



#when we click submit in section 1 is updates contents of Program1.s
@app.route('/submit_section1', methods=['POST'])
def submit_section1():
    data = request.json
    content = data.get('content')
    with open('Program1.s', 'w') as f:
        f.write(content + '\n')
    return 'Content from Section 1 submitted successfully.'

#when we click in section2 it updates contents of Program2.s
@app.route('/submit_section2', methods=['POST'])
def submit_section2():
    data = request.json
    content = data.get('content')
    with open('Program2.s', 'w') as f:
        f.write(content + '\n')
    return 'Content from Section 2 submitted successfully.'

#memory webpage
@app.route("/memory.html")
def memory():
    return render_template('memory.html')

#cache webpage
@app.route("/cache.html")
def cache():
    global processor, blockSize, cacheSize, associativity
    cache_contents = list(processor.cache.cacheMemory) if processor is not None else None

    started = True if cache_contents is not None else False

    # Extract all tags from cache memory
    all_tags = []
    if cache_contents is not None:
        for cache_set in cache_contents:
            for cache_block in cache_set.SetBlocks:
                all_tags.extend([cache_block.tag])
        cache_size = len(all_tags)

    if not started:
         return render_template('cache.html',started=started, all_tags= all_tags, cache_size= cacheSize, blockSize=blockSize, associativity = associativity)


    

    return render_template('cache.html',started=started, all_tags= all_tags, cache_size= cacheSize, cacheMemory= processor.cache.cacheMemory, blockSize=blockSize, associativity = associativity)

#processor webpage
@app.route("/processor.html")
def processor1():
    global p
    if p is None:
        return "Processor object is not initialized. Please run the run_function first."
    processor_data = {
        'clock1': p.clock1,
        'clock2': p.clock2,
        'Core1': {
            'instructionExecuted': p.Core1.instructionExecuted,
            'cacheAccess': p.Core1.cacheAccess,
            'cacheMiss': p.Core1.cacheMiss,
            'stalls': p.Core1.stalls,
            'IPC': p.Core1.instructionExecuted / p.clock1 if p.clock1 != 0 else 0
        },
        'Core2': {
            'instructionExecuted': p.Core2.instructionExecuted,
            'cacheAccess': p.Core2.cacheAccess,
            'cacheMiss': p.Core2.cacheMiss,
            'stalls': p.Core2.stalls,
            'IPC': p.Core2.instructionExecuted / p.clock1 if p.clock1 != 0 else 0
        }
    }
    
    return render_template('processor.html', processor=processor_data)



#when run button is clicked it run the program
@app.route('/run_function')
def run_function():
    global cacheSize, blockSize, associativity, accessLatency, dataForward, policy
    global processor,p, latencies
    processor = Processor(cacheSize, blockSize, associativity, accessLatency,policy)
    p=processor
    print(policy,"nu uuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuu")
    instructionsProgram1 = []
    instructionsProgram2 = []
    with open('Program1.s', 'r') as file:
        for line in file:
            if line.strip() != '':
                line = line.split('#', 1)[0]
                line = line.strip()
                instructionsProgram1.append(line)

    print(instructionsProgram1)
    
    p1 = Parser()
    pc1 = p1.parse(memory=(processor.memory1), instruction=instructionsProgram1)
    with open('Program2.s', 'r') as file:
        for line in file:
            if line.strip() != '':
                line = line.split('#', 1)[0]
                line = line.strip()
                instructionsProgram2.append(line)
    p2 = Parser()
    pc2 = p2.parse(memory=(processor.memory2), instruction=instructionsProgram2)
    processor.run(latencies, end_pc1=pc1, end_pc2=pc2, dataForward=dataForward)

    return redirect(url_for('processor1'))



if __name__ == '__main__':
    app.run(debug=True)
