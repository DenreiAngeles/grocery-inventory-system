#include <iostream>
#include <chrono>
#include <thread>
#include <string>
#include <cstdlib>

using namespace std;

struct User {
    string username;
    string password;
    bool isAdmin;
    User* next;
};

User* users = nullptr;

void clearScreen(){
    std::this_thread::sleep_for(chrono::seconds(2));
    cout << "\033[2J\033[1;1H";
}

void registerUser(const string& username, const string& password, bool isAdmin) {
    User* newUser = new User();
    newUser->username = username;
    newUser->password = password;
    newUser->isAdmin = isAdmin;
    newUser->next = users;
    users = newUser;
}

bool loginUser(const string& username, const string& password, bool &isAdmin) {
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
const int MAX_CART_ITEMS = 100;

struct Item {
    string name;
    double price;
    int quantity;
};

Item inventory[MAX_ITEMS];
int itemCount = 0;

struct CartItem {
    Item item;
    int quantity;
};

CartItem cart[MAX_CART_ITEMS];
int cartItemCount = 0;

void addItem(const string& name, double price, int quantity) {
    if (itemCount < MAX_ITEMS) {
        inventory[itemCount].name = name;
        inventory[itemCount].price = price;
        inventory[itemCount].quantity = quantity;
        itemCount++;
    } else {
        cout << "Inventory full, cannot add more items." << endl;
        clearScreen();
    }
}

void viewItems() {
    if (itemCount == 0) {
        cout << "No items in the inventory." << endl;
        clearScreen();
    } else {
        for (int i = 0; i < itemCount; i++) {
            cout << i + 1 << ". " << inventory[i].name << " - $" << inventory[i].price << " - Quantity: " << inventory[i].quantity << endl;
        }
    }
}

void editItem(int index, const string& name, double price, int quantity) {
    if (index >= 0 && index < itemCount) {
        inventory[index].name = name;
        inventory[index].price = price;
        inventory[index].quantity = quantity;
    } else {
        cout << "Invalid item index." << endl;
        clearScreen();
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
        clearScreen();
    }
}

void addToCart(int index, int quantity) {
    if (index >= 0 && index < itemCount && quantity > 0 && inventory[index].quantity >= quantity) {
        inventory[index].quantity -= quantity;
        bool found = false;
        for (int i = 0; i < cartItemCount; ++i) {
            if (cart[i].item.name == inventory[index].name) {
                cart[i].quantity += quantity;
                found = true;
                break;
            }
        }
        if (!found && cartItemCount < MAX_CART_ITEMS) {
            cart[cartItemCount++] = {inventory[index], quantity};
        }
        cout << "Added " << quantity << " of " << inventory[index].name << " to the cart." << endl;
    } else {
        cout << "Invalid item index or insufficient quantity." << endl;
        clearScreen();
    }
}

void viewCart() {
    if (cartItemCount == 0) {
        cout << "Cart is empty." << endl;
        clearScreen();
    } else {
        cout << "Items in the cart:" << endl;
        for (int i = 0; i < cartItemCount; ++i) {
            cout << i + 1 << ". " << cart[i].item.name << " - $" << cart[i].item.price << " - Quantity: " << cart[i].quantity << endl;
        }
    }
}

void checkout() {
    if (cartItemCount == 0) {
        cout << "Cart is empty. Nothing to checkout." << endl;
        clearScreen();
    } else {
        double total = 0;
        for (int i = 0; i < cartItemCount; ++i) {
            total += cart[i].item.price * cart[i].quantity;
        }
        cout << "Total amount to pay: $" << total << endl;
        cout << "Confirm checkout? (y/n): ";
        char confirmation;
        cin >> confirmation;
        if (confirmation == 'y' || confirmation == 'Y') {
            cartItemCount = 0; // Clear cart after checkout
            cout << "Checked out successfully. Thank you for your purchase!" << endl;
            clearScreen();
        } else {
            cout << "Checkout canceled." << endl;
            clearScreen();
        }
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
            cout << "Enter item name (or 0 to cancel): ";
            cin.ignore();
            getline(cin, name);
            if (name == "0") continue;
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
            cout << "Enter new item name (or 0 to cancel): ";
            cin.ignore();
            getline(cin, name);
            if (name == "0") continue;
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
    clearScreen();
}

void userMenu() {
    int choice;
    do {
        cout << "1. View Items\n2. Buy Item\n3. View Cart\n4. Checkout\n5. Logout\nEnter choice: ";
        cin >> choice;
        if (choice == 1) {
            clearScreen();
            viewItems();
            cout << "\nPress ENTER to return to the user menu.";
            cin.ignore();
            cin.get();
            clearScreen();
        } else if (choice == 2) {
            if (itemCount == 0) {
                cout << "No items available to buy." << endl;
                clearScreen();
            } else {
                clearScreen();
                viewItems();
                int index, quantity;
                cout << "\nEnter item index to buy: ";
                cin >> index;
                clearScreen();
                cout << "Enter quantity to buy: ";
                cin >> quantity;
                addToCart(index - 1, quantity);
            }
        } else if (choice == 3) {
            clearScreen();
            viewCart();
        } else if (choice == 4) {
            clearScreen();
            checkout();
        }
    } while (choice != 5);
    cout << "Logging out . . .";
    clearScreen();
}

void addDefaultItems() {
    addItem("Apple", 1.0, 50);
    addItem("Banana", 0.5, 100);
    addItem("Orange", 0.8, 75);
    addItem("Milk", 1.5, 30);
    addItem("Bread", 2.0, 25);
}

int main() {
    registerUser("admin", "admin123", true);
    addDefaultItems();

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
                    clearScreen();
                    adminMenu();
                } else {
                    cout << "Logged in succesfully . . .";
                    clearScreen();
                    userMenu();
                }
            } else {
                cout << "Invalid login credentials." << endl;
                clearScreen();
            }
        } else if (startChoice == 2) {
            string username, password;
            clearScreen();
            cout << "Enter new username (or 0 to cancel): ";
            cin >> username;
            if (username == "0") continue;
            cout << "Enter new password: ";
            cin >> password;

            registerUser(username, password, false);
            cout << "User registered successfully." << endl;
            clearScreen();
        }
    } while (startChoice != 3);


    while (users != nullptr) {
        User* temp = users;
        users = users->next;
        delete temp;
    }

    return 0;
}
