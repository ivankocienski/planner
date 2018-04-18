#pragma once

#include <QString>

namespace ik {
namespace db {

class NoteStore {
private:

    bool m_valid { false };
    bool m_dirty { false };
    int m_id { -1 };
    int m_xpos { 0 };
    int m_ypos { 0 };
    int m_width { 0 };
    int m_height { 0 };
    QString m_title;
    QString m_body_text;
    unsigned int m_stack_order { 0 };

    NoteStore(int, int, int, int, int, const QString&, unsigned int, const QString& );

public:

    NoteStore();

    void save();
    void delete_record();

    bool is_valid() const;
    bool is_dirty() const;
    int xpos() const;
    int ypos() const;
    int width() const;
    int height() const;
    unsigned int stack_order() const;
    QString title() const;
    QString body_text() const;

    void set_pos(int, int);
    void set_size(int, int);
    void set_stack_order(unsigned int);
    void set_title(const QString&);
    void set_body_text(const QString&);

    static NoteStore find_by_id(int);
    static NoteStore create(int, int, int, int, const QString &, unsigned int, const QString &);
    static int count();
    static void migrate();
    static QList<NoteStore> find_all();
};

}
}

