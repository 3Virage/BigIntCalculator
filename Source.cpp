#include <iostream>
#include <string>
using namespace std;
class BigInt {
	int* mem;
	int size;
	bool sign; //0-plus 1-minus
public:


	BigInt() :mem(NULL), size(0), sign(0) {}
	BigInt(int a) {
		size = a;
		mem = new int[size];
		for (int i = 0; i < size; i++)
			mem[i] = 0;
		sign = 0;
	}
	BigInt(string num) {
		if (num[0] == '-') {
			sign = 1;
			size = num.length() - 1;
			mem = new int[size];
			for (unsigned int i = 1; i < num.length(); i++) {
				char c = num[i];
				mem[i - 1] = atoi(&c);
			}
		}
		else {
			sign = 0;
			size = num.length();
			mem = new int[size];
			for (int i = 0; i < size; i++) {
				char c = num[i];
				mem[i] = atoi(&c);
			}
		}
	}
	BigInt(const BigInt   &copy) {
		size = copy.size;
		mem = new int[size];
		for (int i = 0; i < size; i++)
			mem[i] = copy.mem[i];
		sign = copy.sign;
	}
	~BigInt() {
		delete[] mem;
	}


	int get()const {
		return size;
	}
	BigInt resize(int a) {
		int diff = a - size;
		BigInt temp(*this);
		delete[] mem;
		size = a;
		mem = new int[size];
		for (int i = a; i >= 0; i--) {
			if (i >= diff)mem[i] = temp.mem[i - diff];
			else mem[i] = 0;
		}
		return *this;
	}
	BigInt delzeros() {


		int k = 0;
		int* i = mem;
		while (!*i) {
			k++; //count leading zeros
			i++;
			if (i - mem == size) {
				BigInt temp(*this);
				delete[] mem;
				size = 1;
				sign = 0;
				mem = new int[1];
					mem[0] = 0;
				return *this;
			}
		}
		BigInt temp(*this);
		delete[] mem;
		size = size - k;
		sign = temp.sign;
		mem = new int[size];
		for (int i = 0; i < size; i++)
			mem[i] = temp.mem[i + k];
		return *this;
	}
	void print() {
		if (sign)cout << "-";
		for (int i = 0; i < size; i++)
			cout << mem[i];
		cout << endl;
	}


	BigInt operator+(const BigInt  &add)const {
		BigInt obj(*this);
		BigInt addobj(add);
		int bigger;				//set to the same size

		if (obj.get() > addobj.get())
			bigger = obj.get();
		else
			bigger = addobj.get();
		obj.resize(bigger + 1);
		addobj.resize(bigger + 1);

		int plus = 0;
		for (int i = bigger; i >= 0; i--) {
			if (obj.sign)obj.mem[i] *= (-1);
			if (addobj.sign)addobj.mem[i] *= (-1);
			obj.mem[i] += plus;
			if (obj.mem[i] + addobj.mem[i] > 9) {
				obj.mem[i] = obj.mem[i] + addobj.mem[i] - 10;
				plus = 1;
			}
			else {
				obj.mem[i] = obj.mem[i] + addobj.mem[i];
				plus = 0;
			}
		}
		if (obj.mem[0] < 0) {
			obj.sign = 1;
			obj.mem[0] *= -1;
		}
		return obj.delzeros();
	}
	BigInt operator-(const BigInt &sub) {
		BigInt subobj(sub);
		subobj.sign = !sub.sign;
		return (*this + subobj).delzeros();
	}
	BigInt &operator=(const BigInt &eq) {
		size = eq.size;
		sign = eq.sign;
		delete[] mem;
		mem = new int[size];
		for (int i = 0; i < size; i++)
			mem[i] = eq.mem[i];
		return *this;
	}
	
	BigInt operator*(const BigInt &mul){ //good opytmalization but not working
		int plus = 0;
		int x = mul.get();
		int y = mul.get() + get();

		int **tab = new int*[x];   //2D array for results
		for (int i = 0; i < x; i++)
			tab[i] = new int[y];
		for (int i = 0; i < x; i++)
			for (int j = 0; j < y; j++)
				tab[i][j] = 0;

		int res = 0;
		int k = y-1;
		
		for (int i = 0; i < x; i++) {
			k = y - 1 - i;
			for (int j = get()-1; j >=0; j--) {
				tab[i][k] = res;
				tab[i][k] += mul.mem[mul.get() - i - 1] * mem[j];
				if (tab[i][k]>9) {
					res = (tab[i][k] - (tab[i][k] % 10)) / 10;
						tab[i][k] = tab[i][k] % 10;
				}
				else {
					res = 0;
				}
				k--;
			}
			tab[i][k] = res;
			res = 0;
		}

/*
		for (int i = 0; i < x; i++) {
			cout << endl;
			for (int j = 0; j < y; j++)
				cout << tab[i][j];
		}
*/
		BigInt temp(y + 1);
		for (int i = 0; i < temp.get(); i++)
			temp.mem[i] = 0;

		int sum = 0;
		k = temp.size-1;
		for (int j =y-1; j>=0; j--) { //sum of array
			for (int i = 0; i < x; i++)
				sum += tab[i][j];
			if (sum < 10) {
				temp.mem[k] = sum;
				sum = 0;
			}
			else {
				temp.mem[k] = sum % 10;
				sum = (sum - (sum % 10)) / 10;
			}
			k--;
		}
		temp.mem[k] = sum;
		temp.sign = (sign == mul.sign ? 0 : 1);


		for (int i = 0; i < x; i++)
			delete[] tab[i];
		delete[] tab;

		return temp.delzeros();
	}
	
	bool operator==(const BigInt &eq) {
		if (size != eq.size)return false;
		if (sign != eq.sign)return false;
		for (int i = 0; i < size; i++)
			if (mem[i] != eq.mem[i])return false;
			return true;
	}
	bool operator>(const BigInt &eq) {
		if (*this == eq)return false;
		if (sign == 0 && eq.sign == 1 )return true;
		if (sign == 1 && eq.sign == 0)return false;
		if (size > eq.size)return true;
		for (int i = 0; i < size; i++)
			if (mem[i] < eq.mem[i])return false;
		return true;		
	}
	bool operator<(const BigInt &eq) {
		if (*this>eq || *this == eq)return false;
		return true;
	}
};


int main() {
	while (1) {
		cout << "Big Int Calculator:" << endl;
		string first;
		string second;
		cout << "Type first number:" << endl;
		cin >> first;
		cout << "Type second number:" << endl;
		cin >> second;
		cout << "Select operation(+,-,*,<,>,=(isequal))" << endl;
		char task;
		cin >> task;
		BigInt A(first);
		BigInt B(second);
		BigInt C(first);
		cout << endl;
		A.print();
		cout << task << endl;
		B.print();
		cout << "ans:" << endl;
		switch (task) {
		case '+':
			(A + B).print();
			break;
		case '-':
			(A - B).print();
			break;
		case '*':
			(A * B).print();
			break;
		case '<':
			cout << (A < B) << endl;
			break;
		case '>':
			cout << (A > B) << endl;
			break;
		case '=':
			cout << (A == B) << endl;
			break;
		}
	}

	return 0;
}