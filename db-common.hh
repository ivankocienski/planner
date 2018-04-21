#pragma once

#include <QtSql>
#include <QString>

namespace ik {
namespace db {

extern QSqlDatabase db_handle;

void init_book_connection();
bool table_exists(const QString &);
void safe_query_exec(QSqlQuery&, const QString&);

}
}
