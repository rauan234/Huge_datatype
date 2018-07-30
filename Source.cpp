#include <algorithm>
#include <iostream>
#include <conio.h>
#include <string>

using namespace std;



#define byte unsigned char
#define ush  unsigned short
#define uint unsigned int

#define Digits "0123456789abcdef"



class Huge {
private:
	void* value;

	byte* get(uint ind) {
		return (static_cast<byte*>(value) + ind);
	}

public:
	bool initalized = 0;
	uint length = 0;

	Huge() {
		set(1);
	}
	Huge(int newlen) {
		set(newlen);
	}

	void operator=(Huge other) {
		if (!other.initalized) {
			throw std::runtime_error("Uninitalized Huge used");
		}

		length = other.length;


		value = new byte[length];
		for (uint q = 0; q < length; q++) {
			static_cast<byte*>(value)[q] = static_cast<byte*>(other.value)[q];
		}
	}
	template<typename datatype>
	void operator=(datatype other) {
		if (sizeof(other) > length) {
			set(sizeof(other));
		}
		*static_cast<datatype*>(value) = other;
	}
	void operator+=(Huge& other) {
		*this = this->operator+(other);
	}
	void operator*=(Huge& other) {
		*this = this->operator*(other);
	}
	template<typename datatype>
	void operator*=(datatype num) {
		*this = this->operator*(num);
	}

	bool operator==(Huge& other) {
		if (length != other.length) {
			return 0;
		}

		for (uint q = 0; q < length; q++) {
			if (static_cast<byte*>(value)[q] != static_cast<byte*>(other.value)[q]) {
				return 0;
			}
		}

		return 1;
	}
	bool operator!=(Huge& other) {
		return !this->operator==(other);
	}
	bool operator>(Huge& other) {
		if (length != other.length) {
			return 0;
		}

		for (uint q = 0; q < length; q++) {
			if (static_cast<byte*>(value)[length - 1 - q] > static_cast<byte*>(other.value)[length - 1 - q]) {
				return 1;
			}
			if (static_cast<byte*>(value)[length - 1 - q] < static_cast<byte*>(other.value)[length - 1 - q]) {
				return 0;
			}
		}

		return 0;
	}
	bool operator<(Huge& other) {
		if (length != other.length) {
			return 0;
		}

		for (uint q = 0; q < length; q++) {
			if (static_cast<byte*>(value)[length - 1 - q] < static_cast<byte*>(other.value)[length - 1 - q]) {
				return 1;
			}
			if (static_cast<byte*>(value)[length - 1 - q] > static_cast<byte*>(other.value)[length - 1 - q]) {
				return 0;
			}
		}

		return 0;
	}
	template<typename datatype>
	bool operator==(datatype num) {
		if (sizeof(num) > length) {
			set(sizeof(num));
		}

		return (*static_cast<datatype*>(value) == num);
	}
	template<typename datatype>
	bool operator!=(datatype num) {
		return !this->operator==(num);
	}
	template<typename datatype>
	bool operator>(datatype num) {
		if (sizeof(num) > length) {
			throw std::invalid_argument("Too big comparison value");
		}

		return (*static_cast<datatype*>(value) > num);
	}
	template<typename datatype>
	bool operator<(datatype num) {
		if (sizeof(num) > length) {
			throw std::invalid_argument("Too big comparison value");
		}

		return (*static_cast<datatype*>(value) < num);
	}
	Huge operator+(Huge& other) {
		Huge out(max(length, other.length));

		if (other.length > length) {
			set(other.length);
		}
		if (other.length < length) {
			other.set(length);
		}

		ush sum;
		for (uint q = 0; q < length; q++) {
			sum = *get(q) + *other.get(q);

			if (sum < 256) {
				*out.get(q) += sum;
			}
			else {
				cout << sum << endl;
				if (q == length - 1) { // If there`s no place left, 'out' reset required
					out.set(out.length + 1);
				}

				*out.get(q) += sum & 255; // 255 is 11111111 in decimal
				*out.get(q + 1) += sum >> 8;  // sum >> 8 means to cut unrequired bits
			}
		}

		return out;
	}
	Huge operator*(Huge& other) {
		Huge out;

		Huge dlt;
		dlt = (*this);
		for (byte q = 0; q < other.length * 8; q++) {
			if (((*other.get(q / 8)) >> q % 8) & 1) {
				out += dlt;
			}
			dlt.shift();
		}

		return out;
	}
	template<typename datatype>
	Huge operator*(datatype num) {
		Huge out;

		Huge dlt;
		dlt = *this;
		for (byte q = 0; q < sizeof(num) * 8; q++) {
			if ((num >> q) & 1) {
				out += dlt;
			}
			dlt.shift();
		}

		return out;
	}

	void set(int newlen) {
		void* newval;

		if (initalized) {
			newval = new byte[newlen];
			for (uint q = 0; q < length; q++) {
				static_cast<byte*>(newval)[q] = static_cast<byte*>(value)[q];
			}
			for (uint q = length; q < newlen; q++) {
				static_cast<byte*>(newval)[q] = 0;
			}

			delete value;
			value = newval;
		}
		else {
			value = new byte[newlen];
			for (uint q = 0; q < newlen; q++) {
				static_cast<byte*>(value)[q] = 0;
			}
			initalized = 1;
		}

		length = newlen;
	}
	void shift() {
		bool last, oldlast;

		oldlast = 0;
		for (uint q = 0; q < length; q++) {
			last = (*get(q) >> 7);

			*get(q) = *get(q) << 1;
			*get(q) += oldlast;

			oldlast = last;
		}
		if (last) {
			set(length + 1);
			*get(length - 1) = 1;
		}
	}
	std::string str() {
		if (!initalized) {
			throw std::runtime_error("Uninitalized Huge used");
		}

		string out;

		for (uint q = 0; q < length; q++) {
			out += Digits[(static_cast<byte*>(value)[length - 1 - q] & 240) >> 4]; // 240 is 11110000 in decimal system
			out += Digits[static_cast<byte*>(value)[length - 1 - q] & 15];         // 15  is 00001111 in decimal system
		}

		return out;
	}
};



int main() {
	Huge A;
	Huge B;

	A = 1000;
	B = 12;

	cout << (A * B).str() << endl;

	cout << endl << "Finish" << endl;
	_getch();
	return 0;
}