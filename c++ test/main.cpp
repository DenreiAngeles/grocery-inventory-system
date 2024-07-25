#include <iostream>
#include <string>
using namespace std;

struct User {
    string username;
    string password;
    bool isAdmin;
    User* next;
};

User* users = nullptr; // Linked list head

void registerUser(string username, string password, bool isAdmin) {
    User* newUser = new User();
    newUser->username = username;
    newUser->password = password;
    newUser->isAdmin = isAdmin;
    newUser->next = users;
    users = newUser;
}

bool loginUser(string username, string password, bool &isAdmin) {
    User* current = users;
    while (current != nullptr) {
        if (current->username == username && current->password == password) {
            isAdmin = current->isAdmin;
            return true;
        }
        current = current->next;
    }
    return false;
}

const int MAX_ITEMS = 100;

struct Item {
    string name;
    double price;
    int quantity;
};

Item inventory[MAX_ITEMS];
int itemCount = 0;

void addItem(string name, double price, int quantity) {
    if (itemCount < MAX_ITEMS) {
        inventory[itemCount].name = name;
        inventory[itemCount].price = price;
        inventory[itemCount].quantity = quantity;
        itemCount++;
    } else {
        cout << "Inventory full, cannot add more items." << endl;
    }
}

void viewItems() {
    if (itemCount == 0) {
        cout << "No items in the inventory." << endl;
    } else {
        for (int i = 0; i < itemCount; i++) {
            cout << i + 1 << ". " << inventory[i].name << " - $" << inventory[i].price << " - Quantity: " << inventory[i].quantity << endl;
        }
    }
}

void editItem(int index, string name, double price, int quantity) {
    if (index >= 0 && index < itemCount) {
        inventory[index].name = name;
        inventory[index].price = price;
        inventory[index].quantity = quantity;
    } else {
        cout << "Invalid item index." << endl;
    }
}

void deleteItem(int index) {
    if (index >= 0 && index < itemCount) {
        for (int i = index; i < itemCount - 1; i++) {
            inventory[i] = inventory[i + 1];
        }
        itemCount--;
    } else {
        cout << "Invalid item index." << endl;
    }
}

void adminMenu() {
    int choice;
    do {
        cout << "1. Add Item\n2. View Items\n3. Edit Item\n4. Delete Item\n5. Logout\nEnter choice: ";
        cin >> choice;
        if (choice == 1) {
            string name;
            double price;
            int quantity;
            cout << "Enter item name: ";
            cin >> name;
            cout << "Enter item price: ";
            cin >> price;
            cout << "Enter item quantity: ";
            cin >> quantity;
            addItem(name, price, quantity);
        } else if (choice == 2) {
            viewItems();
        } else if (choice == 3) {
            int index;
            string name;
            double price;
            int quantity;
            cout << "Enter item index to edit: ";
            cin >> index;
            cout << "Enter new item name: ";
            cin >> name;
            cout << "Enter new item price: ";
            cin >> price;
            cout << "Enter new item quantity: ";
            cin >> quantity;
            editItem(index - 1, name, price, quantity);
        } else if (choice == 4) {
            int index;
            cout << "Enter item index to delete: ";
            cin >> index;
            deleteItem(index - 1);
        }
    } while (choice != 5);
}

void userMenu() {
    int choice;
    do {
        cout << "1. View Items\n2. Buy Item\n3. Logout\nEnter choice: ";
        cin >> choice;
        if (choice == 1) {
            viewItems();
        } else if (choice == 2) {
            if (itemCount == 0) {
                cout << "No items available to buy." << endl;
            } else {
                int index, quantity;
                cout << "Enter item index to buy: ";
                cin >> index;
                cout << "Enter quantity to buy: ";
                cin >> quantity;
                if (index > 0 && index <= itemCount && inventory[index - 1].quantity >= quantity) {
                    inventory[index - 1].quantity -= quantity;
                    cout << "Purchased " << quantity << " of " << inventory[index - 1].name << endl;
                } else {
                    cout << "Invalid item index or insufficient quantity." << endl;
                }
            }
        }
    } while (choice != 3);
}

int main() {
    // Register a default admin
    registerUser("admin", "admin123", true);

    int startChoice;
    do {
        cout << "1. Login\n2. Register\n3. Exit\nEnter choice: ";
        cin >> startChoice;
        if (startChoice == 1) {
            string username, password;
            bool isAdmin;
            cout << "Enter username: ";
            cin >> username;
            cout << "Enter password: ";
            cin >> password;

            if (loginUser(username, password, isAdmin)) {
                if (isAdmin) {
                    adminMenu();
                } else {
                    userMenu();
                }
            } else {
                cout << "Invalid login credentials." << endl;
            }
        } else if (startChoice == 2) {
            string username, password;
            cout << "Enter new username: ";
            cin >> username;
            cout << "Enter new password: ";
            cin >> password;

            registerUser(username, password, false);
            cout << "User registered successfully." << endl;
        }
    } while (startChoice != 3);

    return 0;
}
