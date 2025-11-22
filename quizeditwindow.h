#ifndef QUIZEDITWINDOW_H
#define QUIZEDITWINDOW_H

#include <QDialog>
#include "QuizApp.h"   // for User, Quiz, EditQuizController

namespace Ui {
class QuizEditWindow;
}

class QuizEditWindow : public QDialog
{
    Q_OBJECT

public:
    explicit QuizEditWindow(QWidget *parent = nullptr);
    ~QuizEditWindow();

    // Call this before showing the dialog
    void setQuiz(User &user, Quiz &quiz, EditQuizController &controller);
    void setQuiz(User &user, Quiz &quiz, CreateQuizController &controller);


private slots:
    void on_addQuestionButton_clicked();
    void on_removeQuestionButton_clicked();
    void on_doneButton_clicked();

private:
    Ui::QuizEditWindow *ui;

    User *m_user{nullptr};
    Quiz *m_quiz{nullptr};
    EditQuizController *m_controller{nullptr};
    CreateQuizController *m_createController{nullptr};


    void refreshQuestionList();
};

#endif // QUIZEDITWINDOW_H
