#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QButtonGroup>

#include "QuizApp.h"

class PlayQuizWindow : public QWidget {
    Q_OBJECT
public:
    explicit PlayQuizWindow(const Quiz &quiz, QWidget *parent = nullptr);

private:
    void loadQuestion();
    void finishQuiz();

    Quiz m_quiz;
    int m_index = 0;
    std::vector<int> m_answers; // -1 = unanswered

    QLabel *questionLabel;
    QVBoxLayout *choicesLayout;
    QButtonGroup *choiceButtons;
    QPushButton *nextBtn;
    QPushButton *backBtn;
};
