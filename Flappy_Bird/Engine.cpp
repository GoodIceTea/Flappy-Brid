#include "Engine.h"
#include "Bird.h"
#include "Pipe.h"
#include "PlayerModel.h"

/**
 * @file Engine.cpp
 * @brief Implementacja klasy Engine
 */

/**
 * @brief Inicjalizuje wartości zmiennych statycznych
 */
int Engine::score = 0; /**< Zlicza wynik */
bool Engine::gameRunning = false; /**< Flaga wskazująca, czy gra jest uruchomiona */
bool Engine::gameOvered = false; /**< Flaga wskazująca, czy gra jest zakończona */
Difficulty Engine::chosenDifficulty = Difficulty::Medium; /**< Wybrany poziom trudności */
float Engine::throatDifficulty = 340; /**< Wysokość rury */

/**
 * @brief Konstruktor klasy Engine
 */
Engine::Engine(){
    Engine::setup();
}

/**
 * @brief Destruktor klasy Engine
 */
Engine::~Engine()
{
    Engine::destroy();
}

/**
 * @brief Konfiguruje dźwięki gry
 */
void Engine::setupSounds() {
    pointSoundBuffer.loadFromFile("res/sounds/sfx_point.wav");
    pointSound.setBuffer(pointSoundBuffer);

    wingSoundBuffer.loadFromFile("res/sounds/sfx_wing.wav");
    wingSound.setBuffer(wingSoundBuffer);

    hitSoundBuffer.loadFromFile("res/sounds/sfx_hit.wav");
    hitSound.setBuffer(hitSoundBuffer);

    dieSoundBuffer.loadFromFile("res/sounds/sfx_die.wav");
    dieSound.setBuffer(dieSoundBuffer);
}

/**
 * @brief Inicjalizuje grę
 */
void Engine::setup() {
    inMainMenu = true;
    inGetReady = false;
    GetReadyFrame = false;
    gamePaused = false;

    srand((unsigned int)time(nullptr));
    pipeGeneratingClock = new sf::Clock;

    gameoverTexture = new sf::Texture();
    gameoverTexture->loadFromFile("res/textures/gameover.png");

    logoTexture = new sf::Texture();
    logoTexture->loadFromFile("res/textures/logo.png");
    logoSprite.setTexture(*logoTexture);
    startTexture = new sf::Texture();
    startTexture->loadFromFile("res/textures/start.png");
    startSprite.setTexture(*startTexture);

    getReadyTexture[0] = new sf::Texture();
    getReadyTexture[0]->loadFromFile("res/textures/get_ready/1.png");

    getReadyTexture[1] = new sf::Texture();
    getReadyTexture[1]->loadFromFile("res/textures/get_ready/2.png");

    font = new sf::Font();
    font->loadFromFile("res/fonts/04B_19__.TTF");

    window = new sf::RenderWindow(sf::VideoMode(450, 700), "Flappy Bird 1.1");
    window->setPosition({ 1000, 275 });

    b_skin = PlayerModel::Blue;
    bird = new Bird(b_skin, hitSound, dieSound);

    backgroundTexture = new sf::Texture();

    updateDifficulty(GetDifficulty());

    groundTexture = new sf::Texture();
    groundTexture->loadFromFile("res/textures/ground.png");

    sf::Image pipeImage;
    pipeImage.loadFromFile("res/textures/pipe.png");

    upperPipe = new sf::Texture();
    lowerPipe = new sf::Texture();
    upperPipe->loadFromImage(pipeImage);
    pipeImage.flipVertically();
    lowerPipe->loadFromImage(pipeImage);

    restartTexture = new sf::Texture();
    sf::Image restartImage;
    restartImage.loadFromFile("res/textures/restart.png");
    restartTexture->loadFromImage(restartImage);

    sf::Image coinImage;
    coinImage.loadFromFile("res/textures/coin.png");
    coin = new sf::Texture();
    coin->loadFromImage(coinImage);
    setupSounds();
}

/**
 * @brief Czyści grę
 */
void Engine::destroy() {
    delete window;
    delete bird;
    delete backgroundTexture;
    delete pipeGeneratingClock;
    delete font;
    delete getReadyTexture[0];
    delete getReadyTexture[1];
    delete gameoverTexture;
    delete logoTexture;
    delete startTexture;

    for (auto& pipe : pipes) { delete pipe; }
    pipes.clear();
}

/**
 * @brief Restartuje grę
 */
void Engine::restartGame() {
    pipes.clear();
    score = 0;
    inMainMenu = true;
    inGetReady = false;
    gameRunning = false;
    gameOvered = false;
    bird = new Bird(b_skin, hitSound, dieSound);
    SetGamePaused(false);
}

/**
 * @brief Aktualizuje stan gry
 */
void Engine::update() {
    bird->update(delta, backgroundTexture);
    for (const auto& pipe : pipes) {
        pipe->update(bird, delta);
    }
    if (gameRunning && !gameOvered) {
        if (pipeGeneratingClock->getElapsedTime().asSeconds() > 3.5) {
            pipeGeneratingClock->restart();
            pipes.push_back(new Pipe(pointSound, hitSound, upperPipe, lowerPipe, coin, window));
            if (pipes.size() > 4) {
                delete pipes[0];
                pipes.erase(pipes.begin());
            }
        }
    }
}

/**
 * @brief Przechwytuje zdarzenia
 * 
 * @param event zdarzenie z biblioteki SFML
 */
void Engine::handleEvent(sf::Event& event) {

    if (event.type == sf::Event::Closed) {
        window->close();
    }
    static bool wasBtnPressed = false;
    static bool wasBtnReleased = true;

    if (!inMainMenu && ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) ||
                        (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left))) {
        if (!wasBtnPressed && wasBtnReleased) {
            wasBtnPressed = true;
            wasBtnReleased = false;

            if (!gameRunning) {
                gameRunning = true;
                pipeGeneratingClock->restart();
                pipes.push_back(new Pipe(pointSound, hitSound, upperPipe, lowerPipe, coin, window));
            }
            bird->flap();
            if (wingSound.getStatus() != sf::Sound::Playing) {
                wingSound.play();
            }
        }
    }

    if ((startSprite.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y) && event.type == sf::Event::MouseButtonReleased) && inMainMenu) {
        inMainMenu = false;
        inGetReady = true;
    }

    if ((event.type == sf::Event::KeyPressed || event.type == sf::Event::MouseButtonPressed) && inGetReady) {
        inGetReady = false;
    }

    if (event.type == sf::Event::MouseButtonReleased && gameOvered) {
        sf::Sprite restartSprite;
        restartSprite.setTexture(*restartTexture);
        restartSprite.setPosition(window->getSize().x / 2 - restartSprite.getLocalBounds().width / 2, window->getSize().y / 2 - restartSprite.getLocalBounds().height / 2);

        if (restartSprite.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
            restartGame();
        }
    }

    if ((event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Space) ||
        (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)) {
        wasBtnPressed = false;
        wasBtnReleased = true;
    }

}

/**
 * @brief Rysuje obiekty na ekranie
 */
void Engine::draw() {

    window->clear();
    window->draw(sf::Sprite(*backgroundTexture));

    for (const auto& pipe : pipes) {
        pipe->draw(*window);
    }

    sf::Sprite groundSprite(*groundTexture);
    if (!(not gameRunning || gameOvered)) {
        groundOffset -= delta * 100;
        if (groundOffset <= -24) {
            groundOffset += 24;
        }
    }
    groundSprite.setPosition(groundOffset, backgroundTexture->getSize().y);
    window->draw(groundSprite);

    if(inMainMenu)
        ShowMainMenu();

    if(inGetReady)
    {
        static sf::Clock clk;
        if(clk.getElapsedTime().asMilliseconds() > 400)
        {
            GetReadyFrame = !GetReadyFrame;
            clk.restart();
        }
        ShowGetReady(GetReadyFrame);
    }

    sf::RectangleShape lowerRectangle({
                                              (float)window->getSize().x,
                                              (float)window->getSize().y - backgroundTexture->getSize().y - groundTexture->getSize().y
                                      });
    lowerRectangle.setPosition(0, (float)backgroundTexture->getSize().y + groundTexture->getSize().y);
    lowerRectangle.setFillColor({ 245, 228, 138 });
    window->draw(lowerRectangle);

    bird->draw(*window);

    if(!inMainMenu && !inGetReady)
    {
        sf::Text scoreText("Score: " + to_string(score), *font);
        scoreText.setPosition(window->getSize().x / 2 - scoreText.getLocalBounds().width / 2, 5);
        window->draw(scoreText);
    }

    if (gameOvered) {
        // Draw the restart button only after death
        gameoverSprite.setTexture(*gameoverTexture);
        gameoverSprite.setPosition(window->getSize().x / 2 - gameoverSprite.getLocalBounds().width / 2, window->getSize().y / 4 - gameoverSprite.getLocalBounds().height / 4);



        sf::Sprite restartSprite(*restartTexture);
        restartSprite.setPosition(window->getSize().x / 2 - restartSprite.getLocalBounds().width / 2, window->getSize().y / 2 - restartSprite.getLocalBounds().height / 2);
        window->draw(gameoverSprite);
        window->draw(restartSprite);
    }

    window->display();
}

/**
 * @brief Funkcja główna obsługująca grę
 */
void Engine::Play(){
    sf::Clock deltaClock;
    while (window->isOpen()) {
        //if(GetScore() > 10) updateDifficulty(Difficulty::Nightmare);

        sf::Event event{};

        while (window->pollEvent(event)) {
            handleEvent(event);
        }

        //if (!gamePaused) {
        delta = deltaClock.restart().asSeconds();
        update();
        //}
        draw();
    }
}

/**
 * @brief Wyświetla menu główne
 */
void Engine::ShowMainMenu()
{
    logoSprite.setPosition(window->getSize().x / 2 - logoSprite.getLocalBounds().width / 2, window->getSize().y / 4 - logoSprite.getLocalBounds().height / 4);
    window->draw(logoSprite);

    startSprite.setPosition(window->getSize().x / 2 - startSprite.getLocalBounds().width / 2, window->getSize().y / 2 - startSprite.getLocalBounds().height / 2);
    window->draw(startSprite);
}

/**
 * @brief Wyświetla "Get Ready"
 * 
 * @param idx Index odpowiedzialny za teksturę którą wyświetlić
 */
void Engine::ShowGetReady(bool idx)
{
    getReadySprite.setTexture(*getReadyTexture[static_cast<int>(idx)]);
    getReadySprite.setPosition(window->getSize().x / 2 - getReadySprite.getLocalBounds().width / 2, window->getSize().y / 2 - getReadySprite.getLocalBounds().height / 2);
    window->draw(getReadySprite);
}

/**
 * @brief Uruchamia grę
 */
void Engine::Run()
{
    Play();
    destroy();
}

/**
 * @brief Ustawia stan gry
 * 
 * @param state Odpwoiada za stan gry
 */
void Engine::SetGameRunning(bool state){
    Engine::gameRunning = state;
}

/**
 * @brief Ustawia stan zakończenia gry
 * 
 * @param state Nowy stan zakonczenia gry
 */
void Engine::SetGameOvered(bool state){
    Engine::gameOvered = state;
}

/**
 * @brief Pobiera aktualny stan gry
 * 
 * @return true if the game is running, false otherwise
 */
bool Engine::isGameRunning(){
    return Engine::gameRunning;
}

/**
 * @brief Pobiera aktualny stan gameOvered
 * 
 * @return true if the game is over, false otherwise
 */
bool Engine::isGameOvered(){
    return Engine::gameOvered;
}

/**
 * @brief Pobiera aktualny wynik
 * 
 * @return Aktualny wynik
 */
int Engine::GetScore(){
    return Engine::score;
}

/**
 * @brief Ustanawia aktualny wynik
 * 
 * @param __score Nowy wynik
 */
void Engine::SetScore(int __score){
    Engine::score = __score;
}

/**
 * @brief Ustawia trudność gry
 * 
 * @param __diff Nowa trudność gry
 */
void Engine::SetDifficulty(Difficulty __diff){
    Engine::chosenDifficulty = __diff;
}

/**
 * @brief Pobiera aktualną trudność
 * 
 * @return Aktualna trudność gry
 */
Difficulty Engine::GetDifficulty(){
    return Engine::chosenDifficulty;
}

/**
 * @brief Ustanawia trudność "gardła" rury
 * 
 * @param __throat Nowa trudność "gardła"
 */
void Engine::SetThroatDifficulty(float __throat)
{
    Engine::throatDifficulty = __throat;
}

/**
 * @brief Pobiera aktualną trudność "gardła"
 * 
 * @return Aktualna trudność "gardła"
 */
float Engine::GetThroatDifficulty(){
    return Engine::throatDifficulty;
}

/**
 * @brief Aktualizuje trudność gry
 * 
 * @param diff Nowa trudność gry
 */
void Engine::updateDifficulty(Difficulty diff) {
    SetDifficulty(diff);
    switch(chosenDifficulty)
    {
        case Difficulty::Easy:
            backgroundTexture->loadFromFile("res/textures/background/day.png");
            Engine::SetThroatDifficulty(380);
            break;
        case Difficulty::Medium:
            backgroundTexture->loadFromFile("res/textures/background/day.png");
            Engine::SetThroatDifficulty(340);
            break;
        case Difficulty::Hard:
            backgroundTexture->loadFromFile("res/textures/background/night.png");
            Engine::SetThroatDifficulty(320);
            break;
        case Difficulty::Nightmare:
            backgroundTexture->loadFromFile("res/textures/background/impossible.png");
            Engine::SetThroatDifficulty(315);
            break;
        default:
            backgroundTexture->loadFromFile("res/textures/background/day.png");
            Engine::SetThroatDifficulty(340);
    }
}
