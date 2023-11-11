#pragma once

#include <string>
#include <vector>
#include "../sdk.h"

class Round {
    int id;
    int players_count;
    std::vector<int> scores;
public:
    Round(int id, int players_count) : 
        id(id), players_count(players_count), scores(players_count) {};
    
    int GetId() const {
        return id;
    }

    int GetPlayersCount() const {
        return players_count;
    }

    void SetScore(int player_id, int score) {
        scores[player_id] = score;
    }
};