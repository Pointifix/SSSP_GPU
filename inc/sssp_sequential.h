#ifndef SSSP_Sequential_H
#define SSSP_Sequential_H

#include "sssp.h"
#include <algorithm>
#include <iostream>

class SSSP_Sequential : public SSSP {
public:
    /**
     * SSSP instance for sequential computation the CPU.
     * @param graph graph that is used for computations.
     */
    explicit SSSP_Sequential(std::shared_ptr<Graph> graph);
    std::shared_ptr<Paths> compute(int source_node) override;
};

#endif /* SSSP_Sequential_H */
