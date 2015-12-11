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
#include <algorithm>

#define DEBUG false

using namespace std;

bool contains(const vector<string> & v, string item) {
    if (v.empty())
        return false;
    
    return (std::find(v.begin(), v.end(), item) != v.end());
}

bool is_operator(const string & s) {
    return (s == "+" || s == "." || s == "!");
}

bool is_symbol(char c) {
    return c == '+' || c == '.' || c == '!' || c == '(' || c == ')';
}

bool is_element(const string & s) {
    return s.size() > 1 || !is_symbol(s[0]);
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

    // split to lists of conjunctions which will be multiplied
    
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
        cout << "negation " << s << " -> " << result << endl;
    }
    
    return result;
}

string read_token(string s, int start, int & end) {
    int i = start;
    int size = s.size();
    
    string res = "";
    
    while (true) {
        if (is_symbol(s[i]) && res == "") {
            res += s[i];
            end = i;
            return res;
        }
        
        if (i >= size || is_symbol(s[i])) {
            end = i-1;
            return res;
        }
        
        if (s[i] != ' ')
            res += s[i];
        
        i++;
    }
}

vector<string> * build_postfix(const string & expression) {
    vector<string> tokens;
    
    int position = 0;
    int end;
    int size = expression.size();
    
    while (true) {
        string token = read_token(expression, position, end);
        position = end + 1;
        
        tokens.push_back(token);

        if (position >= size)
            break;
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
    
    while(!stack.empty())
    {
        postfix->push_back(stack.back());
        stack.pop_back();
    }
    
    return postfix;
}

string parse_dnf(vector<string> * postfix) {
    vector<string> stack;
    
    for (long i = 0; i < postfix->size(); i++) {
        string token = postfix->at(i);
        
        if (DEBUG)
            cout << "token " << token << endl;
        
        if (token == "!") {
            string op = stack.back();
            stack.pop_back();
            
            string neg = negation(op);
            stack.push_back(neg);
            
        } else if (token == ".") {
            string op2 = stack.back();
            stack.pop_back();
            string op1 = stack.back();
            stack.pop_back();
            
            string conj = conjunction(op1, op2);
            stack.push_back(conj);
            
        } else if (token == "+") {
            string op2 = stack.back();
            stack.pop_back();
            string op1 = stack.back();
            stack.pop_back();
            
            string disj = disjunction(op1, op2);
            stack.push_back(disj);

        } else {
            stack.push_back(token);
        }
    }
    
    return stack.back();
}

bool evaluate_conjunction(string s) {
    string element = "";
    long size = s.size();
    
    vector<string> positive;
    vector<string> negative;
    
    for (long i = 0; i <= size; i++) {
        if (s[i] == '.' || i == size) {
            if (element[0] == '!') {
                element = element.substr(1, element.size() - 1);
                
                if (contains(positive, element))
                    return false;
                
                negative.push_back(element);
            } else {
                if (contains(negative, element))
                    return false;
                
                positive.push_back(element);
            }
            
            element = "";
        } else {
            element += s[i];
        }
    }
    
    return true;
}

bool SAT(const string & s) {
    cout << s << endl;
    
    vector<string> * postfix = build_postfix(s);
    print(postfix);
    
    string dnf = parse_dnf(postfix);
    cout << dnf << endl;
    cout << "---" << endl;

    bool res = false;
    
    vector<string> * conjuntions = conjunctions_list(dnf);
    
    for (long i = 0; i < conjuntions->size(); i++) {
        if (evaluate_conjunction(conjuntions->at(i))) {
            res = true;
            break;
        }
    }
    
    string str = res ? "TRUE" : "FALSE";
    cout << str << endl << "---" << endl;

    return res;
}

int main(int args, char ** argv) {
    // JO
    SAT("a . ! b + ! ( ( c . d . ! d ) + u . v ) + ! e . f");
    
    // NE
    SAT("aaa.( bbb + ccc ).!bbb.!ccc");
    
    // a) T = {(p ⇒ q) ∧ r, q ∧ r, r ⇒ s, p ∧ ¬s} NE
    SAT("((!p+q).r) . (q.r) . (!r.s) . (p.!s)");
    
    // b) F = {(p ∧ q ∧ r) ⇒ [(s ∧ ¬t) ∨ (¬s ∧ t)], q ∧ r, ¬s, ¬t, p} NE
    SAT("( !(p + q + r) + (s.!t + !s.t) ) . (q+r) . (!s) . (!t) . (p)");
    
    // c) G = {q ⇒ r, r ⇒ p, q ⇒ p} JO
    SAT("(!q + r) . (!r + p) . (!q + p)");
    
    // d) Y = {q ⇒ r, r ⇒ p, ¬(q ⇒ p)} NE
    SAT("(!q + r).(!r + p).!(!q + p)");
    
    // e) Z = {(p ∨ q) ⇔ r, r, ¬p, q} JO
    SAT("((p+q).r + !(p+q).!r) . (r) . (!p) . (q)");
        
    return 0;
}
