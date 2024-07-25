#include <iostream>
#include <string>
using namespace std;

struct User {
    string username;
    string password;
    bool isAdmin;
    User* next;
};

User* users = nullptr;

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

void editItem(int index, const string& name, double price, int quantity) {
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
    }
}

void viewCart() {
    if (cartItemCount == 0) {
        cout << "Cart is empty." << endl;
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
        } else {
            cout << "Checkout canceled." << endl;
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
}

void userMenu() {
    int choice;
    do {
        cout << "1. View Items\n2. Buy Item\n3. View Cart\n4. Checkout\n5. Logout\nEnter choice: ";
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
                addToCart(index - 1, quantity);
            }
        } else if (choice == 3) {
            viewCart();
        } else if (choice == 4) {
            checkout();
        }
    } while (choice != 5);
}

int main() {
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
            cout << "Enter new username (or 0 to cancel): ";
            cin >> username;
            if (username == "0") continue;
            cout << "Enter new password: ";
            cin >> password;

            registerUser(username, password, false);
            cout << "User registered successfully." << endl;
        }
    } while (startChoice != 3);


    while (users != nullptr) {
        User* temp = users;
        users = users->next;
        delete temp;
    }

    return 0;
}
