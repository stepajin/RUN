#include <cstdlib>
#include <iostream>
#include <vector>

using namespace std;

vector<string> * symbols(string s) {
	vector<string> * v = new vector<string>;

	string symb = "";
	int i = 0;
	int size = s.size();

	while (i < size) {
		if (s[i] == '+' && symb != "") {
			v->push_back(symb);
			symb = "";
		} else {
			symb += s[i];
		}

		i++;
	}

	if (symb != "") {
		v->push_back(symb);
	}

	return v;
}

string multiply(string a, string b) {
	if (a == "")
		return b;
	if (b == "")
		return a;

	vector<string> * v1 = symbols(a);
	vector<string> * v2 = symbols(b);

	string s = "";
	for (vector<string>::iterator it1 = v1->begin() ; it1 != v1->end(); ++it1) {
		for (vector<string>::iterator it2 = v2->begin() ; it2 != v2->end(); ++it2) {
			if (s != "") {
				s += "+";
			}

			s += *it1 + *it2;
		}
	}

	cout << "multiply '" << a << "' '" << b << "' -> " << s << endl;
	return s;
}

string append(string a, string b) {
	string r = a;
	if (a != "") {
		r += "+";	
	}
	r += b;

	cout << "append '" << a << "' '" << b << "' -> " << r << endl;
	return r;
}

string parse(string s, int start, int & end) {
	int size = s.size();	
	int i = start;

	string focus = "";
	string res = "";

	while (true) {
		char c = s[i];

		if (s[i] == '+' && focus != "") {
			res = append(res, focus);
			focus = "";
		} else if (c == '(') {
			int end;
			string rec = parse(s, i+1, end);
			i = end;

			focus = multiply(focus, rec);
		} else if (c == ')') {
			end = i;
			break;
		} else {
			string str = "";
			str += c;
			focus = multiply(focus, str);
		}

		i++;
		if (i >= size)
			break;
	}

	if (focus != "") {
		res = append(res, focus);
	}

	cout << "return " << res << endl;
	return res;
}

int main(int args, char ** argv) {
	//string a = (string)argv[1];

	string a = "a+b+c(d+e)+f+g(j+k(i+l))";
	string b = "a(e+b)(c(j+k)+d)f";

//	string c = multiply("a+b+c", "d+e");
	int end;
	string c = parse(b, 0, end);
	cout << c << endl;

	return 0;
}