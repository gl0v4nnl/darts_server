#pragma once

#include <string>
#include "../sdk.h"

class Player {
    int id;
    std::string name;
    std::string picture;
    int played;
    double percent;
    int wins;
public:
    Player(int id, std::string name, std::string picture) :
        id(id), name(name), picture(picture), played(0), percent(0.0), wins(0) {};
    
    int GetId() const {
        return id;
    }

    std::string GetName() const {
        return name;
    }

    std::string GetPicture() const {
        return picture;
    }

    int GetPlayed() const {
        return played;
    }

    double GetPercent() const {
        return percent;
    }

    int GetWins() const {
        return wins;
    }

    void ChangeName(std::string name) {
        this->name = name;
    }

    void ChangePicture(std::string picture) {
        this->picture = picture;
    }

    void SetPlayed(int played) {
        this->played += played;
    }

    void SetPercent(double percent) {
        this->percent = percent;
    }

    void SetWins(int wins) {
        this->wins += wins;
    }
};