
#ifndef QUIZMANAGER_H
#define QUIZMANAGER_H

#include <string>
#include <vector>
#include <iostream>

// QUIZ MANAGER CLASS - Handles all the coding challenges and questions
// Think of this as the "test generator" for the game
class QuizManager {
private:
    std::vector<std::string> questions;  // List of all quiz questions
    std::vector<std::string> answers;    // Correct answers (A, B, C, or D)
    std::vector<std::vector<std::string>> choices;  // Multiple choice options
    
public:
    // Constructor - sets up all the quiz questions
    QuizManager();
    
    // Present a quiz question to the player
    bool presentQuiz(int questionIndex, const std::string& playerAnswer);
    
    // Get the question text
    std::string getQuestion(int questionIndex) const;
    
    // Get the multiple choice options
    std::vector<std::string> getChoices(int questionIndex) const;
    
    // Get total number of questions
    int getQuestionCount() const;
};

#endif
