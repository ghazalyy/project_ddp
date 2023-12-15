#include <iostream>
#include <fstream>
#include <cstring>
#include <stdlib.h>
#include <time.h>

using namespace std;

struct Book
{
    int id;
    char title[100];
    char author[100];
    int quantity;
    string category;
    Book *next;
};

class User
{
private:
    string username;
    string password;

public:
    User(string uname, string pwd) : username(uname), password(pwd) {}

    bool authenticate(string uname, string pwd)
    {
        return (username == uname && password == pwd);
    }
};

class Library
{
private:
    Book *head;
    User *user;

    struct Transaction
    {
        int bookID;
        string borrowerName;
        int quantity;
        string borrowDate;
        string returnDate;
        Transaction *next;
    };
    Transaction *transactionsHead;

public:
    Library() : head(NULL), transactionsHead(NULL)
    {
        user = new User("admin", "password123");
        loadBooksFromFile();
    }

    void login()
    {
        string username, password;
        cout << "Masukkan username: ";
        cin >> username;
        cout << "Masukkan password: ";
        cin >> password;

        if (user->authenticate(username, password))
        {
            cout << "Login berhasil!\n";
        }
        else
        {
            cout << "Username atau password salah. Silakan coba lagi.\n";
            login();
        }
    }

    void addBook(int id, const char *title, const char *author, int quantity, const string &category)
    {
        Book *newBook = new Book;
        newBook->id = id;
        strcpy(newBook->title, title);
        strcpy(newBook->author, author);
        newBook->quantity = quantity;
        newBook->category = category;
        newBook->next = head;
        head = newBook;
        cout << "Buku berhasil ditambahkan.\n";
    }

    void displayBooks()
    {
        if (!head)
        {
            cout << "Tidak ada buku.\n";
            return;
        }

        Book *current = head;
        std::cout << "Daftar Buku:\n";
        while (current)
        {
            cout << "ID: " << current->id << "\n";
            cout << "Judul: " << current->title << "\n";
            cout << "Penulis: " << current->author << "\n";
            cout << "Kuantitas: " << current->quantity << "\n\n";
            current = current->next;
        }
    }

    void searchBook(int id)
    {
        Book *current = head;
        while (current)
        {
            if (current->id == id)
            {
                cout << "ID: " << current->id << "\n";
                cout << "Judul: " << current->title << "\n";
                cout << "Penulis: " << current->author << "\n";
                cout << "Kuantitas: " << current->quantity << "\n";
                return;
            }
            current = current->next;
        }
        cout << "Buku tidak ditemukan.\n";
    }

    void deleteBook(int id)
    {
        Book *current = head;
        Book *prev = NULL;

        while (current && current->id != id)
        {
            prev = current;
            current = current->next;
        }

        if (current)
        {
            if (prev)
            {
                prev->next = current->next;
            }
            else
            {
                head = current->next;
            }
            delete current;
            cout << "Buku berhasil dihapus.\n";
        }
        else
        {
            cout << "Buku tidak ditemukan.\n";
        }
    }

    void editBook(int id)
    {
        Book *current = head;

        while (current && current->id != id)
        {
            current = current->next;
        }

        if (current)
        {
            cout << "Masukkan judul baru: ";
            cin.ignore();
            cin.getline(current->title, sizeof(current->title));

            cout << "Masukkan penulis baru: ";
            cin.getline(current->author, sizeof(current->author));

            cout << "Masukkan kuantitas baru: ";
            cin >> current->quantity;

            cout << "Informasi buku berhasil diubah.\n";
        }
        else
        {
            cout << "Buku tidak ditemukan.\n";
        }
    }

    void saveBooksToFile()
    {
        std::ofstream file("books.txt");

        if (file.is_open())
        {
            Book *current = head;
            while (current)
            {
                file << current->id << " "
                     << current->title << " "
                     << current->author << " "
                     << current->quantity << " "
                     << current->category << "\n";
                current = current->next;
            }

            file.close();
            cout << "Data buku berhasil disimpan ke dalam file.\n";
        }
        else
        {
            cout << "Gagal menyimpan data buku ke dalam file.\n";
        }
    }

    void loadBooksFromFile()
    {
        std::ifstream file("books.txt");

        if (file.is_open())
        {
            int id, quantity;
            char title[100], author[100];
            string category; // Change category type to string

            while (file >> id >> title >> author >> quantity >> category)
            {
                addBook(id, title, author, quantity, category);
            }

            file.close();
            cout << "Data buku berhasil dimuat dari file.\n";
        }
        else
        {
            cout << "File data buku tidak ditemukan. Membuat file baru.\n";
        }
    }

    void borrowBook(int id, const string &borrowerName, int quantity)
    {
        Book *current = head;

        while (current && current->id != id)
        {
            current = current->next;
        }

        if (current && current->quantity >= quantity)
        {
            current->quantity -= quantity;

            Transaction *newTransaction = new Transaction;
            newTransaction->bookID = id;
            newTransaction->borrowerName = borrowerName;
            newTransaction->quantity = quantity;

            // Mendapatkan tanggal saat ini (misalnya, menggunakan format "YYYY-MM-DD")
            time_t now = time(0);
            tm *currentTime = localtime(&now);
            char dateBuffer[80];
            strftime(dateBuffer, 80, "%Y-%m-%d", currentTime);

            newTransaction->borrowDate = dateBuffer;

            // Menghitung tanggal pengembalian (contoh: 7 hari dari peminjaman)
            tm returnTime = *currentTime;
            returnTime.tm_mday += 7;
            mktime(&returnTime);
            strftime(dateBuffer, 80, "%Y-%m-%d", &returnTime);

            newTransaction->returnDate = dateBuffer;

            newTransaction->next = transactionsHead;
            transactionsHead = newTransaction;

            cout << "Buku berhasil dipinjam.\n";
        }
        else
        {
            cout << "Buku tidak tersedia atau tidak ditemukan.\n";
        }
    }

    void displayBooksByCategory(const string &category)
    {
        if (!head)
        {
            cout << "Tidak ada buku.\n";
            return;
        }

        Book *current = head;
        std::cout << "Daftar Buku Kategori " << category << ":\n";
        while (current)
        {
            if (current->category == category)
            {
                cout << "ID: " << current->id << "\n";
                cout << "Judul: " << current->title << "\n";
                cout << "Penulis: " << current->author << "\n";
                cout << "Kuantitas: " << current->quantity << "\n";
                cout << "Kategori: " << current->category << "\n\n";
            }
            current = current->next;
        }
    }

    void printTransactionReceipt()
    {
        if (!transactionsHead)
        {
            cout << "Tidak ada transaksi.\n";
            return;
        }

        Transaction *current = transactionsHead;
        cout << "Bukti Transaksi:\n";
        while (current)
        {
            cout << "ID Buku: " << current->bookID << "\n";
            cout << "Nama Peminjam: " << current->borrowerName << "\n";
            cout << "Jumlah Dipinjam: " << current->quantity << "\n";
            cout << "Tanggal Peminjaman: " << current->borrowDate << "\n";
            cout << "Tanggal Pengembalian: " << current->returnDate << "\n\n";
            current = current->next;
        }
    }

    ~Library()
    {
        saveBooksToFile();

        while (head)
        {
            Book *temp = head;
            head = head->next;
            delete temp;
        }
    }
};

int main()
{
    Library library;
    library.login();
    system("cls");
    int choice;

    do
    {
        cout << "\nMenu:\n";
        cout << "1. Tambah Buku\n";
        cout << "2. Tampilkan Semua Buku\n";
        cout << "3. Cari Buku\n";
        cout << "4. Hapus Buku\n";
        cout << "5. Edit Informasi Buku\n";
        cout << "6. Pinjam buku\n";
        cout << "7. Display buku dari kategori\n";
        cout << "0. Keluar\n";
        cout << "Pilih menu: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
        {
            system("cls");
            int id, quantity;
            char title[100], author[100];
            string category;
            cout << "Masukkan ID Buku: ";
            cin >> id;
            cout << "Masukkan Judul Buku: ";
            cin.ignore();
            cin.getline(title, sizeof(title));
            cout << "Masukkan Nama Penulis: ";
            cin.getline(author, sizeof(author));
            cout << "Masukkan Kuantitas Buku: ";
            cin >> quantity;
            cout << "Masukkan Kategori Buku (fiction / nonfiction): ";
            cin >> category;
            library.addBook(id, title, author, quantity, category);
            break;
        }

        case 2:
            system("cls");
            library.displayBooks();
            break;
        case 3:
        {
            system("cls");
            int id;
            cout << "Masukkan ID Buku yang dicari: ";
            cin >> id;
            library.searchBook(id);
            break;
        }
        case 4:
        {
            system("cls");
            int id;
            cout << "Masukkan ID Buku yang akan dihapus: ";
            cin >> id;
            library.deleteBook(id);
            break;
        }
        case 5:
        {
            system("cls");
            int id;
            cout << "Masukkan ID Buku yang akan diubah: ";
            cin >> id;
            library.editBook(id);
            break;
        }
        case 6:
        {
            system("cls");
            int id, quantity;
            string borrowerName;
            cout << "Masukkan ID Buku yang akan dipinjam: ";
            cin >> id;
            cout << "Masukkan Nama Peminjam: ";
            cin.ignore();
            getline(cin, borrowerName);
            cout << "Masukkan Jumlah Buku yang akan dipinjam: ";
            cin >> quantity;
            library.borrowBook(id, borrowerName, quantity);
            break;
        }
        case 7:
        {
            system("cls");
            string category;
            cout << "Masukkan Kategori Buku untuk Ditampilkan (fiction / nonfiction): ";
            cin >> category;
            library.displayBooksByCategory(category);
            break;
        }
        case 8:
        {
            system("cls");
            library.printTransactionReceipt();
            break;
        }
        case 0:
            cout << "Program Berakhir.\n";
            break;
        default:
            cout << "Pilihan tidak valid.\n";
        }
    } while (choice != 0);

    return 0;
}