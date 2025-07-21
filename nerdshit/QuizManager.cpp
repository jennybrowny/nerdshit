
#include "QuizManager.h"

// CONSTRUCTOR - Sets up all the quiz questions and answers
QuizManager::QuizManager() {
    // ECE160 Python question
    questions.push_back("What does print(\"Hello World\") do in Python?");
    answers.push_back("A");
    choices.push_back({
        "A) Prints Hello World to the console",
        "B) Creates a variable named Hello World", 
        "C) Imports a library called Hello World",
        "D) Defines a function called Hello World"
    });
    
    // ECE323 algorithm complexity question
    questions.push_back("What is the average time complexity of quicksort?");
    answers.push_back("B");
    choices.push_back({
        "A) O(n)",
        "B) O(n log n)",
        "C) O(n^2)",
        "D) O(log n)"
    });
    
    // You can add more questions here later
}

// PRESENT QUIZ - Check if the player's answer is correct
bool QuizManager::presentQuiz(int questionIndex, const std::string& playerAnswer) {
    if (questionIndex >= 0 && questionIndex < questions.size()) {
        std::cout << "Question: " << questions[questionIndex] << std::endl;
        
        // Show all the choices
        for (const std::string& choice : choices[questionIndex]) {
            std::cout << choice << std::endl;
        }
        
        // Check if answer is correct
        bool correct = (playerAnswer == answers[questionIndex]);
        if (correct) {
            std::cout << "Correct! You're a massive nerd!" << std::endl;
        } else {
            std::cout << "Wrong! But don't worry, the curve will save you." << std::endl;
        }
        
        return correct;
    }
    return false;
}

// GET QUESTION - Return the question text
std::string QuizManager::getQuestion(int questionIndex) const {
    if (questionIndex >= 0 && questionIndex < questions.size()) {
        return questions[questionIndex];
    }
    return "No question available";
}

// GET CHOICES - Return the multiple choice options
std::vector<std::string> QuizManager::getChoices(int questionIndex) const {
    if (questionIndex >= 0 && questionIndex < choices.size()) {
        return choices[questionIndex];
    }
    return std::vector<std::string>();
}

// GET QUESTION COUNT - Return total number of questions
int QuizManager::getQuestionCount() const {
    return questions.size();
}
