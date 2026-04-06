#include <iostream>
#include <cmath>
#include <vector>
#include <string>
using namespace std;

// Functions
double add(double a, double b) { return a + b; }
double sub(double a, double b) { return a - b; }
double mul(double a, double b) { return a * b; }

double divide(double a, double b) {
    if(b == 0) {
        cout << "Error: Division by zero!\n";
        return 0;
    }
    return a / b;
}

int main() {
    double x, y;
    int choice;
    vector<string> history; // 📜 store operations

    do {
        cout << "\n==== CALCULATOR ====\n";
        cout << "1. Add\n2. Subtract\n3. Multiply\n4. Divide\n5. Show History\n6. Exit\n";
        cout << "Choose: ";
        cin >> choice;

        if(choice >= 1 && choice <= 4) {
            cout << "Enter two numbers: ";
            cin >> x >> y;
        }

        switch(choice) {
            case 1: {
                double result = add(x, y);
                cout << "Result = " << result << endl;

                history.push_back(to_string(x) + " + " + to_string(y) + " = " + to_string(result));
                break;
            }

            case 2: {
                double result = sub(x, y);
                cout << "Result = " << result << endl;

                history.push_back(to_string(x) + " - " + to_string(y) + " = " + to_string(result));
                break;
            }

            case 3: {
                double result = mul(x, y);
                cout << "Result = " << result << endl;

                history.push_back(to_string(x) + " * " + to_string(y) + " = " + to_string(result));
                break;
            }

            case 4: {
                double result = divide(x, y);
                cout << "Result = " << result << endl;

                history.push_back(to_string(x) + " / " + to_string(y) + " = " + to_string(result));
                break;
            }

            case 5:
                cout << "\n--- History ---\n";
                if(history.empty()) {
                    cout << "No operations yet.\n";
                } else {
                    for(string h : history) {
                        cout << h << endl;
                    }
                }
                break;

            case 6:
                cout << "Goodbye!\n";
                break;

            default:
                cout << "Invalid choice!\n";
        }

    } while(choice != 6);

    return 0;
}