#include "notestore.hh"
#include "db-common.hh"

namespace ik {
namespace db {

NoteStore::NoteStore() {
}

NoteStore::NoteStore(int id, int x, int y, int w, int h, const QString &t, unsigned int o ) :
    m_valid(true),
    m_id(id),
    m_xpos(x),
    m_ypos(y),
    m_width(w),
    m_height(h),
    m_title(t),
    m_stack_order(o) {
}

bool NoteStore::is_dirty() const  { return m_dirty; }
bool NoteStore::is_valid() const  { return m_valid; }
int NoteStore::xpos() const       { return m_xpos; }
int NoteStore::ypos() const       { return m_ypos; }
int NoteStore::width() const      { return m_width; }
int NoteStore::height() const     { return m_height; }
QString NoteStore::title() const  { return m_title; }
unsigned int NoteStore::stack_order() const { return m_stack_order; }

void NoteStore::set_pos(int nx, int ny) {
    m_xpos = nx;
    m_ypos = ny;
    m_dirty = true;
}

void NoteStore::set_size(int nw, int nh) {
    m_width = nw;
    m_height = nh;
    m_dirty = true;
}

void NoteStore::set_stack_order(unsigned int o) {
    m_stack_order = o;
    m_dirty = true;
}

void NoteStore::save() {

    if(m_dirty && m_valid) {
        qDebug() << "NoteStore.save() !!!";

        QString sql_query =
                "UPDATE notes "
                "SET xpos=?, ypos=?, width=?, height=?, title=?, stack_order=? "
                "WHERE id=?";

        QSqlQuery query(sql_query, db_handle);

        query.bindValue(0, m_xpos);
        query.bindValue(1, m_ypos);
        query.bindValue(2, m_width);
        query.bindValue(3, m_height);
        query.bindValue(4, m_title.toLatin1().data());
        query.bindValue(5, m_stack_order);

        // where
        query.bindValue(6, m_id);

        qInfo() << "exec " << query.lastQuery();
        query.exec();

        if(query.lastError().type() != QSqlError::NoError) {
            QString message = "Could not insert note: " + query.lastError().text();
            qFatal(message.toLatin1().constData());
            return; // oops ?
        }

        m_dirty = false;
    }
}

void NoteStore::delete_record() {

    if(m_valid) {
        qDebug() << "NoteStore.delete_record() !!!";

        QString sql_query = "DELETE FROM notes WHERE id=?";

        QSqlQuery query(sql_query, db_handle);

        query.bindValue(0, m_id);

        qInfo() << "exec " << query.lastQuery();
        query.exec();

        if(query.lastError().type() != QSqlError::NoError) {
            QString message = "Could not delete note: " + query.lastError().text();
            qFatal(message.toLatin1().constData());
            return; // oops ?
        }

        m_dirty = false;
        m_valid = false;
    }
}

//
// static class helper methods
//

QList<NoteStore> NoteStore::find_all() {
    QList<NoteStore> list;

    QString sql_query =
            "SELECT id, xpos, ypos, width, height, title, stack_order "
            "FROM notes "
            "ORDER BY stack_order";

    QSqlQuery query(sql_query, db_handle);
    qInfo() << "exec " << query.lastQuery();
    query.exec();

    if(query.lastError().type() != QSqlError::NoError) {
        QString message = "Could not find notes" + query.lastError().text();
        qFatal(message.toLatin1().constData());
        return list;
    }

    while(query.next()) {

        int id     = query.value(0).toInt();
        int xpos   = query.value(1).toInt();
        int ypos   = query.value(2).toInt();
        int width  = query.value(3).toInt();
        int height = query.value(4).toInt();
        QString title = query.value(5).toString();
        unsigned int order = query.value(6).toUInt();

        list << NoteStore(id, xpos, ypos, width, height, title, order);
    }

    return list;
}

NoteStore NoteStore::find_by_id(int id) {

    QSqlQuery query(
                "SELECT xpos, ypos, width, height, title, stack_order FROM notes WHERE id=?",
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
    unsigned int order = query.value(5).toUInt();

    return NoteStore(id, xpos, ypos, width, height, title, order);
}

void NoteStore::migrate() {

    if(!table_exists("notes")) {
        qInfo() << "table 'notes' does not exist";

        QString sql_string =
                "CREATE TABLE notes"
                " (id INTEGER PRIMARY KEY, xpos INTEGER NOT NULL, ypos INTEGER NOT NULL,"
                " width INTEGER NOT NULL, height INTEGER NOT NULL, title VARCHAR NOT NULL "
                " stack_order INTEGER NOT NULL)";

        QSqlQuery query(sql_string, db_handle);
        qInfo() << "exec " << query.lastQuery();
        query.exec();

        if(query.lastError().type() != QSqlError::NoError) {
            qFatal("Could not create table");
        }
    }
}

NoteStore NoteStore::create(int x, int y, int w, int h, const QString &t, unsigned int o) {

    QString sql_string =
            "INSERT INTO notes (xpos, ypos, width, height, title, stack_order) "
            "VALUES (?, ?, ?, ?, ?, ?)";

    QSqlQuery query(sql_string, db_handle);

    query.bindValue(0, x); // xpos
    query.bindValue(1, y); // ypos
    query.bindValue(2, w); // width
    query.bindValue(3, h); // height
    query.bindValue(4, t); // title
    query.bindValue(5, o); // stack order

    qInfo() << "exec " << query.lastQuery();
    query.exec();

    if(query.lastError().type() != QSqlError::NoError) {
        QString message = "Could not insert note: " + query.lastError().text();
        qFatal(message.toLatin1().constData());
        return NoteStore();
    }

    int id = query.lastInsertId().toInt();
    return NoteStore(id, x, y, w, h, t, o);
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
