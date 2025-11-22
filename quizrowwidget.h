#pragma once
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QWidget>

class QuizRowWidget : public QWidget
{
    Q_OBJECT
public:
    QuizRowWidget(const QString &quizName, QWidget *parent = nullptr)
        : QWidget(parent)
    {
        auto *layout = new QHBoxLayout(this);

        nameLabel = new QLabel(quizName);
        playButton = new QPushButton("Play");
        editButton = new QPushButton("Edit");

        layout->addWidget(nameLabel);
        layout->addStretch();
        layout->addWidget(playButton);
        layout->addWidget(editButton);

        connect(playButton, &QPushButton::clicked, this, [this]() {
            emit playQuiz(nameLabel->text());
        });

        connect(editButton, &QPushButton::clicked, this, [this]() {
            emit editQuiz(nameLabel->text());
        });
    }

signals:
    void playQuiz(const QString &quizName);
    void editQuiz(const QString &quizName);

private:
    QLabel *nameLabel;
    QPushButton *playButton;
    QPushButton *editButton;
};
