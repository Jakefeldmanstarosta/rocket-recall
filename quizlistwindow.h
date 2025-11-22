#pragma once
#include <QWidget>
#include <QListWidget>
#include <QStringList>
#include "QuizApp.h"

class QuizListWindow : public QWidget {
    Q_OBJECT
public:
    QuizListWindow(QWidget *parent = nullptr);

    //so that a user can be assigned for a quizlist
    void setUser(const User &user);
signals:
    void createQuizRequested();
    void playQuizRequested(const Quiz &quiz);
    void editQuizRequested(const Quiz &quiz);



private:
    QListWidget *listWidget;
    const User  *m_user     = nullptr;   // we don't own the user
    void populateList();
};


