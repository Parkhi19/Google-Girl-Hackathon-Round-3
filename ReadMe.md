
# Combinational circuit Fault Detection :  Google Girl Hackathon Round - 3 

**Problem Description**

Given a faulty node and its type (whether SA0 or SA1) in a combinational circuit, the task is to determine the input vector to correctly test the fault and also the expected output of the circuit.

----------

## Steps to Run the algorithm

- Install the compiler for C++. (Refer [here](https://code.visualstudio.com/docs/cpp/config-mingw))
- Replace `circuitFile.txt` with your own input circuit file with **same** name.
Note : Circuit file format should be as follows:
```
net_e = A & B
net_f = C | D
net_g = ~ net_f
Z = net_g ^ net_e
```
- Compile the code by writing following command in the terminal.
For windows:

```g++ .\GGH.cpp```

For Mac and Linux:

```g++ ./GGH.cpp```

- Execute the following command with command line arguments.
For windows:

```.\a.exe net_f SA0```

For Mac and Linux:

```./a.exe net_f SA0```

Where `net_f` and `SA0` represents the `FAULT_AT ` and `FAULT_TYPE` respectively.
Replace these with the required fault parameters.

- An `output.txt` file will be generated containing the input vector and faulty output in the following format : 
```
[A, B, C, D] = [0, 0, 0, 1], Z = 1
```