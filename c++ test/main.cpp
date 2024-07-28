#include <iostream>
#include <chrono>
#include <thread>
#include <string>
#include <cstdlib>
#include <iomanip>

using namespace std;

struct User {
    string username;
    string password;
    bool isAdmin;
    User* next;
};

User* users = nullptr;

void clearScreen() {
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

bool loginUser(const string& username, const string& password, bool& isAdmin) {
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

struct Item {
    string name;
    double price;
    int quantity;
    Item* next;
};

Item* inventoryHead = nullptr;

struct CartItemNode {
    Item item;
    int quantity;
    CartItemNode* next;
};

CartItemNode* cartHead = nullptr;

void addItem(const string& name, double price, int quantity) {
    Item* newItem = new Item();
    newItem->name = name;
    newItem->price = price;
    newItem->quantity = quantity;
    newItem->next = inventoryHead;
    inventoryHead = newItem;
}

void viewItems() {
    if (inventoryHead == nullptr) {
        cout << "No items in the inventory." << endl;
        clearScreen();
    } else {
        cout << "===========================================================\n";
        cout << "                    INVENTORY MENU\n";
        cout << "===========================================================\n";
        cout << setw(5) << left << "No."
             << setw(25) << left << "Item Name"
             << setw(10) << left << "Price"
             << setw(10) << left << "Quantity"
             << endl;
        cout << "-----------------------------------------------------------\n";

        Item* current = inventoryHead;
        int index = 1;
        while (current != nullptr) {
            cout << setw(5) << left << index
                 << setw(25) << left << current->name
                 << "$" << setw(9) << left << fixed << setprecision(2) << current->price
                 << setw(10) << left << current->quantity
                 << endl;
            current = current->next;
            index++;
        }
        cout << "===========================================================\n";
    }
}

void editItem(int index, const string& name, double price, int quantity) {
    Item* current = inventoryHead;
    int currentIndex = 0;
    while (current != nullptr && currentIndex < index) {
        current = current->next;
        currentIndex++;
    }
    if (current != nullptr) {
        current->name = name;
        current->price = price;
        current->quantity = quantity;
    } else {
        cout << "Invalid item index." << endl;
        clearScreen();
    }
}

void deleteItem(int index) {
    if (inventoryHead == nullptr) {
        cout << "No items to delete." << endl;
        clearScreen();
        return;
    }
    if (index == 0) {
        Item* temp = inventoryHead;
        inventoryHead = inventoryHead->next;
        delete temp;
    } else {
        Item* current = inventoryHead;
        int currentIndex = 0;
        while (current->next != nullptr && currentIndex < index - 1) {
            current = current->next;
            currentIndex++;
        }
        if (current->next != nullptr) {
            Item* temp = current->next;
            current->next = current->next->next;
            delete temp;
        } else {
            cout << "Invalid item index." << endl;
            clearScreen(); 
        }
    }
}

void addToCart(int index, int quantity) {
    Item* current = inventoryHead;
    int currentIndex = 0;
    while (current != nullptr && currentIndex < index) {
        current = current->next;
        currentIndex++;
    }
    if (current != nullptr && quantity > 0 && current->quantity >= quantity) {
        current->quantity -= quantity;
        CartItemNode* newCartItem = new CartItemNode();
        newCartItem->item = *current;
        newCartItem->quantity = quantity;
        newCartItem->next = cartHead;
        cartHead = newCartItem;
        cout << "Added " << quantity << " of " << current->name << " to the cart." << endl;
        cout << "\nPress ENTER to return to the user menu.";
            cin.ignore();
            cin.get();
            clearScreen();
    } else {
        cout << "Invalid item index or insufficient quantity." << endl;
        clearScreen();
    }
}

void viewCart() {
    if (cartHead == nullptr) {
        cout << "Cart is empty." << endl;
        clearScreen();
    } else {
        cout << "=================================================\n";
        cout << "                ITEMS IN THE CART\n";
        cout << "=================================================\n";
        cout << "\n";
        CartItemNode* current = cartHead;
        int index = 1;
        while (current != nullptr) {
            cout << index << ". " << current->item.name << " - $" << current->item.price << " - Quantity: " << current->quantity << endl;
            cout << "\n";
            current = current->next;
            index++;
        }
    }
}

void removeFromCart(int index) {
    if (cartHead == nullptr) {
        cout << "Cart is empty. Nothing to remove." << endl;
        clearScreen();
        return;
    }

    if (index == 0) {
        CartItemNode* temp = cartHead;
        cartHead = cartHead->next;
        delete temp;
    } else {
        CartItemNode* current = cartHead;
        int currentIndex = 0;
        while (current->next != nullptr && currentIndex < index - 1) {
            current = current->next;
            currentIndex++;
        }

        if (current->next != nullptr) {
            CartItemNode* temp = current->next;
            current->next = current->next->next;
            delete temp;
        } else {
            cout << "Invalid cart item index." << endl;
            clearScreen();
        }
    }
}

void checkout() {
    if (cartHead == nullptr) {
        cout << "Cart is empty. Nothing to checkout." << endl;
        clearScreen();
    } else {
        double total = 0;
        CartItemNode* current = cartHead;
        while (current != nullptr) {
            total += current->item.price * current->quantity;
            current = current->next;
        }
        cout << "Total amount to pay: $" << total << endl;
        cout << "Confirm checkout? (y/n): ";
        char confirmation;
        cin >> confirmation;
        if (confirmation == 'y' || confirmation == 'Y') {
            while (cartHead != nullptr) {
                CartItemNode* temp = cartHead;
                cartHead = cartHead->next;
                delete temp;
            }
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
        cout << "=================================================\n";
        cout << "                   ADMIN MENU\n";
        cout << "=================================================\n";
        cout << "\n";
        cout << "1. Add Item\n2. View Items\n3. Edit Item\n4. Delete Item\n5. Logout\nEnter choice: ";
        cin >> choice;
        if (choice == 1) {
            clearScreen();
            string name;
            double price;
            int quantity;
            cout << "Enter item name (or 0 to cancel): ";
            cin.ignore();
            getline(cin, name);
            if (name == "0")  {
                clearScreen();
                continue;
            }
            cout << "Enter item price: ";
            cin >> price;
            cout << "Enter item quantity: ";
            cin >> quantity;
            addItem(name, price, quantity);
            cout << "Item added to inventory.";
            clearScreen();
        } else if (choice == 2) {
            clearScreen();
            viewItems();
            cout << "\nPress ENTER to return to the user menu.";
            cin.ignore();
            cin.get();
            clearScreen();
        } else if (choice == 3) {
            clearScreen();
            int index;
            string name;
            double price;
            int quantity;
            viewItems();
            cout << endl;
            cout << "Enter item index to edit: ";
            cin >> index;
            cout << "Enter new item name (or 0 to cancel): ";
            cin.ignore();
            getline(cin, name);
            if (name == "0") {
                clearScreen();
                continue;
            }
            cout << "Enter new item price: ";
            cin >> price;
            cout << "Enter new item quantity: ";
            cin >> quantity;
            editItem(index - 1, name, price, quantity);
            cout << "Item successfully changed.";
            clearScreen();
        } else if (choice == 4) {
            clearScreen();
            int index;
            viewItems();
            cout << endl;
            cout << "Enter item index to delete (or 0 to cancel): ";
            cin >> index;
            if (index == 0) {
                clearScreen();
                continue;
            }
            deleteItem(index - 1);
            cout << "Item deleted.";
            clearScreen();
        }
    } while (choice != 5);
    cout << "Logging out. . .";
    clearScreen();
}

void userMenu() {
    int choice;
    do {
        cout << "=================================================\n";
        cout << "                    USER MENU\n";
        cout << "=================================================\n";
        cout << "\n";
        cout << "1. View Items\n2. Buy Item\n3. View Cart\n4. Remove from Cart\n5. Checkout\n6. Logout\nEnter choice: ";
        cin >> choice;
        if (choice == 1) {
            clearScreen();
            viewItems();
            cout << "\nPress ENTER to return to the user menu.";
            cin.ignore();
            cin.get();
            clearScreen();
        } else if (choice == 2) {
            if (inventoryHead == nullptr) {
                cout << "No items available to buy." << endl;
                clearScreen();
            } else {
                clearScreen();
                viewItems();
                int index, quantity;
                cout << "\nEnter item index to buy: ";
                cin >> index;
                cout << "Enter quantity to buy: ";
                cin >> quantity;
                addToCart(index - 1, quantity);
            }
        } else if (choice == 3) {
            clearScreen();
            viewCart();
        } else if (choice == 4) {
            clearScreen();
            viewCart();
            int index;
            cout << "Enter item index to remove from cart: ";
            cin >> index;
            removeFromCart(index - 1);
            clearScreen();
        } else if (choice == 5) {
            clearScreen();
            checkout();
        }
    } while (choice != 6);
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
    clearScreen();
    registerUser("admin", "admin123", true);
    addDefaultItems();

    int startChoice;
    do {
        cout << "=================================================\n";
        cout << "                  WELCOME MENU\n";
        cout << "=================================================\n";
        cout << "\n";
        cout << "1. Login\n2. Register\n3. Exit\nEnter choice: ";
        cin >> startChoice;
        if (startChoice == 1) {
            clearScreen();
            string username, password;
            bool isAdmin;
            cout << "Enter username: ";
            cin >> username;
            cout << "Enter password: ";
            cin >> password;

            if (loginUser(username, password, isAdmin)) {
                if (isAdmin) {
                    cout << "Admin logged in successfully . . .";
                    clearScreen();
                    adminMenu();
                } else {
                    cout << "Logged in successfully . . .";
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
    cout << "Thank you for your patronage!";
    clearScreen();

    while (users != nullptr) {
        User* temp = users;
        users = users->next;
        delete temp;
    }

    while (inventoryHead != nullptr) {
        Item* temp = inventoryHead;
        inventoryHead = inventoryHead->next;
        delete temp;
    }

    while (cartHead != nullptr) {
        CartItemNode* temp = cartHead;
        cartHead = cartHead->next;
        delete temp;
    }

    return 0;
}
