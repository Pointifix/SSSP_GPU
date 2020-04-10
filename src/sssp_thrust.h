#pragma once

#include "sssp.h"

class SSSP_Thrust : public SSSP {
public:
    /**
     * Instance of a SSSP for computation using thrust.
     * @param graph graph that is used for computations.
     */
    explicit SSSP_Thrust(std::shared_ptr<Graph> graph);
    std::vector<std::shared_ptr<Path>> compute(int source_node) override;
};
