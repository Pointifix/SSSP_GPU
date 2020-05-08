#ifndef GRAPH_IO_H
#define GRAPH_IO_H

#include <memory>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "graph.h"

namespace graphio
{
    const std::string file_extension = ".txt";

    /**
     * Saves the given graph into the given file.
     * @param filename Name of the file.
     * @param graph Graph.
     */
    void writeGraph(const std::string& filename, const std::shared_ptr<Graph>& graph)
    {
        std::vector<uint> sources(graph->edges.size());
        std::vector<uint> targets(graph->destinations.size());

        std::ofstream stream;
        stream.open (filename + file_extension);
        stream << "H " + std::to_string(graph->edges.size()) + " " + std::to_string(graph->destinations.size()) + " 0";

        for (size_t i = 0; i < graph->edges.size(); i++)
        {
            int current_edge_index = graph->edges.at(i);
            int next_edge_index;

            if (i == graph->edges.size() - 1) next_edge_index = graph->destinations.size();
            else next_edge_index = graph->edges.at(i + 1);

            for (size_t j = current_edge_index; j < next_edge_index; j++)
            {
                stream << "\nE " + std::to_string(i) + " " + std::to_string(graph->destinations.at(j)) + " " + std::to_string(graph->weights.at(j));
            }
        }

        stream.close();
    }

    /**
     * Reads a graph from the given file. Does not check for simple graph constraints like loops. Vertices and Edges must be sorted.
     * Last line must not end with a newline.
     * @param filename Name of the file.
     * @return a Graph.
     */
    std::shared_ptr<Graph> readGraph(const std::string& filename)
    {
        std::shared_ptr<Graph> graph;

        std::ifstream file(filename + file_extension);

        std::vector<uint> edges;
        std::vector<uint> destinations;
        std::vector<uint> weights;

        if (file.is_open())
        {
            size_t i = 0;
            std::string line;
            std::string value;
            int current_node = -1;
            int current_edge_count = 0;
            int current_target = 0;
            while (getline(file,line))
            {
                std::stringstream ss(line);
                std::vector<std::string> values;
                while (getline(ss, value, ' '))
                {
                    values.push_back(value);
                }
                if (values.size() != 4) return nullptr;

                if (i == 0 && !values.at(0).compare("H"))
                {
                    edges = std::vector<uint>(std::stoi(values.at(1)));
                    destinations = std::vector<uint>(std::stoi(values.at(2)));
                    weights = std::vector<uint>(std::stoi(values.at(2)));
                }
                else if (!values.at(0).compare("E"))
                {
                    int source = std::stoi(values.at(1));
                    int target = std::stoi(values.at(2));
                    int weight = std::stoi(values.at(3));

                    if (source == current_node && target > current_target)
                    {
                        destinations.at(current_edge_count) = target;
                        weights.at(current_edge_count) = weight;
                    }
                    else if (source > current_node)
                    {
                        for (size_t i = current_node + 1; i < source; i++)
                        {
                            edges.at(i) = current_edge_count;
                        }

                        current_node = source;
                        edges.at(current_node) = current_edge_count;
                        destinations.at(current_edge_count) = target;
                        weights.at(current_edge_count) = weight;
                    }
                    else return nullptr;

                    current_target = target;
                    current_edge_count++;
                }
                else return nullptr;

                i++;
            }

            current_node++;
            while (current_node < edges.size())
            {
                edges.at(current_node) = current_edge_count;
                current_node++;
            }

            file.close();

            return std::make_shared<Graph>(edges, destinations, weights);
        }

        return nullptr;
    }

    /**
     * Saves the given paths into the given folder
     * @param filename Name of the file
     * @param shortest_paths shortest path vectors
     */
    void writePaths(const std::string& filename, const std::shared_ptr<Graph>& graph, const std::vector<std::vector<int>> shortest_paths)
    {
        std::ofstream stream;

        for(int i = 0; i < shortest_paths.size(); i++)
        {
            stream.open(filename + std::to_string(shortest_paths[i][shortest_paths[i].size()-1]) + file_extension);
            stream << "H " + std::to_string(shortest_paths[i].size()) + " " + std::to_string(shortest_paths[i].size() - 1) + " 0";

            for(int j = 0; j < shortest_paths[i].size() - 1; j++)
            {
                int lastEdgeIndex = graph->destinations.size();
                if (shortest_paths[i][j] + 1 < graph->edges.size()) lastEdgeIndex = graph->edges.at(shortest_paths[i][j] + 1);

                for(int k = graph->edges.at(shortest_paths[i][j]); k < lastEdgeIndex; k++)
                {
                    if (graph->destinations.at(k) == shortest_paths[i][j + 1]) stream << "\nE " + std::to_string(shortest_paths[i][j]) + " " + std::to_string(shortest_paths[i][j + 1]) + " " + std::to_string(graph->weights.at(k));
                }
            }

            stream.close();
            stream.clear();
        }

    }
}

#endif /* GRAPH_IO_H */