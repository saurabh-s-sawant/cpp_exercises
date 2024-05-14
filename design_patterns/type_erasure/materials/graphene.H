#pragma once

class Graphene {
public:
    Graphene(int uc) : _unitcells{uc} {}

    int get_unitcells() const { return _unitcells; }

private:
    int _unitcells;
};
