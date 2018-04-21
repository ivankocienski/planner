#include "db-common.hh"
#include "notestore.hh"

namespace ik {
namespace db {

QSqlDatabase db_handle;

void safe_query_exec(QSqlQuery &query, const QString &explanation) {

    qInfo() << "DB exec " << query.lastQuery();
    query.exec();

    if(query.lastError().type() != QSqlError::NoError) {

        QString message =
                "FATAL ERROR: " + explanation + " - " + query.lastError().text();
        qFatal(message.toLatin1().constData());
    }
}

bool table_exists(const QString &name) {

    QSqlQuery query("SELECT COUNT(*) FROM sqlite_master WHERE type='table' AND name=?", db_handle);
    query.bindValue(0, name);

    safe_query_exec(query, "determining if table exists");

    query.next();

    int count = query.value(0).toInt();
    //qInfo() << "count=" << count;

    return count == 1;
}

void init_book_connection() {
    db_handle = QSqlDatabase::addDatabase("QSQLITE");

    QString release;

#if QT_NO_DEBUG
    release = "live";
#else
    release = "debug";
#endif

    QFileInfo settings_info(QSettings().fileName());
    QString file_path =
            settings_info.dir().path() +
            "/data." + release + ".file";

    qInfo() << "Using database " << file_path;
    db_handle.setDatabaseName(file_path);

    if(db_handle.open()) {
        qInfo() << "Database openned";

        NoteStore::migrate();
    }
}

} // namespace db
} // namespace ik
