/**
 * @file bird.h
 * @brief Definicja klasy Bird.
 */

#pragma once
#ifndef BIRD_H
#define BIRD_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include "PlayerModel.h"

using namespace std;

/**
 * @class Bird
 * @brief Klasa reprezentująca ptaka w grze.
 */
class Bird {
private:
    pathModel b_skin; /**< Model ścieżki tekstur ptaka */
    sf::Texture* texture; /**< Aktualna tekstura ptaka */
    float y; /**< Pozycja Y ptaka */
    float vel; /**< Prędkość ptaka */
    float currentFrame; /**< Aktualna klatka animacji */
    sf::Clock* animationClock; /**< Zegar animacji */
    vector<sf::Texture*> frames; /**< Klatki animacji ptaka */
    sf::Sound& hitSound; /**< Dźwięk uderzenia */
    sf::Sound& dieSound; /**< Dźwięk śmierci */
    bool hitSound_temp; /**< Flaga dla dźwięku uderzenia */
    bool dieSound_temp; /**< Flaga dla dźwięku śmierci */

public:
    /**
     * @brief Konstruktor klasy Bird.
     * @param skin Model gracza.
     * @param hitSound Dźwięk uderzenia.
     * @param dieSound Dźwięk śmierci.
     */
    Bird(PlayerModel skin, sf::Sound& hitSound, sf::Sound& dieSound);

    /**
     * @brief Destruktor klasy Bird.
     */
    ~Bird();

    /**
     * @brief Pobiera prostokąt kolizji ptaka.
     * @return Prostokąt kolizji ptaka.
     */
    sf::FloatRect getRect();

    /**
     * @brief Umożliwia ptakowi machnięcie skrzydłami.
     */
    void flap();

    /**
     * @brief Rysuje ptaka na oknie.
     * @param window Okno renderowania.
     */
    void draw(sf::RenderWindow& window);

    /**
     * @brief Aktualizuje stan ptaka.
     * @param delta Czas od ostatniej aktualizacji.
     * @param backgroundTexture Tekstura tła.
     */
    void update(float delta, sf::Texture* backgroundTexture);

    /**
     * @brief Pobiera model ścieżki dla podanego modelu gracza.
     * @param model Model gracza.
     * @return Struktura pathModel zawierająca ścieżki do tekstur ptaka.
     */
    pathModel getPathModel(PlayerModel model);
};

#endif
