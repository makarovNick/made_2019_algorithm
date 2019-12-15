#include "graph.h"
#include "box_muller_generator.h"


#include <iostream>

#define print(a) std::cout<<a<<std::endl;
#define PRINTLINE() print("-------------------------------------------------")


class Experiment
{
public:
    void makeExperiment()
    {
        PRINTLINE();
        std::vector<std::pair<double, double>> results;
        for (size_t i = 2; i <= 10; i++)
        {
            std::cout<<"TEST WITH "<<i<<" POINTS"<<std::endl;

            results.push_back(getResults(i));

            int mean = 0;
            for (auto& data : results)
            {
                mean += data.first - data.second;
            }

            mean /= results.size(); 

            double std_ = 0;
            for (auto& data : results)
            {
                int cur = pow(mean - (data.first - data.second), 2);
                std_ += cur;
            }

            if (mean != 0)
            {
                std_ = std::sqrt(std_ /= mean);
            }
            else
            {
                std_ = 0;
            }

            std::cout << "Mean: " << mean << std::endl<<"Std : " << std_ << std::endl;
            PRINTLINE();
        }
    }
private:
    std::pair<double, double> getResults(size_t __N)
    {
        Graph graph(__N);

        for (size_t i = 0; i < graph.getPointsCount(); i++)
        {
            for (size_t j = 0; j < graph.getPointsCount() - 1; j++)
            {
                if(i!=j)
                {
                    double w = boxMueller(100);
                    graph.addEdge(i, j, w );
                }
            }   
        }

        return { graph.exactSolution(), graph.MSTSolution() }; 
    }
};