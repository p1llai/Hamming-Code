#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

// Function to calculate the number of parity bits needed
int calculateParityBits(int m) {
    int r = 0;
    while (pow(2, r) < (m + r + 1)) {
        r++;
    }
    return r;
}

// Function to calculate the parity bits
void calculateParityBits(const vector<int>& data, vector<int>& parity, int r) {
    int n = data.size() + r; // Total number of bits in codeword

    for (int i = 0; i < r; i++) {
        int parityBit = 0;
        for (int j = 0; j < n; j++) {
            if (j & (1 << i)) { // Check if the j-th bit is included in the parity calculation
                parityBit ^= data[j - __builtin_popcount(j)]; // Data bit
            }
        }
        parity[i] = parityBit;
    }
}

// Function to encode data
void encodeHamming(const vector<int>& data, vector<int>& code) {
    int m = data.size();
    int r = calculateParityBits(m);

    // Fill in the codeword
    int j = 0, k = 0;
    for (int i = 0; i < (m + r); i++) {
        if (i == (1 << j) - 1) {
            code[i] = 0; // Placeholder for parity bit
            j++;
        } else {
            code[i] = data[k++];
        }
    }

    vector<int> parity(r);
    calculateParityBits(code, parity, r);

    // Assign calculated parity bits to their positions
    for (int i = 0; i < r; i++) {
        code[(1 << i) - 1] = parity[i];
    }
}

// Function to detect and correct errors
void detectAndCorrect(vector<int>& code) {
    int r = 0;
    while (pow(2, r) < code.size()) {
        r++;
    }

    int errorPos = 0;
    vector<int> parity(r);
    calculateParityBits(code, parity, r);

    // Check for errors
    for (int i = 0; i < r; i++) {
        if (parity[i] != code[(1 << i) - 1]) {
            errorPos += (1 << i);
        }
    }

    if (errorPos > 0) {
        cout << "Error detected at position: " << errorPos << endl;
        code[errorPos - 1] ^= 1; // Flip the bit
    } else {
        cout << "No error detected." << endl;
    }

    cout << "Corrected codeword: ";
    for (int bit : code) {
        cout << bit;
    }
    cout << endl;
}

int main() {
    int m;
    cout << "Enter the number of data bits: ";
    cin >> m;

    vector<int> data(m);
    vector<int> code(m + calculateParityBits(m)); // Codeword will have data bits + parity bits

    cout << "Enter " << m << " bits of data (0 or 1): ";
    for (int i = 0; i < m; i++) {
        cin >> data[i];
    }

    // Encode data using Hamming code
    encodeHamming(data, code);

    cout << "Hamming code: ";
    for (int bit : code) {
        cout << bit;
    }
    cout << endl;

    vector<int> receivedCode(code.size());
    cout << "Enter the received codeword (" << code.size() << " bits): \n";
    for (int i = 0; i < code.size(); i++) {
        cin >> receivedCode[i];
    }

    detectAndCorrect(receivedCode);

    return 0;
}
