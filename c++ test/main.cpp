#include <iostream>
#include <string>
#include <stack>
#include <fstream>
#include <limits>  // For std::numeric_limits

using namespace std;

// Item class definition
class Item {
public:
    Item(int id, const string& name, double price, int quantity)
        : id(id), name(name), price(price), quantity(quantity) {}

    int getId() const { return id; }
    string getName() const { return name; }
    double getPrice() const { return price; }
    int getQuantity() const { return quantity; }

    void setName(const string& name) { this->name = name; }
    void setPrice(double price) { this->price = price; }
    void setQuantity(int quantity) { this->quantity = quantity; }

private:
    int id;
    string name;
    double price;
    int quantity;
};

// Node class definition
class Node {
public:
    Item item;
    Node* next;

    Node(const Item& item) : item(item), next(nullptr) {}
};

// Action class definition
enum ActionType { ADD, DELETE, EDIT };

class Action {
public:
    Action(ActionType type, const Item& item) : type(type), item(item) {}

    ActionType getType() const { return type; }
    Item getItem() const { return item; }

private:
    ActionType type;
    Item item;
};

// Inventory class definition
class Inventory {
public:
    Inventory() : head(nullptr) {}
    ~Inventory() { clear(); }

    void addItem(const Item& item) {
        Node* newNode = new Node(item);
        newNode->next = head;
        head = newNode;
        pushAction(ADD, item);
    }

    bool deleteItem(int id) {
        Node* current = head;
        Node* previous = nullptr;

        while (current != nullptr && current->item.getId() != id) {
            previous = current;
            current = current->next;
        }

        if (current == nullptr) return false; // Item not found

        Item deletedItem = current->item;

        if (previous == nullptr) {
            head = head->next; // Delete the head node
        } else {
            previous->next = current->next;
        }

        delete current;
        pushAction(DELETE, deletedItem);
        return true;
    }

    bool editItem(int id, const string& name, double price, int quantity) {
        Node* current = head;

        while (current != nullptr && current->item.getId() != id) {
            current = current->next;
        }

        if (current == nullptr) return false; // Item not found

        Item oldItem = current->item;
        current->item.setName(name);
        current->item.setPrice(price);
        current->item.setQuantity(quantity);
        pushAction(EDIT, oldItem);
        return true;
    }

    Item* searchItem(int id) {
        Node* current = head;

        while (current != nullptr) {
            if (current->item.getId() == id) {
                return &current->item;
            }
            current = current->next;
        }

        return nullptr; // Item not found
    }

    void displayItems() const {
        Node* current = head;

        if (current == nullptr) {
            cout << "Inventory is empty." << endl;
            return;
        }

        while (current != nullptr) {
            cout << "ID: " << current->item.getId()
                  << ", Name: " << current->item.getName()
                  << ", Price: $" << current->item.getPrice()
                  << ", Quantity: " << current->item.getQuantity() << endl;
            current = current->next;
        }
    }

    bool saveToFile(const string& filename) const {
        ofstream file(filename);
        if (!file.is_open()) {
            cerr << "Failed to open file for writing." << endl;
            return false;
        }

        Node* current = head;

        while (current != nullptr) {
            file << current->item.getId() << " "
                 << current->item.getName() << " "
                 << current->item.getPrice() << " "
                 << current->item.getQuantity() << "\n";
            current = current->next;
        }

        file.close();
        return true;
    }

    bool loadFromFile(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Failed to open file for reading." << endl;
            return false;
        }

        clear();
        int id, quantity;
        double price;
        string name;

        while (file >> id >> ws) {
            getline(file, name, ' ');
            file >> price >> quantity;
            addItem(Item(id, name, price, quantity));
        }

        file.close();
        return true;
    }

    void undoLastAction() {
        if (actionStack.empty()) {
            cout << "No actions to undo." << endl;
            return;
        }

        Action lastAction = actionStack.top();
        actionStack.pop();

        switch (lastAction.getType()) {
            case ADD:
                deleteItem(lastAction.getItem().getId());
                break;
            case DELETE:
                addItem(lastAction.getItem());
                break;
            case EDIT: {
                Item* item = searchItem(lastAction.getItem().getId());
                if (item != nullptr) {
                    item->setName(lastAction.getItem().getName());
                    item->setPrice(lastAction.getItem().getPrice());
                    item->setQuantity(lastAction.getItem().getQuantity());
                }
                break;
            }
        }
    }

private:
    Node* head;
    stack<Action> actionStack;

    void clear() {
        while (head != nullptr) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }

    void pushAction(ActionType type, const Item& item) {
        actionStack.push(Action(type, item));
    }
};

// Admin class definition
class Admin {
public:
    Admin(Inventory& inventory) : inventory(inventory) {}

    void addItem() {
        int id;
        string name;
        double price;
        int quantity;

        cout << "Enter item ID (or 0 to cancel): ";
        while (!(cin >> id) || id < 0) {
            cin.clear(); // Clear error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
            cout << "Invalid input. Please enter a positive numeric ID or 0 to cancel: ";
        }

        if (id == 0) return;

        cout << "Enter item name (or 0 to cancel): ";
        cin.ignore();
        getline(cin, name);
        if (name == "0") return;

        cout << "Enter item price (or 0 to cancel): ";
        while (!(cin >> price) || price < 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a positive numeric price or 0 to cancel: ";
        }
        if (price == 0) return;

        cout << "Enter item quantity (or 0 to cancel): ";
        while (!(cin >> quantity) || quantity < 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a non-negative quantity or 0 to cancel: ";
        }
        if (quantity == 0) return;

        inventory.addItem(Item(id, name, price, quantity));
        cout << "Item added successfully!" << endl;
    }

    void deleteItem() {
        int id;
        cout << "Enter item ID to delete (or 0 to cancel): ";
        while (!(cin >> id) || id < 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a numeric ID or 0 to cancel: ";
        }

        if (id == 0) return;

        if (inventory.deleteItem(id)) {
            cout << "Item deleted successfully!" << endl;
        } else {
            cout << "Item not found!" << endl;
        }
    }

    void editItem() {
        int id;
        string name;
        double price;
        int quantity;

        cout << "Enter item ID to edit (or 0 to cancel): ";
        while (!(cin >> id) || id < 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a numeric ID or 0 to cancel: ";
        }

        if (id == 0) return;

        cout << "Enter new item name (or 0 to cancel): ";
        cin.ignore();
        getline(cin, name);
        if (name == "0") return;

        cout << "Enter new item price (or 0 to cancel): ";
        while (!(cin >> price) || price < 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a positive numeric price or 0 to cancel: ";
        }
        if (price == 0) return;

        cout << "Enter new item quantity (or 0 to cancel): ";
        while (!(cin >> quantity) || quantity < 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a non-negative quantity or 0 to cancel: ";
        }
        if (quantity == 0) return;

        if (inventory.editItem(id, name, price, quantity)) {
            cout << "Item edited successfully!" << endl;
        } else {
            cout << "Item not found!" << endl;
        }
    }

    void undoLastAction() {
        inventory.undoLastAction();
    }

    Inventory& getInventory() { return inventory; }

private:
    Inventory& inventory;
};

// User class definition
class User {
public:
    User(Inventory& inventory) : inventory(inventory) {}

    void viewItems() const {
        inventory.displayItems();
    }

    void purchaseItem() {
        int id, quantity;
        cout << "Enter item ID to purchase (or 0 to cancel): ";
        while (!(cin >> id) || id < 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a numeric ID or 0 to cancel: ";
        }

        if (id == 0) return;

        cout << "Enter quantity to purchase (or 0 to cancel): ";
        while (!(cin >> quantity) || quantity < 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a non-negative quantity or 0 to cancel: ";
        }
        if (quantity == 0) return;

        Item* item = inventory.searchItem(id);

        if (item == nullptr) {
            cout << "Item not found!" << endl;
            return;
        }

        if (item->getQuantity() < quantity) {
            cout << "Insufficient quantity!" << endl;
            return;
        }

        item->setQuantity(item->getQuantity() - quantity);
        cout << "Purchase successful! Total cost: $" << item->getPrice() * quantity << endl;
    }

private:
    Inventory& inventory;
};

// Admin menu function
void adminMenu(Admin& admin) {
    int choice;
    do {
        cout << "\nAdmin Menu:\n";
        cout << "1. Add Item\n";
        cout << "2. Delete Item\n";
        cout << "3. Edit Item\n";
        cout << "4. Save Inventory\n";
        cout << "5. Load Inventory\n";
        cout << "6. Undo Last Action\n";
        cout << "7. Exit to Main Menu\n";
        cout << "Enter your choice: ";
        while (!(cin >> choice) || choice < 1 || choice > 7) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid choice! Please enter a number between 1 and 7: ";
        }

        switch (choice) {
            case 1:
                admin.addItem();
                break;
            case 2:
                admin.deleteItem();
                break;
            case 3:
                admin.editItem();
                break;
            case 4: {
                string filename;
                cout << "Enter filename to save inventory: ";
                cin >> filename;
                if (admin.getInventory().saveToFile(filename)) {
                    cout << "Inventory saved successfully!" << endl;
                } else {
                    cout << "Failed to save inventory!" << endl;
                }
                break;
            }
            case 5: {
                string filename;
                cout << "Enter filename to load inventory: ";
                cin >> filename;
                if (admin.getInventory().loadFromFile(filename)) {
                    cout << "Inventory loaded successfully!" << endl;
                } else {
                    cout << "Failed to load inventory!" << endl;
                }
                break;
            }
            case 6:
                admin.undoLastAction();
                break;
            case 7:
                cout << "Exiting Admin Menu...\n";
                break;
            default:
                cout << "Invalid choice! Please try again.\n";
        }
    } while (choice != 7);
}

// User menu function
void userMenu(User& user) {
    int choice;
    do {
        cout << "\nUser Menu:\n";
        cout << "1. View Items\n";
        cout << "2. Purchase Item\n";
        cout << "3. Exit to Main Menu\n";
        cout << "Enter your choice: ";
        while (!(cin >> choice) || choice < 1 || choice > 3) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid choice! Please enter a number between 1 and 3: ";
        }

        switch (choice) {
            case 1:
                user.viewItems();
                break;
            case 2:
                user.purchaseItem();
                break;
            case 3:
                cout << "Exiting User Menu...\n";
                break;
            default:
                cout << "Invalid choice! Please try again.\n";
        }
    } while (choice != 3);
}

// Main function
int main() {
    Inventory inventory;
    Admin admin(inventory);
    User user(inventory);

    int role;
    do {
        cout << "\nSelect Role:\n";
        cout << "1. Admin\n";
        cout << "2. User\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        while (!(cin >> role) || role < 1 || role > 3) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid choice! Please enter a number between 1 and 3: ";
        }

        switch (role) {
            case 1:
                adminMenu(admin);
                break;
            case 2:
                userMenu(user);
                break;
            case 3:
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid role! Please try again.\n";
        }
    } while (role != 3);

    return 0;
}
