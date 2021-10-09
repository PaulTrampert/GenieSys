//
// Created by paul.trampert on 12/11/2020.
//

#pragma once
#include <vector>
#include "./BitMask.h"


template <class T>
std::vector<T> getPossibleOpcodes(T base, GenieSys::BitMask<T>* field);

template<class T>
std::vector<T> getPossibleOpcodes(T base, std::vector<GenieSys::BitMask<T>*> fields, int i = 0);

template<class T>
std::vector<T> getPossibleOpcodes(T base, std::vector<GenieSys::BitMask<T>*> fields, int i) {
    std::vector<T> fieldResults = getPossibleOpcodes(base, fields[i]);
    if (i == fields.size() - 1) {
        return fieldResults;
    }
    std::vector<T> results;
    for (int j = 0; j < fieldResults.size(); j++) {
        T intermediateBase = fieldResults[j];
        std::vector<T> nextFieldPossibilities = getPossibleOpcodes(intermediateBase, fields, i + 1);
        results.insert(results.end(), nextFieldPossibilities.begin(), nextFieldPossibilities.end());
    }
    return results;
}
template<class T>
std::vector<T> getPossibleOpcodes(T base, GenieSys::BitMask<T>* field) {
    std::vector<T> results;
    for (int i = field->getMinValue(); i <= field->getMaxValue(); i++) {
        results.push_back(field->compose(base, i));
    }
    return results;
}

