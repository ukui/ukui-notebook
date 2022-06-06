#ifndef FONT_COLOR_PUSH_BUTTON_H
#define FONT_COLOR_PUSH_BUTTON_H

#include <QPushButton>

class FontColorPushButton : public QPushButton
{
    Q_OBJECT
private:
    /* data */
public:
    explicit FontColorPushButton(QWidget *parent = 0);
    ~FontColorPushButton();

    void getButtonColor();
    void setButtonColor();

};

#endif // FONT_COLOR_PUSH_BUTTON_H