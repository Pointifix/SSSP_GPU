#include "sssp_thrust.h"

SSSP_Thrust::SSSP_Thrust(std::shared_ptr<Graph> graph) : SSSP(std::move(graph)) {
}

std::vector<std::shared_ptr<Path>> SSSP::compute(int source_node)
{
    // TODO
}