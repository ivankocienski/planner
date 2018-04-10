#include "book.hh"

#include <QtSql>

static QSqlDatabase main_db;

Book::Book() {
    m_valid = false;
    m_id = -1;
    m_author_id = -1;
}

Book::Book(int id, const QString &title, int author_id) {
    m_valid = true;
    m_id = id;
    m_title = title;
    m_author_id = author_id;
}

bool Book::is_valid() const {
    return m_valid;
}

int Book::id() const {
    return m_id;
}

QString Book::title() const {
    return m_title;
}

int Book::author_id() const {
    return m_author_id;
}

//
// non class stuff (sort of)
//

static bool table_exists(const QString &name) {

    QString sql_string = QString("SELECT COUNT(*) FROM sqlite_master WHERE type='table' AND name='%1'").arg(name);
    qInfo() << "exec " << sql_string;

    QSqlQuery query(main_db);
    query.exec(sql_string);
    query.next();

    int count = query.value(0).toInt();
    qInfo() << "count=" << count;

    return count == 1;
}

void Book::migrate() {

    if(!table_exists("books")) {
        qInfo() << "BOOKS table does not exist";

        QString sql_string = "CREATE TABLE books (id INTEGER PRIMARY KEY, name VARCHAR, author_id INTEGER NOT NULL)";
        qInfo() << "exec " << sql_string;

        QSqlQuery query(main_db);
        query.exec(sql_string);

        if(query.lastError().type() != QSqlError::NoError) {
            qFatal("Could not create table");
        }
    }

}

Book Book::find_by_id( int id) {
    QSqlQuery query("SELECT * FROM books WHERE id=?");
    query.bindValue(0, id);

    qInfo() << "exec " << query.lastQuery();
    query.exec();

    if(query.lastError().type() != QSqlError::NoError) {
        qFatal("Could not find book with ID");
        return Book();
    }

    query.next();

    QString title = query.value(1).toString();
    int author_id = query.value(2).toInt();

    return Book(id, title, author_id);
}

Book Book::create(const QString &name, int author_id) {

    QSqlQuery query("INSERT INTO books (name, author_id) VALUES (?, ?)");
    query.bindValue(0, name);
    query.bindValue(1, author_id);

    qInfo() << "exec " << query.lastQuery();
    query.exec();

    if(query.lastError().type() != QSqlError::NoError) {
        qFatal("Could not insert book");
        return Book();
    }

    int id = query.lastInsertId().toInt();

    return Book(id, name, author_id);
}

int Book::count() {

    QString sql_string = "SELECT count(*) FROM books";
    qInfo() << "exec " << sql_string;

    QSqlQuery query(main_db);
    query.exec(sql_string);

    if(query.lastError().type() != QSqlError::NoError) {
        qFatal("Could not count table");
        return -1;
    }

    int count = query.value(0).toInt();
    return count;
}

void init_book_connection() {
    main_db = QSqlDatabase::addDatabase("QSQLITE");
    //main_db.addDatabase("data.sqlite3");
    main_db.setDatabaseName("data.file");

    if(main_db.open()) {
        qInfo() << "Database openned";

        Book::migrate();

        int count = Book::count();
        qInfo() << "Found " << count << "  books";

        //Book b = Book::create("A book title", 123);

        Book b = Book::find_by_id(1);
        if(b.is_valid()) {
            qInfo() << "Found a book!";
            qInfo() << "  id " << b.id();
            qInfo() << "  title " << b.title();
            qInfo() << "  author_id " << b.author_id();

        } else {
            qInfo() << "Did not find book";
        }

    }
}
