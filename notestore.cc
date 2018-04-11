#include "notestore.hh"
#include "db-common.hh"

namespace ik {
namespace db {

NoteStore::NoteStore() {
}

NoteStore::NoteStore(int id, int x, int y, int w, int h, const QString &t ) :
    m_valid(true),
    m_id(id),
    m_xpos(x),
    m_ypos(y),
    m_width(w),
    m_height(h),
    m_title(t) {
}

bool NoteStore::is_valid() const  { return m_valid; }
int NoteStore::xpos() const       { return m_xpos; }
int NoteStore::ypos() const       { return m_ypos; }
int NoteStore::width() const      { return m_width; }
int NoteStore::height() const     { return m_height; }
QString NoteStore::title() const  { return m_title; }

//
// static class helper methods
//

NoteStore NoteStore::find_by_id(int id) {

    QSqlQuery query(
                "SELECT xpos, ypos, width, height, title FROM notes WHERE id=?",
                db_handle);
    query.bindValue(0, id);

    qInfo() << "exec " << query.lastQuery();
    query.exec();

    if(query.lastError().type() != QSqlError::NoError) {
        qFatal("Could not find book with ID");
        return NoteStore();
    }

    query.next();

    int xpos   = query.value(0).toInt();
    int ypos   = query.value(1).toInt();
    int width  = query.value(2).toInt();
    int height = query.value(3).toInt();
    QString title = query.value(4).toString();

    return NoteStore(id, xpos, ypos, width, height, title);
}

void NoteStore::migrate() {

    if(!table_exists("notes")) {
        qInfo() << "table 'notes' does not exist";

        QString sql_string =
                "CREATE TABLE notes"
                " (id INTEGER PRIMARY KEY, xpos INTEGER NOT NULL, ypos INTEGER NOT NULL,"
                " width INTEGER NOT NULL, height INTEGER NOT NULL, title VARCHAR NOT NULL)";

        QSqlQuery query(sql_string, db_handle);
        qInfo() << "exec " << query.lastQuery();
        query.exec();

        if(query.lastError().type() != QSqlError::NoError) {
            qFatal("Could not create table");
        }
    }
}

NoteStore NoteStore::create(int x, int y, int w, int h, const QString &t) {

    QSqlQuery query(
                "INSERT INTO notes (xpos, ypos, width, height title) VALUES (?, ?, ?, ?, ?)",
                db_handle);

    query.bindValue(0, x);
    query.bindValue(1, y);
    query.bindValue(3, w);
    query.bindValue(4, h);
    query.bindValue(5, t);

    qInfo() << "exec " << query.lastQuery();
    query.exec();

    if(query.lastError().type() != QSqlError::NoError) {
        qFatal("Could not insert note");
        return NoteStore();
    }

    int id = query.lastInsertId().toInt();
    return NoteStore(id, x, y, w, h, t);
}

int NoteStore::count() {

    QString sql_string = "SELECT count(*) FROM notes";

    QSqlQuery query(sql_string, db_handle);
    qInfo() << "exec " << query.lastQuery();
    query.exec();

    if(query.lastError().type() != QSqlError::NoError) {
        qFatal("Could not count table");
        return -1;
    }

    query.next();

    return query.value(0).toInt();
}

} // namespace db
} // namespace ik
