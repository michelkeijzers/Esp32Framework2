#pragma once

#include "INode.hpp"

/**
 * Abstract base class for all nodes.
 * Derived classes (MasterNode, SlaveNode) must implement init() and start().
 */
class Node : public INode
{
public:
    Node() = default;
    virtual ~Node() = default;

    virtual esp_err_t init() = 0;
    virtual esp_err_t start() = 0;
};
