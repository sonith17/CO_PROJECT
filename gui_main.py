from flask import Flask, render_template, request, jsonify
import pickle

app = Flask(__name__)

dataForward = False  # Toggle button
cacheSize = 512
blockSize = 8
associativity = 8
accessLatency = 1

latencies = {}
instructionsProgram1 = []
instructionsProgram2 = []
global p
p = None

instruct = ["add", "sub", "or", "and", "xor", "slt", "srl", "sll", "addi", "subi", "ori", "andi", "xori", "slti",
            "srli", "slli", "jalr", "lw", "sw", "beq", "bne", "blt", "bge", "auipc", "jal"]
for x in instruct:
    latencies[x] = [1, 1, 1, 1, 1]

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
    print("I am clicked Data")
    global p, cacheSize, blockSize, associativity, accessLatency

    # Retrieve values from request query parameters and convert them to integers
    new_cache_size = int(request.args.get('cacheSize', default=cacheSize))
    new_block_size = int(request.args.get('blockSize', default=blockSize))
    new_associativity = int(request.args.get('associativity', default=associativity))
    new_access_latency = int(request.args.get('accessLatency', default=accessLatency))

    # Update the global variables only if new values are provided
    if new_cache_size != cacheSize:
        cacheSize = new_cache_size
    if new_block_size != blockSize:
        blockSize = new_block_size
    if new_associativity != associativity:
        associativity = new_associativity
    if new_access_latency != accessLatency:
        accessLatency = new_access_latency

    # Now you can process the received data as needed, for example, print it
    print("Cache Size:", cacheSize)
    print("Block Size:", blockSize)
    print("Associativity:", associativity)
    print("Access Latency:", accessLatency)
    print("DataForward: " , dataForward)

    from Processor import Processor
    global processor
    processor = Processor(cacheSize, blockSize, associativity, accessLatency)
    p = processor
    core1_registers = processor.Core1.registers.tolist()
    core2_registers = processor.Core2.registers.tolist()
    memory_contents_1 = processor.memory1.tolist()
    memory_contents_2 = processor.memory2.tolist()
    

    # Optionally, you can return a response to the client
    return jsonify({"core1_registers": core1_registers, "core2_registers": core2_registers, "memory_contents_1": memory_contents_1,
                    "memory_contents_2": memory_contents_2})



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

#processor webpage
@app.route("/processor.html")
def processor1():
    global p
    if p is None:
        return "Processor object is not initialized. Please run the run_function first."
    processor_data = {
        'clock1': p.clock1,
        'Core1': {
            'instructionExecuted': p.Core1.instructionExecuted,
            'cacheAccess': p.Core1.cacheAccess,
            'cacheMiss': p.Core1.cacheMiss,
            'stalls': p.Core1.stalls
        },
        'Core2': {
            'instructionExecuted': p.Core2.instructionExecuted,
            'cacheAccess': p.Core2.cacheAccess,
            'cacheMiss': p.Core2.cacheMiss,
            'stalls': p.Core2.stalls
        }
    }
    return render_template('processor.html', processor=processor_data)



#when run button is clicked it run the program
@app.route('/run_function')
def run_function():
    global cacheSize, blockSize, associativity, accessLatency, dataForward
    global processor
    with open('Program1.s', 'r') as file:
        for line in file:
            if line.strip() != '':
                line = line.split('#', 1)[0]
                line = line.strip()
                instructionsProgram1.append(line)

    print(instructionsProgram1)
    from Parser import Parser
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

    return 'Function called successfully.'




if __name__ == '__main__':
    app.run(debug=True)
