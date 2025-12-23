#include "Figures.h"
#include <iostream>
#include <iomanip>

using namespace std;

int main() {
    Array arr;
    int choice;

    do {
        cout << "1. Add Octagon\n";
        cout << "2. Add Triangle\n";
        cout << "3. Add Square\n";
        cout << "4. Print all figures\n";
        cout << "5. Show total area\n";
        cout << "6. Remove figure by index\n";
        cout << "7. Show number of figures\n";
        cout << "0. Exit\n";
        cout << "\n";
        cout << "Сhoice: ";
        cin >> choice;

        try {
            switch (choice) {
                case 1: {
                    auto fig = make_unique<Octagon>();
                    cout << "Enter side length for Octagon: ";
                    cin >> *fig;
                    arr.add(move(fig));
                    cout << "Octagon added successfully!\n";
                    break;
                }
                case 2: {
                    auto fig = make_unique<Triangle>();
                    cout << "Enter side length for Triangle: ";
                    cin >> *fig;
                    arr.add(move(fig));
                    cout << "Triangle added successfully!\n";
                    break;
                }
                case 3: {
                    auto fig = make_unique<Square>();
                    cout << "Enter side length for Square: ";
                    cin >> *fig;
                    arr.add(move(fig));
                    cout << "Square added successfully!\n";
                    break;
                }
                case 4:
                    if (arr.getSize() == 0) {
                        cout << "No figures in the array.\n";
                    } else {
                        cout << "\nFigures in total)\n";
                        arr.printAll();
                    }
                    break;
                case 5:
                    cout << "Total area of all figures: " 
                              << fixed << setprecision(2) 
                              << arr.totalArea() << endl;
                    break;
                case 6: {
                    if (arr.getSize() == 0) {
                        cout << "No figures to remove.\n";
                        break;
                    }
                    cout << "Enter index to remove" << endl;
                    size_t idx;
                    cin >> idx;
                    if (idx < arr.getSize()) {
                        arr.remove(idx);
                        cout << "Figure at index " << idx << " removed!\n";
                    } else {
                        cout << "Invalid index!\n";
                    }
                    break;
                }
                case 7:
                    cout << "Number of figures in array: " << arr.getSize() << endl;
                    break;
                case 0:
                    cout << "Exiting program...\n";
                    break;
                default:
                    cout << "Invalid choice! Please try again.\n";
            }
        } catch (const exception& e) {
            cerr << "Error" << endl;
        }

    } while (choice != 0);

    return 0;
}