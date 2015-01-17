

#include <iostream>
#include <iomanip>

#include "experiment.h"

#include "jgsogo/AnCO/algorithm/aco_base.h"
#include "jgsogo/AnCO/algorithm/prox_base.h"
#include "jgsogo/AnCO/graph/memgraph.h"
#include "jgsogo/AnCO/graph/graph_data_file.h"
#include "jgsogo/AnCO/log.h"
#include "jgsogo/AnCO/log_time.h"

using namespace AnCO;

int main(int argc, char* argv[]) {
    typedef experiment<algorithm::aco_base, algorithm::prox_base> _t_experiment;
    if (argc < 2) { // Check the number of parameters
        // Tell the user how to run the program
        std::cerr << "Usage: " << argv[0] << " 'DATASET_FILE'" << std::endl;
        return 1;
        }

    std::cout << "======" << std::endl;
    std::cout << "PSO for AnCO\n";
    std::cout << "======" << std::endl << std::endl;

    std::cout << "1) Graph dataset from file" << std::endl;
    graph_data_file dataset(argv[1]);
    log_time t;
    dataset.load_file();
    t.toc();    

    std::cout << "2) Make graph available on memory" << std::endl;
    t.tic();
    AnCO::memgraph mygraph(dataset);
    t.toc();

    // Parameters
    unsigned int max_iterations = 10;
    // Parameters for ACO_base
    std::set<float> alpha = {1.f};
    std::set<float> beta = {2.5f};
    std::set<float> pheromone_change_factor = {1.f};
    std::set<float> pheromone_evaporation = {0.5f};
    
    std::cout << "3) Running XX experiments" << std::endl;
    std::vector<std::shared_ptr<_t_experiment>> experiments;
    
    std::size_t exp_order = 0;
    for (int i =2; i<=10; i++) {
        std::cout << "\tExperiment " << exp_order++ << std::endl;
        // Simula
        std::shared_ptr<_t_experiment> ptr = std::make_shared<_t_experiment>(i, 10, 100);
        experiments.push_back(ptr);
        ptr->run(mygraph, max_iterations, 0.f);

        // Limpio el grafo para la siguiente (podría utilizar otras posiciones de feromona)
        auto edges = mygraph.get_edges_outgoing();
        std::for_each(edges.begin(), edges.end(), [](std::pair<memgraph::_t_node_id, std::vector<edge_ptr>> item){
            std::for_each(item.second.begin(), item.second.end(), [](edge_ptr ptr){
                for (std::size_t jj=0; jj<edge_data::pheromone_size;  ++jj) {
                    ptr->data.pheromone[jj] = 0.f;
                    }
                });
            });
        }

    std::cout << "4) Print stats" << std::endl;
    std::for_each(experiments.begin(), experiments.end(), [=](std::shared_ptr<_t_experiment> ptr){
        std::cout << "cols\tmetric\ttime\t" << std::endl;
        std::cout << ptr->_n_colonies << "\t";
        std::cout << std::fixed << std::setw(6) << std::setprecision(3) << std::setfill(' ') << *(ptr->_metrics.rbegin()) << "\t";
        std::cout << ptr->_time << std::endl;
        });

    }
