#pragma once

#include "catch.hpp"

#include "graph_generator.h"
#include "graph_io.h"

#include <cstdlib>

using namespace graphgen;
using namespace graphio;

constexpr float den(int e, int v, bool directed) {
    return (float)(directed ? 1 : 2) * (float)e / ((float)v * ((float)v - 1));
}

TEST_CASE("Graph Generator simple constraints") {
    CHECK_FALSE(generateConnectedGraph(-1, 1));
    CHECK_FALSE(generateConnectedGraph(0, 1.0001));
    CHECK_FALSE(generateConnectedGraph(1, -0.0001));
    CHECK(generateConnectedGraph(2, 0));
    CHECK(generateConnectedGraph(3, 0.5));
    CHECK(generateConnectedGraph(4, 1));
    CHECK(generateConnectedGraph(5, 0.25, true));
    CHECK(generateConnectedGraph(6, 0.75, false));
}

TEST_CASE("Graph Generator simple graph") {
    std::shared_ptr<Graph> g;

    g = generateConnectedGraph(1000, 0);
    CHECK(g->edges.size() == 1000);
    CHECK(g->destinations.size() == 0);
    CHECK(g->weights.size() == 0);

    g = generateConnectedGraph(1000, 0.001);
    CHECK(g->edges.size() == 1000);
    CHECK(g->destinations.size() == 999);
    CHECK(g->weights.size() == 999);

    g = generateConnectedGraph(10, 1);
    CHECK(g->edges.size() == 10);
    CHECK(g->destinations.size() == 90);
    CHECK(g->weights.size() == 90);

    g = generateConnectedGraph(10, den(5, 10, true), true);
    CHECK(g->edges.size() == 10);
    CHECK(g->destinations.size() == 5);
    CHECK(g->weights.size() == 5);

    g = generateConnectedGraph(10, den(5, 10, false), false);
    CHECK(g->edges.size() == 10);
    CHECK(g->destinations.size() == 5);
    CHECK(g->weights.size() == 5);

    g = generateConnectedGraph(1000, den(700, 1000, true), true);
    CHECK(g->edges.size() == 1000);
    CHECK(g->destinations.size() == 700);
    CHECK(g->weights.size() == 700);

    g = generateConnectedGraph(1000, den(300, 1000, false), false);
    CHECK(g->edges.size() == 1000);
    CHECK(g->destinations.size() == 300);
    CHECK(g->weights.size() == 300);
}

TEST_CASE("Test graph IO") {
    SECTION("Small normal graph") {
        std::vector<uint> edges {0, 2, 5, 6, 7};
        std::vector<uint> destinations {1, 2, 0, 3, 4, 1, 4, 2};
        std::vector<uint> weights {6, 1, 5, 3, 8, 2, 4, 1};
        Graph graph = Graph(edges, destinations, weights);

        // First read a graph from disk
        auto graph2 = readGraph("test/res/graph_small_normal.txt"); // This file describes the same graph as 'graph' and 'ss'
        REQUIRE(graph2->edges == graph.edges);
        REQUIRE(graph2->destinations == graph.destinations);
        REQUIRE(graph2->weights == graph.weights);

        writeGraph("test/res/test_temp", graph2);
        graph2 = readGraph("test/res/test_temp");
        REQUIRE(graph2->edges == graph.edges);
        REQUIRE(graph2->destinations == graph.destinations);
        REQUIRE(graph2->weights == graph.weights);
    }
}