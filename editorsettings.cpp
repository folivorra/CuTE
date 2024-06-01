#include "editorsettings.h"

QFont EditorSettings::m_font = QFont("Cascadia Code", 14);
int EditorSettings::m_fontSize = 14;

void EditorSettings::setFont(const QFont &font) {
    m_font = font;
}


void EditorSettings::setFontSize(int size) {
    m_fontSize = size;
    m_font.setPointSize(size);
}

QFont EditorSettings::font() {
    return m_font;
}

int EditorSettings::fontSize() {
    return m_fontSize;
}
