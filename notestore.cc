#include "notestore.hh"
#include "db-common.hh"

namespace ik {
namespace db {

// this is probably not the best way of doing DB stuff.

NoteStore::NoteStore() {

    // set to now
    touch_modified();
}

NoteStore::NoteStore(
        int id,
        int x,
        int y,
        int w,
        int h,
        const QString &t,
        unsigned int o,
        const QString &b,
        const QDateTime &d,
        bool s) :
    m_valid(true),
    m_dirty(true),
    m_id(id),
    m_xpos(x),
    m_ypos(y),
    m_width(w),
    m_height(h),
    m_title(t),
    m_body_text(b),
    m_stack_order(o),
    m_modified(d),
    m_is_shaded(s) {

    m_modified.setTimeSpec(Qt::UTC);
}

void NoteStore::touch_modified() {
    m_modified = QDateTime::currentDateTime();
}

bool NoteStore::is_dirty() const  { return m_dirty; }
bool NoteStore::is_valid() const  { return m_valid; }
int NoteStore::xpos() const       { return m_xpos; }
int NoteStore::ypos() const       { return m_ypos; }
int NoteStore::width() const      { return m_width; }
int NoteStore::height() const     { return m_height; }
bool NoteStore::is_shaded() const { return m_is_shaded; }

QString NoteStore::title() const            { return m_title; }
QString NoteStore::body_text() const        { return m_body_text; }
unsigned int NoteStore::stack_order() const { return m_stack_order; }
QDateTime NoteStore::modified() const       { return m_modified; }

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

void NoteStore::set_title(const QString &t) {
    m_title = t;
    m_dirty = true;

    touch_modified();
}

void NoteStore::set_body_text(const QString &t) {
    m_body_text = t;
    m_dirty = true;

    touch_modified();
}

void NoteStore::set_shaded(bool s) {
    m_is_shaded = s;
    m_dirty = true;
}

void NoteStore::save() {

    if(m_dirty && m_valid) {
        qDebug() << "NoteStore.save() !!!";

        QString sql_query =
                "UPDATE notes "
                "SET xpos=?, ypos=?, width=?, height=?, "
                "title=?, stack_order=?, body_text=?, modified_at=?, is_shaded=? "
                "WHERE id=?";

        QSqlQuery query(sql_query, db_handle);

        query.bindValue(0, m_xpos);
        query.bindValue(1, m_ypos);
        query.bindValue(2, m_width);
        query.bindValue(3, m_height);
        query.bindValue(4, m_title.toLatin1().data());
        query.bindValue(5, m_stack_order);
        query.bindValue(6, m_body_text);
        query.bindValue(7, m_modified.toString(Qt::ISODate));
        query.bindValue(8, m_is_shaded ? "1" : "0");

        // where
        query.bindValue(9, m_id);

        safe_query_exec(query, "note save");

        m_dirty = false;
    }
}

void NoteStore::delete_record() {

    if(m_valid) {
        qDebug() << "NoteStore.delete_record() !!!";

        QString sql_query = "DELETE FROM notes WHERE id=?";

        QSqlQuery query(sql_query, db_handle);

        query.bindValue(0, m_id);

        safe_query_exec(query, "note delete record");

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
            "SELECT id, xpos, ypos, width, height, "
            "title, stack_order, body_text, modified_at, is_shaded "
            "FROM notes "
            "ORDER BY stack_order";

    QSqlQuery query(sql_query, db_handle);
    safe_query_exec(query, "note find_all");

    while(query.next()) {

        int id     = query.value(0).toInt();
        int xpos   = query.value(1).toInt();
        int ypos   = query.value(2).toInt();
        int width  = query.value(3).toInt();
        int height = query.value(4).toInt();
        QString title = query.value(5).toString();
        unsigned int order = query.value(6).toUInt();
        QString body_text = query.value(7).toString();
        QDateTime modified = query.value(8).toDateTime();
        bool shaded = (query.value(9).toInt() == 1);

        list << NoteStore(
                    id,
                    xpos,
                    ypos,
                    width,
                    height,
                    title,
                    order,
                    body_text,
                    modified,
                    shaded);
    }

    return list;
}

NoteStore NoteStore::find_by_id(int id) {

    QString sql_query =
            "SELECT "
            "xpos, ypos, width, height, title, stack_order, body_text, modified_at, is_shaded "
            "FROM notes "
            "WHERE id=?";

    QSqlQuery query(sql_query, db_handle);
    query.bindValue(0, id);

    safe_query_exec(query, "note find_by_id");

    query.next();

    int xpos   = query.value(0).toInt();
    int ypos   = query.value(1).toInt();
    int width  = query.value(2).toInt();
    int height = query.value(3).toInt();
    QString title = query.value(4).toString();
    unsigned int order = query.value(5).toUInt();
    QString body_text = query.value(6).toString();
    QDateTime modified = query.value(7).toDateTime();
    bool shaded = (query.value(8).toInt() == 1);

    return NoteStore(
                id,
                xpos,
                ypos,
                width,
                height,
                title,
                order,
                body_text,
                modified,
                shaded);
}

void NoteStore::migrate() {

    if(!table_exists("notes")) {
        QString sql_string =
                "CREATE TABLE IF NOT EXISTS notes"
                " (id INTEGER PRIMARY KEY, xpos INTEGER NOT NULL, ypos INTEGER NOT NULL,"
                " width INTEGER NOT NULL, height INTEGER NOT NULL, title VARCHAR NOT NULL,"
                " stack_order INTEGER NOT NULL, body_text TEXT NOT NULL DEFAULT '',"
                " modified_at DATETIME NOT NULL, is_shaded INTEGER DEFAULT 0)";

        QSqlQuery query(sql_string, db_handle);
        safe_query_exec(query, "migrate note table");
    }
}

NoteStore NoteStore::create(
        int x,
        int y,
        int w,
        int h,
        const QString &t,
        unsigned int o,
        const QString &b) {

    QString sql_string =
            "INSERT INTO notes "
            "(xpos, ypos, width, height, title, stack_order, body_text, modified_at, is_shaded) "
            "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)";

    QDateTime d = QDateTime::currentDateTime();
    d.setTimeSpec(Qt::UTC);

    QSqlQuery query(sql_string, db_handle);

    query.bindValue(0, x); // xpos
    query.bindValue(1, y); // ypos
    query.bindValue(2, w); // width
    query.bindValue(3, h); // height
    query.bindValue(4, t); // title
    query.bindValue(5, o); // stack order
    query.bindValue(6, b); // body text
    query.bindValue(7, d.toString(Qt::ISODate)); // body text
    query.bindValue(8, 0);

    safe_query_exec(query, "creating note");

    int id = query.lastInsertId().toInt();
    return NoteStore(id, x, y, w, h, t, o, b, d, 0);
}

int NoteStore::count() {

    QString sql_string = "SELECT count(*) FROM notes";
    QSqlQuery query(sql_string, db_handle);
    safe_query_exec(query, "counting notes");
    query.next();

    return query.value(0).toInt();
}

} // namespace db
} // namespace ik
