#include "db-common.hh"
#include "notestore.hh"

namespace ik {
namespace db {

QSqlDatabase db_handle;

bool table_exists(const QString &name) {

    QSqlQuery query("SELECT COUNT(*) FROM sqlite_master WHERE type='table' AND name=?", db_handle);
    query.bindValue(0, name);
    qInfo() << "exec " << query.lastQuery();

    query.exec();
    if(query.lastError().type() != QSqlError::NoError) {
        qFatal("Could not see if table exists");
        return false;
    }

    query.next();

    int count = query.value(0).toInt();
    qInfo() << "count=" << count;

    return count == 1;
}

void init_book_connection() {
    db_handle = QSqlDatabase::addDatabase("QSQLITE");
    //main_db.addDatabase("data.sqlite3");
    db_handle.setDatabaseName("data.file");

    if(db_handle.open()) {
        qInfo() << "Database openned";

        NoteStore::migrate();
    }
}

} // namespace db
} // namespace ik
