#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// Class for product details
class Product {
    int pno;
    string name;
    float price, qty, tax, dis;

public:
    void create_product() {
        cout << "\nPlease Enter The Product No. of The Product: ";
        cin >> pno;
        cin.ignore();  // to ignore the newline character left by cin
        cout << "\nPlease Enter The Name of The Product: ";
        getline(cin, name);
        cout << "\nPlease Enter The Price of The Product: ";
        cin >> price;
        cout << "\nPlease Enter The Discount (%): ";
        cin >> dis;
    }

    void show_product() const {
        cout << "\nThe Product No. of The Product: " << pno;
        cout << "\nThe Name of The Product: " << name;
        cout << "\nThe Price of The Product: " << price;
        cout << "\nDiscount: " << dis;
    }

    int retpno() const { return pno; }
    float retprice() const { return price; }
    string retname() const { return name; }
    int retdis() const { return dis; }
}; 

fstream fp;
Product pr;

// Function to write in file
void write_product() {
    fp.open("Shop.dat", ios::out | ios::app);
    pr.create_product();
    fp.write(reinterpret_cast<char*>(&pr), sizeof(Product));
    fp.close();
    cout << "\n\nThe Product Has Been Created ";
}

// Function to read all records from file
void display_all() {
    cout << "\n\n\n\t\tDISPLAY ALL RECORDS!!!\n\n";
    fp.open("Shop.dat", ios::in);
    while (fp.read(reinterpret_cast<char*>(&pr), sizeof(Product))) {
        pr.show_product();
        cout << "\n\n====================================\n";
    }
    fp.close();
}

// Function to read specific record from file
void display_sp(int n) {
    int flag = 0;
    fp.open("Shop.dat", ios::in);
    while (fp.read(reinterpret_cast<char*>(&pr), sizeof(Product))) {
        if (pr.retpno() == n) {
            pr.show_product();
            flag = 1;
            break;
        }
    }
    fp.close();
    if (flag == 0)
        cout << "\n\nRecord does not exist";
}

// Function to modify record of file
void modify_product() {
    int no, found = 0;
    cout << "\n\n\tTo Modify ";
    cout << "\n\n\tPlease Enter The Product No. of The Product: ";
    cin >> no;
    fp.open("Shop.dat", ios::in | ios::out);
    while (fp.read(reinterpret_cast<char*>(&pr), sizeof(Product)) && found == 0) {
        if (pr.retpno() == no) {
            pr.show_product();
            cout << "\nPlease Enter The New Details of Product: ";
            pr.create_product();
            int pos = -1 * static_cast<int>(sizeof(pr));
            fp.seekp(pos, ios::cur);
            fp.write(reinterpret_cast<char*>(&pr), sizeof(Product));
            cout << "\n\n\t Record Updated";
            found = 1;
        }
    }
    fp.close();
    if (found == 0)
        cout << "\n\n Record Not Found ";
}

// Function to delete record of file
void delete_product() {
    int no;
    cout << "\n\n\n\tDelete Record";
    cout << "\n\nPlease Enter The Product No. of The Product You Want To Delete: ";
    cin >> no;
    fp.open("Shop.dat", ios::in | ios::out);
    fstream fp2;
    fp2.open("Temp.dat", ios::out);
    fp.seekg(0, ios::beg);
    while (fp.read(reinterpret_cast<char*>(&pr), sizeof(Product))) {
        if (pr.retpno() != no) {
            fp2.write(reinterpret_cast<char*>(&pr), sizeof(Product));
        }
    }
    fp2.close();
    fp.close();
    remove("Shop.dat");
    rename("Temp.dat", "Shop.dat");
    cout << "\n\n\tRecord Deleted ..";
}

// Function to display all products price list
void menu() {
    fp.open("Shop.dat", ios::in);
    if (!fp) {
        cout << "ERROR!!! FILE COULD NOT BE OPEN\n\n\nGo To Admin Menu to create File";
        cout << "\n\n\nProgram is closing ....";
        exit(0);
    }
    cout << "\n\n\t\tProduct MENU\n\n";
    cout << "====================================================\n";
    cout << "P.NO.\t\tNAME\t\tPRICE\n";
    cout << "====================================================\n";
    while (fp.read(reinterpret_cast<char*>(&pr), sizeof(Product))) {
        cout << pr.retpno() << "\t\t" << pr.retname() << "\t\t" << pr.retprice() << endl;
    }
    fp.close();
}

// Function to place order and generating bill for Products
void place_order() {
    int order_arr[50], quan[50], c = 0;
    float amt, damt, total = 0;
    char ch = 'Y';
    menu();
    cout << "\n============================";
    cout << "\n PLACE YOUR ORDER";
    cout << "\n============================\n";
    do {
        cout << "\n\nEnter The Product No. Of The Product: ";
        cin >> order_arr[c];
        cout << "\nQuantity in number: ";
        cin >> quan[c];
        c++;
        cout << "\nDo You Want To Order Another Product? (y/n): ";
        cin >> ch;
    } while (ch == 'y' || ch == 'Y');
    cout << "\n\nThank You For Placing The Order";
    cout << "\n\n********************************INVOICE************************\n";
    cout << "\nPr No.\tPr Name\tQuantity \tPrice \tAmount \tAmount after discount\n";
    for (int x = 0; x < c; x++) {
        fp.open("Shop.dat", ios::in);
        while (fp.read(reinterpret_cast<char*>(&pr), sizeof(Product))) {
            if (pr.retpno() == order_arr[x]) {
                amt = pr.retprice() * quan[x];
                damt = amt - (amt * pr.retdis() / 100);
                cout << "\n" << order_arr[x] << "\t" << pr.retname() << "\t" << quan[x] << "\t\t" << pr.retprice() << "\t" << amt << "\t\t" << damt;
                total += damt;
            }
        }
        fp.close();
    }
    cout << "\n\n\t\t\t\t\tTOTAL = " << total;
}

// INTRODUCTION FUNCTION
void intro() {
    cout << "\n\n\n\tSUPER MARKET";
    cout << "\n\n\tBILLING";
    cout << "\n\n\tPROJECT";
    cout << "\n\nMADE BY : ZAKEER SHAIK";
}

// ADMINISTRATOR MENU FUNCTION
void admin_menu() {
    char ch2;
    cout << "\n\n\n\tADMIN MENU";
    cout << "\n\n\t1.CREATE PRODUCT";
    cout << "\n\n\t2.DISPLAY ALL PRODUCTS";
    cout << "\n\n\t3.QUERY ";
    cout << "\n\n\t4.MODIFY PRODUCT";
    cout << "\n\n\t5.DELETE PRODUCT";
    cout << "\n\n\t6.VIEW PRODUCT MENU";
    cout << "\n\n\t7.BACK TO MAIN MENU";
    cout << "\n\n\tPlease Enter Your Choice (1-7): ";
    cin >> ch2;
    switch (ch2) {
    case '1':
        write_product();
        break;
    case '2':
        display_all();
        break;
    case '3': {
        int num;
        cout << "\n\n\tPlease Enter The Product No.: ";
        cin >> num;
        display_sp(num);
        break;
    }
    case '4':
        modify_product();
        break;
    case '5':
        delete_product();
        break;
    case '6':
        menu();
        break;
    case '7':
        break;
    default:
        cout << "\a";
        admin_menu();
    }
}

// THE MAIN FUNCTION OF PROGRAM
int main() {
    char ch;
    intro();
    do {
        cout << "\n\n\n\tMAIN MENU";
        cout << "\n\n\t01. CUSTOMER";
        cout << "\n\n\t02. ADMINISTRATOR";
        cout << "\n\n\t03. EXIT";
        cout << "\n\n\tPlease Select Your Option (1-3): ";
        cin >> ch;
        switch (ch) {
        case '1':
            place_order();
            break;
        case '2':
            admin_menu();
            break;
        case '3':
            exit(0);
        default:
            cout << "\a";
        }
    } while (ch != '3');
    return 0;
}
