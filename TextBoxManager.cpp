#include "TextBoxManager.hpp"
#include <iostream>
#include <cmath>

#define MOVINGSPEED 1000

TextBoxManager::TextBoxManager() :
    m_fontSize(60)
    , m_boxSize(800,250)
    , m_writteableArea(700, 200)

    , m_indexOverTotalText(0)

    , m_textCurrentlyDisplayed("")
    , m_textToDisplay("")
    , m_totalText("_Default Text_ \n _Pre Defined_")

    , m_timeBetweenLetters(0.005)

    , m_anchorPoint(-666,-666)
    , m_currentPosition(-666,-666)
    , m_boxShouldBeHidden(true)

    , m_movementTime(0)
{
    m_state = EtextBoxManagerState::Hidden;

    m_timer.restart();
    init();
}

void TextBoxManager::init() {
    m_linesPerBox = 4;
    m_characterSize = 15;
    m_charactersPerLine = std::floor(static_cast<float>(m_writteableArea.x)/static_cast<float>(m_characterSize));

    if(! m_font.loadFromFile("res/fonts/font.ttf") ) { std::cout << "haver estudiao on texboxmanager init" << std::endl; exit(1);}
    m_text.setFont(m_font);
    m_text.setCharacterSize(m_fontSize);
    m_text.setColor(sf::Color::Yellow);
}

EtextBoxManagerState TextBoxManager::getState(){
    return m_state;
}
void TextBoxManager::update(float dt) {

    if(m_anchorPoint.x == -666){
        //encara no ha pogut agafar al posició de la finestra així que no fa res
        return;
    }

    switch (m_state) {
    case EtextBoxManagerState::Writting:
        updateWhenWritting(dt);
        if(m_indexOverTotalText-m_textToDisplay.size() >= m_totalText.size()
                && sf::Keyboard::isKeyPressed(sf::Keyboard::Space)){
            m_state = EtextBoxManagerState::Idle;
        }
        break;
    case EtextBoxManagerState::Hidden:
        if(!m_boxShouldBeHidden){
           m_state = EtextBoxManagerState::MovingUp;
        } else {
            m_totalText = "";
        }
        break;
    case EtextBoxManagerState::MovingUp:
        if (m_currentPosition.y > m_anchorPoint.y-m_boxSize.y) {
            m_movementTime += dt;
            m_currentPosition.y -= MOVINGSPEED*dt;
        }else {
            m_currentPosition.y = m_anchorPoint.y-m_boxSize.y;
            m_movementTime = 0;
            m_state = EtextBoxManagerState::Writting;
        }
        break;
    case EtextBoxManagerState::MovingDown:
        if (m_currentPosition.y < m_anchorPoint.y) {
            m_movementTime += dt;
            m_currentPosition.y += (MOVINGSPEED*dt);
        }else {
            m_currentPosition.y = m_anchorPoint.y;
            m_movementTime = 0;
            m_state = EtextBoxManagerState::Hidden;
        }
        break;
    case EtextBoxManagerState::Idle:
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space)){
            m_boxShouldBeHidden = true;
        }
        if(m_boxShouldBeHidden){
           m_state = EtextBoxManagerState::MovingDown;
        }
        break;
    default:
        std::cout << "you left cases unatended on texboxmanager update switchcase" << std::endl;
        exit(1);
        break;
    }
}

void TextBoxManager::updateWhenWritting(float dt){

    if(static_cast<int>(m_textCurrentlyDisplayed.size()) < m_charactersPerLine * std::min(m_indexOfCurrentLine, m_linesPerBox)){
        if(m_timer.getElapsedTime().asSeconds() > m_timeBetweenLetters){
            m_timer.restart();
            updateText(dt);
        }
    } else {
        ++m_indexOfCurrentLine;
        if(m_indexOfCurrentLine > m_linesPerBox && m_indexOverTotalText < static_cast<int>(m_totalText.size())) {
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space)){
                m_indexOfCurrentLine = 1;
                prepareNewLine();
                m_textCurrentlyDisplayed = "";
            }
         } else{
            m_textCurrentlyDisplayed.push_back('\n');
            prepareNewLine();
        }
    }
}

void TextBoxManager::updateText(float /*dt*/) {
    if (m_textToDisplay.size() > 0){
        m_textCurrentlyDisplayed.push_back(m_textToDisplay[0]);
        m_textToDisplay.erase(m_textToDisplay.begin());
    }
}

void TextBoxManager::displayText(std::string text){
    if(m_totalText == text) return;
    m_totalText = text;
    m_indexOfCurrentLine = 1;
    m_indexOverTotalText = 0;

    m_textCurrentlyDisplayed = "";
    m_textToDisplay = "";

    prepareNewLine();

    m_boxShouldBeHidden = false;
}

void TextBoxManager::draw(sf::RenderTarget& window){

    if(m_anchorPoint.x == -666){
        m_anchorPoint.x = window.getSize().x/2.0f - m_boxSize.x/2.0f;
        m_anchorPoint.y = window.getSize().y;
        m_currentPosition = m_anchorPoint;
    }

    sf::RectangleShape backgroundBox(sf::Vector2f(m_boxSize.x, m_boxSize.y));
    backgroundBox.setFillColor(sf::Color(120,120,120,120));
    backgroundBox.setPosition(m_currentPosition);
    sf::View oldView = window.getView();
    window.setView(window.getDefaultView());
    window.draw(backgroundBox);

    const int margin_X = (m_boxSize.x - m_writteableArea.x)/2.f;
    const int margin_Y = (m_boxSize.y - m_writteableArea.y)/2.f;
    m_text.setPosition(m_currentPosition.x + margin_X, m_currentPosition.y + margin_Y);
    m_text.setString(m_textCurrentlyDisplayed);
    //std::cout << "_textcurrentlyDisplayed " << m_textCurrentlyDisplayed << std::endl;

    int widthInCharacters = std::min(static_cast<int>(m_textCurrentlyDisplayed.size()), m_charactersPerLine);
    widthInCharacters = std::max(widthInCharacters, 1); //prevent 0

    float factor = widthInCharacters*m_characterSize/m_text.getLocalBounds().width;
    /***********0 per cent hacky, totalment legit******************/
    /*btw, es perque no hi hagi microcanvis deguts a les comes*/
    int fixFactor = factor*20;
    factor = fixFactor/20.f;
    /********************** o no *********************************/
    m_text.setScale(factor, factor);
    window.draw(m_text);

    window.setView(oldView);
}

bool TextBoxManager::finishedTextLecture(){
    return static_cast<size_t>(m_indexOverTotalText) >= m_totalText.size();
}

void TextBoxManager::prepareNewLine(){
    m_textToDisplay = "";

    if(finishedTextLecture()) return;

    for(int i = 0; i < m_charactersPerLine && !finishedTextLecture(); ++i){
        m_textToDisplay.push_back(m_totalText[m_indexOverTotalText]);
        ++m_indexOverTotalText;
    }

    while(!finishedTextLecture() && m_textToDisplay.size() > 0 && m_textToDisplay.back() != ' '){
        --m_indexOverTotalText;
        m_textToDisplay.pop_back();
    }

    if(m_textToDisplay.size() < static_cast<size_t>(m_charactersPerLine)){ //fill line with empty spaces
        for(int i = m_textToDisplay.size(); i < m_charactersPerLine; ++i){
            m_textToDisplay.push_back(' ');
        }
    }

}

void TextBoxManager::hideText() {
    m_state = EtextBoxManagerState::MovingDown;
    m_boxShouldBeHidden = true;
}
