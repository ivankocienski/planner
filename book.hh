#pragma once

#include <QString>

void init_book_connection();

// this is a thing *shrugs*

class Book {
private:

    bool m_valid;
    int m_id;
    QString m_title;
    int m_author_id;

public:
    Book();
    Book(int, const QString&, int);
    bool is_valid() const;

    int id() const;
    QString title() const;
    int author_id() const;

    static Book find_by_id(int);
    static Book create(const QString&, int);
    static int count();
    static void migrate();
};

