#pragma once

#include <QString>

namespace ik {
namespace db {

class NoteStore {
private:

    bool m_valid { false };
    int m_id { -1 };
    int m_xpos { 0 };
    int m_ypos { 0 };
    int m_width { 0 };
    int m_height { 0 };
    QString m_title;

    NoteStore(int, int, int, int, int, const QString& );

public:

    NoteStore();

    bool is_valid() const;
    int xpos() const;
    int ypos() const;
    int width() const;
    int height() const;
    QString title() const;

    static NoteStore find_by_id(int);
    static NoteStore create(int, int, int, int, const QString &);
    static int count();
    static void migrate();
};

}
}

