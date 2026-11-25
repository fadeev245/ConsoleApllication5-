
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <iomanip>
#include <limits>

using namespace std;

// ================= КЛАСС BOOK =================

class Book {
private:
    int id;
    string title;
    string author;
    int year;
    bool isIssued;
    int readerId;

public:
    Book(int id, string title, string author, int year)
        : id(id), title(title), author(author),
        year(year), isIssued(false), readerId(-1) {
    }

    int getId() const { return id; }
    string getTitle() const { return title; }
    string getAuthor() const { return author; }
    int getYear() const { return year; }
    bool getIsIssued() const { return isIssued; }
    int getReaderId() const { return readerId; }

    void setTitle(string value) { title = value; }
    void setAuthor(string value) { author = value; }
    void setYear(int value) { year = value; }

    void issue(int idReader) {
        isIssued = true;
        readerId = idReader;
    }

    void returnBook() {
        isIssued = false;
        readerId = -1;
    }

    void display() const {
        cout << left << setw(5) << id
            << setw(27) << title
            << setw(22) << author
            << setw(10) << year
            << (isIssued ? "Выдана" : "Свободна")
            << endl;
    }
};

// ================= КЛАСС READER =================

class Reader {
private:
    int id;
    string fullName;
    string phone;
    vector<int> bookIds;

public:
    Reader(int id, string fullName, string phone)
        : id(id), fullName(fullName), phone(phone) {
    }

    int getId() const { return id; }
    string getFullName() const { return fullName; }
    string getPhone() const { return phone; }

    const vector<int>& getBookIds() const {
        return bookIds;
    }

    void setFullName(string value) { fullName = value; }
    void setPhone(string value) { phone = value; }

    void addBook(int bookId) {
        bookIds.push_back(bookId);
    }

    void removeBook(int bookId) {
        bookIds.erase(
            remove(bookIds.begin(), bookIds.end(), bookId),
            bookIds.end()
        );
    }

    void display() const {
        cout << "ID: " << id
            << " | ФИО: " << fullName
            << " | Телефон: " << phone
            << " | Книг на руках: " << bookIds.size()
            << endl;
    }
};

// ================= КЛАСС LIBRARY =================

class Library {
private:
    vector<unique_ptr<Book>> books;
    vector<shared_ptr<Reader>> readers;

    int nextBookId = 1;
    int nextReaderId = 1;

    Book* findBook(int id) {
        for (auto& book : books) {
            if (book->getId() == id)
                return book.get();
        }
        return nullptr;
    }

    shared_ptr<Reader> findReader(int id) {
        for (auto& reader : readers) {
            if (reader->getId() == id)
                return reader;
        }
        return nullptr;
    }

    void initializeData() {
        books.push_back(make_unique<Book>(
            nextBookId++, "Преступление и наказание",
            "Ф. Достоевский", 1866));

        books.push_back(make_unique<Book>(
            nextBookId++, "Война и мир",
            "Л. Толстой", 1869));

        books.push_back(make_unique<Book>(
            nextBookId++, "Мастер и Маргарита",
            "М. Булгаков", 1967));

        books.push_back(make_unique<Book>(
            nextBookId++, "Отцы и дети",
            "И. Тургенев", 1862));

        readers.push_back(make_shared<Reader>(
            nextReaderId++, "Иванов Иван Иванович",
            "+7-900-111-22-33"));

        readers.push_back(make_shared<Reader>(
            nextReaderId++, "Петров Петр Петрович",
            "+7-900-444-55-66"));

        readers.push_back(make_shared<Reader>(
            nextReaderId++, "Сидоров Алексей Сергеевич",
            "+7-900-777-88-99"));
    }

public:
    Library() {
        initializeData();
    }

    // Добавление книги
    void addBook() {
        string title, author;
        int year;

        cout << "Название книги: ";
        getline(cin >> ws, title);

        cout << "Автор: ";
        getline(cin, author);

        cout << "Год издания: ";
        cin >> year;

        if (year <= 0 || year > 2026) {
            cout << "Некорректный год издания!\n";
            return;
        }

        books.push_back(make_unique<Book>(
            nextBookId++, title, author, year));

        cout << "Книга успешно добавлена!\n";
    }

    // Просмотр всех книг
    void showBooks() const {
        if (books.empty()) {
            cout << "В библиотеке нет книг.\n";
            return;
        }

        cout << "\nСПИСОК КНИГ\n";
        cout << left << setw(5) << "ID"
            << setw(27) << "Название"
            << setw(22) << "Автор"
            << setw(10) << "Год"
            << "Статус\n";

        for (const auto& book : books)
            book->display();
    }

    // Редактирование книги
    void editBook() {
        int id;
        cout << "Введите ID книги: ";
        cin >> id;

        Book* book = findBook(id);

        if (!book) {
            cout << "Книга не найдена!\n";
            return;
        }

        string title, author;
        int year;

        cout << "Новое название: ";
        getline(cin >> ws, title);

        cout << "Новый автор: ";
        getline(cin, author);

        cout << "Новый год: ";
        cin >> year;

        if (year <= 0 || year > 2026) {
            cout << "Некорректный год!\n";
            return;
        }

        book->setTitle(title);
        book->setAuthor(author);
        book->setYear(year);

        cout << "Данные книги обновлены!\n";
    }

    // Удаление книги
    void deleteBook() {
        int id;
        cout << "Введите ID книги: ";
        cin >> id;

        auto it = find_if(books.begin(), books.end(),
            [id](const unique_ptr<Book>& book) {
                return book->getId() == id;
            });

        if (it == books.end()) {
            cout << "Книга не найдена!\n";
            return;
        }

        if ((*it)->getIsIssued()) {
            cout << "Нельзя удалить выданную книгу!\n";
            return;
        }

        books.erase(it);
        cout << "Книга удалена!\n";
    }

    // Добавление читателя
    void addReader() {
        string name, phone;

        cout << "ФИО читателя: ";
        getline(cin >> ws, name);

        cout << "Телефон: ";
        getline(cin, phone);

        readers.push_back(make_shared<Reader>(
            nextReaderId++, name, phone));

        cout << "Читатель добавлен!\n";
    }

    // Просмотр читателей
    void showReaders() const {
        if (readers.empty()) {
            cout << "Читателей нет.\n";
            return;
        }

        cout << "\nСПИСОК ЧИТАТЕЛЕЙ\n";

        for (const auto& reader : readers)
            reader->display();
    }

    // Редактирование читателя
    void editReader() {
        int id;
        cout << "Введите ID читателя: ";
        cin >> id;

        auto reader = findReader(id);

        if (!reader) {
            cout << "Читатель не найден!\n";
            return;
        }

        string name, phone;

        cout << "Новое ФИО: ";
        getline(cin >> ws, name);

        cout << "Новый телефон: ";
        getline(cin, phone);

        reader->setFullName(name);
        reader->setPhone(phone);

        cout << "Данные читателя обновлены!\n";
    }

    // Удаление читателя
    void deleteReader() {
        int id;
        cout << "Введите ID читателя: ";
        cin >> id;

        auto reader = findReader(id);

        if (!reader) {
            cout << "Читатель не найден!\n";
            return;
        }

        if (!reader->getBookIds().empty()) {
            cout << "Нельзя удалить читателя, "
                << "пока у него есть книги!\n";
            return;
        }

        readers.erase(
            remove_if(readers.begin(), readers.end(),
                [id](const shared_ptr<Reader>& r) {
                    return r->getId() == id;
                }),
            readers.end()
        );

        cout << "Читатель удален!\n";
    }

    // Выдача книги
    void issueBook() {
        int bookId, readerId;

        cout << "Введите ID книги: ";
        cin >> bookId;

        Book* book = findBook(bookId);

        if (!book) {
            cout << "Книга не найдена!\n";
            return;
        }

        if (book->getIsIssued()) {
            cout << "Книга уже выдана!\n";
            return;
        }

        cout << "Введите ID читателя: ";
        cin >> readerId;

        auto reader = findReader(readerId);

        if (!reader) {
            cout << "Читатель не найден!\n";
            return;
        }

        book->issue(readerId);
        reader->addBook(bookId);

        cout << "Книга успешно выдана читателю "
            << reader->getFullName() << "!\n";
    }

    // Возврат книги
    void returnBook() {
        int bookId;

        cout << "Введите ID возвращаемой книги: ";
        cin >> bookId;

        Book* book = findBook(bookId);

        if (!book) {
            cout << "Книга не найдена!\n";
            return;
        }

        if (!book->getIsIssued()) {
            cout << "Эта книга не выдана!\n";
            return;
        }

        auto reader = findReader(book->getReaderId());

        if (reader)
            reader->removeBook(bookId);

        book->returnBook();

        cout << "Книга успешно возвращена!\n";
    }

    // Книги конкретного читателя
    void showReaderBooks() {
        int id;

        cout << "Введите ID читателя: ";
        cin >> id;

        auto reader = findReader(id);

        if (!reader) {
            cout << "Читатель не найден!\n";
            return;
        }

        cout << "\nКниги читателя: "
            << reader->getFullName() << "\n";

        if (reader->getBookIds().empty()) {
            cout << "У читателя нет книг.\n";
            return;
        }

        for (int bookId : reader->getBookIds()) {
            Book* book = findBook(bookId);

            if (book) {
                cout << book->getTitle()
                    << " | " << book->getAuthor()
                    << " | " << book->getYear()
                    << endl;
            }
        }
    }

    // Статистика библиотеки
    void showStatistics() const {
        int issued = 0;

        for (const auto& book : books) {
            if (book->getIsIssued())
                issued++;
        }

        cout << "\nСТАТИСТИКА БИБЛИОТЕКИ\n";
        cout << "----------------------------\n";
        cout << "Всего книг: " << books.size() << endl;
        cout << "Свободных книг: "
            << books.size() - issued << endl;
        cout << "Выданных книг: " << issued << endl;
        cout << "Всего читателей: "
            << readers.size() << endl;
    }

    // Подменю книг
    void booksMenu() {
        int choice;

        do {
            cout << "\n--- УПРАВЛЕНИЕ КНИГАМИ ---\n";
            cout << "1. Добавить книгу\n";
            cout << "2. Показать все книги\n";
            cout << "3. Редактировать книгу\n";
            cout << "4. Удалить книгу\n";
            cout << "0. Назад\n";
            cout << "Выберите действие: ";

            cin >> choice;

            switch (choice) {
            case 1: addBook(); break;
            case 2: showBooks(); break;
            case 3: editBook(); break;
            case 4: deleteBook(); break;
            case 0: break;
            default: cout << "Неверный пункт меню!\n";
            }

        } while (choice != 0);
    }

    // Подменю читателей
    void readersMenu() {
        int choice;

        do {
            cout << "\n--- УПРАВЛЕНИЕ ЧИТАТЕЛЯМИ ---\n";
            cout << "1. Добавить читателя\n";
            cout << "2. Показать всех читателей\n";
            cout << "3. Редактировать читателя\n";
            cout << "4. Удалить читателя\n";
            cout << "0. Назад\n";
            cout << "Выберите действие: ";

            cin >> choice;

            switch (choice) {
            case 1: addReader(); break;
            case 2: showReaders(); break;
            case 3: editReader(); break;
            case 4: deleteReader(); break;
            case 0: break;
            default: cout << "Неверный пункт меню!\n";
            }

        } while (choice != 0);
    }

    // Главное меню
    void run() {
        int choice;

        do {
            cout << "\n====================================\n";
            cout << "       ИНФОРМАЦИОННАЯ СИСТЕМА\n";
            cout << "              БИБЛИОТЕКА\n";
            cout << "====================================\n";

            cout << "1. Управление книгами\n";
            cout << "2. Управление читателями\n";
            cout << "3. Выдать книгу читателю\n";
            cout << "4. Посмотреть книги читателя\n";
            cout << "5. Вернуть книгу\n";
            cout << "6. Статистика библиотеки\n";
            cout << "0. Выход\n";

            cout << "Выберите действие: ";
            cin >> choice;

            switch (choice) {
            case 1: booksMenu(); break;
            case 2: readersMenu(); break;
            case 3: issueBook(); break;
            case 4: showReaderBooks(); break;
            case 5: returnBook(); break;
            case 6: showStatistics(); break;
            case 0:
                cout << "Программа завершена.\n";
                break;
            default:
                cout << "Неверный пункт меню!\n";
            }

        } while (choice != 0);
    }
};

// ================= MAIN =================

int main() {
    setlocale(LC_ALL, "Russian");

    Library library;
    library.run();

    return 0;
}
