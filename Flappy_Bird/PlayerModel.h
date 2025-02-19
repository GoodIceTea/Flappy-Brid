#ifndef FLAPPY_BIRD_PLAYERMODEL_H
#define FLAPPY_BIRD_PLAYERMODEL_H

#include <string>

/**
 * @brief Enumeracja reprezentująca dostępne modele gracza w grze Flappy Bird.
 */
enum class PlayerModel {
    Yellow,     /**< Żółty model gracza. */
    Blue,       /**< Niebieski model gracza. */
    Red         /**< Czerwony model gracza. */
};

/**
 * @brief Struktura przechowująca ścieżki do tekstur skrzydeł dla różnych stanów ptaka.
 */
struct pathModel {
    std::string wingUp;         /**< Ścieżka do tekstury skrzydeł w pozycji wzniosu. */
    std::string wingParallel;   /**< Ścieżka do tekstury skrzydeł w pozycji równoległej. */
    std::string wingDown;       /**< Ścieżka do tekstury skrzydeł w pozycji opadania. */
};

#endif //FLAPPY_BIRD_PLAYERMODEL_H
