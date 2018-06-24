/**
  * @author Riccardo Damiani
  * @version 1.0 23/06/18
  */

#ifndef DSTEXTDOCKWIDGET_H
#define DSTEXTDOCKWIDGET_H

#include <QDockWidget>
#include <QHBoxLayout>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QWidget>
#include <QString>

class DSTextDockWidget: public QDockWidget {
    Q_OBJECT
private:
    QHBoxLayout* h_layout;
    QPlainTextEdit* text_edit;
    QPushButton* load_button;

    void setupUI() {
        h_layout->addWidget(text_edit, Qt::AlignCenter);
        h_layout->addWidget(load_button, Qt::AlignCenter);

        QWidget* internal_widget = new QWidget();
        internal_widget->setLayout(h_layout);
        setWidget(internal_widget);
        setWindowTitle("Text");

        connect(load_button, &QPushButton::clicked, this, &DSTextDockWidget::loadButtonClicked);

        text_edit->setMinimumHeight(10);
        text_edit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
        load_button->setMaximumWidth(80);
    }
signals:
    void loadButtonClicked();
public:
    DSTextDockWidget(QWidget* parent):
        QDockWidget(parent),
        h_layout(new QHBoxLayout),
        text_edit(new QPlainTextEdit),
        load_button(new QPushButton("Load File"))
    {
        setupUI();
    }
    QString getText() const { return text_edit->toPlainText(); }
    void setText(const QString& text) { text_edit->setPlainText(text); }
};

#endif // DSTEXTDOCKWIDGET_H
