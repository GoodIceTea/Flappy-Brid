#pragma once
#ifndef PIPE_H
#define PIPE_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include "Bird.h"

/**
 * @brief Klasa reprezentująca pojedynczą rurę (przeszkodę) w grze Flappy Bird.
 *
 * Rura składa się z górnej i dolnej części oraz opcjonalnie monety.
 */
class Pipe {
public:
    /**
     * @brief Konstruktor klasy Pipe.
     *
     * Inicjalizuje obiekt rury (przeszkody) w grze.
     *
     * @param pointSound Dźwięk zdobycia punktu.
     * @param hitSound Dźwięk uderzenia w przeszkodę.
     * @param upperPipe Tekstura rury górnej.
     * @param lowerPipe Tekstura rury dolnej.
     * @param coin Tekstura monety.
     * @param window Wskaźnik na okno renderowania SFML.
     */
    Pipe(sf::Sound &pointSound, sf::Sound &hitSound, sf::Texture* upperPipe, sf::Texture* lowerPipe, sf::Texture* coin, sf::RenderWindow* window);

    /**
     * @brief Rysuje rurę (przeszkodę) oraz monetę na ekranie.
     *
     * @param window Referencja do okna renderowania SFML.
     */
    void draw(sf::RenderWindow& window) const;

    /**
     * @brief Aktualizuje pozycję rury oraz sprawdza kolizje z graczem.
     *
     * @param bird Wskaźnik do obiektu gracza (ptaka).
     * @param delta Czas od ostatniej klatki, używany do obliczeń czasowych.
     */
    void update(Bird *bird, float delta);

    /**
     * @brief Zwraca prostokąt kolizji dla górnej rury.
     *
     * @return Prostokąt kolizji górnej rury.
     */
    sf::FloatRect getUpperRect() const;

    /**
     * @brief Zwraca prostokąt kolizji dla dolnej rury.
     *
     * @return Prostokąt kolizji dolnej rury.
     */
    sf::FloatRect getLowerRect() const;

    /**
     * @brief Zwraca prostokąt kolizji dla monety.
     *
     * @return Prostokąt kolizji monety.
     */
    sf::FloatRect getCoinRect() const;

private:
    float x, y; /**< Pozycja x i y rury. */
    float h_difference; /**< Różnica wysokości pomiędzy górną a dolną częścią rury. */
    bool scored; /**< Flaga informująca, czy rura została już "zdobyta" przez gracza. */
    bool coinVisible; /**< Flaga informująca, czy moneta jest widoczna. */

    sf::Sound &pointSound; /**< Referencja do dźwięku zdobycia punktu. */
    sf::Sound &hitSound; /**< Referencja do dźwięku uderzenia w przeszkodę. */
    sf::Texture* upperPipe; /**< Tekstura rury górnej. */
    sf::Texture* lowerPipe; /**< Tekstura rury dolnej. */
    sf::Texture* coin; /**< Tekstura monety. */
};

#endif
