#ifndef TEXTBOXMANAGER_H
#define TEXTBOXMANAGER_H

#include<SFML/Graphics.hpp>

enum EtextBoxManagerState {
    Hidden, MovingUp, Idle, Writting, MovingDown
};

class TextBoxManager {

public:
    TextBoxManager();

    static TextBoxManager* getTextBoxManager(){
        static TextBoxManager* m_pointer;
        if(! m_pointer) m_pointer = new TextBoxManager();
        return m_pointer;
    }

    void update(float dt);

    void displayText(std::string text);
    void hideText();
    void draw(sf::RenderTarget& window);


    EtextBoxManagerState getState();
private:

    void init();

    void updateText(float);
    void updateWhenWritting(float dt);

    bool finishedTextLecture();
    void prepareNewLine();

    EtextBoxManagerState m_state;

    int m_fontSize;
    int m_linesPerBox;
    int m_characterSize;
    int m_charactersPerLine;
    sf::Vector2i m_boxSize;
    sf::Vector2i m_writteableArea;

    int m_indexOverTotalText;
    int m_indexOfCurrentLine;

    std::string m_textCurrentlyDisplayed;
    std::string m_textToDisplay;
    std::string m_totalText;

    sf::Text m_text;
    sf::Font m_font;
    sf::Clock m_timer;

    sf::Sprite m_endOfBox;
    sf::Texture m_endOfBoxTexture;

    float m_timeBetweenLetters;

    sf::Vector2f m_anchorPoint;
    sf::Vector2f m_currentPosition;

    bool m_boxShouldBeHidden;

    float m_movementTime;
};

#endif // TEXTBOXMANAGER_H
