#ifndef EDITORSETTINGS_H
#define EDITORSETTINGS_H

#include <QFont>
#include <QString>

class EditorSettings {
public:
    static void setFont(const QFont &font);
    static void setFontSize(int size);

    static QFont font();
    static int fontSize();

private:
    static QFont m_font;
    static int m_fontSize;

};

#endif // EDITORSETTINGS_H
