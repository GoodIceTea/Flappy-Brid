#include "Pipe.h"
#include "Engine.h"

/**
 * @brief Konstruktor klasy Pipe.
 *
 * Inicjalizuje obiekt rury (przeszkody) w grze Flappy Bird.
 *
 * @param pointSound Dźwięk zdobycia punktu.
 * @param hitSound Dźwięk uderzenia w przeszkodę.
 * @param upperPipe Tekstura rury górnej.
 * @param lowerPipe Tekstura rury dolnej.
 * @param coin Tekstura monety.
 * @param window Wskaźnik na okno renderowania SFML.
 */
Pipe::Pipe(sf::Sound &pointSound, sf::Sound &hitSound, sf::Texture* upperPipe, sf::Texture* lowerPipe, sf::Texture* coin, sf::RenderWindow* window):
        pointSound(pointSound), hitSound(hitSound), upperPipe(upperPipe), lowerPipe(lowerPipe), coin(coin) {
    x = (float)(window->getSize().x + upperPipe->getSize().x);
    y = 100.0f + (float)(rand() % 5 - 3) * 50;
    scored = false;
    coinVisible = true;
    h_difference = Engine::GetThroatDifficulty();
}

/**
 * @brief Zwraca prostokąt kolizji dla górnej rury.
 *
 * @return Prostokąt kolizji górnej rury.
 */
sf::FloatRect Pipe::getUpperRect() const {
    auto size = upperPipe->getSize();
    return {
            x, y + h_difference,
            (float)size.x, (float)size.y
    };
}

/**
 * @brief Zwraca prostokąt kolizji dla dolnej rury.
 *
 * @return Prostokąt kolizji dolnej rury.
 */
sf::FloatRect Pipe::getLowerRect() const {
    auto size = upperPipe->getSize();
    return {
            x, y - h_difference,
            (float)size.x, (float)size.y
    };
}

/**
 * @brief Zwraca prostokąt kolizji dla monety.
 *
 * @return Prostokąt kolizji monety.
 */
sf::FloatRect Pipe::getCoinRect() const {
    auto size = coin->getSize();
    return {
            x, y + (h_difference / 2),
            (float)size.x, (float)size.y
    };
}

/**
 * @brief Rysuje rurę (przeszkodę) oraz monetę na ekranie.
 *
 * @param window Referencja do okna renderowania SFML.
 */
void Pipe::draw(sf::RenderWindow& window) const {
    sf::Sprite upperSprite(*upperPipe);
    upperSprite.setPosition(x, y + h_difference);
    sf::Sprite lowerSprite(*lowerPipe);
    lowerSprite.setPosition(x, y - h_difference);
    window.draw(upperSprite);
    window.draw(lowerSprite);

    // Wyświetlanie monety w zależności od poziomu trudności
    if(Engine::GetDifficulty()==Difficulty::Easy) {
        if (coinVisible) {
            sf::Sprite coinSprite(*coin);
            coinSprite.setPosition(x, y + (h_difference / 2));
            window.draw(coinSprite);
        }
    }
    if(Engine::GetDifficulty()==Difficulty::Medium) {
        if (coinVisible) {
            sf::Sprite coinSprite(*coin);
            coinSprite.setPosition(x, y + (h_difference / 1.8));
            window.draw(coinSprite);
        }
    }
    if(Engine::GetDifficulty()==Difficulty::Hard) {
        if (coinVisible) {
            sf::Sprite coinSprite(*coin);
            coinSprite.setPosition(x, y + (h_difference / 1.7));
            window.draw(coinSprite);
        }
    }
    if(Engine::GetDifficulty()==Difficulty::Nightmare) {
        if (coinVisible) {
            sf::Sprite coinSprite(*coin);
            coinSprite.setPosition(x, y + (h_difference / 1.5));
            window.draw(coinSprite);
        }
    }
}

/**
 * @brief Aktualizuje pozycję rury oraz sprawdza kolizje z graczem.
 *
 * @param bird Wskaźnik do obiektu gracza (ptaka).
 * @param delta Czas od ostatniej klatki, używany do obliczeń czasowych.
 */
void Pipe::update(Bird *bird, float delta) {
    if (!Engine::isGameRunning() || Engine::isGameOvered()) return;

    x -= 100 * delta;
    auto birdRect = bird->getRect();

    // Sprawdzanie kolizji z graczem (ptakiem)
    if (birdRect.intersects(getUpperRect()) or birdRect.intersects(getLowerRect())) {
        Engine::SetGameOvered(true);
        hitSound.play();
    }

    // Sprawdzanie zdobycia monety przez gracza
    if (birdRect.intersects(getCoinRect()) && coinVisible) {
        coinVisible = false;
        Engine::SetScore(Engine::GetScore() + 1);
        pointSound.play();
    }

    // Oznaczanie zdobytego punktu, gdy gracz minie rurę
    if (x + upperPipe->getSize().x < birdRect.left and not scored) {
        scored = true;
    }
}
