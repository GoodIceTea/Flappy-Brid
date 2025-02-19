/**
 * @file bird.cpp
 * @brief Implementacja klasy Bird.
 */

#include "Bird.h"
#include "Engine.h"
#include "PlayerModel.h"

/**
 * @brief Pobiera model ścieżki dla podanego modelu gracza.
 * @param model Model gracza.
 * @return Struktura pathModel zawierająca ścieżki do tekstur ptaka.
 */
pathModel Bird::getPathModel(PlayerModel model)
{
    switch(model)
    {
        case PlayerModel::Yellow:
            return {"res/textures/bird/1-1.png", "res/textures/bird/1-2.png", "res/textures/bird/1-3.png"};
        case PlayerModel::Blue:
            return {"res/textures/bird/2-1.png", "res/textures/bird/2-2.png", "res/textures/bird/2-3.png"};
        case PlayerModel::Red:
            return {"res/textures/bird/3-1.png", "res/textures/bird/3-2.png", "res/textures/bird/3-3.png"};
        default:
            return {"res/textures/bird/1-1.png", "res/textures/bird/1-2.png", "res/textures/bird/1-3.png"};
    }
}

/**
 * @brief Konstruktor klasy Bird.
 * @param skin Model gracza.
 * @param hitSound Dźwięk uderzenia.
 * @param dieSound Dźwięk śmierci.
 */
Bird::Bird(PlayerModel skin, sf::Sound& hitSound, sf::Sound& dieSound) : hitSound(hitSound), dieSound(dieSound) {
    animationClock = new sf::Clock;
    b_skin = getPathModel(skin);

    for (const auto& path : {b_skin.wingParallel, b_skin.wingDown, b_skin.wingParallel, b_skin.wingUp}) {
        auto frame = new sf::Texture();
        frame->loadFromFile(path);
        frames.push_back(frame);
    }
    texture = frames[0];

    y = 400;
    vel = 0;
    hitSound_temp = false;
    dieSound_temp = false;
}

/**
 * @brief Destruktor klasy Bird.
 */
Bird::~Bird() {
    for (const auto& ptr : frames) {
        delete ptr;
    }
    delete animationClock;
}

/**
 * @brief Pobiera prostokąt kolizji ptaka.
 * @return Prostokąt kolizji ptaka.
 */
sf::FloatRect Bird::getRect() {
    auto size = texture->getSize();
    return {50, y, (float)size.x, (float)size.y};
}

/**
 * @brief Umożliwia ptakowi machnięcie skrzydłami.
 */
void Bird::flap(){
    if (!Engine::isGameRunning() || Engine::isGameOvered()) return;
    vel = -420;
}

/**
 * @brief Rysuje ptaka na oknie.
 * @param window Okno renderowania.
 */
void Bird::draw(sf::RenderWindow& window) {
    sf::Sprite birdSprite(*texture);
    birdSprite.setRotation(8 * (vel / 400));
    birdSprite.setPosition(50, y);
    window.draw(birdSprite);
}

/**
 * @brief Aktualizuje stan ptaka.
 * @param delta Czas od ostatniej aktualizacji.
 * @param backgroundTexture Tekstura tła.
 */
void Bird::update(float delta, sf::Texture* backgroundTexture) {
    currentFrame += delta * 4;
    if (currentFrame > frames.size()) {
        currentFrame -= frames.size();
    }
    texture = frames[(int)currentFrame];
    if (Engine::isGameRunning()) {
        vel += delta * 1200;
        y += vel * delta;

        if (y < 0 or y + texture->getSize().y > backgroundTexture->getSize().y) {
            if (hitSound.getStatus() != sf::Sound::Playing && !hitSound_temp) {
                hitSound.play();
                hitSound_temp = true;
            }
            if (hitSound.getStatus() != sf::Sound::Playing && !dieSound_temp) {
                dieSound.play();
                dieSound_temp = true;
            }
            Engine::SetGameOvered(true);
        }

        if (y + texture->getSize().y > backgroundTexture->getSize().y) {
            y = (float)backgroundTexture->getSize().y - texture->getSize().y;
            vel = 0;
        }
    }
}
