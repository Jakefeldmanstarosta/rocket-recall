#pragma once
#include <QListWidget>
#include <QStringList>
#include <QWidget>
#include "QuizApp.h"

class QuizListWindow : public QWidget
{
    Q_OBJECT
public:
    QuizListWindow(QWidget *parent = nullptr);

    //so that a user can be assigned for a quizlist
    void setUser(User &user);
signals:
    void createQuizRequested();
    void playQuizRequested(const Quiz &quiz);
    void editQuizRequested(const Quiz &quiz);

private:
    QListWidget *listWidget;
    User *m_user = nullptr;
    void populateList();
};
