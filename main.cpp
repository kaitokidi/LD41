#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <fstream>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <stdio.h>
#include <Portada.hpp>
#include <TextBoxManager.hpp>

bool isWhite(sf::Image& image, float px, float py){
	return image.getPixel(px, py) == sf::Color::White;
}

float getAngle(sf::Vector2f &orig, sf::Vector2i &des) {
    return std::atan2(des.y - orig.y, des.x - orig.x)*180/(M_PI);
}

float getModule(const sf::Vector2f &orig, const sf::Vector2f &des) {
    return std::sqrt(std::pow(std::abs(des.x-orig.x), 2) + std::pow(std::abs(des.y-orig.y), 2));
}

int randomSignOne(){
    return 1 - 2*std::rand()%2;
}


float getAngle(const sf::Vector2f &orig,const sf::Vector2f &des) {
    return std::atan2(des.y - orig.y, des.x - orig.x)*180/(M_PI);
}

int main(){

    /* initialize random seed: */
    std::srand (time(NULL));

	//SFML OBJECTS
	sf::View view;
	sf::Event event;
	sf::Clock deltaClock;

    float deltatime = 0.0;

    sf::Font font;
    font.loadFromFile("res/font.otf");

    sf::Text text;
    text.setFont(font);

    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), L"LD41-AChristmasTree", sf::Style::Close);
    view.reset(sf::FloatRect(0,0,
                             window.getSize().x, window.getSize().y));

    window.setFramerateLimit(60);

    sf::Music m;
    m.openFromFile("res/Prokofiev_Vision_Fugitives_ Op_22Boris_Berman.ogg");
    m.setLoop(true);
    m.setVolume(75);
    m.play();

    TextBoxManager* textBoxManager = TextBoxManager::getTextBoxManager();

    Portada portada;
    portada.notAnimation();
    portada.display(&window, "res/portada0.png");
    portada.animation();
    portada.display(&window, "res/credits.png");
    portada.display(&window, "res/portada1.png");




    /*
    sf::SoundBuffer buffer;
    buffer.loadFromFile("res/attack.ogg");
    sf::Sound stepsound;
    stepsound.setBuffer(buffer);
*/

    std::string line;
    std::vector <std::string > m_textos;
    std::ifstream myfile ("res/story.txt");
    if (myfile.is_open()) {
        line = "DOS";

        while (line[0] != '$') {
            if(line[0] == '<'){
                m_textos.emplace_back(line);
            }
            std::getline (myfile,line);
            while (line[0] == '#') std::getline (myfile,line);
        }
        myfile.close();
    } else std::cout << "not file " << std::endl;




    std::vector < sf::Texture > m_textures(32);
    std::vector < sf::Sprite > m_sprites(32);
    std::string name = "res/tree/tree";


    for(int treeIndex = 0; treeIndex <= 27; ++treeIndex){
        m_textures[treeIndex].loadFromFile(name+std::to_string(treeIndex)+".png");
        m_sprites[treeIndex].setTexture(m_textures[treeIndex]);
    }
    m_sprites[28].setTexture(m_textures[27]);
    m_sprites[29].setTexture(m_textures[27]);

    bool introDone = false;
    bool animationChanged = true;
    int currentTreeIndex = 26;
    int currentTextIndex = currentTreeIndex;
    textBoxManager->displayText(m_textos[currentTextIndex]);
	while(window.isOpen()){
        animationChanged = false;
        //TODO


        deltatime = deltaClock.restart().asSeconds();


        textBoxManager->update(deltatime);


        while(window.pollEvent(event)){
			switch (event.type){
				//Close event
				case sf::Event::Closed:
					window.close();
                    break;
				case  sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }
                if (event.key.code == sf::Keyboard::N) {
                    if(textBoxManager->getState() == EtextBoxManagerState::Hidden)
                    {
                        animationChanged = true;
                        currentTreeIndex = std::min(29, currentTreeIndex +1);
                        currentTextIndex = std::min(int(m_textos.size()-1), currentTextIndex+1);
                        textBoxManager->displayText(m_textos[currentTextIndex]);
//                    textBoxManager->displayText("Because I'm an excelent penguin! Because I'm an excelent penguin! Because I'm an excelent penguin! and I will always be!!!, Because I'm an excelent penguin! Because I'm an excelent penguin! Because I'm an excelent penguin! and I will always be!!!");
                    }
                }
            case sf::Event::TouchEnded:
            {
                sf::Vector2f touchPos = sf::Vector2f(event.touch.x,event.touch.y);
                if(m_sprites[currentTreeIndex].getGlobalBounds().contains(touchPos)){
                    if(textBoxManager->getState() == EtextBoxManagerState::Hidden)
                    {
                        animationChanged = true;
                        currentTreeIndex = std::min(29, currentTreeIndex +1);
                        currentTextIndex = std::min(int(m_textos.size()-1), currentTextIndex+1);
                        textBoxManager->displayText(m_textos[currentTextIndex]);
//                    textBoxManager->displayText("Because I'm an excelent penguin! Because I'm an excelent penguin! Because I'm an excelent penguin! and I will always be!!!, Because I'm an excelent penguin! Because I'm an excelent penguin! Because I'm an excelent penguin! and I will always be!!!");
                    }
                }
            }
                //Default
				default:
					//Do nothing
					break;
			}
		}	

        window.clear();


        sf::View view;
        sf::Vector2i desiredSize(m_sprites[currentTreeIndex].getGlobalBounds().width, m_sprites[currentTreeIndex].getGlobalBounds().height);
        int windowX = window.getSize().x; int windowY = window.getSize().y;
        float xr = windowX / float(desiredSize.x);
        float yr = windowY / float(desiredSize.y);
        float aux;
        if (xr < yr) aux = 1/yr;
        else aux = 1/xr;
        xr *= aux;
        yr *= aux;
        sf::Vector2f min,max;
        min.x = (1.f - yr) / 2.f;
        min.y = (1.f - xr) / 2.f;
        max.x = 1.f - min.x*2.0;
        max.y = 1.f - min.y*2.0;
        view.reset(sf::FloatRect(0,0,desiredSize.x,desiredSize.y));
        view.setViewport(sf::FloatRect(min.x,min.y,max.x,max.y));
        window.setView(view);

        if(animationChanged){
            sf::Clock timer;
            sf::Sprite dark;
            sf::Texture textur;
            sf::Image black;
            black.create(window.getSize().x, window.getSize().y+200, sf::Color::Black);
            textur.loadFromImage(black);
            dark.setTexture(textur);
            dark.setOrigin(dark.getLocalBounds().width/2,dark.getLocalBounds().height/2);
            dark.setPosition(window.getSize().x/2,window.getSize().y/2);
            float time = 0;
            float alpha = 255;
            introDone = false;
            while(!introDone){
                dark.setColor(sf::Color(0,0,0,int(alpha)));
                time += timer.restart().asSeconds();
                if(time > 0.05){
                    alpha -= 15;
                    time = 0;
                }
                window.clear();
                window.draw(m_sprites[currentTreeIndex]);
                window.draw(dark);
                window.display();
                if(alpha <= 0) {
                    introDone = true;
                }
                while (window.pollEvent(event)) {
                    switch (event.type) {
                    case sf::Event::Closed:
                        window.close();
                        break;
                    case sf::Event::KeyPressed:
                        if (event.key.code == sf::Keyboard::Escape) window.close();
                        break;
                    default:
                        break;
                   }
                }
              }
         }

        window.draw(m_sprites[currentTreeIndex]);



        window.setView(window.getDefaultView());

        textBoxManager->draw(window);

        window.display();

	}

}
