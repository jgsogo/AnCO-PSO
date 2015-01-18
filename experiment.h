
#pragma once

#include "jgsogo/AnCO/colony/colony.h"
#include "jgsogo/AnCO/algorithm/aco_base.h"
#include "jgsogo/AnCO/algorithm/prox_base.h"
#include "jgsogo/AnCO/colony/neighbourhood.h"
#include "jgsogo/AnCO/log_time.h"

namespace AnCO {

    template <class aco_algorithm = algorithm::aco_base, class prox_algorithm = algorithm::prox_base>
    class experiment {
        typedef AnCO::colony<aco_algorithm> colony_type;
        typedef AnCO::neighbourhood<aco_algorithm, prox_algorithm> neighbourhood_type;

        public:
            experiment(unsigned int n_colonies,
                unsigned int n_ants,
                unsigned int max_steps) : 
                _n_colonies(n_colonies), 
                _n_ants(n_ants), 
                _max_steps(max_steps)
                {
                _time = 0.f;
                aco_alpha = aco_algorithm::alpha;
                aco_beta = aco_algorithm::beta;
                aco_pheromone_change_factor = aco_algorithm::pheromone_change_factor;
                aco_pheromone_evaporation = aco_algorithm::pheromone_evaporation;

                pheromone_sensitivity = prox_algorithm::pheromone_sensitivity;
                proximity_decay = prox_algorithm::proximity_decay;
                gamma = prox_algorithm::gamma;
                delta = prox_algorithm::delta;
                }

            float run(graph& graph, unsigned int max_iterations, float metric_threshold) {
                log_time t;

                neighbourhood_type neighbourhood(graph, _n_colonies, _n_ants, _max_steps);

                _iterations = 0;
                //float _improvement = std::numeric_limits<float>::max();
                while ((_iterations < max_iterations)/* && (_improvement > metric_threshold)*/) {
                    neighbourhood.run();
                    neighbourhood.update();
                    colony_type::aco_algorithm_impl::update_graph(graph);

                    auto metric = neighbourhood.get_metric();                   
                    _metrics.push_back(metric);
                    //_improvement = metric - _improvement;
                    ++_iterations;
                    }

                _time = t.toc();
                }

        public:
            float _time;
            unsigned int _n_colonies, _n_ants, _max_steps;
            unsigned int _iterations;
            std::vector<float> _metrics;
            // ACO
            float aco_alpha;
            float aco_beta;
            float aco_pheromone_change_factor;
            float aco_pheromone_evaporation;

            // Proximity
            float pheromone_sensitivity;
            float proximity_decay;
            float gamma;
            float delta;
        };

    }

    