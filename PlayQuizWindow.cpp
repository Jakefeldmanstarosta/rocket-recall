#include "PlayQuizWindow.h"
#include <QMessageBox>

PlayQuizWindow::PlayQuizWindow(const Quiz &quiz, QWidget *parent)
    : QWidget(parent), m_quiz(quiz)
{
    m_answers.resize(m_quiz.questionCount(), -1);

    auto *mainLayout = new QVBoxLayout(this);

    questionLabel = new QLabel();
    questionLabel->setWordWrap(true);
    questionLabel->setStyleSheet("font-size: 18px; font-weight: bold;");
    mainLayout->addWidget(questionLabel);

    choicesLayout = new QVBoxLayout();
    mainLayout->addLayout(choicesLayout);

    choiceButtons = new QButtonGroup(this);
    choiceButtons->setExclusive(true);

    auto *nav = new QHBoxLayout();
    backBtn = new QPushButton("Back");
    nextBtn = new QPushButton("Next");
    nav->addWidget(backBtn);
    nav->addWidget(nextBtn);
    mainLayout->addLayout(nav);

    connect(backBtn, &QPushButton::clicked, this, [this]() {
        int checked = choiceButtons->checkedId();
        if (checked != -1)
            m_answers[m_index] = checked;
        if (m_index > 0) {
            m_index--;
            loadQuestion();
        }
    });

    connect(nextBtn, &QPushButton::clicked, this, [this]() {
        int checked = choiceButtons->checkedId();
        if (checked != -1)
            m_answers[m_index] = checked;

        if (m_index + 1 >= m_quiz.questionCount()) {
            finishQuiz();
        } else {
            m_index++;
            loadQuestion();
        }
    });

    loadQuestion();
}

void PlayQuizWindow::loadQuestion()
{
    const Question &q = m_quiz.getQuestion(m_index);

    questionLabel->setText(
        QString("Q%1: %2").arg(m_index + 1)
            .arg(QString::fromStdString(q.getPrompt()))
        );

    QLayoutItem *child;
    while ((child = choicesLayout->takeAt(0)) != nullptr) {
        if (child->widget())
            child->widget()->deleteLater();
        delete child;
    }

    choiceButtons->setExclusive(false);
    for (auto *b : choiceButtons->buttons())
        choiceButtons->removeButton(b);
    choiceButtons->setExclusive(true);

    for (int i = 0; i < q.choiceCount(); i++) {
        QPushButton *btn = new QPushButton(
            QString::fromStdString(q.getChoice(i).getText()));
        btn->setCheckable(true);
        btn->setStyleSheet("font-size: 16px; padding: 6px;");
        choicesLayout->addWidget(btn);
        choiceButtons->addButton(btn, i);

        if (m_answers[m_index] == i)
            btn->setChecked(true);
    }

    backBtn->setEnabled(m_index > 0);
    nextBtn->setText(m_index + 1 == m_quiz.questionCount() ? "Finish" : "Next");
}

void PlayQuizWindow::finishQuiz()
{
    int score = 0;

    for (int i = 0; i < m_quiz.questionCount(); i++) {
        const Question &q = m_quiz.getQuestion(i);
        if (m_answers[i] == q.getCorrectIndex())
            score++;
    }

    QMessageBox::information(
        this,
        "Results",
        QString("You scored %1 out of %2")
            .arg(score)
            .arg(m_quiz.questionCount())
        );
}
