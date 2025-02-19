#pragma once
#ifndef ENGINE_H
#define ENGINE_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include "Pipe.h"
#include "Bird.h"
#include "Difficulty.h"
#include "PlayerModel.h"
#include <string>

/**
 * @brief Klasa silnika gry Flappy Bird.
 *
 * Klasa zarządza główną logiką gry, w tym grafiką, dźwiękiem, fizyką obiektów oraz interakcjami użytkownika.
 */
class Engine {
protected:
    static Difficulty chosenDifficulty; /**< Wybrany poziom trudności gry. */
    static float throatDifficulty; /**< Poziom trudności gry ustalony przez gracza. */
    static int score; /**< Aktualny wynik gracza. */
    static bool gameRunning, gameOvered; /**< Flaga określająca, czy gra jest w trakcie działania lub zakończona. */
    bool inMainMenu, inGetReady, GetReadyFrame, gamePaused; /**< Flagi stanów gry: menu główne, przygotowanie do rozpoczęcia, pauza. */
    float groundOffset; /**< Przesunięcie terenu gry (ziemi). */
    float delta; /**< Czas delta - czas od ostatniej klatki, używany do obliczeń fizycznych. */

    sf::RenderWindow* window; /**< Okno renderowania SFML. */

    sf::Texture* backgroundTexture; /**< Tekstura tła gry. */
    Bird *bird; /**< Obiekt reprezentujący postać ptaka w grze. */

    PlayerModel b_skin; /**< Model gracza (postać gracza). */

    std::vector<Pipe*> pipes; /**< Wektor przechowujący wskaźniki na rury (przeszkody) w grze. */

    sf::Texture* groundTexture; /**< Tekstura terenu gry (ziemi). */
    sf::Clock* pipeGeneratingClock; /**< Zegar do generowania rur w grze. */

    sf::Font* font; /**< Czcionka używana do wyświetlania tekstu w grze. */
    sf::SoundBuffer pointSoundBuffer; /**< Bufor dźwięku punktu zdobytego w grze. */

    sf::Sound pointSound; /**< Dźwięk punktu zdobytego w grze. */
    sf::SoundBuffer wingSoundBuffer; /**< Bufor dźwięku skrzydeł ptaka. */

    sf::Sound wingSound; /**< Dźwięk skrzydeł ptaka. */
    sf::SoundBuffer hitSoundBuffer; /**< Bufor dźwięku uderzenia przeszkody w grze. */

    sf::Sound hitSound; /**< Dźwięk uderzenia przeszkody w grze. */
    sf::SoundBuffer dieSoundBuffer; /**< Bufor dźwięku śmierci gracza. */

    sf::Sound dieSound; /**< Dźwięk śmierci gracza. */
    sf::Sprite logoSprite; /**< Sprite logo gry. */

    sf::Sprite startSprite; /**< Sprite przycisku rozpoczęcia gry. */
    sf::Sprite getReadySprite; /**< Sprite ekranu "Get Ready". */

    sf::Texture *getReadyTexture[2]; /**< Tekstura do wyświetlania ekranu "Get Ready". */
    sf::Sprite gameoverSprite; /**< Sprite ekranu końca gry. */

    sf::Texture *gameoverTexture; /**< Tekstura do wyświetlania ekranu końca gry. */
    sf::Texture *logoTexture; /**< Tekstura do wyświetlania logo gry. */

    sf::Texture *startTexture; /**< Tekstura do wyświetlania przycisku startowego. */

    sf::Texture* restartTexture; /**< Tekstura do wyświetlania przycisku restartu. */
    sf::Texture* upperPipe; /**< Tekstura rury górnej (przeszkody). */
    sf::Texture* lowerPipe; /**< Tekstura rury dolnej (przeszkody). */
    sf::Sprite coinSprite; /**< Sprite monety w grze. */

    sf::Texture* coin; /**< Tekstura monety w grze. */

public:
    /**
     * @brief Konstruktor klasy Engine.
     */
    Engine();

    /**
     * @brief Destruktor klasy Engine.
     */
    virtual ~Engine();

    /**
     * @brief Konfiguruje dźwięki gry.
     */
    void setupSounds();

    /**
     * @brief Przygotowuje silnik gry do rozpoczęcia nowej gry.
     */
    void setup();

    /**
     * @brief Niszczy zasoby używane przez silnik gry.
     */
    void destroy();

    /**
     * @brief Restartuje grę (po zakończeniu lub na żądanie).
     */
    void restartGame();

    /**
     * @brief Aktualizuje stan gry i fizykę obiektów w grze.
     */
    void update();

    /**
     * @brief Obsługuje zdarzenia generowane przez użytkownika (np. klawisze, mysz).
     *
     * @param event Zdarzenie SFML do obsłużenia.
     */
    void handleEvent(sf::Event& event);

    /**
     * @brief Renderuje obiekty gry na ekranie.
     */
    void draw();

    /**
     * @brief Rozpoczyna główną pętlę gry.
     */
    void Run();

    /**
     * @brief Rozpoczyna fazę gry.
     */
    void Play();

    /**
     * @brief Wyświetla menu główne gry.
     */
    void ShowMainMenu();

    /**
     * @brief Wyświetla ekran "Get Ready".
     *
     * @param idx Indeks do wyboru tekstury "Get Ready".
     */
    void ShowGetReady(bool idx);

    /**
     * @brief Ustawia stan działania gry (uruchomiona lub zatrzymana).
     *
     * @param state Nowy stan gry.
     */
    static void SetGameRunning(bool state);

    /**
     * @brief Ustawia stan zakończenia gry.
     *
     * @param state Nowy stan zakończenia gry.
     */
    static void SetGameOvered(bool state);

    /**
     * @brief Sprawdza, czy gra jest uruchomiona.
     *
     * @return true jeśli gra jest uruchomiona.
     * @return false jeśli gra nie jest uruchomiona.
     */
    static bool isGameRunning();

    /**
     * @brief Sprawdza, czy gra jest zakończona.
     *
     * @return true jeśli gra jest zakończona.
     * @return false jeśli gra nie jest zakończona.
     */
    static bool isGameOvered();

    /**
     * @brief Zwraca stan pauzy gry.
     *
     * @return true jeśli gra jest w trybie pauzy.
     * @return false jeśli gra nie jest w trybie pauzy.
     */
    bool GetGamePaused(){return gamePaused;};

    /**
     * @brief Ustawia stan pauzy gry.
     *
     * @param __paused Nowy stan pauzy gry.
     */
    void SetGamePaused(bool __paused){gamePaused = __paused;};

    /**
     * @brief Zwraca aktualny wynik gry.
     *
     * @return Aktualny wynik gracza.
     */
    static int GetScore();

    /**
     * @brief Ustawia nowy wynik gry.
     *
     * @param __score Nowy wynik gracza.
     */
    static void SetScore(int __score);

    /**
     * @brief Ustawia poziom trudności gry.
     *
     * @param __diff Nowy poziom trudności.
     */
    static void SetDifficulty(Difficulty __diff);

    /**
     * @brief Zwraca aktualnie ustawiony poziom trudności gry.
     *
     * @return Aktualny poziom trudności gry.
     */
    static Difficulty GetDifficulty();

    /**
     * @brief Aktualizuje poziom trudności gry na podstawie nowego ustawienia.
     *
     * @param __diff Nowy poziom trudności do zaktualizowania.
     */
    void updateDifficulty(Difficulty __diff);

    /**
     * @brief Ustawia poziom trudności fizycznej przeszkód w grze.
     *
     * @param __throat Nowy poziom trudności fizycznej przeszkód.
     */
    static void SetThroatDifficulty(float __throat);

    /**
     * @brief Zwraca aktualnie ustawiony poziom trudności fizycznej przeszkód w grze.
     *
     * @return Aktualny poziom trudności fizycznej przeszkód.
     */
    static float GetThroatDifficulty();
};
#endif
