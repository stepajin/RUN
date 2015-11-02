#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#define BC vector<string> *


using namespace std;

bool isNumber(string str);
bool isBoolean(string str);
double toNumber(string str);
bool validIdentifier(string str);
string toString(int i);

void printVector(std::vector<string> * v);
vector<string> * append(std::vector<string> * v1, std::vector<string> * v2);

BC compile(string s, ifstream & in);

string readWord(ifstream & in);
string readString(string str, ifstream & in);

void rewind();
void printBuffer();


enum BYTE {
    BC_CONST = 1,
    BC_CHAR = 2,
    BC_STRING = 3,
    BC_EQ = 4,
    BC_ASSIGN = 5,
    BC_PLUS = 6,
    BC_MINUS = 7,
    BC_DIVIDE = 8,
    BC_MULTIPLY = 9,
    BC_YES = 10,
    BC_NO = 11,
    BC_LIST = 12,
    BC_LOAD = 13,
    BC_SKIP = 14,
    BC_SKIP_IF_FALSE = 15

};




BC compileBlock(ifstream & in) {
    BC bc = new std::vector<string>;

   // v->push_back("block");

    string s;
    while (true) {
        s = readWord(in);
        if (s == ")") {
           // v->push_back("endblock");
            return bc;
        }

        BC bc2 = compile(s, in);
        bc = append(bc, bc2);
    }


    return bc;
}

BC compileList(ifstream & in, int & numberOfItemsReturn) {
    BC bc = new vector<string>;
    
    int items = 0;
    string s;
    while (true) {
        s = readWord(in);
        if (s == "]") {
            numberOfItemsReturn = items;
            return bc;
        }

        BC bc2 = compile(s, in);
        bc = append(bc, bc2);
        items++;
    }   
    
    return bc; 
}


BC compile(string s, ifstream & in) {
    BC v = new std::vector<string> ();

     if (isNumber(s)) {
        v->push_back("CONST");
        v->push_back(s);
        return v;
     }

     if (s[0] == '"') {
        string str = readString(s, in);

        for (int i = 0; i < str.length(); i++) {
            v->push_back("CHAR");
            v->push_back(toString(str[i]));
        }

        v->push_back("STRING");
        v->push_back(toString(str.length()));
        return v;         
     }

     if (s == "yes") {
        v->push_back("YES");
        return v;
     }

     if (s == "no") {
        v->push_back("NO");
        return v;
     }

     if (s == "==") {
        BC bc1 = compile(readWord(in), in);
        BC bc2 = compile(readWord(in), in);
        v = append(bc1, bc2);
       
        v->push_back("EQ?");
        return v;
     }

     if (s == "=") {
        string id = readWord(in);
        if (!validIdentifier(id))
            exit(1);

        BC val = compile(readWord(in), in);
        v = append(v, val);
        v->push_back(id);
        v->push_back("ASSIGN");
        return v;
     }

     if (s == "+") {
        BC bc1 = compile(readWord(in), in);
        BC bc2 = compile(readWord(in), in);
        v = append(bc1, bc2);
        v->push_back("PLUS");
        return v;
     }

     if (s == "-") {
        BC bc1 = compile(readWord(in), in);
        BC bc2 = compile(readWord(in), in);
        v = append(bc1, bc2);
        v->push_back("MINUS");
        return v;
     }

     if (s == "/") {
        BC bc1 = compile(readWord(in), in);
        BC bc2 = compile(readWord(in), in);
        v = append(bc1, bc2);
        v->push_back("DIVIDE");
        return v;
     }

     if (s == "*") {
        BC bc1 = compile(readWord(in), in);
        BC bc2 = compile(readWord(in), in);
        v = append(bc1, bc2);
        v->push_back("MULTIPLY");
        return v;
     }


     if (s == "(") {
         return compileBlock(in);
     } 

     if (s == "if") {
        BC cond = compile(readWord(in), in);
        BC ifBlock = compile(readWord(in), in);

        int elseBlockLength = 0;
        BC elseBlock = NULL;

        if (readWord(in) == "else") {
            elseBlock = compile(readWord(in), in);
            elseBlockLength = elseBlock->size();
        } else {
            rewind();
        }

        if (elseBlockLength > 0) {
            ifBlock->push_back("SKIP");
            ifBlock->push_back(toString(elseBlockLength));
        }

        int ifBlockLength = ifBlock->size();

        v = append(v, cond);
        v->push_back("SKIP_IF_FALSE");

        int skipLength = ifBlockLength + elseBlockLength;
        stringstream ss;
        ss << ifBlockLength ;
        v->push_back(ss.str());

        v = append(v, ifBlock);
        if (elseBlock)
            v = append(v, elseBlock);
        return v;
     }

     if (s == "[") {
        int listLength;
        BC list = compileList(in, listLength);
        v = list;
        v->push_back("LIST");
        v->push_back(toString(listLength));
        return v;
     }

    if (validIdentifier(s)) {
        v->push_back("LOAD");
        v->push_back(s);
        return v;
    }

    return NULL;
}

int main(int argc, char ** argv) {
    if (argc < 2)
        return 1;

    ifstream in(argv[1]);

    if (!in.is_open()) {
        cout << "blbej file" << endl;
        return 1;
    }

    std::vector<string> * v = NULL;

    while (true) {
        if (in.eof()) {
            break;
        }


        std::vector<string> * v2 = compile(readWord(in), in);
        //printVector(v2);

        v = append(v, v2);
        //printVector(v);
    }

    printVector(v);


    return 0;
}


bool isNumber(string str) {
    if (str.length() == 0)
        return false;
    
    int i = 0;
    if (str[i] == '-' && str.length() > 1)
        i++;
    
    if (str[i] == '.')
        return false;

    int dots = 0;
    
    for (; i < str.length(); i++) {
        char c = str[i];
        if (c == '.') {
            if (++dots > 1 || i == str.length() - 1)
                return false;
            continue;
        }
        
        if (c < '0' || c > '9')
            return false;
    }
    
    return true;
}

bool isBoolean(string str) {
    return str == "yes" || str == "no";
}

double toNumber(string str) {
    return stod(str);
}

string toString(int i) {
    stringstream ss;
    ss << i;
    return ss.str();
}


bool validIdentifier(string str) {
    if (str.length() == 0)
        return false;
    
    
    for (int i = 0; i < str.length(); i++) {
        char c = str[i];
        
        if (c >= 'a' && c <= 'z')
            continue;
        
        if (c >= 'A' && c <= 'Z')
            continue;
        
        if (i > 0 && (c == '?' || c == '!'))
            continue;
        
        if (i > 0 && (c >= '0' && c <= '9'))
            continue;
            
        return false;
    }
    
    return true;
}


void printVector(std::vector<string> * v) {
    for (int i = 0; i < v->size(); i++) {
        cout << i << ": " << v->at(i) << endl;
    }
}

vector<string> * append(std::vector<string> * v1, std::vector<string> * v2) {
    vector<string> * v = new vector<string>;

    if (!v1)
        return v2;
    if (!v2)
        return v1;

    for (int i = 0; i < v1->size(); i++) {
        v->push_back(v1->at(i));
    }

    for (int i = 0; i < v2->size(); i++) {
        v->push_back(v2->at(i));
    }

    return v;
}

#define BUFFER_SIZE 4
string buffer[BUFFER_SIZE];
int bufferPos = 0;

void rewind() {
    bufferPos = (bufferPos + (BUFFER_SIZE - 1)) % BUFFER_SIZE;
    //cout << "rewind" << endl;
    //printBuffer();
}

void printBuffer() {
    cout << bufferPos << ": ";
    for (int i = 0; i < BUFFER_SIZE; i++) {
        if (i == bufferPos) {
            cout << "_" << buffer[i] << "_ ";
        } else if (buffer[i] == "") {
            cout << ". ";
        } else {
            cout << buffer[i] << " ";
        }
    }
    cout << endl;
}

string readWord(ifstream & in) {
    if (buffer[bufferPos] != "") {
        string s = buffer[bufferPos];
        bufferPos = (bufferPos + 1) % BUFFER_SIZE;
        return s;
    }

    string s;
    in >> s;
    
    buffer[bufferPos] = s;
    bufferPos = (bufferPos + 1) % BUFFER_SIZE;
    buffer[bufferPos] = "";

//    printBuffer();
    return s;
}

string readString(string str, ifstream & in) {
    while (str[str.length() - 1] != str[0] || str.length() == 1) {
        str += " ";
        str += readWord(in);
    }
    
    cout << "string " << str.substr(1, str.length() - 2) << endl;
    return str.substr(1, str.length() - 2);
}



