#include <bits/stdc++.h>
using namespace std;

class LogicGate
{
public:
    std::string input1;
    std::string input2;
    std::string expression;
    char logicalOperator;

    LogicGate(const std::string &input1, const std::string &input2, const std::string &expression, char logicalOperator)
        : input1(input1), input2(input2), expression(expression), logicalOperator(logicalOperator)
    {
    }
};


bool findNotOperator(string s)
{
    int pos = s.find('~');
    return pos != string::npos;
}

class Circuit
{
public:
    string FAULT_AT;
    string FAULT_TYPE;
    bool notOperatorPresent = false;
    int totalStringsInLogicalExpression = 0;
    vector<string> stringsInLogicalExpression;
    vector<LogicGate> logicGates;
    unordered_map<string, bool> mapWithoutFault;
    unordered_map<string, bool> mapWithFault;

    Circuit(const string &FAULT_AT, const string &FAULT_TYPE)
        : FAULT_AT(FAULT_AT), FAULT_TYPE(FAULT_TYPE)
    {
    }

    private:
    void parseCircuit(const string &circuitFileName)
    {
       
        ifstream file(circuitFileName);
        if (!file.is_open())
        {
            cout << "Failed to open file: " << circuitFileName << endl;
            return;
        }

        string line;
      
        while (getline(file, line))
        {

            if (findNotOperator(line))
            {
                notOperatorPresent = true;
                totalStringsInLogicalExpression = 4;
            }
            else
            {
                totalStringsInLogicalExpression = 5;
            }

            stringsInLogicalExpression.resize(totalStringsInLogicalExpression);
            int i = 0;
            stringstream ssin(line);
            while (ssin.good() && i < totalStringsInLogicalExpression)
            {
                ssin >> stringsInLogicalExpression[i];
                i++;
            }
            if (notOperatorPresent)
            {
                LogicGate logicGate("not", stringsInLogicalExpression[3], stringsInLogicalExpression[0], stringsInLogicalExpression[2][0]);
                logicGates.push_back(logicGate);
            }
            else
            {
                LogicGate logicGate(stringsInLogicalExpression[2], stringsInLogicalExpression[4], stringsInLogicalExpression[0], stringsInLogicalExpression[3][0]);
                logicGates.push_back(logicGate);
            }
            notOperatorPresent = false;
        }

        file.close();
    }


    void expressionMapping()
    {
        for (int i = 0; i < 16; i++)
        {
            mapWithFault.clear();
            mapWithoutFault.clear();
            int A = (i >> 3) & 1;
            int B = (i >> 2) & 1;
            int C = (i >> 1) & 1;
            int D = i & 1;

            mapWithoutFault.emplace("A", A);
            mapWithoutFault.emplace("B", B);
            mapWithoutFault.emplace("C", C);
            mapWithoutFault.emplace("D", D);

            mapWithFault.emplace("A", A);
            mapWithFault.emplace("B", B);
            mapWithFault.emplace("C", C);
            mapWithFault.emplace("D", D);

            for (auto x : logicGates)
            {

                if (x.input1 == "not")
                {
                    mapWithoutFault[x.expression] = !(mapWithoutFault[x.input2]);

                    if (FAULT_AT == x.expression)
                    {
                        if (FAULT_TYPE == "SA0")
                        {
                            mapWithFault[x.expression] = 0;
                        }
                        else
                        {
                            mapWithFault[x.expression] = 1;
                        }
                    }
                    else
                    {
                        mapWithFault[x.expression] = !(mapWithFault[x.input2]);
                    }
                    continue;
                }
                else
                {

                    bool inputOne = mapWithoutFault[x.input1];
                    bool inputTwo = mapWithoutFault[x.input2];

                    bool result;

                    switch (x.logicalOperator)
                    {
                    case '&':
                        result = inputOne & inputTwo;
                        break;

                    case '|':
                        result = inputOne | inputTwo;
                        break;

                    case '^':
                        result = inputOne ^ inputTwo;
                        break;
                    default:
                        break;
                    }
                    mapWithoutFault[x.expression] = result;

                    if (FAULT_AT == x.expression)
                    {
                        if (FAULT_TYPE == "SA0")
                        {
                            mapWithFault[x.expression] = 0;
                        }
                        else
                        {
                            mapWithFault[x.expression] = 1;
                        }
                    }
                    else
                    {
                        inputOne = mapWithFault[x.input1];
                        inputTwo = mapWithFault[x.input2];
                        switch (x.logicalOperator)
                        {
                        case '&':
                            result = inputOne & inputTwo;
                            break;

                        case '|':
                            result = inputOne | inputTwo;
                            break;

                        case '^':
                            result = inputOne ^ inputTwo;
                            break;
                        default:
                            break;
                        }
                        mapWithFault[x.expression] = result;
                    }
                }
            }
            if (mapWithFault["Z"] != mapWithoutFault["Z"])
            {
                ofstream MyFile("output.txt");
                MyFile <<"[A, B, C, D] = ["<<A<<", "<<B<<", "<<C<<", "<<D<<"], Z = "<<mapWithFault["Z"];
                MyFile.close();
                break;
            }
        }
    }

    public:
    void detectFault(const string &circuitFileName){
        this->parseCircuit(circuitFileName);
        this->expressionMapping();

    }
};

int main(int argc, char** argv)
{
    string faultLocation = argv[1];
    string faultType = argv[2];
    Circuit circuit(faultLocation, faultType);
    circuit.detectFault("circuitFile.txt");

    return 0;
}
