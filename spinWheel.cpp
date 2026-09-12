#include <SFML/Graphics.hpp>
#include <optional>
#include <iostream>
#include <math.h>
class Wheel{


    float toRad(float deg) {
         return deg * 3.14159265f / 180.f;
    }
    public:
        Wheel() : spinButtonText(gameFont),
                   nameTexts{
                       sf::Text(gameFont),
                       sf::Text(gameFont),
                       sf::Text(gameFont),
                       sf::Text(gameFont),
                       sf::Text(gameFont),
                       sf::Text(gameFont),
                       sf::Text(gameFont)
                   }
       {
       }


    bool gamestate=true;
    bool spining=false;
    sf::Clock frameClock;

    float spinForce=300.f;
    float friction=75.f;
    float wheelRotation=0.f;


    float startAngle=-115.714f;
    float radius= 200.f;
    static const int segmentCount=7;

    std::array<std::string, segmentCount> photoPath;
    std::array<sf::Texture,segmentCount>photoTexture;
    std::array<std::optional<sf::Sprite>, segmentCount> photoSprite;


    float segmentAngle = 360.f/segmentCount;
    std::array<sf::ConvexShape,segmentCount>triangles;
    std::array<std::string, segmentCount> names = {"Roman", "Rudra", "Shreya","Anurag","Pratyush","Ram","Himanshu"};
    std::array<sf::Text,segmentCount> nameTexts;


    sf::Font gameFont;
    sf::Text spinButtonText;
    sf::RectangleShape spinButton;


    std::vector <sf::Color> colors = {
            sf::Color::Red,
            sf::Color::Green,
            sf::Color::Blue,
            sf::Color::Yellow,
            sf::Color::Magenta,
            sf::Color::Cyan,
            sf::Color(255, 165, 0) // orange
    };


    void setUp(){
        loadFont();
        loadPhoto();
        setUpTriangles();
        setUpPhotos();
        setUpSpinButton();
        setUpNames();
    }
    void handleEvents(sf::RenderWindow &window){
        while(const std::optional event = window.pollEvent()){
            if(event->is<sf::Event::Closed>()){
                handleClose(window);
                continue;
            }
                
            if(const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
                handleKeyBoard(*keyPressed);


            if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>())
                handleMouse(*mousePressed);
        }
    }


    void draw(sf::RenderWindow &window){
        window.clear(sf::Color::Black);
        drawWheel(window);
        drawNames(window);
        drawSpinButton(window);
        drawNames(window);
        drawPhotos(window);
    }


    void drawWheel(sf::RenderWindow &window){
        sf::Vector2f center(static_cast<float>(window.getSize().x) / 2.f,static_cast<float>(window.getSize().y) / 2.f);
        for(int i=0;i<segmentCount;i++){
            float x1=radius*(std::cos(toRad(startAngle+i*segmentAngle)));
            float y1 =radius*(std::sin(toRad(startAngle+i*segmentAngle)));;;
            float x2= radius*(std::cos(toRad(startAngle+(i+1)*segmentAngle)));;
            float y2= radius*(std::sin(toRad(startAngle+(i+1)*segmentAngle)));;;
            sf::Vector2f p1(x1,y1);
            sf::Vector2f p2(x2,y2);
            drawTriangle(window,i,p1,p2,colors[i % colors.size()]);
        }
    }


    void drawTriangle(sf::RenderWindow &window,int i,sf::Vector2f p1,sf::Vector2f p2, const sf::Color &color){
        sf::Vector2f center(static_cast<float>(window.getSize().x) / 2.f,static_cast<float>(window.getSize().y) / 2.f);
        triangles[i].setOrigin({0.f,0.f});
        triangles[i].setPoint(0,{0.f,0.f});
        triangles[i].setPoint(1,p1);
        triangles[i].setPoint(2,p2);
        triangles[i].setPosition(center);
        triangles[i].setFillColor(color);
        window.draw(triangles[i]);
    }


    void drawNames(sf::RenderWindow &window){
        for(int i=0;i<segmentCount;i++){
            window.draw(nameTexts[i]);
        }
    }

    void drawPhotos(sf::RenderWindow &window){
        for(int i=0;i<segmentCount;i++){
            window.draw(*photoSprite[i]);
        }
    }


    void drawSpinButton(sf::RenderWindow &window){
        window.draw(spinButton);
        window.draw(spinButtonText);
    }
    
    void update(){
        float dt=frameClock.restart().asSeconds();


        if(!spining) return;


        float rotationAmount=spinForce*dt;
        wheelRotation+=rotationAmount;

        for(int i=0;i<segmentCount;i++){triangles[i].rotate(sf::degrees(rotationAmount));}
        spinForce-=friction*dt;

        updateNames();
        updatePhotos();

        if(spinForce<=0){
            spining=false;
            frameClock.stop();
            spinForce=0.f;
        }
    }


    void startSpin(){
        if(!spining){
            frameClock.restart();
            spining=true;
            spinForce=300.f;
        }
    }


    bool isSpinButtonClicked(sf::Vector2f mousePosition){
        return spinButton.getGlobalBounds().contains(mousePosition);
    }


    void loadFont(){
        if (!gameFont.openFromFile("Assets/Fonts/PressStart2P-Regular.ttf"))
             std::cout << "failed to load font\n";
    }


    void setUpTriangles(){
        for(int i=0;i<segmentCount;i++){
            triangles[i].setPointCount(3);
            triangles[i].setFillColor(colors[i % colors.size()]);
        }
    }


    void setUpSpinButton(){
        spinButton.setSize({200.f,70.f});
        spinButton.setPosition({100.f,100.f});
        spinButton.setFillColor(sf::Color::Magenta);
        spinButton.setOutlineColor(sf::Color::White);
        spinButton.setOutlineThickness(2.f);


        spinButtonText.setString("Spin");
        spinButtonText.setCharacterSize(30);


        sf::FloatRect bounds = spinButtonText.getLocalBounds();
        spinButtonText.setOrigin(bounds.position + bounds.size / 2.f);
        spinButtonText.setPosition(spinButton.getPosition() + spinButton.getSize() / 2.f);
    }


    void setUpNames(){
        for(int i=0;i<segmentCount;i++){
            nameTexts[i].setString(names[i]);
            nameTexts[i].setCharacterSize(14);
            nameTexts[i].setFillColor(sf::Color::White);

            sf::FloatRect bounds = nameTexts[i].getLocalBounds();
            nameTexts[i].setOrigin(bounds.position + bounds.size / 2.f);
        }

        updateNames();
    }


    void updateNames(){
        sf::Vector2f center(500.f,400.f);
        float nameRadius=120.f;

        for(int i=0;i<segmentCount;i++){
            float middleAngle=startAngle+(i+0.5f)*segmentAngle+wheelRotation;

            float x=center.x+nameRadius*std::cos(toRad(middleAngle));
            float y=center.y+nameRadius*std::sin(toRad(middleAngle));

            nameTexts[i].setPosition({x,y});
        }
    }


    void handleClose(sf::RenderWindow &window){
        gamestate=false;
        window.close();
    }


    void handleKeyBoard(const sf::Event::KeyPressed &key){
        if(key.scancode == sf::Keyboard::Scancode::S){
            startSpin();
        }
    }


    void handleMouse(const sf::Event::MouseButtonPressed &mouse){
        if (mouse.button == sf::Mouse::Button::Left)
        {
            sf::Vector2f mousePosition(static_cast<float>(mouse.position.x),static_cast<float>(mouse.position.y));
            if(isSpinButtonClicked(mousePosition)){
                startSpin();
            }
        }
    }
    void setPhotoPath(){
        photoPath[0] = "Assets/Photos/Roman.png";
        photoPath[1] = "Assets/Photos/Rudra.png";
        photoPath[2] = "Assets/Photos/Shreya.png";
        photoPath[3] = "Assets/Photos/Anurag.png";
        photoPath[4] = "Assets/Photos/Pratyush.png";
        photoPath[5] = "Assets/Photos/Ram.png";
        photoPath[6] = "Assets/Photos/Himanshu.png";
    }

    void loadPhoto(){
        setPhotoPath();
        for(int i=0;i<segmentCount;i++){
            if (!photoTexture[i].loadFromFile(photoPath[i])){
                std::cout << "failed to load photo\n";
                return;
            }
            photoSprite[i].emplace(photoTexture[i]); 
        }
    }

    void setUpPhotos(){
        float targetSize = 100.f; 
        for(int i = 0; i < segmentCount; ++i){
            if (!photoSprite[i]) {
                continue;
            }
            sf::FloatRect bounds = photoSprite[i]->getLocalBounds();
            photoSprite[i]->setOrigin({bounds.position + bounds.size / 2.f});
            float maxSize = std::max(bounds.size.x, bounds.size.y);
            float scale = (maxSize > 0.f) ? (targetSize / maxSize) : 1.f;
            photoSprite[i]->setScale({scale, scale});
        }
        updatePhotos();
    }

    void updatePhotos(){
        sf::Vector2f center(500.f,400.f);
        float photoRadius=230.f;

        for(int i=0;i<segmentCount;i++){
            float middleAngle=startAngle+(i+0.5f)*segmentAngle+wheelRotation;

            float x=center.x+photoRadius*std::cos(toRad(middleAngle));
            float y=center.y+photoRadius*std::sin(toRad(middleAngle));

            photoSprite[i]->setPosition({x,y});
        }
    }

};
int main(){
    Wheel wheel;
    sf::RenderWindow window(sf::VideoMode({1000,800}),"Spinner");
    wheel.setUp();
    while(wheel.gamestate){
        wheel.handleEvents(window);
        wheel.update();
        wheel.draw(window);
        window.display();
    }
    return 0;
}