#ifndef UTILITIES_H
#define UTILITIES_H

#include <vector>

/**
 * Checks if a given value exists in the vector.
 */
bool exists(const std::vector<int> &v, int value);

bool edgeExists(const std::vector<int> &edges, int x, int y);

#endif
