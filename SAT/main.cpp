//
//  main.cpp
//  SAT
//
//  Created by Štěpánek Jindřich on 11/12/15.
//  Copyright © 2015 stepajin. All rights reserved.
//

#include <cstdlib>
#include <iostream>
#include <vector>
#include <sstream>

#define DEBUG true

using namespace std;

bool is_operator(const string & s) {
    return (s == "+" || s == "." || s == "!");
}

bool is_element(const string & s) {
    return !is_operator(s) && !(s == "(") && !(s == ")");
}

int priority(const string & op) {
    if (op == "!") {
        return 3;
    } else if (op == ".") {
        return 2;
    } else if (op == "+") {
        return 1;
    } else {
        return 0;
    }
}

void print(vector<string> * v) {
    for (int i = 0; i < v->size(); i++) {
        cout << v->at(i) << " ";
    }
    cout << endl;

}

vector<string> * conjunctions_list(string s) {
 	vector<string> * v = new vector<string>;

 	string conj = "";
 	int i = 0;
 	int size = s.size();

 	while (i < size) {
 		if (s[i] == '+' && conj != "") {
 			v->push_back(conj);
 			conj = "";
 		} else if (s[i] != '(' && s[i] != ')') {
 			conj += s[i];
 		}

 		i++;
 	}

 	if (conj != "") {
 		v->push_back(conj);
 	}
    
 	return v;
}

string disjunction(string a, string b) {
    string result = a;
    if (a != "") {
        result += "+";
    }
    result += b;
    
    if (DEBUG)
        cout << "disjunction '" << a << "' || '" << b << "' -> " << result << endl;
    
    return result;
}

string conjunction(string a, string b) {
    if (a == "")
        return b;
    if (b == "")
        return a;

    // split to list of conjunctions which will be multiplied
    
    vector<string> * conjunctions1 = conjunctions_list(a);
    vector<string> * conjunctions2 = conjunctions_list(b);

    string s = "";
    for (vector<string>::iterator it1 = conjunctions1->begin() ; it1 != conjunctions1->end(); ++it1) {
        for (vector<string>::iterator it2 = conjunctions2->begin() ; it2 != conjunctions2->end(); ++it2) {
            if (s != "") {
                s += "+";
            }

            s += "(" + *it1 + "." + *it2 + ")";
        }
    }

    if (DEBUG)
        cout << "conjunction '" << a << "' && '" << b << "' -> " << s << endl;
    
    return s;
}

string negate_conjunction(string s) {
    string result = "";
    int size = s.size();
    int position = 0;
    bool first = true;
    
    while (true) {
        if (position >= size)
            break;

        char c = s[position];
        
        if (c == '.') {
            result += "+";
            first = true;
        } else if (first) {
            if (c != '!') {
                result += '!';
                result += c;
            }
            
            first = false;
        } else {
            result += c;
        }

        position++;
    }
    
    return result;
}

string negation(string s) {
    string result = "";
    
    vector<string> * conjunctions = conjunctions_list(s);

    for (int i = 0; i < conjunctions->size(); i++) {
        string conjuntion = conjunctions->at(i);
        string neg = negate_conjunction(conjuntion);
        
        result = conjunction(result, neg);
    }
    
    if (DEBUG) {
        cout << "negate " << s << " -> " << result << endl;
    }
    
    return result;
}

vector<string> * build_postfix(const string & expression) {
    istringstream iss(expression);
    vector<string> tokens;
    
    while (!iss.eof())
    {
        string temp;
        iss >> temp;
        
        tokens.push_back(temp);
    }
    
    vector<string> * postfix = new vector<string>;
    vector<string> stack;
    
    for (int i = 0; i < tokens.size(); i++) {
        
        if (is_element(tokens[i]))
        {
            postfix->push_back(tokens[i]);
        }
        
        if (tokens[i] == "(")
        {
            stack.push_back(tokens[i]);
        }
        
        if (tokens[i] == ")")
        {
            while(!stack.empty() && stack.back() != "(")
            {
                postfix->push_back(stack.back());
                stack.pop_back();
            }
            
            stack.pop_back();
        }
        
        if (is_operator(tokens[i]) == true)
        {
            while(!stack.empty() && priority(stack.back()) >= priority(tokens[i]))
            {
                postfix->push_back(stack.back());
                stack.pop_back();
            }
            
            stack.push_back(tokens[i]);
        }
        
    }
    
    //pop remaining operators
    while(!stack.empty())
    {
        postfix->push_back(stack.back());
        stack.pop_back();
    }
    
    //tree = reverse_vector(tree);
    
    return postfix;
}

string parse_postfix(vector<string> * postfix) {
    vector<string> stack;
    
    for (long i = 0; i < postfix->size(); i++) {
        string token = postfix->at(i);
        cout << "token " << token << endl;
        
        if (token == "!") {
            string op = stack.back();
            stack.pop_back();
            
            string neg = negation(op);
            stack.push_back(neg);
            
        } else if (token == ".") {
            string op1 = stack.back();
            stack.pop_back();
            string op2 = stack.back();
            stack.pop_back();
            
            string conj = conjunction(op1, op2);
            stack.push_back(conj);
            
        } else if (token == "+") {
            string op1 = stack.back();
            stack.pop_back();
            string op2 = stack.back();
            stack.pop_back();
            
            string disj = disjunction(op1, op2);
            stack.push_back(disj);

        } else {
            stack.push_back(token);
        }
    }
    
    return stack.back();
}

int main(int args, char ** argv) {
    string a = "a . ! b + ! ( ( c . d ) + u . v ) + ! e . f";
    
    vector<string> * tree = build_postfix(a);
    print(tree);
    
    string res = parse_postfix(tree);
    
    cout << "---" << endl << res << endl << "---" << endl;
    
    return 0;
}
