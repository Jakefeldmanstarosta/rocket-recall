#include "quizlistwindow.h"
#include "quizrowwidget.h"
#include "mainwindow.h"

#include <QVBoxLayout>

QuizListWindow::QuizListWindow(QWidget *parent)
    : QWidget(parent)
{
    auto *layout = new QVBoxLayout(this);

    listWidget = new QListWidget(this);
    layout->addWidget(listWidget);

//    // Example quiz list
//    QStringList quizzes = { "Algebra Basics", "C++ Memory", "Geography Flags", "Physics Forces" };

//    //what I want
//    //QStringList quizzes = parent->getQuizzes();


//    for (const QString &quizName : quizzes) {
//        QListWidgetItem *item = new QListWidgetItem(listWidget);
//        item->setSizeHint(QSize(300, 50));

//        QuizRowWidget *row = new QuizRowWidget(quizName);
//        listWidget->setItemWidget(item, row);

//        // Connect signals
//        connect(row, &QuizRowWidget::playQuiz,
//                this, [](const QString &name) {
//                    qDebug() << "Play quiz:" << name;
//                });

//        connect(row, &QuizRowWidget::editQuiz,
//                this, [](const QString &name) {
//                    qDebug() << "Edit quiz:" << name;
//                });
//    }
}

// Called by MainWindow after login / create account
void QuizListWindow::setUser(const User &user)
{
    m_user = &user;
    populateList();
}

void QuizListWindow::populateList()
{
    listWidget->clear();

    if (!m_user) return;

    const auto &quizzes = m_user->getQuizzes();

    //
    // 1. Add existing quiz rows
    //
    for (const Quiz &quiz : quizzes) {

        QString quizName = QString::fromStdString(quiz.getTitle());

        QListWidgetItem *item = new QListWidgetItem(listWidget);
        item->setSizeHint(QSize(300, 50));

        auto *row = new QuizRowWidget(quizName);
        listWidget->setItemWidget(item, row);

        // Connect signals
        connect(row, &QuizRowWidget::playQuiz,
                this, [this,&quiz](const QString &name) {
                    qDebug() << "Play quiz:" << name;
                    emit playQuizRequested(quiz);
                });

        connect(row, &QuizRowWidget::editQuiz,
                this, [this,quiz](const QString &name) {
                    qDebug() << "Edit quiz:" << name;
                    emit editQuizRequested(quiz);
                });
    }

    //
    // 2. Add "Create New Quiz" button as the last item
    //
    {
        QListWidgetItem *buttonItem = new QListWidgetItem(listWidget);
        buttonItem->setSizeHint(QSize(300, 50));

        // A container widget to hold the button
        QWidget *container = new QWidget();
        QHBoxLayout *layout = new QHBoxLayout(container);
        layout->setContentsMargins(0,0,0,0);

        QPushButton *btn = new QPushButton("➕  Create New Quiz");
        btn->setStyleSheet("font-weight: bold; padding: 8px;");
        layout->addWidget(btn);

        listWidget->setItemWidget(buttonItem, container);

        connect(btn, &QPushButton::clicked, this, [this]() {
            qDebug() << "Create New Quiz clicked!";
            // Emit a signal that MainWindow or whoever can handle:
            emit createQuizRequested();
        });
    }
}
